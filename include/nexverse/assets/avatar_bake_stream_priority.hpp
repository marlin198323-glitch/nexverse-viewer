/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_request_priority.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

namespace nexverse::assets
{
// Original server AvatarAppearance TextureEntry references only. The
// Head/Upper/Lower original Firestorm .llm meshes have highest priority;
// only the genuinely supported optional Firestorm Hair face is streamed
// after those three. Do NOT fetch arbitrary unrendered accessory slots.
struct AvatarBakeStreamCandidate
{
    bool is_self = false;
    std::array<std::string,3U> body{};
    float x = 0.0f, y = 0.0f, z = 0.0f;
    // Optional original Firestorm avatar_hair.llm baked Hair slot 5.
    // Lower priority than the three anatomical body bakes.
    std::string hair_bake_id;
};

[[nodiscard]] inline std::vector<std::string>
prioritize_avatar_body_bakes(
    const std::vector<AvatarBakeStreamCandidate>& avatars,
    float self_x,float self_y,float self_z,
    std::size_t max_assets = 12U,float remote_radius = 96.0f)
{
    std::vector<std::string> result;
    if(max_assets==0U || !std::isfinite(remote_radius) ||
       remote_radius<0.0f || !std::isfinite(self_x) ||
       !std::isfinite(self_y) || !std::isfinite(self_z))
        return result;
    max_assets=std::min(max_assets,std::size_t{24U}); // hard request priority cap
    result.reserve(max_assets);
    std::unordered_set<std::string> used;
    const auto append=[&](const AvatarBakeStreamCandidate& avatar)
    {
        const auto add=[&](const std::string& id)
        {
            if(result.size()>=max_assets||id.empty()||
               id==null_texture_asset_id||
               std::find(nexverse::protocol::baked_placeholders.begin(),
                         nexverse::protocol::baked_placeholders.end(),id)!=
                   nexverse::protocol::baked_placeholders.end()||
               !used.insert(id).second)return;
            result.push_back(id);
        };
        // Skin always wins; hair cannot displace Head/Upper/Lower
        // from the bounded highest-priority queue.
        for(const auto& id:avatar.body)add(id);
        add(avatar.hair_bake_id);
    };
    // Authentication/epoch contract is enforced by the caller: these are
    // simulator-owned refs from the current WorldModel, NEVER guessed UUIDs.
    for(const auto& avatar:avatars)
        if(avatar.is_self)append(avatar);

    struct Remote{float distance2;std::size_t index;};
    std::vector<Remote> remotes;
    const double radius2=static_cast<double>(remote_radius)*remote_radius;
    for(std::size_t i=0U;i<avatars.size();++i)
    {
        const auto& a=avatars[i];
        if(a.is_self || !std::isfinite(a.x) || !std::isfinite(a.y) ||
           !std::isfinite(a.z))continue;
        const double dx=static_cast<double>(a.x)-self_x;
        const double dy=static_cast<double>(a.y)-self_y;
        const double dz=static_cast<double>(a.z)-self_z;
        const double dist2=dx*dx+dy*dy+dz*dz;
        if(dist2<=radius2)
            remotes.push_back({static_cast<float>(dist2),i});
    }
    std::sort(remotes.begin(),remotes.end(),[](const Remote& a,const Remote& b)
    {
        if(a.distance2!=b.distance2)return a.distance2<b.distance2;
        return a.index<b.index;
    });
    for(const auto& remote:remotes)
    {
        if(result.size()>=max_assets)break;
        append(avatars[remote.index]);
    }
    return result;
}
}
