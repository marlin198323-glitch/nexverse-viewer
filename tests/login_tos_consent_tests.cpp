/* MPL-2.0: explicit consent only, per server/avatar and user gesture. */
#include "nexverse/protocol/login_tos.hpp"
#include "nexverse/ui/login_tos_layout.hpp"
#include <iostream>
using namespace nexverse;
int main()
{
    protocol::LoginTosConsent s;
    constexpr auto grid="https://hg.stadt-nexverse.de/";
    constexpr auto avatar="Jane Resident";
    if(s.visible()||s.approved_for(avatar,grid)||s.agree()||
       s.document_opened())return 1;
    if(s.challenge("key","http://stadt-nexverse.de/buergerverordnung",
                   avatar,grid)||s.visible()||s.approved_for(avatar,grid))return 2;
    for(auto bad:{"http://attacker.example/terms",
                  "http://stadt-nexverse.de.evil.example/buergerverordnung",
                  "http://stadt-nexverse.de/buergerverordnung?session=SECRET",
                  "https://user:password@stadt-nexverse.de/buergerverordnung",
                  "javascript:alert(1)","file:///tmp/terms",
                  "https://stadt-nexverse.de/buergerverordnung#fake"})
        if(s.challenge("tos",bad,avatar,grid)||s.visible())return 3;
    if(!s.challenge("tos","http://stadt-nexverse.de/buergerverordnung",
                    avatar,grid)||!s.visible()||s.agree()||
       s.approved_for(avatar,grid))return 4;
    ui::TosLayout standard{1280,800};
    if(standard.hit(standard.accept.x+2,standard.accept.y+2,false)!=
          ui::TosAction::none||standard.hit(standard.decline.x+2,
            standard.decline.y+2,false)!=ui::TosAction::decline||
       standard.hit(standard.open_document.x+2,
                    standard.open_document.y+2,false)!=
           ui::TosAction::open_document)return 5;
    if(!s.document_opened()||s.document_opened()||!s.agree()||
       !s.approved_for(avatar,grid)||s.approved_for("Other Resident",grid)||
       s.approved_for(avatar,"https://evil.example/"))return 6;
    if(standard.hit(standard.accept.x+2,standard.accept.y+2,true)!=
       ui::TosAction::accept)return 7;
    // Version upgrades cause a new challenge; previous agree must vanish.
    if(!s.challenge("tos","https://stadt-nexverse.de/buergerverordnung",
                    avatar,grid)||s.approved_for(avatar,grid)||s.agree())
        return 8;
    s.reset();
    if(s.visible()||s.approved_for(avatar,grid))return 9;
    ui::TosLayout widescreen{3440,1440},compact{1100,700};
    if(widescreen.dialog.w<compact.dialog.w||
       compact.dialog.x<0||compact.dialog.y<0||
       compact.decline.x+compact.decline.w>1100||
       compact.decline.y+compact.decline.h>700)return 10;
    std::cout<<"LOGIN_TOS_GESTURE_SCOPE_SECURITY_AND_LAYOUT_PASS\n";
}