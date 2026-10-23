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
    // Alpha.995-1004: late full/compressed self ObjectUpdate packets
    // from a killed LocalID must not recreate own movement/pose binding.
    // This is separate from the old COF outfit TE replay guard.
    if(w.killed_self_local_id_guards()!=1U||
       !w.reject_retired_self_object_pose(own,region,51U,te)||
       !w.reject_retired_self_object_pose(own,region,51U,{})||
       !w.reject_retired_self_object_pose(
            own,region,51U,std::vector<std::uint8_t>{1U,2U})||
       w.killed_self_pose_packets_blocked()!=3U||
       w.retired_self_pose_packets_blocked()!=3U||
       w.self_object_bakes())return 16;
    if(w.reject_retired_self_object_pose(another,region,51U,te)||
       w.reject_retired_self_object_pose(own,region+1U,51U,te)||
       w.reject_retired_self_object_pose(own,region,0U,te)||
       w.killed_self_pose_packets_blocked()!=3U)return 17;
    // Genuine new baked TE on same numerical LocalID is permitted:
    // the original old TE remains rejected AFTER the new TE is bound.
    auto rebind_te=te;
    for(std::size_t j=18U;j<34U;++j)
        rebind_te[j]=0x77U;
    if(w.reject_retired_self_object_pose(own,region,51U,rebind_te)||
       !w.observe_self_object_bakes(own,region,51U,rebind_te)||
       w.reject_retired_self_object_pose(own,region,51U,{})||
       !w.reject_retired_self_object_pose(own,region,51U,te)||
       !w.self_object_bakes()||
       w.killed_self_pose_packets_blocked()!=4U||
       w.killed_self_local_id_guards()!=1U)return 18;
    // Repeated real KillObject of reused numeric LocalID retains both
    // old body fingerprints, deduplicates and stays bounded.
    if(!w.withdraw_self_object_on_kill(own,region,51U)||
       w.killed_self_local_id_guards()!=2U||
       !w.reject_retired_self_object_pose(own,region,51U,rebind_te)||
       !w.reject_retired_self_object_pose(own,region,51U,te))
        return 19;
    // The extra test kill is a direct authenticated model event, not a
    // dispatcher duplicate; reconcile its count for the following
    // existing true-KillObject assertions.
    w.apply_agent({own,"eeeeeeee-bbbb-cccc-dddd-eeeeeeeeeeee",{}, {},true});
    w.apply_agent({own,session,{},{},true});
    if(w.killed_self_local_id_guards()!=0U||
       w.killed_self_pose_packets_blocked()!=0U||
       w.reject_retired_self_object_pose(own,region,51U,te))
        return 20;
    // A fresh session has no prior Low158 or killed ID lease.
    if(!w.observe_self_object_bakes(own,region,51U,te))return 21;
    if(!w.withdraw_self_object_on_kill(own,region,51U)||
       w.self_object_kills()!=1U)return 22;
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
    // Bounded quarantine on many distinct authentic self root kills.
    // Real dispatcher verifies each currently bound LocalID separately.
    world::WorldModel many;
    many.apply_region(region_packet());
    many.apply_agent({own,session,{},{},true});
    for(std::uint32_t local=100U;local<120U;++local)
    {
        if(!many.observe_self_object_bakes(own,region,local,te)||
           !many.withdraw_self_object_on_kill(own,region,local)||
           many.killed_self_local_id_guards()!=
               (local<116U?local-99U:16U))return 23;
    }
    if(many.reject_retired_self_object_pose(own,region,100U,te)||
       !many.reject_retired_self_object_pose(own,region,104U,te)||
       !many.reject_retired_self_object_pose(own,region,119U,te))
        return 24;
    many.apply_region(region_packet(false));
    if(many.killed_self_local_id_guards()!=0U||
       many.killed_self_pose_packets_blocked()!=0U)return 25;

    std::cout<<"OWN_KILLOBJECT_LOCALID_TERSE_PROVISIONAL_LOW158_LEASE_PASS\n";
}
