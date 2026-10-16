/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/camera_zoom_controls.hpp"
#include "nexverse/render/third_person_camera.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
int main()
{
    float z=ui::camera_zoom_default;
    z=ui::camera_zoom_adjust(z,4);
    if(z>=8.0f||z<ui::camera_zoom_min)return 1;
    z=ui::camera_zoom_adjust(z,-4);
    if(std::abs(z-ui::camera_zoom_default)>0.015f)return 2;
    if(ui::camera_zoom_adjust(8.0f,1000)>3.01f ||
       ui::camera_zoom_adjust(8.0f,-1000)<23.99f)return 3;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    if(ui::camera_zoom_adjust(nan,nan)!=ui::camera_zoom_default)return 4;
    const auto near=render::third_person_camera(
        {128,128,24},{},ui::camera_zoom_min);
    const auto far=render::third_person_camera(
        {128,128,24},{},ui::camera_zoom_max);
    if(!near.position_valid||!far.position_valid||
       std::abs(near.position.x-125.0f)>0.01f||
       std::abs(far.position.x-104.0f)>0.01f||
       std::abs(near.forward.x-far.forward.x)>0.01f)return 5;
    const auto controls=ui::camera_zoom_buttons(3440,1440);
    if(ui::hit_camera_zoom(3440,1440,controls.near.x+2,controls.near.y+2)!=
       ui::CameraZoomHit::near)return 6;
    if(ui::hit_camera_zoom(3440,1440,controls.far.x+2,controls.far.y+2)!=
       ui::CameraZoomHit::far)return 7;
    if(ui::hit_camera_zoom(3440,1440,100,100)!=
       ui::CameraZoomHit::none)return 8;
    if(ui::camera_zoom_label(3.75f)!="3.8m")return 9;
    std::cout<<"Camera wheel, limits, pose, and ultrawide +/- clickable controls PASS\n";
    return 0;
}
