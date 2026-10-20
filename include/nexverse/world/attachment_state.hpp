/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/world/world_model.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <unordered_set>
namespace nexverse::world
{
// OpenGenesisLINK LLClientView sends a prim's AttachmentPoint nibble-swapped
// in ObjectUpdate State / Compressed ObjectData State. This is valid only
// for root prims whose ParentID is the CURRENT simulator avatar LocalID.
// A completely unrelated object with a nonzero State is NOT an attachment.
[[nodiscard]] inline std::uint8_t decode_server_attachment_point(
    std::uint8_t wire_state) noexcept
{
    return static_cast<std::uint8_t>(
        (wire_state>>4U)|(wire_state<<4U));
}
[[nodiscard]] inline std::map<std::string,std::uint8_t,std::less<>>
infer_avatar_attachment_points(
    const std::map<std::uint32_t,ObjectState>& objects,
    std::uint32_t avatar_local_id)
{
    std::map<std::string,std::uint8_t,std::less<>> result;
    if(avatar_local_id==0U)return result;
    for(const auto& [local_id,prim]:objects)
    {
        (void)local_id;
        if(prim.pcode!=protocol::pcode_prim ||
           prim.local_id==0U || prim.local_id==avatar_local_id ||
           prim.parent_id!=avatar_local_id || prim.id.empty())
            continue;
        const auto slot=decode_server_attachment_point(prim.state);
        // The Firestorm bind rig provides 56 known body/HUD attachment
        // points. Unknown/zero points must never be mapped to a bone.
        if(slot==0U||slot>=56U)continue;
        result.emplace(prim.id,slot);
    }
    return result;
}

// Reconcile older Low158 attachment entries with the CURRENT object
// stream. ObjectUpdate/Compressed State changes and ParentID are more
// authoritative for already-resident prims. For an object which has not
// arrived yet, retain the server's Low158 point only as a pending binding;
// it cannot draw without an actual region prim.
struct AvatarAttachmentSelection
{
    std::map<std::string,std::uint8_t,std::less<>> points;
    std::size_t current_object_points=0U;
    std::size_t pending_appearance_points=0U;
    std::size_t stale_appearance_points=0U;
};
[[nodiscard]] inline AvatarAttachmentSelection reconcile_avatar_attachments(
    const std::map<std::uint32_t,ObjectState>& objects,
    std::uint32_t avatar_local_id,
    const std::map<std::string,std::uint8_t,std::less<>>& appearance)
{
    AvatarAttachmentSelection result;
    // This same map is also used for remote avatars; never borrow root
    // attachment data from an unrelated avatar when local ID is unknown.
    if(avatar_local_id==0U)return result;
    std::unordered_set<std::string_view> known;
    known.reserve(objects.size());
    for(const auto& [local_id,prim]:objects)
    {
        (void)local_id;
        if(prim.id.empty())continue;
        known.insert(prim.id);
        if(prim.pcode!=protocol::pcode_prim||
           prim.local_id==0U||prim.local_id==avatar_local_id||
           prim.parent_id!=avatar_local_id)
            continue;
        const auto slot=decode_server_attachment_point(prim.state);
        if(slot==0U||slot>=56U)continue;
        result.points.insert_or_assign(prim.id,slot);
        ++result.current_object_points;
    }
    for(const auto& [id,slot]:appearance)
    {
        if(id.empty()||slot==0U||slot>=56U)continue;
        // If an actual ObjectUpdate with this UUID already exists in the
        // current region, its parent/State is authoritative: a detach,
        // a move to another bone, or a move to another avatar must not
        // be reverted by older Low158 appearance data.
        if(known.contains(id))
        {
            if(!result.points.contains(id))
                ++result.stale_appearance_points;
            continue;
        }
        if(result.points.emplace(id,slot).second)
            ++result.pending_appearance_points;
    }
    return result;
}
}
