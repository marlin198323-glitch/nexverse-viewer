/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/assets/texture_loader.hpp"

#include <png.h>

#include <chrono>
#include <limits>
#include <utility>

namespace
{
constexpr std::size_t max_texture_download_bytes =
    32U * 1024U * 1024U;
constexpr std::uint32_t max_texture_dimension = 4096U;

bool safe_texture_id(std::string_view id)
{
    if (id.size() != 36U)
    {
        return false;
    }

    for (std::size_t index = 0U; index < id.size(); ++index)
    {
        const char value = id[index];
        if (index == 8U || index == 13U ||
            index == 18U || index == 23U)
        {
            if (value != '-')
            {
                return false;
            }
            continue;
        }

        const bool hex =
            (value >= '0' && value <= '9') ||
            (value >= 'a' && value <= 'f') ||
            (value >= 'A' && value <= 'F');
        if (!hex)
        {
            return false;
        }
    }

    return true;
}

bool safe_format(std::string_view format)
{
    if (format.empty() || format.size() > 16U)
    {
        return false;
    }

    for (const char value : format)
    {
        const bool ok =
            (value >= 'a' && value <= 'z') ||
            (value >= 'A' && value <= 'Z') ||
            (value >= '0' && value <= '9') ||
            value == '-';
        if (!ok)
        {
            return false;
        }
    }

    return true;
}
}

namespace nexverse::assets
{
std::string build_get_texture_uri(
    std::string_view capability_uri,
    std::string_view texture_id,
    std::string_view format)
{
    if (capability_uri.empty() ||
        !safe_texture_id(texture_id) ||
        !safe_format(format))
    {
        return {};
    }

    std::string uri{capability_uri};
    const bool has_query = uri.find('?') != std::string::npos;

    if (!uri.empty() && uri.back() != '?' && uri.back() != '&')
    {
        uri.push_back(has_query ? '&' : '?');
    }

    uri += "texture_id=";
    uri.append(texture_id);
    uri += "&format=";
    uri.append(format);
    return uri;
}

TextureAssetResult decode_png_texture(
    std::string_view texture_id,
    std::string_view bytes)
{
    TextureAssetResult result;
    result.asset_id = std::string{texture_id};

    if (!safe_texture_id(texture_id))
    {
        result.message = "Texture UUID is invalid";
        return result;
    }
    if (bytes.empty() || bytes.size() > max_texture_download_bytes)
    {
        result.message = "Texture PNG payload size is invalid";
        return result;
    }

    png_image image{};
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(
            &image,
            bytes.data(),
            bytes.size()) == 0)
    {
        result.message = "Texture PNG header is invalid";
        return result;
    }

    if (image.width == 0U ||
        image.height == 0U ||
        image.width > max_texture_dimension ||
        image.height > max_texture_dimension)
    {
        png_image_free(&image);
        result.message = "Texture PNG dimensions are invalid";
        return result;
    }

    image.format = PNG_FORMAT_RGBA;

    const png_alloc_size_t byte_count = PNG_IMAGE_SIZE(image);
    if (byte_count == 0U ||
        byte_count > max_texture_download_bytes)
    {
        png_image_free(&image);
        result.message = "Texture PNG decoded size is invalid";
        return result;
    }

    result.image.width = image.width;
    result.image.height = image.height;
    result.image.pixels.resize(
        static_cast<std::size_t>(byte_count));

    if (png_image_finish_read(
            &image,
            nullptr,
            result.image.pixels.data(),
            0,
            nullptr) == 0)
    {
        png_image_free(&image);
        result.image = {};
        result.message = "Texture PNG decode failed";
        return result;
    }

    png_image_free(&image);

    if (!result.image.valid())
    {
        result.image = {};
        result.message = "Texture PNG RGBA output is invalid";
        return result;
    }

    result.success = true;
    result.message = "Texture PNG loaded";
    return result;
}

TextureAssetLoader::TextureAssetLoader(
    std::shared_ptr<net::IHttpTransport> transport)
    : transport_{std::move(transport)}
{
}

TextureAssetResult TextureAssetLoader::fetch_png(
    std::string_view get_texture_capability,
    std::string_view texture_id) const
{
    if (transport_ == nullptr)
    {
        return {
            false,
            std::string{texture_id},
            {},
            "HTTP transport unavailable"};
    }

    const std::string uri = build_get_texture_uri(
        get_texture_capability,
        texture_id,
        "png");
    if (uri.empty())
    {
        return {
            false,
            std::string{texture_id},
            {},
            "GetTexture URI could not be constructed"};
    }

    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{30};

    const net::HttpResponse response = transport_->get(
        uri,
        max_texture_download_bytes,
        connect_timeout,
        total_timeout);

    if (!response.ok())
    {
        return {
            false,
            std::string{texture_id},
            {},
            response.error.empty()
                ? "GetTexture request failed"
                : response.error};
    }

    return decode_png_texture(
        texture_id,
        response.body);
}
}
