/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
namespace nexverse::render
{
// Simulator Low158 hover parameters are NOT world positions. IEEE NaN
// propagates through std::clamp, so untrusted hover could poison a valid
// AvatarProxy GL transform despite valid AgentPosition/ObjectUpdate.
struct AvatarHoverOffset
{
    float meters=0.0f;
    bool invalid=false;
};
[[nodiscard]] inline AvatarHoverOffset safe_avatar_hover(float z) noexcept
{
    if(!std::isfinite(z))return {0.0f,true};
    return {std::clamp(z,-2.0f,2.0f),false};
}
// Whitelisted status text; never log raw avatar UUID, invalid float or TE.
[[nodiscard]] inline std::string avatar_hover_status(
    bool self_invalid,std::uint32_t remote_invalid)
{
    if(!self_invalid&&remote_invalid==0U)return {};
    return std::string{"Hover korrigiert: Self "}+
        (self_invalid?"1":"0")+" / Fremde "+
        std::to_string(remote_invalid);
}
}
