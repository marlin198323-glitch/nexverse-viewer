/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_cof_change_plan.hpp"
#include "nexverse/protocol/firestorm_outfit_mutation.hpp"
#include "nexverse/protocol/outbound_queue.hpp"
#include <iostream>
using namespace nexverse::protocol;
constexpr auto AG="11111111-1111-1111-1111-111111111111";
constexpr auto SID="22222222-2222-2222-2222-222222222222";
constexpr auto OUTFIT="33333333-3333-3333-3333-333333333333";
constexpr auto COF="44444444-4444-4444-4444-444444444444";
constexpr auto LINK1="55555555-5555-5555-5555-555555555555";
constexpr auto LINK2="66666666-6666-6666-6666-666666666666";
constexpr auto TARGET1="77777777-7777-7777-7777-777777777777";
constexpr auto TARGET2="88888888-8888-8888-8888-888888888888";
constexpr auto TARGET3="99999999-9999-9999-9999-999999999999";
int main()
{
    LoginSession login;login.agent_id=AG;login.session_id=SID;
    CofLinkCommand link{COF,TARGET1,"Outfit", "Item-Link",0x44332211U,18};
    const auto wire=encode_cof_link(100,login,link);
    const auto packet=decode_lludp_packet(wire);
    if(!packet.ok()||!(packet.packet->header.flags&packet_reliable)||
       packet.packet->header.message!=msg_link_inventory_item ||
       packet.packet->header.sequence!=100U)return 1;
    const auto& p=packet.packet->payload;
    if(p.size()!=86U+1U+link.name.size()+1U+
                           1U+link.description.size()+1U||
       p[32]!=0x11||p[33]!=0x22||p[34]!=0x33||p[35]!=0x44||
       p[84]!=24U||p[85]!=18U||
       p[86]!=link.name.size()+1U||p[87]!='O')return 2;
    auto uuid=uuid_to_bytes(COF);
    if(!uuid||!std::equal(uuid->begin(),uuid->end(),p.begin()+36))return 3;
    if(!std::all_of(p.begin()+52,p.begin()+68,
        [](std::uint8_t c){return c==0U;}))return 4;
    uuid=uuid_to_bytes(TARGET1);
    if(!uuid||!std::equal(uuid->begin(),uuid->end(),p.begin()+68))return 5;
    link.name="";if(!encode_cof_link(100,login,link).empty())return 6;
    link.name="Outfit";
    link.callback_id=0U;
    if(!encode_cof_link(100,login,link).empty())return 7;
    link.callback_id=1U;link.linked_item_id="invalid";
    if(!encode_cof_link(100,login,link).empty())return 8;
    link.linked_item_id=TARGET1;
    link.name=std::string(121U,'x');
    if(!encode_cof_link(100,login,link).empty())return 9;
    const auto unlink=encode_cof_unlink(201U,login,{LINK1});
    const auto deleted=decode_lludp_packet(unlink);
    if(!deleted.ok()||deleted.packet->header.message!=msg_remove_inventory_link||
       deleted.packet->payload.size()!=49U||
       !(deleted.packet->header.flags&packet_reliable)||
       deleted.packet->payload[32]!=1U)return 10;
    uuid=uuid_to_bytes(LINK1);
    if(!uuid||!std::equal(uuid->begin(),uuid->end(),
                          deleted.packet->payload.begin()+33))return 11;
    if(!encode_cof_unlink(300,login,{"invalid"}).empty())return 12;
    OutboundCommandQueue queue{2U};
    if(!queue.push(CofLinkCommand{COF,TARGET2,"Wearable","Link",3U,18})||
       !queue.push(CofUnlinkCommand{LINK1}) ||
       queue.push(CofUnlinkCommand{LINK2}))return 13;
    if(!std::holds_alternative<CofLinkCommand>(*queue.try_pop())||
       !std::holds_alternative<CofUnlinkCommand>(*queue.try_pop()))
       return 14;
    const std::vector<OutfitInventoryItem> saved={
        {LINK2,TARGET1,"Shirt",24,18},
        {TARGET2,TARGET2,"Body",5,18}};
    const std::vector<OutfitInventoryItem> current={
        {LINK1,TARGET1,"Old",24,18},
        {TARGET3,TARGET3,"Extra",24,18}};
    const std::vector<OutfitInventoryItem> resolved={
        {TARGET1,TARGET1,"Original",5,18}};
    const auto plan=plan_firestorm_cof_change(
        OUTFIT,COF,&saved,&current,resolved,23U);
    if(!plan.preview_ready()||plan.executable()||
       plan.add.size()!=1U||plan.remove.size()!=1U||
       plan.unchanged!=1U||plan.cof_version!=23U||
       plan.add[0].target_id!=TARGET2||
       plan.remove[0].link_id!=TARGET3)return 15;
    if(plan_firestorm_cof_change(
        OUTFIT,COF,&saved,&current,resolved,0U).readiness!=
        CofPlanReadiness::missing_cof)return 16;
    if(plan_firestorm_cof_change(
        OUTFIT,COF,&saved,nullptr,resolved,23U).preview_ready())return 17;
    auto missing=plan_firestorm_cof_change(
        OUTFIT,COF,&saved,&current,{},23U);
    if(missing.readiness!=CofPlanReadiness::unresolved_target)return 18;
    auto unsafe=saved;
    unsafe[0].asset_type=25;
    if(plan_firestorm_cof_change(
        OUTFIT,COF,&unsafe,&current,resolved,23U).readiness!=
        CofPlanReadiness::unsupported_folder_link)return 19;
    unsafe=saved;
    unsafe.push_back(saved[0]);
    if(plan_firestorm_cof_change(
        OUTFIT,COF,&unsafe,&current,resolved,23U).readiness!=
        CofPlanReadiness::duplicate_target)return 20;
    const auto matched=plan_firestorm_cof_change(
        OUTFIT,COF,&saved,&current,resolved,23U);
    if(matched.executable())return 21;
    const std::vector<OutfitInventoryItem> after={
        {LINK1,TARGET1,"First",24,18},
        {LINK2,TARGET2,"Second",24,18}};
    if(reconcile_cof_readback(plan,4U,4U,24U,&after)!=
        CofReconcileState::confirmed)return 22;
    if(reconcile_cof_readback(plan,4U,5U,24U,&after)!=
        CofReconcileState::stale_region)return 23;
    if(reconcile_cof_readback(plan,4U,4U,23U,&after)!=
        CofReconcileState::stale_snapshot)return 24;
    if(reconcile_cof_readback(plan,4U,4U,25U,&current)!=
        CofReconcileState::different_targets)return 25;
    const std::vector<OutfitInventoryItem> unsafe_after={
        {LINK1,TARGET1,"Not a link",5,18}};
    if(reconcile_cof_readback(plan,4U,4U,25U,&unsafe_after)!=
        CofReconcileState::invalid_server_items)return 26;
    if(reconcile_cof_readback(missing,4U,4U,25U,&after)!=
        CofReconcileState::no_plan)return 27;
    std::cout<<"Original LLUDP COF Link426 Remove270 and read-only version-bound diff OK\n";
    return 0;
}
