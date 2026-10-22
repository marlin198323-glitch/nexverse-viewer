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
    unsigned requests=0U; // Current authenticated nonempty body source.
    unsigned total_requests=0U; // Entire region epoch (anti-churn bound).
    Clock::time_point retry_after{};
    static constexpr unsigned max_requests=3U;
    static constexpr unsigned max_total_requests=12U;
    static constexpr auto interval=std::chrono::seconds{12};

    void enter(std::uint64_t next_epoch)
    {
        if(epoch==next_epoch)return;
        epoch=next_epoch;
        requests=0U;
        total_requests=0U;
        retry_after={};
        current_body_={};
        has_body_=false;
    }
    // A genuinely different, authenticated server-selected outfit may
    // receive its own 3 manual replan attempts without a region teleport.
    // Empty/placeholder/withdrawn TE cannot reset the old count. A second
    // per-region cap and unchanged 12-second throttle bound outfit churn.
    void observe_source(std::uint64_t next_epoch,bool authenticated,
        bool lease_valid,const std::array<std::string,3U>& source)
    {
        enter(next_epoch);
        if(epoch==0U||!authenticated||!lease_valid)return;
        auto next=source;
        for(auto& id:next)
            if(id=="00000000-0000-0000-0000-000000000000"||
               std::find(protocol::baked_placeholders.begin(),
                         protocol::baked_placeholders.end(),id)!=
                   protocol::baked_placeholders.end())
                id.clear();
        if(std::all_of(next.begin(),next.end(),
                       [](const auto& id){return id.empty();}))return;
        if(!has_body_||next!=current_body_)
        {
            current_body_=std::move(next);
            has_body_=true;
            requests=0U;
            // Deliberately preserve retry_after and total_requests.
        }
    }
    [[nodiscard]] bool allowed(bool authenticated,bool lease_valid,
        bool has_cap,bool in_flight,bool has_ready_missing,
        Clock::time_point now) const noexcept
    {
        return epoch!=0U && authenticated && lease_valid && has_cap &&
            !in_flight && has_ready_missing && requests<max_requests &&
            total_requests<max_total_requests && now>=retry_after;
    }
    [[nodiscard]] bool request(bool authenticated,bool lease_valid,
        bool has_cap,bool in_flight,bool has_ready_missing,
        Clock::time_point now)
    {
        if(!allowed(authenticated,lease_valid,has_cap,in_flight,
                    has_ready_missing,now))return false;
        ++requests;
        ++total_requests;
        retry_after=now+interval;
        return true;
    }
private:
    std::array<std::string,3U> current_body_{};
    bool has_body_=false;
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
