/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/world/world_model.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
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
}
