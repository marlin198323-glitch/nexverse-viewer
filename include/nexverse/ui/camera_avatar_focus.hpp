/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/ui/camera_orbit_controls.hpp"
namespace nexverse::ui
{
// Real Firestorm-style camera shortcut for looking AT the self avatar,
// not a server-origin teleport or an AvatarAgent rotation.
struct AvatarCameraFocus
{
    float distance=camera_zoom_default;
    float orbit=180.0f;
};
[[nodiscard]] constexpr AvatarCameraFocus focus_front_avatar() noexcept
{
    return {};
}
[[nodiscard]] inline CameraZoomButton avatar_focus_button(
    int width,int height) noexcept
{
    const float scale=std::clamp(
        float(std::max(height,1))/1080.0f,0.74f,1.5f);
    return {float(width)-773.0f*scale,
        63.0f*scale,116.0f*scale,29.0f*scale};
}
[[nodiscard]] inline bool hit_avatar_focus(
    int width,int height,double mx,double my) noexcept
{
    return avatar_focus_button(width,height).contains(mx,my);
}
}
