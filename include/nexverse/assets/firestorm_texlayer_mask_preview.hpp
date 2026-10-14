/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_texlayer_composite.hpp"
#include "nexverse/assets/firestorm_texlayer_color.hpp"
#include <map>
#include <string>
namespace nexverse::assets
{
struct FirestormMaskPreview
{
    FirestormCompositePreview composite;
    std::size_t mask_layers_applied=0U,missing_masks=0U,
        color_layers_applied=0U,unresolved_color_layers=0U,
        global_color_layers_applied=0U,multiple_mask_layers_applied=0U;
    [[nodiscard]] bool real_bake() const noexcept{return false;}
};
// DIAGNOSTIC approximate mask preview, not official LLTexLayerSet baking.
// Single uncolored alpha param + TGA mask only; domain and Firestorm
// multiply_blend are not yet pixel-identical, so no server/GPU body mutation.
// Actual wearable param weights must be supplied by authenticated originals.
[[nodiscard]] inline FirestormMaskPreview
compose_firestorm_mask_preview(
    const FirestormTexLayerManifest& manifest,unsigned group,
    const std::map<unsigned,ImageRgba8>& sources,
    const std::map<std::string,ImageRgba8>& alpha_masks,
    const std::map<int,float>& visual_params,
    const std::map<std::string,std::array<float,4>>& global_colors={})
{
    FirestormMaskPreview out;
    if(!manifest.valid()||group>=manifest.sets.size())
    {
        out.composite.error="Invalid TexLayer manifest/region";
        return out;
    }
    const auto& layers=manifest.sets[group].layers;
    for(const auto& layer:layers)
    {
        const bool safe_base=layer.texture_entry&&
            !layer.has_visibility_mask&&!layer.is_bump&&!layer.is_mask&&
            !layer.has_external_tga&&!layer.local_alpha_only&&
            !layer.multi_texture;
        const bool known_params=!layer.has_parameters||
            layer.alpha_params_only||layer.color_params_only||
            layer.known_color_and_alpha_only;
        const bool masked=!layer.alpha_params.empty();
        if(!safe_base||!known_params)
        {
            if(layer.has_global_color)++out.unresolved_color_layers;
            ++out.composite.unsupported;
            continue;
        }
        const auto tint=evaluate_firestorm_local_layer_tint(
            layer,visual_params,global_colors);
        if(!tint)
        {
            ++out.unresolved_color_layers;
            ++out.composite.unsupported;
            continue;
        }
        const auto image=sources.find(*layer.texture_entry);
        if(image==sources.end())
        {
            ++out.composite.missing;
            continue;
        }
        const auto& src=image->second;
        if(!src.valid()||src.width>128U||src.height>128U)
        {
            out.composite.error="Invalid source RGBA";
            out.composite.image={};out.composite.applied=0U;
            return out;
        }
        struct MaskSample
        {
            const ImageRgba8* image=nullptr;
            float weight=0.0f;
            bool multiply=false;
        };
        std::vector<MaskSample> samples;
        bool unsupported_mask=false,missing_mask=false;
        if(masked&&layer.alpha_params.front().multiply_blend)
            unsupported_mask=true; // Firestorm inherits previous target alpha.
        for(const auto& param:layer.alpha_params)
        {
            if(unsupported_mask)break;
            const auto weight=visual_params.find(param.id);
            if(weight==visual_params.end()||param.mask_file.empty()||
               !std::isfinite(weight->second)||
               param.domain!=0.0f||
               param.maximum<=param.minimum||
               weight->second<param.minimum-0.00001f||
               weight->second>param.maximum+0.00001f)
            {
                unsupported_mask=true;break;
            }
            const auto file=alpha_masks.find(param.mask_file);
            if(file==alpha_masks.end())
            {
                missing_mask=true;break;
            }
            if(!file->second.valid()||file->second.width==0U||
               file->second.height==0U)
            {
                unsupported_mask=true;break;
            }
            const float strength=std::clamp(
                (weight->second-param.minimum)/
                (param.maximum-param.minimum),0.0f,1.0f);
            if(param.skip_if_zero&&strength<=0.0f)
            {
                unsupported_mask=true;break;
            }
            samples.push_back({&file->second,strength,param.multiply_blend});
        }
        if(unsupported_mask)
        {
            ++out.composite.unsupported;
            continue;
        }
        if(missing_mask)
        {
            ++out.missing_masks;++out.composite.missing;
            continue;
        }
        if(!out.composite.image.valid())
        {
            out.composite.image=ImageRgba8{
                src.width,src.height,
                std::vector<std::uint8_t>(
                    static_cast<std::size_t>(src.width)*src.height*4U,0U)};
        }
        auto& dst=out.composite.image;
        if(dst.width!=src.width||dst.height!=src.height)
        {
            out.composite.error="Preview layer size mismatch";
            out.composite.image={};out.composite.applied=0U;
            return out;
        }
        for(std::size_t i=0U;i<src.pixels.size();i+=4U)
        {
            const auto pixel=i/4U,x=pixel%src.width,y=pixel/src.width;
            float mask_alpha=masked?0.0f:1.0f;
            for(const auto& sample:samples)
            {
                const auto& mask=*sample.image;
                const auto mx=std::min(mask.width-1U,
                    static_cast<unsigned>(
                       (static_cast<std::uint64_t>(x)*mask.width)/
                       src.width));
                const auto my=std::min(mask.height-1U,
                    static_cast<unsigned>(
                       (static_cast<std::uint64_t>(y)*mask.height)/
                       src.height));
                const auto mid=(static_cast<std::size_t>(my)*mask.width+
                                mx)*4U;
                const float alpha=static_cast<float>(
                    mask.pixels[mid])/255.0f*sample.weight;
                // Faithful blend *order* (ADD then MULTIPLY), but CPU
                // alpha approximation is not Firestorm GPU pixel parity.
                if(sample.multiply)mask_alpha*=alpha;
                else mask_alpha=std::min(1.0f,mask_alpha+alpha);
            }
            const float sa=static_cast<float>(src.pixels[i+3U])/255.0f*
                (*tint)[3]*mask_alpha;
            const float da=static_cast<float>(dst.pixels[i+3U])/255.0f;
            const float oa=sa+da*(1.0f-sa);
            for(std::size_t c=0U;c<3U;++c)
            {
                const float rgb=oa>0.0f
                    ?(static_cast<float>(src.pixels[i+c])*(*tint)[c]*sa+
                      static_cast<float>(dst.pixels[i+c])*da*(1.0f-sa))/oa
                    :0.0f;
                dst.pixels[i+c]=static_cast<std::uint8_t>(
                    std::clamp(std::lround(rgb),0L,255L));
            }
            dst.pixels[i+3U]=static_cast<std::uint8_t>(
                std::clamp(std::lround(oa*255.0f),0L,255L));
        }
        ++out.composite.applied;
        if(masked)
        {
            ++out.mask_layers_applied;
            if(samples.size()>1U)++out.multiple_mask_layers_applied;
        }
        if(layer.has_global_color)++out.global_color_layers_applied;
        if(layer.fixed_rgba||layer.has_global_color||!layer.color_params.empty())
            ++out.color_layers_applied;
    }
    if(!out.composite.preview_ready())
        out.composite.error="No supported RGB partial layers decoded";
    return out;
}
}
