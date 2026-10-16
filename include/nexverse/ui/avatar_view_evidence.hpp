/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/render/third_person_camera.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>

namespace nexverse::ui
{
// Pure, camera/projective geometry evidence. "In frustum" never means
// there are actually coloured avatar pixels or a server bake on screen.
struct AvatarViewEvidence
{
    bool valid_pose=false;
    bool center_in_frustum=false;
    bool fallback_orientation=false;
    float normalized_x=0.5f;
    float normalized_y=0.5f;
    float depth=0.0f;
};
[[nodiscard]] inline AvatarViewEvidence inspect_avatar_view(
    const render::ThirdPersonCamera& camera,
    const protocol::Vec3& agent_position,
    int framebuffer_width,int framebuffer_height,
    float vertical_fov_degrees=60.0f) noexcept
{
    AvatarViewEvidence result;
    result.fallback_orientation=!camera.orientation_valid;
    const auto finite=[](float v){return std::isfinite(v);};
    if(!camera.position_valid||framebuffer_width<=0||
       framebuffer_height<=0||!finite(vertical_fov_degrees)||
       vertical_fov_degrees<=1.0f||vertical_fov_degrees>=170.0f||
       !finite(agent_position.x)||!finite(agent_position.y)||
       !finite(agent_position.z))return result;
    const float dx=agent_position.x-camera.position.x;
    const float dy=agent_position.y-camera.position.y;
    const float dz=agent_position.z+1.45f-camera.position.z;
    // The orthonormal up vector used by a look-at matrix derives from
    // Forward cross Left; the camera's nominal Up is world +Z.
    const float upx=camera.forward.y*camera.left.z-
        camera.forward.z*camera.left.y;
    const float upy=camera.forward.z*camera.left.x-
        camera.forward.x*camera.left.z;
    const float upz=camera.forward.x*camera.left.y-
        camera.forward.y*camera.left.x;
    const float depth=dx*camera.forward.x+dy*camera.forward.y+
        dz*camera.forward.z;
    const float horizontal=-(dx*camera.left.x+dy*camera.left.y+
        dz*camera.left.z);
    const float vertical=dx*upx+dy*upy+dz*upz;
    const double half_vertical=std::tan(
        double(vertical_fov_degrees)*3.14159265358979323846/360.0);
    const double aspect=double(framebuffer_width)/framebuffer_height;
    if(!finite(depth)||depth<=0.1f||!finite(horizontal)||
       !finite(vertical)||!std::isfinite(aspect)||aspect<=0.0||
       !std::isfinite(half_vertical)||half_vertical<=0.0)return result;
    const double x=0.5+double(horizontal)/(2.0*double(depth)*
        half_vertical*aspect);
    const double y=0.5-double(vertical)/(2.0*double(depth)*
        half_vertical);
    if(!std::isfinite(x)||!std::isfinite(y))return result;
    result.valid_pose=true;
    result.normalized_x=static_cast<float>(x);
    result.normalized_y=static_cast<float>(y);
    result.depth=depth;
    result.center_in_frustum=x>=0.0&&x<=1.0&&y>=0.0&&y<=1.0;
    return result;
}
[[nodiscard]] inline std::string describe_avatar_view(
    bool world_ready,bool self_present,
    const AvatarViewEvidence& view,
    std::uint64_t original_mesh_draws,
    std::uint64_t proxy_draws)
{
    if(!world_ready)return "Avatar: Simulator wartet";
    if(!self_present)return "Avatar: AgentPosition fehlt";
    if(!view.valid_pose)return "Avatar: Kameraposition ungueltig";
    if(!view.center_in_frustum)return "Avatar: ausserhalb Bildfeld";
    if(original_mesh_draws>0U)
        return "Avatar Mitte im Bildfeld | LLM GL "+
            std::to_string(original_mesh_draws);
    if(proxy_draws>0U)
        return "Avatar Mitte im Bildfeld | Ersatz GL "+
            std::to_string(proxy_draws);
    return "Avatar Mitte im Bildfeld | Mesh GL fehlt";
}
}
