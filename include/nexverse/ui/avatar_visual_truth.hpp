/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
#include <array>
#include <limits>
#include <string>
namespace nexverse::ui
{
// GL renderer counts are uint64_t. Diagnostics are uint32_t: a plain
// implicit cast can wrap to zero and falsely report "no server animation"
// or "no baked skin draw" after long sessions. Saturate, never wrap.
[[nodiscard]] constexpr std::uint32_t
clamp_avatar_gl_draw_count(std::uint64_t source) noexcept
{
    constexpr auto max=std::numeric_limits<std::uint32_t>::max();
    return source>max?max:static_cast<std::uint32_t>(source);
}
[[nodiscard]] constexpr std::uint32_t
sum_avatar_gl_body_draws(std::array<std::uint64_t,3U> draws) noexcept
{
    constexpr auto max=std::numeric_limits<std::uint32_t>::max();
    std::uint64_t result=0U;
    for(const auto count:draws)
    {
        if(count>=max-result)return max;
        result+=count;
    }
    return static_cast<std::uint32_t>(result);
}
struct AvatarVisualTruth
{
    bool server_appearance=false;
    std::uint32_t server_body_references=0U;
    std::uint32_t resident_body_bakes=0U;
    std::uint32_t baked_body_gl_draws=0U;
    bool server_animation_received=false;
    std::uint32_t keyframe_skin_draws=0U;
    std::uint32_t server_selected_skin_draws=0U;
};
[[nodiscard]] inline std::string describe_avatar_skin_truth(
    const AvatarVisualTruth& v)
{
    if(!v.server_appearance)
        return "Skin: ServerAppearance fehlt / Original LLM";
    if(v.server_body_references<3U)
        return "Skin: Head/Upper/Lower Server-Bakes fehlen";
    if(v.resident_body_bakes<3U)
        return "Skin: Server-Bakes noch nicht vollstaendig GPU";
    if(v.baked_body_gl_draws<3U)
        return "Skin: Bakes GPU, Original-Body-Draw ausstehend";
    return "Skin: 3/3 Server-Bakes im Original-GL-Draw";
}
[[nodiscard]] inline std::string describe_avatar_animation_truth(
    const AvatarVisualTruth& v)
{
    if(v.keyframe_skin_draws>0U)
        return "Anim: Keyframe-Asset auf originalem GL-Skin";
    if(v.server_selected_skin_draws>0U)
        return "Anim: Simulatorbewegung im GL-Skin";
    if(v.server_animation_received)
        return "Anim: Server meldet Clips / Bind-Skin";
    return "Anim: nur lokaler Ersatz / keine Serverclips";
}
}
