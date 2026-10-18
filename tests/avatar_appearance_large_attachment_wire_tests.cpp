/* MPL-2.0: modern Firestorm Low158 Variable-1 attachment blocks.
 * End to end: actual LLUDP packet -> Low158 decoder -> self WorldModel. */
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include "nexverse/world/world_model.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
using namespace nexverse;
namespace
{
constexpr auto self="11111111-2222-3333-4444-555555555555";
constexpr auto session="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
constexpr auto other="eeeeeeee-2222-3333-4444-555555555555";
constexpr std::uint64_t region=0x10000000200ULL;
void le32(std::vector<std::uint8_t>& bytes,std::uint32_t value)
{
    for(unsigned shift=0;shift<32;shift+=8)
        bytes.push_back(static_cast<std::uint8_t>((value>>shift)&255U));
}
std::vector<std::uint8_t> entry()
{
    // Firestorm default LLTextureEntry with 45 faces, no baked UUIDs.
    // A valid unbaked self packet is authoritative, not rendered skin.
    std::vector<std::uint8_t> te(16U,0x11U);
    const std::vector<std::uint8_t> fields{
        0U, // no UUID face overrides
        255U,255U,255U,255U,0U, // default white, no color overrides
        0U,0U,128U,63U,0U, // repeat U = 1.0
        0U,0U,128U,63U,0U, // repeat V = 1.0
        0U,0U,0U, // offset U and mask
        0U,0U,0U, // offset V and mask
        0U,0U,0U, // rotation and mask
        0U,0U, // material
        0U,0U, // media
        0U,0U // glow
    };
    te.insert(te.end(),fields.begin(),fields.end());
    return te;
}
std::vector<std::uint8_t> datagram(std::string_view avatar,
                                   unsigned attachments,
                                   std::uint32_t serial=17U)
{
    const auto bytes=protocol::uuid_to_bytes(avatar);
    const auto asset=protocol::uuid_to_bytes(
        "22222222-3333-4444-5555-666666666666");
    if(!bytes||!asset||attachments>255U)return {};
    std::vector<std::uint8_t> payload(bytes->begin(),bytes->end());
    payload.push_back(0U); // trial = false
    const auto te=entry();
    payload.push_back(static_cast<std::uint8_t>(te.size()&255U));
    payload.push_back(static_cast<std::uint8_t>(te.size()>>8U));
    payload.insert(payload.end(),te.begin(),te.end());
    payload.insert(payload.end(),{3U,125U,126U,127U});
    payload.push_back(1U); // AppearanceData Variable-1
    payload.push_back(2U); // AppearanceVersion
    le32(payload,111U); // COF version, not verified
    le32(payload,0U);   // Flags
    payload.push_back(0U); // AppearanceHover Variable-1 = zero
    payload.push_back(static_cast<std::uint8_t>(attachments));
    for(unsigned i=0;i<attachments;++i)
    {
        payload.insert(payload.end(),asset->begin(),asset->end());
        payload.push_back(static_cast<std::uint8_t>((i%53U)+1U));
    }
    return protocol::encode_packet(
        protocol::packet_reliable,serial,protocol::msg_avatar_appearance,
        payload);
}
std::optional<protocol::AvatarAppearanceMessage> decode(
    const std::vector<std::uint8_t>& message,
    protocol::AvatarAppearanceFailure* fail=nullptr)
{
    const auto frame=protocol::decode_lludp_packet(message);
    if(!frame.ok()||
       frame.packet->header.message!=protocol::msg_avatar_appearance)
        return std::nullopt;
    return protocol::decode_avatar_appearance(frame.packet->payload,fail);
}
world::WorldModel make_world()
{
    world::WorldModel w;
    w.apply_region({region,"Test",{},"127.0.0.1",9000U,"","",
        0U,0U,0U,13U,0.0f,"",256U,256U,true});
    w.apply_agent({self,session,{},{},true});
    return w;
}
}
int main()
{
    const auto te=entry();
    if(!protocol::valid_self_appearance_texture_entry(te))return 1;
    auto w=make_world();
    const auto small=decode(datagram(self,1U));
    if(!small||small->attachments.size()!=1U||
       !w.apply_self_appearance(*small))return 2;
    // Previously hard-coded 64-object cutoff broke authentic 65+ object
    // modern packets (multiple attachments per avatar attach point).
    const auto medium=decode(datagram(self,65U));
    if(!medium||medium->attachments.size()!=65U||
       medium->appearance_data.size()!=1U||
       medium->appearance_data.front().cof_version!=111U||
       medium->attachments.back().attachment_point!=12U||
       !w.apply_self_appearance(*medium)||
       w.self_appearance()->attachments.size()!=65U)return 3;
    // Byte-count limit 255: test the field decoder independently.
    // Physical LLUDP MTU must continue to reject an oversize datagram;
    // these 255 entries can NOT arrive as a single unfragmented packet.
    const auto maximal_frame=datagram(self,255U);
    if(maximal_frame.size()<=protocol::lludp_mtu ||
       protocol::decode_lludp_packet(maximal_frame).ok())return 4;
    constexpr std::size_t low158_header=10U; // 6-byte base + 4-byte Low ID
    const auto max=protocol::decode_avatar_appearance(
        std::span<const std::uint8_t>{
            maximal_frame.data()+low158_header,
            maximal_frame.size()-low158_header});
    if(!max||max->attachments.size()!=255U||
       !w.apply_self_appearance(*max)||
       w.self_appearance()->attachments.size()!=255U)return 11;
    const auto retained=w.self_appearance()->attachments.size();
    // Truncation is rejected before WorldModel, so it cannot wipe skin.
    auto broken=datagram(self,65U,20U);
    broken.pop_back();
    protocol::AvatarAppearanceFailure reason{};
    if(decode(broken,&reason)||
       reason!=protocol::AvatarAppearanceFailure::attachments||
       w.self_appearance()->attachments.size()!=retained)return 5;
    // One-byte count cannot claim more items than actually supplied.
    auto inconsistent=datagram(self,65U,21U);
    const auto offset=inconsistent.size()-65U*17U-1U;
    inconsistent[offset]=static_cast<std::uint8_t>(66U);
    if(decode(inconsistent,&reason)||
       reason!=protocol::AvatarAppearanceFailure::attachments)return 6;
    const auto foreign=decode(datagram(other,65U,22U));
    if(!foreign||w.apply_self_appearance(*foreign)||
       w.self_appearance()->attachments.size()!=retained)return 7;
    // Retain authoritative low158 references only within the same
    // authenticated agent and region; no fictitious scene attachments.
    w.apply_agent({other,session,{},{},true});
    if(w.self_appearance())return 8;
    w.apply_agent({self,session,{},{},true});
    const auto next=decode(datagram(self,65U,23U));
    if(!next||!w.apply_self_appearance(*next))return 9;
    w.begin_region_transition({region+1U,"New",{},"127.0.0.1",
        9001U,"","",0U,0U,0U,13U,0.0f,"",256U,256U,true});
    if(w.self_appearance())return 10;
    std::cout<<"OGL_LOW158_ATTACHMENT_65_TO_255_WIRE_AND_MODEL_PASS\n";
}
