/*
 * OpenGenesisLINK Viewer MPL-2.0; real neon login hit/render geometry.
 */
#include "nexverse/ui/neon_login_layout.hpp"
#include "nexverse/ui/notice_clipping.hpp"
#include "nexverse/ui/font_raster_policy.hpp"
#include <array>
#include <iostream>
#include <cstdint>
#include <vector>

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
        if(l.header_h<25||l.footer_h<27||
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
        if(!l.username.contains(l.username.x+l.username.w*0.5f,
                                l.username.y+l.username.h*0.5f)||
           !l.password.contains(l.password.x+l.password.w*0.5f,
                                l.password.y+l.password.h*0.5f)||
           !l.password_eye.contains(l.password_eye.x+l.password_eye.w*0.5f,
                                    l.password_eye.y+l.password_eye.h*0.5f)||
           !l.enter_world.contains(l.enter_world.x+l.enter_world.w*0.5f,
                                   l.enter_world.y+l.enter_world.h*0.5f))
            return 4;
        if(!l.password.contains(l.password_eye.x+5,l.password_eye.y+5))
            return 5;
        if(l.username.y<l.footer.y||
           l.enter_world.y+l.enter_world.h>l.footer.y+l.footer.h)
            return 6;
    }
    // The first image's native 1648 x 928 reference resolution:
    const NeonLoginLayout ref{1648,928};
    // Compact top/bottom bars leave more of the actual NexVerse
    // welcome page visible than the first reference implementation.
    if(ref.header_h!=58.0f||ref.footer_h!=60.0f||
       ref.footer_y!=868.0f||
       ref.username.x<425.0f||ref.username.x>433.0f||
       ref.username.w!=205.0f||ref.username.h!=32.0f||
       ref.password.x<642.0f||ref.password.x>651.0f||
       ref.enter_world.x<858.0f||ref.enter_world.x>867.0f||
       ref.enter_world.w!=180.0f||ref.enter_world.h!=32.0f||
       ref.grid.x<696.0f||ref.grid.x>701.0f||
       ref.remember_avatar.x<1410.0f||
       ref.remember_avatar.x>1430.0f||
       ref.welcome_content_height()!=810||
       ref.welcome_capture_height()!=810||
       NeonLoginLayout{2048,838}.welcome_capture_height()!=
           NeonLoginLayout{2048,838}.welcome_content_height()||
       NeonLoginLayout{2048,838}.welcome_content_height()>=810||
       NeonLoginLayout{800,600}.welcome_capture_height()!=
           NeonLoginLayout{800,600}.welcome_content_height())
        return 7;
    // Header and footer must be far apart; status/fields cannot
    // overlap. Particularly important in 720p and ultra-wide layouts.
    if(!disjoint(ref.brand,ref.grid)||
       !disjoint(ref.grid,ref.grid_status)||
       !disjoint(ref.login_status,ref.world_status))
        return 8;
    // A UTF-8 message must not be split mid-codepoint; do not
    // misreport presence failures by cutting them at 86 bytes.
    const auto glyph_width=[](std::string_view str)
    {
        std::size_t glyphs=0U;
        for(const auto ch:str)
            if((static_cast<unsigned char>(ch)&0xC0U)!=0x80U)
                ++glyphs;
        return static_cast<float>(glyphs*7U);
    };
    using nexverse::ui::fit_notice_to_width;
    const std::string warning=
        "Avatar ist noch angemeldet. Bitte den Gridbetreiber "
        "kontaktieren und die Kontobestätigung prüfen.";
    if(fit_notice_to_width(warning,1500.0f,glyph_width)!=warning)
        return 20;
    const auto clipped=fit_notice_to_width(warning,200.0f,glyph_width);
    if(clipped.size()<4U||!clipped.ends_with("...")||
       glyph_width(clipped)>200.0f)
        return 21;
    // Force the clip exactly inside a multi-byte 'ä' before '...'.
    const std::string utf8="Kontobestätigung";
    const auto small=fit_notice_to_width(utf8,7.0f*13U,glyph_width);
    // The final byte before "..." may legitimately be a UTF-8
    // continuation byte. Validate the complete prefix instead of
    // incorrectly treating that byte alone as proof of truncation.
    const auto valid_utf8=[](std::string_view value)
    {
        for(std::size_t i=0U;i<value.size();)
        {
            const auto h=static_cast<unsigned char>(value[i]);
            const auto n=h<0x80U?1U:
                (h>=0xC2U&&h<=0xDFU)?2U:
                (h>=0xE0U&&h<=0xEFU)?3U:
                (h>=0xF0U&&h<=0xF4U)?4U:0U;
            if(n==0U||i+n>value.size())return false;
            for(std::size_t j=1U;j<n;++j)
                if((static_cast<unsigned char>(value[i+j])&0xC0U)!=0x80U)
                    return false;
            i+=n;
        }
        return true;
    };
    if(!small.ends_with("...")||
       glyph_width(small)>7.0f*13U||
       !valid_utf8(small))
        return 22;
    if(!fit_notice_to_width("Test",3.0f,glyph_width).empty()||
       !fit_notice_to_width("",40.0f,glyph_width).empty())
        return 23;
    const auto hostile=fit_notice_to_width(
        std::string(600U,'x'),10000.0f,glyph_width);
    if(hostile.size()>515U||!hostile.ends_with("..."))
        return 24;

    // Independent of any monitor vendor and window resolution:
    // readable logical font size and physically appropriate FreeType
    // glyphs for 1x/1.5x/2x HiDPI displays.
    using nexverse::ui::readable_ui_font_size;
    using nexverse::ui::font_raster_tier;
    if(readable_ui_font_size(5.0f)!=10.5f||
       readable_ui_font_size(9.0f)!=10.5f||
       readable_ui_font_size(14.0f)!=14.0f||
       font_raster_tier(8.0f,1.0f)!=12U||
       font_raster_tier(8.0f,2.0f)!=24U||
       font_raster_tier(18.0f,2.0f)!=36U||
       font_raster_tier(18.0f,3.0f)!=56U)
        return 25;
    std::vector<std::uint32_t> decoded;
    nexverse::ui::for_each_ui_codepoint(
        "Ärger über Größe – Wohnen / Киев / Αθήνα",
        [&](std::uint32_t cp){decoded.push_back(cp);});
    if(decoded.size()<15U||
       std::find(decoded.begin(),decoded.end(),0x00C4U)==decoded.end()||
       std::find(decoded.begin(),decoded.end(),0x00FCU)==decoded.end()||
       std::find(decoded.begin(),decoded.end(),0x00DFU)==decoded.end()||
       std::find(decoded.begin(),decoded.end(),0x041AU)==decoded.end()||
       std::find(decoded.begin(),decoded.end(),0x0391U)==decoded.end())
        return 26;
    decoded.clear();
    nexverse::ui::for_each_ui_codepoint(
        std::string_view{"\xC3",1U},
        [&](std::uint32_t cp){decoded.push_back(cp);});
    if(decoded.size()!=1U||decoded.front()!='?')return 27;
    decoded.clear();
    nexverse::ui::for_each_ui_codepoint(
        std::string_view{"\xC0\xAF",2U},
        [&](std::uint32_t cp){decoded.push_back(cp);});
    if(decoded.size()!=2U||decoded[0]!='?'||decoded[1]!='?')
        return 28;
    if(nexverse::ui::glyph_cache_key(12U,0x00DCU)==
       nexverse::ui::glyph_cache_key(24U,0x00DCU))
        return 29;

    std::cout << "OGL_NEON_LOGIN_COMPACT_LAYOUT_PASS "
              << "1648x928 and 5 scaled/ultrawide windows; "
              << "render and input share exact rectangles\n";
    return 0;
}
