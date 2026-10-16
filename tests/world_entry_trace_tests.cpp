/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/world_entry_trace.hpp"
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;
int main()
{
    using namespace nexverse::ui;
    const auto t=std::chrono::steady_clock::time_point{100s};
    WorldEntryTrace trace;
    if(trace.size()!=0U)return 1;
    trace.observe(WorldEntryStage::login,t);
    trace.observe(WorldEntryStage::login,t+22s);
    if(trace.size()!=1||trace.at(0).dwell_seconds!=22U)return 2;
    trace.observe(WorldEntryStage::simulator_circuit,t+23s);
    trace.observe(WorldEntryStage::region_handle,t+40s);
    trace.observe(WorldEntryStage::region_connection,t+44s);
    trace.observe(WorldEntryStage::agent_identity,t+47s);
    trace.observe(WorldEntryStage::agent_position,t+51s);
    if(trace.size()!=5||trace.at(0).stage!=
        WorldEntryStage::simulator_circuit||
       trace.at(4).stage!=WorldEntryStage::agent_position)return 3;
    trace.observe(WorldEntryStage::agent_position,t+4100s);
    if(trace.at(4).dwell_seconds!=3600U)return 4;
    trace.observe(WorldEntryStage::simulator_circuit,t+4101s);
    if(trace.at(4).stage!=WorldEntryStage::simulator_circuit||
       trace.at(3).stage!=WorldEntryStage::agent_position)return 5;
    trace.reset();
    if(trace.size()!=0U||trace.at(0).dwell_seconds!=0U)return 6;
    trace.observe(WorldEntryStage::login,t+4200s);
    if(trace.size()!=1||trace.at(0).stage!=WorldEntryStage::login)
        return 7;
    std::cout<<"World entry bounded stage-only trace and reset PASS\n";
    return 0;
}
