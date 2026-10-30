#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/render/sky_projection.hpp"
#include "nexverse/assets/scene_texture_budget_rescue.hpp"
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
    // Nighttime: even a geometrically front-facing sun below the
    // WindLight horizon must not be painted onto the visible sky.
    env.sun_angle=4.71238898f;
    camera.forward={0.0f,0.0f,-1.0f};
    camera.up={1.0f,0.0f,0.0f};
    const auto night=nexverse::render::project_environment_sun(
        env,camera,16.0f/9.0f);
    if(night.sun_in_view)return 19;
    // A newly visible near-side building no longer receives distant
    // 64m/128m low-res mips, while texture GPU budgets still apply.
    const auto near_high=plan_texture_mip_levels(
        {{"street",42,0,0},{"trees",90,0,0},{"far",230,0,0}},
        0,0,0,{}, {});
    if(near_high.at("street")!=0U||
       near_high.at("trees")!=1U||
       near_high.at("far")!=3U)return 18;
    // 1024x1024 RGBA requires >5 MiB including mipmaps. With a strict
    // 5 MiB family cap, admit real decoded pixels one mip smaller.
    const auto scene_rescue=choose_scene_texture_budget_mip_rescue(
        1024U,1024U,0U,0U,0U,128U*1024U*1024U,5U*1024U*1024U,
        {"scene"},{},"scene");
    if(!scene_rescue||*scene_rescue!=1U)return 20;
    if(choose_scene_texture_budget_mip_rescue(
        512U,512U,1U,0U,0U,128U*1024U*1024U,5U*1024U*1024U,
        {"scene"},{},"scene"))return 21;
    if(choose_scene_texture_budget_mip_rescue(
        2048U,2048U,6U,0U,0U,128U*1024U*1024U,1U,
        {"scene"},{},"scene"))return 22;
    SceneTextureMipRescueCooldown scene_cooldown;
    using Clock=SceneTextureMipRescueCooldown::Clock;
    const auto now=Clock::now();
    scene_cooldown.enter(2U);
    scene_cooldown.admitted("scene",3U,now);
    if(scene_cooldown.minimum_mip("scene",now)!=3U ||
       scene_cooldown.minimum_mip("scene",now+
           std::chrono::seconds{91})!=0U)return 23;
    scene_cooldown.enter(3U);
    if(scene_cooldown.minimum_mip("scene",now)!=0U)return 24;
    // Under the live 256 MiB texture pressure prefer one distant
    // resident, real non-avatar image for transactional downsampling.
    const std::vector<std::string> far_priority{
        "self-bake","particle","near","far","tiny"};
    const std::unordered_map<std::string,std::size_t> gpu_bytes{
        {"near",16U*1024U*1024U},
        {"far",8U*1024U*1024U},
        {"tiny",32U}};
    const std::unordered_map<std::string,std::uint8_t> gpu_mips{
        {"near",0U},{"far",1U},{"tiny",2U}};
    const std::unordered_set<std::string> protected_ids{
        "self-bake","particle"};
    const auto chosen=choose_scene_texture_compaction(
        far_priority,gpu_bytes,gpu_mips,gpu_mips,protected_ids,
        256U*1024U*1024U,256U*1024U*1024U,53U);
    if(!chosen||chosen->first!="far"||chosen->second!=2U)
        return 25;
    if(choose_scene_texture_compaction(
        far_priority,gpu_bytes,gpu_mips,gpu_mips,protected_ids,
        200U*1024U*1024U,256U*1024U*1024U,53U))
        return 26;
    if(choose_scene_texture_compaction(
        far_priority,gpu_bytes,gpu_mips,gpu_mips,protected_ids,
        256U*1024U*1024U,256U*1024U*1024U,0U))
        return 27;
    const std::unordered_set<std::string> all_protected{
        "self-bake","particle","near","far"};
    if(choose_scene_texture_compaction(
        far_priority,gpu_bytes,gpu_mips,gpu_mips,all_protected,
        256U*1024U*1024U,256U*1024U*1024U,53U))
        return 28;
    SceneTextureMipRescueCooldown pressure;
    pressure.enter(3U);
    pressure.requested("far",2U,now);
    if(pressure.pending_count(now)!=1U||
       pressure.minimum_mip("far",now)!=2U)
        return 29;
    if(pressure.confirm_requested("far",1U,now)||
       pressure.pending_count(now)!=1U)
        return 30;
    if(!pressure.confirm_requested("far",2U,now)||
       pressure.pending_count(now)!=0U||
       pressure.minimum_mip("far",now)!=2U||
       pressure.minimum_mip("far",now+std::chrono::seconds{91})!=0U)
        return 31;
    pressure.requested("near",1U,now);
    if(pressure.pending_count(now+std::chrono::seconds{9})!=0U||
       pressure.minimum_mip("near",now+std::chrono::seconds{9})!=0U)
        return 32;
    pressure.enter(4U);
    if(pressure.pending_count(now)!=0U ||
       pressure.minimum_mip("far",now)!=0U)
        return 33;
    // Bounded reservations must garbage-collect expired slots; otherwise
    // only eight ever-requested scene assets can be recovered per region.
    for(int index=0;index<14;++index)
        pressure.requested("material-"+std::to_string(index),1U,
            now+std::chrono::seconds{9*index});
    if(pressure.pending_count(now+std::chrono::seconds{13*9})!=1U)
        return 34;
    std::cout<<"CPU mip RGBA, alpha, distance hysteresis and byte-budget OK\n";
    return 0;
}
