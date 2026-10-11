#include "nexverse/assets/mesh_residency.hpp"
#include "nexverse/assets/texture_residency.hpp"
#include "nexverse/assets/asset_gpu_upload_gate.hpp"
#include "nexverse/assets/asset_scene_priority_feedback.hpp"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
int main()
{
    using namespace nexverse::assets;
    std::vector<std::string> texture_ids;
    texture_ids.reserve(700U);
    for(std::size_t i=0;i<700U;++i)
        texture_ids.push_back("texture-"+std::to_string(i));
    auto plan=plan_texture_residency({},texture_ids,{},512U);
    if(plan.desired_ids.size()!=512U || !plan.eviction_ids.empty())
        return 1;
    std::unordered_set<std::string> resident=plan.desired_ids;
    for(int step=0;step<80;++step)
    {
        const auto urgent="particle-"+std::to_string(step);
        const auto next=plan_texture_residency(
            {urgent},texture_ids,resident,512U);
        if(next.desired_ids.size()!=512U ||
           next.preferred_ids.front()!=urgent ||
           next.eviction_ids.size()!=1U)
            return 2;
        // A full slot table must never evict the high-priority particle.
        const auto admission=plan_texture_slot_admission(
            next.preferred_ids,resident,next.eviction_ids,urgent,512U);
        if(!admission.fits || !admission.additional_evictions.empty())
            return 3;
        // Regression: the request must be allowed BEFORE resident
        // eviction, or the viewer stalls at its 512-slot limit.
        if(step==0)
        {
            const auto preflight=plan_texture_slot_admission(
                next.preferred_ids,resident,next.eviction_ids,
                urgent,512U);
            if(!preflight.fits)return 12;
            const auto forbidden=plan_texture_slot_admission(
                {"higher-priority","lower-priority"},
                {"higher-priority"},{},"lower-priority",1U);
            if(forbidden.fits)return 13;
        }
        for(const auto& id:next.eviction_ids)resident.erase(id);
        resident.insert(urgent);
        if(resident.size()!=512U)return 4;
        if(!next.desired_ids.contains(urgent))return 5;
        // Reconcile back to non-particle plan before the next burst.
        const auto restore=plan_texture_residency(
            {},texture_ids,resident,512U);
        for(const auto& id:restore.eviction_ids)resident.erase(id);
        for(const auto& id:restore.preferred_ids)resident.insert(id);
        if(resident.size()!=512U)return 6;
    }
    // Camera-priority feedback remains a soft bonus under a full mesh cap.
    std::vector<MeshDistanceSample> samples;
    for(int i=0;i<400;++i)
        samples.push_back({"mesh-"+std::to_string(i),
            static_cast<float>(i),0.0f,0.0f,1.0f});
    auto mesh=plan_mesh_residency(samples,0.0f,0.0f,0.0f,{},256U);
    if(mesh.desired_ids.size()!=256U)return 7;
    std::unordered_set<std::string> gpu=mesh.desired_ids;
    AssetScenePriorityFeedback feedback;
    feedback.observe(mesh.desired_ids);
    for(int x=1;x<=30;++x)
    {
        mesh=plan_mesh_residency(samples,static_cast<float>(x),
            0.0f,0.0f,gpu,256U,feedback.loaded_bonus(24.0f));
        feedback.observe(mesh.desired_ids);
        if(mesh.desired_ids.size()!=256U)return 8;
        for(const auto& id:mesh.eviction_ids)
        {
            if(mesh.desired_ids.contains(id))return 9;
            gpu.erase(id);
        }
        for(const auto& id:mesh.prioritized_ids)gpu.insert(id);
        if(gpu.size()!=256U)return 10;
    }
    mesh=plan_mesh_residency(samples,399.0f,0.0f,0.0f,
        gpu,256U,feedback.loaded_bonus(24.0f));
    if(!mesh.desired_ids.contains("mesh-399") ||
       mesh.desired_ids.contains("mesh-0"))
        return 11;
    std::cout<<"Full texture/mesh residency under camera churn and urgent particle slots OK\n";
    return 0;
}
