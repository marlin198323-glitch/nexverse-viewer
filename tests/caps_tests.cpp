#include "nexverse/protocol/caps.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

namespace
{
class FakeTransport final : public nexverse::net::IHttpTransport
{
public:
    nexverse::net::HttpResponse get_response;
    nexverse::net::HttpResponse post_response;
    std::string last_url;
    std::string last_content_type;
    std::string last_body;
    int get_calls = 0;
    int post_calls = 0;

    nexverse::net::HttpResponse get(
        std::string_view url,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        ++get_calls;
        last_url = std::string{url};
        return get_response;
    }

    nexverse::net::HttpResponse post(
        std::string_view url,
        std::string_view content_type,
        std::string_view body,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        ++post_calls;
        last_url = std::string{url};
        last_content_type = std::string{content_type};
        last_body = std::string{body};
        return post_response;
    }
};
}

int main()
{
    using namespace nexverse::protocol;

    const auto seed_request = build_seed_capability_request(
        {"EventQueueGet", "SimulatorFeatures"});

    const auto seed_request_llsd = parse_llsd_xml(seed_request);
    if (!seed_request_llsd.ok() ||
        !seed_request_llsd.value->is_array() ||
        seed_request_llsd.value->array_value.size() != 2U)
    {
        std::cerr << "Seed capability request encoding failed\n";
        return 1;
    }

    constexpr std::string_view seed_response =
        "<llsd><map>"
        "<key>EventQueueGet</key><uri>https://caps.example.test/event</uri>"
        "<key>SimulatorFeatures</key><string>https://caps.example.test/features</string>"
        "<key>Broken</key><uri>ftp://invalid.example.test/</uri>"
        "</map></llsd>";

    const auto parsed_seed = parse_seed_capability_response(seed_response);
    if (!parsed_seed.success ||
        parsed_seed.capabilities.size() != 2U ||
        parsed_seed.capabilities.at("EventQueueGet") !=
            "https://caps.example.test/event")
    {
        std::cerr << "Seed capability response parsing failed\n";
        return 2;
    }

    const std::string queue_request =
        build_event_queue_request(123, false);
    const auto queue_request_llsd = parse_llsd_xml(queue_request);
    if (!queue_request_llsd.ok() ||
        queue_request_llsd.value->find("ack") == nullptr ||
        queue_request_llsd.value->find("ack")->integer_value != 123)
    {
        std::cerr << "EventQueue request encoding failed\n";
        return 3;
    }

    constexpr std::string_view queue_response =
        "<llsd><map>"
        "<key>id</key><integer>124</integer>"
        "<key>events</key><array>"
        "<map>"
        "<key>message</key><string>EnableSimulator</string>"
        "<key>body</key><map><key>Handle</key><integer>7</integer></map>"
        "</map>"
        "<map>"
        "<key>message</key><string>TeleportFinish</string>"
        "<key>body</key><map><key>SeedCapability</key><uri>https://caps.example.test/new</uri></map>"
        "</map>"
        "</array>"
        "</map></llsd>";

    const auto batch = parse_event_queue_response(queue_response);
    if (!batch.success ||
        !batch.id.has_value() ||
        *batch.id != 124 ||
        batch.events.size() != 2U ||
        batch.events.front().message != "EnableSimulator")
    {
        std::cerr << "EventQueue response parsing failed\n";
        return 4;
    }

    auto transport = std::make_shared<FakeTransport>();
    transport->post_response = {
        200,
        std::string{seed_response},
        {}};

    CapabilityService service{transport};
    const auto discovered = service.discover(
        "https://caps.example.test/seed",
        {"EventQueueGet", "SimulatorFeatures"});

    if (!discovered.success ||
        transport->post_calls != 1 ||
        transport->last_url != "https://caps.example.test/seed" ||
        transport->last_content_type != "application/llsd+xml")
    {
        std::cerr << "Capability discovery transport failed\n";
        return 5;
    }

    transport->get_response = {
        200,
        "<llsd><map><key>MeshRezEnabled</key><boolean>true</boolean></map></llsd>",
        {}};

    const auto features = service.fetch_simulator_features(
        "https://caps.example.test/features");

    if (!features.success ||
        !features.features.is_map() ||
        features.features.find("MeshRezEnabled") == nullptr ||
        !features.features.find("MeshRezEnabled")->boolean_value)
    {
        std::cerr << "SimulatorFeatures request failed\n";
        return 6;
    }

    transport->post_response = {
        200,
        std::string{queue_response},
        {}};

    transport->get_response = {
        200,
        "<llsd><array><map><key>sky</key><string>legacy</string></map></array></llsd>",
        {}};

    const auto generic_llsd = service.fetch_llsd(
        "https://caps.example.test/environment");

    if (!generic_llsd.success ||
        !generic_llsd.value.is_array() ||
        generic_llsd.value.array_value.size() != 1U)
    {
        std::cerr << "Generic LLSD capability request failed\n";
        return 7;
    }

    transport->post_response = {
        200,
        std::string{queue_response},
        {}};

    const auto polled = service.poll_event_queue(
        "https://caps.example.test/event",
        123,
        false);

    if (!polled.success ||
        polled.events.size() != 2U ||
        transport->last_url != "https://caps.example.test/event")
    {
        std::cerr << "EventQueue transport failed\n";
        return 8;
    }

    std::cout << "CAPS discovery and EventQueue tests passed\n";
    return 0;
}
