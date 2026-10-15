/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/avatar_bake_preview.hpp"
#include <iostream>
#include <string>
#include <unordered_set>
using namespace nexverse;
int main()
{
    protocol::AvatarBakedTextures server{};
    server.ids[0]="actual-head";
    server.ids[1]="actual-upper";
    server.ids[2]="actual-lower";
    const std::unordered_set<std::string> resident{"actual-head","actual-lower","stale"};
    const auto none=render::select_avatar_bake_previews(server,resident,false);
    if(none.resident!=0U||!none.ids[0].empty()||
       !none.ids[2].empty())return 1;
    const auto selected=render::select_avatar_bake_previews(server,resident,true);
    if(selected.resident!=2U||selected.ids[0]!="actual-head"||
       !selected.ids[1].empty()||selected.ids[2]!="actual-lower")
        return 2;
    const auto empty=render::select_avatar_bake_previews(server,{},true);
    if(empty.resident || !empty.ids[0].empty()||
       !empty.ids[1].empty())return 3;
    protocol::AvatarBakedTextures reserved{};
    reserved.ids[0]=std::string{protocol::baked_placeholders[0]};
    reserved.ids[1]="00000000-0000-0000-0000-000000000000";
    const std::unordered_set<std::string> fake_gpu{reserved.ids[0],reserved.ids[1]};
    if(render::select_avatar_bake_previews(reserved,fake_gpu,true).resident)
       return 5;
    protocol::AvatarBakedTextures no_server{};
    if(render::select_avatar_bake_previews(no_server,resident,true).resident)
       return 4;
    std::cout<<"Self Avatar read-only server baked previews PASS\n";
    return 0;
}
