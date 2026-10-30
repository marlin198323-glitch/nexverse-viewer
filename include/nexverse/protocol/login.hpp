/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/http_transport.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
inline constexpr std::size_t max_login_response_bytes = 2U * 1024U * 1024U;

// Human-facing build labels are vMAJOR.MINOR.PATCH.BUILD. OpenGenesisLINK
// OglApprovedViewerPolicy.IsNumericVersion(clientVersion) requires the XML-RPC
// "version" field to START with a decimal digit, or it blocks this genuine
// OGL Viewer with reason="presence" (LoginBlockedProblem). Do not spoof a
// different viewer name; strip only the purely presentational v/V prefix.
[[nodiscard]] constexpr std::string_view ogl_login_wire_version(
    std::string_view display_version) noexcept
{
    return display_version.size()>1U&&
           (display_version.front()=='v'||display_version.front()=='V')&&
           display_version[1]>='0'&&display_version[1]<='9'
        ?display_version.substr(1U):display_version;
}

struct LoginCredentials
{
    std::string first_name;
    std::string last_name;
    std::string password;
    std::string start = "last";
    // False unless the user explicitly accepted the server's ToS challenge
    // for this avatar and this grid. Never silently agree for all logins.
    bool agree_to_tos = false;
    // Firestorm 7.2.4/OpenGenesisLINK #268: a genuine one-part avatar
    // account uses XML-RPC "username" instead of first/last.
    std::string account_username;
};

struct InventoryFolder
{
    std::string folder_id;
    std::string parent_id;
    std::string name;
    std::int32_t type_default = 0;
    std::int32_t version = 0;
};

struct BuddyEntry
{
    std::string buddy_id;
    std::int32_t rights_has = 0;
    std::int32_t rights_given = 0;
};

struct GestureEntry
{
    std::string item_id;
    std::string asset_id;
};

struct InitialOutfit
{
    std::string folder_name;
    std::string gender;
};

struct LoginFlags
{
    std::string ever_logged_in;
    std::string stipend_since_login;
    std::string gendered;
    std::string daylight_savings;
};

struct GlobalTextures
{
    std::string cloud_texture_id;
    std::string sun_texture_id;
    std::string moon_texture_id;
};

struct LoginSession
{
    std::string agent_id;
    std::string session_id;
    std::string secure_session_id;
    std::uint32_t circuit_code = 0;
    std::string seed_capability;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0;
    std::uint32_t region_x = 0;
    std::uint32_t region_y = 0;
    std::string first_name;
    std::string last_name;
    std::string start_location;
    std::string inventory_root_id;
    std::string inventory_library_root_id;
    std::string inventory_library_owner_id;
    std::vector<InventoryFolder> inventory_skeleton;
    std::vector<InventoryFolder> library_skeleton;
    std::vector<BuddyEntry> buddies;
    std::vector<GestureEntry> gestures;
    std::vector<InitialOutfit> initial_outfits;
    LoginFlags login_flags;
    GlobalTextures global_textures;
    std::map<std::string, std::string, std::less<>> scalar_fields;
};

struct LoginResult
{
    bool success = false;
    std::optional<LoginSession> session;
    std::string reason;
    std::string message;
};

[[nodiscard]] std::string hash_login_password(std::string_view password);
[[nodiscard]] std::string build_login_xml_rpc(
    const LoginCredentials& credentials,
    std::string_view channel,
    std::string_view version);
[[nodiscard]] LoginResult parse_login_xml_rpc_response(std::string_view xml);

class LoginService
{
public:
    explicit LoginService(std::shared_ptr<net::IHttpTransport> transport);

    [[nodiscard]] LoginResult login(
        std::string_view login_uri,
        const LoginCredentials& credentials,
        std::string_view channel,
        std::string_view version) const;

private:
    std::shared_ptr<net::IHttpTransport> transport_;
};
}
