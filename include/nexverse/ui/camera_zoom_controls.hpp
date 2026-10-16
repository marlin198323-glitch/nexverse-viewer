/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cmath>
#include <string>

namespace nexverse::ui
{
constexpr float camera_zoom_min=3.0f;
constexpr float camera_zoom_max=24.0f;
constexpr float camera_zoom_default=8.0f;
[[nodiscard]] inline float camera_zoom_adjust(
    float current,double wheel_steps) noexcept
{
    const float base=std::isfinite(current)
        ?std::clamp(current,camera_zoom_min,camera_zoom_max)
        :camera_zoom_default;
    if(!std::isfinite(wheel_steps))return base;
    const double steps=std::clamp(wheel_steps,-12.0,12.0);
    const double target=double(base)*std::pow(0.86,steps);
    return static_cast<float>(std::clamp(
        target,double(camera_zoom_min),double(camera_zoom_max)));
}
enum class CameraZoomHit { none, near, far };
struct CameraZoomButton
{
    float x=0,y=0,w=0,h=0;
    [[nodiscard]] bool contains(double px,double py) const noexcept
    {return px>=x&&py>=y&&px<x+w&&py<y+h;}
};
struct CameraZoomButtons
{
    CameraZoomButton near,far;
};
[[nodiscard]] inline CameraZoomButtons camera_zoom_buttons(
    int window_width,int window_height) noexcept
{
    const float scale=std::clamp(
        float(std::max(window_height,1))/1080.0f,0.74f,1.50f);
    // Keep controls below top session chips and left of the F5 button.
    const float x=float(window_width)-380.0f*scale;
    return {{x,63.0f*scale,31.0f*scale,29.0f*scale},
            {x+39.0f*scale,63.0f*scale,31.0f*scale,29.0f*scale}};
}
[[nodiscard]] inline CameraZoomHit hit_camera_zoom(
    int width,int height,double px,double py) noexcept
{
    const auto c=camera_zoom_buttons(width,height);
    if(c.near.contains(px,py))return CameraZoomHit::near;
    if(c.far.contains(px,py))return CameraZoomHit::far;
    return CameraZoomHit::none;
}
[[nodiscard]] inline std::string camera_zoom_label(float distance)
{
    if(!std::isfinite(distance))distance=camera_zoom_default;
    const int tenths=static_cast<int>(std::round(
        std::clamp(distance,camera_zoom_min,camera_zoom_max)*10.0f));
    return std::to_string(tenths/10)+'.'+std::to_string(tenths%10)+"m";
}
}
