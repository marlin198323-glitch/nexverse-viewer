/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bakes.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace nexverse::assets
{
// Self-avatar body may be within the camera near field while its proxy
// distance sample is dominated by third-person camera offset. Pin only
// existing, VALID simulator body IDs to mip0. Respect existing shared
// 256MiB/512-slot admission, GPU failure, retry and epoch policies.
// Never insert a missing server ID into the texture request plan.
[[nodiscard]] inline std::size_t pin_self_avatar_body_full_mip(
    std::unordered_map<std::string,std::uint8_t>& desired_mips,
    const std::array<std::string,3U>& current_self_body_ids,
    bool current_authenticated_region)
{
    if(!current_authenticated_region)return 0U;
    std::size_t changed=0U;
    for(const auto& id:current_self_body_ids)
    {
        if(id.empty())continue;
        bool reserved=false;
        for(const auto placeholder:nexverse::protocol::baked_placeholders)
            if(id==placeholder){reserved=true;break;}
        if(reserved)continue;
        const auto it=desired_mips.find(id);
        if(it==desired_mips.end() || it->second==0U)continue;
        it->second=0U;
        ++changed;
    }
    return changed;
}
}
