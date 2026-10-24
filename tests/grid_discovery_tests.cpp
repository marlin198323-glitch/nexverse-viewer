#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/grid_discovery.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace
{
class FakeTransport final : public nexverse::net::IHttpTransport
{
public:
    nexverse::net::HttpResponse response;
    int calls = 0;
    std::string last_get_url;

    nexverse::net::HttpResponse get(
        std::string_view url,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        ++calls;
        last_get_url = std::string{url};
        return response;
    }

    nexverse::net::HttpResponse post(
        std::string_view,
        std::string_view,
        std::string_view,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        return {0, {}, "unexpected POST"};
    }
};
}

int main()
{
    using nexverse::protocol::GridDiscoveryService;
    using nexverse::protocol::GridInfoSource;

    const auto root = std::filesystem::temp_directory_path() /
        ("ogl-grid-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    const auto cache = root / "grid_info.xml";

    auto transport = std::make_shared<FakeTransport>();
    transport->response = {
        200,
        "<gridinfo><login>http://hg.stadt-nexverse.de:8002/</login><gridname>NexVerse</gridname></gridinfo>",
        {}};

    GridDiscoveryService discovery{transport, cache};

    const auto first = discovery.refresh();
    if (!first.success || first.source != GridInfoSource::live || !first.changed) return 1;
    if (transport->last_get_url != nexverse::protocol::nexverse_grid_info_endpoint) return 2;
    if (!std::filesystem::exists(cache)) return 3;

    const auto second = discovery.refresh();
    if (!second.success || second.source != GridInfoSource::live || second.changed) return 3;

    transport->response = {0, {}, "offline"};
    const auto fallback = discovery.refresh();
    if (!fallback.success || fallback.source != GridInfoSource::cache) return 4;

    transport->response = {200, "<gridinfo><gridname>broken</gridname></gridinfo>", {}};
    const auto invalid = discovery.refresh();
    if (!invalid.success || invalid.source != GridInfoSource::cache ||
        invalid.info.grid_name() != "NexVerse") return 5;

    transport->response={
        200,
        "<gridinfo><login>http://hg.stadt-nexverse.de@evil.invalid/login</login>"
        "<gridname>Poisoned</gridname></gridinfo>",{}};
    const auto poisoned=discovery.refresh();
    if(!poisoned.success||poisoned.source!=GridInfoSource::cache||
       poisoned.info.grid_name()!="NexVerse")return 7;
    transport->response={0,{},"offline"};
    const auto still_safe=discovery.refresh();
    if(!still_safe.success||still_safe.source!=GridInfoSource::cache||
       still_safe.info.grid_name()!="NexVerse")return 8;

    if (transport->calls != 6) return 6;

    std::filesystem::remove_all(root);
    std::cout << "Grid discovery tests passed\n";
    return 0;
}
