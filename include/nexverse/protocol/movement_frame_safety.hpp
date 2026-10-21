/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
namespace nexverse::protocol
{
// AgentUpdate carries an authoritative QUATERNION to the simulator,
// not just a local camera angle. Never let an invalid frame elapsed time
// or an invalid server heading poison a real outbound LLUDP packet.
// CameraCenter in an AgentUpdate comes from the current server agent
// position. Unknown/NaN simulator positions are NOT a license to
// synthesize a fake walking location for the server.
[[nodiscard]] inline bool valid_agent_update_position(
    Vec3 position) noexcept
{
    return std::isfinite(position.x)&&std::isfinite(position.y)&&
           std::isfinite(position.z)&&
           std::abs(position.x)<1000000.0f&&
           std::abs(position.y)<1000000.0f&&
           std::abs(position.z)<1000000.0f;
}
[[nodiscard]] inline float bounded_agent_update_seconds(
    float seconds) noexcept
{
    if(!std::isfinite(seconds)||seconds<=0.0f)return 0.0f;
    return std::clamp(seconds,0.0f,0.25f);
}
[[nodiscard]] inline float valid_initial_agent_yaw(
    const Quaternion& heading) noexcept
{
    const double x=heading.x,y=heading.y,z=heading.z,w=heading.w;
    if(!std::isfinite(x)||!std::isfinite(y)||
       !std::isfinite(z)||!std::isfinite(w))return 0.0f;
    const double sq=x*x+y*y+z*z+w*w;
    if(!std::isfinite(sq)||sq<=1e-12)return 0.0f;
    const double inv=1.0/std::sqrt(sq);
    const double nx=x*inv,ny=y*inv,nz=z*inv,nw=w*inv;
    const double yaw=std::atan2(
        2.0*(nw*nz+nx*ny),
        1.0-2.0*(ny*ny+nz*nz));
    return std::isfinite(yaw)?static_cast<float>(yaw):0.0f;
}
}
