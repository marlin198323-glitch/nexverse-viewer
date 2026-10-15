/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/texture_entry.hpp"
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// Official Firestorm LLAvatarAppearanceDefines::ETextureIndex. These are
// texture-entry FACE indices, not BAKED_HEAD enum indices (0,1,2...).
inline constexpr std::array<std::size_t,11U> avatar_baked_faces{
    8U,9U,10U,11U,19U,20U,40U,41U,42U,43U,44U
};
inline constexpr std::array<std::string_view,11U> baked_placeholders{
    "5a9f4a74-30f2-821c-b88d-70499d3e7183", // IMG_USE_BAKED_HEAD
    "ae2de45c-d252-50b8-5c6e-19f39ce79317", // UPPER
    "24daea5f-0539-cfcf-047f-fbc40b2786ba", // LOWER
    "52cc6bb6-2ee5-e632-d3ad-50197b1dcb8a", // EYES
    "43529ce8-7faa-ad92-165a-bc4078371687", // SKIRT
    "09aac1fb-6bce-0bee-7d44-caac6dbb6c63", // HAIR
    "ff62763f-d60a-9855-890b-0c96f8f8cd98", // LEFTARM
    "8e915e25-31d1-cc95-ae08-d58a47488251", // LEFTLEG
    "9742065b-19b5-297c-858a-29711d539043", // AUX1
    "03642e83-2bd1-4eb9-34b4-4c47ed586d2d", // AUX2
    "edd51b77-fc10-ce7a-4b3d-011dfc349e4f"  // AUX3
};
struct AvatarBakedTextures
{
    // 11 authoritative non-placeholder baked-asset UUIDs, optional each.
    std::array<std::string,11U> ids;
    [[nodiscard]] std::array<std::string,3U> visible_body() const
    {
        return {ids[0],ids[1],ids[2]};
    }
    [[nodiscard]] std::size_t valid_count() const
    {
        std::size_t count=0U;
        for(const auto& id:ids)if(!id.empty())++count;
        return count;
    }
};
// Viewer self bake sources require current authenticated agent ownership
// AND a live simulator region. A stale other-agent packet is NOT a skin.
[[nodiscard]] inline bool self_bake_owner_is_current(
    std::string_view packet_avatar_id,
    std::string_view active_agent_id,
    std::string_view renderer_agent_id,
    bool region_connected) noexcept
{
    return region_connected && !packet_avatar_id.empty() &&
        packet_avatar_id==active_agent_id &&
        packet_avatar_id==renderer_agent_id;
}

[[nodiscard]] inline AvatarBakedTextures extract_avatar_bakes(
    std::span<const std::uint8_t> texture_entry)
{
    AvatarBakedTextures out;
    // Incomplete legacy/invalid TextureEntry -> no fetch, no ghost skin.
    // Reuse strict common parser to avoid a second independent bitmap codec.
    const auto parsed=parse_texture_entry(texture_entry);
    if(!parsed)return out;
    for(std::size_t i=0U;i<avatar_baked_faces.size();++i)
    {
        // The default avatar texture is not an authoritative bake.
        // A color/UV-only override must not be mistaken for a baked UUID.
        if(!parsed->texture_id_overridden[avatar_baked_faces[i]])
            continue;
        const auto& id=parsed->face(avatar_baked_faces[i]).texture_id;
        if(id.empty()||
           id=="00000000-0000-0000-0000-000000000000")
            continue;
        bool placeholder=false;
        for(const auto marker:baked_placeholders)
            if(id==marker){placeholder=true;break;}
        if(!placeholder)out.ids[i]=id;
    }
    return out;
}
}
