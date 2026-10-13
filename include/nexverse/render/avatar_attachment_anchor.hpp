/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/render/renderer.hpp"
#include <cstdint>
#include <span>

namespace nexverse::render
{
// Read-only lookup of an in-region simulator avatar local ID. A prim whose
// parent ID is a recognized avatar is attached to its live world transform.
// No UUID guesswork, no attachment-point bone rigging. Unknown/zero parent
// IDs never resolve to arbitrary avatar data.
[[nodiscard]] inline const RenderAvatarProxy* find_attachment_avatar_parent(
    std::span<const RenderAvatarProxy> avatars,
    std::uint32_t parent_local_id) noexcept
{
    if(parent_local_id==0U)return nullptr;
    for(const auto& avatar:avatars)
        if(avatar.local_id!=0U && avatar.local_id==parent_local_id)
            return &avatar;
    return nullptr;
}
}
