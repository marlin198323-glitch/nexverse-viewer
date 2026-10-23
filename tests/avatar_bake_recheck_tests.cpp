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
    // Alpha.1015-1024: only EXACT current self Low158 body UUIDs
    // inherit a GetTexture failure, never unrelated scene HTTP 404s.
    (void)retry.failure("unrelated-scene",assets::AssetFailure::missing,t);
    auto stages=assets::inspect_self_bake_gettexture(ids,gpu,retry,t);
    using S=assets::SelfBakeGetTextureStage;
    if(stages!=std::array<S,3>{S::gpu_resident,
                              S::http_missing_backoff,
                              S::ready_to_retry}||
       assets::self_bake_gettexture_summary(stages)!=
           "Body-GetTexture H:GPU U:HTTP404 L:bereit")return 31;
    auto alternate=ids;
    alternate[1]="new-upper";
    // Old outfit/scene failure must not be promoted to a NEW body UUID.
    stages=assets::inspect_self_bake_gettexture(alternate,gpu,retry,t);
    if(stages[1]!=S::ready_to_retry)return 32;
    (void)retry.failure("lower",assets::AssetFailure::transient,t);
    if(assets::inspect_self_bake_gettexture(ids,gpu,retry,t)[2]!=
       S::transient_backoff)return 33;
    (void)retry.failure("lower",assets::AssetFailure::configuration,t);
    if(assets::inspect_self_bake_gettexture(ids,gpu,retry,t)[2]!=
       S::configuration_backoff)return 34;
    (void)retry.failure("lower",assets::AssetFailure::invalid,t);
    if(assets::inspect_self_bake_gettexture(ids,gpu,retry,t)[2]!=
       S::invalid_backoff)return 35;
    auto placeholder=ids;
    placeholder[2]=std::string{protocol::baked_placeholders[0]};
    if(assets::inspect_self_bake_gettexture(placeholder,gpu,retry,t)[2]!=
       S::no_server_reference)return 36;
    if(assets::inspect_self_bake_gettexture(ids,gpu,retry,
           t+std::chrono::minutes{31})[1]!=
       S::ready_to_retry)return 37;
    retry.enter(24U);
    if(assets::inspect_self_bake_gettexture(ids,gpu,retry,t)[1]!=
       S::ready_to_retry)return 38;
    retry.enter(23U);
    (void)retry.failure("upper",assets::AssetFailure::missing,t);
    // Read-only evidence must not clear or override live retry backoff.
    if(retry.ready("upper",t))return 39;
    (void)retry.failure("lower",assets::AssetFailure::invalid,t);
    // Alpha.1025-1034: the current renderer can use different sources
    // per body slot. Low158 Head wins, while missing Low158 Upper can
    // use provisional ObjectUpdate Upper. Never label ObjTE as a COF.
    using B=assets::SelfBakeSource;
    const std::array<std::string,3U> chosen{"head","upper","lower"};
    const std::array<bool,3U> low_source{true,false,false};
    const std::array<bool,3U> obj_source{false,true,true};
    const auto mixed=assets::inspect_self_render_bake_gettexture(
        chosen,low_source,obj_source,gpu,retry,t);
    if(mixed.sources!=std::array<B,3>{B::self_low158,
             B::provisional_object_te,B::provisional_object_te}||
       mixed.stages!=std::array<S,3>{S::gpu_resident,
             S::http_missing_backoff,S::invalid_backoff}||
       assets::self_render_bake_gettexture_summary(mixed)!=
        "Body H:L/GPU U:O/HTTP404 L:O/ungueltig (L=Low158 O=ObjTE)")
        return 40;
    // A newer outfit Low158 Upper immediately supersedes ObjTE Upper;
    // only the new exact UUID may inherit an HTTP failure.
    const std::array<std::string,3U> next_cof{"head","new-upper","lower"};
    auto low_new=low_source;
    low_new[1]=true;
    const auto new_outfit=assets::inspect_self_render_bake_gettexture(
        next_cof,low_new,obj_source,gpu,retry,t);
    if(new_outfit.sources[1]!=B::self_low158||
       new_outfit.stages[1]!=S::ready_to_retry||
       assets::self_render_bake_gettexture_summary(new_outfit).find(
           "U:L/bereit")==std::string::npos)return 41;
    // Missing/placeholder IDs with a false or spoofed authority flag
    // cannot claim a download or emit any body source.
    const auto empty_source=assets::inspect_self_render_bake_gettexture(
        {"",std::string{protocol::baked_placeholders[0]},"lower"},
        {false,false,false},{true,true,false},gpu,retry,t);
    if(empty_source.sources!=std::array<B,3>{B::none,B::none,B::none}||
       empty_source.stages!=std::array<S,3>{S::no_server_reference,
           S::no_server_reference,S::no_server_reference})return 42;
    // Object-only pre-Low158 candidate may be inspected, but must
    // remain explicitly provisional and must not alter retry backoff.
    auto provisional_probe=assets::inspect_self_render_bake_gettexture(
        chosen,{false,false,false},{true,true,true},gpu,retry,t);
    if(provisional_probe.sources[0]!=B::provisional_object_te||
       provisional_probe.sources[1]!=B::provisional_object_te||
       provisional_probe.stages[1]!=S::http_missing_backoff||
       retry.ready("upper",t))return 43;
    // Region-epoch retirement atomically drops old retry attribution.
    retry.enter(25U);
    provisional_probe=assets::inspect_self_render_bake_gettexture(
        chosen,{false,false,false},{true,true,true},gpu,retry,t);
    if(provisional_probe.stages[1]!=S::ready_to_retry)return 44;
    retry.enter(23U);
    (void)retry.failure("upper",assets::AssetFailure::missing,t);
    policy.observe_source(23U,true,true,ids);
    if(!policy.request(true,true,true,false,true,t)||
       policy.request(true,true,true,false,true,t+
           std::chrono::seconds{11}))return 5;
    if(!policy.request(true,true,true,false,true,t+
           std::chrono::seconds{12}) ||
       !policy.request(true,true,true,false,true,t+
           std::chrono::seconds{24}) ||
       policy.request(true,true,true,false,true,t+
           std::chrono::seconds{36}))return 6;
    if(policy.requests!=3U||policy.total_requests!=3U)return 7;
    // The SAME outfit may not bypass 3 attempts by passing an empty,
    // placeholder or temporarily unauthenticated TE to the policy.
    policy.observe_source(23U,true,true,ids);
    policy.observe_source(23U,true,true,{});
    policy.observe_source(23U,true,true,{
        std::string{protocol::baked_placeholders[0]},"",""});
    policy.observe_source(23U,false,true,{"other","upper","lower"});
    policy.observe_source(23U,true,false,{"other","upper","lower"});
    if(policy.requests!=3U||
       policy.request(true,true,true,false,true,
                      t+std::chrono::seconds{36}))return 19;
    // A DIFFERENT authenticated real self TE may be checked, while
    // preserving the region total and the 12-second shared pacing.
    const std::array<std::string,3U> second{"other","upper","lower"};
    policy.observe_source(23U,true,true,second);
    if(policy.requests!=0U||policy.total_requests!=3U||
       !policy.request(true,true,true,false,true,
                       t+std::chrono::seconds{36}))return 20;
    const std::array<std::string,3U> third{"third","upper","lower"};
    policy.observe_source(23U,true,true,third);
    if(policy.request(true,true,true,false,true,
                      t+std::chrono::seconds{42}))return 21;
    if(!policy.request(true,true,true,false,true,
                       t+std::chrono::seconds{48})||
       !policy.request(true,true,true,false,true,
                       t+std::chrono::seconds{60})||
       policy.requests!=2U||policy.total_requests!=6U)return 22;
    // Outfit churn never evades the aggregate region epoch cap.
    for(unsigned i=0U;i<6U;++i)
    {
        auto differing=third;
        differing[0]="outfit-"+std::to_string(i);
        policy.observe_source(23U,true,true,differing);
        if(!policy.request(true,true,true,false,true,
                           t+std::chrono::seconds{72+12*i}))
            return 23;
    }
    policy.observe_source(23U,true,true,{"final","upper","lower"});
    if(policy.total_requests!=12U||
       policy.request(true,true,true,false,true,
                      t+std::chrono::seconds{145}))return 24;
    policy.enter(24U);
    if(policy.requests!=0U||policy.total_requests!=0U||
       !policy.allowed(true,true,true,false,true,t))
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
    if(ui::inspect_avatar_bakes(proof).visual.find("Texturstand abweichend")==std::string::npos)return 19;
    proof.current_body_bake_sources_drawn={true,true,true};
    auto drawn=ui::inspect_avatar_bakes(proof);
    if(drawn.visual.find("Pixelpruefung")==std::string::npos)return 12;
    proof.fragment_query_known=true;proof.fragment_samples_passed=true;
    if(ui::inspect_avatar_bakes(proof).visual.find("sichtbare Depth")==
       std::string::npos)return 13;
    proof.object_fallback_faces[1]=true;
    protocol::AvatarBakedTextures provisional{};
    provisional.ids[1]="upper";
    proof.object_fallback_bakes=protocol::audit_avatar_bake_evidence(
        provisional,{"upper"});
    const auto mixed_display=ui::inspect_avatar_bakes(proof);
    if(mixed_display.rows[1].find("Obj-TE vorlaeufig / GPU geladen")==
           std::string::npos||
       mixed_display.visual.find("Obj-TE Body provisorisch")==
           std::string::npos||
       proof.skin_stage().find("Obj-TE vorlaeufig")==std::string::npos)
        return 18;
    proof.object_fallback_faces={};
    proof.appearance_received=false;
    if(ui::inspect_avatar_bakes(proof).rows[0].find("GPU geladen")!=
       std::string::npos)return 14;
    std::cout<<"Avatar Bakes: current-server state, GPU depth, safe recheck PASS\n";
}
