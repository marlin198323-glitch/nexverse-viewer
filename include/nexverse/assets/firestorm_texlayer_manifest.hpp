/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_wearable_texture_plan.hpp"
#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
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
struct FirestormTexLayerAlphaParam
{
    int id=-1;
    std::string wearable;
    std::string mask_file;
    float minimum=0.0f,maximum=1.0f,default_value=0.0f;
    float domain=0.0f;
    bool multiply_blend=false,skip_if_zero=false;
    bool explicit_default=false;
};
enum class FirestormColorOperation : std::uint8_t
{
    add,multiply,blend
};
struct FirestormTexLayerColorParam
{
    int id=-1;
    float minimum=0.0f,maximum=1.0f;
    FirestormColorOperation operation=FirestormColorOperation::add;
    std::vector<std::array<std::uint8_t,4>> palette;
};
struct FirestormTexGlobalColor
{
    std::string name;
    int wearable_type=-1; // Firestorm WT_SKIN=1, WT_HAIR=2, WT_EYES=3
    std::vector<FirestormTexLayerColorParam> color_params;
};
struct FirestormTexLayer
{
    std::string name;
    std::optional<unsigned> texture_entry;
    std::string local_name;
    std::vector<FirestormTexLayerAlphaParam> alpha_params;
    std::vector<FirestormTexLayerColorParam> color_params;
    std::optional<std::array<std::uint8_t,4>> fixed_rgba;
    std::string global_color_name;
    bool alpha_params_only=false;
    bool color_params_only=false;
    bool known_color_and_alpha_only=false;
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
    std::map<std::string,FirestormTexGlobalColor> global_colors;
    std::size_t layer_count=0U,local_layer_count=0U,
        safe_layer_count=0U,unsupported_layer_count=0U,
        color_layer_count=0U,color_param_count=0U;
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
