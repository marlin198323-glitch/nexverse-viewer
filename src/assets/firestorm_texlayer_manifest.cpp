/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_texlayer_manifest.hpp"
#include <tinyxml2.h>
#include <algorithm>
#include <cmath>
#include <string_view>
#include <fstream>
#include <iterator>
#include <system_error>
namespace nexverse::assets
{
FirestormTexLayerResult parse_firestorm_texlayers(std::string_view input)
{
    auto failure=[](std::string message)->FirestormTexLayerResult{
        return {std::nullopt,std::move(message)};
    };
    if(input.size()<90U||input.size()>1024U*1024U||
       input.find('\0')!=std::string_view::npos)
        return failure("avatar_lad.xml size or binary invalid");
    tinyxml2::XMLDocument xml;
    if(xml.Parse(input.data(),input.size())!=tinyxml2::XML_SUCCESS)
        return failure("avatar_lad.xml parse error");
    auto* root=xml.FirstChildElement("linden_avatar");
    if(!root)return failure("Not a Firestorm linden_avatar XML");
    FirestormTexLayerManifest manifest;
    if(root->QueryIntAttribute("wearable_definition_version",
       &manifest.definition_version)!=tinyxml2::XML_SUCCESS||
       manifest.definition_version<1||manifest.definition_version>64)
        return failure("Invalid wearable definition version");
    std::array<bool,11> seen{};
    for(auto* set=root->FirstChildElement("layer_set");set;
        set=set->NextSiblingElement("layer_set"))
    {
        const auto* region=set->Attribute("body_region");
        if(!region)return failure("Layer set without body_region");
        const auto where=std::find(firestorm_layer_regions.begin(),
                                   firestorm_layer_regions.end(),region);
        if(where==firestorm_layer_regions.end())
            return failure("Unknown Firestorm body_region");
        const auto group=static_cast<unsigned>(
            std::distance(firestorm_layer_regions.begin(),where));
        if(seen[group])return failure("Duplicate Firestorm layer_set region");
        seen[group]=true;
        auto& out=manifest.sets[group];out.group=group;
        if(set->QueryUnsignedAttribute("width",&out.width)!=
           tinyxml2::XML_SUCCESS||
           set->QueryUnsignedAttribute("height",&out.height)!=
           tinyxml2::XML_SUCCESS||
           out.width<8U||out.height<8U||
           out.width>4096U||out.height>4096U)
            return failure("Layer set texture dimensions invalid");
        for(auto* layer=set->FirstChildElement("layer");layer;
            layer=layer->NextSiblingElement("layer"))
        {
            if(++manifest.layer_count>512U||out.layers.size()>=128U)
                return failure("Too many Firestorm TexLayers");
            FirestormTexLayer item;
            item.ordinal=out.layers.size();
            const auto* name=layer->Attribute("name");
            item.name=name?name:"unnamed";
            if(item.name.size()>128U)return failure("TexLayer name too long");
            const auto* pass=layer->Attribute("render_pass");
            item.is_bump=pass&&std::string_view(pass)=="bump";
            item.has_parameters=layer->FirstChildElement("param")!=nullptr;
            bool all_alpha=item.has_parameters;
            for(auto* param=layer->FirstChildElement("param");param;
                param=param->NextSiblingElement("param"))
            {
                if(item.alpha_params.size()>=16U)
                    return failure("Too many alpha params per TexLayer");
                const auto* alpha=param->FirstChildElement("param_alpha");
                if(!alpha||param->FirstChildElement("param_color"))
                {
                    all_alpha=false;
                    continue;
                }
                FirestormTexLayerAlphaParam parsed;
                if(param->QueryIntAttribute("id",&parsed.id)!=
                    tinyxml2::XML_SUCCESS||parsed.id<0||
                    parsed.id>200000)
                    return failure("Invalid Firestorm param_alpha ID");
                const auto* wearable=param->Attribute("wearable");
                parsed.wearable=wearable?wearable:"";
                const auto* filename=alpha->Attribute("tga_file");
                parsed.mask_file=filename?filename:"";
                const auto decimal=[](const tinyxml2::XMLElement* node,
                    const char* key,float& output)->bool
                {
                    if(!node->Attribute(key))return true;
                    return node->QueryFloatAttribute(key,&output)==
                        tinyxml2::XML_SUCCESS&&std::isfinite(output);
                };
                if(!decimal(param,"value_min",parsed.minimum)||
                   !decimal(param,"value_max",parsed.maximum)||
                   !decimal(param,"value_default",parsed.default_value)||
                   !decimal(alpha,"domain",parsed.domain)||
                   parsed.minimum>parsed.maximum||
                   parsed.minimum<0.0f||parsed.maximum>100.0f||
                   parsed.domain<0.0f||parsed.domain>1.0f)
                    return failure("Invalid Firestorm alpha-param ranges");
                parsed.explicit_default=param->Attribute("value_default")!=nullptr;
                parsed.multiply_blend=alpha->BoolAttribute("multiply_blend",false);
                parsed.skip_if_zero=alpha->BoolAttribute("skip_if_zero",false);
                item.alpha_params.push_back(std::move(parsed));
            }
            item.alpha_params_only=all_alpha&&
                !item.alpha_params.empty();
            item.has_global_color=layer->Attribute("global_color")!=nullptr;
            item.has_fixed_color=layer->Attribute("fixed_color")!=nullptr;
            item.has_visibility_mask=layer->Attribute("visibility_mask")!=nullptr;
            std::size_t tex_count=0U;
            for(auto* tex=layer->FirstChildElement("texture");tex;
                tex=tex->NextSiblingElement("texture"))
            {
                if(++tex_count>4U)return failure("TexLayer too many textures");
                const auto* local=tex->Attribute("local_texture");
                if(tex->Attribute("tga_file"))
                    item.has_external_tga=true;
                if(tex->BoolAttribute("file_is_mask",false))
                    item.is_mask=true;
                if(tex->BoolAttribute("local_texture_alpha_only",false))
                    item.local_alpha_only=true;
                if(local)
                {
                    if(item.texture_entry)return failure("Multiple local textures in layer");
                    const auto found=std::find(
                        firestorm_texture_entry_names.begin(),
                        firestorm_texture_entry_names.end(),local);
                    if(found==firestorm_texture_entry_names.end())
                        return failure("Unknown Firestorm local_texture name");
                    const auto te=static_cast<unsigned>(std::distance(
                        firestorm_texture_entry_names.begin(),found));
                    if(firestorm_local_bake_groups[te]!=
                       static_cast<int>(group))
                        return failure("Firestorm local texture in wrong region");
                    item.texture_entry=te;
                    item.local_name=local;
                    ++manifest.local_layer_count;
                }
            }
            item.multi_texture=tex_count>1U;
            item.safe_preview=item.texture_entry.has_value()&&
                !item.is_bump&&!item.is_mask&&
                !item.has_parameters&&!item.has_external_tga&&
                !item.has_global_color&&!item.has_fixed_color&&
                !item.has_visibility_mask&&!item.local_alpha_only&&
                !item.multi_texture;
            if(item.safe_preview)++manifest.safe_layer_count;
            else ++manifest.unsupported_layer_count;
            out.layers.push_back(std::move(item));
        }
    }
    if(!manifest.valid())return failure("No valid Firestorm TexLayers");
    return {std::move(manifest),{}};
}
FirestormTexLayerResult load_firestorm_texlayers(
    const std::filesystem::path& root)
{
    std::error_code ec;
    const auto path=root/"avatar_lad.xml";
    const auto bytes=std::filesystem::file_size(path,ec);
    if(ec||bytes<90U||bytes>1024U*1024U)
        return {std::nullopt,"Firestorm avatar_lad.xml absent/oversized"};
    std::ifstream file(path,std::ios::binary);
    if(!file)return {std::nullopt,"Firestorm avatar_lad.xml not readable"};
    std::string data{std::istreambuf_iterator<char>{file},
                     std::istreambuf_iterator<char>{}};
    return parse_firestorm_texlayers(data);
}
}
