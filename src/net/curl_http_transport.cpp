/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/net/http_transport.hpp"
#include "nexverse/core/version.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <limits>
#include <mutex>
#include <string>
#include <utility>

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
    curl_init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
}

std::size_t write_body(char* data, std::size_t size, std::size_t count, void* user_data)
{
    auto* context = static_cast<WriteContext*>(user_data);
    if (context == nullptr || (count != 0 && size > std::numeric_limits<std::size_t>::max() / count))
    {
        return 0;
    }

    const std::size_t bytes = size * count;
    if (bytes > context->max_bytes || context->body.size() > context->max_bytes - bytes)
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
        value.count(), 1, static_cast<std::chrono::milliseconds::rep>(std::numeric_limits<long>::max()));
    return static_cast<long>(bounded);
}

enum class RequestMethod { get, post, put };
nexverse::net::HttpResponse perform_request(
    std::string_view url,
    std::string_view content_type,
    std::string_view body,
    RequestMethod method,
    std::size_t max_bytes,
    std::chrono::milliseconds connect_timeout,
    std::chrono::milliseconds total_timeout)
{
    std::call_once(curl_init_flag, initialize_curl);
    if (curl_init_result != CURLE_OK)
    {
        return {0, {}, "libcurl global initialization failed"};
    }

    CURL* curl = curl_easy_init();
    if (curl == nullptr)
    {
        return {0, {}, "libcurl handle initialization failed"};
    }

    WriteContext context;
    context.max_bytes = max_bytes;
    context.body.reserve(std::min<std::size_t>(max_bytes, 16U * 1024U));

    const std::string url_copy{url};
    const std::string user_agent =
        std::string{"OpenGenesisLINK-Viewer/"} + std::string{nexverse::core::version()};

    curl_easy_setopt(curl, CURLOPT_URL, url_copy.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    // Mutating PUT must not forward an authenticated capability on
    // redirects, which could carry a request to a different origin.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,
                     method==RequestMethod::put?0L:1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout_ms(connect_timeout));
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms(total_timeout));
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &context);

    curl_slist* headers = nullptr;
    if (method!=RequestMethod::get)
    {
        const std::string type_header = "Content-Type: " + std::string{content_type};
        headers = curl_slist_append(headers, type_header.c_str());
        headers = curl_slist_append(headers, "Accept: text/xml, application/xml");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if(method==RequestMethod::post)
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        else
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.data());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body.size()));
    }

    const CURLcode result = curl_easy_perform(curl);

    long status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

    if (headers != nullptr)
    {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);

    if (context.exceeded)
    {
        return {status_code, {}, "HTTP response exceeds configured size limit"};
    }

    if (result != CURLE_OK)
    {
        return {status_code, {}, std::string{"HTTP request failed: "} + curl_easy_strerror(result)};
    }

    nexverse::net::HttpResponse response{status_code, std::move(context.body), {}};
    if (!response.ok())
    {
        response.error = "HTTP server returned status " + std::to_string(status_code);
    }
    return response;
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
    return perform_request(
        url, {}, {}, RequestMethod::get, max_bytes, connect_timeout, total_timeout);
}

HttpResponse CurlHttpTransport::post(
    std::string_view url,
    std::string_view content_type,
    std::string_view body,
    std::size_t max_bytes,
    std::chrono::milliseconds connect_timeout,
    std::chrono::milliseconds total_timeout)
{
    return perform_request(
        url, content_type, body, RequestMethod::post, max_bytes,
        connect_timeout, total_timeout);
}
HttpResponse CurlHttpTransport::put(
    std::string_view url,
    std::string_view content_type,
    std::string_view body,
    std::size_t max_bytes,
    std::chrono::milliseconds connect_timeout,
    std::chrono::milliseconds total_timeout)
{
    return perform_request(url,content_type,body,RequestMethod::put,
                           max_bytes,connect_timeout,total_timeout);
}
}
