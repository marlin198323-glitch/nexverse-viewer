/* MPL-2.0: Firestorm legacy Low381 request and Low382 initial readback.
 * Not AgentIsNowWearing / AgentSetAppearance, and not COF/bake proof. */
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/world/world_model.hpp"
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
    std::cout<<"OGL_AGENT_WEARABLES_LOW381_382_REAL_DATAGRAM_AUTH_PASS\n";
}
