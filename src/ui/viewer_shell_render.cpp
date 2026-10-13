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
                    ?"ChatFromViewer / lokaler Kanal 0"
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
             (v.region_connected?"verbunden":"nicht verbunden"),
             v.region_connected?theme::success:theme::warning);
        line(2,"Eigene Position: "+position(v.agent_position[0],
                                            v.agent_position[1],
                                            v.agent_position[2]));
        line(3,"Regiongroesse: "+std::to_string(v.region_width)+" x "+
             std::to_string(v.region_height)+" Meter");
        line(4,"Objekte: "+std::to_string(v.objects)+
             "  |  Avatare: "+std::to_string(v.remote_avatars));
        line(5,"Terrain-Patches: "+std::to_string(v.terrain_patches));
        line(6,"Terrain: "+std::string(v.terrain_ready?"bereit":"laedt")+
             "  |  Parzelle: "+
             std::string(v.parcel_ready?"bereit":"nicht geladen"));
        line(7,"Eigene aktive Animationen: "+std::to_string(v.animations));
        line(9,"F7: Region / ESC: schliessen",theme::cyan);
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
        line(0,"Avatar: "+compact(v.avatar_name,50U),theme::text);
        line(1,"Position: "+position(v.agent_position[0],
                                    v.agent_position[1],
                                    v.agent_position[2]));
        line(2,std::string("Appearance: ")+
             (v.appearance_ready?"Simulator-Daten da":"noch nicht geladen"));
        line(3,"Aktive Animationen: "+std::to_string(v.animations));
        line(4,"Attachments: "+std::to_string(v.attachments));
        line(7,"Outfits: Avatar > Outfit-Galerie (nur lesend)",
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
        line(6,"F6: Chat  /  F7: Region  /  F8: Erlebnisse");
        line(7,"F9: Outfits  /  F10: Inventar  /  F11: Vollbild");
        line(8,"ESC: Fenster schliessen",theme::cyan);
    }
}
}
