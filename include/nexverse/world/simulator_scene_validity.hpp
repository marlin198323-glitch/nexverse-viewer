/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <cmath>
#include <cstdint>
namespace nexverse::world
{
// Reject non-finite or absurd coordinates before they can reach the GPU,
// the minimap or server-authoritative movement evidence.
inline bool finite_simulator_position(protocol::Vec3 p) noexcept
{
    return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z)&&
        std::abs(p.x)<=1000000.0f&&std::abs(p.y)<=1000000.0f&&
        std::abs(p.z)<=1000000.0f;
}
inline bool finite_simulator_rotation(protocol::Quaternion q) noexcept
{
    return std::isfinite(q.x)&&std::isfinite(q.y)&&
           std::isfinite(q.z)&&std::isfinite(q.w);
}
inline bool valid_simulator_avatar_pose(protocol::Vec3 p,
                                       protocol::Quaternion q) noexcept
{
    if(!finite_simulator_position(p)||!finite_simulator_rotation(q))
        return false;
    const double norm=double(q.x)*q.x+double(q.y)*q.y+
                      double(q.z)*q.z+double(q.w)*q.w;
    // Matches the existing real High9/Compressed-partial validity gate.
    return std::isfinite(norm)&&norm>=0.25&&norm<=4.0;
}
inline bool valid_simulator_prim_geometry(
    protocol::Vec3 position,protocol::Quaternion rotation,
    protocol::Vec3 scale) noexcept
{
    if(!finite_simulator_position(position)||
       !finite_simulator_rotation(rotation)||
       !std::isfinite(scale.x)||!std::isfinite(scale.y)||
       !std::isfinite(scale.z))
        return false;
    // Zero rotation remains allowed for legacy decoded zero-filled
    // fixtures/primitive defaults; the renderer supplies its fallback.
    // Reject dangerous scale values before they expand GL bounds.
    return scale.x>=0.0f&&scale.y>=0.0f&&scale.z>=0.0f&&
           scale.x<=65536.0f&&scale.y<=65536.0f&&scale.z<=65536.0f;
}
}
