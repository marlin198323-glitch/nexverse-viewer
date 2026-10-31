/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
namespace nexverse::assets
{
// Preserve the first failures, then rate-limit repeated per-asset warnings
// to every 64th event. The true counters continue updating on ALL events;
// operators use streaming metrics for totals, not the warning count.
[[nodiscard]] constexpr bool stream_pressure_warning_due(
    std::size_t total) noexcept
{
    return total>0U&&(total<=3U||total%64U==0U);
}
}
