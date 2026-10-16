/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/ui/camera_zoom_controls.hpp"
namespace nexverse::ui
{
[[nodiscard]] inline CameraZoomButton mesh_quality_button(
    int window_width,int window_height) noexcept
{
    const float s=std::clamp(float(std::max(window_height,1))/
        1080.0f,0.74f,1.50f);
    return {float(window_width)-906.0f*s,63.0f*s,
        116.0f*s,29.0f*s};
}
[[nodiscard]] inline bool hit_mesh_quality_button(
    int w,int h,double x,double y) noexcept
{
    return mesh_quality_button(w,h).contains(x,y);
}
}
