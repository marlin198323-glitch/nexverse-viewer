/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace nexverse::render
{
struct TerrainVertex
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct TerrainMesh
{
    std::vector<TerrainVertex> vertices;
    std::vector<std::uint32_t> indices;
};

[[nodiscard]] TerrainMesh build_terrain_mesh(
    std::span<const float> heights,
    std::span<const std::uint8_t> valid_patches,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t patch_size);
}
