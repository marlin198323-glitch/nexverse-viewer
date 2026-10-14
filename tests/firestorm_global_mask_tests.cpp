/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_texlayer_mask_preview.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <cmath>
#include <iostream>
using namespace nexverse::assets;
static bool near(int a,int b,int tolerance=3)
{return std::abs(a-b)<=tolerance;}
int main()
{
    const std::string xml=R"(<linden_avatar wearable_definition_version="22">
      <global_color name="skin_color">
        <param id="111" wearable="skin" value_min="0" value_max="1">
          <param_color operation="add">
            <value color="0,0,0,255"/><value color="255,100,50,255"/>
          </param_color>
        </param>
      </global_color>
      <layer_set body_region="upper_body" width="512" height="512">
        <layer name="skin-global" global_color="skin_color">
          <texture local_texture="upper_bodypaint"/>
          <param id="210" value_min="0" value_max="1">
            <param_alpha tga_file="one.tga"/>
          </param>
          <param id="211" value_min="0" value_max="1">
            <param_alpha tga_file="two.tga" multiply_blend="true"/>
          </param>
        </layer>
      </layer_set></linden_avatar>)";
    auto parsed=parse_firestorm_texlayers(xml);
    if(!parsed.manifest||parsed.manifest->global_colors.size()!=1U||
       parsed.manifest->global_colors.at("skin_color").wearable_type!=1||
       parsed.manifest->sets[1].layers[0].alpha_params.size()!=2U)
        return 1;
    std::map<int,std::map<int,float>> wearables{{2,{{111,0.5f}}}};
    if(!evaluate_firestorm_global_colors(*parsed.manifest,wearables).empty())
        return 2; // hair must never masquerade as skin
    wearables[1].emplace(111,0.5f);
    const auto colors=evaluate_firestorm_global_colors(*parsed.manifest,wearables);
    if(colors.size()!=1U||!colors.contains("skin_color")||
       !near(static_cast<int>(colors.at("skin_color")[0]*255.0f),128))
        return 3;
    const std::map<unsigned,ImageRgba8> images{
        {5U,{1U,1U,{255U,255U,255U,255U}}}};
    const std::map<std::string,ImageRgba8> masks{
        {"one.tga",{1U,1U,{128U,128U,128U,255U}}},
        {"two.tga",{1U,1U,{255U,255U,255U,255U}}}};
    const std::map<int,float> weights{{210,0.5f},{211,0.5f}};
    const auto preview=compose_firestorm_mask_preview(
        *parsed.manifest,1U,images,masks,weights,colors);
    if(!preview.composite.preview_ready()||preview.real_bake()||
       preview.mask_layers_applied!=1U||
       preview.multiple_mask_layers_applied!=1U||
       preview.global_color_layers_applied!=1U||
       preview.composite.applied!=1U||
       !near(preview.composite.image.pixels[0],128)||
       !near(preview.composite.image.pixels[3],32))
    {
        std::cerr<<"Global/2-mask preview counters/alpha mismatch\n";
        return 4;
    }
    if(compose_firestorm_mask_preview(
         *parsed.manifest,1U,images,masks,weights).composite.applied!=0U)
        return 5; // unresolved global color must not fall back to white
    auto missing_weights=weights;
    missing_weights.erase(211);
    if(compose_firestorm_mask_preview(
         *parsed.manifest,1U,images,masks,missing_weights,colors)
       .composite.applied!=0U)
        return 6; // cannot invent a wearable parameter default
    auto missing_masks=masks;
    missing_masks.erase("two.tga");
    const auto missing=compose_firestorm_mask_preview(
        *parsed.manifest,1U,images,missing_masks,weights,colors);
    if(missing.composite.applied!=0U||missing.missing_masks!=1U)
        return 7;
    auto invalid=xml;
    const auto pos=invalid.find("wearable=\"skin\"");
    invalid.replace(pos,15U,"wearable=\"hair\"");
    if(parse_firestorm_texlayers(invalid).manifest)return 8;
    invalid=xml;
    const auto p2=invalid.find("global_color name=\"skin_color\"");
    invalid.replace(p2,30U,"global_color name=\"alien_color\"");
    if(parse_firestorm_texlayers(invalid).manifest)return 9;
    invalid=xml;
    const auto p3=invalid.find("multiply_blend=\"true\"");
    invalid.replace(p3,21U,"domain=\"0.25\"");
    const auto bounded=parse_firestorm_texlayers(invalid);
    if(!bounded.manifest)return 10;
    if(compose_firestorm_mask_preview(
        *bounded.manifest,1U,images,masks,weights,colors).composite.applied!=0U)
        return 11; // nonzero domain requires exact Firestorm treatment
    if(const auto root=find_firestorm_avatar_character_directory())
    {
        auto live=load_firestorm_texlayers(*root);
        if(!live.manifest||live.manifest->global_colors.size()!=3U||
           live.manifest->global_colors.at("hair_color").wearable_type!=2||
           live.manifest->global_colors.at("eye_color").wearable_type!=3||
           live.manifest->global_colors.at("skin_color").color_params.size()<3U)
            return 12;
    }
    std::cout<<"Original Firestorm global color provenance and ordered "
                "two-mask diagnostic preview validated (NO avatar bake)\n";
    return 0;
}
