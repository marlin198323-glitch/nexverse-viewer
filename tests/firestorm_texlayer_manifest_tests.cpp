/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_texlayer_composite.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <algorithm>
#include <iostream>
using namespace nexverse::assets;
int main()
{
    const std::string xml=R"(<?xml version="1.0"?>
<linden_avatar wearable_definition_version="22" version="2.0">
 <layer_set body_region="upper_body" width="1024" height="1024">
   <layer name="shirt"><texture local_texture="upper_shirt"/></layer>
   <layer name="masked" visibility_mask="true">
      <texture local_texture="upper_alpha"/>
      <param id="123"><param_alpha domain="0.1"/></param>
   </layer>
   <layer name="jacket"><texture local_texture="upper_jacket"/></layer>
   <layer name="bump" render_pass="bump">
     <texture local_texture="upper_gloves"/>
   </layer>
 </layer_set>
 <layer_set body_region="head" width="2048" height="2048">
  <layer name="head"><texture local_texture="head_bodypaint"/></layer>
 </layer_set>
</linden_avatar>
)";
    const auto parsed=parse_firestorm_texlayers(xml);
    if(!parsed.manifest||!parsed.manifest->valid()||
       parsed.manifest->definition_version!=22||
       parsed.manifest->layer_count!=5U||
       parsed.manifest->local_layer_count!=5U||
       parsed.manifest->safe_layer_count!=3U||
       parsed.manifest->unsupported_layer_count!=2U)
        return 1;
    const auto& upper=parsed.manifest->sets[1U];
    if(upper.layers.size()!=4U||
       upper.layers[0].texture_entry!=1U||
       upper.layers[1].safe_preview||
       upper.layers[2].texture_entry!=13U||
       upper.layers[2].ordinal!=2U||
       upper.layers[3].safe_preview||
       upper.width!=1024U)return 2;
    std::map<unsigned,ImageRgba8> sources;
    sources.insert_or_assign(1U,ImageRgba8{1U,1U,{255U,0U,0U,255U}});
    sources.insert_or_assign(13U,ImageRgba8{1U,1U,{0U,0U,255U,128U}});
    auto preview=compose_firestorm_partial_texlayers(
        *parsed.manifest,1U,sources);
    if(!preview.preview_ready()||preview.is_verified_bake()||
       preview.applied!=2U||preview.unsupported!=2U||
       preview.missing!=0U||preview.image.pixels.size()!=4U||
       preview.image.pixels[0U]<125U||
       preview.image.pixels[0U]>129U||
       preview.image.pixels[2U]<126U||
       preview.image.pixels[2U]>130U||
       preview.image.pixels[3U]!=255U)
        return 3;
    sources.erase(13U);
    preview=compose_firestorm_partial_texlayers(*parsed.manifest,1U,sources);
    if(!preview.preview_ready()||preview.applied!=1U||
       preview.missing!=1U)return 4;
    sources[13U]=ImageRgba8{2U,1U,{0U,0U,255U,128U,0U,0U,255U,128U}};
    preview=compose_firestorm_partial_texlayers(*parsed.manifest,1U,sources);
    if(preview.preview_ready()||preview.error.empty())return 5;
    if(compose_firestorm_partial_texlayers(*parsed.manifest,12U,sources)
       .preview_ready())return 6;
    if(parse_firestorm_texlayers("bad").manifest)return 7;
    auto bad=xml;auto index=bad.find("upper_jacket");
    bad.replace(index,12U,"bad_unknown1");
    if(parse_firestorm_texlayers(bad).manifest)return 8;
    bad=xml;index=bad.find("upper_jacket");
    bad.replace(index,12U,"lower_pants");
    if(parse_firestorm_texlayers(bad).manifest)return 9;
    bad=xml;index=bad.find("width=\"1024\"");
    bad.replace(index,12U,"width=\"99999\"");
    if(parse_firestorm_texlayers(bad).manifest)return 10;
    bad=xml;index=bad.find("upper_jacket");
    bad.replace(index,12U,"head-baked  ");
    if(parse_firestorm_texlayers(bad).manifest)return 11;
    bad=xml;index=bad.find("body_region=\"head\"");
    bad.replace(index,18U,"body_region=\"upper_body\"");
    if(parse_firestorm_texlayers(bad).manifest)return 12;
    if(const auto root=find_firestorm_avatar_character_directory())
    {
        auto live=load_firestorm_texlayers(*root);
        if(!live.manifest)
        {
            std::cerr<<"Original Firestorm XML parse: "<<live.error<<"\n";
            return 13;
        }
        if(live.manifest->sets[0U].layers.empty()||
           live.manifest->sets[1U].layers.empty()||
           live.manifest->sets[2U].layers.empty()||
           live.manifest->layer_count<40U)
            return 14;
        std::cout<<"Original Firestorm avatar_lad.xml parsed "
                 <<live.manifest->layer_count<<" ordered layers, "
                 <<live.manifest->safe_layer_count<<" simple inputs\n";
    }
    std::cout<<"Synthetic Original TexLayer ordered/compositing vs unsafe alpha/bump/missing and malformed manifest OK\n";
    return 0;
}
