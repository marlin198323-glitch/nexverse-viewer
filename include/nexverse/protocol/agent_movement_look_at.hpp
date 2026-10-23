/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <cmath>
#include <optional>
namespace nexverse::protocol
{
// LLUDP AgentMovementComplete carries LookAt, NOT BodyRotation.
// The simulator-supplied horizontal look direction can yield the
// unit heading quaternion. All-zero/vertical/invalid directions do
// not authorize a manufactured initial yaw for High4 AgentUpdate.
[[nodiscard]] inline std::optional<Quaternion>
simulator_look_at_heading(Vec3 look_at) noexcept
{
    const double x=look_at.x,y=look_at.y,z=look_at.z;
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z))
        return std::nullopt;
    const double h2=x*x+y*y;
    if(!std::isfinite(h2)||h2<=1e-8)
        return std::nullopt;
    const double yaw=std::atan2(y,x);
    const double half=yaw*0.5;
    const double sn=std::sin(half),cs=std::cos(half);
    if(!std::isfinite(sn)||!std::isfinite(cs))
        return std::nullopt;
    return Quaternion{0.0f,0.0f,static_cast<float>(sn),
                      static_cast<float>(cs)};
}
}
