/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_memory_cache_stats.hpp"
#include <cstddef>
#include <cstdint>
namespace nexverse::assets
{
// Compare two independent cache sampling points. Avoid cumulative hit rates
// hiding an active cache-thrash episode. An observed pressure window pauses
// discretionary disk-only RAM prefetch, never required scene downloads.
class AssetCachePressureWindow
{
public:
    void reset() noexcept {*this={};}
    void observe(const AssetMemoryCacheStats& current) noexcept
    {
        if(!sampled_ || current.hits<prior_.hits ||
           current.misses<prior_.misses ||
           current.evictions<prior_.evictions)
        {
            prior_=current;
            sampled_=true;
            hit_pct_=100U;
            evictions_=0U;
            pressure_=false;
            healthy_windows_=0U;
            return;
        }
        const auto hits=current.hits-prior_.hits;
        const auto misses=current.misses-prior_.misses;
        evictions_=current.evictions-prior_.evictions;
        const auto total=hits+misses;
        // Numerically safe; avoid hits*100 overflow by dividing to double.
        hit_pct_=total
            ? static_cast<std::uint32_t>(
                static_cast<double>(hits)*100.0/
                static_cast<double>(total))
            : 100U;
        const bool stressed=misses>=8U && evictions_>=2U &&
            hit_pct_<50U;
        if(stressed)
        {
            pressure_=true;
            healthy_windows_=0U;
        }
        else if(pressure_)
        {
            // Avoid 30-second on/off prefetch oscillation on a single
            // quiet sampling interval after sustained RAM thrashing.
            if(++healthy_windows_>=2U)
            {
                pressure_=false;
                healthy_windows_=0U;
            }
        }
        prior_=current;
    }
    [[nodiscard]] bool under_pressure() const noexcept{return pressure_;}
    [[nodiscard]] std::uint32_t hit_pct() const noexcept{return hit_pct_;}
    [[nodiscard]] std::size_t interval_evictions() const noexcept
    {return evictions_;}
private:
    AssetMemoryCacheStats prior_{};
    bool sampled_=false;
    bool pressure_=false;
    std::uint32_t hit_pct_=100U;
    std::size_t evictions_=0U;
    unsigned healthy_windows_=0U;
};
}
