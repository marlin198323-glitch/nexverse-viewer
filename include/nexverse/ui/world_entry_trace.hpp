/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/ui/world_entry_stage.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
namespace nexverse::ui
{
// No identities, URLs, raw server strings, session IDs or timestamps.
// Fixed-size process-memory evidence, reset when the user is unauthenticated.
struct WorldEntryTraceItem
{
    WorldEntryStage stage=WorldEntryStage::login;
    std::uint32_t dwell_seconds=0;
};
class WorldEntryTrace
{
public:
    static constexpr std::size_t capacity=5U;
    void reset() noexcept
    {
        items_.fill({});
        size_=0U;
        since_={};
    }
    void observe(WorldEntryStage stage,
                 std::chrono::steady_clock::time_point now) noexcept
    {
        if(size_!=0U && items_[size_-1U].stage==stage)
        {
            items_[size_-1U].dwell_seconds=elapsed(now,since_);
            return;
        }
        if(size_==capacity)
        {
            for(std::size_t i=1U;i<capacity;++i)items_[i-1U]=items_[i];
            size_=capacity-1U;
        }
        items_[size_++]={stage,0U};
        since_=now;
    }
    [[nodiscard]] std::size_t size() const noexcept{return size_;}
    [[nodiscard]] WorldEntryTraceItem at(std::size_t i) const noexcept
    {
        return i<size_?items_[i]:WorldEntryTraceItem{};
    }
    [[nodiscard]] std::array<WorldEntryTraceItem,capacity> snapshot() const
        noexcept
    {
        return items_;
    }
private:
    [[nodiscard]] static std::uint32_t elapsed(
        std::chrono::steady_clock::time_point now,
        std::chrono::steady_clock::time_point since) noexcept
    {
        if(now<since)return 0U;
        const auto sec=
            std::chrono::duration_cast<std::chrono::seconds>(now-since);
        return static_cast<std::uint32_t>(
            std::clamp<std::int64_t>(sec.count(),0,3600));
    }
    std::array<WorldEntryTraceItem,capacity> items_{};
    std::size_t size_=0U;
    std::chrono::steady_clock::time_point since_{};
};
}
