/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/llsd.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
inline constexpr std::size_t max_caps_response_bytes = 8U * 1024U * 1024U;

using CapabilityMap = std::map<std::string, std::string, std::less<>>;

struct CapabilityDiscoveryResult
{
    bool success = false;
    CapabilityMap capabilities;
    std::string message;
};

struct EventQueueEvent
{
    std::string message;
    LlsdValue body;
};

struct EventQueueBatch
{
    bool success = false;
    std::optional<std::int32_t> id;
    std::vector<EventQueueEvent> events;
    std::string message;
    // OpenSimulator returns HTTP 502 when a long-poll has no events.
    // HTTP 404 means that this region's EventQueue endpoint is gone.
    bool idle = false;
    bool expired = false;
    long http_status = 0;
};

struct SimulatorFeaturesResult
{
    bool success = false;
    LlsdValue features;
    std::string message;
};

struct LlsdCapabilityResult
{
    bool success = false;
    LlsdValue value;
    std::string message;
};

[[nodiscard]] std::vector<std::string> default_seed_capabilities();
[[nodiscard]] std::string build_seed_capability_request(
    const std::vector<std::string>& capabilities);
[[nodiscard]] CapabilityDiscoveryResult parse_seed_capability_response(
    std::string_view response);

[[nodiscard]] std::string build_event_queue_request(
    std::optional<std::int32_t> acknowledgement,
    bool done);
[[nodiscard]] EventQueueBatch parse_event_queue_response(
    std::string_view response);

class CapabilityService
{
public:
    explicit CapabilityService(std::shared_ptr<net::IHttpTransport> transport);

    [[nodiscard]] CapabilityDiscoveryResult discover(
        std::string_view seed_capability,
        const std::vector<std::string>& requested = default_seed_capabilities()) const;

    [[nodiscard]] SimulatorFeaturesResult fetch_simulator_features(
        std::string_view capability_uri) const;

    [[nodiscard]] LlsdCapabilityResult fetch_llsd(
        std::string_view capability_uri) const;

    [[nodiscard]] EventQueueBatch poll_event_queue(
        std::string_view capability_uri,
        std::optional<std::int32_t> acknowledgement,
        bool done = false) const;

private:
    std::shared_ptr<net::IHttpTransport> transport_;
};
}
