/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/terrain_detail_retry.hpp"
#include "nexverse/render/terrain_layer_coverage.hpp"
#include <cstdint>
#include <string>
#include <array>
namespace nexverse::ui
{
[[nodiscard]] inline std::string terrain_recovery_status(
    const nexverse::assets::TerrainDetailRetry& retry,
    std::uint32_t active_gpu_layers,bool terrain_drawn,bool textured,
    bool in_flight=false,bool capability_available=true,
    nexverse::assets::TerrainDetailRetry::Clock::time_point now=
        nexverse::assets::TerrainDetailRetry::Clock::now())
{
    const std::string base=nexverse::render::terrain_material_status(
        terrain_drawn,textured,active_gpu_layers);
    if(!retry.initialized)return base+" | Material wartet";
    if(active_gpu_layers>=4U)return base;
    if(!capability_available)return base+" | GetTexture-CAP fehlt";
    if(in_flight)return base+" | Download aktiv, Versuch "+
        std::to_string(retry.attempts)+"/"+
        std::to_string(retry.max_attempts);
    if(retry.attempts>=retry.max_attempts)
    {
        const auto wait=retry.retry_wait_seconds(now);
        if(wait>0U)return base+" | Retry-Limit erreicht; warten "+
            std::to_string(wait)+"s";
        if(retry.manual_restarts>=retry.max_manual_restarts)
            return base+" | Retry-Limit erreicht; manuell ausgeschoepft";
        return base+" | Retry-Limit erreicht; manuell [T] verfuegbar";
    }
    if(retry.attempts==0U)return base+" | Nachladen bereit";
    const auto wait=retry.retry_wait_seconds(now);
    return base+" | Versuch "+std::to_string(retry.attempts)+
        "/"+std::to_string(retry.max_attempts)+
        (wait>0U?" | Pause "+std::to_string(wait)+"s":"");
}
[[nodiscard]] inline std::string terrain_recovery_layers(
    const nexverse::assets::TerrainDetailRetry& retry,
    const std::array<std::string,4>& ids)
{
    std::string result="Server-Terrain: ";
    for(std::size_t i=0U;i<ids.size();++i)
    {
        if(i>0U)result+=" | ";
        result+="L"+std::to_string(i+1U)+":";
        if(ids[i].empty()||ids[i]=="00000000-0000-0000-0000-000000000000")
            result+="nicht gesetzt";
        else
            result+=retry.loaded[i]?"geladen":"fehlt";
    }
    return result;
}
}
