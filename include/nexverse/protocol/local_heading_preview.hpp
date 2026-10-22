/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <chrono>
#include <cmath>
#include <optional>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
// The user's requested yaw can animate the LOCAL camera/body,
// but must never overwrite WorldModel's simulator-authoritative
// AgentState rotation (the server may reject/lag a control packet).
struct LocalHeadingPreview
{
    std::string agent_id;
    std::string session_id;
    Quaternion rotation{};
    std::chrono::steady_clock::time_point queued_at{};
};
[[nodiscard]] inline std::optional<Quaternion> active_local_heading_preview(
    const std::optional<LocalHeadingPreview>& preview,
    std::string_view agent_id,std::string_view session_id,
    bool simulator_connected,
    std::chrono::steady_clock::time_point now) noexcept
{
    if(!simulator_connected||!preview||
       agent_id.empty()||session_id.empty()||
       preview->agent_id!=agent_id||
       preview->session_id!=session_id)
        return std::nullopt;
    const auto age=now-preview->queued_at;
    if(age<std::chrono::steady_clock::duration::zero()||
       age>std::chrono::milliseconds{350})
        return std::nullopt;
    const auto& q=preview->rotation;
    const double n=double(q.x)*q.x+double(q.y)*q.y+
                   double(q.z)*q.z+double(q.w)*q.w;
    if(!std::isfinite(n)||n<0.25||n>4.0)return std::nullopt;
    return q;
}
}
