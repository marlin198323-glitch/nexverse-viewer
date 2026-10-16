/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/ui/camera_zoom_controls.hpp"
#include <cmath>
#include <string>
namespace nexverse::ui
{
[[nodiscard]] inline float orbit_normalize(float degrees) noexcept
{
    if(!std::isfinite(degrees))return 0.0f;
    const double normalized=std::remainder(double(degrees),360.0);
    return std::abs(normalized)<0.001?0.0f:float(normalized);
}
[[nodiscard]] inline float orbit_step(float degrees,int direction) noexcept
{
    const float current=orbit_normalize(degrees);
    if(direction==0)return current;
    return orbit_normalize(current+(direction>0?45.0f:-45.0f));
}
[[nodiscard]] inline float orbit_front_back(float degrees) noexcept
{
    return std::abs(orbit_normalize(degrees))==180.0f?0.0f:180.0f;
}
[[nodiscard]] inline std::string orbit_label(float degrees)
{
    const float angle=orbit_normalize(degrees);
    if(angle==0.0f)return "Hinten";
    if(std::abs(angle)==180.0f)return "Vorne";
    if(angle==90.0f)return "Rechts";
    if(angle==-90.0f)return "Links";
    return "Seite "+std::to_string(int(std::round(angle)));
}
enum class CameraOrbitHit { none, turn_left, front_back, turn_right };
struct CameraOrbitButtons
{
    CameraZoomButton left,flip,right;
};
[[nodiscard]] inline CameraOrbitButtons camera_orbit_buttons(
    int width,int height) noexcept
{
    const float scale=std::clamp(
        float(std::max(height,1))/1080.0f,0.74f,1.50f);
    const float x=float(width)-642.0f*scale;
    return {{x,63.0f*scale,34.0f*scale,29.0f*scale},
            {x+40.0f*scale,63.0f*scale,34.0f*scale,29.0f*scale},
            {x+80.0f*scale,63.0f*scale,34.0f*scale,29.0f*scale}};
}
[[nodiscard]] inline CameraOrbitHit hit_camera_orbit(
    int width,int height,double px,double py) noexcept
{
    const auto b=camera_orbit_buttons(width,height);
    if(b.left.contains(px,py))return CameraOrbitHit::turn_left;
    if(b.flip.contains(px,py))return CameraOrbitHit::front_back;
    if(b.right.contains(px,py))return CameraOrbitHit::turn_right;
    return CameraOrbitHit::none;
}
}
