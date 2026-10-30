/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/login.hpp"

#include "nexverse/protocol/grid_info.hpp"

#include <openssl/evp.h>
#include <tinyxml2.h>

#include <array>
#include <chrono>
#include <charconv>
#include <iomanip>
#include <sstream>
#include <utility>

namespace
{
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

void append_member(XMLDocument& document, XMLElement* structure, const char* name, std::string_view value)
{
    auto* member = document.NewElement("member");
    auto* name_element = document.NewElement("name");
    name_element->SetText(name);
    auto* value_element = document.NewElement("value");
    auto* string_element = document.NewElement("string");
    string_element->SetText(std::string{value}.c_str());

    value_element->InsertEndChild(string_element);
    member->InsertEndChild(name_element);
    member->InsertEndChild(value_element);
    structure->InsertEndChild(member);
}

void append_boolean(XMLDocument& document, XMLElement* structure, const char* name, bool value)
{
    auto* member = document.NewElement("member");
    auto* name_element = document.NewElement("name");
    name_element->SetText(name);
    auto* value_element = document.NewElement("value");
    auto* boolean_element = document.NewElement("boolean");
    boolean_element->SetText(value ? "1" : "0");

    value_element->InsertEndChild(boolean_element);
    member->InsertEndChild(name_element);
    member->InsertEndChild(value_element);
    structure->InsertEndChild(member);
}

void append_options(XMLDocument& document, XMLElement* structure)
{
    constexpr std::array<std::string_view, 16> options{
        "inventory-root",
        "inventory-skeleton",
        "inventory-lib-root",
        "inventory-lib-owner",
        "inventory-skel-lib",
        "initial-outfit",
        "gestures",
        "event_categories",
        "event_notifications",
        "classified_categories",
        "buddy-list",
        "ui-config",
        "tutorial_settings",
        "login-flags",
        "global-textures",
        "adult_compliant"
    };

    auto* member = document.NewElement("member");
    auto* name_element = document.NewElement("name");
    name_element->SetText("options");
    auto* value_element = document.NewElement("value");
    auto* array_element = document.NewElement("array");
    auto* data = document.NewElement("data");

    for (const auto option : options)
    {
        auto* item_value = document.NewElement("value");
        auto* item_string = document.NewElement("string");
        item_string->SetText(std::string{option}.c_str());
        item_value->InsertEndChild(item_string);
        data->InsertEndChild(item_value);
    }

    array_element->InsertEndChild(data);
    value_element->InsertEndChild(array_element);
    member->InsertEndChild(name_element);
    member->InsertEndChild(value_element);
    structure->InsertEndChild(member);
}

const XMLElement* response_struct(const XMLDocument& document)
{
    const auto* response = document.FirstChildElement("methodResponse");
    if (response == nullptr) return nullptr;
    const auto* params = response->FirstChildElement("params");
    if (params == nullptr) return nullptr;
    const auto* param = params->FirstChildElement("param");
    if (param == nullptr) return nullptr;
    const auto* value = param->FirstChildElement("value");
    if (value == nullptr) return nullptr;
    return value->FirstChildElement("struct");
}

std::string scalar_value(const XMLElement* value)
{
    if (value == nullptr) return {};

    const XMLElement* typed = value->FirstChildElement();
    if (typed == nullptr)
    {
        const char* text = value->GetText();
        return text == nullptr ? std::string{} : std::string{text};
    }

    const std::string_view type{typed->Name()};
    if (type == "string" || type == "boolean" || type == "int" ||
        type == "i4" || type == "double" || type == "dateTime.iso8601" ||
        type == "base64")
    {
        const char* text = typed->GetText();
        return text == nullptr ? std::string{} : std::string{text};
    }

    return {};
}

std::map<std::string, std::string, std::less<>> parse_scalar_members(const XMLElement* structure)
{
    std::map<std::string, std::string, std::less<>> fields;
    if (structure == nullptr) return fields;

    for (const XMLElement* member = structure->FirstChildElement("member");
         member != nullptr;
         member = member->NextSiblingElement("member"))
    {
        const XMLElement* name = member->FirstChildElement("name");
        const XMLElement* value = member->FirstChildElement("value");
        if (name == nullptr || value == nullptr || name->GetText() == nullptr) continue;

        std::string scalar = scalar_value(value);
        if (!scalar.empty())
        {
            fields[std::string{name->GetText()}] = std::move(scalar);
        }
    }
    return fields;
}

const XMLElement* member_value(const XMLElement* structure, std::string_view key)
{
    if (structure == nullptr) return nullptr;

    for (const XMLElement* member = structure->FirstChildElement("member");
         member != nullptr;
         member = member->NextSiblingElement("member"))
    {
        const XMLElement* name = member->FirstChildElement("name");
        if (name == nullptr || name->GetText() == nullptr) continue;
        if (std::string_view{name->GetText()} == key)
        {
            return member->FirstChildElement("value");
        }
    }
    return nullptr;
}

std::vector<std::map<std::string, std::string, std::less<>>> parse_array_of_structs(
    const XMLElement* value)
{
    std::vector<std::map<std::string, std::string, std::less<>>> result;
    if (value == nullptr) return result;

    const XMLElement* array = value->FirstChildElement("array");
    if (array == nullptr) return result;

    const XMLElement* data = array->FirstChildElement("data");
    if (data == nullptr) return result;

    for (const XMLElement* item = data->FirstChildElement("value");
         item != nullptr;
         item = item->NextSiblingElement("value"))
    {
        const XMLElement* structure = item->FirstChildElement("struct");
        if (structure == nullptr) continue;
        result.push_back(parse_scalar_members(structure));
    }

    return result;
}

std::string field(
    const std::map<std::string, std::string, std::less<>>& fields,
    std::string_view key)
{
    const auto found = fields.find(key);
    return found == fields.end() ? std::string{} : found->second;
}

template <typename Integer>
Integer parse_integer(std::string_view value)
{
    Integer result{};
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    return parsed.ec == std::errc{} ? result : Integer{};
}

std::vector<nexverse::protocol::InventoryFolder> parse_inventory_folders(
    const XMLElement* value)
{
    std::vector<nexverse::protocol::InventoryFolder> folders;
    for (const auto& item : parse_array_of_structs(value))
    {
        nexverse::protocol::InventoryFolder folder;
        folder.folder_id = field(item, "folder_id");
        folder.parent_id = field(item, "parent_id");
        folder.name = field(item, "name");
        folder.type_default = parse_integer<std::int32_t>(field(item, "type_default"));
        folder.version = parse_integer<std::int32_t>(field(item, "version"));

        if (!folder.folder_id.empty())
        {
            folders.push_back(std::move(folder));
        }
    }
    return folders;
}

std::vector<nexverse::protocol::BuddyEntry> parse_buddies(const XMLElement* value)
{
    std::vector<nexverse::protocol::BuddyEntry> buddies;
    for (const auto& item : parse_array_of_structs(value))
    {
        nexverse::protocol::BuddyEntry buddy;
        buddy.buddy_id = field(item, "buddy_id");
        buddy.rights_has = parse_integer<std::int32_t>(field(item, "buddy_rights_has"));
        buddy.rights_given = parse_integer<std::int32_t>(field(item, "buddy_rights_given"));

        if (!buddy.buddy_id.empty())
        {
            buddies.push_back(std::move(buddy));
        }
    }
    return buddies;
}

std::string first_array_scalar(
    const XMLElement* value,
    std::string_view preferred_key)
{
    const auto items = parse_array_of_structs(value);
    if (items.empty()) return {};

    const std::string preferred = field(items.front(), preferred_key);
    if (!preferred.empty()) return preferred;

    if (!items.front().empty())
    {
        return items.front().begin()->second;
    }
    return {};
}

std::vector<nexverse::protocol::GestureEntry> parse_gestures(const XMLElement* value)
{
    std::vector<nexverse::protocol::GestureEntry> gestures;
    for (const auto& item : parse_array_of_structs(value))
    {
        nexverse::protocol::GestureEntry gesture;
        gesture.item_id = field(item, "item_id");
        gesture.asset_id = field(item, "asset_id");

        if (!gesture.item_id.empty() || !gesture.asset_id.empty())
        {
            gestures.push_back(std::move(gesture));
        }
    }
    return gestures;
}

std::vector<nexverse::protocol::InitialOutfit> parse_initial_outfits(const XMLElement* value)
{
    std::vector<nexverse::protocol::InitialOutfit> outfits;
    for (const auto& item : parse_array_of_structs(value))
    {
        nexverse::protocol::InitialOutfit outfit;
        outfit.folder_name = field(item, "folder_name");
        outfit.gender = field(item, "gender");

        if (!outfit.folder_name.empty() || !outfit.gender.empty())
        {
            outfits.push_back(std::move(outfit));
        }
    }
    return outfits;
}

nexverse::protocol::LoginFlags parse_login_flags(const XMLElement* value)
{
    nexverse::protocol::LoginFlags flags;
    const auto items = parse_array_of_structs(value);
    if (items.empty()) return flags;

    flags.ever_logged_in = field(items.front(), "ever_logged_in");
    flags.stipend_since_login = field(items.front(), "stipend_since_login");
    flags.gendered = field(items.front(), "gendered");
    flags.daylight_savings = field(items.front(), "daylight_savings");
    return flags;
}

nexverse::protocol::GlobalTextures parse_global_textures(const XMLElement* value)
{
    nexverse::protocol::GlobalTextures textures;
    const auto items = parse_array_of_structs(value);
    if (items.empty()) return textures;

    textures.cloud_texture_id = field(items.front(), "cloud_texture_id");
    textures.sun_texture_id = field(items.front(), "sun_texture_id");
    textures.moon_texture_id = field(items.front(), "moon_texture_id");
    return textures;
}

bool login_true(std::string_view value)
{
    return value == "true" || value == "1";
}
}

