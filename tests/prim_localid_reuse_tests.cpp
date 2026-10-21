/* MPL-2.0: a new prim UUID at a recycled simulator LocalID
   retires old nested mesh descendants, without fabricating KillObject. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <iostream>
using namespace nexverse;
namespace {
world::RegionUpdate region(std::uint64_t handle=0x10000000200ULL,
                            bool connected=true)
{
    return {handle,"NexVerse",{128,128,25},"127.0.0.1",9000U,
            "","",0U,0U,0U,13U,20.0f,"",256U,256U,connected};
}
world::ObjectState prim(std::uint32_t id,const char* uuid,
                        std::uint32_t parent=0U)
{
    world::ObjectState p;
    p.local_id=id;p.id=uuid;p.pcode=protocol::pcode_prim;
    p.parent_id=parent;p.rotation={0,0,0,1};
    p.sculpt_asset_id=uuid;p.sculpt_type=5U;
    p.texture_entry={1U,2U,3U};
    return p;
}
}
int main()
{
    world::WorldModel w;
    w.apply_region(region());
    w.apply_agent({"self","session",{},{},true});
    w.upsert_object(prim(50U,"old-root"));
    w.upsert_object(prim(51U,"old-child",50U));
    w.upsert_object(prim(52U,"old-grandchild",51U));
    w.upsert_object(prim(70U,"unrelated"));
    w.upsert_object(prim(71U,"unrelated-child",70U));
    if(w.objects().size()!=5U||w.prim_local_id_rebinds()!=0U)
        return 1;
    // Same UUID refresh must preserve children, not count a rebind.
    auto same=prim(50U,"old-root");same.position={1,2,3};
    w.upsert_object(same);
    if(w.objects().size()!=5U||w.prim_local_id_rebinds()!=0U||
       w.objects().at(50U).position.x!=1.0f)return 2;
    // Another UUID owns 50 now; old descendants MUST disappear.
    auto replacement=prim(50U,"new-root");replacement.position={5,6,7};
    const auto old_revision=w.object_revision();
    w.upsert_object(replacement);
    if(w.objects().size()!=3U||
       w.objects().at(50U).id!="new-root"||
       w.objects().contains(51U)||w.objects().contains(52U)||
       !w.objects().contains(70U)||!w.objects().contains(71U)||
       w.prim_local_id_rebinds()!=1U||
       w.remote_avatar_kills()!=0U||w.removed_prim_kills()!=0U||
       w.object_revision()<=old_revision)return 3;
    // Delayed High9 may only move known new root, never resurrect old mesh.
    if(!w.apply_object_terse(50U,{8,9,10},{0,0,0,1})||
       w.apply_object_terse(51U,{0,0,0},{0,0,0,1})||
       w.objects().at(50U).id!="new-root")return 4;
    // Identity-less updates are ignored; no ghost.
    w.upsert_object(prim(99U,""));
    w.upsert_object(prim(0U,"invalid"));
    if(w.objects().size()!=3U||w.prim_local_id_rebinds()!=1U)
        return 5;
    // Separate real KillObject removes the replacement and counts it,
    // while an implicit rebind never increments packet-derived totals.
    if(!w.kill_simulator_local_id(50U)||
       w.removed_prim_kills()!=1U||
       w.prim_local_id_rebinds()!=1U)return 6;
    // Region lease reset; disconnected updates cannot repopulate scene.
    w.apply_region(region(0x10000000300ULL));
    if(!w.objects().empty()||w.prim_local_id_rebinds()!=0U)return 7;
    w.apply_region(region(0x10000000300ULL,false));
    w.upsert_object(prim(90U,"disconnected"));
    if(!w.objects().empty())return 8;
    std::cout<<"PRIM_LOCALID_REBIND_NESTED_MESH_GHOST_PREVENTION_PASS\n";
}
