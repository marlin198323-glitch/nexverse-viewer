/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once

#include "nexverse/protocol/avatar_bakes.hpp"
#include <array>
#include <cstddef>
#include <string_view>
#include <string>
#include <unordered_set>

namespace nexverse::protocol
{
// This is evidence about server-supplied baked UUIDs and textures that are
// ALREADY resident in the current region's renderer. It is not evidence that
// any client-side bake was generated, uploaded, or accepted by the simulator.
enum class AvatarBakeTextureState : unsigned char
{
    missing,         // Server's TextureEntry lacks a valid baked UUID.
    server_reference,// Valid server UUID; corresponding GPU texture not resident.
    gpu_resident     // Matching UUID in current region's live GPU asset set.
};

inline constexpr std::array<std::string_view,11U> avatar_bake_region_names{{
    "Head","Upper","Lower","Eyes","Skirt","Hair","LeftArm","LeftLeg",
    "Aux1","Aux2","Aux3"
}};

struct AvatarBakeEvidence
{
    std::array<AvatarBakeTextureState,11U> regions{};
    std::size_t server_references=0U;
    std::size_t gpu_resident=0U;
    std::size_t visible_body_resident=0U;

    [[nodiscard]] bool complete() const noexcept
    {return gpu_resident==regions.size();}

    [[nodiscard]] bool upload_authorized() const noexcept
    {return false;} // Read-only state; no validated client-side bake.
};

// Caller must supply UUIDs decoded from a current authenticated simulator
// AvatarAppearance TextureEntry and a CURRENT-REGION GPU residency set.
[[nodiscard]] inline AvatarBakeEvidence audit_avatar_bake_evidence(
    const AvatarBakedTextures& server_bakes,
    const std::unordered_set<std::string>& resident_gpu_assets)
{
    AvatarBakeEvidence result;
    for(std::size_t i=0U;i<result.regions.size();++i)
    {
        const auto& id=server_bakes.ids[i];
        if(id.empty())continue;
        ++result.server_references;
        if(resident_gpu_assets.contains(id))
        {
            result.regions[i]=AvatarBakeTextureState::gpu_resident;
            ++result.gpu_resident;
            if(i<3U)++result.visible_body_resident;
        }
        else result.regions[i]=AvatarBakeTextureState::server_reference;
    }
    return result;
}

[[nodiscard]] inline char avatar_bake_state_symbol(
    AvatarBakeTextureState state) noexcept
{
    switch(state)
    {
        case AvatarBakeTextureState::gpu_resident:return 'G';
        case AvatarBakeTextureState::server_reference:return 'S';
        case AvatarBakeTextureState::missing:return '-';
    }
    return '-';
}

// Short enough for the 95-character F9 status row, with stage/limits first.
// Server/Inventory version equality does not prove Firestorm pixel parity.
[[nodiscard]] inline std::string describe_avatar_bake_evidence(
    std::uint32_t simulator_cof_version,
    std::uint32_t inventory_cof_version,
    const AvatarBakeEvidence& evidence,bool live=false)
{
    const bool versions_match=simulator_cof_version!=0U &&
        simulator_cof_version==inventory_cof_version;
    return std::string{live?"KEIN BAKE LIVE | COF ":"KEIN BAKE | COF "}+
        std::to_string(simulator_cof_version)+"/"+
        std::to_string(inventory_cof_version)+
        (versions_match?" OK":" OFFEN")+
        " | TE "+std::to_string(evidence.server_references)+"/11"+
        " GPU "+std::to_string(evidence.gpu_resident)+"/11"+
        " Body "+std::to_string(evidence.visible_body_resident)+"/3"+
        " H:"+avatar_bake_state_symbol(evidence.regions[0])+
        " U:"+avatar_bake_state_symbol(evidence.regions[1])+
        " L:"+avatar_bake_state_symbol(evidence.regions[2])+
        " (G=GPU S=ID -=fehlt)";
}
}
