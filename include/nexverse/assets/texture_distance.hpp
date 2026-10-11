/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/assets/asset_view_priority.hpp"
#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nexverse::assets
{
struct TextureDistanceSample
{
    std::string id;
    float x=0.0f, y=0.0f, z=0.0f;
};
[[nodiscard]] inline std::vector<std::string> rank_texture_distances(
    const std::vector<TextureDistanceSample>& samples,
    float x,float y,float z,
    const std::unordered_set<std::string>& loaded,
    float hysteresis_m = 8.0f,
    const AssetCameraHint* camera = nullptr)
{
    std::unordered_map<std::string,double> distances;
    for(const auto& item:samples)
    {
        if(item.id.empty()||!std::isfinite(item.x)||
           !std::isfinite(item.y)||!std::isfinite(item.z))continue;
        const double dx=static_cast<double>(item.x)-x;
        const double dy=static_cast<double>(item.y)-y;
        const double dz=static_cast<double>(item.z)-z;
        const double d=std::sqrt(dx*dx+dy*dy+dz*dz);
        if(!std::isfinite(d))continue;
        const double weighted = d*(camera
            ? view_priority_multiplier(*camera,item.x,item.y,item.z) : 1.0);
        const auto it=distances.find(item.id);
        if(it==distances.end())distances.emplace(item.id,weighted);
        else it->second=std::min(it->second,weighted);
    }
    std::vector<std::pair<std::string,double>> priority;
    priority.reserve(distances.size());
    for(const auto& [id,d]:distances)
        priority.emplace_back(id,std::max(0.0,d-
            (loaded.contains(id)?std::max(0.0f,hysteresis_m):0.0f)));
    std::sort(priority.begin(),priority.end(),[](const auto& a,const auto& b)
    {return a.second!=b.second?a.second<b.second:a.first<b.first;});
    std::vector<std::string> ids;
    ids.reserve(priority.size());
    for(const auto& item:priority)ids.push_back(item.first);
    return ids;
}
}
