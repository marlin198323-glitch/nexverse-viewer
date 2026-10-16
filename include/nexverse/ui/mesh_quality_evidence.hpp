/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/mesh_lod_policy.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
namespace nexverse::ui
{
struct MeshQualityEvidence
{
    std::uint32_t high_requested=0U;
    std::uint32_t medium_requested=0U;
    std::uint32_t low_requested=0U;
    std::uint32_t lowest_requested=0U;
    std::uint32_t matched_gpu_lod=0U;
    std::uint32_t resident_gpu_lod=0U;
    [[nodiscard]] std::uint32_t total_requested() const noexcept
    {
        return high_requested+medium_requested+
            low_requested+lowest_requested;
    }
};
[[nodiscard]] inline MeshQualityEvidence inspect_mesh_quality(
    const std::unordered_map<std::string,nexverse::assets::MeshLod>& requested,
    const std::unordered_map<std::string,nexverse::assets::MeshLod>& gpu)
{
    MeshQualityEvidence e;
    for(const auto& [id,lod]:requested)
    {
        switch(lod)
        {
        case nexverse::assets::MeshLod::high:++e.high_requested;break;
        case nexverse::assets::MeshLod::medium:++e.medium_requested;break;
        case nexverse::assets::MeshLod::low:++e.low_requested;break;
        case nexverse::assets::MeshLod::lowest:++e.lowest_requested;break;
        }
        if(const auto it=gpu.find(id);it!=gpu.end())
        {
            ++e.resident_gpu_lod;
            // An uploaded LOW is not HIGH even when an object has been
            // counted as "loaded" by UUID. Exact LOD match only.
            if(it->second==lod)++e.matched_gpu_lod;
        }
    }
    return e;
}
[[nodiscard]] inline std::string describe_mesh_quality(
    const MeshQualityEvidence& e)
{
    return "LOD H:"+std::to_string(e.high_requested)+
        " M:"+std::to_string(e.medium_requested)+
        " L:"+std::to_string(e.low_requested)+
        " | GPU "+std::to_string(e.matched_gpu_lod)+"/"+
        std::to_string(e.total_requested());
}
}
