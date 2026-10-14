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
        color_layers_applied=0U,unresolved_color_layers=0U;
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
    const std::map<int,float>& visual_params)
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
            !layer.has_global_color&&
            !layer.has_visibility_mask&&!layer.is_bump&&!layer.is_mask&&
            !layer.has_external_tga&&!layer.local_alpha_only&&
            !layer.multi_texture;
        const bool known_params=!layer.has_parameters||
            layer.alpha_params_only||layer.color_params_only||
            layer.known_color_and_alpha_only;
        const bool masked=!layer.alpha_params.empty()&&
            layer.alpha_params.size()==1U;
        if(!safe_base||!known_params||
           (!layer.alpha_params.empty()&&!masked))
        {
            if(layer.has_global_color)++out.unresolved_color_layers;
            ++out.composite.unsupported;
            continue;
        }
        const auto tint=evaluate_firestorm_local_layer_tint(
            layer,visual_params);
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
        const ImageRgba8* mask=nullptr;
        float strength=1.0f;
        if(masked)
        {
            const auto& param=layer.alpha_params.front();
            const auto weight=visual_params.find(param.id);
            // No blind use of definition defaults: a user-specific worn
            // LLWearable asset has to supply an actual value.
            if(weight==visual_params.end()||
               param.mask_file.empty()||
               !std::isfinite(weight->second))
            {
                ++out.composite.unsupported;
                continue;
            }
            const auto file=alpha_masks.find(param.mask_file);
            if(file==alpha_masks.end())
            {
                ++out.missing_masks;
                ++out.composite.missing;
                continue;
            }
            mask=&file->second;
            if(!mask->valid()||mask->width==0U||mask->height==0U)
            {
                ++out.composite.unsupported;
                continue;
            }
            const auto range=param.maximum-param.minimum;
            if(range<=0.0f)
            {
                ++out.composite.unsupported;
                continue;
            }
            strength=std::clamp((weight->second-param.minimum)/range,
                                0.0f,1.0f);
            if(param.skip_if_zero&&strength<=0.0f)
            {
                ++out.composite.unsupported;
                continue;
            }
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
            float mask_alpha=1.0f;
            if(mask)
            {
                const auto mx=std::min(mask->width-1U,
                    static_cast<unsigned>(
                       (static_cast<std::uint64_t>(x)*mask->width)/
                       src.width));
                const auto my=std::min(mask->height-1U,
                    static_cast<unsigned>(
                       (static_cast<std::uint64_t>(y)*mask->height)/
                       src.height));
                const auto mid=(static_cast<std::size_t>(my)*mask->width+mx)*4U;
                mask_alpha=static_cast<float>(mask->pixels[mid])/255.0f;
            }
            const float sa=static_cast<float>(src.pixels[i+3U])/255.0f*
                (*tint)[3]*mask_alpha*strength;
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
        if(masked)++out.mask_layers_applied;
        if(layer.fixed_rgba||!layer.color_params.empty())
            ++out.color_layers_applied;
    }
    if(!out.composite.preview_ready())
        out.composite.error="No supported RGB partial layers decoded";
    return out;
}
}
