/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_wearable_texture_plan.hpp"
#include "nexverse/assets/texture_mip.hpp"
#include "jpeg2000_fixture.hpp"
#include <iostream>
using namespace nexverse::assets;
constexpr auto A="11111111-1111-1111-1111-111111111111";
constexpr auto B="22222222-2222-2222-2222-222222222222";
constexpr auto C="33333333-3333-3333-3333-333333333333";
constexpr auto D="44444444-4444-4444-4444-444444444444";
int main()
{
    FirestormWearableAsset skin;skin.type=1;
    skin.textures.insert({0U,A}); // HEAD
    skin.textures.insert({5U,B}); // UPPER
    skin.textures.insert({6U,B}); // LOWER same tex UUID, dedup HTTP
    skin.textures.insert({8U,C}); // baked output is never local preview
    skin.textures.insert({20U,C}); // baked output
    skin.textures.insert({45U,C}); // beyond original TEX_NUM_INDICES
    skin.textures.insert({3U,D}); // Eyes wearable only, invalid on Skin
    FirestormWearableAsset shirt;shirt.type=4;
    shirt.textures.insert({1U,D}); // UPPER
    shirt.textures.insert({8U,A}); // baked output
    std::map<std::string,FirestormWearableAsset,std::less<>> set;
    set.emplace("asset-skin",skin);
    set.emplace("asset-shirt",shirt);
    auto plan=plan_wearable_texture_preview(set,12U);
    if(!plan.ready()||plan.unique_assets!=2U||
       plan.local_refs!=4U||plan.duplicates!=1U||
       plan.invalid_refs!=2U||plan.baked_output_refs!=3U||
       plan.candidates.size()!=3U||plan.truncated!=0U||
       plan.local_group_counts[0U]!=1U||
       plan.local_group_counts[1U]!=2U||
       plan.local_group_counts[2U]!=1U)return 1;
    for(const auto& item:plan.candidates)
    {
        if(item.uuid==C||item.wearable_type<0||
           item.bake_group>=11U)return 2;
    }
    plan=plan_wearable_texture_preview(set,1U);
    if(plan.candidates.size()!=1U||plan.truncated!=2U)return 3;
    plan=plan_wearable_texture_preview(set,0U);
    if(plan.ready()||plan.truncated!=3U)return 4;
    const std::set<std::string,std::less<>> expected{A,B,D};
    WearableTexturePreviewResult result;
    result.region_epoch=13U;result.cof_version=71U;
    result.cof_id="cof-original";result.asset_id=A;
    result.wearable_type=1;result.bake_group=0U;
    if(!wearable_preview_matches(result,13U,"cof-original",71U,expected)||
       wearable_preview_matches(result,14U,"cof-original",71U,expected)||
       wearable_preview_matches(result,13U,"other",71U,expected)||
       wearable_preview_matches(result,13U,"cof-original",72U,expected))
        return 5;
    result.asset_id=C;
    if(wearable_preview_matches(result,13U,"cof-original",71U,expected))
        return 6;
    result.asset_id=A;result.bake_group=11U;
    if(wearable_preview_matches(result,13U,"cof-original",71U,expected))
        return 7;
    result.bake_group=0U;result.wearable_type=17;
    if(wearable_preview_matches(result,13U,"cof-original",71U,expected))
        return 8;
    // Actual JPEG2000 J2C 16x16 -> RGBA; a swatch is NOT a bake.
    const std::string j2c{
        reinterpret_cast<const char*>(j2c_fixture),sizeof(j2c_fixture)};
    const auto decoded=decode_jpeg2000_texture(A,j2c);
    if(!decoded.success||!decoded.image.valid()||
       decoded.image.width!=16U||decoded.image.height!=16U)
        return 9;
    const auto mip=texture_mip_image(decoded.image,1U);
    if(!mip||!mip->valid()||mip->width!=8U||mip->height!=8U)
        return 10;
    const auto bad=decode_jpeg2000_texture(A,"not-jpeg2000");
    if(bad.success||bad.image.valid())return 11;
    std::cout<<"Original Firestorm local TE wearable owner, 11 bake groups, bounded deduplicated original GetTexture swatches, epoch+COF guards, real J2C -> RGBA -> mip OK\n";
    return 0;
}
