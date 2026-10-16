/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include <iostream>
int main()
{
    using namespace nexverse;
    ui::AvatarPipelineProof p;
    if(ui::describe_avatar_pipeline(p)!="Appearance: Paket fehlt"||
       ui::describe_avatar_body_pipeline(p)!="Body H:- U:- L:- | GL 0")return 1;
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
    p.appearance_received=false;
    if(p.body_gpu_drawn())return 9;
    if(p.bakes.upload_authorized())return 5;
    std::cout<<"Avatar packet, server refs, GPU and actual GL read-only stages PASS\n";
    return 0;
}
