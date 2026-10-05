#include "nexverse/protocol/caps_world_events.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace
{
using nexverse::protocol::LlsdValue;

LlsdValue map_of(
    std::initializer_list<std::pair<const std::string, LlsdValue>> values)
{
    std::map<std::string, LlsdValue, std::less<>> map;
    for (const auto& [key, value] : values)
    {
        map[key] = value;
    }
    return LlsdValue::map(std::move(map));
}

LlsdValue array_of(std::initializer_list<LlsdValue> values)
{
    return LlsdValue::array(std::vector<LlsdValue>{values});
}

std::vector<std::uint8_t> handle_bytes(std::uint64_t value)
{
    std::vector<std::uint8_t> bytes(8U);
    for (std::size_t index = 0U; index < 8U; ++index)
    {
        bytes[index] = static_cast<std::uint8_t>(
            (value >> ((7U - index) * 8U)) & 0xFFU);
    }
    return bytes;
}
}

int main()
{
    using namespace nexverse::protocol;

    constexpr std::uint64_t handle = 0x1122334455667788ULL;

    EventQueueEvent enable;
    enable.message = "EnableSimulator";
    enable.body = map_of({
        {"SimulatorInfo", array_of({
            map_of({
                {"Handle", LlsdValue::binary(handle_bytes(handle))},
                {"IP", LlsdValue::binary({127U, 0U, 0U, 1U})},
                {"Port", LlsdValue::integer(9000)},
                {"RegionSizeX", LlsdValue::integer(256)},
                {"RegionSizeY", LlsdValue::integer(256)}
            })
        })}
    });

    const auto enabled = parse_enable_simulator_event(enable);
    if (!enabled.has_value() ||
        enabled->simulator.region_handle != handle ||
        enabled->simulator.simulator_ip != "127.0.0.1" ||
        enabled->simulator.simulator_port != 9000U)
    {
        std::cerr << "EnableSimulator parse failed\n";
        return 1;
    }

    EventQueueEvent establish;
    establish.message = "EstablishAgentCommunication";
    establish.body = map_of({
        {"agent-id", LlsdValue::uuid("11111111-2222-3333-4444-555555555555")},
        {"sim-ip-and-port", LlsdValue::string("127.0.0.1:9000")},
        {"seed-capability", LlsdValue::uri("https://caps.example/seed")}
    });

    const auto established =
        parse_establish_agent_communication_event(establish);
    if (!established.has_value() ||
        established->simulator_endpoint != "127.0.0.1:9000" ||
        established->seed_capability != "https://caps.example/seed")
    {
        std::cerr << "EstablishAgentCommunication parse failed\n";
        return 2;
    }

    EventQueueEvent teleport;
    teleport.message = "TeleportFinish";
    teleport.body = map_of({
        {"Info", array_of({
            map_of({
                {"AgentID", LlsdValue::uuid("11111111-2222-3333-4444-555555555555")},
                {"SimIP", LlsdValue::binary({10U, 20U, 30U, 40U})},
                {"SimPort", LlsdValue::integer(9100)},
                {"RegionHandle", LlsdValue::binary(handle_bytes(handle))},
                {"SeedCapability", LlsdValue::uri("https://caps.example/new")},
                {"TeleportFlags", LlsdValue::integer(16)},
                {"RegionSizeX", LlsdValue::integer(512)},
                {"RegionSizeY", LlsdValue::integer(256)}
            })
        })}
    });

    const auto teleported = parse_teleport_finish_caps_event(teleport);
    if (!teleported.has_value() ||
        teleported->simulator.region_handle != handle ||
        teleported->simulator.simulator_ip != "10.20.30.40" ||
        teleported->simulator.simulator_port != 9100U ||
        teleported->simulator.region_size_x != 512U ||
        teleported->teleport_flags != 16U)
    {
        std::cerr << "TeleportFinish parse failed\n";
        return 3;
    }

    EventQueueEvent crossed;
    crossed.message = "CrossedRegion";
    crossed.body = map_of({
        {"AgentData", array_of({
            map_of({
                {"AgentID", LlsdValue::uuid("11111111-2222-3333-4444-555555555555")},
                {"SessionID", LlsdValue::uuid("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee")}
            })
        })},
        {"Info", array_of({
            map_of({
                {"Position", array_of({
                    LlsdValue::real(12.0),
                    LlsdValue::real(34.0),
                    LlsdValue::real(56.0)
                })},
                {"LookAt", array_of({
                    LlsdValue::real(1.0),
                    LlsdValue::real(0.0),
                    LlsdValue::real(0.0)
                })}
            })
        })},
        {"RegionData", array_of({
            map_of({
                {"RegionHandle", LlsdValue::binary(handle_bytes(handle))},
                {"SeedCapability", LlsdValue::uri("https://caps.example/crossed")},
                {"SimIP", LlsdValue::binary({192U, 0U, 2U, 5U})},
                {"SimPort", LlsdValue::integer(9200)},
                {"RegionSizeX", LlsdValue::integer(256)},
                {"RegionSizeY", LlsdValue::integer(256)}
            })
        })}
    });

    const auto region = parse_crossed_region_caps_event(crossed);
    if (!region.has_value() ||
        region->position.x != 12.0f ||
        region->position.y != 34.0f ||
        region->position.z != 56.0f ||
        region->simulator.simulator_ip != "192.0.2.5" ||
        region->simulator.simulator_port != 9200U)
    {
        std::cerr << "CrossedRegion parse failed\n";
        return 4;
    }

    EventQueueEvent invalid = teleport;
    invalid.body = LlsdValue::map();
    if (parse_teleport_finish_caps_event(invalid).has_value())
    {
        std::cerr << "Invalid TeleportFinish unexpectedly accepted\n";
        return 5;
    }

    std::cout << "CAPS world event tests passed\n";
    return 0;
}
