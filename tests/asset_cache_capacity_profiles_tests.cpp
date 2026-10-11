#include "nexverse/assets/texture_memory_cache.hpp"
#include "nexverse/assets/mesh_cache.hpp"
#include "nexverse/assets/asset_cache_pressure_window.hpp"
#include <array>
#include <cstdio>
#include <iostream>
#include <string>
namespace
{
std::string id(unsigned i)
{
    char buf[40]{};
    std::snprintf(buf,sizeof(buf),"88888888-8888-8888-8888-%012x",i);
    return {buf};
}
struct Summary
{
    std::size_t hits=0U,misses=0U,evictions=0U;
};
Summary profile_texture(std::size_t slots)
{
    using namespace nexverse::assets;
    constexpr std::size_t bytes_per_image=8U*8U*4U;
    TextureMemoryCache cache{slots*bytes_per_image};
    const ImageRgba8 image{8U,8U,
        std::vector<std::uint8_t>(bytes_per_image,240U)};
    AssetCachePressureWindow pressure;
    pressure.observe(cache.stats());
    for(unsigned phase=0U;phase<6U;++phase)
    {
        // 96 active UUIDs, every third visit is a common scene asset.
        for(unsigned k=0U;k<192U;++k)
        {
            const unsigned key=k%3U==0U ? 0U :
                1U+((k+phase*13U)%95U);
            if(!cache.load(id(key)))
                (void)cache.store(id(key),image);
        }
        pressure.observe(cache.stats());
    }
    const auto s=cache.stats();
    if(s.bytes>slots*bytes_per_image||s.entries>slots)
        return {0U,0U,999999U};
    return {s.hits,s.misses,s.evictions};
}
Summary profile_mesh(std::size_t slots)
{
    using namespace nexverse::assets;
    const MeshGeometry mesh{
        {2,0,0,0,0,1,0,0,2,1,0,0,0,1,1,0,2,0,1,0,0,1,0,1},
        {0U,1U,2U},{{0U,0U,3U}}};
    MeshMemoryCache cache{slots*mesh_byte_size(mesh)};
    AssetCachePressureWindow pressure;
    pressure.observe(cache.stats());
    for(unsigned phase=0U;phase<6U;++phase)
    {
        for(unsigned k=0U;k<192U;++k)
        {
            const unsigned key=k%3U==0U?0U:
                1U+((k+phase*13U)%95U);
            if(!cache.load(id(key)))
                (void)cache.store(id(key),mesh);
        }
        pressure.observe(cache.stats());
    }
    const auto s=cache.stats();
    if(s.bytes>slots*mesh_byte_size(mesh)||s.entries>slots)
        return {0U,0U,999999U};
    return {s.hits,s.misses,s.evictions};
}
}
int main()
{
    for(auto slots:std::array<std::size_t,3U>{8U,64U,128U})
    {
        const auto t=profile_texture(slots);
        const auto m=profile_mesh(slots);
        std::cout<<"CACHE_PROFILE slots="<<slots
            <<" texture_hits="<<t.hits<<" texture_misses="<<t.misses
            <<" texture_evictions="<<t.evictions
            <<" mesh_hits="<<m.hits<<" mesh_misses="<<m.misses
            <<" mesh_evictions="<<m.evictions<<'\n';
        if(t.hits==0U||m.hits==0U||t.misses==0U||m.misses==0U)
            return 1;
        if(t.evictions==999999U || m.evictions==999999U)return 2;
        if(slots==8U && (t.evictions<100U||m.evictions<100U))
            return 3;
        if(slots>=128U && (t.evictions!=0U||m.evictions!=0U))
            return 4;
    }
    return 0;
}
