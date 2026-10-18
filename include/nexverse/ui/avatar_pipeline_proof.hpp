/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bake_evidence.hpp"
#include "nexverse/ui/avatar_view_evidence.hpp"
#include "nexverse/ui/avatar_appearance_runtime.hpp"
#include "nexverse/ui/avatar_visual_truth.hpp"
#include "nexverse/ui/avatar_part_visibility.hpp"
#include <array>
#include <cstdint>
#include <string>

namespace nexverse::ui
{
// Read-only, authenticated CURRENT-REGION proof. These counters must NOT
// be interpreted as confirmation of a visible on-screen avatar or bake upload.
struct AvatarPipelineProof
{
    bool appearance_received=false;
    // Authenticated self ObjectUpdate TE can supply genuine baked faces,
    // but this flag is NOT evidence of a Low158 AvatarAppearance packet.
    bool provisional_object_bakes=false;
    AvatarAppearanceRuntime appearance_runtime{};
    AvatarVisualTruth visual_truth{};
    bool world_ready=false;
    bool self_present=false;
    AvatarViewEvidence view{};
    std::uint64_t proxy_draws=0U;
    // Pre-UI, depth-tested original-body GL fragments from an asynchronous
    // query of a previous frame in the SAME simulator region epoch.
    bool fragment_query_known=false;
    bool fragment_samples_passed=false;
    AvatarPartVisibility part_visibility{};
    nexverse::protocol::AvatarBakeEvidence bakes{};
    std::uint64_t original_body_draws=0U;
    std::array<std::uint64_t,3U> body_bake_draws{};
    // All three current-server regions must be GPU-resident AND actually
    // submitted by the original LLM GL mesh in THIS frame.
    [[nodiscard]] std::string view_stage() const
    {
        return describe_avatar_view(world_ready,self_present,view,
            original_body_draws,proxy_draws);
    }
    [[nodiscard]] std::string fragment_stage() const
    {
        if(!world_ready||!self_present)
            return "Avatar Fragmente: keine aktive Szene";
        if(original_body_draws==0U)
            return "Avatar Fragmente: kein Original-LLM-Draw";
        const bool known=part_visibility.any_known()||fragment_query_known;
        if(!known)
            return "Avatar Fragmente: GPU-Abfrage laeuft";
        const bool visible=part_visibility.any_passed()||
            (fragment_query_known&&fragment_samples_passed);
        return visible
            ?"Avatar Fragmente: sichtbar (Depth-Test)"
            :"Avatar Fragmente: 0 (verdeckt/ausserhalb)";
    }
    [[nodiscard]] std::string skin_stage() const
    {
        if(provisional_object_bakes && !appearance_received)
            return "Skin: Obj-TE vorlaeufig GPU "+
                std::to_string(bakes.visible_body_resident)+
                "/3 (Low158 fehlt)";
        return describe_avatar_skin_truth(visual_truth);
    }
    [[nodiscard]] std::string animation_stage() const
    {
        return describe_avatar_animation_truth(visual_truth);
    }
    [[nodiscard]] bool provisional_body_gpu_submitted() const noexcept
    {
        return !appearance_received && provisional_object_bakes &&
            bakes.visible_body_resident==3U && original_body_draws>=3U &&
            body_bake_draws[0]>0U && body_bake_draws[1]>0U &&
            body_bake_draws[2]>0U;
    }
    [[nodiscard]] bool body_gpu_drawn() const noexcept
    {
        return appearance_received && bakes.visible_body_resident==3U &&
            original_body_draws>=3U && body_bake_draws[0]>0U &&
            body_bake_draws[1]>0U && body_bake_draws[2]>0U;
    }
};
[[nodiscard]] inline std::string describe_avatar_pipeline(
    const AvatarPipelineProof& proof)
{
    if(!proof.appearance_received)
    {
        if(proof.provisional_object_bakes)
            return "Obj-TE vorlaeufig "+
                std::to_string(proof.bakes.server_references)+
                "/3 GPU "+std::to_string(proof.bakes.gpu_resident)+
                "/3 (Low158 fehlt)";
        return describe_appearance_runtime(proof.appearance_runtime,false);
    }
    return "Server TE "+std::to_string(proof.bakes.server_references)+
        "/11 GPU "+std::to_string(proof.bakes.gpu_resident)+"/11";
}
[[nodiscard]] inline std::string describe_avatar_body_pipeline(
    const AvatarPipelineProof& proof)
{
    if(!proof.appearance_received && !proof.provisional_object_bakes)
        return "Body H:- U:- L:- | GL 0";
    const auto s=[](nexverse::protocol::AvatarBakeTextureState state)->char {
        return nexverse::protocol::avatar_bake_state_symbol(state);
    };
    const auto baked=proof.body_bake_draws[0]+
        proof.body_bake_draws[1]+proof.body_bake_draws[2];
    return std::string(proof.provisional_object_bakes&&!proof.appearance_received?
            "ObjTE ":"")+
        "H:"+std::string(1,s(proof.bakes.regions[0]))+
        " U:"+std::string(1,s(proof.bakes.regions[1]))+
        " L:"+std::string(1,s(proof.bakes.regions[2]))+
        " | LLM "+std::to_string(proof.original_body_draws)+
        " BakeGL "+std::to_string(baked);
}
}
