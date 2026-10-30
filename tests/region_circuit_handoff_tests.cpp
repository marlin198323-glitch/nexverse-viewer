#include "nexverse/protocol/region_circuit_handoff.hpp"
#include <iostream>
using namespace nexverse::protocol;
int main()
{
    if(region_handoff_action("127.0.0.1",9000,
       "127.0.0.1",9000,false)!=RegionHandoffAction::unchanged)return 1;
    if(region_handoff_action("127.0.0.1",9000,
       "127.0.0.2",9000,false)!=
       RegionHandoffAction::wait_for_old_worker)return 2;
    if(region_handoff_action("127.0.0.1",9000,
       "127.0.0.2",9000,true)!=
       RegionHandoffAction::launch_new_worker)return 3;
    if(region_handoff_action("127.0.0.1",9000,
       "",9000,true)!=RegionHandoffAction::invalid_destination)return 4;
    if(region_handoff_action("127.0.0.1",9000,
       "127.0.0.2",0,true)!=RegionHandoffAction::invalid_destination)return 5;
    if(region_handoff_action("127.0.0.1",9000,
       std::string_view{"12345678901234567890"}.substr(0,20),0,true)!=
       RegionHandoffAction::invalid_destination)return 6;
    if(!region_packet_matches("127.0.0.2",9000,
       "127.0.0.2",9000,false))return 7;
    if(region_packet_matches("127.0.0.1",9000,
       "127.0.0.2",9000,false))return 8;
    if(region_packet_matches("127.0.0.2",9000,
       "127.0.0.2",9000,true))return 9;
    if(region_packet_matches("127.0.0.2",9000,
       "127.0.0.2",8000,false))return 10;
    // After a successful new circuit to region B, the original
    // login-region endpoint A must no longer gate simulation READY.
    if(!region_circuit_ready("127.0.0.2",9001,
          "127.0.0.2",9001,true,false))return 11;
    if(region_circuit_ready("127.0.0.1",9000,
          "127.0.0.2",9001,true,false))return 12;
    if(region_circuit_ready("127.0.0.2",9001,
          "127.0.0.2",9001,false,false))return 13;
    if(region_circuit_ready("127.0.0.2",9001,
          "127.0.0.2",9001,true,true))return 14;
    // Same IP:port is not the same simulator incarnation. After
    // TeleportFinish or CrossedRegion an explicit pending region
    // transition must open a NEW authenticated LLUDP worker, even
    // when the destination server reuses its UDP socket.
    const RegionCircuitKey region_a{
        "192.0.2.10",9000U,0x1234000056780000ULL,5U};
    const RegionCircuitKey region_b{
        "192.0.2.10",9000U,0x1235000056780000ULL,6U};
    const RegionCircuitKey same_region_new_teleport{
        "192.0.2.10",9000U,0x1234000056780000ULL,6U};
    if(region_handoff_action(region_a,region_b,false,true)!=
       RegionHandoffAction::wait_for_old_worker)return 15;
    if(region_handoff_action(region_a,region_b,true,true)!=
       RegionHandoffAction::launch_new_worker)return 16;
    if(region_handoff_action(region_a,same_region_new_teleport,false,true)!=
       RegionHandoffAction::wait_for_old_worker)return 17;
    if(region_handoff_action(region_a,same_region_new_teleport,true,true)!=
       RegionHandoffAction::launch_new_worker)return 18;
    // A transient circuit disconnect is not a new TeleportFinish:
    // its generation may change, but MUST use bounded manual/backoff
    // recovery rather than repeatedly spawning new workers.
    if(region_handoff_action(region_a,same_region_new_teleport,true,false)!=
       RegionHandoffAction::unchanged)return 19;
    if(region_handoff_action(region_a,region_a,true,true)!=
       RegionHandoffAction::unchanged)return 20;
    if(region_handoff_action(region_a,
        {"",9000U,region_b.handle,region_b.generation},true,true)!=
       RegionHandoffAction::invalid_destination)return 21;
    if(region_circuit_ready(region_a,region_b,true,false))return 22;
    if(region_circuit_ready(region_a,same_region_new_teleport,true,false))
       return 23;
    if(!region_circuit_ready(region_b,region_b,true,false))return 24;
    if(region_packet_matches(region_a,region_b,false))return 25;
    if(region_packet_matches(region_a,same_region_new_teleport,false))
       return 26;
    if(!region_packet_matches(region_b,region_b,false))return 27;
    if(region_packet_matches(region_b,region_b,true))return 28;
    // Different endpoint always demands the same existing safe
    // old-worker stop / join / queue-drain sequence.
    if(region_handoff_action(region_a,
        {"192.0.2.11",9000U,region_a.handle,region_a.generation},
        false,false)!=RegionHandoffAction::wait_for_old_worker)
        return 29;
    if(region_handoff_action(region_a,
        {"192.0.2.11",9000U,region_a.handle,region_a.generation},
        true,false)!=RegionHandoffAction::launch_new_worker)
        return 30;
    std::cout<<"LLUDP region handoff policy and stale-source guards passed\n";
    return 0;
}
