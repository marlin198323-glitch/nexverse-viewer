#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/login.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

namespace
{
class FakeTransport final : public nexverse::net::IHttpTransport
{
public:
    nexverse::net::HttpResponse response;
    std::string posted_body;
    std::string posted_url;

    nexverse::net::HttpResponse get(
        std::string_view,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        return {0, {}, "unexpected GET"};
    }

    nexverse::net::HttpResponse post(
        std::string_view url,
        std::string_view,
        std::string_view body,
        std::size_t,
        std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        posted_url = std::string{url};
        posted_body = std::string{body};
        return response;
    }
};
}

int main()
{
    using nexverse::protocol::LoginCredentials;
    using nexverse::protocol::LoginService;
    using nexverse::protocol::hash_login_password;
    using nexverse::protocol::parse_login_xml_rpc_response;

    if (hash_login_password("test") != "$1$098f6bcd4621d373cade4e832627b4f6")
    {
        std::cerr << "Password MD5 format mismatch\n";
        return 1;
    }

    LoginCredentials credentials{"Jane", "Doe", "secret", "home"};
    const std::string request =
        nexverse::protocol::build_login_xml_rpc(credentials, "OpenGenesisLINK Viewer", "0.1.0-alpha.1");

    if (request.find("login_to_simulator") == std::string::npos ||
        request.find("<name>first</name>") == std::string::npos ||
        request.find("<string>Jane</string>") == std::string::npos ||
        request.find("secret") != std::string::npos ||
        request.find("$1$5ebe2294ecd0e0f08eab7690d2a6ee69") == std::string::npos)
    {
        std::cerr << "Login XML-RPC request is invalid or exposes plaintext password\n";
        return 2;
    }

    constexpr auto success_xml = R"XML(
<methodResponse><params><param><value><struct>
<member><name>login</name><value><string>true</string></value></member>
<member><name>message</name><value><string>Welcome</string></value></member>
<member><name>agent_id</name><value><string>11111111-1111-1111-1111-111111111111</string></value></member>
<member><name>session_id</name><value><string>22222222-2222-2222-2222-222222222222</string></value></member>
<member><name>secure_session_id</name><value><string>33333333-3333-3333-3333-333333333333</string></value></member>
<member><name>circuit_code</name><value><int>424242</int></value></member>
<member><name>sim_ip</name><value><string>192.0.2.44</string></value></member>
<member><name>sim_port</name><value><int>9000</int></value></member>
<member><name>region_x</name><value><int>1000</int></value></member>
<member><name>region_y</name><value><int>1001</int></value></member>
<member><name>seed_capability</name><value><string>https://caps.example.test/seed</string></value></member>
<member><name>first_name</name><value><string>Jane</string></value></member>
<member><name>last_name</name><value><string>Doe</string></value></member>
<member><name>start_location</name><value><string>home</string></value></member>
<member><name>inventory-root</name><value><array><data>
<value><struct>
<member><name>folder_id</name><value><string>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</string></value></member>
</struct></value>
</data></array></value></member>
<member><name>inventory-lib-root</name><value><array><data>
<value><struct>
<member><name>folder_id</name><value><string>bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb</string></value></member>
</struct></value>
</data></array></value></member>
<member><name>inventory-lib-owner</name><value><array><data>
<value><struct>
<member><name>agent_id</name><value><string>cccccccc-cccc-cccc-cccc-cccccccccccc</string></value></member>
</struct></value>
</data></array></value></member>
<member><name>inventory-skeleton</name><value><array><data>
<value><struct>
<member><name>folder_id</name><value><string>dddddddd-dddd-dddd-dddd-dddddddddddd</string></value></member>
<member><name>parent_id</name><value><string>aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa</string></value></member>
<member><name>name</name><value><string>Objects</string></value></member>
<member><name>type_default</name><value><int>6</int></value></member>
<member><name>version</name><value><int>12</int></value></member>
</struct></value>
</data></array></value></member>
<member><name>inventory-skel-lib</name><value><array><data>
<value><struct>
<member><name>folder_id</name><value><string>eeeeeeee-eeee-eeee-eeee-eeeeeeeeeeee</string></value></member>
<member><name>parent_id</name><value><string>bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb</string></value></member>
<member><name>name</name><value><string>Library Objects</string></value></member>
<member><name>type_default</name><value><int>6</int></value></member>
<member><name>version</name><value><int>1</int></value></member>
</struct></value>
</data></array></value></member>
<member><name>buddy-list</name><value><array><data>
<value><struct>
<member><name>buddy_id</name><value><string>ffffffff-ffff-ffff-ffff-ffffffffffff</string></value></member>
<member><name>buddy_rights_has</name><value><int>1</int></value></member>
<member><name>buddy_rights_given</name><value><int>3</int></value></member>
</struct></value>
</data></array></value></member>
</struct></value></param></params></methodResponse>
)XML";

    const auto parsed = parse_login_xml_rpc_response(success_xml);
    if (!parsed.success || !parsed.session.has_value())
    {
        std::cerr << "Successful login response was rejected\n";
        return 3;
    }

    if (parsed.session->circuit_code != 424242U ||
        parsed.session->simulator_port != 9000U ||
        parsed.session->seed_capability != "https://caps.example.test/seed" ||
        parsed.session->inventory_root_id != "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa" ||
        parsed.session->inventory_library_root_id != "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb" ||
        parsed.session->inventory_library_owner_id != "cccccccc-cccc-cccc-cccc-cccccccccccc" ||
        parsed.session->inventory_skeleton.size() != 1U ||
        parsed.session->inventory_skeleton.front().name != "Objects" ||
        parsed.session->inventory_skeleton.front().version != 12 ||
        parsed.session->library_skeleton.size() != 1U ||
        parsed.session->buddies.size() != 1U ||
        parsed.session->buddies.front().rights_has != 1 ||
        parsed.session->buddies.front().rights_given != 3)
    {
        std::cerr << "Session or bootstrap data was not parsed correctly\n";
        return 4;
    }

    constexpr auto failure_xml = R"XML(
<methodResponse><params><param><value><struct>
<member><name>login</name><value><string>false</string></value></member>
<member><name>reason</name><value><string>key</string></value></member>
<member><name>message</name><value><string>Invalid credentials</string></value></member>
</struct></value></param></params></methodResponse>
)XML";

    const auto failed = parse_login_xml_rpc_response(failure_xml);
    if (failed.success || failed.reason != "key" || failed.message != "Invalid credentials")
    {
        std::cerr << "Failed login response was not handled correctly\n";
        return 5;
    }

    auto transport = std::make_shared<FakeTransport>();
    transport->response = {200, std::string{success_xml}, {}};

    LoginService service{transport};
    const auto service_result = service.login(
        "http://grid.example.test:8002/",
        credentials,
        "OpenGenesisLINK Viewer",
        "0.1.0-alpha.1");

    if (!service_result.success ||
        transport->posted_url != "http://grid.example.test:8002/" ||
        transport->posted_body.find("secret") != std::string::npos)
    {
        std::cerr << "Login service transport contract failed\n";
        return 6;
    }

    std::cout << "OpenSim login protocol tests passed\n";
    return 0;
}
