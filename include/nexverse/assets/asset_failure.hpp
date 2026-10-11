/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/net/http_transport.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace nexverse::assets
{
enum class AssetFailure : std::uint8_t
{
    none, transient, missing, invalid, configuration
};
[[nodiscard]] constexpr std::string_view asset_failure_name(
    AssetFailure failure) noexcept
{
    switch (failure)
    {
    case AssetFailure::none: return "none";
    case AssetFailure::transient: return "transient";
    case AssetFailure::missing: return "missing";
    case AssetFailure::invalid: return "invalid";
    case AssetFailure::configuration: return "configuration";
    }
    return "unknown";
}
[[nodiscard]] inline AssetFailure classify_http_asset_failure(
    const net::HttpResponse& response) noexcept
{
    if (response.ok()) return AssetFailure::none;
    if (response.status_code == 404L || response.status_code == 410L)
        return AssetFailure::missing;
    if (response.status_code == 408L || response.status_code == 425L ||
        response.status_code == 429L || response.status_code >= 500L ||
        response.status_code == 0L)
        return AssetFailure::transient;
    // Distinguish authentication/configuration from malformed content.
    if (response.status_code == 400L ||
        response.status_code == 401L ||
        response.status_code == 403L)
        return AssetFailure::configuration;
    return AssetFailure::invalid;
}
[[nodiscard]] constexpr AssetFailure merge_asset_failure(
    AssetFailure a,AssetFailure b) noexcept
{
    if (a==AssetFailure::transient || b==AssetFailure::transient)
        return AssetFailure::transient;
    if (a==AssetFailure::configuration ||
        b==AssetFailure::configuration)
        return AssetFailure::configuration;
    if (a==AssetFailure::invalid || b==AssetFailure::invalid)
        return AssetFailure::invalid;
    if (a==AssetFailure::missing || b==AssetFailure::missing)
        return AssetFailure::missing;
    return AssetFailure::none;
}

class AssetRetrySchedule
{
public:
    using Clock=std::chrono::steady_clock;
    // Never suppress a new region's first request based on previous failures.
    void enter(std::uint64_t epoch)
    {
        if (!initialized_ || epoch_ != epoch)
        {
            initialized_=true;
            epoch_=epoch;
            failures_.clear();
        }
    }
    [[nodiscard]] bool ready(std::string_view key,Clock::time_point now) const
    {
        const auto it=failures_.find(std::string{key});
        return it==failures_.end() || now>=it->second.next;
    }
    [[nodiscard]] bool failed(std::string_view key) const
    {
        return failures_.contains(std::string{key});
    }
    void success(std::string_view key)
    {
        failures_.erase(std::string{key});
    }
    [[nodiscard]] std::chrono::milliseconds failure(
        std::string_view key,AssetFailure reason,
        Clock::time_point now)
    {
        if (reason==AssetFailure::none)
        {
            success(key);
            return std::chrono::milliseconds{0};
        }
        if (failures_.size()>=4096U &&
            !failures_.contains(std::string{key}))
            failures_.clear(); // bound memory in large/churning scenes
        auto& entry=failures_[std::string{key}];
        if (entry.kind != reason) entry.consecutive=0U;
        entry.kind=reason;
        entry.consecutive=std::min(12U,entry.consecutive+1U);
        std::chrono::milliseconds delay{0};
        switch(reason)
        {
        case AssetFailure::transient:
            // Short exponential attempts, then a 5-minute circuit break.
            if (entry.consecutive<=3U)
                delay=std::chrono::seconds{2U<< (entry.consecutive-1U)};
            else delay=std::chrono::minutes{5};
            break;
        case AssetFailure::missing: delay=std::chrono::minutes{10};break;
        case AssetFailure::invalid: delay=std::chrono::minutes{30};break;
        case AssetFailure::configuration:
            delay=std::chrono::minutes{5};break;
        case AssetFailure::none: break;
        }
        entry.next=now+delay;
        return delay;
    }
    [[nodiscard]] std::size_t count() const noexcept
    {
        return failures_.size();
    }
    [[nodiscard]] std::size_t count_kind(AssetFailure kind) const noexcept
    {
        std::size_t n=0;
        for(const auto& [key,entry]:failures_)
        {
            (void)key;
            if (entry.kind==kind) ++n;
        }
        return n;
    }
private:
    struct Entry
    {
        AssetFailure kind=AssetFailure::none;
        unsigned consecutive=0U;
        Clock::time_point next{};
    };
    std::uint64_t epoch_=0U;
    bool initialized_=false;
    std::unordered_map<std::string,Entry> failures_;
};
}
