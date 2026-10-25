/*
 * OpenGenesisLINK Viewer MPL-2.0; real neon login hit/render geometry.
 */
#include "nexverse/ui/neon_login_layout.hpp"
#include <array>
#include <iostream>

int main()
{
    using nexverse::ui::NeonLoginLayout;
    using nexverse::ui::NeonRect;
    const std::array<std::array<int,2>,6> sizes{{{{
        800,600}},{{1100,700}},{{1280,720}},{{1648,928}},
        {{1920,1080}},{{3440,1440}}}};
    const auto disjoint=[](NeonRect a,NeonRect b){
        return a.x+a.w<=b.x||b.x+b.w<=a.x||
               a.y+a.h<=b.y||b.y+b.h<=a.y;
    };
    for(const auto wh:sizes)
    {
        const NeonLoginLayout l{wh[0],wh[1]};
        if(l.header_h<30||l.footer_h<35||
           l.footer_y<=l.header_h)return 1;
        const auto inside=[&](NeonRect box){
            return box.x>=0&&box.y>=0&&
                box.x+box.w<=l.width+0.01f&&
                box.y+box.h<=l.height+0.01f;
        };
        if(!inside(l.brand)||!inside(l.grid)||!inside(l.footer)||
           !inside(l.username)||!inside(l.password)||
           !inside(l.password_eye)||!inside(l.enter_world)||
           !inside(l.remember_avatar)||!inside(l.world_status))
            return 2;
        if(!disjoint(l.username,l.password)||
           !disjoint(l.password,l.enter_world)||
           !disjoint(l.enter_world,l.remember_avatar))
            return 3;
        if(!l.username.contains(l.username.x+15,l.username.y+10)||
           !l.password.contains(l.password.x+15,l.password.y+10)||
           !l.password_eye.contains(l.password_eye.x+12,l.password_eye.y+9)||
           !l.enter_world.contains(l.enter_world.x+25,l.enter_world.y+18))
            return 4;
        if(!l.password.contains(l.password_eye.x+5,l.password_eye.y+5))
            return 5;
        if(l.username.y<l.footer.y||
           l.enter_world.y+l.enter_world.h>l.footer.y+l.footer.h)
            return 6;
    }
    // The first image's native 1648 x 928 reference resolution:
    const NeonLoginLayout ref{1648,928};
    if(ref.footer_y<800||ref.footer_y>835||
       ref.username.x<400||ref.username.x>440||
       ref.password.x<710||ref.password.x>770||
       ref.enter_world.x<1030||ref.enter_world.x>1090||
       ref.enter_world.w<250||
       ref.grid.x<640||ref.grid.x>685)
        return 7;
    std::cout << "OGL_NEON_LOGIN_LAYOUT_PASS "
              << "1648x928 and 5 scaled/ultrawide windows; "
              << "render and input share exact rectangles\n";
    return 0;
}
