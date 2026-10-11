/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// A live scene diagnostic, not a guarantee of draw-call visibility.
struct AssetSceneCoverage
{
    std::size_t requested = 0U;
    std::size_t resident = 0U;
    std::size_t pending = 0U;
    [[nodiscard]] unsigned percentage() const noexcept
    {
        return requested == 0U
            ? 100U
            : static_cast<unsigned>((100U*resident)/requested);
    }
};
[[nodiscard]] inline AssetSceneCoverage asset_scene_coverage(
    const std::vector<std::string>& priority,
    const std::unordered_set<std::string>& gpu_resident)
{
    AssetSceneCoverage coverage;
    std::unordered_set<std::string> seen;
    for (const auto& id:priority)
    {
        if (id.empty() || !seen.insert(id).second) continue;
        ++coverage.requested;
        if (gpu_resident.contains(id)) ++coverage.resident;
    }
    coverage.pending=coverage.requested-coverage.resident;
    return coverage;
}
}
