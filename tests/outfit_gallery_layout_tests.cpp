/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/ui/outfit_gallery_layout.hpp"
#include <array>
#include <iostream>
using namespace nexverse::ui;
int main()
{
    for(const auto wh:std::array<std::array<int,2>,4>{
        {{1280,720},{1920,1080},{3440,1440},{5120,1440}}})
    {
        OutfitGalleryLayout g{wh[0],wh[1]};
        const float sc=g.sc;
        if(g.hit(g.x+g.pw-32.0f*sc,g.y+27.0f*sc)!=
           OutfitGalleryHit::close)return 1;
        if(g.hit(g.x+g.pw-151.0f*sc,g.y+24.0f*sc)!=
           OutfitGalleryHit::refresh)return 2;
        if(g.hit(g.x+g.pw-313.0f*sc,g.y+24.0f*sc)!=
           OutfitGalleryHit::verify)return 3;
        if(g.hit(g.x+g.pw-477.0f*sc,g.y+24.0f*sc)!=
           OutfitGalleryHit::plan)return 7;
        if(g.hit(g.x+g.pw-155.0f*sc,g.y+405.0f*sc)!=
           OutfitGalleryHit::link_add)return 8;
        if(g.hit(g.x+g.pw-155.0f*sc,g.y+384.0f*sc)==
           OutfitGalleryHit::link_add)return 9;
        if(g.hit(g.x+115.0f*sc,g.y+115.0f*sc)!=
           OutfitGalleryHit::select)return 4;
        if(g.selected_row(g.y+115.0f*sc)!=0U||
           g.selected_row(g.y+230.0f*sc)!=3U||
           g.selected_row(g.y+416.0f*sc)!=9U)return 5;
        if(g.hit(5.0f,5.0f)!=OutfitGalleryHit::none)return 6;
    }
    std::cout<<"Outfit GUI real close/refresh/COF verify and list selection hitboxes OK\n";
    return 0;
}
