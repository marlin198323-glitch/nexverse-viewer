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
        header_h=58.0f*scale;
        footer_h=60.0f*scale;
        footer_y=height-footer_h;
        footer={8.0f*scale,footer_y+3.0f*scale,
                width-16.0f*scale,footer_h-6.0f*scale};
        brand={19.0f*scale,9.0f*scale,38.0f*scale,38.0f*scale};
        grid={width*0.5f-125.0f*scale,10.0f*scale,
              250.0f*scale,35.0f*scale};
        const float chips_right=width-29.0f*scale;
        grid_status={chips_right-270.0f*scale,10.0f*scale,
                     82.0f*scale,35.0f*scale};
        login_status={chips_right-181.0f*scale,10.0f*scale,
                      85.0f*scale,35.0f*scale};
        world_status={chips_right-89.0f*scale,10.0f*scale,
                      87.0f*scale,35.0f*scale};
        avatar_icon={22.0f*scale,footer_y+12.0f*scale,
                     36.0f*scale,36.0f*scale};
        const float control_y=footer_y+14.0f*scale;
        const float controls_start=std::min(315.0f*scale,
            std::max(190.0f*scale,width*0.20f));
        const float field_w=205.0f*scale;
        const float gap=12.0f*scale;
        username={controls_start,control_y,field_w,32.0f*scale};
        password={username.x+username.w+gap,control_y,
                  field_w,32.0f*scale};
        password_eye={password.x+password.w-32.0f*scale,
                      control_y+3.0f*scale,27.0f*scale,26.0f*scale};
        enter_world={password.x+password.w+gap,control_y,
                     180.0f*scale,32.0f*scale};
        remember_avatar={enter_world.x+enter_world.w+13.0f*scale,
                         control_y+3.0f*scale,142.0f*scale,26.0f*scale};
        help={width-35.0f*scale,control_y+3.0f*scale,
              22.0f*scale,26.0f*scale};
    }
};
}
