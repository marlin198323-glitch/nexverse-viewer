/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/avatar_bake_stream_priority.hpp"
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
    // All names are synthetic fixtures: no network or upload involved.
    std::cout<<"Avatar baked body stream priority and GPU eviction PASSED\n";
    return 0;
}
