/* MPL-2.0: High9 ImprovedTerse updates pose only, never object identity. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/attachment_state.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
int main()
{
    world::WorldModel model;
    model.apply_region({
        0x0000010000000200ULL,"NexVerse",{128.0f,128.0f,25.0f},
        "127.0.0.1",9000U,"https://caps.example/seed",
        "aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb",0x10U,0U,0U,
        13U,20.0f,"OpenGenesisLINK Test",256U,256U,true});
    world::ObjectState sleeve;
    sleeve.local_id=42U;sleeve.id="live-sleeve";
    sleeve.pcode=protocol::pcode_prim;sleeve.state=0x10U;
    sleeve.parent_id=51U;
    sleeve.position={1.0f,2.0f,3.0f};
    sleeve.rotation={0.0f,0.0f,0.0f,1.0f};
    sleeve.scale={0.5f,0.6f,0.7f};
    sleeve.shape.path_curve=0x10U;
    sleeve.texture_entry={10U,11U,12U};
    sleeve.parsed_texture_entry=protocol::TextureEntry{};
    sleeve.default_texture=protocol::TextureEntryDefaultFace{};
    sleeve.sculpt_asset_id="actual-mesh-asset";
    sleeve.sculpt_type=0x05U;
    model.upsert_object(sleeve);
    if(model.object_revision()!=1U)return 1;
    if(world::infer_avatar_attachment_points(model.objects(),51U)
        .at("live-sleeve")!=1U)return 2;
    const auto move_rotation=protocol::Quaternion{
        0.0f,0.0f,0.25f,0.96824583f};
    if(!model.apply_object_terse(
        42U,{1.5f,2.5f,3.5f},move_rotation)||
       model.object_revision()!=2U)return 3;
    const auto& updated=model.objects().at(42U);
    if(updated.position.x!=1.5f||updated.rotation.z!=0.25f||
       updated.id!="live-sleeve"||updated.state!=0x10U||
       updated.parent_id!=51U||updated.scale.z!=0.7f||
       updated.shape.path_curve!=0x10U||
       updated.texture_entry!=sleeve.texture_entry||
       !updated.parsed_texture_entry||!updated.default_texture||
       updated.sculpt_asset_id!="actual-mesh-asset"||
       !updated.is_mesh()||
       world::infer_avatar_attachment_points(model.objects(),51U)
           .at("live-sleeve")!=1U)return 4;
    if(model.apply_object_terse(
            42U,{1.5f,2.5f,3.5f},move_rotation)||
       model.object_revision()!=2U)return 5;
    if(model.apply_object_terse(
            300U,{2.0f,3.0f,4.0f},move_rotation)||
       model.objects().size()!=1U||
       model.object_revision()!=2U)return 6;
    const auto before=model.snapshot().objects;
    if(model.apply_object_terse(
          42U,{std::numeric_limits<float>::quiet_NaN(),3.0f,4.0f},
              move_rotation)||
       model.apply_object_terse(
          42U,{1.0f,3.0f,4.0f},{0.0f,0.0f,0.0f,0.0f})||
       model.object_revision()!=2U||
       model.objects().at(42U).position.x!=before.at(42U).position.x)
        return 7;
    model.remove_object(42U);
    if(model.apply_object_terse(
          42U,{1.0f,2.0f,3.0f},move_rotation)||
       !model.objects().empty()||
       world::infer_avatar_attachment_points(model.objects(),51U).size()!=0U)
        return 8;
    model.upsert_object(sleeve);
    model.begin_region_transition({
        0x0000010000000300ULL,"Other Region",{20.0f,20.0f,25.0f},
        "127.0.0.1",9001U,"https://caps.example/other",
        "bbbbbbbb-1111-2222-3333-cccccccccccc",0x10U,0U,0U,
        13U,20.0f,"OpenGenesisLINK Test",256U,256U,true});
    if(model.apply_object_terse(
          42U,{1.0f,2.0f,3.0f},move_rotation)||
       !model.objects().empty())return 9;
    std::cout<<"OPENGENESIS_TERSE_ATTACHMENT_AND_MESH_PRESERVED_PASS\n";
}
