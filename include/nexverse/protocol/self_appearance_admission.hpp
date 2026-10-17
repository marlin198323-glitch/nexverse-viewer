/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"
#include <string_view>

namespace nexverse::protocol
{
// The simulator can send preview/trial and partial appearance snapshots.
// Those must NOT replace an already-authoritative self avatar and
// thereby blank real baked skin or weighted body parameters.
enum class SelfAppearanceAdmission
{
    accepted,
    region_not_ready,
    not_self,
    trial_preview,
    texture_entry_missing,
    visual_params_missing
};
[[nodiscard]] inline SelfAppearanceAdmission assess_self_appearance(
    const AvatarAppearanceMessage& candidate,
    std::string_view current_agent,
    bool region_connected) noexcept
{
    if(!region_connected)return SelfAppearanceAdmission::region_not_ready;
    if(current_agent.empty()||candidate.avatar_id!=current_agent)
        return SelfAppearanceAdmission::not_self;
    if(candidate.trial)return SelfAppearanceAdmission::trial_preview;
    if(candidate.texture_entry.empty())
        return SelfAppearanceAdmission::texture_entry_missing;
    if(candidate.visual_params.empty())
        return SelfAppearanceAdmission::visual_params_missing;
    return SelfAppearanceAdmission::accepted;
}
[[nodiscard]] inline bool appearance_is_authoritative(
    SelfAppearanceAdmission verdict) noexcept
{
    return verdict==SelfAppearanceAdmission::accepted;
}
}
