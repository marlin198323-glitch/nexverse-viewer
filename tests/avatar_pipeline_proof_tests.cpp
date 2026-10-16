/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include <iostream>
int main()
{
    using namespace nexverse;
    ui::AvatarPipelineProof p;
    if(ui::describe_avatar_pipeline(p)!="Appearance: Paket fehlt"||
       ui::describe_avatar_body_pipeline(p)!="Body H:- U:- L:- | GL 0")return 1;
    if(p.view_stage()!="Avatar: Simulator wartet")return 12;
    p.world_ready=true;
    if(p.view_stage()!="Avatar: AgentPosition fehlt")return 13;
    p.self_present=true;
    if(p.view_stage()!="Avatar: Kameraposition ungueltig")return 14;
    p.view.valid_pose=true;
    p.view.center_in_frustum=true;
    if(p.view_stage()!="Avatar Mitte im Bildfeld | Mesh GL fehlt")return 15;
    p.proxy_draws=7U;
    if(p.view_stage()!="Avatar Mitte im Bildfeld | Ersatz GL 7")return 16;
    p.original_body_draws=3U;
    if(p.view_stage()!="Avatar Mitte im Bildfeld | LLM GL 3")return 17;
    p.original_body_draws=0U;
    p.proxy_draws=0U;
    p.appearance_received=true;
    if(ui::describe_avatar_pipeline(p)!="Server TE 0/11 GPU 0/11")return 2;
    protocol::AvatarBakedTextures ids{};
    ids.ids[0]="head";
    ids.ids[1]="upper";
    ids.ids[2]="lower";
    p.bakes=protocol::audit_avatar_bake_evidence(ids,{"head","lower"});
    p.original_body_draws=3;
    p.body_bake_draws={1U,0U,1U};
    if(ui::describe_avatar_pipeline(p)!="Server TE 3/11 GPU 2/11")return 3;
    if(ui::describe_avatar_body_pipeline(p)!=
       "H:G U:S L:G | LLM 3 BakeGL 2")return 4;
    if(p.body_gpu_drawn())return 6;
    p.bakes=protocol::audit_avatar_bake_evidence(ids,{"head","upper","lower"});
    if(p.body_gpu_drawn())return 7;
    p.body_bake_draws={1U,1U,1U};
    if(!p.body_gpu_drawn())return 8;
    if(p.fragment_stage()!="Avatar Fragmente: GPU-Abfrage laeuft")
        return 18;
    p.fragment_query_known=true;
    p.fragment_samples_passed=true;
    if(p.fragment_stage()!="Avatar Fragmente: sichtbar (Depth-Test)")
        return 19;
    p.fragment_samples_passed=false;
    if(p.fragment_stage()!="Avatar Fragmente: 0 (verdeckt/ausserhalb)")
        return 20;
    p.appearance_received=false;
    if(p.body_gpu_drawn())return 9;
    if(p.bakes.upload_authorized())return 5;
    std::cout<<"Avatar packet, server refs, GPU and actual GL read-only stages PASS\n";
    return 0;
}
