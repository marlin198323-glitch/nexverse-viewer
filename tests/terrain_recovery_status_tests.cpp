/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/terrain_recovery_status.hpp"
#include <iostream>
int main()
{
    using namespace nexverse;
    assets::TerrainDetailRetry retry{};
    if(ui::terrain_recovery_status(retry,0,false,false).find("wartet")==
       std::string::npos)return 1;
    (void)retry.enter(2,3);
    if(ui::terrain_recovery_status(retry,0,true,false).find("bereit")==
       std::string::npos)return 2;
    const auto now=assets::TerrainDetailRetry::Clock::now();
    retry.dispatched(now);
    if(ui::terrain_recovery_status(retry,1,true,true).find("Versuch 1/4")==
       std::string::npos)return 3;
    if(ui::terrain_recovery_status(retry,4,true,true).find("4/4")==
       std::string::npos)return 4;
    retry.dispatched(now);
    retry.dispatched(now);
    retry.dispatched(now);
    if(ui::terrain_recovery_status(retry,2,true,true).find("Retry-Limit")==
       std::string::npos)return 5;
    std::cout<<"Terrain recovery state is grounded in GPU draws and real retry attempts PASS\n";
    return 0;
}
