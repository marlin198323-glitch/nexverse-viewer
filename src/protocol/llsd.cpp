/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/llsd.hpp"

#include <tinyxml2.h>
#include <openssl/evp.h>

#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <sstream>
#include <utility>

namespace
{
using nexverse::protocol::LlsdParseResult;
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

std::string text_of(const XMLElement* element)
{
    if (element == nullptr || element->GetText() == nullptr) return {};
    return std::string{element->GetText()};
}

std::string base64_encode(std::span<const std::uint8_t> bytes)
{
    if (bytes.empty()) return {};

    std::string output;
    output.resize(4U * ((bytes.size() + 2U) / 3U));
    const int written = EVP_EncodeBlock(
        reinterpret_cast<unsigned char*>(output.data()),
        bytes.data(),
        static_cast<int>(bytes.size()));
    if (written < 0) return {};
    output.resize(static_cast<std::size_t>(written));
    return output;
}

std::optional<std::vector<std::uint8_t>> base64_decode(std::string_view value)
{
    std::string compact;
    compact.reserve(value.size());
    for (const char ch : value)
    {
        if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n')
        {
            compact.push_back(ch);
        }
    }

    if (compact.empty()) return std::vector<std::uint8_t>{};
    if (compact.size() % 4U != 0U) return std::nullopt;

    std::vector<std::uint8_t> decoded(3U * compact.size() / 4U);
    const int written = EVP_DecodeBlock(
        decoded.data(),
        reinterpret_cast<const unsigned char*>(compact.data()),
        static_cast<int>(compact.size()));
    if (written < 0) return std::nullopt;

    std::size_t size = static_cast<std::size_t>(written);
    if (!compact.empty() && compact.back() == '=') --size;
    if (compact.size() >= 2U && compact[compact.size() - 2U] == '=') --size;
    decoded.resize(size);
    return decoded;
}

void append_xml_value(XMLDocument& document, XMLElement* parent, const LlsdValue& value)
{
    XMLElement* element = nullptr;

    switch (value.type)
    {
        case LlsdType::undefined:
            element = document.NewElement("undef");
            break;
        case LlsdType::boolean:
            element = document.NewElement("boolean");
            element->SetText(value.boolean_value ? "true" : "false");
            break;
        case LlsdType::integer:
            element = document.NewElement("integer");
            element->SetText(value.integer_value);
            break;
        case LlsdType::real:
            element = document.NewElement("real");
            element->SetText(value.real_value);
            break;
        case LlsdType::string:
            element = document.NewElement("string");
            element->SetText(value.string_value.c_str());
            break;
        case LlsdType::uuid:
            element = document.NewElement("uuid");
            element->SetText(value.string_value.c_str());
            break;
        case LlsdType::uri:
            element = document.NewElement("uri");
            element->SetText(value.string_value.c_str());
            break;
        case LlsdType::date:
            element = document.NewElement("date");
            element->SetText(value.string_value.c_str());
            break;
        case LlsdType::binary:
        {
            element = document.NewElement("binary");
            element->SetAttribute("encoding", "base64");
            const std::string encoded = base64_encode(value.binary_value);
            element->SetText(encoded.c_str());
            break;
        }
        case LlsdType::array:
            element = document.NewElement("array");
            for (const auto& item : value.array_value)
            {
                append_xml_value(document, element, item);
            }
            break;
        case LlsdType::map:
            element = document.NewElement("map");
            for (const auto& [key, item] : value.map_value)
            {
                auto* key_element = document.NewElement("key");
                key_element->SetText(key.c_str());
                element->InsertEndChild(key_element);
                append_xml_value(document, element, item);
            }
            break;
    }

    parent->InsertEndChild(element);
}

LlsdParseResult parse_xml_value(const XMLElement* element)
{
    if (element == nullptr)
    {
        return {std::nullopt, "LLSD XML value element is missing"};
    }

    const std::string_view name{element->Name()};

    if (name == "undef")
    {
        return {LlsdValue{}, {}};
    }
    if (name == "boolean")
    {
        const std::string value = text_of(element);
        return {LlsdValue::boolean(value == "true" || value == "1"), {}};
    }
    if (name == "integer")
    {
        const std::string raw = text_of(element);
        const std::size_t first = raw.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
        {
            // LLSD XML allows an empty integer to express its default 0.
            return {LlsdValue::integer(0), {}};
        }
        const std::size_t last = raw.find_last_not_of(" \t\r\n");
        std::string_view value{raw.data() + first, last - first + 1U};
        if (!value.empty() && value.front() == '+') value.remove_prefix(1U);
        std::int64_t result = 0;
        const auto parsed = std::from_chars(
            value.data(), value.data() + value.size(), result);
        if (parsed.ec != std::errc{} ||
            parsed.ptr != value.data() + value.size())
        {
            return {std::nullopt,
                    "LLSD integer is invalid or outside signed 64-bit range"};
        }
        return {LlsdValue::integer(result), {}};
    }
    if (name == "real")
    {
        const std::string raw = text_of(element);
        if (raw.empty())
        {
            return {LlsdValue::real(0.0), {}};
        }

        std::size_t first = 0U;
        while (first < raw.size() &&
               std::isspace(static_cast<unsigned char>(raw[first])) != 0)
        {
            ++first;
        }

        std::size_t last = raw.size();
        while (last > first &&
               std::isspace(static_cast<unsigned char>(raw[last - 1U])) != 0)
        {
            --last;
        }

        if (first == last)
        {
            return {LlsdValue::real(0.0), {}};
        }

        const std::string_view value{raw.data() + first, last - first};
        double result = 0.0;
        const auto parsed = std::from_chars(
            value.data(),
            value.data() + value.size(),
            result,
            std::chars_format::general);

        if (parsed.ec != std::errc{} ||
            parsed.ptr != value.data() + value.size())
        {
            const std::string shown{value.substr(0U, 64U)};
            return {
                std::nullopt,
                "LLSD real is invalid: '" + shown + "'"};
        }

        return {LlsdValue::real(result), {}};
    }
    if (name == "string")
    {
        return {LlsdValue::string(text_of(element)), {}};
    }
    if (name == "uuid")
    {
        return {LlsdValue::uuid(text_of(element)), {}};
    }
    if (name == "uri")
    {
        return {LlsdValue::uri(text_of(element)), {}};
    }
    if (name == "date")
    {
        // An LLSD date is metadata, not an instruction. Preserve its
        // lexical UTC representation exactly (including fractions).
        const std::string value = text_of(element);
        if (value.size() > 128U)
            return {std::nullopt, "LLSD date exceeds size limit"};
        return {LlsdValue::date(value), {}};
    }
    if (name == "binary")
    {
        const auto decoded = base64_decode(text_of(element));
        if (!decoded.has_value())
        {
            return {std::nullopt, "LLSD binary base64 is invalid"};
        }
        return {LlsdValue::binary(*decoded), {}};
    }
    if (name == "array")
    {
        std::vector<LlsdValue> values;
        for (const XMLElement* child = element->FirstChildElement();
             child != nullptr;
             child = child->NextSiblingElement())
        {
            auto parsed = parse_xml_value(child);
            if (!parsed.ok()) return parsed;
            values.push_back(std::move(*parsed.value));
        }
        return {LlsdValue::array(std::move(values)), {}};
    }
    if (name == "map")
    {
        std::map<std::string, LlsdValue, std::less<>> values;
        const XMLElement* child = element->FirstChildElement();

        while (child != nullptr)
        {
            if (std::string_view{child->Name()} != "key")
            {
                return {std::nullopt, "LLSD map expected key element"};
            }

            const std::string key = text_of(child);
            child = child->NextSiblingElement();
            if (child == nullptr)
            {
                return {std::nullopt, "LLSD map key has no value"};
            }

            auto parsed = parse_xml_value(child);
            if (!parsed.ok()) return parsed;
            values[key] = std::move(*parsed.value);
            child = child->NextSiblingElement();
        }

        return {LlsdValue::map(std::move(values)), {}};
    }

    return {std::nullopt, "Unsupported LLSD XML type: " + std::string{name}};
}

void append_be_u32(std::vector<std::uint8_t>& out, std::uint32_t value)
{
    out.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
}

std::optional<std::uint32_t> read_be_u32(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset)
{
    if (offset + 4U > bytes.size()) return std::nullopt;
    const std::uint32_t value =
        (static_cast<std::uint32_t>(bytes[offset]) << 24U) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 8U) |
        static_cast<std::uint32_t>(bytes[offset + 3U]);
    offset += 4U;
    return value;
}

