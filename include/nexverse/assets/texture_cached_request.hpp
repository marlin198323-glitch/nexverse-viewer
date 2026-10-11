/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_disk_cache.hpp"
#include "nexverse/assets/texture_memory_cache.hpp"
#include "nexverse/assets/texture_mip.hpp"
#include <functional>
#include <string>
#include <string_view>
#include <utility>

namespace nexverse::assets
{
// One region-scoped worker path for RAM, disk and network GetTexture.
// The transport callback contains no capabilities in RAM/disk cache keys.
// A late network reply is discarded BEFORE admission to either CPU cache.
[[nodiscard]] inline TextureAssetResult load_texture_cached_scoped(
    std::string_view id,std::uint8_t mip,
    TextureMemoryCache& memory,const TextureDiskCache& disk,
    const std::function<TextureAssetResult()>& download,
    const std::function<bool()>& active)
{
    TextureAssetResult result;
    result.asset_id=std::string{id};
    const auto live=[&]{return !active || active();};
    if(!live())
    {
        result.message="Old region GetTexture job skipped";
        result.cancelled_before_download=true;
        return result;
    }
    if(const auto hit=memory.load(id))
    {
        result.success=true;
        result.image=*hit;
        result.message="RAM texture cache hit";
    }
    else if(const auto disk_hit=disk.load(id,&result.disk_lookup))
    {
        result.success=true;
        result.image=*disk_hit;
        result.message="Disk texture cache hit";
        if(live())(void)memory.store(id,result.image);
    }
    else
    {
        if(!live())
        {
            result.message="Old region GetTexture job skipped before HTTP";
            result.cancelled_before_download=true;
            return result;
        }
        const auto status=result.disk_lookup;
        result=download();
        result.disk_lookup=status;
        result.network_requested=true;
        if(!live())
        {
            result.success=false;
            result.image={};
            result.message="Old region GetTexture result discarded";
            return result;
        }
        if(result.success && result.asset_id!=id)
        {
            result.success=false;
            result.image={};
            result.failure=AssetFailure::invalid;
            result.message="GetTexture response asset ID mismatch";
            return result;
        }
        if(result.success && result.image.valid())
        {
            (void)memory.store(id,result.image);
            if(live())(void)disk.store(id,result.image);
        }
    }
    if(result.success && result.image.valid() && mip>0U)
    {
        if(auto reduced=texture_mip_image(result.image,mip))
            result.image=std::move(*reduced);
        else
        {
            result.success=false;
            result.message="Texture mip reduction failed";
            result.failure=AssetFailure::invalid;
        }
    }
    if(!live())
    {
        result.success=false;
        result.image={};
        result.message="Old region GetTexture result discarded";
    }
    return result;
}
}
