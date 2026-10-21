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
                  "https://stadt-nexverse.de/buergerverordnung#fake",
                  "https://stadt-nexverse.de/service/tos?token=private",
                  "https://stadt-nexverse.de/service/tos#not-tos",
                  "https://stadt-nexverse.de.evil/service/tos",
                  "https://attacker@stadt-nexverse.de/service/tos",
                  "http://stadt-nexverse.de:8080/service/tos",
                  "http://stadt-nexverse.de/service/tos/../admin"})
        if(s.challenge("tos",bad,avatar,grid)||s.visible())return 3;
    // Server #263 returns the HTTPS canonical address for OGL and
    // can return an HTTP /service/tos entrypoint for Firestorm.
    for(const auto modern:{"https://stadt-nexverse.de/service/tos",
                           "http://stadt-nexverse.de/service/tos",
                           "https://stadt-nexverse.de/service/tos/",
                           "http://stadt-nexverse.de/service/tos/"})
    {
        if(!s.challenge("tos",modern,avatar,grid)||!s.visible()||
           s.document_url!=protocol::nexverse_tos_page||
           s.agree()||s.approved_for(avatar,grid))return 12;
    }
    // Trusted legacy #260 still opens its *own* original document.
    for(const auto old:{"https://stadt-nexverse.de/buergerverordnung",
                        "http://stadt-nexverse.de/buergerverordnung/"})
    {
        if(!s.challenge("tos",old,avatar,grid)||
           s.document_url!=protocol::nexverse_legacy_tos_page)return 13;
    }
    if(!s.challenge("tos","https://stadt-nexverse.de/service/tos",
                    avatar,grid)||!s.visible()||s.agree()||
       s.approved_for(avatar,grid)||
       s.document_url!=protocol::nexverse_tos_page)return 4;
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
                    avatar,grid)||s.approved_for(avatar,grid)||s.agree()||
       s.document_url!=protocol::nexverse_legacy_tos_page)
        return 8;
    // Keyboard-only user must be able to reach every enabled action.
    if(ui::next_tos_focus(ui::TosAction::open_document,false)!=
           ui::TosAction::decline||
       ui::next_tos_focus(ui::TosAction::decline,false)!=
           ui::TosAction::open_document||
       ui::next_tos_focus(ui::TosAction::open_document,true)!=
           ui::TosAction::accept||
       ui::next_tos_focus(ui::TosAction::accept,true)!=
           ui::TosAction::decline||
       ui::next_tos_focus(ui::TosAction::decline,true)!=
           ui::TosAction::open_document||
       ui::next_tos_focus(ui::TosAction::accept,true,true)!=
           ui::TosAction::open_document||
       ui::next_tos_focus(ui::TosAction::open_document,true,true)!=
           ui::TosAction::decline)
        return 11;
    s.reset();
    if(s.visible()||s.approved_for(avatar,grid)||
       s.document_url!=protocol::nexverse_tos_page)return 9;
    ui::TosLayout widescreen{3440,1440},compact{1100,700};
    if(widescreen.dialog.w<compact.dialog.w||
       compact.dialog.x<0||compact.dialog.y<0||
       compact.decline.x+compact.decline.w>1100||
       compact.decline.y+compact.decline.h>700)return 10;
    std::cout<<"LOGIN_TOS_GESTURE_SCOPE_SECURITY_AND_LAYOUT_PASS\n";
}