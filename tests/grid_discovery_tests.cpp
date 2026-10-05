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

    nexverse::net::HttpResponse get(
        std::string_view,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        ++calls;
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

    GridDiscoveryService discovery{transport, cache, "http://test.invalid/get_grid_info"};

    const auto first = discovery.refresh();
    if (!first.success || first.source != GridInfoSource::live || !first.changed) return 1;
    if (!std::filesystem::exists(cache)) return 2;

    const auto second = discovery.refresh();
    if (!second.success || second.source != GridInfoSource::live || second.changed) return 3;

    transport->response = {0, {}, "offline"};
    const auto fallback = discovery.refresh();
    if (!fallback.success || fallback.source != GridInfoSource::cache) return 4;

    transport->response = {200, "<gridinfo><gridname>broken</gridname></gridinfo>", {}};
    const auto invalid = discovery.refresh();
    if (!invalid.success || invalid.source != GridInfoSource::cache ||
        invalid.info.grid_name() != "NexVerse") return 5;

    if (transport->calls != 4) return 6;

    std::filesystem::remove_all(root);
    std::cout << "Grid discovery tests passed\n";
    return 0;
}
