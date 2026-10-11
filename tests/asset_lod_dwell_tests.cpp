#include "nexverse/assets/asset_lod_dwell.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    AssetLodDwell<int> lod{milliseconds{1200}};
    const auto t=AssetLodDwell<int>::Clock::time_point{seconds{100}};
    const std::unordered_map<std::string,int> loaded{{"a",2}};
    if(lod.resolve("new",5,loaded,t)!=5)return 1;
    if(lod.resolve("a",2,loaded,t)!=2 || lod.waiting()!=0)return 2;
    if(lod.resolve("a",1,loaded,t)!=2 || lod.waiting()!=1)return 3;
    if(lod.resolve("a",1,loaded,t+milliseconds{1199})!=2)return 4;
    if(lod.resolve("a",1,loaded,t+milliseconds{1200})!=1 ||
       lod.waiting()!=0)return 5;
    if(lod.resolve("a",3,loaded,t+milliseconds{1300})!=2)return 6;
    // Oscillation to another quality resets the timer.
    if(lod.resolve("a",1,loaded,t+milliseconds{2400})!=2 ||
       lod.resolve("a",1,loaded,t+milliseconds{3500})!=2 ||
       lod.resolve("a",1,loaded,t+milliseconds{3600})!=1)return 7;
    (void)lod.resolve("a",3,loaded,t+milliseconds{3800});
    lod.prune({{"other",1}});
    if(lod.waiting()!=0)return 8;
    (void)lod.resolve("a",3,loaded,t+milliseconds{4100});
    lod.reset();
    if(lod.waiting()!=0)return 9;
    std::cout<<"LOD dwell, anti-oscillation, new assets and epoch resets OK\n";
    return 0;
}
