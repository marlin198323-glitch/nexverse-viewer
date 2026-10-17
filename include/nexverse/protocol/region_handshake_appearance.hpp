/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>

namespace nexverse::protocol
{
// From Second Life viewer main: llviewerregion.h
// REGION_HANDSHAKE_SUPPORTS_SELF_APPEARANCE = 1U << 2.
// This is VIEWER->SIM handshake negotiation, not proof of a server
// AvatarAppearance reply. Cache probe bits 0/1 are independent and
// MUST remain clear until a real LL object-cache client is implemented.
inline constexpr std::uint32_t region_handshake_supports_self_appearance =
    1U<<2U;
inline constexpr std::uint32_t region_handshake_object_cache_probes =
    1U<<0U;
inline constexpr std::uint32_t region_handshake_empty_object_cache =
    1U<<1U;
inline constexpr std::uint32_t ogl_region_handshake_flags =
    region_handshake_supports_self_appearance;

[[nodiscard]] constexpr bool advertises_self_appearance(
    std::uint32_t flags) noexcept
{
    return (flags&region_handshake_supports_self_appearance)!=0U;
}
static_assert((ogl_region_handshake_flags&
    (region_handshake_object_cache_probes|
     region_handshake_empty_object_cache))==0U);
}
