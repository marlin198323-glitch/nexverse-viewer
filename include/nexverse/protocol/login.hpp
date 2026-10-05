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

namespace nexverse::protocol
{
inline constexpr std::size_t max_login_response_bytes = 2U * 1024U * 1024U;

struct LoginCredentials
{
    std::string first_name;
    std::string last_name;
    std::string password;
    std::string start = "last";
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
