#include "nexverse/assets/asset_stream_scheduler.hpp"
#include <chrono>
#include <iostream>
#include <unordered_set>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    AssetRequestPacer pace{milliseconds{150}};
    const auto t=AssetRequestPacer::Clock::time_point{milliseconds{1000}};
    if(!pace.ready(t))return 1;
    pace.dispatched(t);
    if(pace.ready(t) || pace.ready(t+milliseconds{149}) ||
       !pace.ready(t+milliseconds{150}))return 2;
    pace.reset();
    if(!pace.ready(t))return 3;
    TexturePrefetchScope scope;
    scope.enter(3);
    if(!scope.current(3) || scope.current(4))return 4;
    const std::vector<std::string> ids{
        "a","a","",std::string{null_texture_asset_id},
        "b","c","d","e","f"};
    const auto first=scope.candidates(ids,{"b"},{"c"},3);
    if(first!=std::vector<std::string>({"a","d","e"}))return 5;
    scope.dispatched("a");scope.dispatched("a");
    if(scope.attempted()!=1U ||
       scope.candidates(ids,{"b"},{"c"},3)!=
           std::vector<std::string>({"d","e","f"}))return 6;
    scope.enter(3);
    if(scope.attempted()!=1U)return 7;
    scope.enter(4);
    if(scope.attempted()!=0U || !scope.current(4) ||
       scope.current(3))return 8;
    if(scope.candidates(ids,{"b"},{"c"},0).size()!=0U)return 9;
    std::cout<<"Asset request pacing, region epochs and bounded prefetch OK\n";
    return 0;
}