void append_be_double(std::vector<std::uint8_t>& out, double value)
{
    const std::uint64_t bits = std::bit_cast<std::uint64_t>(value);
    for (int shift = 56; shift >= 0; shift -= 8)
    {
        out.push_back(static_cast<std::uint8_t>((bits >> shift) & 0xFFU));
    }
}

std::optional<double> read_be_double(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset)
{
    if (offset + 8U > bytes.size()) return std::nullopt;

    std::uint64_t bits = 0U;
    for (int index = 0; index < 8; ++index)
    {
        bits = (bits << 8U) | bytes[offset + static_cast<std::size_t>(index)];
    }
    offset += 8U;
    return std::bit_cast<double>(bits);
}

std::optional<std::array<std::uint8_t, 16>> parse_uuid(std::string_view uuid)
{
    if (uuid.size() != 36U) return std::nullopt;

    std::string compact;
    compact.reserve(32U);
    for (const char ch : uuid)
    {
        if (ch != '-') compact.push_back(ch);
    }
    if (compact.size() != 32U) return std::nullopt;

    std::array<std::uint8_t, 16> bytes{};
    for (std::size_t index = 0; index < 16U; ++index)
    {
        unsigned int byte = 0U;
        const auto part = std::string_view{compact}.substr(index * 2U, 2U);
        const auto parsed = std::from_chars(
            part.data(),
            part.data() + part.size(),
            byte,
            16);
        if (parsed.ec != std::errc{} || byte > 0xFFU) return std::nullopt;
        bytes[index] = static_cast<std::uint8_t>(byte);
    }
    return bytes;
}

