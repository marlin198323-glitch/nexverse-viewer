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
    if(ui::terrain_recovery_layers(retry,{"id1","id2","","id4"}).find(
           "L3:nicht gesetzt")==std::string::npos)return 6;
    if(ui::terrain_recovery_layers(retry,{"id1","id2","","id4"}).find(
           "L1:fehlt")==std::string::npos)return 7;
    retry.succeeded(0);
    if(ui::terrain_recovery_layers(retry,{"id1","id2","","id4"}).find(
           "L1:geladen")==std::string::npos)return 8;
    if(ui::terrain_recovery_status(retry,0,false,false,false,false).find(
           "CAP fehlt")==std::string::npos)return 9;
    if(ui::terrain_recovery_status(retry,0,false,false,true,true).find(
           "Download aktiv")==std::string::npos)return 10;
    if(ui::terrain_recovery_status(retry,0,false,false,false,true,
           now+std::chrono::seconds{60}).find(
           "manuell [T]")==std::string::npos)return 11;
    std::cout<<"Terrain recovery UI: per-layer, CAP, in-flight, cooldown PASS\n";
    return 0;
}
