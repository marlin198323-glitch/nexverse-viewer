/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/mesh_quality_control.hpp"
#include "nexverse/ui/camera_avatar_focus.hpp"
#include <array>
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    for(const auto& [w,h]:std::array<std::pair<int,int>,4>{{
        {1100,700},{1920,1080},{3440,1440},{3840,1080}
    }})
    {
        const auto b=mesh_quality_button(w,h);
        const auto f=avatar_focus_button(w,h);
        if(b.x<0||b.y<0||b.x+b.w>float(w)||b.y+b.h>float(h))
            return 1;
        if(!(b.x+b.w<f.x))return 2;
        if(!hit_mesh_quality_button(w,h,b.x+b.w*0.5,b.y+b.h*0.5))
            return 3;
        if(hit_mesh_quality_button(w,h,b.x-2,b.y))return 4;
    }
    std::cout<<"F2 actual clickable mesh detail control for widescreen and compact PASS\n";
    return 0;
}
