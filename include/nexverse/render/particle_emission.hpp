/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/render/renderer.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>

namespace nexverse::render
{
// Numbered bursts retain their original emission timestamps.
// SourceMaxAge stops NEW births; already-born particles finish life.
[[nodiscard]] inline std::optional<float> particle_burst_age(
    float emitter_elapsed,
    float burst_interval,
    float particle_max_age,
    float source_max_age,
    std::size_t previous_bursts)
{
    if (!std::isfinite(emitter_elapsed) ||
        !std::isfinite(burst_interval) ||
        !std::isfinite(particle_max_age) ||
        !std::isfinite(source_max_age) ||
        emitter_elapsed < 0.0f || burst_interval <= 0.0f ||
        particle_max_age <= 0.0f || source_max_age < 0.0f)
        return std::nullopt;
    const float step = std::max(0.04f, burst_interval);
    const float current_burst = std::floor(emitter_elapsed / step);
    if (static_cast<float>(previous_bursts) > current_burst)
        return std::nullopt;
    const float born = (current_burst -
        static_cast<float>(previous_bursts)) * step;
    if (source_max_age > 0.0f && born >= source_max_age)
        return std::nullopt;
    const float age = emitter_elapsed - born;
    if (age < 0.0f || age >= particle_max_age)
        return std::nullopt;
    return age;
}

// Quaternion rotation for ObjectRelative source flags, applied to
// velocity and acceleration before world-space ballistic integration.
[[nodiscard]] inline RenderVec3 rotate_particle_vector(
    RenderQuaternion q, RenderVec3 v)
{
    const float norm =
        q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    if (!std::isfinite(norm) || norm < 0.000001f)
        return v;
    const float factor = 2.0f / norm;
    const RenderVec3 cross{
        q.y*v.z - q.z*v.y,
        q.z*v.x - q.x*v.z,
        q.x*v.y - q.y*v.x};
    const RenderVec3 second{
        q.y*cross.z - q.z*cross.y,
        q.z*cross.x - q.x*cross.z,
        q.x*cross.y - q.y*cross.x};
    return {
        v.x + factor * (q.w*cross.x + second.x),
        v.y + factor * (q.w*cross.y + second.y),
        v.z + factor * (q.w*cross.z + second.z)};
}
}
