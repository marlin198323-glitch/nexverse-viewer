#include "nexverse/assets/asset_reuse_history.hpp"
#include <iostream>
int main()
{
    using nexverse::assets::AssetReuseHistory;
    using K=AssetReuseHistory::Kind;
    AssetReuseHistory reuse{2U};
    if(reuse.uploaded("asset-A",1U,0U,false)!=K::first_seen)return 1;
    if(reuse.uploaded("asset-A",1U,1U,true)!=K::cached_lod_swap)return 2;
    if(reuse.uploaded("asset-A",2U,1U,true)!=K::cached_region_reuse)return 3;
    if(reuse.uploaded("asset-A",3U,1U,false)!=K::repeat)return 4;
    if(reuse.lod_cache_hits()!=1U||reuse.region_cache_hits()!=1U)return 5;
    (void)reuse.uploaded("asset-B",3U,0U,false);
    (void)reuse.uploaded("asset-C",3U,0U,false);
    if(reuse.tracked()!=2U)return 6;
    AssetReuseHistory zero{0U};
    if(zero.uploaded("one",1U,0U,false)!=K::first_seen ||
       zero.tracked()!=0U)return 7;
    if(reuse.uploaded("",4U,0U,true)!=K::first_seen)return 8;
    AssetReuseHistory provenance{3U};
    using T=AssetReuseHistory::Tier;
    if(provenance.uploaded("first",1U,1U,T::network)!=K::first_seen)
        return 9;
    if(provenance.uploaded("second",1U,0U,T::network)!=K::first_seen)
        return 10;
    if(provenance.uploaded("first",1U,2U,T::disk)!=K::cached_lod_swap)
        return 11;
    if(provenance.uploaded("first",2U,2U,T::memory)!=K::cached_region_reuse)
        return 12;
    if(provenance.uploaded("third",2U,0U,T::network)!=K::first_seen)
        return 13;
    if(provenance.uploaded("fourth",2U,0U,T::network)!=K::first_seen)
        return 14;
    if(provenance.evictions()!=1U || provenance.tracked()!=3U)
        return 15;
    // The LRU must evict untouched "second", not recently accessed "first".
    if(provenance.uploaded("first",3U,2U,T::disk)!=K::cached_region_reuse)
        return 16;
    if(provenance.uploaded("second",3U,0U,T::disk)!=K::first_seen)
        return 17;
    if(provenance.region_ram_hits()!=1U ||
       provenance.region_disk_hits()!=1U ||
       provenance.lod_disk_hits()!=1U ||
       provenance.lod_ram_hits()!=0U)return 18;
    std::cout<<"Bounded CPU cache reuse across LOD/region, not GPU handle reuse OK\n";
    return 0;
}
