/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_texlayer_manifest.hpp"
#include <tinyxml2.h>
#include <algorithm>
#include <charconv>
#include <optional>
#include <cmath>
#include <string_view>
#include <fstream>
#include <iterator>
#include <system_error>
namespace nexverse::assets
{
namespace
{
std::optional<std::array<std::uint8_t,4>> xml_rgba(
    std::string_view input)
{
    std::array<std::uint8_t,4> result{};
    for(auto& channel:result)
    {
        while(!input.empty()&&input.front()==' ')input.remove_prefix(1U);
        const auto end=input.find(',');
        const auto current=input.substr(0U,end);
        int value=-1;
        const auto [stop,err]=std::from_chars(
            current.data(),current.data()+current.size(),value);
        if(err!=std::errc{}||value<0||value>255||
           stop!=current.data()+current.size())
            return std::nullopt;
        channel=static_cast<std::uint8_t>(value);
        if(end==std::string_view::npos)
        {
            if(&channel!=&result.back())return std::nullopt;
            input={};
        }
        else
        {
            if(&channel==&result.back())return std::nullopt;
            input.remove_prefix(end+1U);
        }
    }
    return input.empty()?std::optional{result}:std::nullopt;
}
}
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
            if(const auto* fixed=layer->Attribute("fixed_color"))
            {
                item.fixed_rgba=xml_rgba(fixed);
                if(!item.fixed_rgba)return failure("Malformed Firestorm fixed_color");
            }
            if(const auto* global=layer->Attribute("global_color"))
            {
                item.global_color_name=global;
                if(item.global_color_name.size()>80U)
                    return failure("Global color name oversized");
            }
            bool all_alpha=item.has_parameters;
            bool all_color=item.has_parameters;
            bool only_known=item.has_parameters;
            for(auto* param=layer->FirstChildElement("param");param;
                param=param->NextSiblingElement("param"))
            {
                if(item.alpha_params.size()>=16U)
                    return failure("Too many alpha params per TexLayer");
                const auto* color=param->FirstChildElement("param_color");
                if(color)
                {
                    all_alpha=false;
                    if(param->FirstChildElement("param_alpha")||
                       item.color_params.size()>=16U)
                        return failure("Conflicting/too many Firestorm color params");
                    FirestormTexLayerColorParam entry;
                    if(param->QueryIntAttribute("id",&entry.id)!=
                        tinyxml2::XML_SUCCESS||entry.id<0||entry.id>200000)
                        return failure("Invalid Firestorm color param id");
                    const auto op=color->Attribute("operation");
                    const auto value=op?std::string_view{op}:std::string_view{"add"};
                    if(value=="add")entry.operation=FirestormColorOperation::add;
                    else if(value=="multiply")
                        entry.operation=FirestormColorOperation::multiply;
                    else if(value=="blend")
                        entry.operation=FirestormColorOperation::blend;
                    else return failure("Unknown Firestorm color operation");
                    if(param->Attribute("value_min")&&
                       param->QueryFloatAttribute("value_min",&entry.minimum)!=
                           tinyxml2::XML_SUCCESS)
                        return failure("Color minimum invalid");
                    if(param->Attribute("value_max")&&
                       param->QueryFloatAttribute("value_max",&entry.maximum)!=
                           tinyxml2::XML_SUCCESS)
                        return failure("Color maximum invalid");
                    if(!std::isfinite(entry.minimum)||
                       !std::isfinite(entry.maximum)||
                       entry.maximum<=entry.minimum||
                       entry.minimum<0.0f||entry.maximum>100.0f)
                        return failure("Color param range invalid");
                    for(auto* value_node=color->FirstChildElement("value");
                        value_node;value_node=value_node->NextSiblingElement("value"))
                    {
                        const auto* rgba=value_node->Attribute("color");
                        if(entry.palette.size()>=32U||!rgba)
                            return failure("Color palette too large/invalid");
                        const auto parsed=xml_rgba(rgba);
                        if(!parsed)return failure("Bad Firestorm palette RGBA");
                        entry.palette.push_back(*parsed);
                    }
                    if(entry.palette.empty()||
                       (entry.operation==FirestormColorOperation::blend&&
                        entry.palette.size()!=1U))
                        return failure("Invalid Firestorm color palette");
                    item.color_params.push_back(std::move(entry));
                    ++manifest.color_param_count;
                    continue;
                }
                all_color=false;
                const auto* alpha=param->FirstChildElement("param_alpha");
                if(!alpha)
                {
                    all_alpha=false;
                    only_known=false;
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
            item.color_params_only=all_color&&
                !item.color_params.empty();
            item.known_color_and_alpha_only=only_known&&
                !item.color_params.empty()&&!item.alpha_params.empty();
            if(!item.color_params.empty()||item.fixed_rgba||
               !item.global_color_name.empty())++manifest.color_layer_count;
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
    // Original Firestorm avatar_lad.xml defines skin/hair/eye global
    // colors separately from region layers. Never infer palette or defaults.
    for(auto* node=root->FirstChildElement("global_color");node;
        node=node->NextSiblingElement("global_color"))
    {
        const auto* name=node->Attribute("name");
        if(!name)return failure("Missing Firestorm global color name");
        const std::string key{name};
        int expected_type=-1;
        if(key=="skin_color")expected_type=1;
        else if(key=="hair_color")expected_type=2;
        else if(key=="eye_color")expected_type=3;
        else return failure("Unknown Firestorm global color name");
        if(manifest.global_colors.contains(key))
            return failure("Duplicate Firestorm global color name");
        FirestormTexGlobalColor global;
        global.name=key;global.wearable_type=expected_type;
        for(auto* param=node->FirstChildElement("param");param;
            param=param->NextSiblingElement("param"))
        {
            if(global.color_params.size()>=16U)
                return failure("Too many global color params");
            const auto* wearable=param->Attribute("wearable");
            const auto* color=param->FirstChildElement("param_color");
            if(!wearable||!color||
               std::string_view{wearable}!=
                   (expected_type==1?"skin":expected_type==2?"hair":"eyes"))
                return failure("Firestorm global param owner/color invalid");
            FirestormTexLayerColorParam item;
            if(param->QueryIntAttribute("id",&item.id)!=tinyxml2::XML_SUCCESS||
               item.id<0||item.id>200000)
                return failure("Invalid global color parameter id");
            if(std::any_of(global.color_params.begin(),global.color_params.end(),
                 [&](const auto& x){return x.id==item.id;}))
                return failure("Duplicate global color parameter");
            if(param->Attribute("value_min")&&
               param->QueryFloatAttribute("value_min",&item.minimum)!=
                   tinyxml2::XML_SUCCESS)
                return failure("Invalid global color minimum");
            if(param->Attribute("value_max")&&
               param->QueryFloatAttribute("value_max",&item.maximum)!=
                   tinyxml2::XML_SUCCESS)
                return failure("Invalid global color maximum");
            if(!std::isfinite(item.minimum)||!std::isfinite(item.maximum)||
               item.minimum<0.0f||item.maximum>100.0f||
               item.minimum>=item.maximum)
                return failure("Global color range invalid");
            const auto* op=color->Attribute("operation");
            const auto operation=op?std::string_view{op}:std::string_view{"add"};
            if(operation=="add")item.operation=FirestormColorOperation::add;
            else if(operation=="multiply")
                item.operation=FirestormColorOperation::multiply;
            else if(operation=="blend")item.operation=FirestormColorOperation::blend;
            else return failure("Global color operation invalid");
            for(auto* value=color->FirstChildElement("value");value;
                value=value->NextSiblingElement("value"))
            {
                if(item.palette.size()>=32U)
                    return failure("Global palette oversized");
                const auto* rgba=value->Attribute("color");
                const auto parsed=rgba?xml_rgba(rgba):std::nullopt;
                if(!parsed)return failure("Global palette color invalid");
                item.palette.push_back(*parsed);
            }
            if(item.palette.empty()||
               (item.operation==FirestormColorOperation::blend&&
                item.palette.size()!=1U))
                return failure("Global palette missing or invalid");
            global.color_params.push_back(std::move(item));
        }
        if(global.color_params.empty())
            return failure("Global color without parameters");
        manifest.global_colors.emplace(key,std::move(global));
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
