/* MPL-2.0: simulator AgentMovementComplete LookAt, never a forged BodyRotation. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/agent_movement_look_at.hpp"
#include "nexverse/protocol/movement_controls.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>
using namespace nexverse;
using namespace nexverse::world;
using namespace nexverse::protocol;
static constexpr auto self="11111111-2222-3333-4444-555555555555";
static constexpr auto sess="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static constexpr std::uint64_t region=0x0000010000000200ULL;
static RegionUpdate area(bool connected)
{
    return {region,"NexVerse",{},"127.0.0.1",9000U,
            "","",0U,0U,0U,13U,0.0f,"",256U,256U,connected};
}
int main()
{
    const auto invalid=[](Vec3 look){return !simulator_look_at_heading(look);};
    if(!invalid({0.0f,0.0f,0.0f})||
       !invalid({0.0f,0.0f,1.0f})||
       !invalid({0.00000001f,0.0f,0.0f})||
       !invalid({std::numeric_limits<float>::quiet_NaN(),1.0f,0.0f})||
       !invalid({0.0f,std::numeric_limits<float>::infinity(),0.0f}))
        return 1;
    const auto east=simulator_look_at_heading({1.0f,0.0f,0.0f});
    const auto north=simulator_look_at_heading({0.0f,3.0f,0.0f});
    const auto west=simulator_look_at_heading({-1.0f,0.0f,0.0f});
    if(!east||!north||!west||
       std::abs(east->z)>0.001f||std::abs(east->w-1.0f)>0.001f||
       std::abs(north->z-0.70710678f)>0.001f||
       std::abs(north->w-0.70710678f)>0.001f||
       std::abs(west->z-1.0f)>0.001f||
       std::abs(west->w)>0.001f)
        return 2;
    WorldModel world;
    world.apply_region(area(true));
    world.apply_agent({self,sess,{},{},true});
    const Vec3 initial{128.0f,124.0f,30.0f};
    if(world.accept_simulator_self_position("other",sess,region,initial)||
       world.accept_simulator_self_position(self,"wrong",region,initial)||
       world.accept_simulator_self_position(self,sess,region+1,initial))
        return 3;
    Vec3 invalid_position=initial;
    invalid_position.x=std::numeric_limits<float>::quiet_NaN();
    if(world.accept_simulator_self_position(self,sess,region,invalid_position))
        return 4;
    // Actual valid simulator location, but LookAt=0: the server has NOT
    // given a heading quaternion, so do not let High4 issue WASD.
    if(!world.accept_simulator_self_position(self,sess,region,initial)||
       world.agent().position.x!=128.0f||
       world.self_avatar_controls_ready()||
       world.simulator_self_pose_confirmed())return 5;
    const Vec3 next{128.0f,130.0f,31.0f};
    if(!world.accept_simulator_self_pose(self,sess,region,next,*north)||
       !world.self_avatar_controls_ready()||
       world.agent().rotation.z<0.707f||
       world.agent().rotation.w<0.707f)return 6;
    // High4 WASD now uses the server's +Y heading (pi/2 yaw).
    MovementController controls;
    controls.reset(world.agent().rotation);
    MovementKeys wasd{};wasd.forward=true;
    const auto update=controls.step(wasd,0.1f);
    if((update.control_flags&control_at_pos)==0U||
       std::abs(update.body_rotation.z-0.70710678f)>0.001f)
        return 7;
    // Later server movement with invalid LookAt can refresh *position*
    // without throwing away a separately verified, current BodyRotation.
    const Vec3 after{129.0f,131.0f,31.0f};
    if(!world.accept_simulator_self_position(self,sess,region,after)||
       !world.self_avatar_controls_ready()||
       world.agent().position.x!=129.0f||
       std::abs(world.agent().rotation.z-0.70710678f)>0.001f)
        return 8;
    world.apply_region(area(false));
    if(world.self_avatar_controls_ready()||
       world.accept_simulator_self_position(self,sess,region+1,after))
        return 9;
    world.apply_region(area(true));
    world.apply_agent({self,sess,{},{},true});
    if(world.self_avatar_controls_ready())return 10;
    if(!world.accept_simulator_self_position(self,sess,region,after)||
       world.self_avatar_controls_ready())return 11;
    world.reset();
    if(world.simulator_self_pose_confirmed())return 12;
    std::cout<<"OGL_LO158_MOVEMENT_COMPLETE_LOOKAT_NOT_FABRICATED_YAW_PASS\n";
}
