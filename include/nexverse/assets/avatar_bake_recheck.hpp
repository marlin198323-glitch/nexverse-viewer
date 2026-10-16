/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_failure.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_set>

namespace nexverse::assets
{
// An explicit local request to reevaluate current, authentic self-bake
// texture priorities. It never clears the shared failure/backoff schedule,
// never invents an AvatarAppearance, and never uploads a client bake.
struct AvatarBakeRecheck
{
    using Clock=std::chrono::steady_clock;
    std::uint64_t epoch=0U;
    unsigned requests=0U;
    Clock::time_point retry_after{};
    static constexpr unsigned max_requests=3U;
    static constexpr auto interval=std::chrono::seconds{12};

    void enter(std::uint64_t next_epoch)
    {
        if(epoch==next_epoch)return;
        epoch=next_epoch;
        requests=0U;
        retry_after={};
    }
    [[nodiscard]] bool allowed(bool authenticated,bool lease_valid,
        bool has_cap,bool in_flight,bool has_ready_missing,
        Clock::time_point now) const noexcept
    {
        return epoch!=0U && authenticated && lease_valid && has_cap &&
            !in_flight && has_ready_missing && requests<max_requests &&
            now>=retry_after;
    }
    [[nodiscard]] bool request(bool authenticated,bool lease_valid,
        bool has_cap,bool in_flight,bool has_ready_missing,
        Clock::time_point now)
    {
        if(!allowed(authenticated,lease_valid,has_cap,in_flight,
                    has_ready_missing,now))return false;
        ++requests;
        retry_after=now+interval;
        return true;
    }
};
// Only authoritative, non-placeholder three server bake IDs are inputs.
// Shares exactly the normal GetTexture failure schedule: manual recheck
// cannot bypass transient/invalid/missing-asset cooldowns.
[[nodiscard]] inline std::size_t avatar_bake_ready_missing(
    const std::array<std::string,3>& ids,
    const std::unordered_set<std::string>& gpu,
    const AssetRetrySchedule& schedule,
    AssetRetrySchedule::Clock::time_point now)
{
    std::size_t total=0U;
    std::unordered_set<std::string> unique;
    for(const auto& id:ids)
        if(!id.empty() &&
           unique.insert(id).second &&
           id!="00000000-0000-0000-0000-000000000000" &&
           std::find(nexverse::protocol::baked_placeholders.begin(),
                     nexverse::protocol::baked_placeholders.end(),id)==
               nexverse::protocol::baked_placeholders.end() &&
           !gpu.contains(id) && schedule.ready(id,now))
            ++total;
    return total;
}
}
