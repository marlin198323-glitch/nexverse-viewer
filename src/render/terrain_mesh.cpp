/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/terrain_mesh.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

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

float bilinear(
    const std::array<float, 4>& values,
    float u,
    float v)
{
    const float bottom =
        values[0] + (values[2] - values[0]) * u;
    const float top =
        values[1] + (values[3] - values[1]) * u;
    return bottom + (top - bottom) * v;
}

std::array<float, 3> mix_color(
    const std::array<float, 3>& a,
    const std::array<float, 3>& b,
    float amount)
{
    return {
        a[0] + (b[0] - a[0]) * amount,
        a[1] + (b[1] - a[1]) * amount,
        a[2] + (b[2] - a[2]) * amount};
}

nexverse::render::TerrainVertex make_vertex(
    float x,
    float y,
    float z,
    std::uint32_t width,
    std::uint32_t height,
    const std::array<float, 4>& start_heights,
    const std::array<float, 4>& height_ranges)
{
    const float u =
        width > 1U ? x / static_cast<float>(width - 1U) : 0.0f;
    const float v =
        height > 1U ? y / static_cast<float>(height - 1U) : 0.0f;

    const float start = bilinear(start_heights, u, v);
    const float range =
        std::max(0.001f, bilinear(height_ranges, u, v));
    const float t =
        std::clamp((z - start) / range, 0.0f, 1.0f);

    constexpr std::array<float, 3> layer0{0.33f, 0.28f, 0.18f};
    constexpr std::array<float, 3> layer1{0.17f, 0.43f, 0.18f};
    constexpr std::array<float, 3> layer2{0.38f, 0.36f, 0.24f};
    constexpr std::array<float, 3> layer3{0.62f, 0.62f, 0.64f};

    std::array<float, 3> color{};
    if (t < 0.333333f)
    {
        color = mix_color(layer0, layer1, t / 0.333333f);
    }
    else if (t < 0.666667f)
    {
        color = mix_color(
            layer1,
            layer2,
            (t - 0.333333f) / 0.333334f);
    }
    else
    {
        color = mix_color(
            layer2,
            layer3,
            (t - 0.666667f) / 0.333333f);
    }

    return {
        x,
        y,
        z,
        color[0],
        color[1],
        color[2],
        t};
}
}

namespace nexverse::render
{
TerrainMesh build_terrain_mesh(
    std::span<const float> heights,
    std::span<const std::uint8_t> valid_patches,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t patch_size,
    const std::array<float, 4>& start_heights,
    const std::array<float, 4>& height_ranges)
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

    bool any_valid = false;
    for (std::uint32_t y = 0U; y < height && !any_valid; ++y)
    {
        for (std::uint32_t x = 0U; x < width; ++x)
        {
            if (sample_valid(
                    valid_patches,
                    x,
                    y,
                    width,
                    height,
                    patch_size))
            {
                any_valid = true;
                break;
            }
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
                width,
                height,
                start_heights,
                height_ranges);
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
