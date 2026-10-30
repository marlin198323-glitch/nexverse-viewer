/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/assets/asset_gpu_budget.hpp"
#include "nexverse/assets/gpu_staging_budget.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_set>
#include <utility>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace nexverse::assets
{
// Last-resort quality reduction for a real, downloaded NON-avatar scene
// image. A smaller original mip is preferable to an empty gray material.
// Uses no additional network request and retains ALL normal GPU budget,
// region-epoch and transactional upload gates. Priority/byte evictions remain
// authoritative; never produce a fabricated texture.
[[nodiscard]] inline std::optional<std::uint8_t>
choose_scene_texture_budget_mip_rescue(
    std::uint32_t width,std::uint32_t height,std::uint8_t current_mip,
    std::size_t gpu_texture_bytes,std::size_t gpu_mesh_bytes,
    std::size_t shared_budget,std::size_t family_budget,
    const std::vector<std::string>& prioritized,
    const std::unordered_map<std::string,std::size_t>& resident,
    std::string_view incoming)
{
    if(width==0U||height==0U||width>4096U||height>4096U||
       current_mip>=6U||incoming.empty())return std::nullopt;
    const auto family=effective_gpu_family_budget(
        family_budget,shared_budget,gpu_mesh_bytes);
    const auto fits=[&](std::uint32_t w,std::uint32_t h)
    {
        const auto bytes=gpu_rgba8_mip_bytes(w,h);
        return bytes!=0U &&
            gpu_staging_budget(gpu_texture_bytes,gpu_mesh_bytes,
                               bytes,shared_budget).fits &&
            plan_texture_byte_budget(prioritized,resident,incoming,
                                     bytes,family).fits;
    };
    if(fits(width,height))return std::nullopt;
    for(std::uint8_t extra=1U;current_mip+extra<=6U;++extra)
    {
        width=(width+1U)/2U;
        height=(height+1U)/2U;
        if(fits(width,height))return extra;
    }
    return std::nullopt;
}
// When the scene cache reaches its fixed byte budget, favor actual
// coverage over oversized far-away materials. Choose one already
// GPU-resident image for a one-step quality reduction. It is NOT
// evicted here: the normal texture worker reloads its original pixels
// from RAM/disk, and GL replaces the old handle atomically.
[[nodiscard]] inline std::optional<std::pair<std::string,std::uint8_t>>
choose_scene_texture_compaction(
    const std::vector<std::string>& priority,
    const std::unordered_map<std::string,std::size_t>& resident_bytes,
    const std::unordered_map<std::string,std::uint8_t>& resident_mips,
    const std::unordered_map<std::string,std::uint8_t>& desired_mips,
    const std::unordered_set<std::string>& excluded,
    std::size_t used_bytes,std::size_t budget_bytes,
    std::size_t waiting_assets)
{
    if(budget_bytes==0U||used_bytes<budget_bytes-budget_bytes/20U||
       waiting_assets==0U)return std::nullopt;
    constexpr std::size_t min_candidate=512U*1024U;
    // Reverse global priority. Do not sacrifice near, visible avatars,
    // local clothing, or particle effects merely to improve coverage.
    for(auto it=priority.rbegin();it!=priority.rend();++it)
    {
        if(excluded.contains(*it))continue;
        const auto bytes=resident_bytes.find(*it);
        const auto mip=resident_mips.find(*it);
        const auto target=desired_mips.find(*it);
        if(bytes==resident_bytes.end()||bytes->second<min_candidate||
           mip==resident_mips.end()||mip->second>=5U||
           target==desired_mips.end()||
           target->second!=mip->second)
            continue;
        return std::pair{*it,static_cast<std::uint8_t>(mip->second+1U)};
    }
    return std::nullopt;
}

// An admitted fallback is not immediately upscaled on the next 750ms
// planner tick: retry original quality after a bounded 90s dwell. Store only
// IDs, never their URLs or pixels; clear on a new region epoch.
class SceneTextureMipRescueCooldown
{
public:
    using Clock=std::chrono::steady_clock;
    void enter(std::uint64_t epoch)
    {
        if(epoch_==epoch)return;
        epoch_=epoch;
        accepted_.clear();
        pending_.clear();
    }
    void requested(std::string_view id,std::uint8_t mip,
                   Clock::time_point now)
    {
        if(epoch_==0U||id.empty()||mip==0U||mip>6U)return;
        // An HTTP failure can outlive its 8-second reservation. Expired
        // requests must never exhaust the bounded map and block all later
        // scene textures on a long-running authenticated session.
        for(auto it=pending_.begin();it!=pending_.end();)
            if(now>=it->second.until)it=pending_.erase(it);
            else ++it;
        if(!pending_.contains(std::string{id})&&pending_.size()>=8U)
            return;
        pending_.insert_or_assign(std::string{id},
            Entry{mip,now+std::chrono::seconds{8}});
    }
    [[nodiscard]] bool confirm_requested(std::string_view id,
                      std::uint8_t mip,Clock::time_point now)
    {
        const auto it=pending_.find(std::string{id});
        if(it==pending_.end()||it->second.mip!=mip||
           now>=it->second.until)return false;
        pending_.erase(it);
        admitted(id,mip,now);
        return true;
    }
    [[nodiscard]] std::size_t pending_count(Clock::time_point now) const
    {
        std::size_t active=0U;
        for(const auto& [id,entry]:pending_)
        {
            (void)id;
            if(now<entry.until)++active;
        }
        return active;
    }
    void admitted(std::string_view id,std::uint8_t mip,
                  Clock::time_point now)
    {
        if(epoch_==0U||id.empty()||mip==0U||mip>6U)return;
        const std::string key{id};
        if(!accepted_.contains(key)&&accepted_.size()>=256U)
            accepted_.erase(accepted_.begin());
        pending_.erase(key);
        accepted_.insert_or_assign(key,Entry{
            mip,now+std::chrono::seconds{90}});
    }
    [[nodiscard]] std::uint8_t minimum_mip(
        std::string_view id,Clock::time_point now) const
    {
        std::uint8_t floor=0U;
        if(const auto it=accepted_.find(std::string{id});
           it!=accepted_.end()&&now<it->second.until)
            floor=it->second.mip;
        if(const auto it=pending_.find(std::string{id});
           it!=pending_.end()&&now<it->second.until)
            floor=std::max(floor,it->second.mip);
        return floor;
    }
    [[nodiscard]] std::size_t count() const noexcept
    {return accepted_.size();}
private:
    struct Entry{std::uint8_t mip;Clock::time_point until;};
    std::uint64_t epoch_=0U;
    std::unordered_map<std::string,Entry> accepted_;
    std::unordered_map<std::string,Entry> pending_;
};
}
