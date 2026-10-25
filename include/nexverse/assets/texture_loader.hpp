/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/http_transport.hpp"
#include "nexverse/assets/asset_failure.hpp"
#include "nexverse/assets/asset_cache_lookup.hpp"
#include "nexverse/assets/texture_format_preference.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::assets
{
struct ImageRgba8
{
    std::uint32_t width = 0U;
    std::uint32_t height = 0U;
    std::vector<std::uint8_t> pixels;

    [[nodiscard]] bool valid() const noexcept
    {
        return width > 0U &&
               height > 0U &&
               pixels.size() ==
                   static_cast<std::size_t>(width) *
                   static_cast<std::size_t>(height) * 4U;
    }
};

struct TextureAssetResult
{
    bool success = false;
    std::string asset_id;
    ImageRgba8 image;
    std::string message;
    AssetFailure failure = AssetFailure::none;
    AssetCacheLookup disk_lookup = AssetCacheLookup::not_checked;
    bool network_requested = false;
    // Only a genuine successful fresh reread after a malformed J2C
    // transfer; not inferred from an ordinary cache hit or PNG fallback.
    bool recovered_j2c_transfer=false;
    bool cancelled_before_download = false;
};

[[nodiscard]] std::string build_get_texture_uri(
    std::string_view capability_uri,
    std::string_view texture_id,
    std::string_view format = "png");

[[nodiscard]] TextureAssetResult decode_png_texture(
    std::string_view texture_id,
    std::string_view bytes);

// Decode JPEG2000 codestream (J2C) or boxed JP2 into RGBA8.
[[nodiscard]] TextureAssetResult decode_jpeg2000_texture(
    std::string_view texture_id,
    std::string_view bytes);

class TextureAssetLoader
{
public:
    explicit TextureAssetLoader(
        std::shared_ptr<net::IHttpTransport> transport);

    // Optional per-region evidence, shared across successive one-shot
    // HTTP workers. No token, server URL, UUID or secret retained.
    void set_format_preference(
        std::shared_ptr<TextureFormatPreference> preference,
        std::uint64_t region_epoch) noexcept
    {
        format_preference_=std::move(preference);
        format_epoch_=region_epoch;
    }

    // Request PNG first; retry the native OpenSim x-j2c codestream
    // on conversion/HTTP/decode failures.
    [[nodiscard]] TextureAssetResult fetch_rgba(
        std::string_view get_texture_capability,
        std::string_view texture_id) const;

private:
    std::shared_ptr<net::IHttpTransport> transport_;
    std::shared_ptr<TextureFormatPreference> format_preference_;
    std::uint64_t format_epoch_=0U;
};
}
