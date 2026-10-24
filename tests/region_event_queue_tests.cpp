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
    bool missing=false;
    std::vector<int> acks;
    bool duplicate_forever=false;
    bool missing_id_forever=false;
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
            return {200,serialize_llsd_xml(LlsdValue::map({
                {"EventQueueGet",LlsdValue::string(
                    missing?"":(seed_calls==1?
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
            if(expire &&queue_calls==1)return {404,{},""};
            const int id=duplicate_forever?17:(queue_calls==1?17:
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
       no_id_mock->acks!=std::vector<int>({-1,-1}))
       return 7;
    std::cout<<"Region-scoped EventQueue duplicate replay bounded "
                "without CPU hot loop\n";
    std::cout<<"Region-scoped EventQueue source, ack reset,"
                " deduplication and lease cancellation passed\n";
    return 0;
}
