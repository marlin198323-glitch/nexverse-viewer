/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/ui/viewer_shell.hpp"
#include <array>
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    const std::array<std::array<int,2>,3> viewports{{
        {{1100,700}},{{1920,1080}},{{3440,1440}}
    }};
    // The F3->F9 button is hidden if the status panel is too narrow,
    // rather than showing an off-window clickable region.
    {
        const ViewerShellLayout narrow{480,420};
        ViewerShellState small{};
        small.window=ViewerWindow::avatar;
        if(narrow.panel_w>=715.0f*narrow.scale)return 96;
        if(narrow.hit(narrow.panel_x+630.0f*narrow.scale,
            narrow.panel_y+narrow.panel_h-75.0f*narrow.scale,
            small)==ViewerAction::outfits)return 97;
    }
    for(const auto& wh:viewports)
    {
        const ViewerShellLayout l{wh[0],wh[1]};
        const float sc=l.scale;
        ViewerShellState s;
        int menu=-2;
        if(l.hit(32.0f*sc,80.0f*sc,s,&menu)!=ViewerAction::none||
           menu!=0)return 1;
        s.open_menu=0;
        if(l.hit(40.0f*sc,153.0f*sc,s)!=ViewerAction::inventory)
            return 2;
        if(l.hit(40.0f*sc,230.0f*sc,s)!=ViewerAction::animations)
            return 27;
        s.open_menu=2;
        if(l.hit(328.0f*sc,190.0f*sc,s)!=ViewerAction::teleport)
            return 3;
        s.open_menu=-1;
        if(l.hit(30.0f*sc,(144.0f+50.0f*5.0f+12.0f)*sc,s)!=
           ViewerAction::inventory)return 4;
        if(l.hit(30.0f*sc,(144.0f+50.0f*2.0f+12.0f)*sc,s)!=
           ViewerAction::disabled)return 5;
        if(l.hit((214.0f+104.0f*0.0f+16.0f)*sc,
                 l.height-(58.0f-12.0f)*sc,s)!=
           ViewerAction::chat)return 6;
        if(l.hit((214.0f+104.0f*4.0f+16.0f)*sc,
                 l.height-(58.0f-12.0f)*sc,s)!=
           ViewerAction::outfits)return 7;
        if(l.hit((214.0f+104.0f*6.0f+16.0f)*sc,
                 l.height-(58.0f-12.0f)*sc,s)!=
           ViewerAction::map)return 8;
        // Alpha1194: a REAL clickable Flight action in the existing
        // Movement panel, across standard and ultrawide monitors.
        // Disabled until a valid authenticated simulator SELF pose
        // exists; opening this non-editing panel must not block WASD.
        s.window=ViewerWindow::move;
        if(s.movement_input_captured())return 98;
        const float fx=l.panel_x+70.0f*sc;
        const float fy=l.panel_y+l.panel_h-77.0f*sc;
        if(l.hit(fx,fy,s)!=ViewerAction::disabled)return 99;
        s.fly_available=true;
        if(l.hit(fx,fy,s)!=ViewerAction::toggle_fly)return 100;
        if(l.hit(l.panel_x+270.0f*sc,fy,s)!=
           ViewerAction::none)return 101;
        s.fly_local_requested=true;
        if(l.hit(fx,fy,s)!=ViewerAction::toggle_fly)return 102;
        s.fly_available=false;
        s.fly_local_requested=false;
        // F3 is non-modal: diagnostic HUD cannot freeze WASD.
        s.window=ViewerWindow::avatar;
        if(s.movement_input_captured())return 90;
        s.window=ViewerWindow::region;
        if(s.movement_input_captured())return 91;
        s.window=ViewerWindow::chat;
        if(!s.movement_input_captured())return 92;
        s.window=ViewerWindow::teleport;
        if(!s.movement_input_captured())return 93;
        s.window=ViewerWindow::none;
        if(s.movement_input_captured())return 94;
        s.window=ViewerWindow::avatar;
        if(l.hit(l.panel_x+75.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::save_avatar_report)return 52;
        if(l.hit(l.panel_x+275.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::cycle_avatar_bakes)return 53;
        if(l.hit(l.panel_x+365.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::disabled)return 56;
        s.avatar_recheck_available=true;
        if(l.hit(l.panel_x+365.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::recheck_avatar_bakes)return 57;
        s.avatar_recheck_available=false;
        if(l.hit(l.panel_x+495.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::focus_avatar_camera)return 58;
        if(l.panel_w>=715.0f*sc &&
           l.hit(l.panel_x+630.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::outfits)return 95;
        if(l.hit(l.panel_x+l.panel_w-35.0f*sc,
                 l.panel_y+25.0f*sc,s)!=ViewerAction::close)return 54;
        if(window_for_action(ViewerAction::avatar)!=
           ViewerWindow::avatar)return 55;
        s.window=ViewerWindow::region;
        if(l.hit(l.panel_x+80.0f*sc,
                 l.panel_y+l.panel_h-50.0f*sc,s)!=ViewerAction::disabled)
            return 70;
        s.terrain_retry_available=true;
        if(l.hit(l.panel_x+80.0f*sc,
                 l.panel_y+l.panel_h-50.0f*sc,s)!=ViewerAction::retry_terrain)
            return 71;
        if(l.hit(l.panel_x+300.0f*sc,
                 l.panel_y+l.panel_h-50.0f*sc,s)!=ViewerAction::none)
            return 72;
        if(l.hit(l.panel_x+l.panel_w-35.0f*sc,
                 l.panel_y+25.0f*sc,s)!=ViewerAction::close)
            return 73;
        s.terrain_retry_available=false;
        s.window=ViewerWindow::chat;
        if(l.hit(l.panel_x+39.0f*sc,
                 l.panel_y+l.panel_h-135.0f*sc,s)!=
           ViewerAction::chat_whisper)return 30;
        if(l.hit(l.panel_x+193.0f*sc,
                 l.panel_y+l.panel_h-135.0f*sc,s)!=
           ViewerAction::chat_say)return 31;
        if(l.hit(l.panel_x+347.0f*sc,
                 l.panel_y+l.panel_h-135.0f*sc,s)!=
           ViewerAction::chat_shout)return 32;
        if(chat_mode_name(ChatDeliveryMode::whisper)!="Fluestern"||
           chat_mode_name(ChatDeliveryMode::say)!="Sagen"||
           chat_mode_name(ChatDeliveryMode::shout)!="Rufen")
            return 33;
        if(l.hit(l.panel_x+l.panel_w-35.0f*sc,
                 l.panel_y+25.0f*sc,s)!=ViewerAction::close)return 9;
        if(l.hit(l.panel_x+l.panel_w-100.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::send_chat)return 10;
        if(l.hit(l.panel_x+40.0f*sc,
                 l.panel_y+l.panel_h-75.0f*sc,s)!=
           ViewerAction::focus_chat)return 11;
        if(l.hit(30.0f*sc,80.0f*sc,s)!=ViewerAction::none)return 12;
        s.window=ViewerWindow::teleport;
        if(l.hit(l.panel_x+44.0f*sc,
                 l.panel_y+170.0f*sc,s)!=
           ViewerAction::focus_teleport_x)return 13;
        if(l.hit(l.panel_x+214.0f*sc,
                 l.panel_y+170.0f*sc,s)!=
           ViewerAction::focus_teleport_y)return 14;
        if(l.hit(l.panel_x+384.0f*sc,
                 l.panel_y+170.0f*sc,s)!=
           ViewerAction::focus_teleport_z)return 15;
        if(l.hit(l.panel_x+l.panel_w-100.0f*sc,
                 l.panel_y+l.panel_h-80.0f*sc,s)!=
           ViewerAction::send_teleport)return 16;
        s.window=ViewerWindow::objects;
        s.nearby_object_ids={17U,29U,43U};
        if(l.hit(l.panel_x+40.0f*sc,
                 l.panel_y+138.0f*sc,s)!=
           ViewerAction::select_object)return 35;
        if(l.hit(l.panel_x+40.0f*sc,
                 l.panel_y+170.0f*sc,s)!=
           ViewerAction::select_object)return 36;
        if(l.hit(l.panel_x+l.panel_w-99.0f*sc,
                 l.panel_y+l.panel_h-70.0f*sc,s)!=
           ViewerAction::touch_press)return 37;
        if(window_for_action(ViewerAction::objects)!=
            ViewerWindow::objects)return 38;
        if(!l.world_viewport_hit(550.0f*sc,400.0f*sc)||
            l.world_viewport_hit(22.0f*sc,400.0f*sc)||
            l.world_viewport_hit(550.0f*sc,33.0f*sc)||
            l.world_viewport_hit(l.width-10.0f*sc,400.0f*sc)||
            l.world_viewport_hit(550.0f*sc,l.height-8.0f*sc))
            return 40;
        s.window=ViewerWindow::none;
        s.open_menu=2;
        if(l.hit(466.0f*sc,244.0f*sc,s)!=
           ViewerAction::objects)return 39;
        s.open_menu=-1;
        s.window=ViewerWindow::animations;
        if(l.hit(l.panel_x+l.panel_w-100.0f*sc,
                 l.panel_y+l.panel_h-82.0f*sc,s)!=
           ViewerAction::retry_animation)return 34;
        if(l.hit(l.panel_x+42.0f*sc,l.panel_y+149.0f*sc,s)!=
           ViewerAction::select_animation)return 28;
        s.window=ViewerWindow::settings;
        if(l.hit(l.panel_x+50.0f*sc,
                 l.panel_y+175.0f*sc,s)!=
           ViewerAction::fullscreen)return 17;
        s.close();
        if(s.input_captured()||s.open_menu!=-1)return 18;
    }
    ViewerShellState state;
    state.teleport_coords={"128","50.5","31"};
    const auto valid=valid_teleport_target(state,256U,256U);
    if(!valid||(*valid)[0]!=128.0f||(*valid)[1]!=50.5f||
       (*valid)[2]!=31.0f)return 19;
    state.teleport_coords[0U]="256";
    if(valid_teleport_target(state,256U,256U))return 20;
    state.teleport_coords[0U]="nan";
    if(valid_teleport_target(state,256U,256U))return 21;
    state.teleport_coords[0U]="100foo";
    if(valid_teleport_target(state,256U,256U))return 22;
    state.teleport_coords[0U]="-1";
    if(valid_teleport_target(state,256U,256U))return 23;
    state.teleport_coords[0U]="100";
    state.teleport_coords[2U]="9999";
    if(valid_teleport_target(state,256U,256U))return 24;
    state.teleport_coords[2U]="-50";
    if(!valid_teleport_target(state,256U,256U))return 25;
    if(window_for_action(ViewerAction::animations)!=ViewerWindow::animations)
        return 29;
    if(window_for_action(ViewerAction::inventory)!=ViewerWindow::none ||
       window_for_action(ViewerAction::disabled)!=ViewerWindow::none ||
       window_for_action(ViewerAction::chat)!=ViewerWindow::chat)return 26;
    std::cout<<"Interactive ViewerShell menus, dock, modal focus, and teleport validation passed\n";
    return 0;
}
