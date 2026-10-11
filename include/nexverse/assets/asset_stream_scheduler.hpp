/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_request_priority.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// Frame-independent request pacing. This limits main-thread GPU upload
// pressure from fast RAM/Disk hits as well as the network.
class AssetRequestPacer
{
public:
    using Clock = std::chrono::steady_clock;
    explicit AssetRequestPacer(
        std::chrono::milliseconds interval = std::chrono::milliseconds{125})
        : interval_(interval) {}
    [[nodiscard]] bool ready(Clock::time_point now) const noexcept
    {
        return !armed_ || now >= next_;
    }
    void dispatched(Clock::time_point now) noexcept
    {
        armed_ = true;
        next_ = now + interval_;
    }
    void reset() noexcept {armed_=false;next_={};}
private:
    std::chrono::milliseconds interval_;
    bool armed_=false;
    Clock::time_point next_{};
};

// Per-region tracking prevents repeated prefetch probes for the same UUID,
// and rejects stale worker results from an earlier region epoch.
class TexturePrefetchScope
{
public:
    void enter(std::uint64_t epoch)
    {
        if (!initialized_ || epoch_ != epoch)
        {
            initialized_=true;
            epoch_=epoch;
            attempted_.clear();
        }
    }
    [[nodiscard]] std::uint64_t epoch() const noexcept {return epoch_;}
    [[nodiscard]] bool current(std::uint64_t epoch) const noexcept
    {
        return initialized_ && epoch_==epoch;
    }
    [[nodiscard]] std::vector<std::string> candidates(
        const std::vector<std::string>& priority,
        const std::unordered_set<std::string>& resident,
        const std::unordered_set<std::string>& failed,
        std::size_t limit = 4U) const
    {
        std::vector<std::string> result;
        std::unordered_set<std::string> seen;
        for (const auto& id : priority)
        {
            if (result.size()>=limit)break;
            if (id.empty() || id==null_texture_asset_id ||
                !seen.insert(id).second || attempted_.contains(id) ||
                resident.contains(id) || failed.contains(id))
                continue;
            result.push_back(id);
        }
        return result;
    }
    void dispatched(const std::string& id)
    {
        if (!id.empty())attempted_.insert(id);
    }
    [[nodiscard]] std::size_t attempted() const noexcept
    {return attempted_.size();}
private:
    bool initialized_=false;
    std::uint64_t epoch_=0U;
    std::unordered_set<std::string> attempted_;
};
}
