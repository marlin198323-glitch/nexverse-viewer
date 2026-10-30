/* MPL-2.0: Firestorm legacy Low381 request and Low382 initial readback.
 * Not AgentIsNowWearing / AgentSetAppearance, and not COF/bake proof. */
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/legacy_wearable_inventory_probe.hpp"
#include "nexverse/protocol/legacy_wearable_local_preview.hpp"
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/wearables_serial_order.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace nexverse;
constexpr auto self_id="11111111-2222-3333-4444-555555555555";
constexpr auto session_id="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
constexpr auto other_id="99999999-2222-3333-4444-555555555555";
constexpr std::uint64_t region=0x0000010000000200ULL;
void append_uuid(std::vector<std::uint8_t>& out,std::string_view id)
{
    const auto bytes=protocol::uuid_to_bytes(id);
    if(bytes)out.insert(out.end(),bytes->begin(),bytes->end());
}
std::vector<std::uint8_t> packet(std::string_view agent,std::string_view session,
                                 std::uint32_t serial,unsigned slots)
{
    std::vector<std::uint8_t> data;
    append_uuid(data,agent);append_uuid(data,session);
    for(unsigned shift=0U;shift<32U;shift+=8U)
        data.push_back(static_cast<std::uint8_t>((serial>>shift)&0xffU));
    data.push_back(static_cast<std::uint8_t>(slots));
    for(unsigned i=0U;i<slots;++i)
    {
        append_uuid(data,i==0U?"00000000-0000-0000-0000-000000000000":
                          "33333333-3333-3333-3333-333333333333");
        append_uuid(data,i==0U?"00000000-0000-0000-0000-000000000000":
                          "44444444-4444-4444-4444-444444444444");
        data.push_back(static_cast<std::uint8_t>(i));
    }
    return protocol::encode_packet(protocol::packet_reliable,serial+3U,
                                  protocol::msg_agent_wearables_update,data);
}
std::optional<protocol::AgentWearablesUpdateMessage> decode(
    const std::vector<std::uint8_t>& datagram)
{
    const auto parsed=protocol::decode_lludp_packet(datagram);
    if(!parsed.ok()||
       parsed.packet->header.message!=protocol::msg_agent_wearables_update)
        return std::nullopt;
    return protocol::decode_agent_wearables_update(parsed.packet->payload);
}
int main()
{
    protocol::LoginSession login;
    login.agent_id=self_id;login.session_id=session_id;
    const auto request=protocol::encode_agent_wearables_request(15U,login);
    const auto decoded=protocol::decode_lludp_packet(request);
    if(!decoded.ok()||decoded.packet->header.sequence!=15U||
       decoded.packet->header.message!=protocol::msg_agent_wearables_request||
       (decoded.packet->header.flags & protocol::packet_reliable)==0U||
       decoded.packet->payload.size()!=32U)return 1;
    const auto expected_id=protocol::uuid_to_bytes(self_id);
    if(!std::equal(expected_id->begin(),expected_id->end(),
                   decoded.packet->payload.begin()))return 2;
    login.session_id="not-a-uuid";
    if(!protocol::encode_agent_wearables_request(16U,login).empty())return 3;
    const auto frame=packet(self_id,session_id,4U,17U);
    const auto worn=decode(frame);
    if(!worn||worn->agent_id!=self_id||worn->session_id!=session_id||
       worn->serial!=4U||worn->wearables.size()!=17U||
       worn->wearables[16].wearable_type!=16U)return 4;
    auto w=world::WorldModel{};
    w.apply_region({region,"Test",{},"127.0.0.1",9000U,"","",
         0U,0U,0U,13U,0.0f,"",256U,256U,true});
    w.apply_agent({self_id,session_id,{},{},true});
    if(!w.apply_self_wearables(*worn)||
       !w.snapshot().self_wearables||
       w.snapshot().self_wearables->wearables.size()!=17U)return 5;
    if(w.apply_self_wearables(*worn))return 6;
    const auto older=decode(packet(self_id,session_id,3U,4U));
    if(!older||w.apply_self_wearables(*older)||
       w.self_wearables()->serial!=4U)return 7;
    const auto foreign=decode(packet(other_id,session_id,5U,4U));
    if(!foreign||w.apply_self_wearables(*foreign))return 8;
    const auto wrong_session=decode(packet(self_id,other_id,5U,4U));
    if(!wrong_session||w.apply_self_wearables(*wrong_session))return 9;
    const auto fresh=decode(packet(self_id,session_id,5U,1U));
    if(!fresh||!w.apply_self_wearables(*fresh)||
       w.self_wearables()->wearables.size()!=1U)return 10;
    auto broken=frame;broken.pop_back();
    const auto broken_frame=protocol::decode_lludp_packet(broken);
    if(!broken_frame.ok()||
       protocol::decode_agent_wearables_update(
           broken_frame.packet->payload))return 11;
    auto extra=frame;extra.push_back(0U);
    const auto extra_frame=protocol::decode_lludp_packet(extra);
    if(!extra_frame.ok()||
       protocol::decode_agent_wearables_update(
           extra_frame.packet->payload))return 12;
    auto invalid=frame;invalid.back()=17U;
    const auto invalid_frame=protocol::decode_lludp_packet(invalid);
    if(!invalid_frame.ok()||
       protocol::decode_agent_wearables_update(
           invalid_frame.packet->payload))return 13;
    const auto dummy=decode(packet(self_id,session_id,6U,0U));
    if(!dummy||!w.apply_self_wearables(*dummy)||
       !w.self_wearables()->wearables.empty())return 14;
    w.apply_agent({other_id,session_id,{},{},true});
    if(w.self_wearables())return 15;
    w.apply_agent({self_id,session_id,{},{},true});
    if(!w.apply_self_wearables(*worn))return 16;
    w.begin_region_transition({region+1U,"New",{},"127.0.0.1",
      9001U,"","",0U,0U,0U,13U,0.0f,"",256U,256U,true});
    if(w.self_wearables())return 17;
    // Actual LLUDP datagrams traverse encode -> Low382 decoder ->
    // session/region-authenticated model. Wrap must NOT freeze outfits.
    using protocol::wearables_serial_newer;
    if(!wearables_serial_newer(0U,0xffffffffU)||
       !wearables_serial_newer(1U,0xfffffffeU)||
       wearables_serial_newer(0xffffffffU,0U)||
       wearables_serial_newer(4U,4U)||
       wearables_serial_newer(0x80000000U,0U)||
       wearables_serial_newer(0U,0x80000000U))
        return 18;
    const auto near_wrap=decode(packet(self_id,session_id,0xfffffffeU,2U));
    const auto wrapped0=decode(packet(self_id,session_id,0U,3U));
    const auto wrapped1=decode(packet(self_id,session_id,1U,4U));
    const auto old_retransmit=decode(packet(self_id,session_id,0xffffffffU,9U));
    if(!near_wrap||!wrapped0||!wrapped1||!old_retransmit||
       !w.apply_self_wearables(*near_wrap)||
       w.self_wearables()->serial!=0xfffffffeU||
       !w.apply_self_wearables(*wrapped0)||
       w.self_wearables()->wearables.size()!=3U||
       w.apply_self_wearables(*old_retransmit)||
       !w.apply_self_wearables(*wrapped1)||
       w.self_wearables()->serial!=1U||
       w.self_wearables()->wearables.size()!=4U)return 19;
    // A pre-wrap retransmit still cannot revive previous outfit data,
    // and an exact duplicate cannot create a second "new" readback.
    if(w.apply_self_wearables(*near_wrap)||
       w.apply_self_wearables(*wrapped1)||
       w.self_wearables()->serial!=1U)return 20;
    // New login may restart serial numbering from any initial value.
    w.apply_agent({other_id,session_id,{},{},true});
    w.apply_agent({self_id,session_id,{},{},true});
    if(w.self_wearables()||!w.apply_self_wearables(*worn)||
       w.self_wearables()->serial!=4U)return 21;
    // Empty authenticated CurrentOutfitFolder cannot infer worn clothing,
    // but Low382 original item IDs may be independently checked read-only.
    protocol::AgentWearablesUpdateMessage legacy{};
    legacy.agent_id=self_id;
    legacy.session_id=session_id;
    legacy.serial=42U;
    const std::array<std::string,4> originals{
        "10000000-0000-0000-0000-000000000001",
        "10000000-0000-0000-0000-000000000002",
        "10000000-0000-0000-0000-000000000003",
        "10000000-0000-0000-0000-000000000004"};
    const std::array<std::string,4> assets{
        "20000000-0000-0000-0000-000000000001",
        "20000000-0000-0000-0000-000000000002",
        "20000000-0000-0000-0000-000000000003",
        "20000000-0000-0000-0000-000000000004"};
    for(unsigned i=0;i<4;++i)
        legacy.wearables.push_back({originals[i],assets[i],
                                   static_cast<std::uint8_t>(i)});
    const auto plan=protocol::plan_legacy_wearable_inventory_probe(
        &legacy,self_id,session_id);
    if(!plan.eligible||plan.ids.size()!=4U ||
       protocol::plan_legacy_wearable_inventory_probe(
           &legacy,other_id,session_id).eligible)return 22;
    protocol::InventoryItemFetch fetched{};
    fetched.valid=true;
    for(unsigned i=0;i<4;++i)
        fetched.items.push_back({originals[i],assets[i],"Body",13,18,i});
    const auto proof=protocol::audit_legacy_wearable_inventory_probe(
        plan,fetched);
    if(!proof.valid||!proof.complete_primary||proof.bodyparts!=4U||
       proof.matched!=4U||proof.missing!=0U)return 23;
    fetched.items[2].asset_id=assets[1];
    const auto mismatch=protocol::audit_legacy_wearable_inventory_probe(
        plan,fetched);
    if(mismatch.complete_primary||mismatch.matched!=3U||
       mismatch.missing!=1U)return 24;
    legacy.wearables.push_back(legacy.wearables.front());
    if(protocol::plan_legacy_wearable_inventory_probe(
           &legacy,self_id,session_id).eligible)return 25;
    legacy.wearables.pop_back();
    legacy.wearables[0].wearable_type=17U;
    if(protocol::plan_legacy_wearable_inventory_probe(
           &legacy,self_id,session_id).eligible)return 26;
    // 2026-10-31 live: all five Low382 originals valid, all four bodypart
    // originals exist, but COF server returned a genuinely empty array.
    // This authorizes ONLY an UNBAKED local self preview, never a new
    // AvatarAppearance or automatic inventory modification.
    const auto admitted=protocol::legacy_local_preview_admitted(
        proof,true,9U,9U,42U,42U,self_id,self_id,
        session_id,session_id);
    if(!admitted)return 27;
    if(protocol::legacy_local_preview_admitted(
        proof,false,9U,9U,42U,42U,self_id,self_id,
        session_id,session_id))return 28;
    if(protocol::legacy_local_preview_admitted(
        proof,true,9U,10U,42U,42U,self_id,self_id,
        session_id,session_id))return 29;
    if(protocol::legacy_local_preview_admitted(
        proof,true,9U,9U,42U,43U,self_id,self_id,
        session_id,session_id))return 30;
    if(protocol::legacy_local_preview_admitted(
        proof,true,9U,9U,42U,42U,self_id,other_id,
        session_id,session_id))return 31;
    if(protocol::legacy_local_preview_admitted(
        proof,true,9U,9U,42U,42U,self_id,self_id,
        session_id,other_id))return 32;
    if(protocol::legacy_local_preview_admitted(
        mismatch,true,9U,9U,42U,42U,self_id,self_id,
        session_id,session_id))return 33;
    std::cout<<"OGL_AGENT_WEARABLES_LOW381_382_REAL_DATAGRAM_AUTH_PASS\n";
}
