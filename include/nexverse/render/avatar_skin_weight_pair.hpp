/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace nexverse::render
{
// Firestorm .llm packs adjacent joint palette indices in a single float:
// floor(weight) -> first joint, fract(weight) -> interpolation toward next.
// A final palette entry MUST be mapped to the last valid pair at blend=1,
// not incorrectly blend the final bone against GL's identity remainder.
struct AvatarSkinWeightPair
{
    std::size_t first=0U;
    std::size_t second=0U;
    float blend=0.0f;
    bool valid=false;
};

[[nodiscard]] inline AvatarSkinWeightPair resolve_avatar_skin_weight(
    float weight,std::size_t palette_size) noexcept
{
    if(palette_size<2U||palette_size>32U||
       !std::isfinite(weight)||weight<0.0f||
       weight>=static_cast<float>(palette_size))
        return {};
    const auto index=static_cast<std::size_t>(std::floor(weight));
    const auto first=std::min(index,palette_size-2U);
    const float blend=std::clamp(weight-static_cast<float>(first),0.0f,1.0f);
    return {first,first+1U,blend,true};
}
}
