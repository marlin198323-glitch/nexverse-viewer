/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/render/renderer.hpp"
#include <cmath>
#include <cstdint>
#include <limits>
namespace nexverse::render
{
// This is a GPU admission test only. Never mutate the authoritative
// WorldModel AgentState, LLUDP packets or server-provided physics pose.
struct AvatarProxyPoseAudit
{
    bool position_valid=false;
    bool rotation_repaired=false;
    bool position_dropped=false;
};
[[nodiscard]] inline AvatarProxyPoseAudit sanitize_avatar_proxy_pose(
    RenderAvatarProxy& avatar) noexcept
{
    AvatarProxyPoseAudit audit{};
    const auto finite=[](float v) noexcept
    {
        return std::isfinite(v)&&std::abs(v)<1000000.0f;
    };
    if(!finite(avatar.position.x)||!finite(avatar.position.y)||
       !finite(avatar.position.z))
    {
        audit.position_dropped=true;
        return audit;
    }
    audit.position_valid=true;
    // Double precision norm avoids finite-but-large float squares that
    // overflow to infinity, and zero/nonfinite quaternions. Invalid
    // heading keeps avatar geometry at its known valid server position.
    auto& q=avatar.rotation;
    const double norm2=
        double(q.x)*q.x+double(q.y)*q.y+
        double(q.z)*q.z+double(q.w)*q.w;
    if(!std::isfinite(norm2)||norm2<=1e-12)
    {
        q={0.0f,0.0f,0.0f,1.0f};
        audit.rotation_repaired=true;
        return audit;
    }
    const auto inv=1.0/std::sqrt(norm2);
    const RenderQuaternion normalized{
        static_cast<float>(q.x*inv),static_cast<float>(q.y*inv),
        static_cast<float>(q.z*inv),static_cast<float>(q.w*inv)};
    if(!std::isfinite(normalized.x)||!std::isfinite(normalized.y)||
       !std::isfinite(normalized.z)||!std::isfinite(normalized.w))
    {
        q={0.0f,0.0f,0.0f,1.0f};
        audit.rotation_repaired=true;
        return audit;
    }
    // Standard normalization also fixes non-unit input, but this is only
    // a diagnostics warning for structurally unusable rotations.
    q=normalized;
    return audit;
}
}
