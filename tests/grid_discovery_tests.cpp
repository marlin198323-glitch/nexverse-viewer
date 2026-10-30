#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/grid_discovery.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string_view>
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

    // GridInfo is plain HTTP; the login URL is a password-bearing
    // XML-RPC destination. The XML parser accepts any well-formed URL,
    // but THIS NexVerse-only client must never cache/choose an attacker
    // hostname or unauthorized port from the HTTP response.
    const std::string_view foreign_logins[]{
        "http://evil.invalid:8002/login",
        "http://hg.stadt-nexverse.de.evil.invalid:8002/",
        "http://hg.stadt-nexverse.de--evil.invalid:8002/",
        "http://192.0.2.10:8002/",
        "http://hg.stadt-nexverse.de:8003/",
        "http://hg.stadt-nexverse.de/",
        "https://hg.stadt-nexverse.de:8443/",
        "https://hg.stadt-nexverse.de.attacker.example/",
        "http://hg.stadt-nexverse.de:8002@evil.invalid/",
    };
    for(auto candidate:foreign_logins)
    {
        transport->response={
            200,"<gridinfo><login>"+std::string{candidate}+
            "</login><gridname>Attacker</gridname></gridinfo>",{}};
        const auto refused=discovery.refresh();
        if(!refused.success||refused.source!=GridInfoSource::cache||
           refused.info.grid_name()!="NexVerse"||
           refused.info.login_uri()!="http://hg.stadt-nexverse.de:8002/")
            return 9;
    }
    // ASCII hostnames are case-insensitive; the exact trusted hostname
    // may be advertised in different case or with HTTPS/TLS.
    transport->response={200,
        "<gridinfo><login>http://HG.STADT-NEXVERSE.DE:8002/</login>"
        "<gridname>Case</gridname></gridinfo>",{}};
    const auto case_ok=discovery.refresh();
    if(!case_ok.success||case_ok.source!=GridInfoSource::live||
       case_ok.info.grid_name()!="Case")return 10;

    transport->response={200,
        "<gridinfo><login>https://hg.stadt-nexverse.de:443/</login>"
        "<gridname>TLS</gridname></gridinfo>",{}};
    const auto tls_ok=discovery.refresh();
    if(!tls_ok.success||tls_ok.source!=GridInfoSource::live||
       tls_ok.info.grid_name()!="TLS")return 11;
    transport->response={200,
        "<gridinfo><login>https://hg.stadt-nexverse.de:8002/</login>"
        "<gridname>Secure8002</gridname></gridinfo>",{}};
    const auto secure_port=discovery.refresh();
    if(!secure_port.success||secure_port.source!=GridInfoSource::live||
       secure_port.info.grid_name()!="Secure8002")return 12;

    // A malicious or corrupted disk cache is also untrusted. Even
    // during an outage it cannot become the next login destination.
    {
        std::ofstream output{cache,std::ios::binary|std::ios::trunc};
        output<<"<gridinfo><login>http://evil.invalid:8002/</login>"
                 "<gridname>PoisonedCache</gridname></gridinfo>";
    }
    transport->response={0,{},"offline"};
    const auto poisoned_disk=discovery.refresh();
    if(poisoned_disk.success||poisoned_disk.source!=GridInfoSource::none)
        return 13;
    {
        std::ofstream output{cache,std::ios::binary|std::ios::trunc};
        output<<"<gridinfo><login>http://hg.stadt-nexverse.de:8002/</login>"
                 "<gridname>NexVerse</gridname></gridinfo>";
    }
    const auto restored_cache=discovery.refresh();
    if(!restored_cache.success||
       restored_cache.source!=GridInfoSource::cache||
       restored_cache.info.grid_name()!="NexVerse")return 14;

    std::filesystem::remove_all(root);
    std::cout << "Grid discovery tests passed\n";
    return 0;
}
