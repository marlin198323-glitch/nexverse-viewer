#include "nexverse/protocol/caps_world_events.hpp"

#include <iostream>
#include <limits>
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

    // A CAPS CrossedRegion can be replayed after reconnect or supplied
    // with malformed LLSD. Reject unsafe simulator positions/LookAt
    // BEFORE they overwrite the world-model pose or camera.
    auto position_of=[](EventQueueEvent& e)->LlsdValue& {
        return e.body.map_value.at("Info").array_value.at(0U)
            .map_value.at("Position");
    };
    auto look_at_of=[](EventQueueEvent& e)->LlsdValue& {
        return e.body.map_value.at("Info").array_value.at(0U)
            .map_value.at("LookAt");
    };
    auto region_of=[](EventQueueEvent& e)->LlsdValue& {
        return e.body.map_value.at("RegionData").array_value.at(0U);
    };
    const double nan=std::numeric_limits<double>::quiet_NaN();
    const double inf=std::numeric_limits<double>::infinity();
    for(const double bad:{nan,inf,-inf,1.0e100,-1.0e100})
    {
        auto bad_pose=crossed;
        position_of(bad_pose).array_value[0U]=LlsdValue::real(bad);
        if(parse_crossed_region_caps_event(bad_pose))return 8;
        auto bad_look=crossed;
        look_at_of(bad_look).array_value[2U]=LlsdValue::real(bad);
        if(parse_crossed_region_caps_event(bad_look))return 9;
    }
    auto valid_negative=crossed;
    position_of(valid_negative).array_value[0U]=
        LlsdValue::real(-1024.5);
    if(const auto valid=parse_crossed_region_caps_event(valid_negative);
       !valid||valid->position.x!=-1024.5f)return 10;
    // A malformed port cannot be converted into a default or wrapped
    // simulator UDP socket.
    for(const std::int64_t bad_port:{0LL,65536LL,4294967297LL,-1LL})
    {
        auto bad=crossed;
        region_of(bad).map_value["SimPort"]=LlsdValue::integer(bad_port);
        if(parse_crossed_region_caps_event(bad))return 11;
    }
    auto bad_size=crossed;
    region_of(bad_size).map_value["RegionSizeX"]=
        LlsdValue::integer(4294967297LL);
    if(parse_crossed_region_caps_event(bad_size))return 12;
    // Region size is used for terrain/viewport planning. Never let an
    // unbounded LLSD region-size value allocate enormous resources.
    for(const auto dimension:{0LL,16385LL,4294967295LL})
    {
        auto x=crossed;
        region_of(x).map_value["RegionSizeX"]=
            LlsdValue::integer(dimension);
        if(parse_crossed_region_caps_event(x))return 16;
        auto y=crossed;
        region_of(y).map_value["RegionSizeY"]=
            LlsdValue::integer(dimension);
        if(parse_crossed_region_caps_event(y))return 17;
    }
    auto variable_region=crossed;
    region_of(variable_region).map_value["RegionSizeX"]=
        LlsdValue::integer(8192);
    region_of(variable_region).map_value["RegionSizeY"]=
        LlsdValue::integer(4096);
    if(const auto good=parse_crossed_region_caps_event(variable_region);
       !good||good->simulator.region_size_x!=8192U||
       good->simulator.region_size_y!=4096U)
        return 18;
    // A positive int64 RegionHandle must not truncate to uint32.
    auto high_handle=crossed;
    region_of(high_handle).map_value["RegionHandle"]=
        LlsdValue::integer(0x11223344556677LL);
    if(const auto good=parse_crossed_region_caps_event(high_handle);
       !good||good->simulator.region_handle!=0x11223344556677ULL)
        return 13;
    for(const auto* invalid_seed:{
        "ftp://caps.example/seed",
        "https://user@caps.example/seed",
        "https://caps.example/seed#fragment"})
    {
        auto bad=crossed;
        region_of(bad).map_value["SeedCapability"]=
            LlsdValue::uri(invalid_seed);
        if(parse_crossed_region_caps_event(bad))return 14;
    }
    // A syntactically accepted, but structurally invalid
    // EstablishAgentCommunication previously appeared as VALIDATED in
    // logs. Never advertise an unusable socket or untrusted seed.
    for(const auto* invalid:{
        "", "192.0.2.5", "192.0.2.5:0", "192.0.2.5:65536",
        "192.0.2.5:99999", "192.0.2.5:-1",
        "192.0.2.5:9999junk", "256.0.2.5:9000",
        "192.0.2.999:9000", "192.0.2.5:9000/path",
        "https://evil.invalid:9000", "192.0.2.5:9000@evil.invalid",
        "[2001:db8::1]:0", "[2001:db8::1]:65536",
        "2001:db8::1:9000", "[2001:db8::1]9000"})
    {
        auto bad=establish;
        bad.body.map_value["sim-ip-and-port"]=
            LlsdValue::string(invalid);
        if(parse_establish_agent_communication_event(bad))return 19;
    }
    for(const auto* valid:{
        "127.0.0.1:9000", "192.0.2.5:65535",
        "simulator.nexverse.example:9000", "[2001:db8::1]:9000"})
    {
        auto good=establish;
        good.body.map_value["sim-ip-and-port"]=
            LlsdValue::string(valid);
        if(const auto e=parse_establish_agent_communication_event(good);
           !e||e->simulator_endpoint!=valid)return 20;
    }
    for(const auto* invalid:{
        "", "file:///tmp/token", "http://evil.invalid:443@caps.test/",
        "http://caps.example/seed#fragment", "https://caps.example\\bad/"})
    {
        auto bad=establish;
        bad.body.map_value["seed-capability"]=LlsdValue::uri(invalid);
        if(parse_establish_agent_communication_event(bad))return 21;
    }
    auto valid_seed=establish;
    valid_seed.body.map_value["seed-capability"]=
        LlsdValue::uri("https://caps.example:443/seed?identity=opaque");
    if(!parse_establish_agent_communication_event(valid_seed))return 22;
    // Host-only and bracketed-port IPv6 parsing must reject malformed
    // hextet counts, triple-colons, oversized segments and mapped
    // IPv4 octets without disabling legitimate IPv6 socket handoff.
    for(const auto* invalid:{
        "[1:2:3]:9000", "[1:::2]:9000",
        "[1:2:3:4:5:6:7:8:9]:9000",
        "[fffff::1]:9000", "[::ffff:999.1.2.3]:9000",
        "[1::2::3]:9000", "[]:9000"})
    {
        auto bad=establish;
        bad.body.map_value["sim-ip-and-port"]=LlsdValue::string(invalid);
        if(parse_establish_agent_communication_event(bad))return 30;
    }
    for(const auto* valid:{
        "[::1]:9000", "[2001:db8::1]:9000",
        "[1:2:3:4:5:6:7:8]:9000",
        "[::ffff:192.0.2.5]:9000"})
    {
        auto good=establish;
        good.body.map_value["sim-ip-and-port"]=LlsdValue::string(valid);
        if(!parse_establish_agent_communication_event(good))return 31;
    }
    // Every CAPS event carrying simulator address text shares the
    // same strict host check; binary IPv4/IPv6 remains interoperable.
    for(const auto* invalid:{
        "", "999.1.2.3", "10.20.30.999", "1.2.3",
        "1.2.3.4/path", "host:9000", "bad host",
        "fake.invalid@127.0.0.1"})
    {
        auto bad=teleport;
        bad.body.map_value.at("Info").array_value.at(0U)
           .map_value["SimIP"]=LlsdValue::string(invalid);
        if(parse_teleport_finish_caps_event(bad))return 23;
    }
    for(const auto* valid:{
        "192.0.2.5", "my-region.example", "2001:db8::1"})
    {
        auto good=teleport;
        good.body.map_value.at("Info").array_value.at(0U)
           .map_value["SimIP"]=LlsdValue::string(valid);
        if(!parse_teleport_finish_caps_event(good))return 24;
    }
    // EnableSimulator/CrossedRegion share the exact same string-IP
    // validator as TeleportFinish (binary IPv4/IPv6 remain intact).
    auto bad_enable=enable;
    bad_enable.body.map_value.at("SimulatorInfo").array_value.at(0U)
        .map_value["IP"]=LlsdValue::string("192.0.2.999");
    if(parse_enable_simulator_event(bad_enable))return 25;
    auto hostname_enable=enable;
    hostname_enable.body.map_value.at("SimulatorInfo").array_value.at(0U)
        .map_value["IP"]=LlsdValue::string("region.example");
    if(!parse_enable_simulator_event(hostname_enable))return 26;
    auto bad_crossed=crossed;
    region_of(bad_crossed).map_value["SimIP"]=
        LlsdValue::string("10.0.0.0:9999");
    if(parse_crossed_region_caps_event(bad_crossed))return 27;
    auto ipv6_crossed=crossed;
    region_of(ipv6_crossed).map_value["SimIP"]=
        LlsdValue::string("2001:db8::1");
    if(!parse_crossed_region_caps_event(ipv6_crossed))return 28;
    auto binary_ipv6=crossed;
    region_of(binary_ipv6).map_value["SimIP"]=
        LlsdValue::binary({0x20U,0x01U,0x0dU,0xb8U,0U,0U,0U,0U,
                            0U,0U,0U,0U,0U,0U,0U,1U});
    if(const auto parsed=parse_crossed_region_caps_event(binary_ipv6);
       !parsed||parsed->simulator.simulator_ip!="2001:db8:0:0:0:0:0:1")
        return 29;
    // A normal HTTPS seed, 512m region and valid negative position
    // remain fully compatible with modern OpenSim grid events.
    if(!parse_crossed_region_caps_event(crossed))return 15;

    std::cout << "CAPS world event tests passed\n";
    return 0;
}
