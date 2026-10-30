/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/assets/texture_loader.hpp"
#include "nexverse/assets/asset_cache_lookup.hpp"
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>

namespace nexverse::assets
{
class TextureDiskCache
{
public:
    // Firestorm-style user-adjustable disk budget, independent of
    // GPU VRAM. OGL_TEXTURE_CACHE_MIB accepts 128..2048 MiB,
    // otherwise use a bounded 512 MiB default.
    [[nodiscard]] static std::uint64_t default_budget_bytes() noexcept;
    explicit TextureDiskCache(std::filesystem::path directory,
        std::uint64_t budget_bytes = default_budget_bytes());
    // Cheap metadata-only test. Actual decoder/checksum checks are
    // STILL enforced by load(). Never return the path or a secret URI.
    [[nodiscard]] bool contains(std::string_view uuid) const noexcept;
    [[nodiscard]] std::optional<ImageRgba8> load(std::string_view uuid,
        AssetCacheLookup* status = nullptr) const;
    [[nodiscard]] bool store(std::string_view uuid, const ImageRgba8& image) const;
    [[nodiscard]] std::size_t prune() const;
    [[nodiscard]] static std::filesystem::path default_directory();
private:
    std::filesystem::path directory_;
    std::uint64_t budget_bytes_;
};
}
