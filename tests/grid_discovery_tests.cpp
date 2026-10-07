/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

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
};

std::filesystem::path unique_test_directory()
{
    const auto suffix = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() /
           ("opengenesislink-grid-discovery-" + std::to_string(suffix));
}
}

int main()
{
    using nexverse::protocol::GridDiscoveryService;
    using nexverse::protocol::GridInfoSource;

    const auto directory = unique_test_directory();
    const auto cache = directory / "grid_info.xml";
    const auto transport = std::make_shared<FakeTransport>();

    transport->response.status_code = 200;
    transport->response.body =
        "<gridinfo>"
        "<platform>OpenSim</platform>"
        "<login>http://hg.stadt-nexverse.de:8002/</login>"
        "<gridname>NexVerse</gridname>"
        "<gridnick>NexVerse</gridnick>"
        "</gridinfo>";

    GridDiscoveryService service{
        transport,
        cache,
        "http://test.invalid/get_grid_info"};

    const auto first = service.refresh();
    if (!first.success ||
        first.source != GridInfoSource::live ||
        !first.changed ||
        first.info.grid_name() != "NexVerse")
    {
        std::cerr << "Initial live discovery failed\n";
        std::filesystem::remove_all(directory);
        return 1;
    }

    if (!std::filesystem::exists(cache))
    {
        std::cerr << "Valid live GridInfo was not cached\n";
        std::filesystem::remove_all(directory);
        return 2;
    }

    const auto second = service.refresh();
    if (!second.success ||
        second.source != GridInfoSource::live ||
        second.changed)
    {
        std::cerr << "Unchanged live GridInfo was not detected\n";
        std::filesystem::remove_all(directory);
        return 3;
    }

    transport->response = nexverse::net::HttpResponse{
        0,
        {},
        "simulated network failure"};

    const auto fallback = service.refresh();
    if (!fallback.success ||
        fallback.source != GridInfoSource::cache ||
        fallback.info.login_uri() != "http://hg.stadt-nexverse.de:8002/")
    {
        std::cerr << "Valid cache fallback failed\n";
        std::filesystem::remove_all(directory);
        return 4;
    }

    transport->response = nexverse::net::HttpResponse{
        200,
        "<gridinfo><gridname>Broken</gridname></gridinfo>",
        {}};

    const auto invalid_live = service.refresh();
    if (!invalid_live.success ||
        invalid_live.source != GridInfoSource::cache ||
        invalid_live.info.grid_name() != "NexVerse")
    {
        std::cerr << "Invalid live GridInfo replaced valid cache\n";
        std::filesystem::remove_all(directory);
        return 5;
    }

    const auto empty_directory = unique_test_directory();
    const auto empty_cache = empty_directory / "grid_info.xml";
    const auto failing_transport = std::make_shared<FakeTransport>();
    failing_transport->response = nexverse::net::HttpResponse{
        0,
        {},
        "offline"};

    GridDiscoveryService no_cache{
        failing_transport,
        empty_cache,
        "http://test.invalid/get_grid_info"};

    const auto unavailable = no_cache.refresh();
    if (unavailable.success || unavailable.source != GridInfoSource::none)
    {
        std::cerr << "Discovery without live data or cache must fail\n";
        std::filesystem::remove_all(directory);
        std::filesystem::remove_all(empty_directory);
        return 6;
    }

    if (transport->calls < 4)
    {
        std::cerr << "Discovery did not perform a live request on every refresh\n";
        std::filesystem::remove_all(directory);
        std::filesystem::remove_all(empty_directory);
        return 7;
    }

    std::filesystem::remove_all(directory);
    std::filesystem::remove_all(empty_directory);

    std::cout << "Grid discovery tests passed\n";
    return 0;
}
