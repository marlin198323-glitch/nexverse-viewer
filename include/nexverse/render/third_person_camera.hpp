/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <cmath>

namespace nexverse::render
{
// Third-person scene rendering uses a horizontal heading (the avatar's
// yaw), not an arbitrary unnormalised network quaternion or its pitch.
// The view always looks towards the self avatar, not away from the region.
struct ThirdPersonCamera
{
    protocol::Vec3 position{128.0f,120.0f,34.2f};
    protocol::Vec3 forward{0.88f,0.0f,-0.46f};
    protocol::Vec3 left{0.0f,1.0f,0.0f};
    protocol::Vec3 up{0.0f,0.0f,1.0f};
    bool position_valid=false;
    bool orientation_valid=false;
};
[[nodiscard]] inline ThirdPersonCamera third_person_camera(
    const protocol::Vec3& agent_position,
    const protocol::Quaternion& agent_rotation,
    float distance=8.0f,float orbit_degrees=0.0f) noexcept
{
    // Zoom is local-only: it never changes the authoritative AgentState
    // or sends any movement packet to the simulator.
    const float zoom=std::isfinite(distance)
        ?std::clamp(distance,3.0f,24.0f):8.0f;
    ThirdPersonCamera result;
    const auto valid=[](float n){return std::isfinite(n);};
    // Reject representable but implausible server coordinates that
    // overflow GL view-matrix arithmetic or lose meter-scale precision.
    const bool position_ok=
        valid(agent_position.x)&&valid(agent_position.y)&&
        valid(agent_position.z)&&
        std::abs(agent_position.x)<1000000.0f&&
        std::abs(agent_position.y)<1000000.0f&&
        std::abs(agent_position.z)<1000000.0f;
    const protocol::Vec3 origin=position_ok
        ?agent_position:protocol::Vec3{128.0f,128.0f,25.0f};
    result.position_valid=position_ok;
    float hx=1.0f,hy=0.0f;
    const double norm2=
        double(agent_rotation.x)*agent_rotation.x+
        double(agent_rotation.y)*agent_rotation.y+
        double(agent_rotation.z)*agent_rotation.z+
        double(agent_rotation.w)*agent_rotation.w;
    if(std::isfinite(norm2)&&norm2>1e-12)
    {
        const double inv=1.0/std::sqrt(norm2);
        const double x=agent_rotation.x*inv;
        const double y=agent_rotation.y*inv;
        const double z=agent_rotation.z*inv;
        const double w=agent_rotation.w*inv;
        const double fx=1.0-2.0*(y*y+z*z);
        const double fy=2.0*(x*y+w*z);
        const double flat=std::hypot(fx,fy);
        if(std::isfinite(flat)&&flat>1e-6)
        {
            hx=static_cast<float>(fx/flat);
            hy=static_cast<float>(fy/flat);
            result.orientation_valid=true;
        }
    }
    // Invalid orientation safely uses the +X heading; bad position safely
    // uses a finite default. Both flags remain available for diagnostics.
    const protocol::Vec3 target{
        origin.x,origin.y,origin.z+1.45f};
    // Orbit rotates the LOCAL viewing position around the simulator's
    // self avatar. It must not rotate the authoritative AgentState.
    // A 180 degree orbit is in front of the avatar looking back at it.
    constexpr double pi=3.14159265358979323846;
    const double angle=std::isfinite(orbit_degrees)
        ?std::remainder(double(orbit_degrees),360.0)*pi/180.0:0.0;
    const float cs=static_cast<float>(std::cos(angle));
    const float sn=static_cast<float>(std::sin(angle));
    const float ox=hx*cs-hy*sn;
    const float oy=hx*sn+hy*cs;
    result.position={
        target.x-zoom*ox,
        target.y-zoom*oy,
        target.z+zoom*0.525f};
    const float height=zoom*0.525f;
    // Use sanitised zoom, never the raw (possibly NaN) network/UI input.
    const float inv_length=1.0f/std::sqrt(
        zoom*zoom+height*height);
    result.forward={
        zoom*ox*inv_length,
        zoom*oy*inv_length,
        -height*inv_length};
    result.left={-oy,ox,0.0f};
    return result;
}
}
