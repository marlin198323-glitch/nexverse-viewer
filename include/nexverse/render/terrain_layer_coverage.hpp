/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
#include <string>
namespace nexverse::render
{
// Reports the ACTUAL loaded texture layers for the current material epoch,
// not how many texture asset IDs were returned by the simulator.
[[nodiscard]] inline std::string terrain_material_status(
    bool terrain_drawn,bool shader_textured,std::uint32_t real_layers)
{
    if(!terrain_drawn)return "Boden: noch kein GPU-Draw";
    if(!shader_textured)return "Boden: Fallback (grau)";
    if(real_layers>=4U)return "Boden: 4/4 echte Terrain-Texturen";
    return "Boden: "+std::to_string(real_layers)+
        "/4 echte Texturen + Ersatz";
}
}
