/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>

namespace nexverse::protocol
{
struct EventQueueRetryDecision
{
    std::uint32_t failures = 0U;
    bool refresh_seed = false;
    std::chrono::milliseconds pause{0};
};

// OpenSimulator uses HTTP 502 for an ordinary empty long-poll. Only
// transport/protocol failures count toward reconnect and backoff.
class EventQueueRecovery
{
public:
    [[nodiscard]] std::uint32_t failures() const noexcept { return failures_; }

    void on_contact() noexcept
    {
        failures_ = 0U;
    }

    [[nodiscard]] EventQueueRetryDecision on_failure(
        bool endpoint_expired) noexcept
    {
        failures_ = std::min<std::uint32_t>(failures_ + 1U, 100000U);

        // Refresh immediately when the capability expires. For transient
        // failures, refresh only periodically instead of hammering seed CAPS.
        const bool refresh = endpoint_expired || failures_ % 3U == 0U;

        // Bounded exponential-ish backoff: 1, 2, 4, 8, 12 seconds.
        const auto pause = std::chrono::seconds{
            std::min<std::uint32_t>(
                failures_ <= 4U ? (1U << (failures_ - 1U)) : 12U,
                12U)};
        return {failures_, refresh,
                std::chrono::duration_cast<std::chrono::milliseconds>(pause)};
    }

private:
    std::uint32_t failures_ = 0U;
};
}
