/* MPL-2.0: synthetic authenticated LLUDP-self origin admission, not live grid. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/movement_controls.hpp"
#include "nexverse/protocol/movement_frame_safety.hpp"
#include <iostream>
#include <limits>
using namespace nexverse;
using namespace nexverse::world;
using namespace nexverse::protocol;
static constexpr std::uint64_t region_handle=0x0000010000000200ULL;
static constexpr auto agent="11111111-2222-3333-4444-555555555555";
static constexpr auto session="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static RegionUpdate region(std::uint16_t port,bool connected)
{
    return {region_handle,"NexVerse",{},"127.0.0.1",port,
            "","",0U,0U,0U,13U,0.0f,"",256U,256U,connected};
}
int main()
{
    WorldModel world;
    world.apply_region(region(9000U,true));
    world.apply_agent({agent,session,{},{},true});
    MovementController controller;
    MovementKeys wasd;
    wasd.forward=true;
    // A provisional login-origin (0,0,0) is FINITE and passed the old
    // camera-only safety check, despite NO decoded simulator pose.
    if(!valid_agent_update_position(world.agent().position)||
       world.self_avatar_controls_ready())return 1;
    auto requested_flags=[&]()
    {
        if(!world.self_avatar_controls_ready())return std::uint32_t{0U};
        controller.reset(world.agent().rotation);
        return controller.step(wasd,0.1f).control_flags;
    };
    if(requested_flags()!=0U)return 2;
    if(world.accept_simulator_self_pose("foreign",session,region_handle,
                                        {128.0f,127.0f,20.0f},{}))
        return 3;
    if(world.accept_simulator_self_pose(agent,"foreign",region_handle,
                                        {128.0f,127.0f,20.0f},{}))
        return 4;
    if(world.accept_simulator_self_pose(agent,session,region_handle,
                {std::numeric_limits<float>::quiet_NaN(),127.0f,20.0f},{}))
        return 5;
    if(world.self_avatar_controls_ready()||requested_flags()!=0U)return 6;
    if(!world.accept_simulator_self_pose(agent,session,region_handle,
                                         {128.0f,127.0f,20.0f},{})||
       !world.self_avatar_controls_ready()||
       (requested_flags()&control_at_pos)==0U)return 7;
    world.apply_region(region(9000U,false));
    if(world.self_avatar_controls_ready()||requested_flags()!=0U)return 8;
    world.apply_region(region(9000U,true));
    world.apply_agent({agent,session,{},{},true});
    if(world.self_avatar_controls_ready()||requested_flags()!=0U)return 9;
    if(!world.accept_simulator_self_pose(agent,session,region_handle,
                                         {125.0f,130.0f,20.0f},{}))
        return 10;
    // Endpoint changed with SAME region handle: no stale High4 controls.
    world.apply_region(region(9001U,true));
    if(world.self_avatar_controls_ready()||requested_flags()!=0U)return 11;
    world.apply_agent({agent,session,{},{},true});
    if(world.self_avatar_controls_ready())return 12;
    if(!world.accept_simulator_self_pose(agent,session,region_handle,
                                         {130.0f,130.0f,30.0f},{})||
       !world.self_avatar_controls_ready())return 13;
    // New login identity may not inherit the previous valid rotation.
    world.apply_agent({agent,"different-session",{},{},true});
    if(world.self_avatar_controls_ready()||requested_flags()!=0U)return 14;
    world.reset();
    if(world.self_avatar_controls_ready())return 15;
    std::cout<<"OGL_HIGH4_WASD_WAIT_VERIFIED_SIMULATOR_POSE_PASS\n";
}
