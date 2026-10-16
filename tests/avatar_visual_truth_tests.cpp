/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_visual_truth.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    AvatarVisualTruth v{};
    if(describe_avatar_skin_truth(v).find("fehlt")==std::string::npos)return 1;
    v.server_appearance=true;
    if(describe_avatar_skin_truth(v).find("Head/Upper/Lower")==std::string::npos)
        return 2;
    v.server_body_references=3;
    if(describe_avatar_skin_truth(v).find("nicht vollstaendig")==
       std::string::npos)return 3;
    v.resident_body_bakes=3;
    if(describe_avatar_skin_truth(v).find("ausstehend")==std::string::npos)
        return 4;
    v.baked_body_gl_draws=3;
    if(describe_avatar_skin_truth(v).find("3/3")==std::string::npos)return 5;
    if(describe_avatar_animation_truth(v).find("lokaler Ersatz")==
       std::string::npos)return 6;
    v.server_animation_received=true;
    if(describe_avatar_animation_truth(v).find("Server meldet")==
       std::string::npos)return 7;
    v.server_selected_skin_draws=1;
    if(describe_avatar_animation_truth(v).find("Simulatorbewegung")==
       std::string::npos)return 8;
    v.keyframe_skin_draws=1;
    if(describe_avatar_animation_truth(v).find("Keyframe-Asset")==
       std::string::npos)return 9;
    std::cout<<"Avatar server-bake and actual GL animation truth stages PASS\n";
    return 0;
}
