/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/ui/viewer_shell_render.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
namespace nexverse::ui
{
namespace
{
std::string compact(std::string value,std::size_t max=76U)
{
    if(value.size()>max)value.resize(max);
    for(char& ch:value)
        if(ch=='\r'||ch=='\n'||ch=='\t')ch=' ';
    return value;
}
std::string position(float a,float b,float c)
{
    const auto integer=[](float value)
    {return std::to_string(static_cast<int>(std::round(value)));};
    return integer(a)+" / "+integer(b)+" / "+integer(c);
}
std::string_view window_title(ViewerWindow window)
{
    switch(window)
    {
    case ViewerWindow::chat:return "Lokaler Chat";
    case ViewerWindow::move:return "Bewegung und Kamera";
    case ViewerWindow::teleport:return "Teleport";
    case ViewerWindow::region:return "Region und Simulator";
    case ViewerWindow::map:return "Karte der Region";
    case ViewerWindow::avatar:return "Mein Avatar";
    case ViewerWindow::animations:return "Aktive Avataranimationen";
    case ViewerWindow::objects:return "Objekte in der Naehe";
    case ViewerWindow::settings:return "Viewer-Einstellungen";
    case ViewerWindow::help:return "Hilfe / Tastatur";
    default:return "";
    }
}
}
void render_viewer_shell(UiRenderer& r,int width,int height,
                         const ViewerShellState& state,
                         const ViewerShellView& v)
{
    const ViewerShellLayout l{width,height};
    const float sc=l.scale;
    const std::array<float,5> menu_x{18.0f,145.0f,310.0f,448.0f,572.0f};
    // Always visible in-world, even without opening a floater.
    r.rect(0.0f,66.0f*sc,l.width,32.0f*sc,
           Color{0.010f,0.027f,0.048f,0.98f});
    r.rect(0.0f,97.0f*sc,l.width,1.0f*sc,
           theme::border);
    for(std::size_t i=0;i<menu_labels.size();++i)
    {
        const bool selected=state.open_menu==static_cast<int>(i);
        r.rect(menu_x[i]*sc,66.0f*sc,
               (i==1?151.0f:120.0f)*sc,30.0f*sc,
               selected?Color{0.055f,0.160f,0.250f,0.98f}
                       :Color{0.018f,0.044f,0.079f,0.92f},5.0f*sc);
        r.text((menu_x[i]+12.0f)*sc,75.0f*sc,9.6f*sc,
               menu_labels[i],selected?theme::cyan:theme::text);
    }
    if(state.open_menu>=0&&state.open_menu<5)
    {
        const auto m=static_cast<std::size_t>(state.open_menu);
        std::size_t rows=0U;
        for(const auto& item:menu_items[m])
            if(item.action!=ViewerAction::none)++rows;
        r.rect(menu_x[m]*sc,101.0f*sc,240.0f*sc,
               (12.0f+39.0f*static_cast<float>(rows))*sc,
               Color{0.012f,0.032f,0.056f,0.99f},8.0f*sc);
        for(std::size_t row=0;row<menu_items[m].size();++row)
        {
            const auto& item=menu_items[m][row];
            if(item.action==ViewerAction::none)continue;
            const bool disabled=item.action==ViewerAction::disabled;
            const float yy=(104.0f+39.0f*static_cast<float>(row))*sc;
            r.rect((menu_x[m]+4.0f)*sc,yy,
                   232.0f*sc,36.0f*sc,
                   disabled?Color{0.013f,0.022f,0.034f,0.98f}
                           :Color{0.029f,0.065f,0.108f,0.98f},5.0f*sc);
            r.text((menu_x[m]+14.0f)*sc,yy+9.0f*sc,9.8f*sc,
                   item.label,disabled?theme::text_muted:theme::text);
        }
    }
    if(state.window==ViewerWindow::none)return;
    // Modal input is captured by the same ViewerShellLayout hit-testing
    // code; scene/world toolbar clicks cannot leak behind the floater.
    const float x=l.panel_x,y=l.panel_y,pw=l.panel_w,ph=l.panel_h;
    r.rect(0.0f,98.0f*sc,l.width,l.height-98.0f*sc,
           Color{0.002f,0.009f,0.020f,0.48f});
    r.rect(x,y,pw,ph,Color{0.014f,0.032f,0.056f,0.99f},13.0f*sc);
    r.rect(x,y,pw,4.0f*sc,theme::cyan);
    r.text(x+25.0f*sc,y+22.0f*sc,17.0f*sc,
           window_title(state.window),theme::text);
    r.rect(x+pw-56.0f*sc,y+10.0f*sc,46.0f*sc,36.0f*sc,
           Color{0.130f,0.061f,0.125f,0.95f},7.0f*sc);
    r.text(x+pw-38.0f*sc,y+19.0f*sc,12.0f*sc,"X",theme::text);
    r.text(x+25.0f*sc,y+60.0f*sc,9.2f*sc,
           "OpenGenesisLINK  /  NexVerse  /  OpenSim",
           theme::cyan);
    const auto line=[&](int index,const std::string& text,
                        Color color=theme::text_muted)
    {
        r.text(x+27.0f*sc,
               y+(100.0f+static_cast<float>(index)*30.0f)*sc,
               10.0f*sc,compact(text,85U),color);
    };
    if(state.window==ViewerWindow::chat)
    {
        r.rect(x+23.0f*sc,y+97.0f*sc,pw-46.0f*sc,
               ph-210.0f*sc,Color{0.020f,0.047f,0.080f,0.94f},
               7.0f*sc);
        if(v.chat_lines.empty())
            line(2,"Noch keine lokalen Chat-Nachrichten empfangen.");
        const auto count=std::min(v.chat_lines.size(),std::size_t{8U});
        for(std::size_t i=0;i<count;++i)
            r.text(x+35.0f*sc,
                   y+(111.0f+static_cast<float>(i)*29.0f)*sc,
                   9.8f*sc,compact(v.chat_lines[
                       v.chat_lines.size()-count+i],79U),theme::text);
        const std::array<ChatDeliveryMode,3U> modes{
            ChatDeliveryMode::whisper,
            ChatDeliveryMode::say,
            ChatDeliveryMode::shout};
        const std::array<std::string_view,3U> labels{
            "Fluestern","Sagen","Rufen"};
        for(std::size_t i=0U;i<modes.size();++i)
        {
            const float bx=x+(26.0f+154.0f*static_cast<float>(i))*sc;
            const float by=y+ph-151.0f*sc;
            const bool active=state.chat_mode==modes[i];
            r.rect(bx,by,144.0f*sc,38.0f*sc,
                   active?Color{0.060f,0.172f,0.249f,0.98f}
                         :Color{0.023f,0.055f,0.084f,0.94f},7.0f*sc);
            r.text(bx+17.0f*sc,by+12.0f*sc,10.0f*sc,
                   labels[i],active?theme::cyan:theme::text_muted);
        }
        r.text(x+28.0f*sc,y+ph-103.0f*sc,8.0f*sc,
               "Standardreichweiten: 10 / 20 / 100 m; Serverkonfiguration gilt.",
               theme::text_muted);
        r.rect(x+26.0f*sc,y+ph-92.0f*sc,pw-212.0f*sc,
               48.0f*sc,Color{0.032f,0.077f,0.117f,0.99f},8.0f*sc);
        const auto prompt=state.chat_draft.empty()
            ?std::string{"Nachricht eingeben (ENTER = Senden)"}
            :compact(state.chat_draft,67U);
        r.text(x+38.0f*sc,y+ph-76.0f*sc,9.5f*sc,prompt,
               state.chat_draft.empty()?theme::text_muted:theme::text);
        r.rect(x+pw-170.0f*sc,y+ph-92.0f*sc,
               145.0f*sc,48.0f*sc,theme::blue,8.0f*sc);
        r.text(x+pw-138.0f*sc,y+ph-76.0f*sc,10.0f*sc,
               "Senden",theme::text);
        r.text(x+28.0f*sc,y+ph-29.0f*sc,8.6f*sc,
               state.status_text.empty()
                    ?"ChatFromViewer: "+std::string(chat_mode_name(state.chat_mode))+
                     " / Kanal 0 (lokal)"
                    :compact(state.status_text,84U),
               theme::cyan);
    }
    else if(state.window==ViewerWindow::teleport)
    {
        line(0,"Ziel in der verbundenen Region angeben.",theme::text);
        line(1,"Nur gueltige Zielkoordinaten werden gesendet.");
        const float fy=y+155.0f*sc;
        const std::array<std::string_view,3> labels{"X","Y","Z"};
        for(std::size_t i=0U;i<3U;++i)
        {
            const float fx=x+(30.0f+170.0f*static_cast<float>(i))*sc;
            r.text(fx,fy-26.0f*sc,9.0f*sc,labels[i],theme::cyan);
            r.rect(fx,fy,155.0f*sc,50.0f*sc,
                   static_cast<int>(i)==state.teleport_focus
                       ?Color{0.065f,0.150f,0.220f,0.98f}
                       :Color{0.028f,0.067f,0.108f,0.95f},7.0f*sc);
            r.text(fx+12.0f*sc,fy+15.0f*sc,11.0f*sc,
                   state.teleport_coords[i],theme::text);
        }
        line(5,"Region: "+compact(v.region_name,38U));
        line(6,"Aktuell: "+position(v.agent_position[0],
                                   v.agent_position[1],
                                   v.agent_position[2]));
        r.rect(x+pw-170.0f*sc,y+ph-100.0f*sc,
               145.0f*sc,48.0f*sc,theme::blue,8.0f*sc);
        r.text(x+pw-148.0f*sc,y+ph-84.0f*sc,
               10.0f*sc,"Teleport",theme::text);
        r.text(x+28.0f*sc,y+ph-27.0f*sc,8.6f*sc,
               state.status_text.empty()
                   ?"TAB: naechstes Feld / ENTER: Teleport anfordern"
                   :compact(state.status_text),
               state.status_text.empty()?theme::text_muted:theme::warning);
    }
    else if(state.window==ViewerWindow::region)
    {
        line(0,"Name: "+compact(v.region_name,55U),theme::text);
        line(1,std::string("Simulator: ")+
             (v.region_connected?"verbunden":"nicht verbunden")+
             " | Szene-Epoch: "+std::to_string(v.region_generation),
             v.region_connected?theme::success:theme::warning);
        line(2,"Eigene Position: "+position(v.agent_position[0],
                                            v.agent_position[1],
                                            v.agent_position[2]));
        line(3,"Regiongroesse: "+std::to_string(v.region_width)+" x "+
             std::to_string(v.region_height)+" Meter");
        line(4,"Objekte: "+std::to_string(v.objects)+
             " | Avatare: "+std::to_string(v.remote_avatars)+
             " (ObjUpdate:"+std::to_string(
                 v.remote_avatar_object_updates)+
             " / Karte:"+std::to_string(
                 v.remote_avatar_coarse_only)+")");
        line(5,"Terrain-Patches: "+std::to_string(v.terrain_patches));
        line(6,"Terrain: "+std::string(v.terrain_ready?"bereit":"laedt")+
             "  |  Parzelle: "+
             std::string(v.parcel_ready?"bereit":"nicht geladen"));
        line(7,"Eigene aktive Animationen: "+std::to_string(v.animations)+
             " | KillObject Avatar/Prims: "+
             std::to_string(v.removed_remote_avatar_kills)+"/"+
             std::to_string(v.removed_prim_kills)+
             " | ID neu:"+std::to_string(
                 v.remote_avatar_local_id_rebinds)+
             " / Prim neu:"+std::to_string(v.prim_local_id_rebinds));
        line(8,v.terrain_recovery_status.empty()
             ?"Terrain-Details werden ermittelt":
             compact(v.terrain_recovery_status,72U),
             theme::cyan);
        line(9,v.terrain_recovery_layers.empty()
             ?"Server-Layer: noch nicht ermittelt":
             compact(v.terrain_recovery_layers,72U),theme::cyan);
        const bool enabled=state.terrain_retry_available;
        r.rect(x+27.0f*sc,y+ph-67.0f*sc,228.0f*sc,39.0f*sc,
               enabled?theme::blue:
               Color{0.035f,0.054f,0.071f,0.98f},7.0f*sc);
        r.text(x+39.0f*sc,y+ph-55.0f*sc,9.5f*sc,
               enabled?"Fehlende Layer nachladen [T]":
                       "Nachladen derzeit gesperrt",
               enabled?theme::text:theme::text_muted);
        r.text(x+270.0f*sc,y+ph-53.0f*sc,8.6f*sc,
               state.status_text.empty()?"F7 / ESC: schliessen":
                   compact(state.status_text,49U),
               theme::text_muted);
    }
    else if(state.window==ViewerWindow::map)
    {
        line(0,"Lokaler Regionsausschnitt – keine globale Gridkarte.",
             theme::text);
        const float side=std::min(255.0f*sc,ph-159.0f*sc);
        const float mx=x+40.0f*sc,my=y+143.0f*sc;
        r.rect(mx,my,side,side,
               Color{0.020f,0.063f,0.095f,0.98f},7.0f*sc);
        const float sx=static_cast<float>(std::max(v.region_width,1U));
        const float sy=static_cast<float>(std::max(v.region_height,1U));
        const auto dot=[&](float px,float py,Color color,float size)
        {
            const float dx=std::clamp(px/sx,0.0f,1.0f)*side;
            const float dy=std::clamp(py/sy,0.0f,1.0f)*side;
            r.rect(mx+dx-size*0.5f,my+side-dy-size*0.5f,
                   size,size,color,size*0.5f);
        };
        for(const auto& pos:v.avatar_positions)
            dot(pos[0],pos[1],theme::magenta,7.0f*sc);
        if(v.avatar_present)
            dot(v.agent_position[0],v.agent_position[1],
                theme::cyan,11.0f*sc);
        r.text(mx+side+24.0f*sc,my+20.0f*sc,10.0f*sc,
               compact(v.region_name,28U),theme::text);
        r.text(mx+side+24.0f*sc,my+60.0f*sc,9.0f*sc,
               "Cyan: eigener Avatar",theme::cyan);
        r.text(mx+side+24.0f*sc,my+87.0f*sc,9.0f*sc,
               "Magenta: andere Avatare",theme::magenta);
        r.text(mx+side+24.0f*sc,my+130.0f*sc,9.0f*sc,
               "X/Y "+position(v.agent_position[0],
                               v.agent_position[1],0.0f),
               theme::text_muted);
        r.text(mx+side+24.0f*sc,my+155.0f*sc,8.0f*sc,
               "Welt > Teleport",theme::text_muted);
    }
    else if(state.window==ViewerWindow::avatar)
    {
        if(state.avatar_bake_page==0U)
        {
            line(0,"Avatar: "+compact(v.avatar_name,29U)+
                " | UDP-WASD:"+std::to_string(v.avatar_udp_directional_sent),
                theme::text);
            line(1,compact("Position: "+position(v.agent_position[0],
                                        v.agent_position[1],
                                        v.agent_position[2])+
                                        " | "+v.avatar_movement_status,72U));
            line(2,v.avatar_skin_status.empty()
                ?(v.appearance_ready?"Appearance: Server-Daten da":
                   "Appearance: noch nicht geladen")
                :v.avatar_skin_status);
            line(3,compact((v.avatar_animation_status.empty()
                ?"Anim: noch keine aktuellen Daten":v.avatar_animation_status)+
                " | "+v.avatar_skinning_status,72U));
            line(4,compact(
                (v.avatar_self_attachment_packet
                    ?"SSA-Anhaenge:"+std::to_string(v.attachments)+
                        " nur Server, Render offen"
                    :(v.appearance_ready
                        ?"SSA-Anhaenge: Legacy-Block fehlt"
                        :"SSA-Anhaenge: Serverantwort ausstehend"))+
                " | Obj-Anhaenge:"+std::to_string(
                    v.avatar_object_attachment_roots)+
                " | Bind:"+std::to_string(
                    v.avatar_attachment_effective_bindings)+
                " | Offen:"+std::to_string(
                    v.avatar_attachment_pending_low158)+
                " | Alt:"+std::to_string(
                    v.avatar_attachment_stale_low158)+
                " | HUD 3D aus:"+std::to_string(v.avatar_hud_world_excluded)+
                " | Linktiefe:"+std::to_string(v.avatar_scene_max_parent_depth)+
                " | Bone-Pose:"+std::to_string(v.avatar_bone_pose_avatars),
                72U),theme::text_muted);
            line(5,v.avatar_view_status,theme::cyan);
            line(6,v.avatar_fragment_status,theme::warning);
            line(7,compact(
                v.avatar_wearables_status+
                (v.avatar_admission_status.empty()
                    ?" | Bakes: "+v.avatar_bake_status
                    :" | "+v.avatar_admission_status),72U),
                theme::warning);
        }
        else
        {
            const std::size_t begin=state.avatar_bake_page==1U?0U:6U;
            const std::size_t end=state.avatar_bake_page==1U?6U:11U;
            line(0,compact(v.avatar_bake_summary,72U),theme::cyan);
            for(std::size_t i=begin;i<end;++i)
                line(1+static_cast<int>(i-begin),
                     compact(v.avatar_bake_rows[i],72U),
                     i<3U?theme::cyan:theme::text);
            line(7,compact(v.avatar_bake_visual,72U),theme::warning);
        }
        line(8,state.avatar_bake_page==0U
                ?compact(v.avatar_depth_parts,72U)
                :compact(v.avatar_bake_stream_status,72U),
             state.avatar_recheck_available?theme::cyan:theme::text_muted);
        line(9,compact(v.avatar_handshake_status,72U),
             v.appearance_ready?theme::success:theme::warning);
        r.rect(x+27.0f*sc,y+ph-93.0f*sc,126.0f*sc,41.0f*sc,
               Color{0.040f,0.157f,0.235f,0.98f},7.0f*sc);
        r.text(x+36.0f*sc,y+ph-79.0f*sc,8.7f*sc,
               "Bericht [Strg+S]",theme::cyan);
        r.rect(x+163.0f*sc,y+ph-93.0f*sc,126.0f*sc,41.0f*sc,
               Color{0.055f,0.165f,0.235f,0.98f},7.0f*sc);
        r.text(x+171.0f*sc,y+ph-79.0f*sc,8.8f*sc,
               state.avatar_bake_page==0U?"Bakes 1/2 [B]":
               state.avatar_bake_page==1U?"Bakes 2/2 [B]":
               "Uebersicht [B]",theme::cyan);
        r.rect(x+299.0f*sc,y+ph-93.0f*sc,126.0f*sc,41.0f*sc,
               state.avatar_recheck_available?theme::blue:
               Color{0.035f,0.054f,0.071f,0.98f},7.0f*sc);
        r.text(x+306.0f*sc,y+ph-79.0f*sc,8.6f*sc,
               state.avatar_recheck_available?"Bakes pruefen [R]":
               "Pruefung aus",
               state.avatar_recheck_available?theme::text:theme::text_muted);
        r.rect(x+435.0f*sc,y+ph-93.0f*sc,126.0f*sc,41.0f*sc,
               Color{0.030f,0.151f,0.215f,0.97f},7.0f*sc);
        r.text(x+441.0f*sc,y+ph-79.0f*sc,8.4f*sc,
               "Avatar Fokus [F4]",theme::cyan);
        if(pw>=715.0f*sc)
        {
            r.rect(x+571.0f*sc,y+ph-93.0f*sc,126.0f*sc,41.0f*sc,
                   Color{0.025f,0.142f,0.172f,0.98f},7.0f*sc);
            r.text(x+578.0f*sc,y+ph-79.0f*sc,8.2f*sc,
                   "Outfit [O]",theme::cyan);
        }
        r.text(x+27.0f*sc,y+ph-24.0f*sc,8.1f*sc,
               state.status_text.empty()?
                 "Self-Serverdaten; keine Uploads / IDs / Secrets.":
                 compact(state.status_text,76U),
               theme::text_muted);
    }
    else if(state.window==ViewerWindow::objects)
    {
        line(0,"Reale Objekte der verbundenen Region (bis 15 m).",
             theme::text);
        line(1,"Objekt waehlen, dann BERUEHREN gedrueckt halten.",
             theme::text_muted);
        line(2,v.mesh_quality_status.empty()?"LOD: noch keine Daten":
             v.mesh_quality_status,theme::cyan);
        for(std::size_t i=0U;i<v.nearby_objects.size()&&i<5U;++i)
        {
            const auto& obj=v.nearby_objects[i];
            const float yy=y+(122.0f+32.0f*static_cast<float>(i))*sc;
            const bool active=obj.local_id==state.selected_object_id;
            r.rect(x+27.0f*sc,yy,pw-54.0f*sc,30.0f*sc,
                active?Color{0.052f,0.168f,0.225f,0.96f}
                      :Color{0.020f,0.052f,0.082f,0.94f},6.0f*sc);
            r.text(x+39.0f*sc,yy+9.0f*sc,9.3f*sc,
                "Prim #"+std::to_string(obj.local_id)+"  ("+
                std::to_string(static_cast<int>(obj.distance_metres))+
                " m)",active?theme::cyan:theme::text);
            r.text(x+pw-325.0f*sc,yy+9.0f*sc,8.5f*sc,
                compact(obj.object_id,36U),theme::text_muted);
        }
        if(v.nearby_objects.empty())
            line(3,"Keine beruehrbaren Root-Prims im Umkreis.",
                 theme::warning);
        if(state.world_pick_object_id!=0U &&
           state.world_pick_object_id==state.selected_object_id)
        {
            const auto info=state.world_pick_exact
                ?"3D Mesh Face "+std::to_string(state.world_pick_face)+
                  " | UV "+std::to_string(state.world_pick_u)+
                  ", "+std::to_string(state.world_pick_v)
                :"3D-Klick: BoundingBox-Naeherung / kein exaktes Face";
            r.text(x+29.0f*sc,y+ph-125.0f*sc,8.0f*sc,
                   compact(info+
                     (state.touch_surface_sent?
                       " | SurfaceInfo gesendet":
                       " | SurfaceInfo nicht verfuegbar"),72U),
                   state.world_pick_exact?theme::cyan:theme::text_muted);
        }
        r.rect(x+pw-216.0f*sc,y+ph-90.0f*sc,
               188.0f*sc,43.0f*sc,
               state.selected_object_id!=0U?
                   Color{0.055f,0.165f,0.235f,0.98f}:
                   Color{0.025f,0.045f,0.064f,0.95f},7.0f*sc);
        r.text(x+pw-198.0f*sc,y+ph-74.0f*sc,10.0f*sc,
               state.held_object_id!=0U?"BERUEHRUNG AKTIV":"BERUEHREN",
               state.selected_object_id!=0U?theme::cyan:theme::text_muted);
        r.text(x+28.0f*sc,y+ph-25.0f*sc,8.2f*sc,
               state.status_text.empty()?
                   "Loslassen sendet ObjectDeGrab, kein synthetisches touch().":
                   compact(state.status_text,76U),
               state.held_object_id!=0U?theme::cyan:theme::text_muted);
    }
    else if(state.window==ViewerWindow::animations)
    {
        line(0,"Vom OpenSim-Simulator gemeldete Animationen.",
             theme::text);
        line(1,"Aktive Animationen: "+std::to_string(v.animation_entries.size())+
             " / geladene Clips: "+std::to_string(v.animation_cache_size));
        line(2,v.avatar_animation_status.empty()
            ?"Skin: Serverclip noch nicht in GPU-Body"
            :v.avatar_animation_status,
            theme::cyan);
        for(std::size_t i=0U;i<std::min(
            v.animation_entries.size(),std::size_t{8U});++i)
        {
            const float yy=y+(130.0f+31.0f*static_cast<float>(i))*sc;
            const bool selected=i==state.animation_selected;
            r.rect(x+26.0f*sc,yy,pw-52.0f*sc,29.0f*sc,
                   selected?Color{0.053f,0.151f,0.220f,0.98f}
                           :Color{0.021f,0.052f,0.084f,0.96f},5.0f*sc);
            const auto& [id,seq]=v.animation_entries[i];
            r.text(x+35.0f*sc,yy+8.0f*sc,9.1f*sc,
                   compact(id,38U),selected?theme::cyan:theme::text);
            r.text(x+pw-135.0f*sc,yy+8.0f*sc,9.1f*sc,
                   "Seq "+std::to_string(seq),theme::text_muted);
        }
        if(v.animation_entries.empty())
            line(3,"Noch kein aktiver Animationssatz empfangen.",
                 theme::warning);
        if(state.animation_selected<v.animation_entries.size())
        {
            const auto& [id,sequence]=
                v.animation_entries[state.animation_selected];
            r.text(x+28.0f*sc,y+ph-82.0f*sc,9.4f*sc,
                   "Ausgewaehlt: "+compact(id,38U),theme::cyan);
            const auto status=
                state.animation_selected<v.animation_asset_status.size()
                ?v.animation_asset_status[state.animation_selected]
                :std::string{"Warten auf Transfer"};
            r.text(x+28.0f*sc,y+ph-57.0f*sc,8.5f*sc,
                   "Sequenz: "+std::to_string(sequence)+" | "+
                   compact(status,57U),
                   v.selected_animation_retryable?
                       theme::warning:theme::text_muted);
            r.rect(x+pw-197.0f*sc,y+ph-100.0f*sc,
                   172.0f*sc,38.0f*sc,
                   v.selected_animation_retryable
                     ?Color{0.070f,0.166f,0.242f,0.97f}
                     :Color{0.022f,0.037f,0.054f,0.92f},7.0f*sc);
            r.text(x+pw-180.0f*sc,y+ph-89.0f*sc,9.0f*sc,
                   "Download erneut",
                   v.selected_animation_retryable?
                       theme::cyan:theme::text_muted);
        }
        if(v.animation_entries.size()>8U)
            r.text(x+28.0f*sc,y+ph-111.0f*sc,8.0f*sc,
                   "Weitere aktive Animationen vorhanden.",theme::warning);
        r.text(x+28.0f*sc,y+ph-28.0f*sc,8.0f*sc,
               "Server-Readback: manuelles Start/Stop noch nicht freigegeben",
               theme::warning);
    }
    else if(state.window==ViewerWindow::settings)
    {
        line(0,"Darstellung: NexVerse Dark / Cyan / Violett",theme::text);
        line(1,"Breitbild/Widescreen: dynamisch angepasst");
        line(2,"Fenstermodus: maximierbar und schliessbar");
        r.rect(x+30.0f*sc,y+155.0f*sc,190.0f*sc,44.0f*sc,
               theme::blue,7.0f*sc);
        r.text(x+43.0f*sc,y+168.0f*sc,10.0f*sc,
               "Vollbild umschalten",theme::text);
        line(5,"Weitere Firestorm-Einstellungen in Entwicklung.",
             theme::warning);
    }
    else if(state.window==ViewerWindow::move||
            state.window==ViewerWindow::help)
    {
        line(0,"W / S: vorwaerts / rueckwaerts",theme::text);
        line(1,"A / D: seitwaerts bewegen");
        line(2,"Pfeiltasten links/rechts: Avatar drehen");
        line(3,"SHIFT: schneller bewegen");
        line(4,"SPACE: springen  /  C: abwaerts");
        line(5,"F: Flugmodus umschalten");
        line(6,"F3: Avatar-Status  /  F6: Chat  /  F7: Region");
        line(7,"F9: Outfits  /  F10: Inventar  /  F11: Vollbild");
        line(8,"ESC: Fenster schliessen",theme::cyan);
    }
}
}
