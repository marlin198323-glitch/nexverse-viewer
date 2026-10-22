/* MPL-2.0. Only genuine authenticated simulator self pose is renderable. */
#include "nexverse/world/world_model.hpp"
#include <iostream>
#include <limits>
using namespace nexverse;
using namespace nexverse::world;
static constexpr std::uint64_t region=0x0000010000000200ULL;
static constexpr std::uint64_t region2=0x0000030000000400ULL;
static constexpr auto id="11111111-2222-3333-4444-555555555555";
static constexpr auto sess="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static RegionUpdate current(std::uint64_t handle,bool online)
{
    return {handle,"NexVerse",{},"127.0.0.1",9000U,"",
        "",0U,0U,0U,13U,0.0f,"",256U,256U,online};
}
int main()
{
    WorldModel model;
    model.apply_region(current(region,true));
    model.apply_agent({id,sess,{},{},true});
    // Circuit up: world and terrain may stream but never draw a fake
    // body at the yet unknown login spawn (zero coordinates).
    if(!model.scene_ready()||model.self_avatar_renderable()||
       model.simulator_self_pose_confirmed())return 1;
    const protocol::Vec3 server{128.0f,128.0f,25.0f};
    const protocol::Quaternion normal{};
    if(model.accept_simulator_self_pose("other",sess,region,server,normal)||
       model.accept_simulator_self_pose(id,"other",region,server,normal)||
       model.accept_simulator_self_pose(id,sess,region2,server,normal)||
       model.self_avatar_renderable())return 2;
    auto bad=server;
    bad.x=std::numeric_limits<float>::quiet_NaN();
    if(model.accept_simulator_self_pose(id,sess,region,bad,normal)||
       model.self_avatar_renderable())return 3;
    if(model.accept_simulator_self_pose(id,sess,region,server,
                                        {0.0f,0.0f,0.0f,0.0f})||
       model.self_avatar_renderable())return 4;
    if(!model.accept_simulator_self_pose(id,sess,region,server,normal)||
       !model.self_avatar_renderable()||
       model.agent().position.z!=25.0f)return 5;
    // Identical agent/session refresher is NOT a logout or invalidation.
    model.apply_agent({id,sess,server,normal,true});
    if(!model.self_avatar_renderable())return 6;
    // Own root killed: no surviving ghost.
    if(!model.withdraw_self_object_on_kill(id,region,54U)||
       model.self_avatar_renderable())return 7;
    if(!model.accept_simulator_self_pose(id,sess,region,server,normal)||
       !model.self_avatar_renderable())return 8;
    // CrossedRegion resets proof even if same user+session.
    model.begin_region_transition(current(region2,false));
    model.apply_agent({id,sess,{},normal,false});
    if(model.self_avatar_renderable()||
       model.accept_simulator_self_pose(id,sess,region,server,normal))return 9;
    // AgentMovementComplete may precede network "connected" flip.
    if(!model.accept_simulator_self_pose(id,sess,region2,server,normal)||
       model.self_avatar_renderable()||
       !model.simulator_self_pose_confirmed())return 10;
    model.apply_region(current(region2,true));
    model.apply_agent({id,sess,server,normal,true});
    if(!model.self_avatar_renderable())return 11;
    // Circuit drop must revoke own body's rendering without stale reuse.
    model.apply_region(current(region2,false));
    if(model.simulator_self_pose_confirmed()||model.self_avatar_renderable())
        return 12;
    model.apply_region(current(region2,true));
    model.apply_agent({id,sess,server,normal,true});
    if(model.self_avatar_renderable())return 13;
    if(!model.accept_simulator_self_pose(id,sess,region2,server,normal))
        return 14;
    // A new user/session on same circuit may not inherit earlier proof.
    model.apply_agent({id,"new-session",{},normal,true});
    if(model.self_avatar_renderable()||
       model.accept_simulator_self_pose(id,sess,region2,server,normal))
        return 15;
    if(!model.accept_simulator_self_pose(id,"new-session",region2,
                                         server,normal)||
       !model.self_avatar_renderable())return 16;
    model.reset();
    if(model.self_avatar_renderable()||
       model.simulator_self_pose_confirmed())return 17;
    std::cout<<"OGL_SIMULATOR_SELF_POSE_LIVE_RENDER_ADMISSION_PASS\n";
}
