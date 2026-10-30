/* This Source Code Form is subject to MPL-2.0. */
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// GPU UUID residency ≠ correct material, visible texel resolution,
// texture fidelity or a correctly textured draw. This breakdown counts
// only the planner's current unique wanted texture IDs; not scene faces.
struct TextureMipEvidence
{
    std::size_t base_mip=0U;
    std::size_t reduced_mip=0U;
    std::size_t unknown_mip=0U;
    [[nodiscard]] constexpr std::size_t counted() const noexcept
    {
        return base_mip+reduced_mip+unknown_mip;
    }
};
[[nodiscard]] inline TextureMipEvidence texture_mip_evidence(
    const std::vector<std::string>& wanted_ids,
    const std::unordered_set<std::string>& resident_ids,
    const std::unordered_map<std::string,std::uint8_t>& resident_mips)
{
    TextureMipEvidence result;
    std::unordered_set<std::string> seen;
    for(const auto& id:wanted_ids)
    {
        if(id.empty()||!seen.insert(id).second||
           !resident_ids.contains(id))continue;
        if(const auto it=resident_mips.find(id);it!=resident_mips.end())
        {
            if(it->second==0U)++result.base_mip;
            else ++result.reduced_mip;
        }
        else ++result.unknown_mip;
    }
    return result;
}
}
