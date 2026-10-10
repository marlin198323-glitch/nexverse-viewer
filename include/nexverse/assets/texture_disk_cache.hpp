/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/assets/texture_loader.hpp"
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>

namespace nexverse::assets
{
class TextureDiskCache
{
public:
    explicit TextureDiskCache(std::filesystem::path directory,
        std::uint64_t budget_bytes = 256ULL * 1024ULL * 1024ULL);
    [[nodiscard]] std::optional<ImageRgba8> load(std::string_view uuid) const;
    [[nodiscard]] bool store(std::string_view uuid, const ImageRgba8& image) const;
    [[nodiscard]] std::size_t prune() const;
    [[nodiscard]] static std::filesystem::path default_directory();
private:
    std::filesystem::path directory_;
    std::uint64_t budget_bytes_;
};
}
