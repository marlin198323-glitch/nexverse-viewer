/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>

namespace nexverse::protocol
{
// Firestorm Low158 allows multiple AppearanceData blocks. A trailing
// COF=0 is an unknown revision, NOT proof that the preceding nonzero
// version vanished. The WorldModel rollback gate and both F9 views must
// select the identical most-recent meaningful row.
[[nodiscard]] inline std::optional<AvatarAppearanceData>
latest_nonzero_avatar_cof(const AvatarAppearanceMessage& packet)
{
    const auto revision=std::find_if(
        packet.appearance_data.rbegin(),
        packet.appearance_data.rend(),
        [](const auto& entry){return entry.cof_version!=0U;});
    if(revision==packet.appearance_data.rend())return std::nullopt;
    return *revision;
}
[[nodiscard]] inline std::uint32_t
avatar_cof_version_for_display(const AvatarAppearanceMessage& packet)
{
    const auto revision=latest_nonzero_avatar_cof(packet);
    return revision?revision->cof_version:0U;
}
}
