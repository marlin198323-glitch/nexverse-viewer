/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace nexverse::assets
{
// Tracks successful CPU-cache-backed reuploads after GPU LOD or region changes.
// This does NOT retain GPU handles between regions or imply network savings
// without a reported RAM/disk cache hit. Bounded to 4096 UUIDs per family.
class AssetReuseHistory
{
public:
    explicit AssetReuseHistory(std::size_t limit=4096U)
        : limit_{limit} {}
    enum class Kind { first_seen, repeat, cached_lod_swap, cached_region_reuse };
    [[nodiscard]] Kind uploaded(std::string_view id,std::uint64_t epoch,
        std::uint32_t quality,bool cpu_cache_hit)
    {
        if(id.empty())return Kind::first_seen;
        const std::string key{id};
        const auto previous=items_.find(key);
        Kind kind=Kind::first_seen;
        if(previous!=items_.end())
        {
            if(cpu_cache_hit && previous->second.epoch!=epoch)
            {
                kind=Kind::cached_region_reuse;
                ++region_cache_hits_;
            }
            else if(cpu_cache_hit && previous->second.quality!=quality)
            {
                kind=Kind::cached_lod_swap;
                ++lod_cache_hits_;
            }
            else kind=Kind::repeat;
        }
        if(limit_==0U)return kind;
        if(previous==items_.end()&&items_.size()>=limit_)
            items_.erase(items_.begin());
        items_.insert_or_assign(key,Entry{epoch,quality});
        return kind;
    }
    [[nodiscard]] std::size_t region_cache_hits() const noexcept
    {return region_cache_hits_;}
    [[nodiscard]] std::size_t lod_cache_hits() const noexcept
    {return lod_cache_hits_;}
    [[nodiscard]] std::size_t tracked() const noexcept{return items_.size();}
private:
    struct Entry
    {
        std::uint64_t epoch=0U;
        std::uint32_t quality=0U;
    };
    std::size_t limit_;
    std::size_t region_cache_hits_=0U;
    std::size_t lod_cache_hits_=0U;
    std::unordered_map<std::string,Entry> items_;
};
}
