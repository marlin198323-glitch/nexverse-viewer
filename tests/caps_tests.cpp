#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/event_queue_recovery.hpp"

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
    std::chrono::milliseconds last_post_timeout{0};
    std::chrono::milliseconds last_post_connect_timeout{0};

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
        std::chrono::milliseconds connect_timeout,
        std::chrono::milliseconds total_timeout) override
    {
        last_post_connect_timeout = connect_timeout;
        last_post_timeout = total_timeout;
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

    const auto all_seed_names=default_seed_capabilities();
    for(const auto& name:std::vector<std::string>{
            "FetchInventory2","FetchInventoryDescendents2",
            "InventoryThumbnailUpload","GetExperienceInfo",
            "GetExperiences","AgentExperiences","ExperiencePreferences",
            "FindExperienceByName","GetAdminExperiences",
            "GetCreatorExperiences","GroupExperiences",
            "IsExperienceAdmin","UpdateExperience"})
    {
        if(std::count(all_seed_names.begin(),all_seed_names.end(),name)!=1)
        {
            std::cerr<<"Firestorm seed CAP missing/duplicated: "<<name<<'\n';
            return 30;
        }
    }
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

    // Never deliver events which the next poll cannot acknowledge:
    // otherwise a broken server/proxy can duplicate inventory/teleport.
    const auto without_id=parse_event_queue_response(
        "<llsd><map><key>events</key><array>"
        "<map><key>message</key><string>TeleportFinish</string>"
        "<key>body</key><map/></map></array></map></llsd>");
    if(without_id.success||!without_id.events.empty()||
       without_id.message.find("acknowledgement ID")==std::string::npos)
       return 42;
    // An empty heartbeat without ID is a harmless, unacknowledgeable
    // idle reply and will be paced by the region worker.
    const auto empty_without_id=parse_event_queue_response(
        "<llsd><map><key>events</key><array/></map></llsd>");
    if(!empty_without_id.success||
       empty_without_id.id.has_value()||
       !empty_without_id.events.empty())return 43;

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

    // v0.4: forged CAPS authority must be rejected BEFORE the POST.
    const auto posts_before=transport->post_calls;
    const auto url_before=transport->last_url;
    const auto malicious=service.discover(
        "https://caps.example.test@evil.invalid/seed",
        {"EventQueueGet"});
    if(malicious.success||transport->post_calls!=posts_before||
       transport->last_url!=url_before)return 40;
    constexpr std::string_view poisoned_seed=
        "<llsd><map>"
        "<key>EventQueueGet</key>"
        "<uri>https://caps.example.test@evil.invalid/steal-token</uri>"
        "<key>GetTexture</key><string>https://caps.example.test:99999/a</string>"
        "<key>SimulatorFeatures</key>"
        "<uri>https://caps.example.test/features</uri>"
        "</map></llsd>";
    const auto safe_seed=parse_seed_capability_response(poisoned_seed);
    if(!safe_seed.success||safe_seed.capabilities.size()!=1U||
       !safe_seed.capabilities.contains("SimulatorFeatures"))
        return 41;

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

    if (transport->last_post_timeout < std::chrono::seconds{70} ||
        transport->last_post_connect_timeout > std::chrono::seconds{10})
    {
        std::cerr << "EventQueue timeout shorter than OpenSim long-poll window\n";
        return 9;
    }

    transport->post_response = {502, {}, "HTTP server returned status 502"};
    const auto idle = service.poll_event_queue(
        "https://caps.example.test/event", 124, false);
    if (idle.success || !idle.idle || idle.expired ||
        idle.http_status != 502 || idle.id != 124)
    {
        std::cerr << "Expected OpenSim idle 502 was treated as an error\n";
        return 10;
    }
    transport->post_response = {404, {}, "HTTP server returned status 404"};
    const auto expired = service.poll_event_queue(
        "https://caps.example.test/event", 124, false);
    if (expired.success || expired.idle || !expired.expired ||
        expired.http_status != 404 || expired.id != 124)
    {
        std::cerr << "EventQueue 404 did not expire the endpoint\n";
        return 11;
    }
    transport->post_response = {503, {}, "HTTP server returned status 503"};
    const auto unavailable = service.poll_event_queue(
        "https://caps.example.test/event", 124, false);
    if (unavailable.success || unavailable.idle || unavailable.expired ||
        unavailable.http_status != 503 ||
        unavailable.message.find("503") == std::string::npos)
    {
        std::cerr << "EventQueue failure diagnostics omitted HTTP status\n";
        return 12;
    }
    transport->post_response = {0, {}, "HTTP request failed: Timeout was reached"};
    const auto timeout = service.poll_event_queue(
        "https://caps.example.test/event", 124, false);
    if (timeout.success || timeout.idle || timeout.expired ||
        timeout.http_status != 0 ||
        timeout.message.find("Timeout") == std::string::npos)
    {
        std::cerr << "EventQueue transport timeout was not diagnosed\n";
        return 13;
    }
    transport->post_response = {200, "<invalid>", {}};
    const auto bad_xml = service.poll_event_queue(
        "https://caps.example.test/event", 124, false);
    if (bad_xml.success || bad_xml.idle ||
        bad_xml.message.empty() || bad_xml.http_status != 200)
    {
        std::cerr << "Malformed EventQueue LLSD was not diagnosed\n";
        return 14;
    }

    EventQueueRecovery recovery;
    if (recovery.failures() != 0U)
    {
        std::cerr << "EventQueue initial failure count incorrect\n";
        return 15;
    }
    const auto first = recovery.on_failure(false);
    const auto second = recovery.on_failure(false);
    const auto third = recovery.on_failure(false);
    if (first.failures != 1U || first.refresh_seed ||
        first.pause != std::chrono::seconds{1} ||
        second.failures != 2U || second.refresh_seed ||
        second.pause != std::chrono::seconds{2} ||
        third.failures != 3U || !third.refresh_seed ||
        third.pause != std::chrono::seconds{4})
    {
        std::cerr << "EventQueue retry/rediscovery cadence incorrect\n";
        return 16;
    }
    recovery.on_contact(); // Real event batch OR expected idle 502.
    if (recovery.failures() != 0U)
    {
        std::cerr << "EventQueue health contact did not clear failures\n";
        return 17;
    }
    if (!recovery.on_failure(true).refresh_seed)
    {
        std::cerr << "EventQueue 404 must trigger immediate seed refresh\n";
        return 18;
    }
    for (int i = 0; i < 12; ++i) (void)recovery.on_failure(false);
    if (recovery.on_failure(false).pause > std::chrono::seconds{12})
    {
        std::cerr << "EventQueue retry backoff not bounded\n";
        return 19;
    }

    // The LOGIN worker now retries both transient seed discovery
    // failures and a seed that initially lacks EventQueueGet. Exercise
    // the real CAPS HTTP codec used by that worker, not just the
    // backoff arithmetic, before marking the endpoint available.
    auto recover_transport=std::make_shared<FakeTransport>();
    CapabilityService recover_service{recover_transport};
    recover_transport->post_response={
        503,{},"temporary login simulator seed unavailable"};
    const auto unavailable_seed=recover_service.discover(
        "https://caps.example.test/seed",{"EventQueueGet"});
    if(unavailable_seed.success||recover_transport->post_calls!=1)
        return 42;
    EventQueueRecovery retry_seed;
    const auto missing_decision=retry_seed.on_failure(true);
    if(!missing_decision.refresh_seed||
       missing_decision.pause!=std::chrono::seconds{1})return 43;
    recover_transport->post_response={
        200,
        "<llsd><map><key>SimulatorFeatures</key>"
        "<uri>https://caps.example.test/features</uri></map></llsd>",
        {}};
    const auto initial_no_queue=recover_service.discover(
        "https://caps.example.test/seed",{"EventQueueGet"});
    if(!initial_no_queue.success||
       initial_no_queue.capabilities.contains("EventQueueGet")||
       recover_transport->post_calls!=2)return 44;
    const auto next_decision=retry_seed.on_failure(true);
    if(!next_decision.refresh_seed||
       next_decision.pause!=std::chrono::seconds{2})return 45;
    recover_transport->post_response={
        200,std::string{seed_response},{}};
    const auto recovered_seed=recover_service.discover(
        "https://caps.example.test/seed",{"EventQueueGet"});
    if(!recovered_seed.success||
       !recovered_seed.capabilities.contains("EventQueueGet")||
       recovered_seed.capabilities.at("EventQueueGet")!=
           "https://caps.example.test/event"||
       recover_transport->post_calls!=3)return 46;
    retry_seed.on_contact();
    if(retry_seed.failures()!=0U)return 47;
    // HTTP404 invalidates the old queue; a NEW validated seed
    // response must be used on the next poll, not an old ACK.
    recover_transport->post_response={404,{},"expired"};
    const auto old_queue=recover_service.poll_event_queue(
        recovered_seed.capabilities.at("EventQueueGet"),
        124,false);
    if(!old_queue.expired||old_queue.http_status!=404)
        return 48;
    recover_transport->post_response={
        200,std::string{seed_response},{}};
    const auto renewed_seed=recover_service.discover(
        "https://caps.example.test/seed",{"EventQueueGet"});
    if(!renewed_seed.success||
       renewed_seed.capabilities.at("EventQueueGet")!=
           "https://caps.example.test/event")return 49;
    recover_transport->post_response={
        200,std::string{queue_response},{}};
    const auto renewed_batch=recover_service.poll_event_queue(
        renewed_seed.capabilities.at("EventQueueGet"),
        std::nullopt,false);
    if(!renewed_batch.success||renewed_batch.id!=124||
       renewed_batch.events.size()!=2U)return 50;

    std::cout << "CAPS discovery and EventQueue tests passed\n";
    return 0;
}
