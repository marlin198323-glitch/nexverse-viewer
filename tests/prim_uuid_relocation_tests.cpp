/* MPL-2.0: genuine prim UUID cannot live at two LocalIDs or leave
 * orphaned GPU mesh descendants when KillObject was lost. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/simulator_scene_validity.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
static world::RegionUpdate region(bool connected=true)
{
    return {0x10000000200ULL,"Test Region",{128,128,25},
        "127.0.0.1",9000,"","",0,0,0,13,20.0f,"",256,256,connected};
}
static world::ObjectState prim(std::uint32_t id,const char* uuid,
                               std::uint32_t parent=0)
{
    world::ObjectState p{};
    p.local_id=id;p.id=uuid;p.pcode=protocol::pcode_prim;
    p.parent_id=parent;p.sculpt_asset_id=uuid;p.sculpt_type=5;
    p.rotation={0,0,0,1};return p;
}
int main()
{
    using std::uint32_t;
    world::WorldModel w;
    w.apply_region(region());
    w.apply_agent({"own","session",{},{},true});
    w.upsert_object(prim(50,"same-id"));
    w.upsert_object(prim(51,"old-child",50));
    w.upsert_object(prim(52,"grandchild",51));
    w.upsert_object(prim(200,"building"));
    w.upsert_object(prim(201,"building-child",200));
    const auto first_revision=w.object_revision();
    // In the absence of KillObject the UUID has moved to LocalID 90.
    // All older descendants must stop rendering.
    auto relocated=prim(90,"same-id");
    relocated.position={9,10,11};
    w.upsert_object(relocated);
    if(w.objects().contains(50)||w.objects().contains(51)||
       w.objects().contains(52)||w.objects().size()!=3||
       w.objects().at(90).id!="same-id"||
       w.objects().at(90).position.x!=9.0f||
       !w.objects().contains(200)||!w.objects().contains(201)||
       w.prim_uuid_relocations()!=1||w.prim_local_id_rebinds()!=0||
       w.remote_avatar_kills()!=0||w.removed_prim_kills()!=0||
       w.object_revision()<=first_revision)return 1;
    // Repetition for unchanged UUID/localID cannot trigger more removal.
    w.upsert_object(relocated);
    if(w.prim_uuid_relocations()!=1||w.objects().size()!=3)return 2;
    w.upsert_object(prim(91,"new-child",90));
    // Invalid replacement has no permission to evict the known good tree.
    auto invalid=prim(90,"untrusted");
    invalid.position.x=std::numeric_limits<float>::quiet_NaN();
    const auto rev=w.object_revision();
    w.upsert_object(invalid);
    if(w.objects().at(90).id!="same-id"||
       !w.objects().contains(91)||w.object_revision()!=rev||
       w.invalid_scene_updates()!=1||w.prim_local_id_rebinds()!=0)
       return 3;
    invalid=prim(90,"untrusted");
    invalid.scale.x=std::numeric_limits<float>::infinity();
    w.upsert_object(invalid);
    if(w.invalid_scene_updates()!=2||w.objects().at(90).id!="same-id")
        return 4;
    invalid=prim(90,"untrusted");
    invalid.rotation.y=std::numeric_limits<float>::quiet_NaN();
    w.upsert_object(invalid);
    if(w.invalid_scene_updates()!=3||w.objects().at(90).id!="same-id")
        return 5;
    // Different UUID now replaces 90: this is a LocalID collision, not
    // another UUID relocation.
    w.upsert_object(prim(90,"replacement"));
    if(w.objects().contains(91)||w.objects().at(90).id!="replacement"||
       w.prim_local_id_rebinds()!=1||w.prim_uuid_relocations()!=1||
       w.removed_prim_kills()!=0)return 6;
    // Bad remote Full/Compressed avatar pose may not displace a valid
    // avatar or delete attachments when its LocalID would collide.
    w.update_avatar_transform("old-avatar",{1,2,3},{0,0,0,1},70,true);
    w.upsert_object(prim(71,"attachment",70));
    const auto avatar_count=w.avatars().size();
    const auto invalid_count=w.invalid_scene_updates();
    w.update_avatar_transform("new-avatar",
        {std::numeric_limits<float>::quiet_NaN(),2,3},
        {0,0,0,1},70,true);
    w.update_avatar_transform("new-avatar",{1,2,3},{0,0,0,0},70,true);
    if(w.avatars().size()!=avatar_count||
       w.avatars().contains("new-avatar")||
       w.objects().at(71).id!="attachment"||
       w.invalid_scene_updates()!=invalid_count+2)return 7;
    // Live decoded Terse/Full guard must exclude nonsensical rotations.
    if(!world::valid_simulator_avatar_pose({1,2,3},{0,0,0,1})||
       world::valid_simulator_avatar_pose({1,2,3},{0,0,0,0})||
       world::valid_simulator_avatar_pose({1,2,3},{0,0,0,9})||
       world::finite_simulator_position({1000001.0f,0,0}))return 8;
    // Old region counters may not leak into newly authenticated scene.
    w.apply_region(region(false));
    if(w.prim_uuid_relocations()!=0||w.prim_local_id_rebinds()!=0||
       w.invalid_scene_updates()!=0||!w.objects().empty())return 9;
    std::cout<<"PRIM_UUID_RELOCATION_VALIDATION_NO_GHOST_MESH_PASS\n";
}