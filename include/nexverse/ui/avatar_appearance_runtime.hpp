/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
#include <chrono>
#include <string>
namespace nexverse::ui
{
// Strictly counters and verified current-region state, NEVER IDs or
// untrusted text from an incoming packet.
struct AvatarAppearanceRuntime
{
    std::uint32_t received=0;
    std::uint32_t malformed=0;
    std::uint32_t ignored_region=0;
    std::uint32_t other_avatar=0;
    std::uint32_t self_accepted=0;
};
// ACK confirms delivery of the handshake datagram, NEVER an
// AvatarAppearance packet. A passive 15s warning does not request any
// further packets; simulator delivery remains authoritative.
[[nodiscard]] inline std::string describe_self_appearance_handshake(
    bool region_connected,bool advertised,bool acknowledged,bool received,
    std::chrono::seconds since_ack)
{
    if(!region_connected)
        return "SSA: Regionsverbindung ausstehend";
    if(!advertised)
        return "SSA: RegionHandshakeReply Bit 2 nicht gesendet";
    if(received)
        return "SSA: Self-Paket empfangen / Bit 2 gesendet";
    if(!acknowledged)
        return "SSA: Bit 2 gesendet / Simulator-ACK ausstehend";
    if(since_ack>=std::chrono::seconds{15})
        return "SSA: Bit 2 quittiert / Self nach 15s weiter ausstehend";
    return "SSA: Bit 2 quittiert / Self-Serverantwort fehlt";
}
[[nodiscard]] inline std::string describe_self_appearance_handshake(
    bool region_connected,bool advertised,bool acknowledged,bool received)
{
    return describe_self_appearance_handshake(
        region_connected,advertised,acknowledged,received,
        std::chrono::seconds{0});
}
[[nodiscard]] inline std::string describe_self_appearance_handshake(
    bool region_connected,bool advertised,bool received)
{
    if(!region_connected)
        return "SSA: Regionsverbindung ausstehend";
    if(!advertised)
        return "SSA: RegionHandshakeReply Bit 2 nicht gesendet";
    if(received)
        return "SSA: Bit 2 gesendet / Self vom Simulator empfangen";
    return "SSA: Bit 2 gesendet / Self-Serverantwort fehlt";
}
[[nodiscard]] inline std::string describe_appearance_runtime(
    const AvatarAppearanceRuntime& state,bool current_self)
{
    if(current_self)
        return "Appearance: Self vom Server bestaetigt";
    if(state.malformed)
        return "Appearance: "+std::to_string(state.malformed)+
               " ungueltige Pakete; Self fehlt";
    if(state.ignored_region)
        return "Appearance: Regionwechsel, Self fehlt";
    if(state.other_avatar)
        return "Appearance: nur fremde Avatare; Self fehlt";
    if(state.self_accepted)
        return "Appearance: alte Region, Self erneut ausstehend";
    return "Appearance: kein Self-Paket vom Simulator";
}
}
