/* OpenGenesisLINK Viewer MPL-2.0.
 * Firestorm-style distinct font scaling independent of window chrome.
 * Raster tiers follow physical framebuffer pixels, never a GPU brand.
 */
#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <string_view>

namespace nexverse::ui
{
inline constexpr std::array<unsigned,6> font_raster_tiers{
    12U,16U,24U,36U,56U,96U};

[[nodiscard]] inline float readable_ui_font_size(float requested) noexcept
{
    if(!std::isfinite(requested)||requested<=0.0f)return 0.0f;
    // Existing neon layout uses 5–9px text on 720/1080p displays.
    // All viewport sizes need readable UI, not a single 48px downsized
    // atlas. This floor is independent of monitor resolution or VRAM.
    return std::clamp(requested,10.5f,96.0f);
}
[[nodiscard]] inline unsigned font_raster_tier(float logical_px,
                                               float framebuffer_scale=1.0f) noexcept
{
    const float physical_px=readable_ui_font_size(logical_px)*
        (std::isfinite(framebuffer_scale)
         ?std::clamp(framebuffer_scale,1.0f,3.0f):1.0f);
    // Nearest actual raster size, not "48px => 7px" bilinear blur.
    unsigned result=font_raster_tiers.front();
    float error=std::abs(physical_px-static_cast<float>(result));
    for(const unsigned candidate:font_raster_tiers)
    {
        const float next=std::abs(physical_px-static_cast<float>(candidate));
        if(next<error){result=candidate;error=next;}
    }
    return result;
}
[[nodiscard]] constexpr std::uint64_t glyph_cache_key(
    unsigned raster_px,std::uint32_t codepoint) noexcept
{
    return (static_cast<std::uint64_t>(raster_px)<<32U)|codepoint;
}

// Consume a full Unicode scalar, not the individual UTF-8 bytes.
// Invalid byte sequences become a '?' placeholder instead of invisible
// text or reading beyond the end of an untrusted server response.
template<typename Callback>
void for_each_ui_codepoint(std::string_view utf8,Callback call)
{
    for(std::size_t pos=0;pos<utf8.size();)
    {
        const auto b0=static_cast<unsigned char>(utf8[pos]);
        std::uint32_t cp=0U;
        std::size_t n=1U;
        if(b0<0x80U)cp=b0;
        else if(b0>=0xC2U&&b0<=0xDFU){n=2U;cp=b0&0x1FU;}
        else if(b0>=0xE0U&&b0<=0xEFU){n=3U;cp=b0&0x0FU;}
        else if(b0>=0xF0U&&b0<=0xF4U){n=4U;cp=b0&0x07U;}
        else {call(static_cast<std::uint32_t>('?'));++pos;continue;}
        bool valid=pos+n<=utf8.size();
        for(std::size_t i=1;i<n&&valid;++i)
        {
            const auto b=static_cast<unsigned char>(utf8[pos+i]);
            if((b&0xC0U)!=0x80U)valid=false;
            else cp=(cp<<6U)|(b&0x3FU);
        }
        if(!valid||cp>0x10FFFFU||
           (cp>=0xD800U&&cp<=0xDFFFU)||
           (n==3U&&cp<0x800U)||
           (n==4U&&cp<0x10000U))
        {
            call(static_cast<std::uint32_t>('?'));
            ++pos;
            continue;
        }
        call(cp);
        pos+=n;
    }
}
}
