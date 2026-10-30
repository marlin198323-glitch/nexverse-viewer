/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/mesh_loader.hpp"
#include "nexverse/assets/mesh_residency.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// Shared predicate used by BOTH cache-first and network fallback planners.
// A resident asset at the requested LOD is ALREADY satisfied. Reuploading
// the same mesh used to exhaust the 16ms GPU upload gate and cause hundreds
// of false LOD swaps per minute when the disk cache was warm.
[[nodiscard]] inline bool mesh_gpu_request_needed(
    const std::string& id,MeshLod target,
    const std::unordered_map<std::string,MeshLod>& gpu_lods,
    const std::unordered_set<std::string>& gpu_ids,
    std::size_t gpu_slots)
{
    if(const auto current=gpu_lods.find(id);
       current!=gpu_lods.end() && current->second==target)
        return false;
    return gpu_ids.contains(id)||gpu_ids.size()<gpu_slots;
}

// Selection by apparent-size proxy (distance / sqrt(max prim dimension)).
// The closest instance controls LOD when many objects reuse one mesh UUID.
// Hysteresis prevents oscillation near distance thresholds.
[[nodiscard]] inline std::unordered_map<std::string, MeshLod> plan_mesh_lods(
    const std::vector<MeshDistanceSample>& samples,
    float x, float y, float z,
    const std::unordered_map<std::string, MeshLod>& previous = {},
    bool detailed=false)
{
    std::unordered_map<std::string, double> distance;
    for (const auto& s : samples)
    {
        if (s.id.empty() || !std::isfinite(s.x) || !std::isfinite(s.y) ||
            !std::isfinite(s.z) || !std::isfinite(s.diameter) ||
            !std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
            continue;
        const double dx = static_cast<double>(s.x) - x;
        const double dy = static_cast<double>(s.y) - y;
        const double dz = static_cast<double>(s.z) - z;
        const double d = std::hypot(dx, dy, dz) /
            std::sqrt(std::clamp(static_cast<double>(s.diameter),0.5,16.0));
        if (!std::isfinite(d)) continue;
        auto [it, created] = distance.try_emplace(s.id,d);
        if (!created && d < it->second) it->second = d;
    }
    std::unordered_map<std::string, MeshLod> result;
    result.reserve(distance.size());
    for (const auto& [id,d] : distance)
    {
        // DETAIL profile uses source-provided high/medium LODs for
        // nearby geometry, while the original BALANCED profile remains
        // available for low-memory hardware.
        MeshLod lod = detailed
            ?(d < 45.0 ? MeshLod::high :
              d < 110.0 ? MeshLod::medium :
              d < 210.0 ? MeshLod::low : MeshLod::lowest)
            :(d < 20.0 ? MeshLod::high :
              d < 55.0 ? MeshLod::medium :
              d < 120.0 ? MeshLod::low : MeshLod::lowest);
        if (const auto it=previous.find(id); it!=previous.end())
        {
            switch (it->second)
            {
            case MeshLod::high:
                if (d < (detailed?53.0:24.0)) lod = MeshLod::high;
                break;
            case MeshLod::medium:
                if (d >= (detailed?38.0:16.0) &&
                    d < (detailed?126.0:66.0)) lod = MeshLod::medium;
                break;
            case MeshLod::low:
                if (d >= (detailed?98.0:44.0) &&
                    d < (detailed?238.0:144.0)) lod = MeshLod::low;
                break;
            case MeshLod::lowest:
                if (d >= (detailed?185.0:96.0)) lod = MeshLod::lowest;
                break;
            }
        }
        result.emplace(id,lod);
    }
    return result;
}
}
