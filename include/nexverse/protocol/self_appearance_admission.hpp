/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"
#include <string_view>
#include <span>
#include <cstdint>

namespace nexverse::protocol
{
// Strictly parse complete Firestorm texture entries before trusting a
// non-trial self appearance as a skin. No baked texture needs to be
// present to pass: an authentic but unbaked appearance remains possible.
// Implemented in the protocol library to reuse the actual TE decoder.
[[nodiscard]] bool valid_self_appearance_texture_entry(
    std::span<const std::uint8_t> texture_entry);

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
