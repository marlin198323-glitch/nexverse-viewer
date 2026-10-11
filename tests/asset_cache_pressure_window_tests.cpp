#include "nexverse/assets/asset_cache_pressure_window.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    AssetCachePressureWindow p;
    p.observe({0,0,0,0,0});
    if(p.under_pressure()||p.hit_pct()!=100U)return 1;
    p.observe({2,12,4,900,3});
    if(!p.under_pressure()||p.hit_pct()!=14U||
       p.interval_evictions()!=4U)return 2;
    p.observe({26,14,4,900,3});
    if(!p.under_pressure()||p.hit_pct()!=92U||
       p.interval_evictions()!=0U)return 3;
    p.observe({30,14,4,900,3});
    if(p.under_pressure()||p.hit_pct()!=100U)return 7;
    p.observe({0,0,0,0,0}); // fresh epoch/reset counters
    if(p.under_pressure()||p.hit_pct()!=100U)return 4;
    p.observe({100,2,0,100,3});
    if(p.under_pressure()||p.hit_pct()!=98U)return 5;
    p.reset();
    if(p.under_pressure()||p.interval_evictions()!=0U)return 6;
    std::cout<<"Sampled cache miss/eviction pressure and recovery OK\n";
    return 0;
}
