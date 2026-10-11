#include "nexverse/assets/texture_mip.hpp"
#include <iostream>
#include <limits>

using namespace nexverse::assets;
int main()
{
    // Red opaque beside fully transparent saturated green: no green bleed.
    const ImageRgba8 original{2U,2U,{
        255,0,0,255, 0,255,0,0,
        255,0,0,255, 0,255,0,0}};
    const auto half=texture_mip_image(original,1U);
    if (!half || half->width!=1U || half->height!=1U ||
        half->pixels!=std::vector<std::uint8_t>({255,0,0,128}))
        return 1;
    const auto same=texture_mip_image(original,0U);
    if (!same || same->pixels!=original.pixels ||
        texture_mip_image(original,13U))
        return 2;
    const ImageRgba8 odd{3U,5U,std::vector<std::uint8_t>(3U*5U*4U,255U)};
    const auto resized=texture_mip_image(odd,3U);
    if (!resized || resized->width!=1U || resized->height!=1U ||
        resized->pixels.size()!=4U || resized->pixels[3]!=255U)
        return 3;
    if (gpu_rgba8_mip_bytes(4,4)!=84U ||
        gpu_rgba8_mip_bytes(3,5)!=72U ||
        gpu_rgba8_mip_bytes(0,5)!=0U)
        return 4;
    std::vector<TextureDistanceSample> samples{
        {"near",10,0,0}, {"mid",42,0,0}, {"far",93,0,0},
        {"distant",200,0,0}, {"near",300,0,0},
        {"broken",std::numeric_limits<float>::quiet_NaN(),0,0}};
    const auto first=plan_texture_mip_levels(samples,0,0,0,{},{"far"});
    if (first.size()!=4U || first.at("near")!=0U ||
        first.at("mid")!=1U || first.at("far")!=0U ||
        first.at("distant")!=3U)
        return 5;
    const auto stable=plan_texture_mip_levels(
        {{"mid",69,0,0}},0,0,0,first,{});
    if (stable.at("mid")!=1U) return 6;
    const auto upgraded=plan_texture_mip_levels(
        {{"mid",12,0,0}},0,0,0,stable,{});
    if (upgraded.at("mid")!=0U) return 7;
    const std::unordered_map<std::string,std::size_t> resident{
        {"near",70U},{"mid",45U},{"far",25U}};
    const auto fit=plan_texture_byte_budget(
        {"incoming","near","mid","far"},resident,"incoming",50U,160U);
    if (!fit.fits || fit.eviction_ids.size()!=2U ||
        fit.eviction_ids[0]!="far" || fit.eviction_ids[1]!="mid" ||
        fit.remaining_bytes!=40U)
        return 8;
    const auto replace=plan_texture_byte_budget(
        {"near","mid","far"},resident,"near",20U,160U);
    if (!replace.fits || !replace.eviction_ids.empty())
        return 9;
    const auto too_large=plan_texture_byte_budget(
        {"near"},resident,"new",200U,160U);
    if (too_large.fits || !too_large.eviction_ids.empty())
        return 10;
    const auto forced=plan_texture_byte_budget(
        {"new","near","mid","far"},resident,"new",50U,50U);
    if (!forced.fits || forced.eviction_ids.size()!=3U)
        return 11;
    const auto low_priority=plan_texture_byte_budget(
        {"near","mid","far","new"},resident,"new",50U,160U);
    if (low_priority.fits || !low_priority.eviction_ids.empty())
        return 12;
    std::cout<<"CPU mip RGBA, alpha, distance hysteresis and byte-budget OK\n";
    return 0;
}
