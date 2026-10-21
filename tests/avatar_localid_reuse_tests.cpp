/* MPL-2.0: a real Avatar ObjectUpdate can recycle a LocalID even if the
 * preceding UDP KillObject was dropped. No old outfit/attachment sharing. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/attachment_state.hpp"
#include <iostream>
using namespace nexverse;
static world::RegionUpdate region(bool connected=true)
{
    return {0x0000010000000200ULL,"NexVerse",{128,128,25},
        "127.0.0.1",9000U,"https://caps.invalid/seed",
        "11111111-2222-3333-4444-555555555555",
        0x10U,0U,0U,13U,20.0f,"OpenGenesisLINK",256U,256U,connected};
}
static void prim(world::WorldModel& w,std::uint32_t id,
                 std::uint32_t parent,const char* asset)
{
    world::ObjectState p;
    p.id=asset;p.local_id=id;p.parent_id=parent;
    p.pcode=protocol::pcode_prim;p.state=0x10U;
    p.sculpt_asset_id=asset;p.sculpt_type=5U;
    p.texture_entry={11,12,13};
    w.upsert_object(std::move(p));
}
int main()
{
    world::WorldModel w;
    w.apply_region(region());
    w.apply_agent({"self","session",{0,0,0},{},true});
    constexpr auto alice="aaaaaaaa-1111-2222-3333-aaaaaaaaaaaa";
    constexpr auto bob="bbbbbbbb-1111-2222-3333-bbbbbbbbbbbb";
    constexpr auto charlie="cccccccc-1111-2222-3333-cccccccccccc";
    // Rejected before valid simulator avatar ObjectUpdate admission.
    w.update_avatar_transform(alice,{1,2,3},{0,0,0,1},0U,true);
    if(w.avatars().contains(alice))return 10;
    w.update_avatar_transform(alice,{1,2,3},{0,0,0,1},51U,true);
    auto old=w.avatars().at(alice);
    old.texture_entry={10,20,30};
    old.visual_params={40,50};
    old.attachments={{"alice-worn-root",1U}};
    w.upsert_avatar(old);
    prim(w,110U,51U,"alice-worn-root");
    prim(w,111U,110U,"alice-worn-child");
    w.update_avatar_transform(charlie,{8,9,10},{0,0,0,1},53U,true);
    prim(w,310U,53U,"charlie-worn-root");
    // A Full/Compressed ObjectUpdate arrives for *Bob* reusing 51,
    // but KillObject(51) was lost on UDP. Only Bob may own LocalID 51.
    w.update_avatar_transform(bob,{4,5,6},{0,0,0,1},51U,true);
    if(w.avatars().contains(alice)||!w.avatars().contains(bob)||
       !w.avatars().contains(charlie)||
       w.avatars().at(bob).local_id!=51U||
       w.avatars().at(bob).coarse_position_only||
       !w.avatars().at(bob).simulator_object_update_seen||
       w.objects().contains(110U)||w.objects().contains(111U)||
       !w.objects().contains(310U)||
       w.remote_avatar_local_id_rebinds()!=1U||
       w.remote_avatar_kills()!=0U||w.removed_prim_kills()!=0U)
        return 1;
    const auto same_rev=w.object_revision();
    // Network duplicate with identical UUID and LocalID is idempotent.
    w.update_avatar_transform(bob,{4,5,6},{0,0,0,1},51U,true);
    if(w.remote_avatar_local_id_rebinds()!=1U||
       w.object_revision()!=same_rev)return 2;
    auto bob_meta=w.avatars().at(bob);
    bob_meta.texture_entry={92,93,94};
    bob_meta.visual_params={64,65};
    bob_meta.attachments={{"bob-live-worn",5U}};
    w.upsert_avatar(bob_meta);
    prim(w,210U,51U,"bob-live-worn");
    prim(w,211U,210U,"bob-live-child");
    const auto before_new_id=w.object_revision();
    // Same agent's UUID gets a new LocalID without another UUID.
    // Preserve genuine Low158 / animation metadata, but retire all
    // prim attachments still bound to the previous LocalID 51.
    w.update_avatar_transform(bob,{11,12,13},{0,0,0,1},54U,true);
    if(!w.avatars().contains(bob)||
       w.avatars().at(bob).local_id!=54U||
       w.avatars().at(bob).texture_entry!=bob_meta.texture_entry||
       w.avatars().at(bob).visual_params!=bob_meta.visual_params||
       w.avatars().at(bob).attachments.size()!=1U||
       w.avatars().at(bob).attachments[0].object_id!="bob-live-worn"||
       w.avatars().at(bob).attachments[0].attachment_point!=5U||
       w.objects().contains(210U)||w.objects().contains(211U)||
       w.object_revision()!=before_new_id+1U||
       w.remote_avatar_local_id_rebinds()!=2U||
       w.remote_avatar_kills()!=0U||w.removed_prim_kills()!=0U)
        return 3;
    // A stale coarse packet for the original avatar only creates a
    // map dot; it must not inherit Bob's old mesh or simulator body.
    protocol::CoarseLocationUpdate coarse;
    coarse.avatars={{alice,{20,30,40}},{bob,{99,99,99}}};
    if(!w.apply_coarse_avatar_locations(coarse)||
       !w.avatars().at(alice).coarse_position_only||
       w.avatars().at(alice).simulator_object_update_seen||
       w.avatars().at(alice).local_id!=0U||
       w.avatars().at(bob).position.x!=11.0f)
        return 4;
    // A normal KillObject after verified rebinding is STILL counted,
    // unlike a locally inferred rebind/missed prior KillObject.
    if(!w.kill_simulator_local_id(54U)||
       w.remote_avatar_kills()!=1U||
       w.remote_avatar_local_id_rebinds()!=2U)return 5;
    // A new region (or logged-in identity) must clear historical counters.
    w.apply_region(region(false));
    if(w.remote_avatar_local_id_rebinds()!=0U||
       w.remote_avatar_kills()!=0U||
       w.removed_prim_kills()!=0U||!w.avatars().empty())
        return 6;
    w.update_avatar_transform(alice,{9,9,9},{0,0,0,1},51U,true);
    if(w.avatars().contains(alice))return 11;
    std::cout<<"CURRENT_REMOTE_LOCALID_REUSE_RETIRES_OLD_GPU_MESH_ONLY_PASS\n";
}
