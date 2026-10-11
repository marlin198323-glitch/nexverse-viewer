#include "nexverse/assets/texture_memory_cache.hpp"
#include <iostream>
#include <thread>
#include <vector>
int main()
{
    using nexverse::assets::TextureMemoryCache;
    using nexverse::assets::ImageRgba8;
    constexpr auto a="AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA";
    constexpr auto b="bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    constexpr auto c="cccccccc-cccc-cccc-cccc-cccccccccccc";
    ImageRgba8 red{1,1,{255,0,0,255}};
    ImageRgba8 blue{1,1,{0,0,255,255}};
    TextureMemoryCache cache{8U};
    if(!cache.store(a,red)||!cache.store(b,blue)||
       !cache.load("aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa") ||
       cache.bytes()!=8U||cache.count()!=2U)return 1;
    if(!cache.store(c,blue) || cache.load(b) ||
       !cache.load(a) || !cache.load(c) ||
       cache.bytes()!=8U || cache.count()!=2U)return 2;
    ImageRgba8 bad{2,2,{1,2}};
    if(cache.store("../../../malicious",red) ||
       cache.store(a,bad) || cache.count()!=2U)return 3;
    TextureMemoryCache tiny{3U};
    if(tiny.store(a,red)||tiny.count()!=0U)return 4;
    std::vector<std::thread> workers;
    for(int i=0;i<4;++i)
        workers.emplace_back([&]{
            for(int j=0;j<250;++j)
            {
                (void)cache.store(b,red);
                (void)cache.load(a);
                (void)cache.load(b);
            }
        });
    for(auto& t:workers)t.join();
    if(cache.bytes()>8U || cache.count()>2U)return 5;
    std::cout<<"Texture decoded RAM LRU, validation and concurrency OK\n";
    return 0;
}
