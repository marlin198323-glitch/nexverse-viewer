/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/world_hud_toggle.hpp"
#include <iostream>
int main()
{
    using nexverse::ui::world_hud_toggle_rect;
    const auto box=world_hud_toggle_rect(1920,1080);
    if(!box.contains(1800,78)||box.contains(1600,78)||
       box.contains(1800,45)||box.contains(1940,78))return 1;
    const auto wide=world_hud_toggle_rect(3840,1080);
    if(!wide.contains(3720,77)||wide.contains(1800,77))return 2;
    const auto compact=world_hud_toggle_rect(1100,700);
    if(!compact.contains(compact.x+1,compact.y+1)||
       compact.y<=0.0f||compact.x<0.0f)return 3;
    using nexverse::ui::world_hud_click_target;
    using nexverse::ui::WorldHudClickTarget;
    if(world_hud_click_target(1920,1080,1800.0,78.0,false,false)!=
       WorldHudClickTarget::toggle_hud)return 4;
    if(world_hud_click_target(1920,1080,100.0,420.0,false,false)!=
       WorldHudClickTarget::scene)return 5;
    if(world_hud_click_target(1920,1080,100.0,420.0,true,false)!=
       WorldHudClickTarget::dashboard_or_floater)return 6;
    if(world_hud_click_target(1920,1080,100.0,420.0,false,true)!=
       WorldHudClickTarget::dashboard_or_floater)return 7;
    std::cout<<"World HUD clickable on/off button coordinates for 16:9, ultrawide, compact PASS\n";
    return 0;
}
