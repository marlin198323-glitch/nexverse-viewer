/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/caps_world_events.hpp"
#include "nexverse/protocol/grid_info.hpp"

#include <array>
#include <algorithm>
#include <charconv>
#include <cctype>
#include <string_view>
#include <cstddef>
#include <cmath>
#include <limits>
#include <sstream>

namespace
{
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;

const LlsdValue* first_map_in_array(
    const LlsdValue& body,
    std::string_view key)
{
    const LlsdValue* value = body.find(key);
    if (value == nullptr ||
        value->type != LlsdType::array ||
        value->array_value.empty() ||
        !value->array_value.front().is_map())
    {
        return nullptr;
    }

    return &value->array_value.front();
}

bool read_string_like(
    const LlsdValue* value,
    std::string& out)
{
    if (value == nullptr)
    {
        return false;
    }

    if (value->type != LlsdType::string &&
        value->type != LlsdType::uri &&
        value->type != LlsdType::uuid)
    {
        return false;
    }

    out.assign(value->as_string());
    return !out.empty();
}

bool read_u32(
    const LlsdValue* value,
    std::uint32_t& out)
{
    if (value == nullptr ||
        value->type != LlsdType::integer ||
        value->integer_value < 0 ||
        value->integer_value >
            static_cast<std::int64_t>(
                std::numeric_limits<std::uint32_t>::max()))
    {
        return false;
    }

    out = static_cast<std::uint32_t>(value->integer_value);
    return true;
}

bool read_u16(
    const LlsdValue* value,
    std::uint16_t& out)
{
    std::uint32_t parsed = 0U;
    if (!read_u32(value, parsed) || parsed == 0U ||
        parsed > 65535U)
    {
        return false;
    }

    out = static_cast<std::uint16_t>(parsed);
    return true;
}

bool read_region_handle(
    const LlsdValue* value,
    std::uint64_t& out)
{
    if (value == nullptr)
    {
        return false;
    }

    if (value->type == LlsdType::binary &&
        value->binary_value.size() == 8U)
    {
        out = 0U;
        for (const std::uint8_t byte : value->binary_value)
        {
            out = (out << 8U) | static_cast<std::uint64_t>(byte);
        }
        return out != 0U;
    }

    if (value->type == LlsdType::integer &&
        value->integer_value > 0)
    {
        // LLSD integer is signed 64-bit. Never truncate region
        // handles to 32 bits: high bits distinguish neighboring grids.
        out = static_cast<std::uint64_t>(value->integer_value);
        return true;
    }

    return false;
}

// A valid IPv6 literal has eight hex groups, or fewer when precisely
// one :: compresses missing groups. IPv4-mapped endings count as two
// groups. Reject impostors such as triple colons and too many groups.
bool valid_ipv6_literal(std::string_view address)
{
    if(address.size()<2U||address.size()>45U)return false;
    const auto compressed=address.find("::");
    if(compressed!=std::string_view::npos&&
       address.find("::",compressed+2U)!=std::string_view::npos)
        return false;
    if(compressed==std::string_view::npos&&
       (address.front()==':'||address.back()==':'))
        return false;
    unsigned groups=0U;
    std::size_t pos=0U;
    while(pos<address.size())
    {
        if(address[pos]==':')
        {
            if(pos!=compressed)return false;
            pos+=2U;
            continue;
        }
        const auto end=address.find(':',pos);
        const auto part=address.substr(pos,
            end==std::string_view::npos?
                std::string_view::npos:end-pos);
        if(part.empty())return false;
        if(part.find('.')!=std::string_view::npos)
        {
            // A mapped IPv4 tail must be the final two groups.
            if(end!=std::string_view::npos||part.size()>15U)
                return false;
            std::size_t start=0U;
            for(unsigned i=0U;i<4U;++i)
            {
                const auto stop=part.find('.',start);
                if(i==3U?stop!=std::string_view::npos:
                         stop==std::string_view::npos)return false;
                const auto octet_text=part.substr(start,
                    stop==std::string_view::npos?
                        std::string_view::npos:stop-start);
                if(octet_text.empty()||octet_text.size()>3U)
                    return false;
                unsigned octet=0U;
                const auto result=std::from_chars(octet_text.data(),
                    octet_text.data()+octet_text.size(),octet);
                if(result.ec!=std::errc{}||
                   result.ptr!=octet_text.data()+octet_text.size()||
                   octet>255U)return false;
                start=stop+1U;
            }
            groups+=2U;
        }
        else
        {
            if(part.size()>4U)return false;
            if(!std::all_of(part.begin(),part.end(),
                  [](unsigned char ch){return std::isxdigit(ch)!=0;}))
                return false;
            ++groups;
        }
        if(groups>8U)return false;
        pos=end==std::string_view::npos?address.size():end;
        if(pos<address.size())
        {
            if(pos==compressed)pos+=2U;
            else ++pos;
        }
    }
    return compressed==std::string_view::npos?
        groups==8U:groups<8U;
}
// CAPS simulator addresses come from untrusted LLSD. Preserve DNS,
// IPv4 and unbracketed IPv6 literals, but never accept URL paths,
// embedded credentials, socket delimiters, or malformed IPv4 octets.
bool valid_simulator_host(std::string_view host)
{
    if(host.empty()||host.size()>253U||
       host.find_first_of("/?#@\\ \r\n\t")!=std::string_view::npos)
        return false;
    if(host.find(':')!=std::string_view::npos)
    {
        // IPv6 SimIP strings carry no brackets; the existing hardened
        // URI validator accepts the bracketed form without zone IDs.
        if(host.find('[')!=std::string_view::npos||
           host.find(']')!=std::string_view::npos||
           std::count(host.begin(),host.end(),':')<2)
            return false;
        return valid_ipv6_literal(host);
    }
    if(!nexverse::protocol::is_valid_http_uri(
        "http://"+std::string{host}+"/"))
        return false;
    const bool dotted_numeric=host.find('.')!=std::string_view::npos&&
        std::all_of(host.begin(),host.end(),[](unsigned char ch)
        {return (ch>='0'&&ch<='9')||ch=='.';});
    if(!dotted_numeric)return true; // DNS name is resolved by transport
    std::size_t start=0U;
    for(int group=0;group<4;++group)
    {
        const auto end=host.find('.',start);
        if(group==3?end!=std::string_view::npos:
                    end==std::string_view::npos)return false;
        const auto part=host.substr(
            start,end==std::string_view::npos
                ?std::string_view::npos:end-start);
        if(part.empty()||part.size()>3U)return false;
        unsigned octet=0U;
        const auto result=std::from_chars(
            part.data(),part.data()+part.size(),octet);
        if(result.ec!=std::errc{}||
           result.ptr!=part.data()+part.size()||octet>255U)
            return false;
        start=end+1U;
    }
    return true;
}
bool valid_simulator_endpoint(std::string_view endpoint)
{
    // EstablishAgentCommunication has an explicit host:port value.
    // URI-like strings and omitted/overflowing ports are not sockets.
    if(endpoint.empty()||endpoint.size()>300U||
       endpoint.find_first_of("/?#@\\ \r\n\t")!=std::string_view::npos)
        return false;
    std::string_view host,port;
    if(endpoint.front()=='[')
    {
        const auto close=endpoint.find(']');
        if(close==std::string_view::npos||close+1U>=endpoint.size()||
           endpoint[close+1U]!=':')return false;
        host=endpoint.substr(1U,close-1U);
        port=endpoint.substr(close+2U);
        if(host.find(':')==std::string_view::npos)return false;
    }
    else
    {
        const auto split=endpoint.rfind(':');
        if(split==std::string_view::npos||
           endpoint.find(':')!=split)return false;
        host=endpoint.substr(0U,split);
        port=endpoint.substr(split+1U);
    }
    if(!valid_simulator_host(host)||port.empty()||port.size()>5U)
        return false;
    unsigned number=0U;
    const auto result=std::from_chars(
        port.data(),port.data()+port.size(),number);
    return result.ec==std::errc{}&&
        result.ptr==port.data()+port.size()&&
        number>=1U&&number<=65535U;
}
bool read_ip(
    const LlsdValue* value,
    std::string& out)
{
    if (value == nullptr)
    {
        return false;
    }

    if (value->type == LlsdType::string)
    {
        out = value->string_value;
        return valid_simulator_host(out);
    }

    if (value->type != LlsdType::binary)
    {
        return false;
    }

    if (value->binary_value.size() == 4U)
    {
        std::ostringstream stream;
        for (std::size_t index = 0U; index < 4U; ++index)
        {
            if (index != 0U)
            {
                stream << '.';
            }
            stream << static_cast<unsigned int>(value->binary_value[index]);
        }
        out = stream.str();
        return true;
    }

    if (value->binary_value.size() == 16U)
    {
        std::ostringstream stream;
        stream << std::hex;
        for (std::size_t index = 0U; index < 16U; index += 2U)
        {
            if (index != 0U)
            {
                stream << ':';
            }
            const std::uint16_t word =
                (static_cast<std::uint16_t>(value->binary_value[index]) << 8U) |
                static_cast<std::uint16_t>(value->binary_value[index + 1U]);
            stream << word;
        }
        out = stream.str();
        return true;
    }

    return false;
}

bool read_number(
    const LlsdValue& value,
    float& out)
{
    if(value.type!=LlsdType::real&&
       value.type!=LlsdType::integer)return false;
    const auto number=value.type==LlsdType::real
        ?value.real_value:static_cast<double>(value.integer_value);
    // Simulator events must not replace the avatar position/look-at
    // with NaN, infinity, or values that overflow float during cast.
    // Preserve ordinary negative coordinates and finite LLSD doubles.
    if(!std::isfinite(number)||
       number>static_cast<double>(std::numeric_limits<float>::max())||
       number<-static_cast<double>(std::numeric_limits<float>::max()))
        return false;
    out=static_cast<float>(number);
    return std::isfinite(out);
}

bool read_vec3(
    const LlsdValue* value,
    nexverse::protocol::Vec3& out)
{
    if (value == nullptr ||
        value->type != LlsdType::array ||
        value->array_value.size() != 3U)
    {
        return false;
    }

    return
        read_number(value->array_value[0U], out.x) &&
        read_number(value->array_value[1U], out.y) &&
        read_number(value->array_value[2U], out.z);
}

bool parse_simulator_info(
    const LlsdValue& map,
    nexverse::protocol::CapsSimulatorInfo& out,
    bool require_seed)
{
    if (!map.is_map())
    {
        return false;
    }

    if (!read_region_handle(map.find("RegionHandle"), out.region_handle) &&
        !read_region_handle(map.find("Handle"), out.region_handle))
    {
        return false;
    }

    if (!read_ip(map.find("SimIP"), out.simulator_ip) &&
        !read_ip(map.find("IP"), out.simulator_ip))
    {
        return false;
    }

    if (!read_u16(map.find("SimPort"), out.simulator_port) &&
        !read_u16(map.find("Port"), out.simulator_port))
    {
        return false;
    }

    if (const LlsdValue* size_x = map.find("RegionSizeX");
        size_x != nullptr)
    {
        if (!read_u32(size_x, out.region_size_x) ||
            (out.region_size_x==0U||out.region_size_x>16384U))
        {
            return false;
        }
    }

    if (const LlsdValue* size_y = map.find("RegionSizeY");
        size_y != nullptr)
    {
        if (!read_u32(size_y, out.region_size_y) ||
            (out.region_size_y==0U||out.region_size_y>16384U))
        {
            return false;
        }
    }

    if (const LlsdValue* seed = map.find("SeedCapability");
        seed != nullptr)
    {
        if (!read_string_like(seed, out.seed_capability) ||
            !nexverse::protocol::is_valid_http_uri(out.seed_capability))
        {
            // The next CAPS worker must not be started on an opaque
            // or malformed seed URI supplied by a broken event.
            return false;
        }
    }
    else if (require_seed)
    {
        return false;
    }

    return true;
}
}

