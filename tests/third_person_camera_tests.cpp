/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/render/third_person_camera.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
namespace
{
bool close(float a,float b){return std::abs(a-b)<0.02f;}
}
int main()
{
    const auto c=render::third_person_camera({4.0f,8.0f,25.0f},{});
    if(!c.position_valid||!c.orientation_valid ||
       !close(c.position.x,-4.0f)||!close(c.position.y,8.0f)||
       !close(c.position.z,30.65f)||c.forward.x<=0.0f||
       c.forward.z>=0.0f)return 1;
    // 90-degree yaw; orientation remains behind avatar to the south.
    const auto yaw=render::third_person_camera({4.0f,8.0f,25.0f},
        {0.0f,0.0f,0.70710678f,0.70710678f});
    if(!close(yaw.position.x,4.0f)||!close(yaw.position.y,0.0f)||
       !close(yaw.left.x,-1.0f)||!close(yaw.forward.x,0.0f))return 2;
    const auto scaled=render::third_person_camera({4.0f,8.0f,25.0f},
        {0.0f,0.0f,7.0710678f,7.0710678f});
    if(!close(scaled.position.x,yaw.position.x)||
       !close(scaled.position.y,yaw.position.y))return 3;
    const auto no_rotation=render::third_person_camera({4.0f,8.0f,25.0f},
        {0.0f,0.0f,0.0f,0.0f});
    if(no_rotation.orientation_valid||!close(no_rotation.position.x,-4.0f))
        return 4;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    const auto invalid=render::third_person_camera({nan,8.0f,25.0f},
        {nan,0.0f,0.0f,1.0f});
    if(invalid.position_valid||invalid.orientation_valid||
       !std::isfinite(invalid.position.x)||
       !std::isfinite(invalid.position.y)||
       !std::isfinite(invalid.position.z)||
       !std::isfinite(invalid.forward.z))return 5;
    const auto tilted=render::third_person_camera({4.0f,8.0f,25.0f},
        {0.0f,0.3f,0.0f,0.9539392f});
    if(!tilted.orientation_valid||!close(tilted.position.x,-4.0f))
        return 6;
    const auto huge=render::third_person_camera(
        {std::numeric_limits<float>::max(),8.0f,25.0f},{});
    if(huge.position_valid||!std::isfinite(huge.position.x))return 7;
    std::cout<<"Third-person yaw, camera target, quaternion norm and invalid input PASS\n";
    return 0;
}
