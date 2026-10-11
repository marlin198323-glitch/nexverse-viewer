#include "nexverse/assets/asset_cache_pressure_window.hpp"
#include "nexverse/assets/texture_memory_cache.hpp"
#include "nexverse/assets/mesh_cache.hpp"
#include <cstdio>
#include <iostream>
#include <string>
int main()
{
    using namespace nexverse::assets;
    const auto id=[](unsigned n)
    {
        char buffer[40]{};
        std::snprintf(buffer,sizeof(buffer),
            "77777777-7777-7777-7777-%012x",n);
        return std::string{buffer};
    };
    TextureMemoryCache textures{8U};
    MeshMemoryCache meshes{240U};
    const ImageRgba8 pixel{1U,1U,{55U,66U,77U,255U}};
    const MeshGeometry tri{
        {2,0,0,0,0,1,0,0,2,1,0,0,0,1,1,0,2,0,1,0,0,1,0,1},
        {0U,1U,2U},{{0U,0U,3U}}};
    AssetCachePressureWindow tp,mp;
    tp.observe(textures.stats());
    mp.observe(meshes.stats());
    // Active region traverses >20 new scene textures/meshes while RAM
    // budget only retains 2 each. Exercise the ACTUAL caches.
    for(unsigned i=1U;i<=24U;++i)
    {
        const auto key=id(i);
        if(textures.load(key)||meshes.load(key))return 1;
        if(!textures.store(key,pixel)||!meshes.store(key,tri))
            return 2;
    }
    const auto tstats=textures.stats(),mstats=meshes.stats();
    if(tstats.misses!=24U||mstats.misses!=24U ||
       tstats.evictions<20U||mstats.evictions<20U)return 3;
    tp.observe(tstats);
    mp.observe(mstats);
    if(!tp.under_pressure()||!mp.under_pressure()||
       tp.hit_pct()!=0U||mp.hit_pct()!=0U)return 4;
    // The same high-priority asset is now repeatedly reused rather than
    // thrashing. Two *observed* stable windows resume optional prefetch.
    for(unsigned sample=0U;sample<2U;++sample)
    {
        for(unsigned i=0U;i<18U;++i)
        {
            if(!textures.load(id(24U))||!meshes.load(id(24U)))
                return 5;
        }
        tp.observe(textures.stats());
        mp.observe(meshes.stats());
        if(sample==0U && (!tp.under_pressure()||
                          !mp.under_pressure()))return 6;
    }
    if(tp.under_pressure()||mp.under_pressure() ||
       tp.hit_pct()!=100U ||mp.hit_pct()!=100U)return 7;
    // A region reset starts a new pressure sampling epoch, without
    // destroying valid CPU cache contents.
    tp.reset();
    mp.reset();
    tp.observe(textures.stats());
    mp.observe(meshes.stats());
    if(tp.under_pressure()||mp.under_pressure() ||
       !textures.load(id(24U))||!meshes.load(id(24U)))return 8;
    std::cout<<"Real texture/mesh LRU thrashing, two-window recovery, "
        "region pressure reset with resident CPU cache intact OK\n";
    return 0;
}
