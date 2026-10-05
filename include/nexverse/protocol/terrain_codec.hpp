/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace nexverse::protocol
{
inline constexpr std::size_t terrain_patch_size = 16U;
inline constexpr std::size_t terrain_patch_samples =
    terrain_patch_size * terrain_patch_size;
inline constexpr std::size_t terrain_patches_per_edge = 16U;

struct TerrainPatch
{
    std::uint8_t x = 0U;
    std::uint8_t y = 0U;
    std::array<float, terrain_patch_samples> heights{};
};

struct TerrainDecodeResult
{
    bool success = false;
    std::vector<TerrainPatch> patches;
    std::string message;
};

[[nodiscard]] TerrainDecodeResult decode_terrain_layer(
    std::span<const std::uint8_t> data,
    std::uint8_t expected_layer_type);
}
