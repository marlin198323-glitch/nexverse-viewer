#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/render/sky_projection.hpp"
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
        first.at("mid")!=0U || first.at("far")!=0U ||
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
    // Server sun/east angles must be projected through the actual
    // viewer camera, not stuck at fixed screen XY on every view.
    nexverse::render::RenderEnvironment env{};
    env.available=true;env.east_angle=0.0f;env.sun_angle=0.0f;
    nexverse::render::RenderCamera camera{};
    camera.forward={1,0,0};
    camera.left={0,1,0};
    camera.up={0,0,1};
    camera.vertical_fov_degrees=60.0f;
    const auto front=nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f);
    if(!front.sun_in_view ||std::abs(front.x-0.5f)>0.001f ||
       std::abs(front.y-0.5f)>0.001f)return 13;
    camera.forward={-1,0,0};
    const auto rear=nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f);
    if(rear.sun_in_view)return 14;
    camera.forward={1,0,0};
    env.east_angle=3.14159265358979323846f*0.5f;
    const auto side=nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f);
    if(side.sun_in_view)return 15;
    env.east_angle=0.0f;
    camera.forward={1,0,0.35f};
    camera.up={-0.35f,0,1};
    const auto pitch=nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f);
    if(!(pitch.camera_pitch>0.1f)||
       !(pitch.y<0.5f))return 16;
    env.sun_angle=std::numeric_limits<float>::quiet_NaN();
    if(nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f).sun_in_view)return 17;
    // A newly visible near-side building no longer receives distant
    // 64m/128m low-res mips, while texture GPU budgets still apply.
    const auto near_high=plan_texture_mip_levels(
        {{"street",42,0,0},{"trees",90,0,0},{"far",230,0,0}},
        0,0,0,{}, {});
    if(near_high.at("street")!=0U||
       near_high.at("trees")!=1U||
       near_high.at("far")!=3U)return 18;
    std::cout<<"CPU mip RGBA, alpha, distance hysteresis and byte-budget OK\n";
    return 0;
}
