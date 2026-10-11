/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <string_view>

namespace nexverse::net
{
struct HttpResponse
{
    long status_code = 0;
    std::string body;
    std::string error;

    [[nodiscard]] bool ok() const noexcept
    {
        return error.empty() && status_code >= 200 && status_code < 300;
    }
};

class IHttpTransport
{
public:
    virtual ~IHttpTransport() = default;

    virtual HttpResponse get(
        std::string_view url,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) = 0;

    virtual HttpResponse post(
        std::string_view url,
        std::string_view content_type,
        std::string_view body,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) = 0;

    // Opt-in mutating CAPS operation. Existing mocks and transports fail
    // closed unless they implement PUT. Only explicit UI confirmation
    // may call this method.
    virtual HttpResponse put(
        std::string_view, std::string_view, std::string_view,
        std::size_t, std::chrono::milliseconds,
        std::chrono::milliseconds)
    {
        return {0, {}, "HTTP PUT unsupported by transport"};
    }
    virtual HttpResponse remove(
        std::string_view,std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds)
    {
        return {0, {}, "HTTP DELETE unsupported by transport"};
    }
};

class CurlHttpTransport final : public IHttpTransport
{
public:
    HttpResponse get(
        std::string_view url,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override;

    HttpResponse post(
        std::string_view url,
        std::string_view content_type,
        std::string_view body,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override;
    HttpResponse put(
        std::string_view url,
        std::string_view content_type,
        std::string_view body,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override;
    HttpResponse remove(
        std::string_view url,std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override;
};
}
