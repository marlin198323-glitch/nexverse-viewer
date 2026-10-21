/* MPL-2.0: real GPU admission pose and truthful camera projection. */
#include "nexverse/render/avatar_proxy_pose_integrity.hpp"
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include <cmath>
#include <iostream>
#include <limits>

int main()
{
    using namespace nexverse;
    using render::RenderAvatarProxy;
    using render::sanitize_avatar_proxy_pose;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    const float inf=std::numeric_limits<float>::infinity();
    const protocol::Vec3 agent{128.0f,128.0f,25.0f};
    const protocol::Quaternion rotation{0.0f,0.0f,0.0f,1.0f};
    for(const float bad:{nan,inf,-inf,1000000.0f,-1000000.0f})
        for(int axis=0;axis<3;++axis)
        {
            RenderAvatarProxy proxy{};
            proxy.position={agent.x,agent.y,agent.z};
            if(axis==0)proxy.position.x=bad;
            if(axis==1)proxy.position.y=bad;
            if(axis==2)proxy.position.z=bad;
            const auto audit=sanitize_avatar_proxy_pose(proxy);
            if(audit.position_valid||!audit.position_dropped)return 1;
        }
    for(const auto bad: {render::RenderQuaternion{nan,0.0f,0.0f,1.0f},
                        render::RenderQuaternion{inf,0.0f,0.0f,1.0f},
                        render::RenderQuaternion{0.0f,0.0f,0.0f,0.0f}})
    {
        RenderAvatarProxy proxy{};
        proxy.position={agent.x,agent.y,agent.z};
        proxy.rotation=bad;
        const auto audit=sanitize_avatar_proxy_pose(proxy);
        if(!audit.position_valid||audit.position_dropped||
           !audit.rotation_repaired||proxy.rotation.w!=1.0f||
           proxy.rotation.x!=0.0f||proxy.rotation.y!=0.0f||
           proxy.rotation.z!=0.0f)return 2;
    }
    // Finite but huge raw quaternion must not overflow the GL matrix.
    RenderAvatarProxy large{};
    large.position={agent.x,agent.y,agent.z};
    large.rotation={1e30f,0.0f,0.0f,1e30f};
    const auto wide=sanitize_avatar_proxy_pose(large);
    const float qnorm2=large.rotation.x*large.rotation.x+
        large.rotation.w*large.rotation.w;
    if(!wide.position_valid||wide.rotation_repaired||
       !std::isfinite(qnorm2)||std::abs(qnorm2-1.0f)>0.0001f)return 3;
    RenderAvatarProxy drawable{};
    drawable.position={agent.x,agent.y,agent.z+1.0f};
    drawable.rotation={0.0f,0.0f,0.0f,2.0f};
    const auto clean=sanitize_avatar_proxy_pose(drawable);
    if(!clean.position_valid||clean.rotation_repaired||
       std::abs(drawable.rotation.w-1.0f)>0.0001f)return 4;
    const auto camera=render::third_person_camera(agent,rotation,5.5f,0.0f);
    const auto view=ui::inspect_avatar_view(
        camera,{drawable.position.x,drawable.position.y,
                drawable.position.z},3440,1440);
    const auto old_view=ui::inspect_avatar_view(camera,agent,3440,1440);
    if(!view.valid_pose||!view.center_in_frustum||
       !(view.normalized_y<old_view.normalized_y))return 5;
    ui::AvatarPipelineProof proof{};
    proof.world_ready=true;proof.self_present=true;
    proof.self_gpu_pose_dropped=true;
    proof.remote_gpu_pose_dropped=2;
    proof.rotation_gpu_pose_repaired=3;
    if(proof.view_stage().find("GPU-Pose ungueltig")==std::string::npos||
       proof.pose_stage().find("Fremde 2")==std::string::npos||
       proof.pose_stage().find("Rotation 3")==std::string::npos)
        return 6;
    proof.self_gpu_pose_dropped=false;proof.remote_gpu_pose_dropped=0;
    proof.rotation_gpu_pose_repaired=0;
    if(!proof.pose_stage().empty())return 7;
    std::cout<<"OGL_REAL_GPU_AVATAR_POSE_FINITE_QUATERNION_HOVER_PROJECTED_PASS\n";
}