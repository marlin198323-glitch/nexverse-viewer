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
        const bool candidate=proof.provisional_object_bakes &&
            !proof.appearance_received && i<3U;
        const bool has_server_appearance=proof.appearance_received;
        const bool source_available=has_server_appearance||candidate;
        auto state=source_available
            ?proof.bakes.regions[i]
            :nexverse::protocol::AvatarBakeTextureState::missing;
        std::string source;
        switch(state)
        {
        case nexverse::protocol::AvatarBakeTextureState::missing:
            source=source_available?(candidate?"Obj-TE: fehlt":"Server: fehlt"):
                "Server: wartet";
            break;
        case nexverse::protocol::AvatarBakeTextureState::server_reference:
            source=candidate?"Obj-TE: ID / GPU fehlt":"Server: ID / GPU fehlt";
            break;
        case nexverse::protocol::AvatarBakeTextureState::gpu_resident:
            source=candidate?"Obj-TE: ID / GPU geladen":
                "Server: ID / GPU geladen";
            break;
        }
        out.rows[i]=std::string{nexverse::protocol::avatar_bake_region_names[i]}+
            " | "+source;
        if(i<3U&&proof.appearance_received&&
           proof.object_fallback_faces[i])
        {
            const auto provisional_state=proof.object_fallback_bakes.regions[i];
            out.rows[i]=""+std::string{
                nexverse::protocol::avatar_bake_region_names[i]}+
                (provisional_state==
                    nexverse::protocol::AvatarBakeTextureState::gpu_resident
                    ?" | Obj-TE vorlaeufig / GPU geladen"
                    :" | Obj-TE vorlaeufig / GPU fehlt");
        }
        if(i<3U)
            out.rows[i]+=" | BakeGL "+
                std::to_string(source_available
                    ?proof.body_bake_draws[i]:0U)+
                " | "+proof.part_visibility.stage(i);
    }
    out.summary=proof.appearance_received
        ? "Server-TE "+std::to_string(proof.bakes.server_references)+
          "/11 | GPU "+std::to_string(proof.bakes.gpu_resident)+
          "/11 | Body "+std::to_string(proof.bakes.visible_body_resident)+"/3"
        :proof.provisional_object_bakes
            ?"Obj-TE vorlaeufig: "+
                std::to_string(proof.bakes.server_references)+
                "/3 IDs | GPU "+std::to_string(proof.bakes.gpu_resident)+
                "/3 | Low158 fehlt"
            :describe_appearance_runtime(proof.appearance_runtime,false);
    if(!proof.world_ready || !proof.self_present)
        out.visual="Kein aktiver eigener Avatar in der Region";
    else if(proof.provisional_object_bakes&&!proof.appearance_received)
    {
        if(!proof.provisional_body_gpu_submitted())
            out.visual="Obj-TE vorlaeufig: BakeGL noch unvollstaendig";
        else if(!proof.fragment_query_known)
            out.visual="Obj-TE BakeGL eingereicht; Depth-Test wartet";
        else if(!proof.fragment_samples_passed)
            out.visual="Obj-TE BakeGL, aber keine sichtbaren Depth-Pixel";
        else
            out.visual="Obj-TE sichtbar im GL (Low158 nicht belegt)";
    }
    else if(!proof.appearance_received)
        out.visual="Self-AvatarAppearance fehlt: Bake-GL NICHT belegt";
    else if(proof.object_fallback_faces[0]||
            proof.object_fallback_faces[1]||
            proof.object_fallback_faces[2])
        out.visual="Self Low158 lueckenhaft: Obj-TE Body provisorisch (kein COF-Beweis)";
    else if(proof.bakes.regions[0]==
                nexverse::protocol::AvatarBakeTextureState::missing&&
            proof.bakes.regions[1]==
                nexverse::protocol::AvatarBakeTextureState::missing&&
            proof.bakes.regions[2]==
                nexverse::protocol::AvatarBakeTextureState::missing)
        out.visual="Self Low158 da, Server liefert keine Body-Bake-IDs";
    else if(proof.bakes.regions[0]==
                nexverse::protocol::AvatarBakeTextureState::missing||
            proof.bakes.regions[1]==
                nexverse::protocol::AvatarBakeTextureState::missing||
            proof.bakes.regions[2]==
                nexverse::protocol::AvatarBakeTextureState::missing)
        out.visual="Self Low158: Body-Bake-IDs noch unvollstaendig";
    else if(proof.body_bake_draws[0]>0U&&proof.body_bake_draws[1]>0U&&
            proof.body_bake_draws[2]>0U&&!proof.current_bake_sources_match())
        out.visual="BakeGL Texturstand abweichend: aktuelle Low158 Bakes noch nicht gezeichnet";
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
