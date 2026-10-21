/* MPL-2.0: real KillObject terminates remote avatar & attached linksets. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/attachment_state.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <iostream>
using namespace nexverse;
static world::RegionUpdate region(bool active)
{
    return {0x0000010000000200ULL,"NexVerse",{128,128,25},
        "127.0.0.1",9000U,"https://caps.invalid/seed",
        "11111111-2222-3333-4444-555555555555",
        0x10U,0U,0U,13U,20.0f,"OpenGenesisLINK",256U,256U,active};
}
static void add(world::WorldModel& w,std::uint32_t local,
                std::uint32_t parent,std::string id,std::uint8_t state=0U)
{
    world::ObjectState prim;
    prim.local_id=local;
    prim.parent_id=parent;
    prim.id=std::move(id);
    prim.pcode=protocol::pcode_prim;
    prim.state=state;
    prim.sculpt_asset_id="verified-mesh";
    prim.sculpt_type=5U;
    prim.texture_entry={1U,2U,3U};
    w.upsert_object(std::move(prim));
}
int main()
{
    world::WorldModel w;
    w.apply_region(region(true));
    w.apply_agent({"own-avatar","session",{0,0,0},{},true});
    constexpr auto alice="11111111-1111-1111-1111-aaaaaaaaaaaa";
    constexpr auto bob="22222222-2222-2222-2222-bbbbbbbbbbbb";
    w.update_avatar_transform(alice,{10,20,30},{0,0,0,1},51U,true);
    w.update_avatar_transform(bob,{50,60,70},{0,0,0,1},52U,true);
    auto full=w.avatars().at(alice);
    full.texture_entry={42U,43U};
    full.visual_params={21U,22U};
    full.attachments={{"worn-root",1U}};
    w.upsert_avatar(std::move(full));
    add(w,100U,51U,"worn-root",0x10U);
    add(w,101U,100U,"sleeve-child",0U);
    add(w,102U,101U,"nested-child",0U);
    add(w,200U,52U,"bob-attachment",0x10U);
    add(w,300U,0U,"world-building");
    add(w,301U,300U,"building-child");
    add(w,400U,999U,"orphan-foreign");
    if(w.avatars().size()!=2U||w.objects().size()!=7U||
       world::infer_avatar_attachment_points(w.objects(),51U).size()!=1U)
        return 1;
    // A real low KillObject wire packet lists avatar local 51.
    const std::vector<std::uint8_t> kill_bytes{
        1U,51U,0U,0U,0U};
    const auto kill=protocol::decode_kill_object(kill_bytes);
    if(!kill||kill->local_ids.size()!=1U||kill->local_ids[0]!=51U||
       !w.kill_simulator_local_id(kill->local_ids[0]))return 2;
    if(w.avatars().contains(alice)||!w.avatars().contains(bob)||
       w.objects().contains(100U)||w.objects().contains(101U)||
       w.objects().contains(102U)||
       !world::infer_avatar_attachment_points(w.objects(),51U).empty()||
       !w.objects().contains(200U)||!w.objects().contains(300U)||
       !w.objects().contains(301U)||!w.objects().contains(400U)||
       w.remote_avatar_kills()!=1U||w.removed_prim_kills()!=3U)
        return 3;
    // No stale avatar resurrected from its previous appearance/High20.
    // A new coarse map sighting must stay map-only until a new full
    // ObjectUpdate establishes an actual simulator object.
    protocol::CoarseLocationUpdate coarse;
    coarse.avatars={{alice,{12,20,28}}};
    if(!w.apply_coarse_avatar_locations(coarse)||
       !w.avatars().contains(alice)||
       w.avatars().at(alice).simulator_object_update_seen||
       !w.avatars().at(alice).coarse_position_only||
       w.avatars().at(alice).local_id!=0U||
       !w.avatars().at(alice).texture_entry.empty())
        return 4;
    const auto old_rev=w.object_revision();
    if(w.kill_simulator_local_id(51U)||
       w.kill_simulator_local_id(0U)||
       w.object_revision()!=old_rev||
       w.remote_avatar_kills()!=1U||w.removed_prim_kills()!=3U)
       return 5;
    // Real prim root KillObject removes all its children, but unrelated
    // avatar attachment and world objects must be retained.
    if(!w.kill_simulator_local_id(300U)||
       w.objects().contains(300U)||w.objects().contains(301U)||
       !w.objects().contains(200U)||
       !w.avatars().contains(bob)||
       w.removed_prim_kills()!=5U)return 6;
    const auto next_rev=w.object_revision();
    if(!w.kill_simulator_local_id(52U)||
       w.avatars().contains(bob)||w.objects().contains(200U)||
       w.object_revision()!=next_rev+1U||
       w.remote_avatar_kills()!=2U||w.removed_prim_kills()!=6U)
        return 7;
    // Disconnected circuits must not accept stale removals.
    add(w,550U,0U,"new-prim");
    w.apply_region(region(false));
    if(w.kill_simulator_local_id(550U)||!w.objects().empty()||
       w.remote_avatar_kills()!=0U||w.removed_prim_kills()!=0U)
        return 8;
    std::cout<<"KILLOBJECT_REMOTE_AVATAR_ROOT_CHILDREN_AND_NO_GHOST_PASS\n";
}
