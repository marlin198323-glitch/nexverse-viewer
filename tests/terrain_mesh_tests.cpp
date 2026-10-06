#include "nexverse/render/terrain_mesh.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    constexpr std::uint32_t width = 32U;
    constexpr std::uint32_t height = 16U;
    constexpr std::uint32_t patch_size = 16U;

    std::vector<float> heights(
        static_cast<std::size_t>(width) * height,
        20.0f);

    for (std::uint32_t y = 0U; y < height; ++y)
    {
        for (std::uint32_t x = 0U; x < width; ++x)
        {
            heights[static_cast<std::size_t>(y) * width + x] =
                20.0f + static_cast<float>(x) * 0.1f;
        }
    }

    std::vector<std::uint8_t> valid{1U, 0U};

    const auto partial = nexverse::render::build_terrain_mesh(
        heights,
        valid,
        width,
        height,
        patch_size);

    if (partial.vertices.size() !=
            static_cast<std::size_t>(width) * height ||
        partial.indices.empty())
    {
        std::cerr << "Partial terrain mesh generation failed\n";
        return 1;
    }

    const std::size_t partial_triangles =
        partial.indices.size() / 3U;

    valid[1U] = 1U;

    const auto complete = nexverse::render::build_terrain_mesh(
        heights,
        valid,
        width,
        height,
        patch_size);

    if (complete.indices.size() <= partial.indices.size())
    {
        std::cerr << "Progressive terrain mesh did not grow\n";
        return 2;
    }

    const std::size_t expected_triangles =
        static_cast<std::size_t>(width - 1U) *
        static_cast<std::size_t>(height - 1U) *
        2U;

    if (complete.indices.size() / 3U != expected_triangles)
    {
        std::cerr << "Complete terrain mesh triangle count mismatch\n";
        return 3;
    }

    if (partial_triangles >= expected_triangles)
    {
        std::cerr << "Partial terrain unexpectedly covered full region\n";
        return 4;
    }

    std::cout << "Terrain mesh tests passed\n";
    return 0;
}
