/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
namespace nexverse::assets
{
// State for ONE simulator terrain-material revision. Fetch only missing
// layers, at most 4 attempts with bounded backoff and no endless polling.
struct TerrainDetailRetry
{
    using Clock=std::chrono::steady_clock;
    std::uint32_t generation=0U,revision=0U;
    bool initialized=false;
    std::uint8_t attempts=0U;
    std::array<bool,4> loaded{};
    Clock::time_point retry_after{};
    static constexpr std::uint8_t max_attempts=4U;
    [[nodiscard]] bool enter(std::uint32_t next_generation,
                              std::uint32_t next_revision)
    {
        if(initialized&&generation==next_generation&&
           revision==next_revision)return false;
        *this={};
        initialized=true;
        generation=next_generation;
        revision=next_revision;
        return true;
    }
    [[nodiscard]] std::array<bool,4> missing(
        const std::array<std::string,4>& ids) const
    {
        std::array<bool,4> result{};
        for(std::size_t i=0;i<4;++i)
            result[i]=!loaded[i] && !ids[i].empty() &&
                ids[i]!="00000000-0000-0000-0000-000000000000";
        return result;
    }
    [[nodiscard]] bool ready(const std::array<std::string,4>& ids,
                             Clock::time_point now) const
    {
        if(!initialized || attempts>=max_attempts || now<retry_after)
            return false;
        const auto pending=missing(ids);
        return std::any_of(pending.begin(),pending.end(),
                           [](bool item){return item;});
    }
    void dispatched(Clock::time_point now)
    {
        if(attempts>=max_attempts)return;
        ++attempts;
        // Retry missing layers after 2s, 5s, 15s. Never busy-spin.
        static constexpr std::array<std::chrono::seconds,4> delays{
            std::chrono::seconds{2},std::chrono::seconds{5},
            std::chrono::seconds{15},std::chrono::seconds{30}};
        retry_after=now+delays[attempts-1U];
    }
    void succeeded(std::size_t layer)
    {
        if(layer<loaded.size())loaded[layer]=true;
    }
    [[nodiscard]] std::uint32_t loaded_count() const
    {
        return static_cast<std::uint32_t>(
            std::count(loaded.begin(),loaded.end(),true));
    }
};
}
