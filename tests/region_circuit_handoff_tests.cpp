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
    std::cout<<"LLUDP region handoff policy and stale-source guards passed\n";
    return 0;
}
