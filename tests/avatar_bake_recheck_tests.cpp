/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/avatar_bake_recheck.hpp"
#include "nexverse/ui/avatar_bake_inspector.hpp"
#include <iostream>
#include <unordered_set>
int main()
{
    using namespace nexverse;
    using Time=assets::AvatarBakeRecheck::Clock;
    assets::AvatarBakeRecheck policy;
    const auto t=Time::time_point{}+std::chrono::hours{1};
    if(policy.allowed(true,true,true,false,true,t))return 1;
    policy.enter(23U);
    if(policy.allowed(false,true,true,false,true,t)||
       policy.allowed(true,false,true,false,true,t)||
       policy.allowed(true,true,false,false,true,t)||
       policy.allowed(true,true,true,true,true,t)||
       policy.allowed(true,true,true,false,false,t))return 2;
    std::array<std::string,3> ids{"head","upper","lower"};
    assets::AssetRetrySchedule retry;
    retry.enter(23U);
    std::unordered_set<std::string> gpu{"head"};
    if(assets::avatar_bake_ready_missing(ids,gpu,retry,t)!=2U)return 3;
    (void)retry.failure("upper",assets::AssetFailure::missing,t);
    if(assets::avatar_bake_ready_missing(ids,gpu,retry,t)!=1U)return 4;
    if(!policy.request(true,true,true,false,true,t)||
       policy.request(true,true,true,false,true,t+
           std::chrono::seconds{11}))return 5;
    if(!policy.request(true,true,true,false,true,t+
           std::chrono::seconds{12}) ||
       !policy.request(true,true,true,false,true,t+
           std::chrono::seconds{24}) ||
       policy.request(true,true,true,false,true,t+
           std::chrono::seconds{36}))return 6;
    if(policy.requests!=3U)return 7;
    policy.enter(24U);
    if(policy.requests!=0U||!policy.allowed(true,true,true,false,true,t))
        return 8;
    ids[2]="upper";
    if(assets::avatar_bake_ready_missing(ids,gpu,retry,
           t+std::chrono::minutes{11})!=1U)return 15;
    ids[2]=std::string{protocol::baked_placeholders[2]};
    if(assets::avatar_bake_ready_missing(ids,gpu,retry,t)!=0U)return 9;
    ui::AvatarPipelineProof proof{};
    auto empty=ui::inspect_avatar_bakes(proof);
    if(empty.visual.find("Kein aktiver")==std::string::npos||
       empty.rows[0].find("wartet")==std::string::npos)return 10;
    proof.world_ready=true;
    proof.self_present=true;
    proof.appearance_received=true;
    // A valid Low158 without bake UUIDs is a different blocker from
    // an actual GPU/renderer failure. F3 must state the server gap.
    if(ui::inspect_avatar_bakes(proof).visual.find(
           "keine Body-Bake-IDs")==std::string::npos)return 16;
    protocol::AvatarBakedTextures partial_server{};
    partial_server.ids[0]="head";
    proof.bakes=protocol::audit_avatar_bake_evidence(
        partial_server,{});
    if(ui::inspect_avatar_bakes(proof).visual.find(
           "Body-Bake-IDs noch unvollstaendig")==std::string::npos)
        return 17;
    protocol::AvatarBakedTextures bakes{};
    bakes.ids[0]="head";bakes.ids[1]="upper";bakes.ids[2]="lower";
    proof.bakes=protocol::audit_avatar_bake_evidence(bakes,{"head","lower"});
    proof.original_body_draws=3;
    proof.body_bake_draws={1U,0U,1U};
    auto partial=ui::inspect_avatar_bakes(proof);
    if(partial.rows[0].find("GPU geladen")==std::string::npos ||
       partial.rows[1].find("GPU fehlt")==std::string::npos ||
       partial.visual.find("unvollstaendig")==std::string::npos)return 11;
    proof.bakes=protocol::audit_avatar_bake_evidence(bakes,{"head","upper","lower"});
    proof.body_bake_draws={1,1,1};
    auto drawn=ui::inspect_avatar_bakes(proof);
    if(drawn.visual.find("Pixelpruefung")==std::string::npos)return 12;
    proof.fragment_query_known=true;proof.fragment_samples_passed=true;
    if(ui::inspect_avatar_bakes(proof).visual.find("sichtbare Depth")==
       std::string::npos)return 13;
    proof.appearance_received=false;
    if(ui::inspect_avatar_bakes(proof).rows[0].find("GPU geladen")!=
       std::string::npos)return 14;
    std::cout<<"Avatar Bakes: current-server state, GPU depth, safe recheck PASS\n";
}
