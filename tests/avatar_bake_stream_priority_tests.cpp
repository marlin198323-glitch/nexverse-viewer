/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/avatar_bake_stream_priority.hpp"
#include "nexverse/assets/avatar_bake_gpu_admission.hpp"
#include "nexverse/assets/texture_residency.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

using namespace nexverse::assets;
int main()
{
    using Candidate=AvatarBakeStreamCandidate;
    const std::vector<Candidate> candidates{
        {false,{"near-head","near-upper","near-lower"}, 25.0f,0.0f,0.0f},
        {true,{"self-head","self-upper","self-lower"},0.0f,0.0f,0.0f},
        {false,{"far-head","far-upper","far-lower"},250.0f,0.0f,0.0f},
        {false,{"close-head","self-upper","close-lower"},5.0f,0.0f,0.0f},
        {false,{"bad-head","",""},std::numeric_limits<float>::quiet_NaN(),0.0f,0.0f},
        {false,{"00000000-0000-0000-0000-000000000000","",""},
            2.0f,0.0f,0.0f}
    };
    const auto chosen=prioritize_avatar_body_bakes(candidates,0.0f,0.0f,0.0f);
    const std::vector<std::string> expected{
        "self-head","self-upper","self-lower",
        "close-head","close-lower",
        "near-head","near-upper","near-lower"};
    if(chosen!=expected)return 1;
    if(prioritize_avatar_body_bakes(candidates,0,0,0,0).size()!=0)
        return 2;
    if(prioritize_avatar_body_bakes(candidates,0,0,0,4).size()!=4)
        return 3;
    if(!prioritize_avatar_body_bakes(candidates,
        std::numeric_limits<float>::quiet_NaN(),0,0).empty())
        return 4;
    if(!prioritize_avatar_body_bakes(candidates,0,0,0,12,-1).empty())
        return 5;

    // Saturated scene must still reserve GPU slots for server-provided
    // self Head/Upper/Lower BEFORE particle and world material textures.
    std::vector<std::string> objects;
    for(unsigned i=0U;i<530U;++i)
        objects.push_back("obj-"+std::to_string(i));
    const std::unordered_set<std::string> old_loaded{
        "stale-region-body","self-head","obj-0"};
    const auto plan=plan_texture_residency(
        {"smoke","spark"},objects,old_loaded,512U,chosen);
    if(plan.preferred_ids.size()!=512U||plan.desired_ids.size()!=512U ||
       plan.preferred_ids[0]!="self-head"||
       plan.preferred_ids[1]!="self-upper"||
       plan.preferred_ids[2]!="self-lower"||
       plan.preferred_ids[8]!="smoke"||
       plan.desired_ids.contains("far-head")||
       plan.desired_ids.contains("stale-region-body")||
       !plan.desired_ids.contains("obj-0")||
       plan.eviction_ids.size()!=1U||
       plan.eviction_ids[0]!="stale-region-body")
        return 6;
    const auto no_appearance=plan_texture_residency(
        {},{"new-world"},old_loaded,512U,{});
    if(no_appearance.desired_ids.contains("self-head")||
       !no_appearance.desired_ids.contains("new-world")||
       no_appearance.eviction_ids.size()!=old_loaded.size())
        return 7;

    // Even a large upstream avatar set cannot exceed bounded priority cap.
    std::vector<Candidate> many;
    for(unsigned i=0U;i<20U;++i)
        many.push_back({false,
            {"id-"+std::to_string(i),"",""},1.0f,0.0f,0.0f});
    if(prioritize_avatar_body_bakes(many,0,0,0,100U).size()!=20U)
        return 8;
    // Renderer would reject this malformed remote pose. The GetTexture
    // queue must reject it too, even if it has three server TE asset IDs.
    using nexverse::render::RenderAvatarProxy;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    std::vector<RenderAvatarProxy> proxies;
    RenderAvatarProxy bad_remote{};
    bad_remote.position={nan,1.0f,30.0f};
    bad_remote.baked_texture_ids={"invalid-head","invalid-upper","invalid-lower"};
    proxies.push_back(bad_remote);
    RenderAvatarProxy valid_remote{};
    valid_remote.position={3.0f,0.0f,30.0f};
    valid_remote.baked_texture_ids={"remote-head","remote-upper","remote-lower"};
    proxies.push_back(valid_remote);
    // The self avatar is NOT necessarily the first entry; it must still
    // win head/upper/lower priority and full mip residency.
    RenderAvatarProxy own{};
    own.position={0.0f,0.0f,30.0f};
    own.is_self=true;
    own.baked_texture_ids={"self-head","self-upper","self-lower"};
    proxies.push_back(own);
    const auto admitted=admit_avatar_bake_gpu_candidates(proxies);
    if(admitted.invalid_positions_skipped!=1U||
       admitted.self_position_skipped||admitted.accepted.size()!=2U||
       !admitted.accepted[1].is_self||
       admitted.accepted[1].body!=own.baked_texture_ids)
        return 9;
    const auto valid_priority=prioritize_avatar_body_bakes(
        admitted.accepted,0.0f,0.0f,30.0f);
    const std::vector<std::string> good_order{
        "self-head","self-upper","self-lower",
        "remote-head","remote-upper","remote-lower"};
    if(valid_priority!=good_order)return 10;
    std::unordered_set<std::string> rejected{"invalid-head",
        "invalid-upper","invalid-lower"};
    const auto safe_plan=plan_texture_residency(
        {},{"material"},old_loaded,512U,valid_priority);
    for(const auto& id:rejected)
        if(safe_plan.desired_ids.contains(id))return 11;
    // A damaged nonfinite orientation is repaired by the GPU renderer
    // without excluding an otherwise position-valid self texture.
    proxies[2].rotation={nan,0.0f,0.0f,1.0f};
    const auto still_admitted=admit_avatar_bake_gpu_candidates(proxies);
    if(still_admitted.accepted.size()!=2U||
       still_admitted.self_position_skipped)return 12;
    // But a server self pose rejected by GL must not spend the
    // self-specific GetTexture/mip slots or pretend skin is drawable.
    proxies[2].position.z=std::numeric_limits<float>::infinity();
    const auto invalid_self=admit_avatar_bake_gpu_candidates(proxies);
    if(invalid_self.accepted.size()!=1U||
       !invalid_self.self_position_skipped||
       invalid_self.invalid_positions_skipped!=2U)
        return 13;
    const auto no_self_priority=prioritize_avatar_body_bakes(
        invalid_self.accepted,0.0f,0.0f,30.0f);
    if(no_self_priority.size()!=3U||
       no_self_priority.front()!="remote-head")return 14;
    // The actual separate OpenSim Hair baked face (5) streams only
    // after the three real anatomical body faces. It must not override
    // a higher-priority Head/Upper/Lower bake or accept placeholder
    // and invalid-pose UUIDs from the current frame.
    auto hair_proxies=proxies;
    hair_proxies[0].hair_bake_texture_id="invalid-pose-hair";
    hair_proxies[1].hair_bake_texture_id="remote-hair";
    hair_proxies[2].position={0.0f,0.0f,30.0f};
    hair_proxies[2].hair_bake_texture_id="self-hair";
    const auto hair_admission=admit_avatar_bake_gpu_candidates(hair_proxies);
    if(hair_admission.accepted.size()!=2U||
       hair_admission.accepted[1].hair_bake_id!="self-hair")
        return 15;
    const auto hair_priority=prioritize_avatar_body_bakes(
        hair_admission.accepted,0.0f,0.0f,30.0f);
    const std::vector<std::string> hair_order{
        "self-head","self-upper","self-lower","self-hair",
        "remote-head","remote-upper","remote-lower","remote-hair"};
    if(hair_priority!=hair_order)return 16;
    hair_proxies[2].hair_bake_texture_id=
        std::string{nexverse::protocol::baked_placeholders[0]};
    const auto placeholder_admission=
        admit_avatar_bake_gpu_candidates(hair_proxies);
    if(prioritize_avatar_body_bakes(
           placeholder_admission.accepted,0.0f,0.0f,30.0f).size()!=7U)
        return 17;
    hair_proxies[2].hair_bake_texture_id="self-head";
    const auto dedup_admission=admit_avatar_bake_gpu_candidates(hair_proxies);
    if(prioritize_avatar_body_bakes(
           dedup_admission.accepted,0.0f,0.0f,30.0f).size()!=7U)
        return 18;
    if(prioritize_avatar_body_bakes(
           hair_admission.accepted,0.0f,0.0f,30.0f,3U)!=
       std::vector<std::string>{"self-head","self-upper","self-lower"})
        return 19;
    // All names are synthetic fixtures: no network or upload involved.
    std::cout<<"Avatar baked body stream priority and GPU eviction PASSED\n";
    return 0;
}
