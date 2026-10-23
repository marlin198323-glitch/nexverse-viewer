/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <cmath>
#include <span>
#include <cstdint>
namespace nexverse::protocol
{
// A separate diagnostic distinguishes a valid-looking foreign-region
// Low15 from corrupt wire or another user's login. Only log counts,
// never raw AgentID/SessionID or simulator endpoint.
[[nodiscard]] inline bool movement_complete_wrong_login_region(
    std::span<const std::uint8_t> payload,
    const LoginSession& session) noexcept
{
    if(session.region_x==0U&&session.region_y==0U)return false;
    const auto reply=decode_agent_movement_complete(payload);
    if(!reply||reply->agent_id!=session.agent_id||
       reply->session_id!=session.session_id||reply->region_handle==0U)
        return false;
    const auto expected=
        (static_cast<std::uint64_t>(session.region_x)<<32U)|
        static_cast<std::uint64_t>(session.region_y);
    return reply->region_handle!=expected;
}

// Region/circuit activation cannot be based on only the packet ID.
// The actual Low message must be correctly decoded, match the
// authenticated login, and have plausible simulator coordinates.
// A LookAt lacking horizontal heading is allowed (position-only),
// never fabricate a body rotation here.
[[nodiscard]] inline bool valid_circuit_movement_complete(
    std::span<const std::uint8_t> payload,
    const LoginSession& session,bool movement_request_sent) noexcept
{
    if(!movement_request_sent)return false;
    const auto reply=decode_agent_movement_complete(payload);
    if(!reply||reply->agent_id!=session.agent_id||
       reply->session_id!=session.session_id||
       reply->region_handle==0U)return false;
    // The XML-RPC login response supplies the exact RegionHandle in its
    // region_x/region_y coordinates. WorldModel checks that same handle.
    // Merely requiring NONZERO here let a foreign-region Low15 mark the
    // UDP circuit active while WorldModel correctly rejected the avatar
    // self pose; the circuit then sent Low381 and tried High4 too early.
    // Some legacy logins omit BOTH coordinates: preserve those sessions
    // by deferring region authority to the authenticated WorldModel.
    if(session.region_x!=0U||session.region_y!=0U)
    {
        const auto expected=
            (static_cast<std::uint64_t>(session.region_x)<<32U)|
            static_cast<std::uint64_t>(session.region_y);
        if(reply->region_handle!=expected)return false;
    }
    const auto& p=reply->position;
    return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z)&&
        std::abs(p.x)<=1000000.0f&&std::abs(p.y)<=1000000.0f&&
        std::abs(p.z)<=1000000.0f;
}
}
