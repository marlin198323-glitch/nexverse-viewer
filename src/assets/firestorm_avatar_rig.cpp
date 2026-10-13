/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 * Parse Firestorm's avatar_skeleton.xml and avatar_lad.xml bind positions.
 * Rotational animation/vertex deformation are intentionally separate stages.
 */
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <tinyxml2.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <sstream>
#include <string_view>
#include <unordered_map>
namespace nexverse::assets
{
namespace
{
[[nodiscard]] bool vec3(const char* text,std::array<float,3>& out)
{
    if(!text)return false;
    std::istringstream stream{text};
    for(float& v:out)
        if(!(stream>>v)||!std::isfinite(v)||std::abs(v)>100.0f)
            return false;
    std::string extra;
    return !(stream>>extra);
}
[[nodiscard]] bool safe_xml(const std::filesystem::path& path,
                            tinyxml2::XMLDocument& document)
{
    std::error_code ec;
    const auto bytes=std::filesystem::file_size(path,ec);
    if(ec||bytes<80U||bytes>1024U*1024U)return false;
    return document.LoadFile(path.string().c_str())==tinyxml2::XML_SUCCESS;
}
}
std::optional<FirestormAvatarRig> load_firestorm_avatar_rig(
    const std::filesystem::path& root)
{
    tinyxml2::XMLDocument xml,lad;
    if(!safe_xml(root/"avatar_skeleton.xml",xml)||
       !safe_xml(root/"avatar_lad.xml",lad))
        return std::nullopt;
    const auto* skeleton=xml.FirstChildElement("linden_skeleton");
    const auto* avatar=lad.FirstChildElement("linden_avatar");
    if(!skeleton||!avatar)return std::nullopt;
    FirestormAvatarRig rig;
    std::unordered_map<std::string,std::size_t> indices;
    const std::function<bool(const tinyxml2::XMLElement*,int,
        std::array<float,3>,unsigned)> visit=
      [&](const tinyxml2::XMLElement* element,int parent,
          std::array<float,3> base,unsigned depth)->bool
    {
        if(!element||depth>48U||rig.joints.size()>=256U)
            return false;
        const char* name=element->Attribute("name");
        if(!name||!*name)return false;
        FirestormAvatarJoint joint;
        joint.name=name;
        joint.parent=parent;
        if(!vec3(element->Attribute("pos"),joint.local)||
           !vec3(element->Attribute("rot"),joint.euler_degrees))
            return false;
        for(std::size_t i=0U;i<3U;++i)
            joint.bind_world[i]=base[i]+joint.local[i];
        if(!indices.emplace(joint.name,rig.joints.size()).second)
            return false;
        const int current=static_cast<int>(rig.joints.size());
        rig.joints.push_back(joint);
        for(auto* child=element->FirstChildElement("bone");child;
            child=child->NextSiblingElement("bone"))
            if(!visit(child,current,joint.bind_world,depth+1U))
                return false;
        return true;
    };
    for(auto* bone=skeleton->FirstChildElement("bone");bone;
        bone=bone->NextSiblingElement("bone"))
        if(!visit(bone,-1,{0.0f,0.0f,0.0f},0U))
            return std::nullopt;
    const auto* ladder=avatar->FirstChildElement("skeleton");
    if(!ladder)return std::nullopt;
    for(auto* attach=ladder->FirstChildElement("attachment_point");
        attach;attach=attach->NextSiblingElement("attachment_point"))
    {
        unsigned id=0U;
        if(attach->QueryUnsignedAttribute("id",&id)!=
            tinyxml2::XML_SUCCESS||id==0U||
            id>=rig.attachment_available.size()||
            rig.attachment_available[id])
            return std::nullopt;
        const char* name=attach->Attribute("joint");
        if(!name)return std::nullopt;
        const auto parent=indices.find(name);
        // Firestorm's mScreen (HUD, 31-38) and mRoot (avatar-center 40)
        // are virtual attachment frames, not skeleton bones. Never
        // fabricate a body joint for HUD attachments.
        const bool hud=std::string_view{name}=="mScreen";
        const bool root_joint=std::string_view{name}=="mRoot";
        if(parent==indices.end() && !hud && !root_joint)
            return std::nullopt;
        rig.attachment_is_hud[id]=hud;
        std::array<float,3> offset{};
        if(!vec3(attach->Attribute("position"),offset))
            return std::nullopt;
        if(parent!=indices.end())
        {
            const auto& joint=rig.joints[parent->second].bind_world;
            for(std::size_t component=0U;component<3U;++component)
                offset[component]+=joint[component];
        }
        rig.attachment_bind_offsets[id]=offset;
        rig.attachment_available[id]=true;
        ++rig.attachment_count;
    }
    return rig.valid()?std::optional<FirestormAvatarRig>{std::move(rig)}
        :std::nullopt;
}
}
