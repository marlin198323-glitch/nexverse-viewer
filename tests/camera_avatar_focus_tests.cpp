/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/camera_avatar_focus.hpp"
#include "nexverse/render/third_person_camera.hpp"
#include <cmath>
#include <iostream>
int main()
{
    using namespace nexverse;
    const auto focus=ui::focus_front_avatar();
    if(focus.distance!=8.0f||focus.orbit!=180.0f)return 1;
    const auto pose=render::third_person_camera(
        {128,128,25},{},focus.distance,focus.orbit);
    if(!pose.position_valid||std::abs(pose.position.x-136.0f)>0.01f||
       pose.forward.x>=0.0f)return 2;
    for(const auto& dims: {std::pair{1100,700},
                          std::pair{1920,1080},std::pair{3440,1440}})
    {
        const auto b=ui::avatar_focus_button(dims.first,dims.second);
        if(!ui::hit_avatar_focus(dims.first,dims.second,
            b.x+0.5f*b.w,b.y+0.5f*b.h))return 3;
        if(ui::hit_avatar_focus(dims.first,dims.second,
            b.x-4.0f,b.y))return 4;
        const auto orbit=ui::camera_orbit_buttons(dims.first,dims.second);
        if(b.x+b.w>=orbit.left.x)return 5;
        if(b.x<0||b.y<0)return 6;
    }
    std::cout<<"Real self-avatar front camera focus; no GUI overlap, full viewport scaling PASS\n";
    return 0;
}
