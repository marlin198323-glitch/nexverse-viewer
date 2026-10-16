/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/terrain_layer_coverage.hpp"
#include <iostream>
int main()
{
    using nexverse::render::terrain_material_status;
    if(terrain_material_status(false,false,4).find("kein GPU")==
       std::string::npos)return 1;
    if(terrain_material_status(true,false,4).find("Fallback")==
       std::string::npos)return 2;
    if(terrain_material_status(true,true,1).find("1/4")==
       std::string::npos)return 3;
    if(terrain_material_status(true,true,3).find("Ersatz")==
       std::string::npos)return 4;
    if(terrain_material_status(true,true,4).find("4/4")==
       std::string::npos)return 5;
    std::cout<<"Terrain texture layers distinguish real pixels from 1x1 fallback PASS\n";
    return 0;
}
