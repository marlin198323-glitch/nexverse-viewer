/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_failure.hpp"
#include "nexverse/assets/texture_request_priority.hpp"
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
struct AssetRequestChoice
{
    std::string id;
    std::string retry_key;
    bool is_retry = false;
};

// Existing distance/view/particle ranking remains authoritative inside each
// group. Choose a ready retry every fourth dispatch so new scene assets
// cannot monopolize the sole loader while old failures keep retrying.
class AssetRequestFairness
{
public:
    using Clock=std::chrono::steady_clock;
    [[nodiscard]] std::optional<AssetRequestChoice> choose(
        const std::vector<std::string>& priority,
        const std::unordered_set<std::string>& satisfied,
        const std::unordered_set<std::string>& deferred,
        const AssetRetrySchedule& retry,Clock::time_point now,
        const std::function<std::string(const std::string&)>& key_for = {},
        const std::function<bool(const std::string&)>& eligible = {}) const
    {
        std::optional<AssetRequestChoice> fresh;
        std::optional<AssetRequestChoice> repeat;
        std::unordered_set<std::string> seen;
        for(const auto& id:priority)
        {
            if(id.empty()||id==null_texture_asset_id ||
               !seen.insert(id).second||satisfied.contains(id)||
               (eligible&&!eligible(id)))continue;
            const auto key=key_for?key_for(id):id;
            if(key.empty()||deferred.contains(key)||!retry.ready(key,now))
                continue;
            const bool old=retry.failed(key);
            if(old && !repeat)repeat=AssetRequestChoice{id,key,true};
            if(!old && !fresh)fresh=AssetRequestChoice{id,key,false};
            if(fresh&&repeat)break;
        }
        if(dispatches_%4U==3U && repeat)return repeat;
        return fresh?fresh:repeat;
    }
    void dispatched() noexcept {++dispatches_;}
    void reset() noexcept {dispatches_=0U;}
    [[nodiscard]] std::size_t dispatched_count() const noexcept
    {return dispatches_;}
private:
    std::size_t dispatches_=0U;
};
}
