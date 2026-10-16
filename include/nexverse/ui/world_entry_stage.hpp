/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <string_view>
#include <chrono>
#include <cstdint>
namespace nexverse::ui
{
enum class WorldEntryStage {
    login, simulator_circuit, region_handle, region_connection,
    agent_identity, agent_position, ready
};
[[nodiscard]] constexpr WorldEntryStage inspect_world_entry(
    bool authenticated,bool circuit_active,bool region_has_handle,
    bool region_connected,bool agent_has_identity,bool agent_present) noexcept
{
    if(!authenticated)return WorldEntryStage::login;
    if(!circuit_active)return WorldEntryStage::simulator_circuit;
    if(!region_has_handle)return WorldEntryStage::region_handle;
    if(!region_connected)return WorldEntryStage::region_connection;
    if(!agent_has_identity)return WorldEntryStage::agent_identity;
    if(!agent_present)return WorldEntryStage::agent_position;
    return WorldEntryStage::ready;
}
// Only required LLUDP/region/identity/position milestones count toward
// world-entry progress. CAPS EventQueue, terrain and object packets are
// independent streaming indicators, not readiness conditions.
[[nodiscard]] constexpr float world_entry_fraction(
    WorldEntryStage stage) noexcept
{
    switch(stage)
    {
        case WorldEntryStage::login:return 0.04f;
        case WorldEntryStage::simulator_circuit:return 0.18f;
        case WorldEntryStage::region_handle:return 0.36f;
        case WorldEntryStage::region_connection:return 0.53f;
        case WorldEntryStage::agent_identity:return 0.70f;
        case WorldEntryStage::agent_position:return 0.88f;
        case WorldEntryStage::ready:return 1.0f;
    }
    return 0.04f;
}
[[nodiscard]] constexpr std::string_view world_entry_stage_key(
    WorldEntryStage stage) noexcept
{
    switch(stage)
    {
        case WorldEntryStage::login:return "login";
        case WorldEntryStage::simulator_circuit:return "simulator_circuit";
        case WorldEntryStage::region_handle:return "region_handle";
        case WorldEntryStage::region_connection:return "region_connection";
        case WorldEntryStage::agent_identity:return "identity_pair";
        case WorldEntryStage::agent_position:return "agent_position";
        case WorldEntryStage::ready:return "ready";
    }
    return "unknown";
}
struct WorldEntryWaitMonitor
{
    WorldEntryStage last_stage=WorldEntryStage::login;
    std::chrono::steady_clock::time_point since{};
    [[nodiscard]] std::uint32_t seconds(
        WorldEntryStage stage,std::chrono::steady_clock::time_point now)
    {
        if(stage!=last_stage||since==std::chrono::steady_clock::time_point{}||
           now<since)
        {
            last_stage=stage;
            since=now;
            return 0;
        }
        const auto elapsed=
            std::chrono::duration_cast<std::chrono::seconds>(now-since);
        return static_cast<std::uint32_t>(
            elapsed.count()>3600?3600:elapsed.count());
    }
};
[[nodiscard]] constexpr std::string_view world_entry_wait_hint(
    WorldEntryStage stage,std::uint32_t seconds) noexcept
{
    if(seconds<15U||stage==WorldEntryStage::ready)return {};
    switch(stage)
    {
        case WorldEntryStage::simulator_circuit:
            return "LLUDP antwortet noch nicht. F3: Status/Reconnect.";
        case WorldEntryStage::region_handle:
            return "Simulator aktiv, Region-Handle ausstehend.";
        case WorldEntryStage::region_connection:
            return "Region noch nicht aktiviert.";
        case WorldEntryStage::agent_identity:
            return "Agent-/Session-Bindung ausstehend.";
        case WorldEntryStage::agent_position:
            return "AgentPosition vom Simulator ausstehend.";
        default:return "Weltaufbau dauert laenger als erwartet.";
    }
}

[[nodiscard]] constexpr std::string_view describe_world_entry(
    WorldEntryStage stage) noexcept
{
    switch(stage)
    {
        case WorldEntryStage::login:return "Warte auf NexVerse Login";
        case WorldEntryStage::simulator_circuit:
            return "Warte auf Simulator / LLUDP";
        case WorldEntryStage::region_handle:
            return "Region-Handle fehlt";
        case WorldEntryStage::region_connection:
            return "Warte auf aktive Startregion";
        case WorldEntryStage::agent_identity:
            return "Agent-ID oder Session-ID fehlt";
        case WorldEntryStage::agent_position:
            return "Warte auf AgentPosition";
        case WorldEntryStage::ready:return "3D-Welt freigegeben";
    }
    return "Verbindungsstatus unbekannt";
}
}
