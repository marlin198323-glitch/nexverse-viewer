/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_visual_truth.hpp"
#include <iostream>
#include <limits>
int main()
{
    using namespace nexverse::ui;
    AvatarVisualTruth v{};
    if(describe_avatar_skin_truth(v).find("fehlt")==std::string::npos)return 1;
    v.server_appearance=true;
    if(describe_avatar_skin_truth(v).find("Head/Upper/Lower")==std::string::npos)
        return 2;
    v.server_body_references=3;
    if(describe_avatar_skin_truth(v).find("nicht vollstaendig")==
       std::string::npos)return 3;
    v.resident_body_bakes=3;
    if(describe_avatar_skin_truth(v).find("ausstehend")==std::string::npos)
        return 4;
    v.baked_body_gl_draws=3;
    if(describe_avatar_skin_truth(v).find("3/3")==std::string::npos)return 5;
    if(describe_avatar_animation_truth(v).find("lokaler Ersatz")==
       std::string::npos)return 6;
    v.server_animation_received=true;
    if(describe_avatar_animation_truth(v).find("Server meldet")==
       std::string::npos)return 7;
    v.server_selected_skin_draws=1;
    if(describe_avatar_animation_truth(v).find("Simulatorbewegung")==
       std::string::npos)return 8;
    v.keyframe_skin_draws=1;
    if(describe_avatar_animation_truth(v).find("Keyframe-Asset")==
       std::string::npos)return 9;
    // Real renderer counters are 64-bit. A long-running session may
    // exceed 2^32 draws; diagnostic truth must not silently wrap to 0.
    constexpr auto u32max=std::numeric_limits<std::uint32_t>::max();
    constexpr auto u64max=std::numeric_limits<std::uint64_t>::max();
    static_assert(clamp_avatar_gl_draw_count(u64max)==u32max);
    static_assert(clamp_avatar_gl_draw_count(0U)==0U);
    static_assert(sum_avatar_gl_body_draws({u64max,u64max,u64max})==u32max);
    static_assert(sum_avatar_gl_body_draws({1U,2U,3U})==6U);
    if(clamp_avatar_gl_draw_count(u64max)!=u32max||
       sum_avatar_gl_body_draws({u64max,u64max,u64max})!=u32max||
       sum_avatar_gl_body_draws({0U,0U,0U})!=0U)return 10;
    v.keyframe_skin_draws=clamp_avatar_gl_draw_count(u64max);
    v.server_selected_skin_draws=clamp_avatar_gl_draw_count(u64max);
    v.baked_body_gl_draws=sum_avatar_gl_body_draws(
        {u64max,u64max,u64max});
    if(v.keyframe_skin_draws==0U||v.server_selected_skin_draws==0U||
       v.baked_body_gl_draws==0U)return 11;
    std::cout<<"Avatar server-bake and actual GL animation truth stages PASS\n";
    return 0;
}
