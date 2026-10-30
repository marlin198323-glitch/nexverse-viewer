#include "nexverse/protocol/region_event_queue.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
using namespace nexverse::protocol;
namespace
{
struct MockHttp final : nexverse::net::IHttpTransport
{
    int seed_calls=0;
    int queue_calls=0;
    bool expire=false;
    bool expire_second=false;
    bool same_uri_after_refresh=false;
    bool fail_first_seed=false;
    bool fail_second_seed=false;
    bool missing_second_seed=false;
    bool missing=false;
    std::vector<int> acks;
    bool duplicate_forever=false;
    bool missing_id_forever=false;
    std::vector<int> scripted_ids;
    nexverse::net::HttpResponse get(std::string_view,
        std::size_t,std::chrono::milliseconds,std::chrono::milliseconds) override
    { return {405,{},"unsupported"}; }
    nexverse::net::HttpResponse post(std::string_view url,
        std::string_view,std::string_view body,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {
        if(url=="https://sim.test/seed")
        {
            ++seed_calls;
            if(fail_first_seed&&seed_calls==1)
                return {503,{},"temporary seed unavailable"};
            if(fail_second_seed&&seed_calls==2)
                return {503,{},"temporary rediscovery unavailable"};
            return {200,serialize_llsd_xml(LlsdValue::map({
                {"EventQueueGet",LlsdValue::string(
                    (missing||(missing_second_seed&&seed_calls==2))?
                        "":(same_uri_after_refresh||seed_calls==1?
                        "https://sim.test/queue1":"https://sim.test/queue2"))}
            })),{}};
        }
        if(url=="https://sim.test/queue1"||
           url=="https://sim.test/queue2")
        {
            ++queue_calls;
            const auto parsed=parse_llsd_xml(body);
            if(!parsed.ok())return {400,{},"invalid"};
            const auto* ack=parsed.value->find("ack");
            acks.push_back(ack&&ack->type==LlsdType::integer
                ?static_cast<int>(ack->integer_value):-1);
            if((expire&&queue_calls==1)||
               (expire_second&&queue_calls==2))return {404,{},""};
            const int id=!scripted_ids.empty()?
                scripted_ids[std::min<std::size_t>(
                    static_cast<std::size_t>(queue_calls-1),
                    scripted_ids.size()-1U)]:
                duplicate_forever?17:(queue_calls==1?17:
                queue_calls==2?17:18);
            return {200,serialize_llsd_xml(LlsdValue::map({
                {"id",missing_id_forever?LlsdValue{}:
                    LlsdValue::integer(id)},
                {"events",LlsdValue::array({LlsdValue::map({
                    {"message",LlsdValue::string("BulkUpdateInventory")},
                    {"body",LlsdValue::map({
                        {"signal",LlsdValue::integer(id)}
                    })}
                })})}
            })),{}};
        }
        return {404,{},"unrecognized url"};
    }
};
}
int main()
{
    auto mock=std::make_shared<MockHttp>();
    FirestormRegionLease lease;
    const auto epoch=lease.enter(111U,1U,"https://sim.test/seed");
    std::atomic_bool stop{false};
    std::vector<std::string> seen;
    auto receive=[&](const EventQueueEvent& event)
    { seen.push_back(event.message); };
    const auto result=poll_region_event_queue(
        mock,"https://sim.test/seed",epoch,lease,stop,receive,3U);
    if(!result.advertised||result.polls!=3U||
       result.accepted_events!=2U||mock->queue_calls!=3||
       mock->seed_calls!=1||seen.size()!=2U||
       mock->acks!=std::vector<int>({-1,17,17}))
        return 1;
    seen.clear();
    const auto next=lease.enter(222U,2U,"https://sim.test/new");
    const auto stale=poll_region_event_queue(
        mock,"https://sim.test/seed",epoch,lease,stop,receive,3U);
    if(stale.advertised||stale.polls!=0U||mock->seed_calls!=1)
        return 2;
    const auto after=poll_region_event_queue(
        mock,"https://sim.test/seed",next,lease,stop,
        [&](const EventQueueEvent& event)
        {
            seen.push_back(event.message);
            (void)lease.enter(333U,2U,"https://sim.test/other");
        },3U);
    if(!after.advertised||after.accepted_events!=1U||
       after.polls!=1U||seen.size()!=1U)
        return 3;
    stop.store(true);
    const auto cancelled=poll_region_event_queue(
        mock,"https://sim.test/seed",lease.epoch(),lease,
        stop,receive,3U);
    if(cancelled.advertised||cancelled.polls!=0U)return 4;
    // On HTTP 404, source CAPS rediscovery may rebind a new URI, with
    // its own acknowledgement sequence, and no arbitrary remote URL.
    stop.store(false);
    auto refreshed_mock=std::make_shared<MockHttp>();
    refreshed_mock->expire=true;
    auto last_epoch=lease.enter(444U,2U,"https://sim.test/again");
    const auto refreshed=poll_region_event_queue(
        refreshed_mock,"https://sim.test/seed",last_epoch,lease,
        stop,receive,2U);
    if(refreshed.rediscoveries!=1U||refreshed_mock->seed_calls!=2||
       refreshed_mock->queue_calls!=2||!refreshed.advertised||
       refreshed_mock->acks!=std::vector<int>({-1,-1}))
        return 5;
    // A misbehaving/looping simulator must not cause unbounded
    // duplicate delivery or a busy-spin on immediate HTTP replies.
    auto replay_mock=std::make_shared<MockHttp>();
    replay_mock->duplicate_forever=true;
    last_epoch=lease.enter(555U,2U,"https://sim.test/replay");
    const auto tick=std::chrono::steady_clock::now();
    const auto replay=poll_region_event_queue(
        replay_mock,"https://sim.test/seed",last_epoch,lease,
        stop,receive,4U);
    const auto elapsed=std::chrono::steady_clock::now()-tick;
    if(replay.polls!=4U||replay.accepted_events!=1U||
       replay.replayed_batches!=3U||
       replay_mock->acks!=std::vector<int>({-1,17,17,17})||
       elapsed<std::chrono::milliseconds{300})return 6;
    // Malformed HTTP-200 replies without an ACK cursor never dispatch
    // a teleport/inventory event.  Backoff/retry is bounded.
    auto no_id_mock=std::make_shared<MockHttp>();
    no_id_mock->missing_id_forever=true;
    last_epoch=lease.enter(556U,2U,"https://sim.test/no-id");
    const auto broken=poll_region_event_queue(
        no_id_mock,"https://sim.test/seed",last_epoch,lease,
        stop,receive,2U);
    if(broken.accepted_events!=0U||broken.polls!=2U||
       broken.failed_polls!=2U||
       no_id_mock->acks!=std::vector<int>({-1,-1}))
       return 7;
    // Expiration may return a new queue incarnation at the identical
    // URI. Sending its old ACK strands the new instance.
    auto same_uri_mock=std::make_shared<MockHttp>();
    same_uri_mock->expire_second=true;
    same_uri_mock->same_uri_after_refresh=true;
    last_epoch=lease.enter(557U,2U,"https://sim.test/same-uri");
    const auto reincarnated=poll_region_event_queue(
        same_uri_mock,"https://sim.test/seed",last_epoch,lease,
        stop,receive,3U);
    if(reincarnated.polls!=3U||reincarnated.rediscoveries!=1U||
       same_uri_mock->seed_calls!=2||
       same_uri_mock->acks!=std::vector<int>({-1,17,-1}))
       return 8;

    // A missing/503 EventQueueGet after an HTTP 404 expires the old
    // queue. The worker must TERMINATE and let the region restart gate
    // rediscover the seed; continuing to poll the dead URL indefinitely
    // used to strand CAPS and cross-region teleport notifications.
    auto lost_seed=std::make_shared<MockHttp>();
    lost_seed->expire_second=true;
    lost_seed->fail_second_seed=true;
    last_epoch=lease.enter(565U,2U,"https://sim.test/expired-seed-503");
    const auto failed_refresh=poll_region_event_queue(
        lost_seed,"https://sim.test/seed",last_epoch,lease,
        stop,receive,8U);
    if(failed_refresh.advertised||failed_refresh.polls!=2U||
       failed_refresh.failed_polls!=1U||
       failed_refresh.rediscoveries!=1U||
       failed_refresh.expired_rediscovery_failures!=1U||
       lost_seed->queue_calls!=2||
       lost_seed->seed_calls!=2)
        return 24;
    // Fresh worker and same lease can now rediscover and resume.
    lost_seed->expire_second=false;
    const auto after_retry=poll_region_event_queue(
        lost_seed,"https://sim.test/seed",last_epoch,lease,
        stop,receive,1U);
    if(!after_retry.advertised||after_retry.polls!=1U||
       after_retry.expired_rediscovery_failures!=0U||
       lost_seed->seed_calls!=3)
        return 25;
    auto missing_caps=std::make_shared<MockHttp>();
    missing_caps->expire_second=true;
    missing_caps->missing_second_seed=true;
    last_epoch=lease.enter(566U,2U,"https://sim.test/expired-no-eq");
    const auto missing_refresh=poll_region_event_queue(
        missing_caps,"https://sim.test/seed",last_epoch,lease,
        stop,receive,8U);
    if(missing_refresh.advertised||missing_refresh.polls!=2U||
       missing_refresh.rediscoveries!=1U||
       missing_refresh.expired_rediscovery_failures!=1U||
       missing_caps->queue_calls!=2)
        return 26;

    RegionEventRestartGate gate;
    const auto origin=RegionEventRestartGate::Clock::now();
    if(!gate.can_start(1001U,11U,origin))return 9;
    gate.started(1001U);
    if(gate.can_start(1001U,11U,origin))return 10;
    RegionEventQueueStats unavailable;
    unavailable.issued_epoch=1001U;
    if(!gate.completed(unavailable,origin))return 11;
    if(gate.retry_failures()!=1U ||
       gate.can_start(1001U,11U,origin+std::chrono::seconds{4})||
       !gate.can_start(1001U,11U,origin+std::chrono::seconds{5}))
       return 12;
    gate.started(1001U);
    if(!gate.completed(unavailable,origin+std::chrono::seconds{5})||
       gate.can_start(1001U,11U,origin+std::chrono::seconds{14})||
       !gate.can_start(1001U,11U,origin+std::chrono::seconds{15})||
       gate.can_start(11U,11U,origin+std::chrono::seconds{120}))
       return 13;
    // A new region ignores a stale region's retry cooldown.
    if(!gate.can_start(1002U,11U,origin+std::chrono::seconds{6}))
       return 14;
    gate.started(1002U);
    if(gate.completed(unavailable,origin+std::chrono::seconds{6})||
       gate.can_start(1002U,11U,origin+std::chrono::seconds{6}))
       return 15;
    RegionEventQueueStats healthy;
    healthy.issued_epoch=1002U;
    healthy.advertised=true;
    if(!gate.completed(healthy,origin+std::chrono::seconds{7})||
       gate.can_start(1002U,11U,origin+std::chrono::seconds{120}))
       return 16;
    // An expired queue with failed rediscovery is restartable, but
    // only after the existing bounded region-scoped cooldown.
    const auto expired_epoch=1003U;
    if(!gate.can_start(expired_epoch,11U,origin))return 27;
    gate.started(expired_epoch);
    RegionEventQueueStats expired=failed_refresh;
    expired.issued_epoch=expired_epoch;
    if(!gate.completed(expired,origin) ||
       gate.can_start(expired_epoch,11U,origin+std::chrono::seconds{4})||
       !gate.can_start(expired_epoch,11U,origin+std::chrono::seconds{5}))
        return 28;
    auto no_seed=std::make_shared<MockHttp>();
    no_seed->fail_first_seed=true;
    const auto missing_epoch=lease.enter(558U,2U,"https://sim.test/fail-seed");
    auto temporary=poll_region_event_queue(
        no_seed,"https://sim.test/seed",missing_epoch,lease,stop,receive,1U);
    if(temporary.advertised||temporary.issued_epoch!=missing_epoch||
       temporary.polls!=0U||no_seed->seed_calls!=1)
       return 17;
    const auto recovered=poll_region_event_queue(
        no_seed,"https://sim.test/seed",missing_epoch,lease,stop,receive,1U);
    if(!recovered.advertised||recovered.issued_epoch!=missing_epoch||
       recovered.polls!=1U||no_seed->seed_calls!=2)
       return 18;
    // Reordered replay after a newer ACK: 17,18,17,19 must deliver
    // exactly three batches, and fourth POST must still ACK 18.
    auto reordered=std::make_shared<MockHttp>();
    reordered->scripted_ids={17,18,17,19};
    last_epoch=lease.enter(559U,2U,"https://sim.test/replay-older");
    const auto reordered_result=poll_region_event_queue(
        reordered,"https://sim.test/seed",last_epoch,lease,
        stop,receive,4U);
    if(reordered_result.accepted_events!=3U ||
       reordered_result.replayed_batches!=1U ||
       reordered->acks!=std::vector<int>({-1,17,18,18}))
        return 19;
    // Bounded history must not grow forever; oldest IDs age out.
    EventQueueRecentBatches recent;
    for(int i=0;i<70;++i)
        if(!recent.first_delivery(i))return 20;
    if(recent.size()!=EventQueueRecentBatches::capacity||
       recent.first_delivery(69)||!recent.first_delivery(0))
        return 21;
    recent.reset();
    if(recent.size()!=0U||!recent.first_delivery(69))
        return 22;
    // A new Queue at the same URL is a new incarnation and can reuse
    // a previously accepted ID. Historical ACKs never cross HTTP 404.
    auto replay_after_404=std::make_shared<MockHttp>();
    replay_after_404->expire_second=true;
    replay_after_404->same_uri_after_refresh=true;
    replay_after_404->scripted_ids={17,17,17};
    last_epoch=lease.enter(560U,2U,"https://sim.test/expired-replay");
    const auto after_rebind=poll_region_event_queue(
        replay_after_404,"https://sim.test/seed",last_epoch,lease,
        stop,receive,3U);
    if(after_rebind.accepted_events!=2U ||
       replay_after_404->acks!=std::vector<int>({-1,17,-1}))
        return 23;
    std::cout<<"Region EventQueue nonadjacent replay and 64-ID "
                "bounded ACK history PASS\n";
    std::cout<<"Region EventQueue same-URI 404 ACK reset and "
                "bounded initial-seed retry PASS\n";
    std::cout<<"Region-scoped EventQueue duplicate replay bounded "
                "without CPU hot loop\n";
    std::cout<<"Region-scoped EventQueue source, ack reset,"
                " deduplication and lease cancellation passed\n";
    return 0;
}
