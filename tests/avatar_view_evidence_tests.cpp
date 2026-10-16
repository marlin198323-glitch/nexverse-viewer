/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_view_evidence.hpp"
#include <cmath>
#include <iostream>
#include <limits>
int main()
{
    using namespace nexverse;
    const protocol::Vec3 avatar{128.0f,128.0f,26.0f};
    const protocol::Quaternion quat{};
    const auto behind=render::third_person_camera(avatar,quat,8.0f,0.0f);
    const auto front=render::third_person_camera(avatar,quat,8.0f,180.0f);
    for(const auto& pose:{behind,front})
    {
        const auto e=ui::inspect_avatar_view(pose,avatar,1600,900);
        if(!e.valid_pose||!e.center_in_frustum||
           std::abs(e.normalized_x-0.5f)>0.03f||
           std::abs(e.normalized_y-0.5f)>0.03f||
           e.depth<7.0f)return 1;
    }
    const auto s=ui::inspect_avatar_view(
        render::third_person_camera(avatar,quat,8.0f,90.0f),
        avatar,3440,1440);
    if(!s.valid_pose||!s.center_in_frustum)return 2;
    auto away=behind;
    away.forward.x=-away.forward.x;
    away.forward.z=-away.forward.z;
    const auto opposite=ui::inspect_avatar_view(away,avatar,1600,900);
    if(opposite.valid_pose)return 3; // avatar is behind camera
    const float nan=std::numeric_limits<float>::quiet_NaN();
    const auto invalid=ui::inspect_avatar_view(behind,{nan,0,0},1600,900);
    if(invalid.valid_pose)return 4;
    if(ui::inspect_avatar_view(behind,avatar,0,900).valid_pose)return 5;
    if(ui::inspect_avatar_view(behind,avatar,1600,0).valid_pose)return 6;
    if(ui::describe_avatar_view(true,true,{},0,0)!=
       "Avatar: Kameraposition ungueltig")return 7;
    const auto projected=ui::inspect_avatar_view(behind,avatar,1600,900);
    if(ui::describe_avatar_view(true,true,projected,3,0).find("LLM GL 3")==
       std::string::npos)return 8;
    if(ui::describe_avatar_view(true,true,projected,0,7).find("Ersatz GL 7")==
       std::string::npos)return 9;
    if(ui::describe_avatar_view(false,false,projected,0,0)!=
       "Avatar: Simulator wartet")return 10;
    if(ui::describe_avatar_view(true,false,projected,0,0)!=
       "Avatar: AgentPosition fehlt")return 11;
    std::cout<<"Avatar on-screen projection truthful readiness, front/side/back, limits PASS\n";
    return 0;
}