std::string format_uuid(std::span<const std::uint8_t> bytes)
{
    if (bytes.size() < 16U) return {};

    static constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(36U);

    for (std::size_t index = 0; index < 16U; ++index)
    {
        if (index == 4U || index == 6U || index == 8U || index == 10U)
        {
            result.push_back('-');
        }
        const std::uint8_t byte = bytes[index];
        result.push_back(hex[(byte >> 4U) & 0x0FU]);
        result.push_back(hex[byte & 0x0FU]);
    }
    return result;
}

void append_binary_value(std::vector<std::uint8_t>& out, const LlsdValue& value)
{
    switch (value.type)
    {
        case LlsdType::undefined:
            out.push_back(static_cast<std::uint8_t>('!'));
            break;
        case LlsdType::boolean:
            out.push_back(static_cast<std::uint8_t>(value.boolean_value ? '1' : '0'));
            break;
        case LlsdType::integer:
            out.push_back(static_cast<std::uint8_t>('i'));
            append_be_u32(out, static_cast<std::uint32_t>(value.integer_value));
            break;
        case LlsdType::real:
            out.push_back(static_cast<std::uint8_t>('r'));
            append_be_double(out, value.real_value);
            break;
        case LlsdType::uuid:
        {
            out.push_back(static_cast<std::uint8_t>('u'));
            const auto uuid = parse_uuid(value.string_value);
            if (uuid.has_value())
            {
                out.insert(out.end(), uuid->begin(), uuid->end());
            }
            else
            {
                out.insert(out.end(), 16U, 0U);
            }
            break;
        }
        case LlsdType::date:
            // Binary LLSD dates are big-endian IEEE754 seconds since epoch.
            out.push_back(static_cast<std::uint8_t>('d'));
            append_be_double(out, value.real_value);
            break;
        case LlsdType::string:
        case LlsdType::uri:
        {
            out.push_back(static_cast<std::uint8_t>(
                value.type == LlsdType::uri ? 'l' : 's'));
            append_be_u32(out, static_cast<std::uint32_t>(value.string_value.size()));
            out.insert(out.end(), value.string_value.begin(), value.string_value.end());
            break;
        }
        case LlsdType::binary:
            out.push_back(static_cast<std::uint8_t>('b'));
            append_be_u32(out, static_cast<std::uint32_t>(value.binary_value.size()));
            out.insert(out.end(), value.binary_value.begin(), value.binary_value.end());
            break;
        case LlsdType::array:
            out.push_back(static_cast<std::uint8_t>('['));
            append_be_u32(out, static_cast<std::uint32_t>(value.array_value.size()));
            for (const auto& item : value.array_value)
            {
                append_binary_value(out, item);
            }
            out.push_back(static_cast<std::uint8_t>(']'));
            break;
        case LlsdType::map:
            out.push_back(static_cast<std::uint8_t>('{'));
            append_be_u32(out, static_cast<std::uint32_t>(value.map_value.size()));
            for (const auto& [key, item] : value.map_value)
            {
                out.push_back(static_cast<std::uint8_t>('k'));
                append_be_u32(out, static_cast<std::uint32_t>(key.size()));
                out.insert(out.end(), key.begin(), key.end());
                append_binary_value(out, item);
            }
            out.push_back(static_cast<std::uint8_t>('}'));
            break;
    }
}