namespace nexverse::protocol
{
std::optional<EnableSimulatorEvent>
parse_enable_simulator_event(const EventQueueEvent& event)
{
    if (event.message != "EnableSimulator" ||
        !event.body.is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* simulator_info =
        first_map_in_array(event.body, "SimulatorInfo");
    if (simulator_info == nullptr)
    {
        return std::nullopt;
    }

    EnableSimulatorEvent parsed;
    if (!parse_simulator_info(
            *simulator_info,
            parsed.simulator,
            false))
    {
        return std::nullopt;
    }

    return parsed;
}

std::optional<EstablishAgentCommunicationEvent>
parse_establish_agent_communication_event(const EventQueueEvent& event)
{
    if (event.message != "EstablishAgentCommunication" ||
        !event.body.is_map())
    {
        return std::nullopt;
    }

    EstablishAgentCommunicationEvent parsed;
    if (!read_string_like(
            event.body.find("agent-id"),
            parsed.agent_id) ||
        !read_string_like(
            event.body.find("sim-ip-and-port"),
            parsed.simulator_endpoint) ||
        !valid_simulator_endpoint(parsed.simulator_endpoint) ||
        !read_string_like(
            event.body.find("seed-capability"),
            parsed.seed_capability) ||
        !is_valid_http_uri(parsed.seed_capability))
    {
        return std::nullopt;
    }

    return parsed;
}

std::optional<TeleportFinishCapsEvent>
parse_teleport_finish_caps_event(const EventQueueEvent& event)
{
    if (event.message != "TeleportFinish" ||
        !event.body.is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* info = first_map_in_array(event.body, "Info");
    if (info == nullptr)
    {
        return std::nullopt;
    }

    TeleportFinishCapsEvent parsed;
    if (!read_string_like(info->find("AgentID"), parsed.agent_id) ||
        !parse_simulator_info(*info, parsed.simulator, true))
    {
        return std::nullopt;
    }

    if (const LlsdValue* flags = info->find("TeleportFlags");
        flags != nullptr)
    {
        if (!read_u32(flags, parsed.teleport_flags))
        {
            return std::nullopt;
        }
    }

    return parsed;
}

std::optional<CrossedRegionCapsEvent>
parse_crossed_region_caps_event(const EventQueueEvent& event)
{
    if (event.message != "CrossedRegion" ||
        !event.body.is_map())
    {
        return std::nullopt;
    }

    const LlsdValue* agent_data =
        first_map_in_array(event.body, "AgentData");
    const LlsdValue* info =
        first_map_in_array(event.body, "Info");
    const LlsdValue* region_data =
        first_map_in_array(event.body, "RegionData");

    if (agent_data == nullptr ||
        info == nullptr ||
        region_data == nullptr)
    {
        return std::nullopt;
    }

    CrossedRegionCapsEvent parsed;
    if (!read_string_like(
            agent_data->find("AgentID"),
            parsed.agent_id) ||
        !read_string_like(
            agent_data->find("SessionID"),
            parsed.session_id) ||
        !read_vec3(
            info->find("Position"),
            parsed.position) ||
        !read_vec3(
            info->find("LookAt"),
            parsed.look_at) ||
        !parse_simulator_info(
            *region_data,
            parsed.simulator,
            true))
    {
        return std::nullopt;
    }

    return parsed;
}
}
