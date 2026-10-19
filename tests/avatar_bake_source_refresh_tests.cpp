/* MPL-2.0: immediate scheduler invalidation for real Self baked IDs. */
#include "nexverse/assets/avatar_bake_source_refresh.hpp"
#include "nexverse/assets/avatar_bake_stream_priority.hpp"
#include "nexverse/assets/texture_residency.hpp"
#include <array>
#include <iostream>
using namespace nexverse;
int main()
{
    assets::AvatarBakeSourceRefresh refresh;
    using Body=assets::AvatarBakeSourceRefresh::Body;
    const Body empty{};
    const Body head{"head","",""};
    const Body mixed{"head","upper",""};
    const Body changed{"new-head","upper","lower"};
    if(refresh.observe(0U,true,true,head)||
       refresh.observe(9U,false,true,head)||
       refresh.observe(9U,true,false,head)||
       refresh.source_known())return 1;
    if(refresh.observe(9U,true,true,empty)||
       !refresh.source_known())return 2;
    if(!refresh.observe(9U,true,true,head)||
       refresh.observe(9U,true,true,head))return 3;
    if(!refresh.observe(9U,true,true,mixed)||
       refresh.observe(9U,true,true,mixed))return 4;
    if(!refresh.observe(9U,true,true,changed)||
       refresh.observe(9U,true,true,changed))return 5;
    if(refresh.observe(9U,true,false,changed)||refresh.source_known())
        return 6;
    if(!refresh.observe(9U,true,true,changed))return 7;
    if(!refresh.observe(10U,true,true,changed))return 8;
    if(!refresh.observe(10U,true,true,mixed))return 9;
    // Incomplete appearances can remove bakes. Replan the current
    // residency, but do not report a new fetchable body asset.
    if(!refresh.observe(10U,true,true,empty)||
       !refresh.source_known()||
       refresh.observe(10U,true,true,empty))return 10;
    if(!refresh.observe(10U,true,true,head))return 11;
    Body placeholder{std::string{protocol::baked_placeholders[0]},"",""};
    if(!refresh.observe(10U,true,true,placeholder)||
       refresh.observe(10U,true,true,empty))return 12;
    if(!refresh.observe(10U,true,true,head))return 13;
    // The existing full GPU-residency planner—not a new unsafe
    // downloader—promotes the changed source under a bounded slot budget.
    const Body previous{"old-head","old-upper","old-lower"};
    auto before=assets::prioritize_avatar_body_bakes(
        {{true,previous,1.0f,1.0f,1.0f}},0.0f,0.0f,0.0f);
    auto first=assets::plan_texture_residency(
        {"particle"},{"world"}, {},3U,before);
    if(first.preferred_ids!=std::vector<std::string>(
           previous.begin(),previous.end())||first.desired_ids.size()!=3U)
        return 14;
    if(!refresh.observe(11U,true,true,changed))return 15;
    auto after=assets::prioritize_avatar_body_bakes(
        {{true,changed,1.0f,1.0f,1.0f}},0.0f,0.0f,0.0f);
    auto second=assets::plan_texture_residency(
        {"particle"},{"world"},
        {"old-head","old-upper","old-lower"},3U,after);
    if(second.preferred_ids!=std::vector<std::string>(
           changed.begin(),changed.end())||
       second.eviction_ids.size()!=3U||
       second.desired_ids.contains("old-head")||
       second.desired_ids.contains("particle")||
       refresh.observe(11U,true,true,changed))return 16;
    // Removing one body ID frees that slot for a scene texture;
    // previously selected old body IDs do not remain pinned forever.
    const Body less{"new-head","upper",""};
    if(!refresh.observe(11U,true,true,less))return 17;
    auto third_ids=assets::prioritize_avatar_body_bakes(
        {{true,less,1.0f,1.0f,1.0f}},0.0f,0.0f,0.0f);
    auto third=assets::plan_texture_residency(
        {"particle"},{"world"},
        {"new-head","upper","lower"},3U,third_ids);
    if(third.preferred_ids.size()!=3U||
       third.preferred_ids[0]!="new-head"||
       third.preferred_ids[1]!="upper"||
       third.preferred_ids[2]!="particle"||
       third.eviction_ids!=std::vector<std::string>{"lower"})
        return 18;
    std::cout<<"AVATAR_BAKE_SOURCE_REPLAN_IMMEDIATE_AUTH_EPOCH_PASS\n";
}
