/* MPL-2.0: GPU mesh count target. Byte and shared VRAM limits remain independent. */
#pragma once
#include <algorithm>
#include <cstddef>

namespace nexverse::assets
{
[[nodiscard]] constexpr std::size_t mesh_resident_count_target(
    std::size_t visible_samples, bool detailed_quality) noexcept
{
    if (!detailed_quality) return 256U;
    // A scene with 292 mesh samples must not be truncated at 256 solely
    // because of the old fixed count cap. Never exceed the VRAM byte gates.
    return std::clamp(visible_samples, std::size_t{256}, std::size_t{384});
}
}
