/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include <atomic>
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
    std::size_t accepted_events=0U;
    std::size_t polls=0U;
    std::size_t rediscoveries=0U;
};
using RegionEventSink=std::function<void(const EventQueueEvent&)>;
[[nodiscard]] RegionEventQueueStats poll_region_event_queue(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view region_seed,std::uint64_t issued_epoch,
    const FirestormRegionLease& lease,const std::atomic_bool& stop,
    const RegionEventSink& on_event,
    std::size_t max_polls=0U);
}
