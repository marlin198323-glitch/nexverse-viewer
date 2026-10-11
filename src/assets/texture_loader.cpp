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

TextureAssetResult TextureAssetLoader::fetch_rgba(
    std::string_view get_texture_capability,
    std::string_view texture_id) const
{
    TextureAssetResult result;
    result.asset_id = std::string{texture_id};
    if (transport_ == nullptr)
    {
        result.message = "HTTP transport unavailable";
        result.failure = AssetFailure::configuration;
        return result;
    }

    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{30};

    const std::string png_uri = build_get_texture_uri(
        get_texture_capability, texture_id, "png");
    const std::string j2c_uri = build_get_texture_uri(
        get_texture_capability, texture_id, "x-j2c");
    if (png_uri.empty() || j2c_uri.empty())
    {
        result.message = "GetTexture URI could not be constructed";
        result.failure = AssetFailure::configuration;
        return result;
    }

    const auto png_response = transport_->get(
        png_uri, max_texture_download_bytes,
        connect_timeout, total_timeout);
    std::string png_error;
    AssetFailure png_failure = classify_http_asset_failure(png_response);
    if (png_response.ok())
    {
        result = decode_png_texture(texture_id, png_response.body);
        if (result.success) return result;
        png_error = result.message;
        png_failure = AssetFailure::invalid;
    }
    else
    {
        png_error = png_response.error.empty()
            ? "GetTexture PNG request failed" : png_response.error;
    }

    // A missing server-side PNG converter must not prevent native
    // JPEG2000 textures from loading. Never reuse the failed PNG payload.
    const auto j2c_response = transport_->get(
        j2c_uri, max_texture_download_bytes,
        connect_timeout, total_timeout);
    AssetFailure j2c_failure = classify_http_asset_failure(j2c_response);
    if (j2c_response.ok())
    {
        result = decode_jpeg2000_texture(texture_id, j2c_response.body);
        if (result.success)
        {
            result.message = "GetTexture x-j2c fallback succeeded";
            return result;
        }
        j2c_failure = AssetFailure::invalid;
        result.message = "png: " + png_error +
                         "; x-j2c decode: " + result.message;
    }
    else
    {
        const std::string j2c_error = j2c_response.error.empty()
            ? "GetTexture x-j2c request failed" : j2c_response.error;
        result.message = "png: " + png_error +
                         "; x-j2c: " + j2c_error;
    }
    result.failure = merge_asset_failure(png_failure,j2c_failure);
    return result;
}
}
