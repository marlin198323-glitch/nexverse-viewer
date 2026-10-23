/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/avatar_bake_stream_priority.hpp"
#include "nexverse/render/avatar_proxy_pose_integrity.hpp"
#include <cstdint>
#include <vector>

namespace nexverse::assets
{
// Rendering and GetTexture planning must agree on whether an avatar
// actually has a usable GL world position. Never spend the shared
// VRAM/HTTP budget on NaN/Infinite simulator-proxy coordinates.
//
// The accepted candidates retain the original server-selected baked
// UUIDs. No asset UUIDs are guessed, rewritten, or promoted to Low158.
struct AvatarBakeGpuAdmission
{
    std::vector<AvatarBakeStreamCandidate> accepted;
    std::uint32_t invalid_positions_skipped=0U;
    bool self_position_skipped=false;
};
[[nodiscard]] inline AvatarBakeGpuAdmission
admit_avatar_bake_gpu_candidates(
    const std::vector<nexverse::render::RenderAvatarProxy>& proxies)
{
    AvatarBakeGpuAdmission admitted;
    admitted.accepted.reserve(proxies.size());
    for(const auto& proxy:proxies)
    {
        auto checked=proxy;
        const auto result=nexverse::render::sanitize_avatar_proxy_pose(checked);
        if(!result.position_valid)
        {
            if(admitted.invalid_positions_skipped<UINT32_MAX)
                ++admitted.invalid_positions_skipped;
            if(proxy.is_self)admitted.self_position_skipped=true;
            continue;
        }
        admitted.accepted.push_back({
            proxy.is_self,proxy.baked_texture_ids,
            checked.position.x,checked.position.y,checked.position.z,
            proxy.hair_bake_texture_id,proxy.eyes_bake_texture_id});
    }
    return admitted;
}
}
