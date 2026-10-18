/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once

#include "nexverse/render/avatar_attachment_anchor.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <unordered_map>

namespace nexverse::render
{
// A leaf-to-root ancestry snapshot shared by visibility and world transforms.
// A missing, unrecognized parent does not authorize an avatar anchor.
// A cyclic or overlong linkset is invalid and must never draw in-world.
struct SceneParentChain
{
    static constexpr std::size_t max_depth=128U;
    std::array<std::size_t,max_depth> leaf_to_root{};
    std::size_t count=0U;
    std::uint32_t avatar_local_id=0U;
    bool valid=false;
    bool missing_parent=false;
};

[[nodiscard]] inline SceneParentChain resolve_scene_parent_chain(
    std::span<const RenderSceneObject> objects,
    const std::unordered_map<std::uint32_t,std::size_t>& by_local_id,
    std::span<const RenderAvatarProxy> avatars,
    std::size_t leaf) noexcept
{
    SceneParentChain result;
    if(leaf>=objects.size())return result;
    std::size_t current=leaf;
    for(std::size_t depth=0U;depth<SceneParentChain::max_depth;++depth)
    {
        if(current>=objects.size())return result;
        const auto visited=std::find(
            result.leaf_to_root.begin(),
            result.leaf_to_root.begin()+static_cast<std::ptrdiff_t>(result.count),
            current);
        if(visited!=result.leaf_to_root.begin()+
            static_cast<std::ptrdiff_t>(result.count))
            return result;
        result.leaf_to_root[result.count++]=current;
        const auto& object=objects[current];
        if(object.parent_id==0U)
        {
            result.valid=true;
            return result;
        }
        if(find_attachment_avatar_parent(avatars,object.parent_id))
        {
            result.avatar_local_id=object.parent_id;
            result.valid=true;
            return result;
        }
        const auto found=by_local_id.find(object.parent_id);
        if(found==by_local_id.end())
        {
            result.missing_parent=true;
            result.valid=true; // legacy-safe local root, no fabricated parent
            return result;
        }
        current=found->second;
    }
    return result; // depth cap, no unbounded recursions
}
}
