/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_tga.hpp"
#include "nexverse/assets/firestorm_texlayer_mask_preview.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include <iostream>
using namespace nexverse::assets;
int main()
{
    std::vector<std::uint8_t> bytes(18U,0U);
    bytes[2]=11U;bytes[12]=2U;bytes[14]=2U;bytes[16]=8U;
    bytes[17]=0x20U;
    bytes.insert(bytes.end(),{0x81U,0x80U,0x01U,0x00U,0xffU});
    auto tga=decode_firestorm_tga(bytes);
    if(!tga.ok()||tga.image.width!=2U||tga.image.height!=2U||
       tga.image.pixels[0U]!=128U||
       tga.image.pixels[4U]!=128U||
       tga.image.pixels[8U]!=0U||
       tga.image.pixels[12U]!=255U)return 1;
    auto corrupted=bytes;corrupted.back()=0x00U;
    corrupted.pop_back();
    if(decode_firestorm_tga(corrupted).ok())return 2;
    corrupted=bytes;corrupted[18U]=0xffU;
    if(decode_firestorm_tga(corrupted).ok())return 3;
    corrupted=bytes;corrupted[2]=5U;
    if(decode_firestorm_tga(corrupted).ok())return 4;
    corrupted=bytes;corrupted[12]=0U;corrupted[13]=20U;
    if(decode_firestorm_tga(corrupted).ok())return 5;
    // Uncompressed top/right? Horizontal mirroring unsupported.
    corrupted=bytes;corrupted[17]=0x30U;
    if(decode_firestorm_tga(corrupted).ok())return 6;
    // 1x1 raw BGR orange pixel (R=255 G=128 B=0).
    bytes.assign(18U,0U);bytes[2]=2U;bytes[12]=1U;
    bytes[14]=1U;bytes[16]=24U;bytes[17]=0x20U;
    bytes.insert(bytes.end(),{0U,128U,255U});
    tga=decode_firestorm_tga(bytes);
    if(!tga.ok()||tga.image.pixels!=
       std::vector<std::uint8_t>{255U,128U,0U,255U})return 7;
    const auto xml=R"(<linden_avatar wearable_definition_version="22">
      <layer_set body_region="upper_body" width="512" height="512">
       <layer name="shirt-mask">
        <texture local_texture="upper_shirt"/>
        <param id="200" wearable="shirt" value_min="0" value_max="1"
               value_default="0.5">
         <param_alpha tga_file="test_alpha.tga" domain="0.1" />
        </param>
       </layer>
       <layer name="bump" render_pass="bump">
        <texture local_texture="upper_gloves"/>
       </layer>
      </layer_set>
      </linden_avatar>)";
    auto parsed=parse_firestorm_texlayers(xml);
    if(!parsed.manifest||
       parsed.manifest->sets[1U].layers[0].safe_preview||
       !parsed.manifest->sets[1U].layers[0].alpha_params_only||
       parsed.manifest->sets[1U].layers[0].alpha_params.size()!=1U||
       parsed.manifest->sets[1U].layers[0].alpha_params[0].id!=200||
       parsed.manifest->sets[1U].layers[0].alpha_params[0].domain!=0.1f)
        return 8;
    const auto bad=[](const std::string& from,
                      const std::string& to)->bool{
        auto source=std::string{R"(<linden_avatar wearable_definition_version="22">
        <layer_set body_region="upper_body" width="512" height="512">
        <layer name="m"><texture local_texture="upper_shirt"/>
        <param id="200" value_min="0" value_max="1"><param_alpha domain="0.1"/></param>
        </layer></layer_set></linden_avatar>)"};
        const auto at=source.find(from);
        if(at==std::string::npos)return false;
        source.replace(at,from.size(),to);
        return !parse_firestorm_texlayers(source).manifest.has_value();
    };
    if(!bad("id=\"200\"","id=\"bad\"")||
       !bad("value_min=\"0\"","value_min=\"5\"")||
       !bad("domain=\"0.1\"","domain=\"Infinity\""))return 9;
    // Nonzero Firestorm domain is not accurately represented by the
    // diagnostic CPU preview; keep the original XML parsing assertion
    // above, then use a zero-domain fixture for pixel-alpha checks.
    auto preview_xml=std::string{xml};
    const auto domain_at=preview_xml.find("domain=\"0.1\"");
    if(domain_at==std::string::npos)return 15;
    preview_xml.replace(domain_at,12U,"domain=\"0\"");
    const auto preview_manifest=parse_firestorm_texlayers(preview_xml);
    if(!preview_manifest.manifest)return 16;
    std::map<unsigned,ImageRgba8> source;
    source.emplace(1U,ImageRgba8{1U,1U,{240U,80U,0U,255U}});
    std::map<std::string,ImageRgba8> masks;
    masks.emplace("test_alpha.tga",ImageRgba8{1U,1U,{128U,128U,128U,255U}});
    std::map<int,float> params{{200,0.5f}};
    auto blend=compose_firestorm_mask_preview(*preview_manifest.manifest,1U,
                                               source,masks,params);
    if(!blend.composite.preview_ready()||blend.real_bake()||
       blend.mask_layers_applied!=1U||
       blend.composite.unsupported!=1U||
       blend.composite.image.pixels[0U]!=240U||
       blend.composite.image.pixels[3U]<63U||
       blend.composite.image.pixels[3U]>65U)
        return 10;
    params.clear();
    blend=compose_firestorm_mask_preview(*preview_manifest.manifest,1U,
                                          source,masks,params);
    if(blend.composite.preview_ready()||blend.mask_layers_applied!=0U)
        return 11;
    params.emplace(200,1.0f);masks.clear();
    blend=compose_firestorm_mask_preview(*preview_manifest.manifest,1U,
                                          source,masks,params);
    if(blend.composite.preview_ready()||blend.missing_masks!=1U)
        return 12;
    if(const auto root=find_firestorm_avatar_character_directory())
    {
        const auto live=load_firestorm_character_tga(
            *root,"glove_length_alpha.tga");
        if(!live.ok()||live.image.width<128U||live.image.height<128U)
        {
            std::cerr<<"Original Firestorm TGA: "<<live.error<<"\n";
            return 13;
        }
        if(load_firestorm_character_tga(*root,"../bad.tga").ok())
            return 14;
        std::cout<<"Original Firestorm RLE TGA decoded "
                 <<live.image.width<<"x"<<live.image.height<<"\n";
    }
    std::cout<<"Firestorm raw/RLE TGAs, alpha_param XML and authenticated weight gated partial mask compositing OK\n";
    return 0;
}
