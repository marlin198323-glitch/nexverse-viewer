/* MPL-2.0: modern Firestorm Low158 Variable-1 attachment blocks.
 * End to end: actual LLUDP packet -> Low158 decoder -> self WorldModel. */
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include "nexverse/assets/avatar_bake_stream_priority.hpp"
#include "nexverse/assets/texture_residency.hpp"
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
// Actual OpenSim GetBakesBytes TE can report independently sourced
// head/upper/lower baked IDs. Exercise the full parser's bitfield
// overrides, not fabricated static UUIDs inside the streaming planner.
std::vector<std::uint8_t> baked_entry()
{
    const auto plain=entry();
    std::vector<std::uint8_t> te(plain.begin(),plain.begin()+16U);
    const std::array<const char*,3U> ids{
        "11111111-1111-1111-1111-111111111111",
        "22222222-2222-2222-2222-222222222222",
        "33333333-3333-3333-3333-333333333333"
    };
    const std::array<std::uint8_t,3U> face_masks{0x82U,0x84U,0x88U};
    for(std::size_t i=0U;i<ids.size();++i)
    {
        te.push_back(face_masks[i]);te.push_back(0U);
        const auto bytes=protocol::uuid_to_bytes(ids[i]);
        if(!bytes)return {};
        te.insert(te.end(),bytes->begin(),bytes->end());
    }
    te.push_back(0U); // end texture UUID overrides
    te.insert(te.end(),plain.begin()+17,plain.end());
    return te;
}
std::vector<std::uint8_t> datagram(std::string_view avatar,
                                   unsigned attachments,
                                   std::uint32_t serial=17U,
                                   std::uint32_t cof_version=111U,
                                   std::uint8_t appearance_version=2U,
                                   bool trailing_unknown_cof=false)
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
    payload.push_back(trailing_unknown_cof?2U:1U);
    payload.push_back(appearance_version); // AppearanceVersion
    le32(payload,cof_version); // COF revision is real protocol metadata
    le32(payload,0U);   // Flags
    if(trailing_unknown_cof)
    {
        payload.push_back(appearance_version);
        le32(payload,0U); // unknown version, MUST NOT erase highwater
        le32(payload,0U); // Flags
    }
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
// Exact OpenGenesisLINK main LLClientView.SendAppearance Low158 layout:
// AvatarID, IsTrial, TE Variable2, VisualParams Variable1,
// AppearanceData count=0, AppearanceHover count=1 + 3 floats,
// and NO AttachmentBlock counter. Real server marks it ZeroCoded.
std::vector<std::uint8_t> upstream_legacy_hover_packet(
    std::string_view agent,bool three_body_bakes=false)
{
    std::vector<std::uint8_t> p;
    const auto id=protocol::uuid_to_bytes(agent);
    if(!id)return {};
    p.insert(p.end(),id->begin(),id->end());
    p.push_back(0U); // trial = false
    const auto te=three_body_bakes?baked_entry():entry();
    p.push_back(static_cast<std::uint8_t>(te.size()&255U));
    p.push_back(static_cast<std::uint8_t>(te.size()>>8U));
    p.insert(p.end(),te.begin(),te.end());
    p.insert(p.end(),{3U,120U,121U,122U}); // actual visualparams
    p.push_back(0U); // appearance-data count=0
    p.push_back(1U); // hover count=1
    le32(p,0U);le32(p,0U);le32(p,0x3e800000U); // hover Z=0.25
    const auto plain=protocol::encode_packet(
        protocol::packet_reliable,32U,protocol::msg_avatar_appearance,p);
    // The actual OpenGenesisLINK server sets both reliable and
    // zerocoded bit; encode wire zero runs after the six header bytes.
    std::vector<std::uint8_t> wire(plain.begin(),plain.begin()+6);
    wire[0]=static_cast<std::uint8_t>(
        protocol::packet_reliable|protocol::packet_zerocoded);
    for(std::size_t i=6U;i<plain.size();)
    {
        if(plain[i]!=0U)
        {
            wire.push_back(plain[i++]);
            continue;
        }
        const auto begin=i;
        while(i<plain.size()&&plain[i]==0U&&i-begin<255U)++i;
        wire.push_back(0U);
        wire.push_back(static_cast<std::uint8_t>(i-begin));
    }
    return wire;
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
    const auto upstream_wire=upstream_legacy_hover_packet(self);
    const auto server_frame=protocol::decode_lludp_packet(upstream_wire);
    const auto legacy=decode(upstream_wire);
    if(!server_frame.ok()||!legacy||
       server_frame.packet->header.message!=protocol::msg_avatar_appearance||
       (server_frame.packet->header.flags&protocol::packet_zerocoded)==0U||
       legacy->trial||!legacy->appearance_data.empty()||
       legacy->hover_heights.size()!=1U||
       legacy->hover_heights.front().z!=0.25f||
       legacy->attachment_block_present||!legacy->attachments.empty()||
       !w.apply_self_appearance(*legacy)||
       w.self_appearance()->attachment_block_present)
        return 30;
    auto upstream_truncated=upstream_wire;
    upstream_truncated.pop_back();
    if(decode(upstream_truncated))return 31;
    auto upstream_foreign=decode(upstream_legacy_hover_packet(other));
    if(!upstream_foreign||w.apply_self_appearance(*upstream_foreign)||
       w.self_appearance()->avatar_id!=self)return 32;
    // The three baked body UUIDs now survive the actual OpenGenesisLINK
    // header/zero-coding/Low158/TE parse/authoritative-self pipeline,
    // and are the FIRST assets retained by the real streaming planner.
    auto with_body=make_world();
    const auto baked_wire=upstream_legacy_hover_packet(self,true);
    const auto baked_low158=decode(baked_wire);
    if(!baked_low158||baked_low158->attachment_block_present||
       !protocol::valid_self_appearance_texture_entry(
           baked_low158->texture_entry)||
       !with_body.apply_self_appearance(*baked_low158))return 33;
    const auto refs=protocol::extract_avatar_bakes(
        with_body.self_appearance()->texture_entry);
    const auto body=with_body.self_render_bakes();
    if(refs.valid_count()!=3U||body!=refs.visible_body()||
       body[0]!="11111111-1111-1111-1111-111111111111"||
       body[1]!="22222222-2222-2222-2222-222222222222"||
       body[2]!="33333333-3333-3333-3333-333333333333")return 34;
    const auto priority=assets::prioritize_avatar_body_bakes(
        {{true,body,0.0f,0.0f,0.0f}},0.0f,0.0f,0.0f);
    const auto residency=assets::plan_texture_residency(
        {"particle"}, {"object"}, {"stale-body"},5U,priority);
    if(priority.size()!=3U||
       priority[0]!=body[0]||priority[1]!=body[1]||
       priority[2]!=body[2]||
       residency.preferred_ids.size()!=5U||
       residency.preferred_ids[0]!=body[0]||
       residency.preferred_ids[1]!=body[1]||
       residency.preferred_ids[2]!=body[2]||
       residency.desired_ids.contains("stale-body")||
       residency.eviction_ids.size()!=1U)return 35;
    // Switching the authenticated user erases all three server refs.
    with_body.apply_agent({other,session,{},{},true});
    if(with_body.self_appearance()||
       with_body.self_render_bakes()!=std::array<std::string,3U>{})
        return 36;
    const auto small=decode(datagram(self,1U));
    if(!small||small->attachments.size()!=1U||
       !small->attachment_block_present||
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
    // The real modern Low158 wire COF version must prevent a DELAYED
    // lower revision from silently restoring the former skin/outfit.
    const auto older=decode(datagram(self,2U,24U,110U));
    if(!older||w.apply_self_appearance(*older)||
       w.stale_self_cof_appearances()!=1U||
       w.self_appearance()->appearance_data.back().cof_version!=111U||
       w.self_appearance()->attachments.size()!=65U)return 37;
    const auto newer=decode(datagram(self,2U,25U,112U));
    if(!newer||!w.apply_self_appearance(*newer)||
       w.self_appearance()->appearance_data.back().cof_version!=112U||
       w.self_appearance()->attachments.size()!=2U)return 38;
    // Same COF version can legitimately change the baked TextureEntry;
    // only strictly older revisions are rejected.
    const auto same=decode(datagram(self,3U,26U,112U));
    if(!same||!w.apply_self_appearance(*same)||
       w.self_appearance()->attachments.size()!=3U||
       w.stale_self_cof_appearances()!=1U)return 39;
    // OpenGenesisLINK main emits AppearanceData count=0. Never block
    // the real legacy server just because modern packets were seen.
    const auto legacy_again=decode(upstream_legacy_hover_packet(self,true));
    if(!legacy_again||!legacy_again->appearance_data.empty()||
       !w.apply_self_appearance(*legacy_again)||
       w.self_appearance()->appearance_data.size()!=0U||
       w.stale_self_cof_appearances()!=1U)return 40;
    // Critically, the earlier modern high-water COF version 112 must
    // survive a later legacy Low158 (AppearanceData count=0).
    const auto old_after_legacy=decode(datagram(self,1U,28U,111U));
    if(!old_after_legacy||w.apply_self_appearance(*old_after_legacy)||
       w.stale_self_cof_appearances()!=2U||
       !w.self_appearance()->appearance_data.empty())return 43;
    // A format-version change cannot be compared as a COF revision
    // without knowing whether those integers share the same scale.
    const auto different_format=decode(datagram(self,1U,27U,108U));
    if(!different_format)return 41;
    auto alternate=*different_format;
    alternate.appearance_data.back().appearance_version=3U;
    if(!w.apply_self_appearance(alternate)||
       w.stale_self_cof_appearances()!=2U)return 42;
    // Actual encoded/decoded Low158 wire: alternating version-series
    // cannot allow an earlier format-2 COF revision to reappear.
    const auto replay_v2=decode(datagram(self,1U,29U,110U,2U,true));
    if(!replay_v2||replay_v2->appearance_data.size()!=2U||
       replay_v2->appearance_data.back().cof_version!=0U||
       w.apply_self_appearance(*replay_v2)||
       w.stale_self_cof_appearances()!=3U)return 44;
    const auto fresh_v3=decode(datagram(self,1U,30U,109U,3U,true));
    if(!fresh_v3||!w.apply_self_appearance(*fresh_v3)||
       w.stale_self_cof_appearances()!=3U)return 45;
    const auto delayed_v2=decode(datagram(self,1U,31U,111U,2U,true));
    if(!delayed_v2||w.apply_self_appearance(*delayed_v2)||
       w.stale_self_cof_appearances()!=4U)return 46;
    const auto current_v2=decode(datagram(self,1U,32U,112U,2U,true));
    if(!current_v2||!w.apply_self_appearance(*current_v2)||
       w.stale_self_cof_appearances()!=4U)return 47;
    const auto delayed_v3=decode(datagram(self,1U,33U,108U,3U,true));
    if(!delayed_v3||w.apply_self_appearance(*delayed_v3)||
       w.stale_self_cof_appearances()!=5U)return 48;
    w.begin_region_transition({region+1U,"New",{},"127.0.0.1",
        9001U,"","",0U,0U,0U,13U,0.0f,"",256U,256U,true});
    if(w.self_appearance()||w.stale_self_cof_appearances()!=0U)return 10;
    std::cout<<"OGL_LOW158_ATTACHMENT_65_TO_255_WIRE_AND_MODEL_PASS\n";
}
