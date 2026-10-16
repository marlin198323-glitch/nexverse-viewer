/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
namespace nexverse::render
{
// GPU texture material for CURRENT region generation, not a network
// download count. Terrain geometry and material revisions are independent.
[[nodiscard]] constexpr bool terrain_detail_gpu_ready(
    bool uploaded,std::uint32_t texture_generation,
    std::uint32_t terrain_generation,
    std::uint32_t texture_material_revision,
    std::uint32_t terrain_material_revision) noexcept
{
    return uploaded && texture_generation==terrain_generation &&
        texture_material_revision==terrain_material_revision;
}
}
