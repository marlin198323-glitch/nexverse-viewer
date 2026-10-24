/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/modules/manifest.hpp"
#include "nexverse/modules/abi_v2.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <map>
#include <stdexcept>

namespace nexverse::modules
{
namespace
{
struct Json
{
    enum class Kind { null, string, integer, boolean, array, object } kind = Kind::null;
    std::string text;
    std::vector<Json> items;
    std::map<std::string, Json, std::less<>> fields;
};

class Parser
{
public:
    explicit Parser(std::string_view input) : input_(input) {}
    Json run()
    {
        auto result = value(0);
        whitespace();
        if (pos_ != input_.size()) fail("Trailing JSON data");
        return result;
    }
private:
    std::string_view input_;
    std::size_t pos_ = 0;
    [[noreturn]] void fail(const char* why) const { throw std::runtime_error(why); }
    void whitespace()
    {
        while (pos_ < input_.size() &&
               (input_[pos_] == ' ' || input_[pos_] == '\n' ||
                input_[pos_] == '\r' || input_[pos_] == '\t')) ++pos_;
    }
    char take()
    {
        if (pos_ == input_.size()) fail("Unexpected end of JSON");
        return input_[pos_++];
    }
    bool consume(char c)
    {
        whitespace();
        if (pos_ < input_.size() && input_[pos_] == c) { ++pos_; return true; }
        return false;
    }
    void expect(char c)
    {
        whitespace();
        if (take() != c) fail("Unexpected JSON token");
    }
    static void utf8(std::string& s, unsigned cp)
    {
        if (cp <= 0x7FU) s.push_back(static_cast<char>(cp));
        else if (cp <= 0x7FFU)
        {
            s.push_back(static_cast<char>(0xC0U | (cp >> 6U)));
            s.push_back(static_cast<char>(0x80U | (cp & 63U)));
        }
        else if (cp <= 0xFFFFU)
        {
            s.push_back(static_cast<char>(0xE0U | (cp >> 12U)));
            s.push_back(static_cast<char>(0x80U | ((cp >> 6U) & 63U)));
            s.push_back(static_cast<char>(0x80U | (cp & 63U)));
        }
        else
        {
            s.push_back(static_cast<char>(0xF0U | (cp >> 18U)));
            s.push_back(static_cast<char>(0x80U | ((cp >> 12U) & 63U)));
            s.push_back(static_cast<char>(0x80U | ((cp >> 6U) & 63U)));
            s.push_back(static_cast<char>(0x80U | (cp & 63U)));
        }
    }
    unsigned hex4()
    {
        unsigned cp = 0;
        for (int i = 0; i < 4; ++i)
        {
            const char c = take();
            cp <<= 4U;
            if (c >= '0' && c <= '9') cp |= static_cast<unsigned>(c - '0');
            else if (c >= 'a' && c <= 'f') cp |= static_cast<unsigned>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') cp |= static_cast<unsigned>(c - 'A' + 10);
            else fail("Invalid Unicode escape");
        }
        return cp;
    }
    std::string str()
    {
        expect('"');
        std::string out;
        while (true)
        {
            char c = take();
            if (c == '"') return out;
            if (static_cast<unsigned char>(c) < 0x20U) fail("Control character in string");
            if (c != '\\') { out.push_back(c); continue; }
            c = take();
            switch (c)
            {
            case '"': case '\\': case '/': out.push_back(c); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case 'u':
            {
                unsigned cp = hex4();
                if (cp >= 0xD800U && cp <= 0xDBFFU)
                {
                    if (take() != '\\' || take() != 'u') fail("Missing low surrogate");
                    const unsigned low = hex4();
                    if (low < 0xDC00U || low > 0xDFFFU) fail("Invalid low surrogate");
                    cp = 0x10000U + ((cp - 0xD800U) << 10U) + (low - 0xDC00U);
                }
                else if (cp >= 0xDC00U && cp <= 0xDFFFU) fail("Unpaired surrogate");
                utf8(out, cp);
                break;
            }
            default: fail("Unknown string escape");
            }
        }
    }
    Json value(int depth)
    {
        if (depth > 12) fail("JSON nesting depth exceeded");
        whitespace();
        if (pos_ == input_.size()) fail("Empty JSON value");
        Json result;
        const char c = input_[pos_];
        if (c == '"')
        {
            result.kind = Json::Kind::string;
            result.text = str();
        }
        else if (c == '[')
        {
            result.kind = Json::Kind::array;
            ++pos_;
            if (!consume(']'))
            {
                do { result.items.push_back(value(depth + 1)); }
                while (consume(','));
                expect(']');
            }
        }
        else if (c == '{')
        {
            result.kind = Json::Kind::object;
            ++pos_;
            if (!consume('}'))
            {
                do
                {
                    whitespace();
                    const auto key = str();
                    expect(':');
                    if (!result.fields.emplace(key, value(depth + 1)).second)
                        fail("Duplicate JSON key");
                } while (consume(','));
                expect('}');
            }
        }
        else if (c >= '0' && c <= '9')
        {
            result.kind = Json::Kind::integer;
            const auto start = pos_;
            while (pos_ < input_.size() &&
                   input_[pos_] >= '0' && input_[pos_] <= '9') ++pos_;
            result.text = std::string(input_.substr(start, pos_ - start));
        }
        else
        {
            const auto rest = input_.substr(pos_);
            if (rest.starts_with("null")) { pos_ += 4; }
            else if (rest.starts_with("true")) { pos_ += 4; result.kind = Json::Kind::boolean; result.text = "true"; }
            else if (rest.starts_with("false")) { pos_ += 5; result.kind = Json::Kind::boolean; result.text = "false"; }
            else fail("Unsupported JSON value");
        }
        return result;
    }
};

const Json& required(const Json& root, std::string_view key, Json::Kind kind)
{
    const auto it = root.fields.find(key);
    if (it == root.fields.end() || it->second.kind != kind)
        throw std::runtime_error("Missing or invalid manifest field: " + std::string(key));
    return it->second;
}
std::string field(const Json& root, std::string_view key)
{
    const auto& f = required(root, key, Json::Kind::string);
    if (f.text.empty() || f.text.size() > 512) throw std::runtime_error("Invalid manifest string length");
    return f.text;
}
std::vector<std::string> array_field(const Json& root, std::string_view key)
{
    std::vector<std::string> out;
    for (const auto& v : required(root, key, Json::Kind::array).items)
    {
        if (v.kind != Json::Kind::string || v.text.empty() || v.text.size() > 128)
            throw std::runtime_error("Invalid manifest array item");
        if (std::find(out.begin(), out.end(), v.text) != out.end())
            throw std::runtime_error("Duplicate manifest array item");
        out.push_back(v.text);
    }
    if (out.size() > 32) throw std::runtime_error("Manifest array too long");
    return out;
}
bool valid_id(std::string_view id)
{
    if (id.empty() || id.size() > 128 || id.front() < 'a' || id.front() > 'z')
        return false;
    if (id.back() == '.' || id.back() == '-') return false;
    for (const char c : id)
        if (!(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9') &&
            c != '.' && c != '-') return false;
    return id.find("..") == std::string_view::npos && id.find("--") == std::string_view::npos;
}
struct Version
{
    unsigned major = 0, minor = 0, patch = 0, revision = 0, alpha = 0;
    bool prerelease = false;
};
std::optional<Version> parse_version(std::string_view s)
{
    Version out;
    auto number = [&s](unsigned& n) -> bool
    {
        if (s.empty() || s.front() < '0' || s.front() > '9') return false;
        const auto end = s.find_first_not_of("0123456789");
        const auto part = s.substr(0, end);
        if (part.size() > 1 && part.front() == '0') return false;
        const auto [ptr, ec] = std::from_chars(part.data(), part.data() + part.size(), n);
        if (ec != std::errc{} || ptr != part.data() + part.size()) return false;
        s.remove_prefix(part.size());
        return true;
    };
    // Backwards compatible with historical unsigned module manifests
    // 0.2.0-alpha.N and 0.1.0, but current Viewer uses vX.Y.Z.W.
    if (s.starts_with("v")) s.remove_prefix(1);
    if (!number(out.major) || !s.starts_with(".")) return {};
    s.remove_prefix(1);
    if (!number(out.minor) || !s.starts_with(".")) return {};
    s.remove_prefix(1);
    if (!number(out.patch)) return {};
    if (s.empty()) return out;
    if (s.starts_with("."))
    {
        s.remove_prefix(1);
        if (!number(out.revision) || !s.empty()) return {};
        return out;
    }
    if (!s.starts_with("-alpha.")) return {};
    s.remove_prefix(7);
    if (!number(out.alpha) || !s.empty()) return {};
    out.prerelease = true;
    return out;
}
}

bool version_at_least(std::string_view actual, std::string_view minimum)
{
    const auto a = parse_version(actual), b = parse_version(minimum);
    if (!a || !b) return false;
    if (a->major != b->major) return a->major > b->major;
    if (a->minor != b->minor) return a->minor > b->minor;
    if (a->patch != b->patch) return a->patch > b->patch;
    if (a->prerelease != b->prerelease) return !a->prerelease;
    if (a->prerelease) return a->alpha >= b->alpha;
    return a->revision >= b->revision;
}
ManifestResult parse_manifest(std::string_view json)
{
    try
    {
        if (json.size() > 65536U) throw std::runtime_error("Manifest exceeds 64 KiB");
        const Json root = Parser{json}.run();
        if (root.kind != Json::Kind::object) throw std::runtime_error("Manifest must be a JSON object");
        Manifest m;
        m.id = field(root, "id");
        m.name = field(root, "name");
        m.version = field(root, "version");
        m.vendor = field(root, "vendor");
        const auto& api = required(root, "api_version", Json::Kind::integer);
        if (api.text != "1" && api.text != "2")
            throw std::runtime_error("Unsupported API version");
        m.api_version = api.text == "1" ? OGL_MODULE_ABI_VERSION : OGL_MODULE_ABI_VERSION_2;
        m.minimum_viewer_version = field(root, "minimum_viewer_version");
        m.type = field(root, "type");
        m.platforms = array_field(root, "platforms");
        m.architectures = array_field(root, "architectures");
        m.capabilities = array_field(root, "capabilities");
        m.entrypoint = field(root, "entrypoint");
        const auto& deps = required(root, "dependencies", Json::Kind::array);
        if (deps.items.size() > 32) throw std::runtime_error("Too many module dependencies");
        for (const auto& d : deps.items)
        {
            if (d.kind != Json::Kind::object) throw std::runtime_error("Invalid module dependency");
            Dependency dep{field(d, "id"), field(d, "min_version")};
            if (!valid_id(dep.id) || !parse_version(dep.min_version))
                throw std::runtime_error("Invalid module dependency ID/version");
            m.dependencies.push_back(std::move(dep));
        }
        auto optional_field = [&](std::string_view key) -> std::string
        {
            const auto it = root.fields.find(key);
            if (it == root.fields.end() || it->second.kind == Json::Kind::null) return {};
            return field(root, key);
        };
        m.package_hash = optional_field("package_hash");
        m.signature = optional_field("signature");
        const auto ep = std::filesystem::path(m.entrypoint);
        if (!valid_id(m.id) || !parse_version(m.version) ||
            !parse_version(m.minimum_viewer_version) ||
            ep.is_absolute() || ep.has_parent_path() ||
            m.entrypoint == "." || m.entrypoint == ".." ||
            m.entrypoint.find('\\') != std::string::npos ||
            m.entrypoint.find(':') != std::string::npos)
            throw std::runtime_error("Invalid manifest ID, version or entrypoint");
        static constexpr std::string_view known[] = {
            "ui", "audio", "voice", "network", "world-read", "world-edit",
            "inventory", "avatar", "animation", "media", "renderer",
            "gpu", "filesystem-scoped", "developer"
        };
        for (const auto& cap : m.capabilities)
            if (std::find(std::begin(known), std::end(known), cap) == std::end(known))
                throw std::runtime_error("Unknown module capability: " + cap);
        return {std::move(m), {}};
    }
    catch (const std::exception& e) { return {std::nullopt, e.what()}; }
}
std::string host_platform()
{
#if defined(_WIN32)
    return "windows";
#elif defined(__APPLE__)
    return "macos";
#elif defined(__linux__)
    return "linux";
#else
    return "unknown";
#endif
}
std::string host_architecture()
{
#if defined(__x86_64__) || defined(_M_X64)
    return "x64";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "arm64";
#else
    return "unknown";
#endif
}
}
