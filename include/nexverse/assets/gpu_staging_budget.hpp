/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cstddef>
#include <limits>
namespace nexverse::assets
{
// Conservative software staging guard for transactional uploads:
// GPU currently owns old assets AND temporarily needs the entire candidate.
// Driver allocator overhead, UI and terrain remain external to this model.
struct GpuStagingBudget
{
    bool fits=false;
    std::size_t reserve_bytes=0U;
    std::size_t transient_peak_bytes=0U;
    std::size_t maximum_safe_bytes=0U;
};
[[nodiscard]] constexpr GpuStagingBudget gpu_staging_budget(
    std::size_t resident_texture_bytes,
    std::size_t resident_mesh_bytes,
    std::size_t candidate_bytes,
    std::size_t shared_budget_bytes) noexcept
{
    constexpr std::size_t maximum_reserve=16U*1024U*1024U;
    const auto reserve=std::min(maximum_reserve,shared_budget_bytes/32U);
    const auto maximum=shared_budget_bytes-reserve;
    GpuStagingBudget out;
    out.reserve_bytes=reserve;
    out.maximum_safe_bytes=maximum;
    if(resident_texture_bytes>maximum ||
       resident_mesh_bytes>maximum-resident_texture_bytes ||
       candidate_bytes>maximum-resident_texture_bytes-resident_mesh_bytes)
        return out;
    out.fits=true;
    out.transient_peak_bytes=
        resident_texture_bytes+resident_mesh_bytes+candidate_bytes;
    return out;
}
}
