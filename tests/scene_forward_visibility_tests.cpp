/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/render/scene_forward_visibility.hpp"
#include <iostream>
#include <limits>
using nexverse::render::scene_forward_visible;
int main()
{
    if(!scene_forward_visible(4,0,0,1,1,1,1,0,0,40))return 1;
    if(scene_forward_visible(-4,0,0,1,1,1,1,0,0,40))return 2;
    if(!scene_forward_visible(-0.1f,0,0,2,2,2,1,0,0,40))return 3;
    if(scene_forward_visible(60,0,0,1,1,1,1,0,0,40))return 4;
    if(!scene_forward_visible(60,0,0,50,50,50,1,0,0,40))return 5;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    if(scene_forward_visible(nan,0,0,1,1,1,1,0,0,40))return 6;
    if(!scene_forward_visible(3,0,0,1,1,1,0,0,0,40))return 7;
    if(!scene_forward_visible(0,2,0,1,1,1,0,5,0,40))return 8;
    std::cout<<"Scene near/behind/far/large/finite GPU submit cull PASS\n";
    return 0;
}
