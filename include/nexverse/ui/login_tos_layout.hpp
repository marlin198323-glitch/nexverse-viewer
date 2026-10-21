/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
namespace nexverse::ui
{
struct TosRect
{
    float x=0,y=0,w=0,h=0;
    bool contains(double mx,double my) const noexcept
    {
        return mx>=x&&my>=y&&mx<x+w&&my<y+h;
    }
};
enum class TosAction{none,open_document,accept,decline};
struct TosLayout
{
    TosRect dialog,open_document,accept,decline;
    explicit TosLayout(int width,int height)
    {
        const float w=static_cast<float>(std::max(width,1));
        const float h=static_cast<float>(std::max(height,1));
        const float sc=std::clamp(std::min(w/1280.0f,h/800.0f),0.64f,1.3f);
        dialog={w*0.5f-390.0f*sc,h*0.5f-190.0f*sc,
                780.0f*sc,380.0f*sc};
        open_document={dialog.x+30.0f*sc,dialog.y+212.0f*sc,
                       310.0f*sc,48.0f*sc};
        accept={dialog.x+363.0f*sc,dialog.y+212.0f*sc,
                385.0f*sc,48.0f*sc};
        decline={dialog.x+30.0f*sc,dialog.y+286.0f*sc,
                 718.0f*sc,48.0f*sc};
    }
    TosAction hit(double x,double y,bool can_accept) const noexcept
    {
        if(open_document.contains(x,y))return TosAction::open_document;
        if(accept.contains(x,y))return can_accept
            ?TosAction::accept:TosAction::none;
        if(decline.contains(x,y))return TosAction::decline;
        return TosAction::none;
    }
};
}
