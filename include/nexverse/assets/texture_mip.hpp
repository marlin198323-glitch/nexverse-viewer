/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_loader.hpp"
#include "nexverse/assets/texture_distance.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// Software downsampling prepares a smaller base image for GPU upload.
// The renderer still generates OpenGL mipmaps for that smaller image.
[[nodiscard]] std::optional<ImageRgba8> texture_mip_image(
    const ImageRgba8& source, std::uint8_t level);
[[nodiscard]] std::size_t gpu_rgba8_mip_bytes(
    std::uint32_t width, std::uint32_t height) noexcept;

// Proximity-driven base image quality, with hysteresis between bands.
// Active particle images are always requested at full resolution.
[[nodiscard]] std::unordered_map<std::string, std::uint8_t>
plan_texture_mip_levels(
    const std::vector<TextureDistanceSample>& samples,
    float x, float y, float z,
    const std::unordered_map<std::string, std::uint8_t>& previous,
    const std::unordered_set<std::string>& particle_ids);

// Plan lowest-priority evictions required to make room for an upload.
// No mutation occurs until the caller has verified the plan fits.
struct TextureByteBudgetPlan
{
    std::vector<std::string> eviction_ids;
    bool fits = false;
    std::size_t remaining_bytes = 0U;
};
[[nodiscard]] TextureByteBudgetPlan plan_texture_byte_budget(
    const std::vector<std::string>& prioritized,
    const std::unordered_map<std::string, std::size_t>& resident_bytes,
    std::string_view incoming_id, std::size_t incoming_bytes,
    std::size_t byte_budget);
}
