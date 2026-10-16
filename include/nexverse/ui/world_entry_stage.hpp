/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <string_view>
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
