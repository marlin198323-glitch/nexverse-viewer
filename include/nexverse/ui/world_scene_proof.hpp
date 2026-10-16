/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>

namespace nexverse::ui
{
// Read-only evidence of stage progress. Object presence in WorldModel
// and actual GL draw submissions are intentionally separate.
struct WorldSceneProof
{
    bool terrain_available=false;
    std::uint16_t valid_terrain_patches=0U;
    std::size_t world_objects=0U;
    std::uint64_t world_scene_draws=0U;
    std::uint64_t missing_mesh_draws=0U;
};

[[nodiscard]] inline std::string describe_terrain_progress(
    const WorldSceneProof& proof)
{
    if(!proof.terrain_available)
        return "Terrain 0/256 wartet";
    return "Terrain "+std::to_string(std::min(
        std::uint16_t{256},proof.valid_terrain_patches))+"/256 Patches";
}

[[nodiscard]] inline std::string describe_world_draw_progress(
    const WorldSceneProof& proof)
{
    if(proof.world_scene_draws>0U)
        return "GL "+std::to_string(proof.world_scene_draws)+
               " Draw | MeshFehlt "+std::to_string(proof.missing_mesh_draws);
    if(proof.world_objects>0U)
        return "Objekte "+std::to_string(proof.world_objects)+
               " empfangen | GL 0";
    return "Objekte warten | GL 0";
}
}
