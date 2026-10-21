/* MPL-2.0: reject malformed LLUDP pose before app and render. */
#include "nexverse/world/simulator_scene_validity.hpp"
#include <iostream>
#include <limits>
using namespace nexverse;
int main()
{
    const auto nan=std::numeric_limits<float>::quiet_NaN();
    const auto inf=std::numeric_limits<float>::infinity();
    if(!world::finite_simulator_position({-10,20,30})||
       world::finite_simulator_position({inf,2,3})||
       world::finite_simulator_position({nan,2,3})||
       world::finite_simulator_position({1.0e7f,2,3}))return 1;
    if(!world::valid_simulator_avatar_pose({10,20,30},{0,0,0,1})||
       world::valid_simulator_avatar_pose({10,20,30},{0,0,0,0})||
       world::valid_simulator_avatar_pose({10,20,30},{0,0,0,3})||
       world::valid_simulator_avatar_pose({10,20,30},{nan,0,0,1}))
       return 2;
    if(!world::valid_simulator_prim_geometry({1,2,3},{0,0,0,1},{1,1,1})||
       world::valid_simulator_prim_geometry({1,2,3},{0,0,0,1},{inf,1,1})||
       world::valid_simulator_prim_geometry({1,2,3},{0,0,0,1},{-1,1,1})||
       world::valid_simulator_prim_geometry({1,2,3},{0,0,0,1},{70000,1,1}))
       return 3;
    // Default primitive quaternion, tolerated by existing legacy path.
    if(!world::valid_simulator_prim_geometry({1,2,3},{},{1,1,1}))
       return 4;
    std::cout<<"CURRENT_SIMULATOR_POSE_AND_PRIM_SCALE_INPUT_GUARD_PASS\n";
}