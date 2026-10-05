/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/net/http_transport.hpp"

#include "nexverse/core/version.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <limits>
#include <mutex>
#include <string>

namespace
{
struct WriteContext
{
    std::string body;
    std::size_t max_bytes = 0;
    bool exceeded = false;
};

std::once_flag curl_init_flag;
CURLcode curl_init_result = CURLE_FAILED_INIT;

void initialize_curl()
{
    curl_init_result = curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK
        ? CURLE_OK
        : CURLE_FAILED_INIT;
}

std::size_t write_body(char* data, std::size_t size, std::size_t count, void* user_data)
{
    auto* context = static_cast<WriteContext*>(user_data);
    if (context == nullptr)
    {
        return 0;
    }

    if (count != 0 && size > std::numeric_limits<std::size_t>::max() / count)
    {
        context->exceeded = true;
        return 0;
    }

    const std::size_t bytes = size * count;
    if (bytes > context->max_bytes ||
        context->body.size() > context->max_bytes - bytes)
    {
        context->exceeded = true;
        return 0;
    }

    context->body.append(data, bytes);
    return bytes;
}

long timeout_ms(std::chrono::milliseconds value)
{
    const auto bounded = std::clamp<std::chrono::milliseconds::rep>(
        value.count(),
        1,
        static_cast<std::chrono::milliseconds::rep>(std::numeric_limits<long>::max()));
    return static_cast<long>(bounded);
}
}

namespace nexverse::net
{
HttpResponse CurlHttpTransport::get(
    std::string_view url,
    std::size_t max_bytes,
    std::chrono::milliseconds connect_timeout,
    std::chrono::milliseconds total_timeout)
{
    std::call_once(curl_init_flag, initialize_curl);
    if (curl_init_result != CURLE_OK)
    {
        return HttpResponse{0, {}, "libcurl global initialization failed"};
    }

    CURL* curl = curl_easy_init();
    if (curl == nullptr)
    {
        return HttpResponse{0, {}, "libcurl handle initialization failed"};
    }

    const std::string url_copy{url};
    const std::string user_agent =
        std::string{"OpenGenesisLINK-Viewer/"} +
        std::string{nexverse::core::version()};

    WriteContext write_context;
    write_context.max_bytes = max_bytes;
    write_context.body.reserve(std::min<std::size_t>(max_bytes, 16U * 1024U));

    curl_easy_setopt(curl, CURLOPT_URL, url_copy.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout_ms(connect_timeout));
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms(total_timeout));
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_context);

    const CURLcode result = curl_easy_perform(curl);

    long status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    curl_easy_cleanup(curl);

    if (write_context.exceeded)
    {
        return HttpResponse{status_code, {}, "HTTP response exceeds configured size limit"};
    }

    if (result != CURLE_OK)
    {
        return HttpResponse{
            status_code,
            {},
            std::string{"HTTP request failed: "} + curl_easy_strerror(result)};
    }

    HttpResponse response;
    response.status_code = status_code;
    response.body = std::move(write_context.body);

    if (status_code < 200 || status_code >= 300)
    {
        response.error = "HTTP server returned status " + std::to_string(status_code);
    }

    return response;
}
