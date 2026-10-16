/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cmath>
namespace nexverse::ui
{
struct LoadingDiagRect
{
    float x=0,y=0,w=0,h=0;
    [[nodiscard]] bool contains(double px,double py) const noexcept
    {
        return std::isfinite(px)&&std::isfinite(py)&&
            px>=x&&py>=y&&px<=x+w&&py<=y+h;
    }
};
struct LoadingDiagnosticsLayout
{
    float scale=1.0f;
    LoadingDiagRect open,modal,close,save,retry;
    explicit LoadingDiagnosticsLayout(int width,int height) noexcept
    {
        const float w=static_cast<float>(std::max(width,1));
        const float h=static_cast<float>(std::max(height,1));
        scale=std::clamp(std::min(w/1920.0f,h/1080.0f),0.78f,1.75f);
        open={w-218.0f*scale,18.0f*scale,200.0f*scale,
              38.0f*scale};
        const float pw=std::min(680.0f*scale,w-32.0f);
        const float ph=std::min(418.0f*scale,h-48.0f);
        modal={(w-pw)*0.5f,(h-ph)*0.5f,pw,ph};
        close={modal.x+modal.w-56.0f*scale,modal.y+10.0f*scale,
               43.0f*scale,34.0f*scale};
        save={modal.x+24.0f*scale,modal.y+modal.h-64.0f*scale,
              std::min(215.0f*scale,modal.w-40.0f*scale),
              43.0f*scale};
        retry={modal.x+modal.w-236.0f*scale,
               modal.y+modal.h-64.0f*scale,
               212.0f*scale,43.0f*scale};
    }
};
enum class LoadingDiagAction {none,open,close,save,retry};
[[nodiscard]] inline LoadingDiagAction hit_loading_diagnostics(
    int width,int height,double x,double y,bool showing) noexcept
{
    const LoadingDiagnosticsLayout l{width,height};
    if(showing)
    {
        if(l.close.contains(x,y))return LoadingDiagAction::close;
        if(l.save.contains(x,y))return LoadingDiagAction::save;
        if(l.retry.contains(x,y))return LoadingDiagAction::retry;
        return LoadingDiagAction::none;
    }
    return l.open.contains(x,y)
        ?LoadingDiagAction::open:LoadingDiagAction::none;
}
}
