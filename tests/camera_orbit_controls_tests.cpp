/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/camera_orbit_controls.hpp"
#include "nexverse/render/third_person_camera.hpp"
#include <cmath>
#include <iostream>
#include <limits>
using namespace nexverse;
namespace {bool close(float a,float b){return std::abs(a-b)<0.025f;}}
int main()
{
    const float nan=std::numeric_limits<float>::quiet_NaN();
    if(ui::orbit_normalize(nan)!=0.0f||ui::orbit_step(nan,1)!=45.0f)return 1;
    if(ui::orbit_step(135,1)!=180||ui::orbit_step(180,1)!=-135)return 2;
    if(ui::orbit_front_back(0)!=180||ui::orbit_front_back(180)!=0||
       ui::orbit_front_back(90)!=180)return 3;
    if(ui::orbit_label(0)!="Hinten"||ui::orbit_label(180)!="Vorne"||
       ui::orbit_label(90)!="Rechts")return 4;
    const auto behind=render::third_person_camera({4,8,25},{},8,0);
    const auto side=render::third_person_camera({4,8,25},{},8,90);
    const auto front=render::third_person_camera({4,8,25},{},8,180);
    if(!close(behind.position.x,-4)||!close(behind.position.y,8)||
       !close(side.position.x,4)||!close(side.position.y,0)||
       !close(front.position.x,12)||!close(front.position.y,8)||
       front.forward.x>=0||side.forward.y<=0)return 5;
    const auto bad=render::third_person_camera({4,8,25},{},nan,nan);
    if(!std::isfinite(bad.position.x)||!std::isfinite(bad.forward.x)||
       !close(bad.position.x,-4.0f))return 6;
    const auto b=ui::camera_orbit_buttons(3440,1440);
    if(ui::hit_camera_orbit(3440,1440,b.left.x+1,b.left.y+1)!=
       ui::CameraOrbitHit::turn_left||
       ui::hit_camera_orbit(3440,1440,b.flip.x+1,b.flip.y+1)!=
       ui::CameraOrbitHit::front_back||
       ui::hit_camera_orbit(3440,1440,b.right.x+1,b.right.y+1)!=
       ui::CameraOrbitHit::turn_right)return 7;
    if(b.right.x+b.right.w>=ui::camera_zoom_buttons(3440,1440).near.x)
        return 8;
    std::cout<<"Third-person orbit front/side/back, NaN safe zoom, Widescreen buttons PASS\n";
    return 0;
}
