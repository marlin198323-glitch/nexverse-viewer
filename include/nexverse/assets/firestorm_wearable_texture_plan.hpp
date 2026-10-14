/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_wearable_asset.hpp"
#include "nexverse/assets/texture_loader.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::assets
{
// LLAvatarAppearanceDictionary::Textures::Textures in Firestorm_7.2.4:
 // each local texture index belongs to one particular wearable type.
inline constexpr std::array<int,45U> firestorm_local_texture_owner{
    1,4,5,3,2,1,1,6,-1,-1,-1,-1,7,8,8,9,10,11,12,-1,
    -1,13,13,13,13,13,14,14,14,16,16,16,16,16,16,16,16,
    16,16,16,-1,-1,-1,-1,-1
};
static_assert(firestorm_local_texture_owner.size()==
    firestorm_local_bake_groups.size());

struct WearableTextureReference
{
    std::string uuid;
    int wearable_type=-1;
    unsigned texture_entry=0U;
    unsigned bake_group=0U;
};
struct WearableTexturePlan
{
    std::vector<WearableTextureReference> candidates;
    std::array<std::size_t,11> local_group_counts{};
    std::size_t unique_assets=0U,local_refs=0U,duplicates=0U,
        baked_output_refs=0U,invalid_refs=0U,truncated=0U;
    [[nodiscard]] bool ready() const noexcept
    {return !candidates.empty();}
};
// A bounded list of actual input texture UUIDs for authenticated read-only
// GetTexture. Not a Firestorm TexLayer compositing order! We deliberately
// select a representative texture per wearable for a UI swatch, rather
// than claim a pixel-perfect appearance from unordered inventory entries.
[[nodiscard]] inline WearableTexturePlan plan_wearable_texture_preview(
    const std::map<std::string,FirestormWearableAsset,std::less<>>& wearables,
    std::size_t limit=12U)
{
    WearableTexturePlan plan;
    if(limit>24U)limit=24U;
    std::set<std::string,std::less<>> unique;
    for(const auto& [asset_id,wearable]:wearables)
    {
        (void)asset_id;
        if(wearable.type<0||wearable.type>=17)
        {
            ++plan.invalid_refs;
            continue;
        }
        ++plan.unique_assets;
        for(const auto& [index,uuid]:wearable.textures)
        {
            if(index>=firestorm_local_bake_groups.size())
            {
                ++plan.invalid_refs;
                continue;
            }
            const int group=firestorm_local_bake_groups[index];
            if(group<0)
            {
                ++plan.baked_output_refs;
                continue;
            }
            if(firestorm_local_texture_owner[index]!=wearable.type)
            {
                ++plan.invalid_refs;
                continue;
            }
            const auto parsed=nexverse::protocol::uuid_to_bytes(uuid);
            if(!parsed||std::all_of(parsed->begin(),parsed->end(),
                                   [](std::uint8_t b){return b==0U;}))
            {
                ++plan.invalid_refs;
                continue;
            }
            ++plan.local_refs;
            ++plan.local_group_counts[static_cast<std::size_t>(group)];
            if(!unique.insert(uuid).second)
            {
                ++plan.duplicates;
                continue;
            }
            if(plan.candidates.size()>=limit)
            {
                ++plan.truncated;
                continue;
            }
            plan.candidates.push_back({
                uuid,wearable.type,index,static_cast<unsigned>(group)});
        }
    }
    return plan;
}
struct WearableTexturePreviewResult
{
    std::uint64_t region_epoch=0U;
    std::uint64_t cof_version=0U;
    std::string cof_id,asset_id;
    int wearable_type=-1;
    unsigned bake_group=0U;
    ImageRgba8 image;
    std::string error;
};
// Never trust a delayed network response after COF/region/login movement,
// even if the immutable texture UUID also appears in a different outfit.
[[nodiscard]] inline bool wearable_preview_matches(
    const WearableTexturePreviewResult& reply,
    std::uint64_t epoch,std::string_view cof,std::uint64_t version,
    const std::set<std::string,std::less<>>& expected)
{
    return reply.region_epoch!=0U&&reply.region_epoch==epoch&&
           reply.cof_version!=0U&&reply.cof_version==version&&
           reply.cof_id==cof&&expected.contains(reply.asset_id)&&
           reply.wearable_type>=0&&reply.wearable_type<17&&
           reply.bake_group<11U;
}
} // namespace nexverse::assets
