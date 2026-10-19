/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <algorithm>
#include <array>
#include <vector>
#include <charconv>
#include <cmath>
#include <optional>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace nexverse::ui
{
// This is the real in-world mouse hit layout. A menu/toolbar never
// advertises a functionality that is not wired to an explicit action.
enum class ViewerAction {
    none, close, chat, move, teleport, inventory, outfits,
    experiences, region, map, avatar, settings, help,
    send_chat, send_teleport, fullscreen, focus_chat,
    focus_teleport_x, focus_teleport_y, focus_teleport_z,
    refresh_inventory, animations, select_animation,
    chat_whisper, chat_say, chat_shout, retry_animation,
    objects, select_object, touch_press, save_avatar_report, retry_terrain,
    cycle_avatar_bakes, recheck_avatar_bakes, focus_avatar_camera, disabled
};
// Viewer-standard ChatFromViewer packet types: OpenSim and
// OpenGenesisLINK use the same values; inter-region llListen
// forwarding remains server-internal and is NOT a viewer feature.
enum class ChatDeliveryMode : std::uint8_t {
    whisper=0U, say=1U, shout=2U
};
[[nodiscard]] constexpr std::string_view chat_mode_name(
    ChatDeliveryMode mode) noexcept
{
    switch(mode)
    {
    case ChatDeliveryMode::whisper:return "Fluestern";
    case ChatDeliveryMode::say:return "Sagen";
    case ChatDeliveryMode::shout:return "Rufen";
    }
    return "Sagen";
}
enum class ViewerWindow {
    none,chat,move,teleport,region,map,avatar,animations,objects,settings,help
};
struct ViewerShellState
{
    ViewerWindow window=ViewerWindow::none;
    int open_menu=-1;
    std::string chat_draft;
    ChatDeliveryMode chat_mode=ChatDeliveryMode::say;
    std::string status_text;
    bool chat_submit_requested=false;
    bool teleport_submit_requested=false;
    std::array<std::string,3> teleport_coords{"128","128","25"};
    int teleport_focus=0;
    std::size_t animation_selected=0U;
    bool animation_retry_requested=false;
    std::uint8_t avatar_bake_page=0U; // overview, Head..Hair, other 5 bakes
    bool avatar_recheck_available=false;
    bool avatar_recheck_requested=false;
    bool terrain_retry_available=false;
    bool terrain_retry_requested=false;
    std::vector<std::uint32_t> nearby_object_ids;
    std::uint32_t selected_object_id=0U;
    std::uint32_t held_object_id=0U;
    std::uint32_t world_pick_face=0U;
    float world_pick_u=0.0f,world_pick_v=0.0f;
    bool world_pick_exact=false;
    bool touch_surface_sent=false;
    std::uint32_t world_pick_object_id=0U;
    bool object_touch_press_requested=false;
    bool object_touch_release_requested=false;
    bool world_pick_requested=false;
    float world_pick_x=0.0f,world_pick_y=0.0f;
    void close() noexcept { window=ViewerWindow::none; open_menu=-1; }
    [[nodiscard]] bool input_captured() const noexcept
    {return window!=ViewerWindow::none;}
    // World controls remain usable alongside non-editing status panels.
    // Only text-edit windows consume WASD and arrow keys.
    [[nodiscard]] bool movement_input_captured() const noexcept
    {return window==ViewerWindow::chat||window==ViewerWindow::teleport;}
};
struct ViewerMenuItem
{
    std::string_view label;
    ViewerAction action;
};
inline constexpr std::array<std::string_view,5> menu_labels{
    "Avatar","Kommunikation","Welt","Ansicht","Hilfe"
};
inline constexpr std::array<std::array<ViewerMenuItem,5>,5> menu_items{{
    {{{"Avatarstatus [F3]",ViewerAction::avatar},
      {"Inventar",ViewerAction::inventory},
      {"Outfit-Galerie",ViewerAction::outfits},
      {"Animationen",ViewerAction::animations},
      {"",ViewerAction::none}}},
    {{{"Lokaler Chat",ViewerAction::chat},
      {"Erlebnisse",ViewerAction::experiences},
      {"Freunde (geplant)",ViewerAction::disabled},
      {"",ViewerAction::none},
      {"",ViewerAction::none}}},
    {{{"Region",ViewerAction::region},
      {"Karte",ViewerAction::map},
      {"Teleport",ViewerAction::teleport},
      {"Objekte beruehren",ViewerAction::objects},
      {"",ViewerAction::none}}},
    {{{"Bewegung",ViewerAction::move},
      {"Einstellungen",ViewerAction::settings},
      {"Vollbild [F11]",ViewerAction::fullscreen},
      {"",ViewerAction::none},
      {"",ViewerAction::none}}},
    {{{"Steuerung",ViewerAction::help},
      {"Systemstatus",ViewerAction::region},
      {"",ViewerAction::none},
      {"",ViewerAction::none},
      {"",ViewerAction::none}}}
}};
inline constexpr std::array<std::string_view,9> action_labels{
    "Chat","Bewegen","Teleport","Inventar","Outfits","Erlebnisse","Karte",
    "Animationen","Objekte"
};
inline constexpr std::array<ViewerAction,9> action_commands{
    ViewerAction::chat,ViewerAction::move,ViewerAction::teleport,
    ViewerAction::inventory,ViewerAction::outfits,
    ViewerAction::experiences,ViewerAction::map,ViewerAction::animations,
    ViewerAction::objects
};
inline constexpr std::array<ViewerAction,7> navigation_commands{
    ViewerAction::region,ViewerAction::teleport,ViewerAction::disabled,
    ViewerAction::disabled,ViewerAction::disabled,
    ViewerAction::inventory,ViewerAction::disabled
};
struct ViewerShellLayout
{
    float width=1.0f,height=1.0f,scale=1.0f;
    float panel_x=0.0f,panel_y=0.0f,panel_w=0.0f,panel_h=0.0f;
    explicit ViewerShellLayout(int w,int h) noexcept
    {
        width=static_cast<float>(std::max(w,1));
        height=static_cast<float>(std::max(h,1));
        scale=std::clamp(height/1080.0f,0.74f,1.50f);
        panel_w=std::min(740.0f*scale,width-32.0f);
        panel_h=std::min(490.0f*scale,height-130.0f*scale);
        panel_x=(width-panel_w)*0.5f;
        panel_y=(height-panel_h)*0.5f;
    }
    [[nodiscard]] static bool inside(
        double px,double py,float x,float y,float w,float h) noexcept
    {return px>=x && py>=y && px<=x+w && py<=y+h;}
    // Transparent world viewport avoids picking through top/bottom
    // bars, navigation rail and the system status cards.
    [[nodiscard]] bool world_viewport_hit(double x,double y) const noexcept
    {
        return inside(x,y,215.0f*scale,110.0f*scale,
                      std::max(0.0f,width-495.0f*scale),
                      std::max(0.0f,height-168.0f*scale));
    }
    [[nodiscard]] ViewerAction hit(
        double x,double y,const ViewerShellState& state,
        int* menu_change=nullptr) const noexcept
    {
        if(menu_change)*menu_change=-2;
        if(state.window!=ViewerWindow::none)
        {
            if(state.window==ViewerWindow::avatar)
            {
                if(inside(x,y,panel_x+27.0f*scale,
                   panel_y+panel_h-93.0f*scale,
                   126.0f*scale,41.0f*scale))
                    return ViewerAction::save_avatar_report;
                if(inside(x,y,panel_x+163.0f*scale,
                   panel_y+panel_h-93.0f*scale,
                   126.0f*scale,41.0f*scale))
                    return ViewerAction::cycle_avatar_bakes;
                if(inside(x,y,panel_x+299.0f*scale,
                   panel_y+panel_h-93.0f*scale,
                   126.0f*scale,41.0f*scale))
                    return state.avatar_recheck_available
                        ?ViewerAction::recheck_avatar_bakes
                        :ViewerAction::disabled;
                if(inside(x,y,panel_x+435.0f*scale,
                   panel_y+panel_h-93.0f*scale,
                   126.0f*scale,41.0f*scale))
                    return ViewerAction::focus_avatar_camera;
                // F3 status links to the actual read-only F9 inventory
                // floater, not a decorative button or automatic wear.
                if(panel_w>=715.0f*scale&&
                   inside(x,y,panel_x+571.0f*scale,
                   panel_y+panel_h-93.0f*scale,
                   126.0f*scale,41.0f*scale))
                    return ViewerAction::outfits;
            }
            if(inside(x,y,panel_x+panel_w-56.0f*scale,
                      panel_y+10.0f*scale,46.0f*scale,36.0f*scale))
                return ViewerAction::close;
            if(state.window==ViewerWindow::chat)
            {
                // Chat mode buttons match the three actual LLUDP
                // ChatFromViewer wire modes; no client-side re-broadcast.
                for(std::size_t i=0U;i<3U;++i)
                    if(inside(x,y,
                              panel_x+(26.0f+154.0f*static_cast<float>(i))*scale,
                              panel_y+panel_h-151.0f*scale,
                              144.0f*scale,38.0f*scale))
                        return i==0U?ViewerAction::chat_whisper:
                               i==1U?ViewerAction::chat_say:
                                     ViewerAction::chat_shout;
                if(inside(x,y,panel_x+panel_w-170.0f*scale,
                          panel_y+panel_h-92.0f*scale,
                          145.0f*scale,48.0f*scale))
                    return ViewerAction::send_chat;
                if(inside(x,y,panel_x+26.0f*scale,
                          panel_y+panel_h-92.0f*scale,
                          panel_w-212.0f*scale,48.0f*scale))
                    return ViewerAction::focus_chat;
            }
            if(state.window==ViewerWindow::region &&
               inside(x,y,panel_x+27.0f*scale,
                      panel_y+panel_h-67.0f*scale,
                      228.0f*scale,39.0f*scale))
                return state.terrain_retry_available
                    ?ViewerAction::retry_terrain:ViewerAction::disabled;
            if(state.window==ViewerWindow::objects)
            {
                if(inside(x,y,panel_x+panel_w-216.0f*scale,
                          panel_y+panel_h-90.0f*scale,
                          188.0f*scale,43.0f*scale))
                    return ViewerAction::touch_press;
                for(std::size_t i=0U;i<5U;++i)
                    if(inside(x,y,panel_x+27.0f*scale,
                              panel_y+(122.0f+32.0f*static_cast<float>(i))*scale,
                              panel_w-54.0f*scale,30.0f*scale))
                        return ViewerAction::select_object;
            }
            if(state.window==ViewerWindow::animations)
            {
                if(inside(x,y,panel_x+panel_w-197.0f*scale,
                          panel_y+panel_h-100.0f*scale,
                          172.0f*scale,38.0f*scale))
                    return ViewerAction::retry_animation;
                for(std::size_t i=0U;i<8U;++i)
                    if(inside(x,y,panel_x+26.0f*scale,
                              panel_y+(130.0f+31.0f*static_cast<float>(i))*scale,
                              panel_w-52.0f*scale,29.0f*scale))
                        return ViewerAction::select_animation;
            }
            if(state.window==ViewerWindow::teleport)
            {
                if(inside(x,y,panel_x+panel_w-170.0f*scale,
                          panel_y+panel_h-100.0f*scale,
                          145.0f*scale,48.0f*scale))
                    return ViewerAction::send_teleport;
                const float input_y=panel_y+155.0f*scale;
                for(int i=0;i<3;++i)
                    if(inside(x,y,panel_x+(30.0f+170.0f*static_cast<float>(i))*scale,
                              input_y,155.0f*scale,50.0f*scale))
                        return i==0?ViewerAction::focus_teleport_x:
                               i==1?ViewerAction::focus_teleport_y:
                                    ViewerAction::focus_teleport_z;
            }
            if(state.window==ViewerWindow::settings&&
               inside(x,y,panel_x+30.0f*scale,panel_y+155.0f*scale,
                      190.0f*scale,44.0f*scale))
                return ViewerAction::fullscreen;
            // Modal panel consumes all clicks, including outside, so
            // buttons underneath cannot accidentally operate.
            return ViewerAction::none;
        }
        const std::array<float,5> menu_x{18.0f,145.0f,310.0f,448.0f,572.0f};
        for(std::size_t i=0;i<menu_x.size();++i)
            if(inside(x,y,menu_x[i]*scale,66.0f*scale,
                      (i==1?151.0f:120.0f)*scale,30.0f*scale))
            {
                if(menu_change)*menu_change=static_cast<int>(i);
                return ViewerAction::none;
            }
        if(state.open_menu>=0 &&
           static_cast<std::size_t>(state.open_menu)<menu_items.size())
        {
            const auto index=static_cast<std::size_t>(state.open_menu);
            for(std::size_t row=0;row<menu_items[index].size();++row)
            {
                const auto& item=menu_items[index][row];
                if(item.action==ViewerAction::none)continue;
                if(inside(x,y,menu_x[index]*scale,
                          (104.0f+39.0f*static_cast<float>(row))*scale,
                          240.0f*scale,36.0f*scale))
                    return item.action;
            }
            // Click outside dropdown dismisses menu, but never
            // activates an underlying world or toolbar action.
            if(menu_change)*menu_change=-3;
            return ViewerAction::none;
        }
        // Left navigation: all seven entries are visible; unavailable
        // ones are visually disabled and never trigger fake panels.
        const float nav_y=98.0f*scale+12.0f*scale+34.0f*scale;
        for(std::size_t i=0;i<navigation_commands.size();++i)
            if(inside(x,y,18.0f*scale,
                      nav_y+50.0f*scale*static_cast<float>(i),
                      178.0f*scale,48.0f*scale))
                return navigation_commands[i];
        const float bottom_y=height-58.0f*scale+11.0f*scale;
        const float bottom_x=214.0f*scale;
        for(std::size_t i=0;i<action_commands.size();++i)
            if(inside(x,y,bottom_x+104.0f*scale*static_cast<float>(i),
                      bottom_y,96.0f*scale,34.0f*scale))
                return action_commands[i];
        return ViewerAction::none;
    }
};
// Reject invalid/out-of-region coordinates before an explicit teleport
// click ever reaches the OpenSim outbound queue.
[[nodiscard]] inline std::optional<std::array<float,3>>
valid_teleport_target(const ViewerShellState& state,
                      unsigned region_width,unsigned region_height) noexcept
{
    std::array<float,3> target{};
    for(std::size_t i=0;i<target.size();++i)
    {
        const auto& field=state.teleport_coords[i];
        if(field.empty()||field.size()>18U)return std::nullopt;
        const auto result=std::from_chars(field.data(),
            field.data()+field.size(),target[i],
            std::chars_format::general);
        if(result.ec!=std::errc{} ||
           result.ptr!=field.data()+field.size() ||
           !std::isfinite(target[i]))return std::nullopt;
    }
    const float max_x=static_cast<float>(
        std::clamp(region_width,1U,4096U));
    const float max_y=static_cast<float>(
        std::clamp(region_height,1U,4096U));
    if(target[0]<0.0f||target[0]>=max_x||
       target[1]<0.0f||target[1]>=max_y||
       target[2]<-100.0f||target[2]>4096.0f)
        return std::nullopt;
    return target;
}
inline constexpr ViewerWindow window_for_action(ViewerAction action) noexcept
{
    switch(action)
    {
    case ViewerAction::chat:return ViewerWindow::chat;
    case ViewerAction::move:return ViewerWindow::move;
    case ViewerAction::teleport:return ViewerWindow::teleport;
    case ViewerAction::region:return ViewerWindow::region;
    case ViewerAction::map:return ViewerWindow::map;
    case ViewerAction::avatar:return ViewerWindow::avatar;
    case ViewerAction::settings:return ViewerWindow::settings;
    case ViewerAction::help:return ViewerWindow::help;
    case ViewerAction::animations:return ViewerWindow::animations;
    case ViewerAction::objects:return ViewerWindow::objects;
    default:return ViewerWindow::none;
    }
}
}
