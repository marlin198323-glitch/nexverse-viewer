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
// Per-current authenticated self body source evidence. This inspects
// existing GPU residency and region-epoch failure schedules ONLY. A scene
// asset's HTTP 404 cannot be blamed on Head/Upper/Lower unless its exact
// UUID matches the CURRENT source. Never export those UUIDs to the UI.
enum class SelfBakeGetTextureStage : std::uint8_t
{
    no_server_reference=0, gpu_resident=1, ready_to_retry=2,
    http_missing_backoff=3, transient_backoff=4,
    configuration_backoff=5, invalid_backoff=6
};
[[nodiscard]] inline std::array<SelfBakeGetTextureStage,3U>
inspect_self_bake_gettexture(
    const std::array<std::string,3U>& body,
    const std::unordered_set<std::string>& resident,
    const AssetRetrySchedule& retry,
    AssetRetrySchedule::Clock::time_point now)
{
    std::array<SelfBakeGetTextureStage,3U> out{};
    for(std::size_t i=0U;i<body.size();++i)
    {
        const auto& id=body[i];
        if(id.empty()||
           id=="00000000-0000-0000-0000-000000000000"||
           std::find(protocol::baked_placeholders.begin(),
                     protocol::baked_placeholders.end(),id)!=
               protocol::baked_placeholders.end())
            continue;
        if(resident.contains(id))
        {
            out[i]=SelfBakeGetTextureStage::gpu_resident;
            continue;
        }
        // When the backoff expired, don't claim this is a NEW HTTP 404:
        // the asset is now eligible for the standard shared retry plan.
        if(retry.ready(id,now))
        {
            out[i]=SelfBakeGetTextureStage::ready_to_retry;
            continue;
        }
        switch(retry.failure_kind(id))
        {
        case AssetFailure::missing:
            out[i]=SelfBakeGetTextureStage::http_missing_backoff;break;
        case AssetFailure::configuration:
            out[i]=SelfBakeGetTextureStage::configuration_backoff;break;
        case AssetFailure::invalid:
            out[i]=SelfBakeGetTextureStage::invalid_backoff;break;
        case AssetFailure::transient:
            out[i]=SelfBakeGetTextureStage::transient_backoff;break;
        case AssetFailure::none:
            out[i]=SelfBakeGetTextureStage::ready_to_retry;break;
        }
    }
    return out;
}
[[nodiscard]] inline const char* self_bake_gettexture_label(
    SelfBakeGetTextureStage stage) noexcept
{
    switch(stage)
    {
    case SelfBakeGetTextureStage::no_server_reference:return "-";
    case SelfBakeGetTextureStage::gpu_resident:return "GPU";
    case SelfBakeGetTextureStage::ready_to_retry:return "bereit";
    case SelfBakeGetTextureStage::http_missing_backoff:return "HTTP404";
    case SelfBakeGetTextureStage::transient_backoff:return "retry";
    case SelfBakeGetTextureStage::configuration_backoff:return "auth/cfg";
    case SelfBakeGetTextureStage::invalid_backoff:return "ungueltig";
    }
    return "?";
}
[[nodiscard]] inline std::string self_bake_gettexture_summary(
    const std::array<SelfBakeGetTextureStage,3U>& states)
{
    return std::string{"Body-GetTexture H:"}+
        self_bake_gettexture_label(states[0])+
        " U:"+self_bake_gettexture_label(states[1])+
        " L:"+self_bake_gettexture_label(states[2]);
}

// Explain the EXACT current avatar renderer source without upgrading
// provisional ObjectUpdate TE into authoritative Low158/COF evidence.
enum class SelfBakeSource : std::uint8_t
{
    none=0, self_low158=1, provisional_object_te=2
};
struct SelfRenderBakeGetTextureEvidence
{
    std::array<SelfBakeGetTextureStage,3U> stages{};
    std::array<SelfBakeSource,3U> sources{};
};
[[nodiscard]] inline SelfRenderBakeGetTextureEvidence
inspect_self_render_bake_gettexture(
    const std::array<std::string,3U>& selected_body,
    const std::array<bool,3U>& from_low158,
    const std::array<bool,3U>& from_object_update,
    const std::unordered_set<std::string>& resident,
    const AssetRetrySchedule& retry,
    AssetRetrySchedule::Clock::time_point now)
{
    SelfRenderBakeGetTextureEvidence out;
    // Invalid/conflicting source flags cannot be used to claim a
    // server-authenticated GPU download. A validated model selection
    // marks each slot by its actual winning source (Low158 precedence).
    std::array<std::string,3U> admitted{};
    for(std::size_t i=0U;i<selected_body.size();++i)
    {
        if(selected_body[i].empty())continue;
        if(from_low158[i])
            out.sources[i]=SelfBakeSource::self_low158;
        else if(from_object_update[i])
            out.sources[i]=SelfBakeSource::provisional_object_te;
        else continue;
        admitted[i]=selected_body[i];
    }
    out.stages=inspect_self_bake_gettexture(admitted,resident,retry,now);
    for(std::size_t i=0U;i<out.stages.size();++i)
        if(out.stages[i]==SelfBakeGetTextureStage::no_server_reference)
            out.sources[i]=SelfBakeSource::none;
    return out;
}
[[nodiscard]] inline const char* self_render_bake_source_label(
    SelfBakeSource source) noexcept
{
    switch(source)
    {
    case SelfBakeSource::none:return "-";
    case SelfBakeSource::self_low158:return "L";
    case SelfBakeSource::provisional_object_te:return "O";
    }
    return "?";
}
[[nodiscard]] inline std::string self_render_bake_gettexture_summary(
    const SelfRenderBakeGetTextureEvidence& info)
{
    const auto face=[&info](std::size_t i)
    {
        if(info.sources[i]==SelfBakeSource::none)return std::string{"-"};
        return std::string{self_render_bake_source_label(info.sources[i])}+
            "/"+self_bake_gettexture_label(info.stages[i]);
    };
    return "Body H:"+face(0)+" U:"+face(1)+" L:"+face(2)+
        " (L=Low158 O=ObjTE)";
}

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
