/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/world_hud_layout.hpp"
#include <iostream>
int main()
{
    using nexverse::ui::world_hud_panel_visibility;
    auto a=world_hud_panel_visibility(0.0f,1.0f);
    if(a.connection||a.system||a.avatar)return 1;
    a=world_hud_panel_visibility(284.0f,1.0f);
    if(a.connection||a.system||a.avatar)return 2;
    a=world_hud_panel_visibility(285.0f,1.0f);
    if(!a.connection||a.system||a.avatar)return 3;
    a=world_hud_panel_visibility(429.0f,1.0f);
    if(!a.connection||!a.system||a.avatar)return 4;
    a=world_hud_panel_visibility(540.0f,1.0f);
    if(!a.connection||!a.system||a.avatar)return 5;
    a=world_hud_panel_visibility(541.0f,1.0f);
    if(!a.connection||!a.system||!a.avatar)return 6;
    a=world_hud_panel_visibility(400.0f,0.74f);
    if(!a.connection||!a.system||a.avatar)return 7;
    a=world_hud_panel_visibility(1000.0f,0.0f);
    if(a.connection||a.system||a.avatar)return 8;
    std::cout<<"World HUD right cards never overlap anchored FPS footer PASS\n";
    return 0;
}
