#include "nexverse/render/attachment_scene_classification.hpp"
#include <array>
#include <iostream>
#include <unordered_map>
using namespace nexverse::render;
int main()
{
 RenderAvatarProxy avatar{};
 avatar.local_id=51;
 avatar.attachments["hud"]=31;
 avatar.attachments["shirt"]=1;
 std::array<RenderAvatarProxy,1> avatars{avatar};
 RenderAvatarBindRig rig{};
 rig.is_hud[31]=true;
 std::array<RenderSceneObject,8> objects{};
 objects[0].local_id=100;
 objects[1].local_id=200;objects[1].parent_id=51;objects[1].object_id="hud";
 objects[2].local_id=201;objects[2].parent_id=200;
 objects[3].local_id=202;objects[3].parent_id=201;
 objects[4].local_id=300;objects[4].parent_id=51;objects[4].object_id="shirt";
 objects[5].local_id=301;objects[5].parent_id=300;
 objects[6].local_id=700;objects[6].parent_id=700;
 objects[7].local_id=800;objects[7].parent_id=9999;
 std::unordered_map<std::uint32_t,std::size_t> by_id;
 for(std::size_t i=0;i<objects.size();++i)by_id[objects[i].local_id]=i;
 const auto check=[&](std::size_t i){return classify_attachment_scene(objects,by_id,avatars,rig,i);};
 if(check(0)!=AttachmentSceneKind::world)return 1;
 if(check(1)!=AttachmentSceneKind::hud)return 2;
 if(check(2)!=AttachmentSceneKind::hud)return 3;
 if(check(3)!=AttachmentSceneKind::hud)return 4;
 if(check(4)!=AttachmentSceneKind::world)return 5;
 if(check(5)!=AttachmentSceneKind::world)return 6;
 if(check(6)!=AttachmentSceneKind::invalid_parent)return 7;
 if(check(7)!=AttachmentSceneKind::world)return 8;
 if(check(100)!=AttachmentSceneKind::invalid_parent)return 9;
 objects[6].parent_id=800;objects[7].parent_id=700;
 if(check(6)!=AttachmentSceneKind::invalid_parent)return 10;
 std::cout<<"HUD_ATTACHMENT_LINKSET_PASS\n";
}
