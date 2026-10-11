/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cstddef>
namespace nexverse::assets
{
// Estimated VBO/EBO and RGBA8-mipmap resource accounting.
// Terrain, UI, and driver allocation overhead are not included.
[[nodiscard]] constexpr std::size_t effective_gpu_family_budget(
    std::size_t family_cap,std::size_t shared_cap,
    std::size_t other_family_bytes) noexcept
{
    if (other_family_bytes>=shared_cap) return 0U;
    return std::min(family_cap,shared_cap-other_family_bytes);
}
[[nodiscard]] constexpr bool can_replace_gpu_bytes(
    std::size_t used,std::size_t replaced,std::size_t incoming,
    std::size_t budget) noexcept
{
    if (used<replaced || incoming>budget)return false;
    return used-replaced<=budget-incoming;
}
}
