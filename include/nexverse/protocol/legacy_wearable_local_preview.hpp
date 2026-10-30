/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/legacy_wearable_inventory_probe.hpp"
#include <cstdint>
#include <string_view>
namespace nexverse::protocol
{
// Firestorm local UNBAKED self-avatar preview only; no COF mutation,
// no AgentIsNowWearing and no AgentSetAppearance. Before using the
// simulator's legacy worn originals, prove owner/session/serial and real
// empty COF as well as the matching primary bodypart inventory objects.
[[nodiscard]] inline bool legacy_local_preview_admitted(
    const LegacyWearableInventoryEvidence& evidence,
    bool server_cof_raw_empty,std::uint64_t source_epoch,
    std::uint64_t current_epoch,std::uint32_t source_serial,
    std::uint32_t current_serial,std::string_view owner,
    std::string_view current_owner,std::string_view session,
    std::string_view current_session)
{
    return evidence.valid&&evidence.complete_primary&&
        evidence.missing==0U&&evidence.requested>0U&&
        evidence.matched==evidence.requested&&server_cof_raw_empty&&
        source_epoch!=0U&&source_epoch==current_epoch&&
        source_serial==current_serial&&
        !owner.empty()&&owner==current_owner&&
        !session.empty()&&session==current_session;
}
}