LlsdParseResult parse_binary_value(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    int depth)
{
    if (depth > 64)
    {
        return {std::nullopt, "LLSD binary nesting exceeds limit"};
    }
    if (offset >= bytes.size())
    {
        return {std::nullopt, "LLSD binary ended unexpectedly"};
    }

    const std::uint8_t marker = bytes[offset++];

    switch (marker)
    {
        case '!':
            return {LlsdValue{}, {}};
        case '1':
            return {LlsdValue::boolean(true), {}};
        case '0':
            return {LlsdValue::boolean(false), {}};
        case 'i':
        {
            const auto value = read_be_u32(bytes, offset);
            if (!value.has_value()) return {std::nullopt, "LLSD binary integer truncated"};
            return {LlsdValue::integer(static_cast<std::int32_t>(*value)), {}};
        }
        case 'r':
        {
            const auto value = read_be_double(bytes, offset);
            if (!value.has_value()) return {std::nullopt, "LLSD binary real truncated"};
            return {LlsdValue::real(*value), {}};
        }
        case 'd':
        {
            const auto value = read_be_double(bytes, offset);
            if (!value.has_value() || !std::isfinite(*value))
                return {std::nullopt, "LLSD binary date invalid or truncated"};
            return {LlsdValue::date_seconds(*value), {}};
        }
        case 'u':
        {
            if (offset + 16U > bytes.size())
                return {std::nullopt, "LLSD binary UUID truncated"};
            const std::string uuid = format_uuid(bytes.subspan(offset, 16U));
            offset += 16U;
            return {LlsdValue::uuid(uuid), {}};
        }
        case 's':
        case 'l':
        case 'b':
        {
            const auto length = read_be_u32(bytes, offset);
            if (!length.has_value() || offset + *length > bytes.size())
                return {std::nullopt, "LLSD binary variable value truncated"};

            if (marker == 'b')
            {
                std::vector<std::uint8_t> value(
                    bytes.begin() + static_cast<std::ptrdiff_t>(offset),
                    bytes.begin() + static_cast<std::ptrdiff_t>(offset + *length));
                offset += *length;
                return {LlsdValue::binary(std::move(value)), {}};
            }

            std::string value(
                reinterpret_cast<const char*>(bytes.data() + offset),
                *length);
            offset += *length;
            return {
                marker == 'l'
                    ? LlsdValue::uri(std::move(value))
                    : LlsdValue::string(std::move(value)),
                {}};
        }
        case '[':
        {
            const auto count = read_be_u32(bytes, offset);
            if (!count.has_value()) return {std::nullopt, "LLSD binary array count truncated"};
            if (*count > 100000U) return {std::nullopt, "LLSD binary array count exceeds limit"};

            std::vector<LlsdValue> values;
            values.reserve(*count);
            for (std::uint32_t index = 0; index < *count; ++index)
            {
                auto parsed = parse_binary_value(bytes, offset, depth + 1);
                if (!parsed.ok()) return parsed;
                values.push_back(std::move(*parsed.value));
            }

            if (offset >= bytes.size() || bytes[offset++] != ']')
                return {std::nullopt, "LLSD binary array end marker missing"};
            return {LlsdValue::array(std::move(values)), {}};
        }
        case '{':
        {
            const auto count = read_be_u32(bytes, offset);
            if (!count.has_value()) return {std::nullopt, "LLSD binary map count truncated"};
            if (*count > 100000U) return {std::nullopt, "LLSD binary map count exceeds limit"};

            std::map<std::string, LlsdValue, std::less<>> values;
            for (std::uint32_t index = 0; index < *count; ++index)
            {
                if (offset >= bytes.size() || bytes[offset++] != 'k')
                    return {std::nullopt, "LLSD binary map key marker missing"};

                const auto key_length = read_be_u32(bytes, offset);
                if (!key_length.has_value() || offset + *key_length > bytes.size())
                    return {std::nullopt, "LLSD binary map key truncated"};

                std::string key(
                    reinterpret_cast<const char*>(bytes.data() + offset),
                    *key_length);
                offset += *key_length;

                auto parsed = parse_binary_value(bytes, offset, depth + 1);
                if (!parsed.ok()) return parsed;
                values[std::move(key)] = std::move(*parsed.value);
            }

            if (offset >= bytes.size() || bytes[offset++] != '}')
                return {std::nullopt, "LLSD binary map end marker missing"};
            return {LlsdValue::map(std::move(values)), {}};
        }
        default:
        {
            constexpr char hex[] = "0123456789abcdef";
            std::string shown{"0x"};
            shown.push_back(hex[(marker >> 4U) & 0x0FU]);
            shown.push_back(hex[marker & 0x0FU]);
            return {std::nullopt,
                "Unknown LLSD binary marker " + shown +
                " at offset " + std::to_string(offset - 1U)};
        }
    }
}
}

