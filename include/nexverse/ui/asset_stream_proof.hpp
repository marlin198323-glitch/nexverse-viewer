/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstddef>
#include <string>
#include <unordered_set>

namespace nexverse::ui
{
// Region-local read-only asset evidence. Only counts current WorldModel
// unique asset UUIDs and currently GPU-resident matching UUIDs.
struct AssetStreamCoverage
{
    std::size_t requested=0U;
    std::size_t gpu_resident=0U;
};
struct AssetStreamProof
{
    AssetStreamCoverage textures,meshes;
};
template<typename SampleRange>
[[nodiscard]] inline AssetStreamCoverage asset_gpu_coverage(
    const SampleRange& samples,
    const std::unordered_set<std::string>& gpu_ids)
{
    std::unordered_set<std::string> unique;
    for(const auto& sample:samples)
        if(!sample.id.empty()&&sample.id!="00000000-0000-0000-0000-000000000000")
            unique.insert(sample.id);
    AssetStreamCoverage result{unique.size(),0U};
    for(const auto& id:unique)
        if(gpu_ids.contains(id))++result.gpu_resident;
    return result;
}
[[nodiscard]] inline std::string describe_asset_stream(
    const AssetStreamProof& p)
{
    // Clarify that numbers are unique GPU-resident UUIDs; the
    // dashboard must not imply full visible texture or mesh quality.
    return "Tex-IDs "+std::to_string(p.textures.gpu_resident)+"/"+
        std::to_string(p.textures.requested)+" GPU | Mesh-IDs "+
        std::to_string(p.meshes.gpu_resident)+"/"+
        std::to_string(p.meshes.requested)+" GPU";
}
}
