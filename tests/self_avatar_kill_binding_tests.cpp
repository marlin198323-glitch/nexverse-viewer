/* MPL-2.0: own KillObject closes old LocalID/terse and provisional bake;
   actual authenticated Low158 remains authoritative. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include <iostream>
#include <vector>
using namespace nexverse;
namespace
{
constexpr auto own="11111111-2222-3333-4444-555555555555";
constexpr auto another="77777777-2222-3333-4444-555555555555";
constexpr auto session="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
constexpr std::uint64_t region=0x0000010000000200ULL;
world::RegionUpdate region_packet(bool connected=true)
{
    return {region,"Current",{128.0f,128.0f,25.0f},"127.0.0.1",
        9000U,"","",0U,0U,0U,13U,20.0f,"",256U,256U,connected};
}
std::vector<std::uint8_t> real_te()
{
    std::vector<std::uint8_t> te(16U,0x11U);
    te.insert(te.end(),{0x82U,0U});
    const auto uuid=protocol::uuid_to_bytes(
        "99999999-1111-2222-3333-444444444444");
    te.insert(te.end(),uuid->begin(),uuid->end());
    te.insert(te.end(),{
        0U,255U,255U,255U,255U,0U,
        0U,0U,128U,63U,0U,
        0U,0U,128U,63U,0U,
        0U,0U,0U,0U,0U,0U,0U,
        0U,0U,0U,0U,0U,0U});
    return te;
}
}
int main()
{
    world::WorldModel w;
    w.apply_region(region_packet());
    w.apply_agent({own,session,{},{},true});
    const auto te=real_te();
    if(!protocol::valid_self_appearance_texture_entry(te))return 1;
    if(w.withdraw_self_object_on_kill(another,region,51U)||
       w.withdraw_self_object_on_kill(own,region+1U,51U)||
       w.withdraw_self_object_on_kill(own,region,0U)||
       w.self_object_kills()!=0U)return 2;
    if(!w.observe_self_object_bakes(own,region,51U,te)||
       w.self_render_bakes()[0].empty())return 3;
    // Real low KillObject on wire: 1 x local ID uint32, little endian.
    const std::vector<std::uint8_t> packet{1U,51U,0U,0U,0U};
    const auto decoded=protocol::decode_kill_object(packet);
    if(!decoded||decoded->local_ids!=std::vector<std::uint32_t>{51U})
        return 4;
    std::uint32_t current_self_local_id=51U;
    if(current_self_local_id==decoded->local_ids[0]&&
       w.withdraw_self_object_on_kill(own,region,51U))
        current_self_local_id=0U;
    if(current_self_local_id!=0U||w.self_object_bakes()||
       w.self_render_bakes()!=std::array<std::string,3U>{}||
       w.self_object_kills()!=1U)return 5;
    // Duplicate kill is handled by dispatcher local-ID binding, not
    // counted twice; old terse local 51 must no longer match current 0.
    if(current_self_local_id==decoded->local_ids[0]&&
       w.withdraw_self_object_on_kill(own,region,51U))
        return 6;
    if(w.self_object_kills()!=1U)return 7;
    // Genuine authenticated Low158 must NOT be removed merely because
    // the simulator retired one ObjectUpdate LocalID.
    protocol::AvatarAppearanceMessage appearance;
    appearance.avatar_id=own;
    appearance.texture_entry=te;
    appearance.visual_params={125U,126U,127U};
    if(!w.apply_self_appearance(appearance))return 8;
    if(!w.observe_self_object_bakes(own,region,52U,te)||
       w.self_render_bakes()[0].empty())return 9;
    const auto low158=w.self_bake_selection();
    if(!low158.from_low158[0]||low158.from_object_update[0])
        return 10;
    if(!w.withdraw_self_object_on_kill(own,region,52U)||
       !w.self_appearance()||
       w.self_bake_selection().ids[0]!=low158.ids[0]||
       w.self_object_kills()!=2U)return 11;
    w.apply_agent({own,"bbbbbbbb-bbbb-cccc-dddd-eeeeeeeeeeee",
                   {},{},true});
    if(w.self_object_kills()!=0U||
       w.self_object_bakes()||w.self_appearance())return 12;
    if(!w.observe_self_object_bakes(own,region,53U,te))return 13;
    w.apply_region(region_packet(false));
    if(w.self_object_kills()!=0U||w.self_object_bakes()||
       w.withdraw_self_object_on_kill(own,region,53U))return 14;
    w.apply_region(region_packet());
    w.apply_agent({another,session,{},{},true});
    if(w.withdraw_self_object_on_kill(own,region,54U)||
       w.self_object_kills()!=0U)return 15;
    std::cout<<"OWN_KILLOBJECT_LOCALID_TERSE_PROVISIONAL_LOW158_LEASE_PASS\n";
}
