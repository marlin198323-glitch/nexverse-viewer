/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/region_event_queue.hpp"
#include "nexverse/protocol/event_queue_recovery.hpp"
#include <algorithm>
#include <chrono>
#include <thread>
#include <string>

namespace nexverse::protocol
{
RegionEventQueueStats poll_region_event_queue(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view seed,std::uint64_t epoch,
    const FirestormRegionLease& lease,const std::atomic_bool& stop,
    const RegionEventSink& sink,std::size_t max_polls)
{
    RegionEventQueueStats stats;
    const auto active=[&]{return !stop.load()&&lease.matches(epoch);};
    if(!transport||!active()||seed.empty()||seed.size()>4096U)
        return stats;
    CapabilityService service{transport};
    const auto lookup=[&]()
    {
        return service.discover(seed,{"EventQueueGet"});
    };
    auto discovery=lookup();
    if(!active()||!discovery.success)return stats;
    auto queue=discovery.capabilities.find("EventQueueGet");
    if(queue==discovery.capabilities.end()||queue->second.empty())
        return stats;
    std::string uri=queue->second;
    stats.advertised=true;
    std::optional<std::int32_t> ack;
    EventQueueRecovery recovery;
    const auto pause=[&](std::chrono::milliseconds time)
    {
        while(active()&&time.count()>0)
        {
            const auto step=std::min(time,std::chrono::milliseconds{100});
            std::this_thread::sleep_for(step);
            time-=step;
        }
    };
    while(active()&&(max_polls==0U||stats.polls<max_polls))
    {
        ++stats.polls;
        auto batch=service.poll_event_queue(uri,ack,false);
        if(!active())break;
        if(batch.idle)
        {
            recovery.on_contact();
            pause(std::chrono::milliseconds{150});
            continue;
        }
        if(batch.success)
        {
            // A duplicate acknowledgement/batch may be retransmitted.
            // Don't dispatch again in the same queue epoch.
            if(batch.id && ack && *batch.id==*ack)
                continue;
            ack=batch.id;
            recovery.on_contact();
            for(const auto& event:batch.events)
            {
                if(!active())break;
                if(sink)sink(event);
                ++stats.accepted_events;
            }
            continue;
        }
        const auto decision=recovery.on_failure(batch.expired);
        if(decision.refresh_seed&&active())
        {
            ++stats.rediscoveries;
            const auto refreshed=lookup();
            if(!active())break;
            if(refreshed.success)
            {
                const auto found=refreshed.capabilities.find("EventQueueGet");
                if(found!=refreshed.capabilities.end()&&!found->second.empty()&&
                   found->second!=uri)
                {
                    uri=found->second;
                    ack.reset();
                    recovery.on_contact();
                }
            }
        }
        pause(decision.pause);
    }
    return stats;
}
}
