/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_texlayer_mask_preview.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <cmath>
#include <iostream>
using namespace nexverse::assets;
static bool near(float a,float b,float tol=0.02f)
{return std::abs(a-b)<=tol;}
int main()
{
    const std::string xml=R"(<linden_avatar wearable_definition_version="22">
    <layer_set body_region="upper_body" width="512" height="512">
    <layer name="red-add">
      <texture local_texture="upper_shirt"/>
      <param id="210" value_min="0" value_max="1">
        <param_color operation="add">
          <value color="0,0,0,255"/>
          <value color="255,0,0,255"/>
        </param_color>
      </param>
    </layer>
    <layer name="multiply" fixed_color="128,200,255,128">
      <texture local_texture="upper_jacket"/>
      <param id="211" value_min="0" value_max="1">
        <param_color operation="multiply">
          <value color="255,255,255,255"/>
          <value color="128,255,255,255"/>
        </param_color>
      </param>
    </layer>
    <layer name="blend" fixed_color="0,255,0,255">
      <texture local_texture="upper_gloves"/>
      <param id="212" value_min="0" value_max="1">
        <param_color operation="blend">
          <value color="255,0,0,255"/>
        </param_color>
      </param>
    </layer>
    <layer name="alpha-and-color">
      <texture local_texture="upper_undershirt"/>
      <param id="213" value_min="0" value_max="1">
        <param_color operation="add">
          <value color="255,0,0,255"/>
          <value color="0,0,255,255"/>
        </param_color>
      </param>
      <param id="214" value_min="0" value_max="1">
        <param_alpha tga_file="safe_alpha.tga"/>
      </param>
    </layer>
    <layer name="global-skin" global_color="skin_color">
      <texture local_texture="upper_bodypaint"/>
    </layer>
    </layer_set></linden_avatar>)";
    auto parsed=parse_firestorm_texlayers(xml);
    if(!parsed.manifest||parsed.manifest->color_param_count!=4U||
       parsed.manifest->color_layer_count!=5U)
        return 1;
    const auto& layers=parsed.manifest->sets[1U].layers;
    if(layers.size()!=5U||!layers[0].color_params_only||
       !layers[3].known_color_and_alpha_only||
       layers[3].alpha_params.size()!=1U||
       layers[3].color_params.size()!=1U||
       layers[0].safe_preview)
        return 2;
    std::map<int,float> weights{{210,0.5f},{211,0.5f},
                                {212,0.25f},{213,0.5f},{214,0.5f}};
    auto red=evaluate_firestorm_local_layer_tint(layers[0],weights);
    auto mul=evaluate_firestorm_local_layer_tint(layers[1],weights);
    auto blend=evaluate_firestorm_local_layer_tint(layers[2],weights);
    if(!red||!near((*red)[0],0.5f)||(*red)[1]>0.01f||
       !mul||!near((*mul)[0],128.0f/255.0f*
                        (191.5f/255.0f))||
       !near((*mul)[3],128.0f/255.0f)||
       !blend||!near((*blend)[0],0.25f)||
       !near((*blend)[1],0.75f)||(*blend)[2]>0.01f)
        return 3;
    if(evaluate_firestorm_local_layer_tint(layers[4],weights))
        return 4; // never fake skin_color
    weights.erase(210);
    if(evaluate_firestorm_local_layer_tint(layers[0],weights))
        return 5; // missing owner-original param must fail
    weights.insert_or_assign(210,0.5f);
    std::map<unsigned,ImageRgba8> input{
        {1U,{1U,1U,{255U,255U,255U,255U}}},
        {13U,{1U,1U,{255U,255U,255U,255U}}},
        {15U,{1U,1U,{255U,255U,255U,255U}}},
        {16U,{1U,1U,{255U,255U,255U,255U}}},
        {5U,{1U,1U,{255U,255U,255U,255U}}}
    };
    std::map<std::string,ImageRgba8> masks{
        {"safe_alpha.tga",{1U,1U,{128U,128U,128U,255U}}}};
    auto local=compose_firestorm_mask_preview(
        *parsed.manifest,1U,input,masks,weights);
    if(!local.composite.preview_ready()||local.real_bake()||
       local.color_layers_applied!=4U||
       local.mask_layers_applied!=1U||
       local.unresolved_color_layers!=1U||
       local.composite.unsupported!=1U||
       !local.composite.image.valid())
        return 6;
    auto bad=xml;
    auto at=bad.find("fixed_color=\"128,200,255,128\"");
    bad.replace(at,29U,"fixed_color=\"300,0,0,255\"");
    if(parse_firestorm_texlayers(bad).manifest)return 7;
    bad=xml;at=bad.find("operation=\"blend\"");
    bad.replace(at,17U,"operation=\"invalid\"");
    if(parse_firestorm_texlayers(bad).manifest)return 8;
    bad=xml;at=bad.find("value_min=\"0\" value_max=\"1\"");
    bad.replace(at,27U,"value_min=\"0\" value_max=\"NaN\"");
    if(parse_firestorm_texlayers(bad).manifest)return 9;
    if(const auto root=find_firestorm_avatar_character_directory())
    {
        const auto real=load_firestorm_texlayers(*root);
        if(!real.manifest||
           real.manifest->color_param_count<50U||
           real.manifest->color_layer_count<10U)
            return 10;
        bool has_25_color_palette=false;
        for(const auto& set:real.manifest->sets)
            for(const auto& layer:set.layers)
                for(const auto& param:layer.color_params)
                    if(param.palette.size()==25U)
                        has_25_color_palette=true;
        if(!has_25_color_palette)return 11;
        std::cout<<"Real Firestorm palette params: "<<
            real.manifest->color_param_count
            <<", color layers "<<real.manifest->color_layer_count<<"\n";
    }
    std::cout<<"Firestorm real param_color add/multiply/blend, fixed RGBA and masked tint previews, unknown global/weights fail-closed OK\n";
    return 0;
}
