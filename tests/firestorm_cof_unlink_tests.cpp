/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_cof_unlink_consent.hpp"
#include <chrono>
#include <iostream>
using namespace nexverse::protocol;
constexpr auto C="11111111-1111-1111-1111-111111111111";
constexpr auto B="22222222-2222-2222-2222-222222222222";
constexpr auto X="33333333-3333-3333-3333-333333333333";
constexpr auto Y="44444444-4444-4444-4444-444444444444";
constexpr auto L0="55555555-5555-5555-5555-555555555555";
constexpr auto L1="66666666-6666-6666-6666-666666666666";
constexpr auto L2="77777777-7777-7777-7777-777777777777";
int main()
{
    using Clock=CofSingleUnlinkConsent::Clock;
    const auto t=Clock::time_point{std::chrono::seconds{1000}};
    CofChangePlan plan;plan.readiness=CofPlanReadiness::preview_ready;
    plan.cof_version=7U;plan.desired_targets={B,X};
    plan.remove.push_back({L2,Y});
    CofSingleUnlinkConsent gate;
    if(!gate.stage(plan,C,6,t)||
       gate.phase()!=CofUnlinkPhase::confirmation_required)return 1;
    if(gate.confirm(plan,C,7,t+std::chrono::seconds{1}))
       return 2;
    if(gate.phase()!=CofUnlinkPhase::stale_region)return 3;
    if(!gate.stage(plan,C,6,t))return 4;
    auto old=plan;old.cof_version=8U;
    if(gate.confirm(old,C,6,t+std::chrono::seconds{1}))return 5;
    if(!gate.stage(plan,C,6,t))return 6;
    if(gate.confirm(plan,C,6,t+std::chrono::seconds{21}))
       return 7;
    if(!gate.stage(plan,C,6,t))return 8;
    auto cmd=gate.confirm(plan,C,6,t+std::chrono::seconds{1});
    if(!cmd||cmd->link_item_id!=L2||!gate.awaiting_server())return 9;
    const std::vector<OutfitInventoryItem> after={
        {L0,B,"Body",24,18},{L1,X,"Shirt",24,18}};
    gate.observe(6,7U,&after,t+std::chrono::seconds{3});
    if(!gate.awaiting_server())return 10;
    gate.observe(6,8U,&after,t+std::chrono::seconds{4});
    if(gate.phase()!=CofUnlinkPhase::verified)return 11;
    if(!gate.stage(plan,C,6,t))return 12;
    cmd=gate.confirm(plan,C,6,t+std::chrono::seconds{1});
    if(!cmd)return 13;
    const std::vector<OutfitInventoryItem> divergent{
        {L0,B,"Body",24,18},{L1,X,"Shirt",24,18},{L2,Y,"Still present",24,18}};
    gate.observe(6,8U,&divergent,t+std::chrono::seconds{3});
    if(gate.phase()!=CofUnlinkPhase::unconfirmed||
       gate.stage(plan,C,6,t+std::chrono::seconds{4}))return 14;
    gate.reset("fresh COF owner CAPS reread");
    if(!gate.stage(plan,C,6,t))return 15;
    cmd=gate.confirm(plan,C,6,t+std::chrono::seconds{1});
    if(!cmd)return 16;
    gate.observe(6,7U,&after,t+std::chrono::seconds{20});
    if(gate.phase()!=CofUnlinkPhase::unconfirmed)return 17;
    gate.reset();
    if(!gate.stage(plan,C,6,t))return 18;
    cmd=gate.confirm(plan,C,6,t+std::chrono::seconds{1});
    if(!cmd)return 19;
    gate.observe(7,8U,&after,t+std::chrono::seconds{2});
    if(gate.phase()!=CofUnlinkPhase::stale_region)return 20;
    auto unsafe=plan;unsafe.add.push_back({X,"Bad",18});
    if(gate.stage(unsafe,C,6,t))return 21;
    unsafe=plan;unsafe.desired_targets.clear();
    if(gate.stage(unsafe,C,6,t))return 22;
    unsafe=plan;unsafe.remove.push_back({L0,B});
    if(gate.stage(unsafe,C,6,t))return 23;
    std::cout<<"COF single-link explicit unlink/degrab consent, no strip last link, owner readback and no retry OK\n";
    return 0;
}
