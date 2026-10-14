/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_cof_link_consent.hpp"
#include <chrono>
#include <iostream>
using namespace nexverse::protocol;
constexpr auto COF="44444444-4444-4444-4444-444444444444";
constexpr auto TARGET="77777777-7777-7777-7777-777777777777";
constexpr auto LINK="88888888-8888-8888-8888-888888888888";
int main()
{
    using Clock=CofSingleLinkConsent::Clock;
    const auto now=Clock::time_point{std::chrono::seconds{1000}};
    CofChangePlan plan;
    plan.readiness=CofPlanReadiness::preview_ready;
    plan.cof_version=18;
    plan.add.push_back({TARGET,"Avatar shirt",18});
    plan.desired_targets.insert(TARGET);
    CofSingleLinkConsent gate;
    if(!gate.stage(plan,COF,43,now)||
       gate.phase()!=CofLinkPhase::confirmation_required||
       gate.awaiting_server())return 1;
    if(gate.confirm(plan,43,COF,now+std::chrono::seconds{2},0U))
        return 2;
    if(gate.phase()!=CofLinkPhase::stale_or_cancelled)return 3;
    if(!gate.stage(plan,COF,43,now))return 4;
    if(gate.confirm(plan,44,COF,now+std::chrono::seconds{1},1U))
        return 5;
    if(!gate.stage(plan,COF,43,now))return 6;
    if(gate.confirm(plan,43,COF,now+std::chrono::seconds{21},1U))
        return 7;
    if(!gate.stage(plan,COF,43,now))return 8;
    auto changed=plan;changed.cof_version=19;
    if(gate.confirm(changed,43,COF,now+std::chrono::seconds{1},1U))
        return 9;
    if(!gate.stage(plan,COF,43,now))return 10;
    changed=plan;changed.remove.push_back({LINK,TARGET});
    if(gate.confirm(changed,43,COF,now+std::chrono::seconds{1},1U))
        return 11;
    if(!gate.stage(plan,COF,43,now))return 12;
    auto send=gate.confirm(plan,43,COF,now+std::chrono::seconds{2},17U);
    if(!send||send->folder_id!=COF||
       send->linked_item_id!=TARGET||send->callback_id!=17U||
       !gate.awaiting_server())return 13;
    if(gate.stage(plan,COF,43,now)||gate.confirm(plan,43,COF,now,1U))
        return 14;
    std::vector<OutfitInventoryItem> current{
        {LINK,TARGET,"Shirt",24,18}};
    gate.observe(43,18U,&current,now+std::chrono::seconds{4});
    if(gate.phase()!=CofLinkPhase::waiting_server)return 15;
    gate.observe(43,19U,&current,now+std::chrono::seconds{4});
    if(gate.phase()!=CofLinkPhase::verified)return 16;
    if(!gate.stage(plan,COF,43,now))return 17;
    send=gate.confirm(plan,43,COF,now+std::chrono::seconds{1},18U);
    if(!send)return 18;
    gate.observe(43,19U,nullptr,now+std::chrono::seconds{20});
    if(gate.phase()!=CofLinkPhase::not_confirmed)return 19;
    // Unknown write outcome must NOT be blindly retried; require a
    // fresh authenticated user-initiated CurrentOutfitFolder readback.
    if(gate.stage(plan,COF,43,now))return 20;
    gate.reset("manual current COF reread completed");
    if(!gate.stage(plan,COF,43,now))return 30;
    send=gate.confirm(plan,43,COF,now+std::chrono::seconds{1},19U);
    if(!send)return 21;
    gate.observe(44,19U,&current,now+std::chrono::seconds{2});
    if(gate.phase()!=CofLinkPhase::stale_or_cancelled)return 22;
    if(!gate.stage(plan,COF,43,now))return 23;
    send=gate.confirm(plan,43,COF,now+std::chrono::seconds{1},20U);
    if(!send)return 24;
    gate.failed_to_queue();
    if(gate.phase()!=CofLinkPhase::stale_or_cancelled)return 25;
    auto unsafe=plan;unsafe.remove.push_back({LINK,TARGET});
    if(gate.stage(unsafe,COF,43,now))return 26;
    unsafe=plan;unsafe.add.push_back({LINK,"Body",18});
    if(gate.stage(unsafe,COF,43,now))return 27;
    unsafe=plan;unsafe.cof_version=0;
    if(gate.stage(unsafe,COF,43,now))return 28;
    unsafe=plan;unsafe.add[0].inventory_type=-4;
    if(gate.stage(unsafe,COF,43,now))return 29;
    std::cout<<"COF explicit 2-click consent, single-link no-deletion, epoch/version, bounded authoritative readback OK\n";
    return 0;
}
