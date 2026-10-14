/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_texlayer_manifest.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
namespace nexverse::assets
{
// Deliberately a PARTIAL local source-layer preview.
// Firestorm's full layer set requires TGA assets, color params, weighted
// alpha masks, visibility masks, wearable tint and bake hashing. Until
// those are present we never export this as a server-accepted AvatarBake.
struct FirestormCompositePreview
{
    ImageRgba8 image;
    std::size_t applied=0U,unsupported=0U,missing=0U;
    std::string error;
    [[nodiscard]] bool preview_ready() const noexcept
    {return applied>0U&&image.valid();}
    [[nodiscard]] bool is_verified_bake() const noexcept {return false;}
};
[[nodiscard]] inline FirestormCompositePreview
compose_firestorm_partial_texlayers(
    const FirestormTexLayerManifest& manifest,unsigned group,
    const std::map<unsigned,ImageRgba8>& sources)
{
    FirestormCompositePreview out;
    if(!manifest.valid()||group>=manifest.sets.size())
    {
        out.error="Invalid TexLayer manifest/region";
        return out;
    }
    const auto& layers=manifest.sets[group].layers;
    if(layers.empty())
    {
        out.error="Firestorm TexLayer region has no layers";
        return out;
    }
    for(const auto& layer:layers)
    {
        if(!layer.safe_preview||!layer.texture_entry)
        {
            ++out.unsupported;
            continue;
        }
        const auto found=sources.find(*layer.texture_entry);
        if(found==sources.end())
        {
            ++out.missing;
            continue;
        }
        const auto& img=found->second;
        if(!img.valid()||img.width>128U||img.height>128U)
        {
            out.error="Invalid local RGBA layer";
            out.image={};
            out.applied=0U;
            return out;
        }
        if(!out.image.valid())
        {
            out.image=ImageRgba8{
                img.width,img.height,
                std::vector<std::uint8_t>(img.pixels.size(),0U)};
        }
        if(out.image.width!=img.width||
           out.image.height!=img.height)
        {
            out.error="Local TexLayer dimensions mismatch";
            out.image={};
            out.applied=0U;
            return out;
        }
        // Standard unpremultiplied Porter-Duff source-over for
        // ordinary color-bearing layers only; XML-driven unsafe mask,
        // global-color, bump and alpha-only inputs are skipped above.
        for(std::size_t i=0U;i<img.pixels.size();i+=4U)
        {
            const float sa=static_cast<float>(img.pixels[i+3U])/255.0f;
            const float da=static_cast<float>(out.image.pixels[i+3U])/255.0f;
            const float oa=sa+da*(1.0f-sa);
            for(std::size_t c=0U;c<3U;++c)
            {
                const float source=static_cast<float>(img.pixels[i+c]);
                const float dest=static_cast<float>(out.image.pixels[i+c]);
                const float rgb=oa>0.0f
                    ?(source*sa+dest*da*(1.0f-sa))/oa:0.0f;
                out.image.pixels[i+c]=static_cast<std::uint8_t>(
                    std::clamp(std::lround(rgb),0L,255L));
            }
            out.image.pixels[i+3U]=static_cast<std::uint8_t>(
                std::clamp(std::lround(oa*255.0f),0L,255L));
        }
        ++out.applied;
    }
    if(!out.preview_ready())
        out.error="No safe decoded local layer in this region";
    return out;
}
}