namespace nexverse::protocol
{
std::string hash_login_password(std::string_view password)
{
    if (password.starts_with("$1$") && password.size() == 35U)
    {
        return std::string{password};
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
    unsigned int digest_size = 0;

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context == nullptr) return {};

    const bool ok =
        EVP_DigestInit_ex(context, EVP_md5(), nullptr) == 1 &&
        EVP_DigestUpdate(context, password.data(), password.size()) == 1 &&
        EVP_DigestFinal_ex(context, digest.data(), &digest_size) == 1;

    EVP_MD_CTX_free(context);
    if (!ok) return {};

    std::ostringstream output;
    output << "$1$" << std::hex << std::setfill('0');
    for (unsigned int index = 0; index < digest_size; ++index)
    {
        output << std::setw(2) << static_cast<unsigned int>(digest[index]);
    }
    return output.str();
}

std::string build_login_xml_rpc(
    const LoginCredentials& credentials,
    std::string_view channel,
    std::string_view version)
{
    XMLDocument document;

    auto* call = document.NewElement("methodCall");
    document.InsertEndChild(call);

    auto* method = document.NewElement("methodName");
    method->SetText("login_to_simulator");
    call->InsertEndChild(method);

    auto* params = document.NewElement("params");
    auto* param = document.NewElement("param");
    auto* value = document.NewElement("value");
    auto* structure = document.NewElement("struct");

    // Exactly one identity shape is sent. Legacy first/last requests
    // must remain wire-identical; modern single-name users go through
    // the upstream server's Resident username resolver (#268).
    if(!credentials.account_username.empty())
        append_member(document, structure, "username",
                      credentials.account_username);
    else
    {
        append_member(document, structure, "first", credentials.first_name);
        append_member(document, structure, "last", credentials.last_name);
    }
    append_member(document, structure, "passwd", hash_login_password(credentials.password));
    append_member(document, structure, "start", credentials.start.empty() ? "last" : credentials.start);
    append_member(document, structure, "channel", channel);
    // OpenGenesisLINK's viewer allowlist requires a numeric-leading
    // XML-RPC version, even though our on-screen build label is vX.X.X.X.
    append_member(document, structure, "version",
                  ogl_login_wire_version(version));
#if defined(_WIN32)
    append_member(document, structure, "platform", "Win");
#elif defined(__APPLE__)
    append_member(document, structure, "platform", "Mac");
#else
    append_member(document, structure, "platform", "Linux");
#endif
    append_member(document, structure, "platform_version", "");
    append_member(document, structure, "mac", "");
    append_member(document, structure, "id0", "");
    append_member(document, structure, "viewer_digest", "");
    append_boolean(document, structure, "agree_to_tos",
                   credentials.agree_to_tos);
    append_boolean(document, structure, "read_critical", true);
    append_options(document, structure);

    value->InsertEndChild(structure);
    param->InsertEndChild(value);
    params->InsertEndChild(param);
    call->InsertEndChild(params);

    tinyxml2::XMLPrinter printer;
    document.Print(&printer);
    return std::string{printer.CStr(), printer.CStrSize() - 1U};
}

LoginResult parse_login_xml_rpc_response(std::string_view xml)
{
    if (xml.empty())
    {
        return {false, std::nullopt, "empty_response", "Login server returned an empty response"};
    }
    if (xml.size() > max_login_response_bytes)
    {
        return {false, std::nullopt, "response_too_large", "Login response exceeds configured size limit"};
    }

    XMLDocument document;
    if (document.Parse(xml.data(), xml.size()) != tinyxml2::XML_SUCCESS)
    {
        return {false, std::nullopt, "invalid_xml", "Login response is not valid XML-RPC"};
    }

    const XMLElement* structure = response_struct(document);
    if (structure == nullptr)
    {
        return {false, std::nullopt, "invalid_response", "Login response does not contain an XML-RPC struct"};
    }

    auto fields = parse_scalar_members(structure);
    const std::string login = field(fields, "login");
    const std::string reason = field(fields, "reason");
    const std::string message = field(fields, "message");

    if (!login_true(login))
    {
        return {
            false,
            std::nullopt,
            reason.empty() ? "login_failed" : reason,
            message.empty() ? "OpenSim login rejected" : message};
    }

    LoginSession session;
    session.agent_id = field(fields, "agent_id");
    session.session_id = field(fields, "session_id");
    session.secure_session_id = field(fields, "secure_session_id");
    session.seed_capability = field(fields, "seed_capability");
    session.simulator_ip = field(fields, "sim_ip");
    session.first_name = field(fields, "first_name");
    session.last_name = field(fields, "last_name");
    session.start_location = field(fields, "start_location");
    session.circuit_code = parse_integer<std::uint32_t>(field(fields, "circuit_code"));
    session.simulator_port = parse_integer<std::uint16_t>(field(fields, "sim_port"));
    session.region_x = parse_integer<std::uint32_t>(field(fields, "region_x"));
    session.region_y = parse_integer<std::uint32_t>(field(fields, "region_y"));

    session.inventory_root_id = first_array_scalar(
        member_value(structure, "inventory-root"),
        "folder_id");
    session.inventory_library_root_id = first_array_scalar(
        member_value(structure, "inventory-lib-root"),
        "folder_id");
    session.inventory_library_owner_id = first_array_scalar(
        member_value(structure, "inventory-lib-owner"),
        "agent_id");
    session.inventory_skeleton = parse_inventory_folders(
        member_value(structure, "inventory-skeleton"));
    session.library_skeleton = parse_inventory_folders(
        member_value(structure, "inventory-skel-lib"));
    session.buddies = parse_buddies(
        member_value(structure, "buddy-list"));
    session.gestures = parse_gestures(
        member_value(structure, "gestures"));
    session.initial_outfits = parse_initial_outfits(
        member_value(structure, "initial-outfit"));
    session.login_flags = parse_login_flags(
        member_value(structure, "login-flags"));
    session.global_textures = parse_global_textures(
        member_value(structure, "global-textures"));

    session.scalar_fields = std::move(fields);

    if (session.agent_id.empty() ||
        session.session_id.empty() ||
        session.secure_session_id.empty() ||
        session.circuit_code == 0 ||
        session.simulator_ip.empty() ||
        session.simulator_port == 0 ||
        session.seed_capability.empty())
    {
        return {
            false,
            std::nullopt,
            "incomplete_session",
            "Login succeeded but required simulator session data is missing"};
    }

    return {true, std::move(session), {}, message};
}

LoginService::LoginService(std::shared_ptr<net::IHttpTransport> transport)
    : transport_{std::move(transport)}
{
}

LoginResult LoginService::login(
    std::string_view login_uri,
    const LoginCredentials& credentials,
    std::string_view channel,
    std::string_view version) const
{
    if (transport_ == nullptr)
    {
        return {false, std::nullopt, "transport_unavailable", "HTTP transport unavailable"};
    }
    if (!is_valid_http_uri(login_uri))
    {
        return {false, std::nullopt, "invalid_login_uri", "Grid supplied an invalid login URI"};
    }
    const bool account_identity=
        !credentials.account_username.empty() &&
        credentials.account_username.find_first_not_of(' ')!=std::string::npos;
    const bool legacy_identity=
        !credentials.first_name.empty()&&!credentials.last_name.empty();
    // Fail closed on ambiguous identity: an account-only login must
    // never silently override a separately provided first/last user.
    if (credentials.password.empty() ||
        (!account_identity&&!legacy_identity)||
        (account_identity&&legacy_identity))
    {
        return {false, std::nullopt, "missing_credentials",
                "Avatar name and password are required"};
    }

    const std::string request = build_login_xml_rpc(credentials, channel, version);
    if (request.find("<string></string>") != std::string::npos)
    {
        // Empty optional fields are valid; do not reject them.
    }

    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{30};

    const net::HttpResponse response = transport_->post(
        login_uri,
        "text/xml",
        request,
        max_login_response_bytes,
        connect_timeout,
        total_timeout);

    if (!response.ok())
    {
        return {
            false,
            std::nullopt,
            "http_error",
            response.error.empty() ? "OpenSim login request failed" : response.error};
    }

    return parse_login_xml_rpc_response(response.body);
}
}