namespace nexverse::protocol
{
LlsdValue LlsdValue::boolean(bool value)
{
    LlsdValue result;
    result.type = LlsdType::boolean;
    result.boolean_value = value;
    return result;
}

LlsdValue LlsdValue::integer(std::int64_t value)
{
    LlsdValue result;
    result.type = LlsdType::integer;
    result.integer_value = value;
    return result;
}

LlsdValue LlsdValue::real(double value)
{
    LlsdValue result;
    result.type = LlsdType::real;
    result.real_value = value;
    return result;
}

LlsdValue LlsdValue::string(std::string value)
{
    LlsdValue result;
    result.type = LlsdType::string;
    result.string_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::uuid(std::string value)
{
    LlsdValue result;
    result.type = LlsdType::uuid;
    result.string_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::uri(std::string value)
{
    LlsdValue result;
    result.type = LlsdType::uri;
    result.string_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::date(std::string value)
{
    LlsdValue result;
    result.type = LlsdType::date;
    result.string_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::date_seconds(double value)
{
    LlsdValue result;
    result.type = LlsdType::date;
    result.real_value = value;
    return result;
}

LlsdValue LlsdValue::binary(std::vector<std::uint8_t> value)
{
    LlsdValue result;
    result.type = LlsdType::binary;
    result.binary_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::array(std::vector<LlsdValue> value)
{
    LlsdValue result;
    result.type = LlsdType::array;
    result.array_value = std::move(value);
    return result;
}

LlsdValue LlsdValue::map(
    std::map<std::string, LlsdValue, std::less<>> value)
{
    LlsdValue result;
    result.type = LlsdType::map;
    result.map_value = std::move(value);
    return result;
}

const LlsdValue* LlsdValue::find(std::string_view key) const
{
    if (type != LlsdType::map) return nullptr;
    const auto found = map_value.find(key);
    return found == map_value.end() ? nullptr : &found->second;
}

std::string_view LlsdValue::as_string() const noexcept
{
    if (type == LlsdType::string ||
        type == LlsdType::uuid ||
        type == LlsdType::uri)
    {
        return string_value;
    }
    return {};
}

bool LlsdValue::is_map() const noexcept
{
    return type == LlsdType::map;
}

bool LlsdValue::is_array() const noexcept
{
    return type == LlsdType::array;
}

std::string serialize_llsd_xml(const LlsdValue& value)
{
    XMLDocument document;
    auto* root = document.NewElement("llsd");
    document.InsertEndChild(root);
    append_xml_value(document, root, value);

    tinyxml2::XMLPrinter printer;
    document.Print(&printer);
    return std::string{printer.CStr(), printer.CStrSize() - 1U};
}

LlsdParseResult parse_llsd_xml(std::string_view xml)
{
    if (xml.empty())
    {
        return {std::nullopt, "LLSD XML is empty"};
    }
    if (xml.size() > 16U * 1024U * 1024U)
    {
        return {std::nullopt, "LLSD XML exceeds size limit"};
    }

    XMLDocument document;
    if (document.Parse(xml.data(), xml.size()) != tinyxml2::XML_SUCCESS)
    {
        return {std::nullopt, "LLSD XML is invalid"};
    }

    const XMLElement* root = document.FirstChildElement("llsd");
    if (root == nullptr)
    {
        return {std::nullopt, "LLSD XML root element is missing"};
    }

    const XMLElement* value = root->FirstChildElement();
    if (value == nullptr)
    {
        return {LlsdValue{}, {}};
    }

    return parse_xml_value(value);
}

std::vector<std::uint8_t> serialize_llsd_binary(
    const LlsdValue& value,
    bool prepend_header)
{
    std::vector<std::uint8_t> output;
    if (prepend_header)
    {
        constexpr std::string_view header{"<?llsd/binary?>\n"};
        output.insert(output.end(), header.begin(), header.end());
    }
    append_binary_value(output, value);
    return output;
}

LlsdParseResult parse_llsd_binary_prefix(
    std::span<const std::uint8_t> bytes,
    std::size_t& bytes_consumed)
{
    if (bytes.empty())
    {
        return {std::nullopt, "LLSD binary is empty"};
    }
    if (bytes.size() > 32U * 1024U * 1024U)
    {
        return {std::nullopt, "LLSD binary exceeds size limit"};
    }

    std::size_t offset = 0U;
    constexpr std::string_view header1{"<?llsd/binary?>"};
    constexpr std::string_view header2{"<? llsd/binary ?>"};

    const auto skip_header =
        [&](std::string_view header) -> bool
        {
            if (bytes.size() < header.size()) return false;
            const std::string_view prefix{
                reinterpret_cast<const char*>(bytes.data()),
                header.size()};
            if (prefix != header) return false;
            offset = header.size();
            while (offset < bytes.size() &&
                   (bytes[offset] == ' ' ||
                    bytes[offset] == '\t' ||
                    bytes[offset] == '\r' ||
                    bytes[offset] == '\n'))
            {
                ++offset;
            }
            return true;
        };

    if (!skip_header(header1))
    {
        (void)skip_header(header2);
    }

    auto parsed = parse_binary_value(bytes, offset, 0);
    if (!parsed.ok()) return parsed;
    bytes_consumed = offset;
    return parsed;
}

LlsdParseResult parse_llsd_binary(std::span<const std::uint8_t> bytes)
{
    std::size_t offset = 0U;
    auto parsed = parse_llsd_binary_prefix(bytes, offset);
    if (!parsed.ok()) return parsed;

    while (offset < bytes.size() &&
           (bytes[offset] == ' ' ||
            bytes[offset] == '\t' ||
            bytes[offset] == '\r' ||
            bytes[offset] == '\n'))
    {
        ++offset;
    }

    if (offset != bytes.size())
    {
        return {std::nullopt, "LLSD binary contains trailing data"};
    }

    return parsed;
}
}
