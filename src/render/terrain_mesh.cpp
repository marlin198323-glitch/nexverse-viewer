/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/terrain_mesh.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>

namespace
{
bool patch_valid(
    std::span<const std::uint8_t> valid_patches,
    std::uint32_t patch_x,
    std::uint32_t patch_y,
    std::uint32_t patches_x)
{
    const std::size_t slot =
        static_cast<std::size_t>(patch_y) * patches_x + patch_x;
    return slot < valid_patches.size() && valid_patches[slot] != 0U;
}

bool sample_valid(
    std::span<const std::uint8_t> valid_patches,
    std::uint32_t x,
    std::uint32_t y,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t patch_size)
{
    if (x >= width || y >= height || patch_size == 0U)
    {
        return false;
    }

    const std::uint32_t patches_x =
        (width + patch_size - 1U) / patch_size;
    const std::uint32_t patch_x = x / patch_size;
    const std::uint32_t patch_y = y / patch_size;
    return patch_valid(
        valid_patches,
        patch_x,
        patch_y,
        patches_x);
}

nexverse::render::TerrainVertex make_vertex(
    float x,
    float y,
    float z,
    float min_height,
    float max_height)
{
    const float span = std::max(max_height - min_height, 0.001f);
    const float t = std::clamp((z - min_height) / span, 0.0f, 1.0f);

    nexverse::render::TerrainVertex vertex;
    vertex.x = x;
    vertex.y = y;
    vertex.z = z;

    if (t < 0.35f)
    {
        const float local = t / 0.35f;
        vertex.r = 0.08f + local * 0.06f;
        vertex.g = 0.28f + local * 0.22f;
        vertex.b = 0.12f + local * 0.06f;
    }
    else if (t < 0.78f)
    {
        const float local = (t - 0.35f) / 0.43f;
        vertex.r = 0.14f + local * 0.28f;
        vertex.g = 0.50f - local * 0.18f;
        vertex.b = 0.18f + local * 0.06f;
    }
    else
    {
        const float local = (t - 0.78f) / 0.22f;
        vertex.r = 0.42f + local * 0.38f;
        vertex.g = 0.32f + local * 0.48f;
        vertex.b = 0.24f + local * 0.56f;
    }

    return vertex;
}
}

namespace nexverse::render
{
TerrainMesh build_terrain_mesh(
    std::span<const float> heights,
    std::span<const std::uint8_t> valid_patches,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t patch_size)
{
    TerrainMesh mesh;

    if (width < 2U ||
        height < 2U ||
        patch_size == 0U ||
        heights.size() <
            static_cast<std::size_t>(width) * height)
    {
        return mesh;
    }

    float min_height = std::numeric_limits<float>::max();
    float max_height = std::numeric_limits<float>::lowest();
    bool any_valid = false;

    for (std::uint32_t y = 0U; y < height; ++y)
    {
        for (std::uint32_t x = 0U; x < width; ++x)
        {
            if (!sample_valid(
                    valid_patches,
                    x,
                    y,
                    width,
                    height,
                    patch_size))
            {
                continue;
            }

            const float z =
                heights[static_cast<std::size_t>(y) * width + x];
            min_height = std::min(min_height, z);
            max_height = std::max(max_height, z);
            any_valid = true;
        }
    }

    if (!any_valid)
    {
        return mesh;
    }

    mesh.vertices.resize(
        static_cast<std::size_t>(width) * height);

    for (std::uint32_t y = 0U; y < height; ++y)
    {
        for (std::uint32_t x = 0U; x < width; ++x)
        {
            const std::size_t slot =
                static_cast<std::size_t>(y) * width + x;
            mesh.vertices[slot] = make_vertex(
                static_cast<float>(x),
                static_cast<float>(y),
                heights[slot],
                min_height,
                max_height);
        }
    }

    mesh.indices.reserve(
        static_cast<std::size_t>(width - 1U) *
        static_cast<std::size_t>(height - 1U) * 6U);

    for (std::uint32_t y = 0U; y + 1U < height; ++y)
    {
        for (std::uint32_t x = 0U; x + 1U < width; ++x)
        {
            const bool valid =
                sample_valid(valid_patches, x, y, width, height, patch_size) &&
                sample_valid(valid_patches, x + 1U, y, width, height, patch_size) &&
                sample_valid(valid_patches, x, y + 1U, width, height, patch_size) &&
                sample_valid(valid_patches, x + 1U, y + 1U, width, height, patch_size);

            if (!valid)
            {
                continue;
            }

            const std::uint32_t a = y * width + x;
            const std::uint32_t b = a + 1U;
            const std::uint32_t c = a + width;
            const std::uint32_t d = c + 1U;

            mesh.indices.push_back(a);
            mesh.indices.push_back(b);
            mesh.indices.push_back(d);
            mesh.indices.push_back(a);
            mesh.indices.push_back(d);
            mesh.indices.push_back(c);
        }
    }

    return mesh;
}
}
