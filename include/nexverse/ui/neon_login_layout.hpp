/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <algorithm>
#include <cmath>
namespace nexverse::ui
{
struct NeonRect
{
    float x{},y{},w{},h{};
    [[nodiscard]] bool contains(double px,double py) const noexcept
    {
        return px>=x&&py>=y&&px<x+w&&py<y+h;
    }
};

// Shared layout for actual GLFW mouse hit-testing and login rendering.
// Reference composition: 1648x928 neon-blue NexVerse first design.
// All active controls stay wholly inside small/resized windows.
struct NeonLoginLayout
{
    float width{},height{},scale{},header_h{},footer_h{},footer_y{};
    NeonRect brand{},grid{},grid_status{},login_status{},world_status{};
    NeonRect footer{},avatar_icon{},username{},password{},password_eye{};
    NeonRect enter_world{},remember_avatar{},help{};
    explicit NeonLoginLayout(int w,int h) noexcept
    {
        width=static_cast<float>(std::max(w,1));
        height=static_cast<float>(std::max(h,1));
        scale=std::clamp(std::min(width/1648.0f,height/928.0f),0.38f,1.65f);
        // A client window smaller than the design minimum still shrinks
        // control geometry rather than clipping credentials off-screen.
        scale=std::min(scale,std::max(0.26f,width/1110.0f));
        header_h=72.0f*scale;
        footer_h=77.0f*scale;
        footer_y=height-footer_h;
        footer={8.0f*scale,footer_y+3.0f*scale,
                width-16.0f*scale,footer_h-6.0f*scale};
        brand={24.0f*scale,11.0f*scale,48.0f*scale,48.0f*scale};
        grid={width*0.5f-142.0f*scale,14.0f*scale,
              284.0f*scale,43.0f*scale};
        const float chips_right=width-54.0f*scale;
        grid_status={chips_right-317.0f*scale,14.0f*scale,
                     92.0f*scale,43.0f*scale};
        login_status={chips_right-217.0f*scale,14.0f*scale,
                      96.0f*scale,43.0f*scale};
        world_status={chips_right-113.0f*scale,14.0f*scale,
                      102.0f*scale,43.0f*scale};
        avatar_icon={25.0f*scale,footer_y+17.0f*scale,
                     44.0f*scale,44.0f*scale};
        const float control_y=footer_y+17.0f*scale;
        const float controls_start=std::min(350.0f*scale,
            std::max(200.0f*scale,width*0.22f));
        const float field_w=245.0f*scale;
        const float gap=13.0f*scale;
        username={controls_start,control_y,field_w,39.0f*scale};
        password={username.x+username.w+gap,control_y,
                  field_w,39.0f*scale};
        password_eye={password.x+password.w-40.0f*scale,
                      control_y+4.0f*scale,34.0f*scale,31.0f*scale};
        enter_world={password.x+password.w+gap,control_y,
                     210.0f*scale,39.0f*scale};
        remember_avatar={enter_world.x+enter_world.w+14.0f*scale,
                         control_y+5.0f*scale,156.0f*scale,29.0f*scale};
        help={width-40.0f*scale,control_y+5.0f*scale,
              24.0f*scale,29.0f*scale};
    }
};
}
