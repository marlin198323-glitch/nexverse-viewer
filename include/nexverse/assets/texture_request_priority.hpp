/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
inline constexpr std::string_view null_texture_asset_id =
    "00000000-0000-0000-0000-000000000000";

// Select particle images first, preserving the priority order established
// by distance to the avatar. Reuse the existing object texture cache,
// suppress duplicate requests and ignore known unavailable assets.
[[nodiscard]] inline std::optional<std::string> next_texture_request(
    const std::vector<std::string>& emitter_textures,
    const std::vector<std::string>& object_textures,
    const std::unordered_set<std::string>& loaded,
    const std::unordered_set<std::string>& failed)
{
    std::unordered_set<std::string> seen;
    const auto find = [&](const std::vector<std::string>& ids)
        -> std::optional<std::string>
    {
        for (const auto& id : ids)
        {
            if (id.empty() || id == null_texture_asset_id ||
                !seen.insert(id).second ||
                loaded.contains(id) || failed.contains(id))
                continue;
            return id;
        }
        return std::nullopt;
    };
    if (const auto particle = find(emitter_textures))
        return particle;
    return find(object_textures);
}
}
