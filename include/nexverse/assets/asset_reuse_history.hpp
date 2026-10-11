/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
#include <cstdint>
#include <list>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>

namespace nexverse::assets
{
// CPU cache provenance of a SUCCESSFUL GPU upload. No GPU objects are
// retained across region epochs, and a cache hit must be reported by loader.
class AssetReuseHistory
{
public:
    enum class Tier { network, memory, disk };
    enum class Kind {
        first_seen, repeat, cached_lod_swap, cached_region_reuse
    };
    explicit AssetReuseHistory(std::size_t limit=4096U): limit_{limit} {}

    [[nodiscard]] Kind uploaded(std::string_view id,std::uint64_t epoch,
        std::uint32_t quality,Tier tier)
    {
        if(id.empty() || limit_==0U)return Kind::first_seen;
        std::string key{id};
        auto found=items_.find(key);
        Kind kind=Kind::first_seen;
        if(found!=items_.end())
        {
            if(tier!=Tier::network&&found->second.epoch!=epoch)
                kind=Kind::cached_region_reuse;
            else if(tier!=Tier::network&&found->second.quality!=quality)
                kind=Kind::cached_lod_swap;
            else kind=Kind::repeat;
            order_.splice(order_.end(),order_,found->second.age);
        }
        else
        {
            if(items_.size()>=limit_)
            {
                items_.erase(order_.front());
                order_.pop_front();
                ++evictions_;
            }
            order_.push_back(key);
            found=items_.emplace(key,Entry{epoch,quality,
                std::prev(order_.end())}).first;
        }
        found->second.epoch=epoch;
        found->second.quality=quality;
        if(kind==Kind::cached_region_reuse)
        {
            ++region_cache_hits_;
            if(tier==Tier::memory)++region_ram_hits_;
            else ++region_disk_hits_;
        }
        else if(kind==Kind::cached_lod_swap)
        {
            ++lod_cache_hits_;
            if(tier==Tier::memory)++lod_ram_hits_;
            else ++lod_disk_hits_;
        }
        return kind;
    }
    // Compatibility overload for older tests/callers. Prefer Tier.
    [[nodiscard]] Kind uploaded(std::string_view id,std::uint64_t epoch,
        std::uint32_t quality,bool cpu_cache_hit)
    {
        return uploaded(id,epoch,quality,
            cpu_cache_hit?Tier::memory:Tier::network);
    }
    [[nodiscard]] std::size_t region_cache_hits() const noexcept
    {return region_cache_hits_;}
    [[nodiscard]] std::size_t lod_cache_hits() const noexcept
    {return lod_cache_hits_;}
    [[nodiscard]] std::size_t region_ram_hits() const noexcept
    {return region_ram_hits_;}
    [[nodiscard]] std::size_t region_disk_hits() const noexcept
    {return region_disk_hits_;}
    [[nodiscard]] std::size_t lod_ram_hits() const noexcept
    {return lod_ram_hits_;}
    [[nodiscard]] std::size_t lod_disk_hits() const noexcept
    {return lod_disk_hits_;}
    [[nodiscard]] std::size_t evictions() const noexcept
    {return evictions_;}
    [[nodiscard]] std::size_t tracked() const noexcept{return items_.size();}
private:
    struct Entry
    {
        std::uint64_t epoch=0U;
        std::uint32_t quality=0U;
        std::list<std::string>::iterator age;
    };
    std::size_t limit_;
    std::size_t region_cache_hits_=0U;
    std::size_t lod_cache_hits_=0U;
    std::size_t region_ram_hits_=0U;
    std::size_t region_disk_hits_=0U;
    std::size_t lod_ram_hits_=0U;
    std::size_t lod_disk_hits_=0U;
    std::size_t evictions_=0U;
    std::list<std::string> order_;
    std::unordered_map<std::string,Entry> items_;
};
} // namespace nexverse::assets
