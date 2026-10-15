/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/outfit_gallery_layout.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    const OutfitGalleryLayout wide{1920,1080};
    const auto bx=wide.x+wide.pw-100.0f*wide.sc;
    const auto by=wide.y+120.0f*wide.sc;
    if(wide.hit(bx,by)!=OutfitGalleryHit::bake_preview_toggle)
        return 1;
    if(wide.hit(wide.x+wide.pw-30.0f*wide.sc,
         wide.y+25.0f*wide.sc)!=OutfitGalleryHit::close)
        return 2;
    if(wide.hit(wide.x+wide.pw-150.0f*wide.sc,
         wide.y+82.0f*wide.sc)!=OutfitGalleryHit::appearance_check)
        return 3;
    const OutfitGalleryLayout narrow{420,720};
    if(narrow.pw>=600.0f*narrow.sc)return 4;
    if(narrow.hit(narrow.x+narrow.pw-100.0f*narrow.sc,
        narrow.y+120.0f*narrow.sc)==OutfitGalleryHit::bake_preview_toggle)
        return 5;
    std::cout<<"F9 bake panel hit targets and narrow viewport fallback PASS\n";
    return 0;
}
