/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace nexverse::assets
{
inline constexpr std::uint32_t max_j2c_side = 4096U;
inline constexpr std::size_t max_j2c_rgba_bytes = 64U*1024U*1024U;

// Restrict image dimensions and decoded footprint independently from the
// compressed stream length. Diagnostic strings must never include UUIDs,
// stream contents or server URLs.
[[nodiscard]] constexpr std::string_view jpeg2000_image_limits_reason(
    std::uint64_t width,std::uint64_t height,
    std::uint32_t components) noexcept
{
    if(width==0U||height==0U)return "image_extent";
    if(components<1U||components>4U)return "channel_count";
    if(width>max_j2c_side||height>max_j2c_side)
        return "image_dimensions";
    if(width*height>max_j2c_rgba_bytes/4U)
        return "rgba_memory_budget";
    return {};
}
[[nodiscard]] constexpr std::string_view jpeg2000_component_limits_reason(
    std::uint64_t width,std::uint64_t height,std::uint32_t dx,
    std::uint32_t dy,std::uint32_t precision) noexcept
{
    if(width==0U||height==0U)return "component_extent";
    if(width>max_j2c_side||height>max_j2c_side)
        return "component_dimensions";
    if(dx==0U||dy==0U)return "component_subsampling";
    if(precision<1U||precision>16U)return "component_precision";
    return {};
}
}
