/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/world_scene_proof.hpp"
#include <iostream>
#include <string>
using namespace nexverse::ui;
int main()
{
    WorldSceneProof proof{};
    if(describe_terrain_progress(proof)!="Terrain 0/256 wartet" ||
       describe_world_draw_progress(proof)!="Objekte warten | GL 0")
        return 1;
    proof.world_objects=42U;
    if(describe_world_draw_progress(proof)!="Objekte 42 empfangen | GL 0")
        return 2;
    proof.world_scene_draws=7U;
    proof.missing_mesh_draws=2U;
    if(describe_world_draw_progress(proof)!="GL 7 Draw | MeshFehlt 2")
        return 3;
    proof.terrain_available=true;
    proof.valid_terrain_patches=14U;
    if(describe_terrain_progress(proof)!="Terrain 14/256 Patches")
        return 4;
    proof.valid_terrain_patches=257U;
    if(describe_terrain_progress(proof)!="Terrain 256/256 Patches")
        return 5;
    proof.camera_fallback=true;
    if(describe_world_draw_progress(proof)!=
       "Kamera: ungültige Serverpose (Fallback)")return 6;
    proof.camera_fallback=false;
    std::cout<<"Terrain wait/stream and world CPU vs GL provenance PASS\n";
    return 0;
}
