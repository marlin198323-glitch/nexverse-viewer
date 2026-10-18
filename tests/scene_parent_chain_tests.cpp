/* MPL-2.0 */
#include "nexverse/render/scene_parent_chain.hpp"
#include "nexverse/render/attachment_scene_classification.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace nexverse::render;
int main()
{
    RenderAvatarProxy self{};
    self.local_id=55U;
    self.attachments["worn"]=1U;
    self.attachments["hud"]=31U;
    RenderAvatarBindRig rig{};
    rig.is_hud[31U]=true;
    std::array<RenderAvatarProxy,1> avatars{self};
    std::vector<RenderSceneObject> objects(129U);
    std::unordered_map<std::uint32_t,std::size_t> lookup;
    for(std::size_t i=0;i<objects.size();++i)
    {
        objects[i].local_id=static_cast<std::uint32_t>(1000U+i);
        objects[i].parent_id=i==0U?55U:objects[i-1U].local_id;
        lookup[objects[i].local_id]=i;
    }
    objects[0].object_id="worn";
    const auto deep=resolve_scene_parent_chain(objects,lookup,avatars,20U);
    if(!deep.valid||deep.count!=21U||deep.avatar_local_id!=55U||
       deep.leaf_to_root[0]!=20U||deep.leaf_to_root[20U]!=0U)
        return 1;
    if(classify_attachment_scene(objects,lookup,avatars,rig,20U)!=
       AttachmentSceneKind::world)return 2;
    objects[0].object_id="hud";
    if(classify_attachment_scene(objects,lookup,avatars,rig,20U)!=
       AttachmentSceneKind::hud)return 3;
    const auto at_limit=resolve_scene_parent_chain(objects,lookup,avatars,127U);
    if(!at_limit.valid||at_limit.count!=128U||
       at_limit.avatar_local_id!=55U)return 4;
    const auto over_limit=resolve_scene_parent_chain(objects,lookup,avatars,128U);
    if(over_limit.valid||
       classify_attachment_scene(objects,lookup,avatars,rig,128U)!=
       AttachmentSceneKind::invalid_parent)return 5;
    // Hard cycle must not leak avatar positions or loop forever.
    objects[0].parent_id=objects[5].local_id;
    if(resolve_scene_parent_chain(objects,lookup,avatars,5U).valid||
       classify_attachment_scene(objects,lookup,avatars,rig,5U)!=
       AttachmentSceneKind::invalid_parent)return 6;
    objects[0].parent_id=999999U;
    const auto incomplete=resolve_scene_parent_chain(objects,lookup,avatars,20U);
    if(!incomplete.valid||!incomplete.missing_parent||
       incomplete.avatar_local_id!=0U)return 7;
    // Unknown avatar-parent is NOT evidence for an attachment.
    if(classify_attachment_scene(objects,lookup,avatars,rig,20U)!=
       AttachmentSceneKind::world)return 8;
    objects[0].parent_id=55U;
    if(resolve_scene_parent_chain(objects,lookup,avatars,objects.size()).valid)
        return 9;
    // Original object type, no invented attachment bone or simulator packet.
    std::cout<<"OGL_SCENE_PARENT_128_DEPTH_PASS\n";
}
