/* MPL-2.0: real OpenGenesisLINK attachment points in LLUDP root prim State */
#include "nexverse/world/attachment_state.hpp"
#include "nexverse/render/attachment_scene_classification.hpp"
#include <iostream>
#include <array>
#include <unordered_map>
int main()
{
    using namespace nexverse;
    world::ObjectState sleeve{};
    sleeve.local_id=100U;sleeve.id="sleeve";
    sleeve.pcode=protocol::pcode_prim;sleeve.parent_id=51U;
    sleeve.state=0x10U; // left shoulder point 1
    world::ObjectState hud=sleeve;
    hud.local_id=200U;hud.id="hud";hud.state=0xF1U; // HUD slot 31
    world::ObjectState other=sleeve;
    other.local_id=300U;other.id="unrelated";other.parent_id=88U;
    world::ObjectState world_object=sleeve;
    world_object.local_id=400U;world_object.id="world";world_object.parent_id=0U;
    world::ObjectState unknown=sleeve;
    unknown.local_id=500U;unknown.id="unknown";unknown.state=0xFFU;
    world::ObjectState invalid=sleeve;
    invalid.local_id=600U;invalid.id="none";invalid.state=0U;
    world::ObjectState child=sleeve;
    child.local_id=700U;child.id="child";child.parent_id=100U;
    std::map<std::uint32_t,world::ObjectState> objects;
    for(const auto& o:{sleeve,hud,other,world_object,unknown,invalid,child})
        objects.emplace(o.local_id,o);
    if(world::decode_server_attachment_point(0x10U)!=1U ||
       world::decode_server_attachment_point(0xF1U)!=31U ||
       world::decode_server_attachment_point(0U)!=0U)return 1;
    auto selected=world::infer_avatar_attachment_points(objects,51U);
    if(selected.size()!=2U||selected.at("sleeve")!=1U||
       selected.at("hud")!=31U||selected.contains("unrelated")||
       selected.contains("child")||selected.contains("none")||
       selected.contains("unknown"))return 2;
    if(!world::infer_avatar_attachment_points(objects,0U).empty()||
       !world::infer_avatar_attachment_points(objects,901U).empty())
       return 3;
    // Low158 was captured before the latest ObjectUpdate. A recent
    // real prim State must override the old appearance point, otherwise
    // attachments remain on old bones or fail to detach.
    std::map<std::string,std::uint8_t,std::less<>> low158{
        {"sleeve",31U}, // wrong old HUD state; current prim says shoulder
        {"hud",1U},     // wrong old shoulder; current prim says HUD
        {"world",1U},   // real prim is not attached to this avatar
        {"unrelated",1U}, // actually belongs to avatar 88
        {"unknown",1U}, // current object has invalid slot
        {"none",1U},    // current object State=0 (detached)
        {"pending",5U}, // true Low158 entry; prim not loaded yet
    };
    auto merged=world::reconcile_avatar_attachments(objects,51U,low158);
    if(merged.points.size()!=3U||
       merged.points.at("sleeve")!=1U||
       merged.points.at("hud")!=31U||
       merged.points.at("pending")!=5U||
       merged.pending_appearance_points!=1U||
       merged.stale_appearance_points!=4U||
       merged.current_object_points!=2U)return 4;
    render::RenderAvatarProxy avatar{};
    avatar.local_id=51U;
    avatar.attachments.insert(merged.points.begin(),merged.points.end());
    render::RenderAvatarBindRig rig{};
    rig.available[1U]=true;rig.available[31U]=true;
    rig.is_hud[31U]=true;
    std::array<render::RenderSceneObject,3U> scene{};
    scene[0].local_id=100U;scene[0].object_id="sleeve";
    scene[0].parent_id=51U;
    scene[1].local_id=200U;scene[1].object_id="hud";
    scene[1].parent_id=51U;
    scene[2].local_id=201U;scene[2].parent_id=200U;
    std::unordered_map<std::uint32_t,std::size_t> lookup{
        {100U,0U},{200U,1U},{201U,2U}};
    std::array<render::RenderAvatarProxy,1U> avatars{avatar};
    const auto kind=[&](std::size_t i)
    {
        return render::classify_attachment_scene(scene,lookup,avatars,rig,i);
    };
    if(kind(0U)!=render::AttachmentSceneKind::world||
       kind(1U)!=render::AttachmentSceneKind::hud||
       kind(2U)!=render::AttachmentSceneKind::hud)return 5;
    // Removing the root from the current region prevents resurrecting old
    // attachments after teleport or a stale object kill.
    objects.erase(200U);
    selected=world::infer_avatar_attachment_points(objects,51U);
    if(selected.contains("hud")||selected.size()!=1U)return 6;
    // With the prim gone the old Low158 entry is only pending, not
    // GPU-confirmed. A matching object with new State changes to HUD.
    merged=world::reconcile_avatar_attachments(objects,51U,low158);
    if(merged.points.at("hud")!=1U||
       merged.pending_appearance_points!=2U)return 7;
    objects.at(100U).state=0xF1U;
    merged=world::reconcile_avatar_attachments(objects,51U,low158);
    if(merged.points.at("sleeve")!=31U)return 8;
    objects.at(100U).state=0U;
    merged=world::reconcile_avatar_attachments(objects,51U,low158);
    if(merged.points.contains("sleeve"))return 9;
    objects.at(100U).parent_id=88U;
    objects.at(100U).state=0x10U;
    merged=world::reconcile_avatar_attachments(objects,51U,low158);
    if(merged.points.contains("sleeve")||
       !world::reconcile_avatar_attachments(objects,0U,low158)
            .points.empty())return 10;
    std::cout<<"OPENGENESISLINK_OBJECTUPDATE_ATTACHMENT_STATE_AND_HUD_PASS\n";
}
