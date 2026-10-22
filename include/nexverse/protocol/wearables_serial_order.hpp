/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
namespace nexverse::protocol
{
// OpenSim Low382 AgentWearablesUpdate SerialNum is UInt32.
// RFC1982-style bounded modulo ordering; never compare cyclic values
// with <=. Exact repeats and ambiguous half-cycle jumps are stale.
// Called only after authenticated agent+session+region admission.
[[nodiscard]] constexpr bool wearables_serial_newer(
    std::uint32_t incoming,std::uint32_t last) noexcept
{
    const auto delta=static_cast<std::uint32_t>(incoming-last);
    return delta!=0U&&delta<0x80000000U;
}
}
