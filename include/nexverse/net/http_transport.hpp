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
};

class CurlHttpTransport final : public IHttpTransport
{
public:
    HttpResponse get(
        std::string_view url,
        std::size_t max_bytes,
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override;
};
}
