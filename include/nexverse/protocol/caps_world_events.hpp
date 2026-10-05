/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace nexverse::protocol
{
struct CapsSimulatorInfo
{
    std::uint64_t region_handle = 0U;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0U;
    std::string seed_capability;
    std::uint32_t region_size_x = 256U;
    std::uint32_t region_size_y = 256U;
};

struct EnableSimulatorEvent
{
    CapsSimulatorInfo simulator;
};

struct EstablishAgentCommunicationEvent
{
    std::string agent_id;
    std::string simulator_endpoint;
    std::string seed_capability;
};

struct TeleportFinishCapsEvent
{
    std::string agent_id;
    std::uint32_t teleport_flags = 0U;
    CapsSimulatorInfo simulator;
};

struct CrossedRegionCapsEvent
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Vec3 look_at;
    CapsSimulatorInfo simulator;
};

[[nodiscard]] std::optional<EnableSimulatorEvent>
parse_enable_simulator_event(const EventQueueEvent& event);

[[nodiscard]] std::optional<EstablishAgentCommunicationEvent>
parse_establish_agent_communication_event(const EventQueueEvent& event);

[[nodiscard]] std::optional<TeleportFinishCapsEvent>
parse_teleport_finish_caps_event(const EventQueueEvent& event);

[[nodiscard]] std::optional<CrossedRegionCapsEvent>
parse_crossed_region_caps_event(const EventQueueEvent& event);
}
