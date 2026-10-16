/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/terrain_texture_readiness.hpp"
#include <iostream>
int main()
{
    using nexverse::render::terrain_detail_gpu_ready;
    if(!terrain_detail_gpu_ready(true,2,2,5,5))return 1;
    if(terrain_detail_gpu_ready(false,2,2,5,5))return 2;
    if(terrain_detail_gpu_ready(true,1,2,5,5))return 3;
    if(terrain_detail_gpu_ready(true,2,2,4,5))return 4;
    if(!terrain_detail_gpu_ready(true,5,5,0,0))return 5;
    std::cout<<"Actual uploaded terrain detail texture is gated by region/material epoch PASS\n";
    return 0;
}
