/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/render/renderer.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace nexverse::render
{
// OpenMetaverse ParticleDataFlags. These apply to individual particles,
// not to the source-system flags.
inline constexpr std::uint32_t particle_interp_color = 0x001U;
inline constexpr std::uint32_t particle_interp_scale = 0x002U;
inline constexpr std::uint32_t particle_bounce = 0x004U;
inline constexpr std::uint32_t particle_wind = 0x008U;
inline constexpr std::uint32_t particle_follow_source = 0x010U;
inline constexpr std::uint32_t particle_target_position = 0x040U;
inline constexpr std::uint32_t particle_target_linear = 0x080U;
inline constexpr std::uint32_t particle_emissive = 0x100U;
inline constexpr std::uint32_t particle_beam = 0x200U;
inline constexpr std::uint32_t particle_ribbon = 0x400U;

struct ParticleMotionSample
{
    RenderVec3 position;
    float fraction = 0.0f;
};

// Capped fixed-step integration prevents huge time jumps from
// destabilizing local wind and bounce approximation. No physics queries
// or fictitious terrain heights are used: source altitude is the floor.
[[nodiscard]] inline ParticleMotionSample sample_particle_motion(
    RenderVec3 source,
    RenderVec3 velocity,
    RenderVec3 acceleration,
    RenderVec3 wind,
    float age,
    float max_age,
    std::uint32_t flags,
    bool has_target = false,
    RenderVec3 target = {})
{
    ParticleMotionSample result;
    result.position = source;
    if (!std::isfinite(age) || !std::isfinite(max_age) ||
        max_age <= 0.0f) return result;
    age = std::clamp(age, 0.0f, std::min(max_age, 60.0f));
    result.fraction = age / max_age;

    if (has_target && (flags & particle_target_linear) != 0U)
    {
        result.position = {
            std::lerp(source.x, target.x, result.fraction),
            std::lerp(source.y, target.y, result.fraction),
            std::lerp(source.z, target.z, result.fraction)};
        return result;
    }
    const float step_size = age / 48.0f;
    RenderVec3 pos = source;
    for (int i = 0; i < 48 && step_size > 0.0f; ++i)
    {
        if ((flags & particle_wind) != 0U)
        {
            constexpr float drag = 0.65f;
            velocity.x += (wind.x - velocity.x) * std::min(1.0f, drag * step_size);
            velocity.y += (wind.y - velocity.y) * std::min(1.0f, drag * step_size);
        }
        velocity.x += acceleration.x * step_size;
        velocity.y += acceleration.y * step_size;
        velocity.z += acceleration.z * step_size;
        pos.x += velocity.x * step_size;
        pos.y += velocity.y * step_size;
        pos.z += velocity.z * step_size;
        if ((flags & particle_bounce) != 0U && pos.z < source.z)
        {
            pos.z = source.z + (source.z - pos.z);
            velocity.z = std::abs(velocity.z) * 0.55f;
        }
    }
    if (has_target && (flags & particle_target_position) != 0U)
    {
        const float t = result.fraction * result.fraction *
            (3.0f - 2.0f * result.fraction);
        pos.x = std::lerp(pos.x, target.x, t);
        pos.y = std::lerp(pos.y, target.y, t);
        pos.z = std::lerp(pos.z, target.z, t);
    }
    result.position = pos;
    return result;
}
}
