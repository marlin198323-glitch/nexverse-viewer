/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_wearable_texture_plan.hpp"
#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::assets
{
// Original Firestorm_7.2.4 LLAvatarAppearanceDictionary::Textures names,
// keyed by the canonical ETextureIndex; output baked slots are excluded.
inline constexpr std::array<std::string_view,45U> firestorm_texture_entry_names{
    "head_bodypaint","upper_shirt","lower_pants","eyes_iris","hair_grain",
    "upper_bodypaint","lower_bodypaint","lower_shoes",
    "head-baked","upper-baked","lower-baked","eyes-baked",
    "lower_socks","upper_jacket","lower_jacket","upper_gloves",
    "upper_undershirt","lower_underpants","skirt",
    "skirt-baked","hair-baked",
    "lower_alpha","upper_alpha","head_alpha","eyes_alpha","hair_alpha",
    "head_tattoo","upper_tattoo","lower_tattoo",
    "head_universal_tattoo","upper_universal_tattoo",
    "lower_universal_tattoo","skirt_tattoo","hair_tattoo","eyes_tattoo",
    "leftarm_tattoo","leftleg_tattoo","aux1_tattoo","aux2_tattoo",
    "aux3_tattoo","leftarm-baked","leftleg-baked","aux1-baked",
    "aux2-baked","aux3-baked"
};
inline constexpr std::array<std::string_view,11U> firestorm_layer_regions{
    "head","upper_body","lower_body","eyes","skirt","hair",
    "leftarm","leftleg","aux1","aux2","aux3"
};
struct FirestormTexLayer
{
    std::string name;
    std::optional<unsigned> texture_entry;
    std::string local_name;
    std::size_t ordinal=0U;
    bool is_bump=false;
    bool is_mask=false;
    bool has_parameters=false;
    bool has_external_tga=false;
    bool has_global_color=false;
    bool has_fixed_color=false;
    bool has_visibility_mask=false;
    bool local_alpha_only=false;
    bool multi_texture=false;
    bool safe_preview=false;
};
struct FirestormTexLayerSet
{
    unsigned group=0U,width=0U,height=0U;
    std::vector<FirestormTexLayer> layers;
};
struct FirestormTexLayerManifest
{
    int definition_version=-1;
    std::array<FirestormTexLayerSet,11U> sets{};
    std::size_t layer_count=0U,local_layer_count=0U,
        safe_layer_count=0U,unsupported_layer_count=0U;
    [[nodiscard]] bool valid() const noexcept
    {return definition_version>0&&layer_count>0U;}
};
struct FirestormTexLayerResult
{
    std::optional<FirestormTexLayerManifest> manifest;
    std::string error;
};
[[nodiscard]] FirestormTexLayerResult parse_firestorm_texlayers(
    std::string_view xml);
[[nodiscard]] FirestormTexLayerResult load_firestorm_texlayers(
    const std::filesystem::path& character_directory);
// A manifest defines source ordering, not a verified Firestorm bake. It
// must NEVER be sent via AgentSetAppearance / baked-asset upload.
}
