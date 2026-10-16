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
    // The viewer must not offer a manual HTTP storm: four automatic
    // attempts, a real 30s final backoff, at most two explicit restarts.
    TerrainDetailRetry manual{};
    if(!manual.enter(7,12))return 12;
    manual.succeeded(0);
    if(manual.manual_allowed(ids,t,false,true))return 13;
    manual.dispatched(t);
    manual.dispatched(t+std::chrono::seconds{5});
    manual.dispatched(t+std::chrono::seconds{20});
    manual.dispatched(t+std::chrono::seconds{40});
    if(manual.manual_allowed(ids,t+std::chrono::seconds{69},false,true))
        return 14;
    if(manual.retry_wait_seconds(t+std::chrono::seconds{69})!=1U)
        return 15;
    if(manual.manual_allowed(ids,t+std::chrono::seconds{71},true,true)||
       manual.manual_allowed(ids,t+std::chrono::seconds{71},false,false))
        return 16;
    if(!manual.restart_manually(ids,t+std::chrono::seconds{71},
                                false,true)||
       manual.loaded_count()!=1U||manual.manual_restarts!=1U||
       !manual.ready(ids,t+std::chrono::seconds{71}))return 17;
    const auto remaining=manual.missing(ids);
    if(remaining[0]||!remaining[1]||!remaining[2]||!remaining[3])
        return 18;
    manual.dispatched(t+std::chrono::seconds{71});
    manual.dispatched(t+std::chrono::seconds{74});
    manual.dispatched(t+std::chrono::seconds{79});
    manual.dispatched(t+std::chrono::seconds{86});
    if(!manual.restart_manually(ids,t+std::chrono::seconds{117},
                                false,true))return 19;
    manual.dispatched(t+std::chrono::seconds{118});
    manual.dispatched(t+std::chrono::seconds{122});
    manual.dispatched(t+std::chrono::seconds{128});
    manual.dispatched(t+std::chrono::seconds{145});
    if(manual.restart_manually(ids,t+std::chrono::seconds{177},
                               false,true)||
       manual.manual_restarts!=2U)return 20;
    if(!manual.enter(7,13)||manual.manual_restarts!=0U||
       manual.loaded_count()!=0U||!manual.ready(ids,t))return 21;
    std::cout<<"Terrain retry: epoch-safe, 4 auto + max 2 manual, missing-only PASS\n";
    return 0;
}
