/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/avatar_bake_evidence.hpp"
#include "nexverse/ui/avatar_view_evidence.hpp"
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
    bool world_ready=false;
    bool self_present=false;
    AvatarViewEvidence view{};
    std::uint64_t proxy_draws=0U;
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
        return "Appearance: Paket fehlt";
    return "Server TE "+std::to_string(proof.bakes.server_references)+
        "/11 GPU "+std::to_string(proof.bakes.gpu_resident)+"/11";
}
[[nodiscard]] inline std::string describe_avatar_body_pipeline(
    const AvatarPipelineProof& proof)
{
    if(!proof.appearance_received)
        return "Body H:- U:- L:- | GL 0";
    const auto s=[](nexverse::protocol::AvatarBakeTextureState state)->char {
        return nexverse::protocol::avatar_bake_state_symbol(state);
    };
    const auto baked=proof.body_bake_draws[0]+
        proof.body_bake_draws[1]+proof.body_bake_draws[2];
    return "H:"+std::string(1,s(proof.bakes.regions[0]))+
        " U:"+std::string(1,s(proof.bakes.regions[1]))+
        " L:"+std::string(1,s(proof.bakes.regions[2]))+
        " | LLM "+std::to_string(proof.original_body_draws)+
        " BakeGL "+std::to_string(baked);
}
}
