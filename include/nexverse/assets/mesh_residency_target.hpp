/* MPL-2.0: GPU mesh count target. Byte and shared VRAM limits remain independent. */
#pragma once
#include <algorithm>
#include <cstddef>

namespace nexverse::assets
{
// One shared capacity contract for planner AND renderer. Quality mode
// and conservative GPU byte budgets determine actual residency; the
// backend must not silently reject meshes 257..384 before byte guards.
// This is vendor-independent and bounded on low-memory GPUs.
inline constexpr std::size_t max_mesh_resident_assets=384U;
inline constexpr std::size_t economy_mesh_resident_assets=256U;
[[nodiscard]] constexpr std::size_t mesh_resident_count_target(
    std::size_t visible_samples, bool detailed_quality) noexcept
{
    if(!detailed_quality)return economy_mesh_resident_assets;
    // The scene may demand more than the economy limit, but actual
    // allocations remain constrained by existing mesh/shared byte budgets
    // and the GPU driver's allocation result.
    return std::clamp(visible_samples,economy_mesh_resident_assets,
                      max_mesh_resident_assets);
}
}
