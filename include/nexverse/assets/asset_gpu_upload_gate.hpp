/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>
namespace nexverse::assets
{
// Shared renderer-thread upload gate. Worker futures remain valid/ready
// while an upload is deferred to a later frame.
class AssetGpuUploadGate
{
public:
    using Clock=std::chrono::steady_clock;
    explicit AssetGpuUploadGate(
        std::chrono::milliseconds interval=std::chrono::milliseconds{30})
        : interval_{interval} {}
    [[nodiscard]] bool ready(Clock::time_point now) const noexcept
    {
        return !armed_ || now>=next_;
    }
    void uploaded(Clock::time_point now,std::size_t bytes,
        std::chrono::microseconds submission_time={}) noexcept
    {
        // Adaptive pacing is based on observed CPU submission latency, not
        // an unsupported claim about asynchronous GPU completion.
        const auto micros=std::max<std::int64_t>(
            0,submission_time.count());
        const auto requested=std::chrono::milliseconds{
            std::clamp<std::int64_t>((micros+999)/1000*4,
                interval_.count(),150)};
        effective_interval_=std::max(interval_,requested);
        next_=now+effective_interval_;
        armed_=true;
        ++uploads_;
        bytes_uploaded_+=bytes;
    }
    void reset() noexcept {next_={};armed_=false;effective_interval_=interval_;}
    [[nodiscard]] std::chrono::milliseconds effective_interval() const noexcept
    {return effective_interval_;}
    [[nodiscard]] std::size_t count() const noexcept{return uploads_;}
    [[nodiscard]] std::size_t bytes() const noexcept{return bytes_uploaded_;}
private:
    std::chrono::milliseconds interval_;
    std::chrono::milliseconds effective_interval_{interval_};
    Clock::time_point next_{};
    bool armed_=false;
    std::size_t uploads_=0;
    std::size_t bytes_uploaded_=0;
};

struct TextureSlotAdmission
{
    bool fits=false;
    std::vector<std::string> additional_evictions;
};
// The byte budget can fit while the GPU slot budget is exhausted. Select
// only lower-priority already-resident textures for an additional eviction.
[[nodiscard]] inline TextureSlotAdmission plan_texture_slot_admission(
    const std::vector<std::string>& prioritized,
    const std::unordered_set<std::string>& resident,
    const std::vector<std::string>& planned_byte_evictions,
    const std::string& incoming, std::size_t cap)
{
    TextureSlotAdmission outcome;
    if(resident.contains(incoming))
    {
        outcome.fits=true;
        return outcome;
    }
    std::unordered_set<std::string> removed{
        planned_byte_evictions.begin(),planned_byte_evictions.end()};
    std::size_t occupied=0U;
    for(const auto& id:resident)
        if(!removed.contains(id))++occupied;
    if(occupied<cap)
    {
        outcome.fits=true;
        return outcome;
    }
    const auto target=std::find(prioritized.begin(),prioritized.end(),incoming);
    if(target==prioritized.end())return outcome;
    // The last resident entry *after* the incoming priority rank can go.
    for(auto it=prioritized.rbegin();it!=prioritized.rend();++it)
    {
        if(*it==incoming)break;
        if(resident.contains(*it)&&!removed.contains(*it))
        {
            outcome.fits=true;
            outcome.additional_evictions.push_back(*it);
            return outcome;
        }
    }
    return outcome;
}
}
