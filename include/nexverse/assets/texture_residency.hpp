/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/assets/texture_request_priority.hpp"
#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
struct TextureResidencyPlan
{
    std::vector<std::string> preferred_ids;
    std::unordered_set<std::string> desired_ids;
    std::vector<std::string> eviction_ids;
};

// Active self/nearby avatar baked BODY assets are held ahead of particle
// and background materials. Priority is bounded by the same shared GPU
// budget, and region transitions naturally revoke stale preferred IDs.
[[nodiscard]] inline TextureResidencyPlan plan_texture_residency(
    const std::vector<std::string>& particle_ids,
    const std::vector<std::string>& object_ids,
    const std::unordered_set<std::string>& loaded,
    std::size_t budget = 512U,
    const std::vector<std::string>& avatar_body_ids = {})
{
    TextureResidencyPlan plan;
    plan.preferred_ids.reserve(std::min(budget,
        avatar_body_ids.size()+particle_ids.size()+object_ids.size()));
    plan.desired_ids.reserve(budget);
    const auto visit=[&](const std::vector<std::string>& ids)
    {
        for(const auto& id:ids)
        {
            if(plan.preferred_ids.size()>=budget)break;
            if(id.empty() || id==null_texture_asset_id ||
               !plan.desired_ids.insert(id).second)continue;
            plan.preferred_ids.push_back(id);
        }
    };
    visit(avatar_body_ids);
    visit(particle_ids);
    visit(object_ids);
    for (const auto& id:loaded)
        if(!plan.desired_ids.contains(id))
            plan.eviction_ids.push_back(id);
    std::sort(plan.eviction_ids.begin(),plan.eviction_ids.end());
    return plan;
}
}
