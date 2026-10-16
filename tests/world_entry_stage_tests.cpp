/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/world_entry_stage.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    if(inspect_world_entry(false,true,true,true,true,true)!=
       WorldEntryStage::login)return 1;
    if(inspect_world_entry(true,false,true,true,true,true)!=
       WorldEntryStage::simulator_circuit)return 2;
    if(inspect_world_entry(true,true,false,true,true,true)!=
       WorldEntryStage::region_handle)return 3;
    if(inspect_world_entry(true,true,true,false,true,true)!=
       WorldEntryStage::region_connection)return 4;
    if(inspect_world_entry(true,true,true,true,false,true)!=
       WorldEntryStage::agent_identity)return 5;
    if(inspect_world_entry(true,true,true,true,true,false)!=
       WorldEntryStage::agent_position)return 6;
    if(inspect_world_entry(true,true,true,true,true,true)!=
       WorldEntryStage::ready)return 7;
    if(describe_world_entry(WorldEntryStage::agent_identity)!=
       "Agent-ID oder Session-ID fehlt")return 8;
    std::cout<<"Login -> circuit -> region -> agent ID -> agent position stages PASS\n";
    return 0;
}
