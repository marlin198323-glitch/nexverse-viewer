/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
namespace nexverse::ui
{
struct WorldHudToggleRect
{
    float x=0.0f,y=0.0f,width=0.0f,height=0.0f;
    [[nodiscard]] constexpr bool contains(double px,double py) const noexcept
    {
        return px>=x&&py>=y&&px<x+width&&py<y+height;
    }
};
// Button remains in same screen position whether HUD is on/off, and
// never overlaps top-right session chips (which end at 54*scale).
[[nodiscard]] inline WorldHudToggleRect world_hud_toggle_rect(
    int width,int height) noexcept
{
    const float scale=std::clamp(
        static_cast<float>(std::max(height,1))/1080.0f,0.74f,1.5f);
    return {static_cast<float>(width)-176.0f*scale,
        63.0f*scale,160.0f*scale,29.0f*scale};
}
enum class WorldHudClickTarget
{
    toggle_hud,
    scene,
    dashboard_or_floater
};
[[nodiscard]] inline WorldHudClickTarget world_hud_click_target(
    int width,int height,double mouse_x,double mouse_y,
    bool hud_visible,bool floater_open) noexcept
{
    if(world_hud_toggle_rect(width,height).contains(mouse_x,mouse_y))
        return WorldHudClickTarget::toggle_hud;
    if(!hud_visible&&!floater_open)
        return WorldHudClickTarget::scene;
    return WorldHudClickTarget::dashboard_or_floater;
}
}
