/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_part_visibility.hpp"
#include "nexverse/ui/avatar_bake_inspector.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    AvatarPartVisibility parts;
    parts.known={true,true,true};
    parts.passed={true,false,false};
    if(parts.any_known()||parts.any_passed()||
       parts.stage(0)!="kein LLM-Draw")return 1;
    parts.original_draws=3U;
    if(!parts.any_known()||!parts.any_passed()||
       parts.stage(0)!="Depth sichtbar"||
       parts.stage(1)!="Depth 0 (verdeckt)")return 2;
    // A negative result in lower-body MUST NOT overwrite a positive head.
    parts.passed={false,false,false};
    if(parts.any_passed())return 3;
    parts.known={false,false,false};
    if(parts.any_known()||parts.any_passed()||
       parts.compact().find("H:?")==std::string::npos)return 4;
    parts.known={true,false,true};
    parts.passed={true,false,false};
    AvatarPipelineProof proof{};
    proof.world_ready=true;
    proof.self_present=true;
    proof.original_body_draws=3U;
    proof.part_visibility=parts;
    if(proof.fragment_stage().find("sichtbar")==std::string::npos)
        return 5;
    auto view=inspect_avatar_bakes(proof);
    if(view.rows[0].find("Depth sichtbar")==std::string::npos||
       view.rows[1].find("Depth ausstehend")==std::string::npos||
       view.rows[2].find("Depth 0")==std::string::npos)return 6;
    // A previously successful old-region query must not be shown if
    // the active renderer no longer submits original self body GL.
    proof.original_body_draws=0U;
    proof.part_visibility.original_draws=0U;
    if(proof.fragment_stage()!="Avatar Fragmente: kein Original-LLM-Draw")
        return 7;
    std::cout<<"Independent original-avatar GL Depth H/U/L PASS\n";
}
