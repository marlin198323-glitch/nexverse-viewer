/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/loading_diagnostics_layout.hpp"
#include <array>
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    for(const auto wh:std::array<std::array<int,2>,4>{{
        {{1100,700}},{{1920,1080}},{{3440,1440}},{{1280,720}}
    }})
    {
        const LoadingDiagnosticsLayout l{wh[0],wh[1]};
        const auto center=[](LoadingDiagRect r)
        {return std::array<double,2>{r.x+r.w*0.5,r.y+r.h*0.5};};
        for(const auto r:{l.modal,l.open,l.close,l.save,l.retry})
            if(r.x<0||r.y<0||r.w<1||r.h<1||
               r.x+r.w>wh[0]+0.1||r.y+r.h>wh[1]+0.1)return 1;
        auto p=center(l.open);
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],false)!=
           LoadingDiagAction::open)return 2;
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],true)!=
           LoadingDiagAction::none)return 3;
        p=center(l.save);
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],true)!=
           LoadingDiagAction::save)return 4;
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],false)!=
           LoadingDiagAction::none)return 5;
        p=center(l.retry);
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],true)!=
           LoadingDiagAction::retry)return 7;
        p=center(l.close);
        if(hit_loading_diagnostics(wh[0],wh[1],p[0],p[1],true)!=
           LoadingDiagAction::close)return 6;
    }
    std::cout<<"Loading F3 diagnostics real 16:9/32:9/open/save/close hit rects PASS\n";
    return 0;
}
