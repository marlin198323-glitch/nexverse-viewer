/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include <atomic>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

namespace nexverse::protocol
{
// Region-bound EventQueueGet worker. Only an explicitly verified current
// seed can supply the queue; acknowledgements never cross a CAP URL change.
struct RegionEventQueueStats
{
    bool advertised=false;
    // Every result, including discovery failure, is bound to the exact
    // region epoch so a stale worker cannot disarm a new region worker.
    std::uint64_t issued_epoch=0U;
    std::size_t accepted_events=0U;
    std::size_t polls=0U;
    std::size_t rediscoveries=0U;
};
// Render-thread restart gate. A failed first seed discovery used to strand
// EventQueueGet for the entire region until teleport. Retry only after
// a bounded cooldown; stale workers never reset the current region gate.
class RegionEventRestartGate
{
public:
    using Clock=std::chrono::steady_clock;
    using Time=Clock::time_point;
    [[nodiscard]] bool can_start(std::uint64_t epoch,
                                 std::uint64_t login_epoch,Time now) const noexcept
    {
        return epoch!=0U&&epoch!=login_epoch&&epoch!=started_epoch_&&
               (epoch!=retry_epoch_||now>=retry_after_);
    }
    void started(std::uint64_t epoch) noexcept
    {
        if(retry_epoch_!=epoch)
        {
            retry_epoch_=epoch;
            failures_=0U;
            retry_after_=Time{};
        }
        started_epoch_=epoch;
    }
    // false means stale / wrong region; true means current worker was
    // processed, whether it had a valid endpoint or not.
    [[nodiscard]] bool completed(const RegionEventQueueStats& result,Time now) noexcept
    {
        if(result.issued_epoch==0U||result.issued_epoch!=started_epoch_)
            return false;
        if(result.advertised)
            return true; // healthy worker ended: do not respawn blindly
        started_epoch_=0U;
        failures_=std::min(failures_+1U,5U);
        retry_epoch_=result.issued_epoch;
        // 5, 10, 20, 40, 60s; no frame-by-frame reconnect storm.
        const auto seconds=std::min(60U,5U*(1U<<(failures_-1U)));
        retry_after_=now+std::chrono::seconds{seconds};
        return true;
    }
    [[nodiscard]] unsigned retry_failures() const noexcept {return failures_;}
private:
    std::uint64_t started_epoch_=0U, retry_epoch_=0U;
    unsigned failures_=0U;
    Time retry_after_{};
};
using RegionEventSink=std::function<void(const EventQueueEvent&)>;
[[nodiscard]] RegionEventQueueStats poll_region_event_queue(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view region_seed,std::uint64_t issued_epoch,
    const FirestormRegionLease& lease,const std::atomic_bool& stop,
    const RegionEventSink& on_event,
    std::size_t max_polls=0U);
}
