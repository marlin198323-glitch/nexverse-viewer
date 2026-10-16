/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/terrain_detail_retry.hpp"
#include "nexverse/render/terrain_layer_coverage.hpp"
#include <cstdint>
#include <string>
namespace nexverse::ui
{
[[nodiscard]] inline std::string terrain_recovery_status(
    const nexverse::assets::TerrainDetailRetry& retry,
    std::uint32_t active_gpu_layers,bool terrain_drawn,bool textured)
{
    const std::string base=nexverse::render::terrain_material_status(
        terrain_drawn,textured,active_gpu_layers);
    if(!retry.initialized)return base+" | Material wartet";
    if(active_gpu_layers>=4U)return base;
    if(retry.attempts>=retry.max_attempts)
        return base+" | Retry-Limit erreicht";
    if(retry.attempts==0U)return base+" | Nachladen bereit";
    return base+" | Versuch "+std::to_string(retry.attempts)+
        "/"+std::to_string(retry.max_attempts);
}
}
