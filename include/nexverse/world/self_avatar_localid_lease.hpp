/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
namespace nexverse::world
{
// High9 ImprovedTerse has ONLY a simulator-local numeric ID, no avatar UUID.
// A region handle may be reused by a different simulator endpoint; never
// classify a LocalID as SELF unless the current WorldModel generation is
// the very one in which authenticated Full/Compressed ObjectUpdate bound it.
[[nodiscard]] constexpr std::uint32_t current_self_avatar_local_id(
    std::uint32_t local_id,std::uint64_t bound_region,
    std::uint32_t bound_generation,std::uint64_t current_region,
    std::uint32_t current_generation) noexcept
{
    // Initial login legitimately uses generation 0; matching the live
    // generation is what matters, NOT a synthetic nonzero requirement.
    return local_id!=0U&&bound_region!=0U&&
        bound_region==current_region&&bound_generation==current_generation
        ?local_id:0U;
}
}
