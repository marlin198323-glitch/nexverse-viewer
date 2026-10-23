/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <cmath>
#include <span>
#include <cstdint>
namespace nexverse::protocol
{
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
    const auto& p=reply->position;
    return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z)&&
        std::abs(p.x)<=1000000.0f&&std::abs(p.y)<=1000000.0f&&
        std::abs(p.z)<=1000000.0f;
}
}
