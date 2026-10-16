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
    using namespace std::chrono_literals;
    if(world_entry_fraction(WorldEntryStage::login)!=0.04f||
       world_entry_fraction(WorldEntryStage::agent_position)!=0.88f||
       world_entry_fraction(WorldEntryStage::ready)!=1.0f)return 9;
    if(world_entry_stage_key(WorldEntryStage::agent_identity)!="identity_pair")
        return 10;
    WorldEntryWaitMonitor timer{};
    const auto now=std::chrono::steady_clock::now();
    if(timer.seconds(WorldEntryStage::simulator_circuit,now)!=0U||
       timer.seconds(WorldEntryStage::simulator_circuit,now+16s)!=16U||
       timer.seconds(WorldEntryStage::agent_position,now+18s)!=0U||
       timer.seconds(WorldEntryStage::agent_position,now+38s)!=20U)
        return 11;
    if(world_entry_wait_hint(WorldEntryStage::ready,60)!=""||
       world_entry_wait_hint(WorldEntryStage::agent_position,10)!=""||
       world_entry_wait_hint(WorldEntryStage::agent_position,16).empty())
        return 12;
    std::cout<<"Login -> circuit -> region -> agent ID -> agent position stages PASS\n";
    return 0;
}
