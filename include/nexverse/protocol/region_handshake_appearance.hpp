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

// A packet was accepted for reliable transmission; only an ACK of that
// particular pending wire sequence may confirm receipt by the simulator.
// This proves neither AvatarAppearance delivery nor server bake pixels.
struct SelfAppearanceHandshakeReceipt
{
    std::uint32_t sequence=0U;
    bool sent=false;
    bool acknowledged=false;
    void reset() noexcept
    {
        sequence=0U;sent=false;acknowledged=false;
    }
    void note_sent(std::uint32_t outgoing_sequence) noexcept
    {
        reset();
        if(outgoing_sequence!=0U)
        {
            sequence=outgoing_sequence;
            sent=true;
        }
    }
    [[nodiscard]] bool note_ack(std::uint32_t ack_sequence,
                                bool pending_tracker_matched) noexcept
    {
        if(!sent||acknowledged||!pending_tracker_matched||
           ack_sequence!=sequence)return false;
        acknowledged=true;
        return true;
    }
};
}
