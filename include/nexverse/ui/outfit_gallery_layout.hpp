/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <algorithm>
#include <cstddef>
#include <cmath>
namespace nexverse::ui
{
enum class OutfitGalleryHit { none, close, refresh, verify, plan, link_add, batch_add, appearance_check, wearable_check, select };
struct OutfitGalleryLayout
{
    float sc=1.0f,x=0.0f,y=0.0f,pw=790.0f;
    explicit OutfitGalleryLayout(int w,int h)
    {
        const float fw=static_cast<float>(std::max(1,w));
        const float fh=static_cast<float>(std::max(1,h));
        sc=std::min(std::clamp(fh/1080.0f,0.75f,1.5f),
                    std::max(0.4f,(fh-48.0f)/675.0f));
        pw=std::min(790.0f*sc,std::max(100.0f,fw-32.0f));
        x=(fw-pw)*0.5f;
        // The ten rows end at y+412*sc. Status begins y+524*sc.
        // Compact height uses the identical renderer and input scale.
        y=std::max(24.0f,(fh-675.0f*sc)*0.5f);
    }
    static bool inside(float px,float py,float x,float y,float w,float h)
    {return px>=x&&py>=y&&px<x+w&&py<y+h;}
    [[nodiscard]] OutfitGalleryHit hit(float px,float py) const
    {
        if(inside(px,py,x+pw-55.0f*sc,y+10.0f*sc,
                  45.0f*sc,35.0f*sc))return OutfitGalleryHit::close;
        if(inside(px,py,x+pw-226.0f*sc,y+11.0f*sc,
                  153.0f*sc,34.0f*sc))return OutfitGalleryHit::refresh;
        if(inside(px,py,x+pw-390.0f*sc,y+11.0f*sc,
                  151.0f*sc,34.0f*sc))return OutfitGalleryHit::verify;
        if(inside(px,py,x+pw-550.0f*sc,y+11.0f*sc,
                  147.0f*sc,34.0f*sc))return OutfitGalleryHit::plan;
        if(inside(px,py,x+pw-297.0f*sc,y+392.0f*sc,
                  274.0f*sc,37.0f*sc))return OutfitGalleryHit::link_add;
        if(inside(px,py,x+pw-297.0f*sc,y+439.0f*sc,
                  274.0f*sc,37.0f*sc))return OutfitGalleryHit::batch_add;
        if(inside(px,py,x+pw-211.0f*sc,y+64.0f*sc,
                  185.0f*sc,33.0f*sc))return OutfitGalleryHit::appearance_check;
        if(inside(px,py,x+pw-409.0f*sc,y+64.0f*sc,
                  186.0f*sc,33.0f*sc))return OutfitGalleryHit::wearable_check;
        if(inside(px,py,x+22.0f*sc,y+115.0f*sc,
                  425.0f*sc,300.0f*sc))return OutfitGalleryHit::select;
        return OutfitGalleryHit::none;
    }
    [[nodiscard]] std::size_t selected_row(float py) const
    {
        const float relative=(py-(y+115.0f*sc))/(30.0f*sc);
        if(!std::isfinite(relative)||relative<0.0f)return 0U;
        return std::min(static_cast<std::size_t>(relative),std::size_t{9U});
    }
};
}
