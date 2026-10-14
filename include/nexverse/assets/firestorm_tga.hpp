/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_loader.hpp"
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>
namespace nexverse::assets
{
struct FirestormTgaResult
{
    ImageRgba8 image;
    std::string error;
    [[nodiscard]] bool ok() const noexcept{return image.valid();}
};
// Firestorm's character/ alpha masks are typically grayscale RLE TGA
// type 11. Both uncompressed 2/3 and RLE 10/11 supported; no palette,
// malformed origin or trailing read outside stream is tolerated.
[[nodiscard]] FirestormTgaResult decode_firestorm_tga(
    std::span<const std::uint8_t> bytes);
[[nodiscard]] FirestormTgaResult load_firestorm_character_tga(
    const std::filesystem::path& character_directory,
    const std::string& basename);
}
