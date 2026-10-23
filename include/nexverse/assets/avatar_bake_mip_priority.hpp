/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bakes.hpp"
#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/assets/gpu_staging_budget.hpp"
#include "nexverse/assets/asset_gpu_budget.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <optional>
#include <algorithm>
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
// A bounded in-memory rescue of an already decoded CURRENT own-body
// texture which cannot be staged at the pinned full mip. No new HTTP,
// retry reset, extra texture slots or non-transactional GPU eviction.
// Always prefer the original texture when it fits.
[[nodiscard]] inline std::optional<std::uint8_t>
choose_self_avatar_budget_mip_rescue(
    std::uint32_t width,std::uint32_t height,
    std::uint8_t requested_mip,
    std::size_t resident_textures,std::size_t resident_meshes,
    std::size_t shared_gpu_budget,std::size_t texture_family_budget,
    const std::vector<std::string>& priority,
    const std::unordered_map<std::string,std::size_t>& resident_bytes,
    std::string_view incoming_id)
{
    if(width==0U||height==0U||width>4096U||height>4096U||
       requested_mip>=3U||incoming_id.empty())return std::nullopt;
    const auto family=effective_gpu_family_budget(
        texture_family_budget,shared_gpu_budget,resident_meshes);
    const auto fits=[&](std::uint32_t w,std::uint32_t h)
    {
        const auto bytes=gpu_rgba8_mip_bytes(w,h);
        return bytes>0U &&
            gpu_staging_budget(resident_textures,resident_meshes,
                               bytes,shared_gpu_budget).fits &&
            plan_texture_byte_budget(priority,resident_bytes,
                                     incoming_id,bytes,family).fits;
    };
    if(fits(width,height))return std::nullopt;
    // Maximum additional downgrade is 3 steps across the whole mip
    // ladder. The first quality that fits wins.
    for(std::uint8_t extra=1U;extra<=3U-requested_mip;++extra)
    {
        width=(width+1U)/2U;
        height=(height+1U)/2U;
        if(fits(width,height))return extra;
    }
    return std::nullopt;
}
// Hold the accepted smaller GPU mip briefly. Otherwise the automatic
// full-quality pin immediately requests mip0 again every 750ms and
// thrashes the shared GPU/HTTP path. Retry full quality after 60s, or
// reset immediately on the next region/material epoch.
class SelfAvatarMipRescueCooldown
{
public:
    using Clock=std::chrono::steady_clock;
    void enter(std::uint64_t epoch)
    {
        if(epoch_==epoch)return;
        epoch_=epoch;
        floor_.clear();
    }
    void admitted(std::string_view id,std::uint8_t mip,
                  Clock::time_point now)
    {
        if(epoch_==0U||id.empty()||mip==0U||mip>3U)return;
        // At most 3 current own-body IDs plus short-lived outfit changes.
        if(floor_.size()>=16U&&!floor_.contains(std::string{id}))
            floor_.erase(floor_.begin());
        floor_[std::string{id}]={mip,now+std::chrono::seconds{60}};
    }
    [[nodiscard]] std::uint8_t minimum_mip(
        std::string_view id,Clock::time_point now) const
    {
        const auto it=floor_.find(std::string{id});
        return it==floor_.end()||now>=it->second.until
            ?0U:it->second.level;
    }
private:
    struct Entry
    {
        std::uint8_t level=0U;
        Clock::time_point until{};
    };
    std::uint64_t epoch_=0U;
    std::unordered_map<std::string,Entry> floor_;
};

}
