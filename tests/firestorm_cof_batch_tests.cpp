/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_cof_batch_add.hpp"
#include <chrono>
#include <iostream>
using namespace nexverse::protocol;
constexpr auto C="11111111-1111-1111-1111-111111111111";
constexpr auto B="22222222-2222-2222-2222-222222222222";
constexpr auto X="33333333-3333-3333-3333-333333333333";
constexpr auto Y="44444444-4444-4444-4444-444444444444";
constexpr auto Z="55555555-5555-5555-5555-555555555555";
constexpr auto L0="66666666-6666-6666-6666-666666666666";
constexpr auto L1="77777777-7777-7777-7777-777777777777";
constexpr auto L2="88888888-8888-8888-8888-888888888888";
constexpr auto L3="99999999-9999-9999-9999-999999999999";
int main()
{
    using Clock=CofBatchAdd::Clock;
    const auto t=Clock::time_point{std::chrono::seconds{900}};
    CofChangePlan plan;
    plan.readiness=CofPlanReadiness::preview_ready;
    plan.cof_version=10U;
    plan.desired_targets={B,X,Y,Z};
    plan.add={{X,"First",18},{Y,"Second",18},{Z,"Third",18}};
    CofBatchAdd batch;
    if(!batch.stage(plan,C,8,t)||batch.total()!=3U||batch.done()!=0U||
       batch.phase()!=CofBatchPhase::awaiting_consent||
       batch.stage(plan,C,8,t))return 1;
    if(batch.confirm(plan,C,8,t+std::chrono::seconds{1},0U))return 2;
    if(batch.phase()!=CofBatchPhase::stale_region)return 3;
    if(!batch.stage(plan,C,8,t))return 4;
    if(batch.confirm(plan,C,9,t+std::chrono::seconds{1},1U))return 5;
    if(!batch.stage(plan,C,8,t))return 6;
    auto changed=plan;changed.cof_version=11U;
    if(batch.confirm(changed,C,8,t+std::chrono::seconds{1},2U))return 7;
    if(!batch.stage(plan,C,8,t))return 8;
    if(batch.confirm(plan,C,8,t+std::chrono::seconds{21},3U))return 9;
    if(!batch.stage(plan,C,8,t))return 10;
    auto first=batch.confirm(plan,C,8,t+std::chrono::seconds{2},3U);
    if(!first||first->linked_item_id!=X||first->callback_id!=3U||
       batch.phase()!=CofBatchPhase::awaiting_queue)return 11;
    batch.queue_result(true,t+std::chrono::seconds{2});
    if(!batch.awaiting_readback())return 12;
    const std::vector<OutfitInventoryItem> after_first={
        {L0,B,"Base",24,18},{L1,X,"First",24,18}};
    if(batch.observe(8,10U,&after_first,t+std::chrono::seconds{4},4U))
        return 13;
    if(batch.done()!=0U||!batch.awaiting_readback())return 14;
    auto second=batch.observe(8,11U,&after_first,
                              t+std::chrono::seconds{5},4U);
    if(!second||second->linked_item_id!=Y||batch.done()!=1U||
       batch.phase()!=CofBatchPhase::awaiting_queue)return 15;
    batch.queue_result(true,t+std::chrono::seconds{5});
    const std::vector<OutfitInventoryItem> after_second={
        {L0,B,"Base",24,18},{L1,X,"First",24,18},
        {L2,Y,"Second",24,18}};
    auto third=batch.observe(8,12U,&after_second,
                             t+std::chrono::seconds{7},5U);
    if(!third||third->linked_item_id!=Z||batch.done()!=2U)return 16;
    batch.queue_result(true,t+std::chrono::seconds{7});
    const std::vector<OutfitInventoryItem> after_third={
        {L0,B,"Base",24,18},{L1,X,"First",24,18},
        {L2,Y,"Second",24,18},{L3,Z,"Third",24,18}};
    if(batch.observe(8,13U,&after_third,
                     t+std::chrono::seconds{9},6U)||
       batch.phase()!=CofBatchPhase::confirmed_links||
       batch.done()!=3U||batch.active())return 17;
    // An out-of-band COF mutation forbids the NEXT packet.
    if(!batch.stage(plan,C,8,t))return 18;
    if(!batch.confirm(plan,C,8,t+std::chrono::seconds{1},7U))return 19;
    batch.queue_result(true,t+std::chrono::seconds{1});
    if(batch.observe(8,11U,&after_second,
                     t+std::chrono::seconds{2},8U)||
       batch.phase()!=CofBatchPhase::partial_or_unknown||
       batch.stage(plan,C,8,t+std::chrono::seconds{3}))return 20;
    batch.reset("fresh owner COF reread");
    if(!batch.stage(plan,C,8,t))return 21;
    if(!batch.confirm(plan,C,8,t+std::chrono::seconds{1},9U))return 22;
    batch.queue_result(false,t+std::chrono::seconds{1});
    if(batch.phase()!=CofBatchPhase::partial_or_unknown)return 23;
    batch.reset();
    if(!batch.stage(plan,C,8,t))return 24;
    if(!batch.confirm(plan,C,8,t+std::chrono::seconds{1},10U))return 25;
    batch.queue_result(true,t+std::chrono::seconds{1});
    if(batch.observe(8,10U,&after_first,
                     t+std::chrono::seconds{20},11U)||
       batch.phase()!=CofBatchPhase::partial_or_unknown)return 26;
    batch.reset();
    if(!batch.stage(plan,C,8,t))return 27;
    if(!batch.confirm(plan,C,8,t+std::chrono::seconds{1},12U))return 28;
    batch.queue_result(true,t+std::chrono::seconds{1});
    if(batch.observe(9,11U,&after_first,
                     t+std::chrono::seconds{2},13U)||
       batch.phase()!=CofBatchPhase::stale_region)return 29;
    changed=plan;changed.remove.push_back({L0,B});
    if(batch.stage(changed,C,8,t))return 30;
    changed=plan;changed.add.resize(5U);
    if(batch.stage(changed,C,8,t))return 31;
    changed=plan;changed.add.resize(1U);
    if(batch.stage(changed,C,8,t))return 32;
    std::cout<<"COF 2-4 sequentially added links with per-step owner readback, partial-failure stop and no deletion/auto-resend OK\n";
    return 0;
}
