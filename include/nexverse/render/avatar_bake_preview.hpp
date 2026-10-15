/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bakes.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <unordered_set>

namespace nexverse::render
{
// Read-only Firestorm simulator Head/Upper/Lower baked texture preview.
// Never accepts UUIDs from another agent or a previous region as valid:
// the caller must pass the AUTHENTICATED current self AvatarAppearance,
// same render-thread and current GPU resident set. No new asset request.
struct AvatarBakePreviewSelection
{
    std::array<std::string,3U> ids{};
    std::size_t resident=0U;
};

[[nodiscard]] inline AvatarBakePreviewSelection select_avatar_bake_previews(
    const nexverse::protocol::AvatarBakedTextures& server_bakes,
    const std::unordered_set<std::string>& gpu_resident,
    bool authenticated_current_region)
{
    AvatarBakePreviewSelection out;
    if(!authenticated_current_region)return out;
    const auto visible=server_bakes.visible_body();
    for(std::size_t i=0U;i<visible.size();++i)
    {
        const auto& id=visible[i];
        if(id.empty() || !gpu_resident.contains(id) ||
           id=="00000000-0000-0000-0000-000000000000")continue;
        bool placeholder=false;
        for(const auto reserved:nexverse::protocol::baked_placeholders)
            if(id==reserved){placeholder=true;break;}
        if(placeholder)continue;
        out.ids[i]=id;
        ++out.resident;
    }
    return out;
}
}
