/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/caps.hpp"

#include "nexverse/protocol/grid_info.hpp"

#include <chrono>
#include <span>
#include <utility>

namespace
{
using nexverse::protocol::LlsdParseResult;
using nexverse::protocol::LlsdType;
using nexverse::protocol::LlsdValue;

LlsdParseResult parse_llsd_response(std::string_view response)
{
    const std::size_t first = response.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos)
    {
        return {std::nullopt, "LLSD response is empty"};
    }

    if (response[first] == '<')
    {
        return nexverse::protocol::parse_llsd_xml(response.substr(first));
    }

    const auto* data = reinterpret_cast<const std::uint8_t*>(
        response.data() + static_cast<std::ptrdiff_t>(first));
    return nexverse::protocol::parse_llsd_binary(
        std::span<const std::uint8_t>{data, response.size() - first});
}

bool string_like(const LlsdValue& value)
{
    return value.type == LlsdType::string ||
           value.type == LlsdType::uri;
}
}

namespace nexverse::protocol
{
std::vector<std::string> default_seed_capabilities()
{
    return {
        "EventQueueGet",
        "SimulatorFeatures",
        "GetTexture",
        "GetMesh",
        "ViewerAsset",
        "FetchInventory2",
        "FetchLib2",
        "GetDisplayNames",
        "GetObjectCost",
        "GetObjectPhysicsData",
        "RenderMaterials",
        "UpdateAvatarAppearance",
        "NewFileAgentInventory",
        "UploadBakedTexture",
        "GetExperienceInfo",
        "AgentPreferences",
        "UpdateAgentLanguage",
        "EnvironmentSettings"
    };
}

std::string build_seed_capability_request(
    const std::vector<std::string>& capabilities)
{
    std::vector<LlsdValue> requested;
    requested.reserve(capabilities.size());

    for (const auto& capability : capabilities)
    {
        if (!capability.empty())
        {
            requested.push_back(LlsdValue::string(capability));
        }
    }

    return serialize_llsd_xml(LlsdValue::array(std::move(requested)));
}

CapabilityDiscoveryResult parse_seed_capability_response(
    std::string_view response)
{
    const auto parsed = parse_llsd_response(response);
    if (!parsed.ok())
    {
        return {false, {}, parsed.error};
    }
    if (!parsed.value->is_map())
    {
        return {false, {}, "Seed capability response is not an LLSD map"};
    }

    CapabilityMap capabilities;
    for (const auto& [name, value] : parsed.value->map_value)
    {
        if (!string_like(value))
        {
            continue;
        }

        const std::string uri{value.as_string()};
        if (!is_valid_http_uri(uri))
        {
            continue;
        }

        capabilities[name] = uri;
    }

    if (capabilities.empty())
    {
        return {
            false,
            {},
            "Seed capability response contains no valid HTTP capability URLs"};
    }

    return {
        true,
        std::move(capabilities),
        "Capability discovery completed"};
}

std::string build_event_queue_request(
    std::optional<std::int32_t> acknowledgement,
    bool done)
{
    std::map<std::string, LlsdValue, std::less<>> request;
    request["ack"] = acknowledgement.has_value()
        ? LlsdValue::integer(*acknowledgement)
        : LlsdValue{};
    request["done"] = LlsdValue::boolean(done);
    return serialize_llsd_xml(LlsdValue::map(std::move(request)));
}

EventQueueBatch parse_event_queue_response(std::string_view response)
{
    const auto parsed = parse_llsd_response(response);
    if (!parsed.ok())
    {
        return {false, std::nullopt, {}, parsed.error};
    }
    if (!parsed.value->is_map())
    {
        return {false, std::nullopt, {}, "EventQueue response is not an LLSD map"};
    }

    EventQueueBatch batch;
    batch.success = true;
    batch.message = "EventQueue batch parsed";

    if (const LlsdValue* id = parsed.value->find("id");
        id != nullptr && id->type == LlsdType::integer)
    {
        batch.id = id->integer_value;
    }

    const LlsdValue* events = parsed.value->find("events");
    if (events == nullptr)
    {
        return batch;
    }
    if (!events->is_array())
    {
        return {
            false,
            batch.id,
            {},
            "EventQueue events field is not an LLSD array"};
    }

    batch.events.reserve(events->array_value.size());
    for (const auto& event_value : events->array_value)
    {
        if (!event_value.is_map())
        {
            continue;
        }

        const LlsdValue* message = event_value.find("message");
        const LlsdValue* body = event_value.find("body");
        if (message == nullptr || !string_like(*message) || body == nullptr)
        {
            continue;
        }

        EventQueueEvent event;
        event.message = std::string{message->as_string()};
        event.body = *body;
        batch.events.push_back(std::move(event));
    }

    return batch;
}

CapabilityService::CapabilityService(
    std::shared_ptr<net::IHttpTransport> transport)
    : transport_{std::move(transport)}
{
}

CapabilityDiscoveryResult CapabilityService::discover(
    std::string_view seed_capability,
    const std::vector<std::string>& requested) const
{
    if (transport_ == nullptr)
    {
        return {false, {}, "HTTP transport unavailable"};
    }
    if (!is_valid_http_uri(seed_capability))
    {
        return {false, {}, "Seed capability URI is invalid"};
    }

    const std::string request = build_seed_capability_request(requested);
    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{20};

    const net::HttpResponse response = transport_->post(
        seed_capability,
        "application/llsd+xml",
        request,
        max_caps_response_bytes,
        connect_timeout,
        total_timeout);

    if (!response.ok())
    {
        return {
            false,
            {},
            response.error.empty()
                ? "Seed capability request failed"
                : response.error};
    }

    return parse_seed_capability_response(response.body);
}

SimulatorFeaturesResult CapabilityService::fetch_simulator_features(
    std::string_view capability_uri) const
{
    const auto result = fetch_llsd(capability_uri);
    if (!result.success)
    {
        return {false, {}, result.message};
    }
    if (!result.value.is_map())
    {
        return {false, {}, "SimulatorFeatures response is not an LLSD map"};
    }

    return {
        true,
        result.value,
        "SimulatorFeatures loaded"};
}

LlsdCapabilityResult CapabilityService::fetch_llsd(
    std::string_view capability_uri) const
{
    if (transport_ == nullptr)
    {
        return {false, {}, "HTTP transport unavailable"};
    }
    if (!is_valid_http_uri(capability_uri))
    {
        return {false, {}, "Capability URI is invalid"};
    }

    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{20};

    const net::HttpResponse response = transport_->get(
        capability_uri,
        max_caps_response_bytes,
        connect_timeout,
        total_timeout);

    if (!response.ok())
    {
        return {
            false,
            {},
            response.error.empty()
                ? "Capability request failed"
                : response.error};
    }

    auto parsed = parse_llsd_response(response.body);
    if (!parsed.ok())
    {
        return {false, {}, parsed.error};
    }

    return {
        true,
        std::move(*parsed.value),
        "LLSD capability loaded"};
}

EventQueueBatch CapabilityService::poll_event_queue(
    std::string_view capability_uri,
    std::optional<std::int32_t> acknowledgement,
    bool done) const
{
    if (transport_ == nullptr)
    {
        return {false, acknowledgement, {}, "HTTP transport unavailable"};
    }
    if (!is_valid_http_uri(capability_uri))
    {
        return {false, acknowledgement, {}, "EventQueue URI is invalid"};
    }

    const std::string request =
        build_event_queue_request(acknowledgement, done);

    constexpr auto connect_timeout = std::chrono::seconds{8};
    constexpr auto total_timeout = std::chrono::seconds{35};

    const net::HttpResponse response = transport_->post(
        capability_uri,
        "application/llsd+xml",
        request,
        max_caps_response_bytes,
        connect_timeout,
        total_timeout);

    if (!response.ok())
    {
        return {
            false,
            acknowledgement,
            {},
            response.error.empty()
                ? "EventQueue request failed"
                : response.error};
    }

    return parse_event_queue_response(response.body);
}
}
