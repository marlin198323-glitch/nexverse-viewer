/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/render/avatar_attachment_anchor.hpp"
#include <cstddef>
#include <cstdint>
#include <span>
#include <unordered_map>
namespace nexverse::render
{
// HUD attachments must render in a distinct screen-space pass, not in
// the region. Resolve the attachment root through its complete prim chain.
enum class AttachmentSceneKind : std::uint8_t { world, hud, invalid_parent };
[[nodiscard]] inline AttachmentSceneKind classify_attachment_scene(
    std::span<const RenderSceneObject> objects,
    const std::unordered_map<std::uint32_t,std::size_t>& lookup,
    std::span<const RenderAvatarProxy> avatars,
    const RenderAvatarBindRig& rig,std::size_t first) noexcept
{
    if(first>=objects.size())return AttachmentSceneKind::invalid_parent;
    std::size_t current=first;
    for(std::size_t depth=0U;depth<objects.size()+1U&&depth<128U;++depth)
    {
        const auto& object=objects[current];
        if(object.parent_id==0U)return AttachmentSceneKind::world;
        if(const auto* avatar=find_attachment_avatar_parent(
            avatars,object.parent_id))
        {
            const auto it=avatar->attachments.find(object.object_id);
            if(it==avatar->attachments.end())
                return AttachmentSceneKind::world;
            const auto slot=static_cast<std::size_t>(it->second);
            if(slot>=rig.is_hud.size())return AttachmentSceneKind::world;
            return rig.is_hud[slot] ? AttachmentSceneKind::hud
                                   : AttachmentSceneKind::world;
        }
        const auto parent=lookup.find(object.parent_id);
        if(parent==lookup.end())return AttachmentSceneKind::world;
        if(parent->second>=objects.size()||parent->second==current)
            return AttachmentSceneKind::invalid_parent;
        current=parent->second;
    }
    return AttachmentSceneKind::invalid_parent;
}
}
