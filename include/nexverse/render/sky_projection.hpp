/* GPL-compatible in-repo math only. No sky image asset.
 * Project camera-relative solar position, not fixed screen coordinates. */
#pragma once
#include "nexverse/render/renderer.hpp"
#include <algorithm>
#include <cmath>
namespace nexverse::render
{
struct SkyScreenPosition
{
    float x=-10.0f;
    float y=-10.0f;
    bool sun_in_view=false;
    float camera_pitch=0.0f;
};
[[nodiscard]] inline SkyScreenPosition project_environment_sun(
    const RenderEnvironment& environment,
    const RenderCamera& camera,float aspect) noexcept
{
    SkyScreenPosition result;
    const auto dot=[](RenderVec3 a,RenderVec3 b)
    {return a.x*b.x+a.y*b.y+a.z*b.z;};
    const auto length=[](RenderVec3 x)
    {return std::sqrt(x.x*x.x+x.y*x.y+x.z*x.z);};
    auto forward=camera.forward;
    const auto norm=length(forward);
    if(!std::isfinite(norm)||norm<0.001f)return result;
    forward={forward.x/norm,forward.y/norm,forward.z/norm};
    result.camera_pitch=std::asin(std::clamp(forward.z,-1.0f,1.0f));
    if(!std::isfinite(environment.sun_angle)||
       !std::isfinite(environment.east_angle)||
       !std::isfinite(camera.vertical_fov_degrees)||
       camera.vertical_fov_degrees<=0.0f||
       !std::isfinite(aspect)||aspect<=0.0f)return result;
    const float az=environment.east_angle,alt=environment.sun_angle;
    const RenderVec3 sun={
        std::cos(alt)*std::cos(az),
        std::cos(alt)*std::sin(az),
        std::sin(alt)};
    const float z=dot(sun,forward);
    if(z<=0.001f)return result; // behind the viewer
    const float tan_half=std::tan(
        camera.vertical_fov_degrees*0.5f*3.14159265358979323846f/180.0f);
    if(!std::isfinite(tan_half)||tan_half<=0.001f)return result;
    // Camera.left points left; screen positive x points right.
    const float ndc_x=-dot(sun,camera.left)/(z*tan_half*aspect);
    const float ndc_y=dot(sun,camera.up)/(z*tan_half);
    result.x=0.5f+ndc_x*0.5f;
    result.y=0.5f+ndc_y*0.5f;
    result.sun_in_view=std::abs(ndc_x)<=1.2f&&std::abs(ndc_y)<=1.2f;
    return result;
}
}
