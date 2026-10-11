/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/assets/asset_view_priority.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nexverse::assets
{
struct MeshDistanceSample
{
    std::string id;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float diameter = 1.0f;
};

struct MeshResidencyPlan
{
    std::vector<std::string> prioritized_ids;
    std::unordered_set<std::string> desired_ids;
    std::vector<std::string> eviction_ids;
};

// Keep nearby mesh assets resident without permanently blocking new
// objects after reaching the GPU budget. A small preference for loaded
// assets prevents cache churn when the camera barely moves.
[[nodiscard]] inline MeshResidencyPlan plan_mesh_residency(
    const std::vector<MeshDistanceSample>& samples,
    float x, float y, float z,
    const std::unordered_set<std::string>& loaded,
    std::size_t budget = 256U,
    float keep_loaded_bonus_m = 24.0f,
    const AssetCameraHint* camera = nullptr)
{
    MeshResidencyPlan plan;
    if (budget == 0U) {
        plan.eviction_ids.assign(loaded.begin(), loaded.end());
        std::sort(plan.eviction_ids.begin(), plan.eviction_ids.end());
        return plan;
    }

    std::unordered_map<std::string, double> min_distance;
    min_distance.reserve(samples.size());
    for (const auto& item : samples)
    {
        if (item.id.empty() || !std::isfinite(item.x) ||
            !std::isfinite(item.y) || !std::isfinite(item.z))
            continue;
        const double dx = static_cast<double>(item.x) - x;
        const double dy = static_cast<double>(item.y) - y;
        const double dz = static_cast<double>(item.z) - z;
        const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (!std::isfinite(distance) || !std::isfinite(item.diameter)) continue;
        // Projected-size heuristic: larger meshes remain visually
        // significant at a longer distance. This is priority only,
        // not a fabricated mesh LOD level.
        const double diameter = std::clamp(
            static_cast<double>(item.diameter), 0.5, 16.0);
        const double weighted = distance/std::sqrt(diameter)*
            (camera ? view_priority_multiplier(
                *camera,item.x,item.y,item.z) : 1.0);
        const auto it = min_distance.find(item.id);
        if (it == min_distance.end())
            min_distance.emplace(item.id, weighted);
        else if (weighted < it->second)
            it->second = weighted;
    }

    std::vector<std::pair<std::string, double>> ranked;
    ranked.reserve(min_distance.size());
    for (const auto& [id, distance] : min_distance)
    {
        const double bonus = loaded.contains(id)
            ? std::max(0.0f, keep_loaded_bonus_m) : 0.0f;
        ranked.emplace_back(id, std::max(0.0, distance - bonus));
    }
    std::sort(ranked.begin(), ranked.end(),
        [](const auto& a, const auto& b)
        {
            if (a.second != b.second) return a.second < b.second;
            return a.first < b.first;
        });

    plan.prioritized_ids.reserve(std::min(budget, ranked.size()));
    plan.desired_ids.reserve(std::min(budget, ranked.size()));
    for (std::size_t i = 0U; i < std::min(budget, ranked.size()); ++i)
    {
        plan.prioritized_ids.push_back(ranked[i].first);
        plan.desired_ids.insert(ranked[i].first);
    }
    for (const auto& id : loaded)
        if (!plan.desired_ids.contains(id))
            plan.eviction_ids.push_back(id);
    std::sort(plan.eviction_ids.begin(), plan.eviction_ids.end());
    return plan;
}
}
