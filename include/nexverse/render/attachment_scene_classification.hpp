/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/render/scene_parent_chain.hpp"
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
    const auto chain=resolve_scene_parent_chain(
        objects,lookup,avatars,first);
    if(!chain.valid)return AttachmentSceneKind::invalid_parent;
    if(chain.avatar_local_id==0U)return AttachmentSceneKind::world;
    const auto* avatar=find_attachment_avatar_parent(
        avatars,chain.avatar_local_id);
    if(!avatar||chain.count==0U)return AttachmentSceneKind::world;
    const auto& root=objects[chain.leaf_to_root[chain.count-1U]];
    const auto it=avatar->attachments.find(root.object_id);
    if(it==avatar->attachments.end())return AttachmentSceneKind::world;
    const auto slot=static_cast<std::size_t>(it->second);
    if(slot>=rig.is_hud.size())return AttachmentSceneKind::world;
    return rig.is_hud[slot] ? AttachmentSceneKind::hud
                            : AttachmentSceneKind::world;
}
}
