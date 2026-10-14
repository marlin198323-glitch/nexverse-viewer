/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/firestorm_texlayer_manifest.hpp"
#include <array>
#include <map>
#include <string>
namespace nexverse::assets
{
// Read-only input-coverage diagnostics across all eleven Firestorm bake
// regions. A source count is NOT a valid composite or simulator bake.
// In particular bump/alpha-only/morph/visibility, missing mask and
// global color semantics cannot be inferred from these counts.
struct FirestormBakeRegionCoverage
{
    std::size_t layers=0U;
    std::size_t local_sources=0U,decoded_sources=0U,missing_sources=0U;
    std::size_t complex_layers=0U;
    bool declared=false;
    [[nodiscard]] bool has_preview_input() const noexcept
    {return declared&&decoded_sources>0U;}
};
struct FirestormBakeCoverage
{
    std::array<FirestormBakeRegionCoverage,11U> regions{};
    std::size_t decoded=0U,missing=0U,complex=0U,
                covered_regions=0U;
    [[nodiscard]] bool upload_authorized() const noexcept
    {return false;} // A verified client bake/upload is NOT implemented.
};
[[nodiscard]] inline FirestormBakeCoverage
audit_firestorm_bake_sources(
    const FirestormTexLayerManifest& manifest,
    const std::map<unsigned,ImageRgba8>& authenticated_sources)
{
    FirestormBakeCoverage audit;
    if(!manifest.valid())return audit;
    for(std::size_t group=0U;group<manifest.sets.size();++group)
    {
        const auto& set=manifest.sets[group];
        auto& row=audit.regions[group];
        row.declared=set.width>0U&&set.height>0U;
        row.layers=set.layers.size();
        for(const auto& layer:set.layers)
        {
            if(layer.is_bump||layer.is_mask||layer.local_alpha_only||
               layer.has_visibility_mask||layer.has_external_tga||
               layer.multi_texture||layer.has_parameters||
               layer.has_global_color||layer.has_fixed_color)
            {
                ++row.complex_layers;
                ++audit.complex;
            }
            if(!layer.texture_entry)continue;
            ++row.local_sources;
            const auto image=authenticated_sources.find(*layer.texture_entry);
            if(image==authenticated_sources.end()||
               !image->second.valid()||image->second.width>128U||
               image->second.height>128U)
            {
                ++row.missing_sources;++audit.missing;
            }
            else
            {
                ++row.decoded_sources;++audit.decoded;
            }
        }
        if(row.has_preview_input())++audit.covered_regions;
    }
    return audit;
}
}
