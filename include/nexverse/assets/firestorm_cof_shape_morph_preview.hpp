/* MPL-2.0. Original Firestorm avatar_lad.xml audited shape param IDs. */
#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace nexverse::assets
{
struct FirestormCofMorphPreview
{
    std::vector<std::pair<std::string,float>> head;
    std::vector<std::pair<std::string,float>> eyelashes;
    [[nodiscard]] bool active() const noexcept
    {return !head.empty()||!eyelashes.empty();}
};
// Static, individually checked pairs from ORIGINAL Firestorm Nightly LAD.
// No ID guessing from Low158 unnamed visual-param byte positions.
// A missing param has no morph applied (original bind pose).
// Invalid/out-of-range/NaN parameters refuse the ENTIRE preview,
// never leave a partially updated/stale previous GPU face.
[[nodiscard]] inline std::optional<FirestormCofMorphPreview>
firestorm_cof_shape_morph_preview(
    const std::map<std::int32_t,float>& shape_params)
{
    struct Known {int id;const char* name;float min;float max;};
    constexpr std::array<Known,4U> known{{
        {770,"Elongate_Head",-1.0f,1.0f},
        {186,"Egg_Head",-1.3f,1.0f},
        {686,"Head_Eyes_Big",-2.0f,2.0f},
        {518,"Eyelashes_Long",-0.3f,1.5f}
    }};
    FirestormCofMorphPreview selection;
    for(const auto& param:known)
    {
        const auto it=shape_params.find(param.id);
        if(it==shape_params.end())continue;
        const auto value=it->second;
        if(!std::isfinite(value)||value<param.min||value>param.max)
            return std::nullopt;
        if(std::abs(value)<0.0001f)continue;
        // Firestorm's original eyelashes contain three Head-compatible
        // morphs too; each applied to COPY of its own .llm vertex data.
        if(param.id!=518)
            selection.head.emplace_back(param.name,value);
        selection.eyelashes.emplace_back(param.name,value);
    }
    return selection;
}
}
