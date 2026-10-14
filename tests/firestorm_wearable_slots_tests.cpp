/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_appearance_packets.hpp"
#include "nexverse/protocol/firestorm_wearable_slots.hpp"
#include <bit>
#include <cstdio>
#include <iostream>
#include <limits>
using namespace nexverse::protocol;
static std::string uuid(std::uint32_t v)
{
    char buf[37];
    (void)std::snprintf(buf,sizeof(buf),
        "00000000-0000-0000-0000-%012x",v);
    return buf;
}
int main()
{
    constexpr auto owner="11111111-1111-1111-1111-111111111111";
    constexpr auto sessionid="22222222-2222-2222-2222-222222222222";
    LoginSession session;session.agent_id=owner;session.session_id=sessionid;
    std::vector<OutfitInventoryItem> originals,cof;
    for(unsigned i=0U;i<5U;++i)
    {
        const auto target=uuid(i+1U),link=uuid(i+20U);
        originals.push_back({target,uuid(i+101U),"wearable",i<4U?13:5,18,i});
        cof.push_back({link,target,"COF wearable",24,18});
    }
    auto check=resolve_firestorm_wearables(&cof,originals);
    if(check.readiness!=WearableReadiness::ready_for_appearance_pipeline||
       check.wearable_count!=5U||check.link_count!=5U||
       check.count[0]!=1U||check.count[4]!=1U||
       check.worn_items[0]!=uuid(1U)||check.may_send_appearance())
        return 1;
    auto missing=originals;
    missing[0].flags.reset();
    if(resolve_firestorm_wearables(&cof,missing).readiness!=
       WearableReadiness::missing_subtype)return 2;
    missing=originals;missing[0].flags=0x01000000U;
    if(resolve_firestorm_wearables(&cof,missing).readiness!=
       WearableReadiness::ready_for_appearance_pipeline)return 3;
    missing=originals;missing[0].flags=255U;
    if(resolve_firestorm_wearables(&cof,missing).readiness!=
       WearableReadiness::invalid_subtype)return 4;
    missing=originals;missing[0].asset_type=5;
    if(resolve_firestorm_wearables(&cof,missing).readiness!=
       WearableReadiness::asset_mismatch)return 5;
    missing=originals;missing[0].inventory_type=6;
    if(resolve_firestorm_wearables(&cof,missing).readiness!=
       WearableReadiness::asset_mismatch)return 6;
    auto without=cof;
    without.erase(without.begin());
    if(resolve_firestorm_wearables(&without,originals).readiness!=
       WearableReadiness::missing_bodypart)return 7;
    auto dup=cof;dup.push_back(cof[4]);
    if(resolve_firestorm_wearables(&dup,originals).readiness!=
       WearableReadiness::asset_mismatch)return 8;
    dup=cof;
    dup.push_back({uuid(90U),uuid(91U),"missing",24,18});
    if(resolve_firestorm_wearables(&dup,originals).readiness!=
       WearableReadiness::missing_target)return 9;
    auto layers=originals;layers.push_back({
        uuid(40U),uuid(140U),"extra shirt",5,18,4U});
    dup=cof;dup.push_back({uuid(80U),uuid(40U),"link",24,18});
    if(resolve_firestorm_wearables(&dup,layers).readiness!=
       WearableReadiness::multiwear_requires_bake)return 10;
    layers=originals;layers.push_back({
        uuid(40U),uuid(140U),"extra skin",13,18,1U});
    if(resolve_firestorm_wearables(&dup,layers).readiness!=
       WearableReadiness::multiple_primary)return 11;
    const auto blank=resolve_firestorm_wearables(nullptr,originals);
    if(blank.readiness!=WearableReadiness::no_cof)return 12;
    std::vector<FirestormWearableSlot> wear;
    for(std::uint8_t i=0U;i<17U;++i)
        wear.push_back({i,i<5U?uuid(i+1U):uuid(0U)});
    auto packet=decode_lludp_packet(
        encode_agent_is_now_wearing(75U,session,wear));
    if(!packet.ok()||!(packet.packet->header.flags&packet_reliable)||
       packet.packet->header.message!=msg_agent_is_now_wearing||
       packet.packet->payload.size()!=32U+1U+17U*17U||
       packet.packet->payload[32]!=17U)return 13;
    const auto id=uuid_to_bytes(uuid(1U));
    if(!id||!std::equal(id->begin(),id->end(),
                       packet.packet->payload.begin()+33U)||
       packet.packet->payload[49U]!=0U)return 14;
    wear[2].type=1U;
    if(!encode_agent_is_now_wearing(75U,session,wear).empty())return 15;
    wear[2].type=2U;wear[0].item_id=uuid(0U);
    if(!encode_agent_is_now_wearing(75U,session,wear).empty())return 16;
    AgentSetAppearanceCommand command;
    command.serial=5U;
    command.avatar_size={0.5f,0.4f,1.75f};
    command.wearable_cache={{uuid(27U),0U},{uuid(28U),8U}};
    command.texture_entry={0xa1U,0xb2U,0xc3U};
    command.visual_params={0x00U,0x80U,0xffU};
    packet=decode_lludp_packet(
        encode_agent_set_appearance(76U,session,command));
    if(!packet.ok()||packet.packet->header.message!=
       msg_agent_set_appearance||!(packet.packet->header.flags&packet_reliable)||
       packet.packet->payload.size()!=32U+4U+12U+1U+34U+
           2U+3U+1U+3U||packet.packet->payload[32]!=5U||
       packet.packet->payload[48]!=2U||
       packet.packet->payload[83U]!=3U||
       packet.packet->payload[84U]!=0U||
       packet.packet->payload[88U]!=3U)return 17;
    command.serial=0U;
    if(!encode_agent_set_appearance(77U,session,command).empty())return 18;
    command.serial=6U;
    command.avatar_size[2]=std::numeric_limits<float>::quiet_NaN();
    if(!encode_agent_set_appearance(77U,session,command).empty())return 19;
    command.avatar_size[2]=1.75f;
    command.wearable_cache[1].texture_index=0U;
    if(!encode_agent_set_appearance(77U,session,command).empty())return 20;
    command.wearable_cache[1].texture_index=8U;
    command.texture_entry.clear();
    if(!encode_agent_set_appearance(77U,session,command).empty())return 21;
    std::cout<<"Firestorm 17 wearable flags/slots and Low383/Low84 byte-perfect safe packet encoders, no automatic appearance mutation OK\n";
    return 0;
}
