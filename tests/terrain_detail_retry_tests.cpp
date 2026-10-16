/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/terrain_detail_retry.hpp"
#include <iostream>
int main()
{
    using nexverse::assets::TerrainDetailRetry;
    TerrainDetailRetry r{};
    const std::array<std::string,4> ids{"g1","g2","g3","g4"};
    const auto t=TerrainDetailRetry::Clock::time_point{}+
        std::chrono::hours{10};
    if(r.ready(ids,t))return 1;
    if(!r.enter(3,5))return 2;
    if(!r.ready(ids,t))return 3;
    r.dispatched(t);
    r.succeeded(0);
    r.succeeded(2);
    if(r.ready(ids,t+std::chrono::seconds{1}))return 4;
    auto pending=r.missing(ids);
    if(pending[0]||!pending[1]||pending[2]||!pending[3])return 5;
    if(!r.ready(ids,t+std::chrono::seconds{2}))return 6;
    r.dispatched(t+std::chrono::seconds{2});
    r.succeeded(1);
    r.succeeded(3);
    if(r.loaded_count()!=4U||r.ready(ids,t+std::chrono::hours{2}))
        return 7;
    if(r.enter(3,5)||r.loaded_count()!=4U)return 8;
    if(!r.enter(3,6)||r.loaded_count()!=0U||
       !r.ready(ids,t))return 9;
    const std::array<std::string,4> emptyids{"","","",""};
    if(r.ready(emptyids,t))return 10;
    r.dispatched(t);
    r.dispatched(t+std::chrono::seconds{5});
    r.dispatched(t+std::chrono::seconds{20});
    r.dispatched(t+std::chrono::seconds{40});
    if(r.ready(ids,t+std::chrono::hours{10}))return 11;
    std::cout<<"Terrain retry: partial layers retained, bounded retries, region epoch isolation PASS\n";
    return 0;
}
