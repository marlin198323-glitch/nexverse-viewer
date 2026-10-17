/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include <array>
#include <cstddef>
#include <string>

namespace nexverse::ui
{
// Never displays server-supplied UUIDs or derives GPU success from a
// server TextureEntry alone. The caller first verifies self identity/epoch.
struct AvatarBakeInspector
{
    std::array<std::string,11U> rows{};
    std::string summary;
    std::string visual;
};
[[nodiscard]] inline AvatarBakeInspector inspect_avatar_bakes(
    const AvatarPipelineProof& proof)
{
    AvatarBakeInspector out;
    for(std::size_t i=0U;i<out.rows.size();++i)
    {
        auto state=proof.appearance_received
            ?proof.bakes.regions[i]
            :nexverse::protocol::AvatarBakeTextureState::missing;
        std::string source;
        switch(state)
        {
        case nexverse::protocol::AvatarBakeTextureState::missing:
            source=proof.appearance_received?"Server: fehlt":"Server: wartet";
            break;
        case nexverse::protocol::AvatarBakeTextureState::server_reference:
            source="Server: ID / GPU fehlt";
            break;
        case nexverse::protocol::AvatarBakeTextureState::gpu_resident:
            source="Server: ID / GPU geladen";
            break;
        }
        out.rows[i]=std::string{nexverse::protocol::avatar_bake_region_names[i]}+
            " | "+source;
        if(i<3U)
            out.rows[i]+=" | BakeGL "+
                std::to_string(proof.appearance_received
                    ?proof.body_bake_draws[i]:0U)+
                " | "+proof.part_visibility.stage(i);
    }
    out.summary=proof.appearance_received
        ? "Server-TE "+std::to_string(proof.bakes.server_references)+
          "/11 | GPU "+std::to_string(proof.bakes.gpu_resident)+
          "/11 | Body "+std::to_string(proof.bakes.visible_body_resident)+"/3"
        : describe_appearance_runtime(proof.appearance_runtime,false);
    if(!proof.world_ready || !proof.self_present)
        out.visual="Kein aktiver eigener Avatar in der Region";
    else if(!proof.appearance_received)
        out.visual="Self-AvatarAppearance fehlt: Bake-GL NICHT belegt";
    else if(!proof.body_gpu_drawn())
        out.visual="Original Head/Upper/Lower BakeGL noch unvollstaendig";
    else if(!proof.fragment_query_known)
        out.visual="BakeGL eingereicht; Depth-Pixelpruefung laeuft";
    else if(!proof.fragment_samples_passed)
        out.visual="BakeGL eingereicht; kein sichtbarer Depth-Pixelbeleg";
    else
        out.visual="Server-Bakes: Original-GL + sichtbare Depth-Fragmente";
    return out;
}
}
