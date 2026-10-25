/* This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. */
#include "nexverse/render/self_avatar_visibility_pose_gate.hpp"
#include <iostream>
#include <optional>
using nexverse::render::SelfAvatarVisibilityPoseGate;
int main()
{
    using Pose=SelfAvatarVisibilityPoseGate::Pose;
    SelfAvatarVisibilityPoseGate gate;
    const Pose start{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
    if(!gate.observe(start)||gate.observe(start))return 1;
    // Ordinary network jitter must not cause a permanent unknown.
    if(gate.observe(Pose{3.01f,0,0,0,0,0,1})||
       gate.observe(Pose{3.03f,0,0,0,0,0,1}))return 2;
    // Position comparison anchors to previous invalidation, not each
    // most recent frame; slow locomotion eventually retires old proof.
    if(!gate.observe(Pose{3.07f,0,0,0,0,0,1}))return 3;
    if(gate.observe(Pose{3.07f,0,0,0,0,0,1}))return 4;
    // q and -q describe the same quaternion rotation.
    if(gate.observe(Pose{3.07f,0,0,0,0,0,-1}))return 5;
    // Significantly different yaw invalidates at an identical position.
    if(!gate.observe(Pose{3.07f,0,0,0,0,0.08715574f,0.9961947f}))
        return 6;
    if(gate.observe(Pose{3.07f,0,0,0,0,-0.08715574f,-0.9961947f}))
        return 7;
    // Current Self disappears on logout/region handoff; proof resets.
    if(!gate.observe(std::nullopt)||gate.observe(std::nullopt))
        return 8;
    if(!gate.observe(start)||gate.observe(start))return 9;
    gate.reset();
    if(!gate.observe(start))return 10;
    std::cout<<"OGL_SELF_AVATAR_VISIBILITY_POSE_GATE_PASS\n";
}
