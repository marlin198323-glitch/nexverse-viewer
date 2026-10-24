#include "nexverse/protocol/grid_info.hpp"

#include <iostream>
#include <string>

int main()
{
    using nexverse::protocol::parse_grid_info_xml;
    using nexverse::protocol::is_valid_http_uri;

    constexpr auto xml = R"XML(
<gridinfo>
  <login> http://hg.stadt-nexverse.de:8002/ </login>
  <gridname>NexVerse &amp; Friends</gridname>
  <gridnick>NexVerse</gridnick>
  <future_field>kept</future_field>
</gridinfo>)XML";

    const auto parsed = parse_grid_info_xml(xml);
    if (!parsed.ok()) return 1;
    if (parsed.document->info.login_uri() != "http://hg.stadt-nexverse.de:8002/") return 2;
    if (parsed.document->info.grid_name() != "NexVerse & Friends") return 3;

    const auto* future = parsed.document->info.get("future_field");
    if (future == nullptr || *future != "kept") return 4;

    if (parse_grid_info_xml("<gridinfo><gridname>X</gridname></gridinfo>").ok()) return 5;
    if (parse_grid_info_xml("<bad><login>http://x.test/</login></bad>").ok()) return 6;
    if (parse_grid_info_xml("<gridinfo><login>ftp://x.test/</login></gridinfo>").ok()) return 7;

    if (!is_valid_http_uri("https://grid.example.org/login")) return 8;
    if (is_valid_http_uri("https://")) return 9;
    if (is_valid_http_uri("https://bad host/login")) return 10;

    // v0.4.0.0: the same HTTP URI predicate guards password-bearing
    // XML-RPC logins and CAPS token-bearing GetTexture/Inventory URLs.
    // Regression: userinfo and host authority confusion MUST fail.
    constexpr std::string_view valid_uris[]{
        "http://hg.stadt-nexverse.de:8002/",
        "https://hg.stadt-nexverse.de/get_grid_info",
        "https://caps.example.org/seed?token=abc%2Fdef",
        "http://localhost:8002/",
        "http://127.0.0.1:9000/",
        "http://[::1]:9000/",
        "https://[2001:db8::1]/caps",
        "https://foo-bar.example.net:443/path?x=1&y=2",
        "https://caps.example.test/assets/a%20b"
    };
    for(const auto uri:valid_uris)
        if(!is_valid_http_uri(uri))
        {
            std::cerr<<"valid server URI rejected: "<<uri<<"\n";
            return 11;
        }

    constexpr std::string_view forbidden_uris[]{
        "",
        "HTTP://grid.example.org/login",
        "ftp://grid.example.org/login",
        "https://",
        "https:///",
        "https://hg.stadt-nexverse.de@evil.example/login",
        "https://user:pass@hg.stadt-nexverse.de/login",
        "https://foo.example.org:443@evil.example/",
        "https://foo.example.org:",
        "https://foo.example.org:0",
        "https://foo.example.org:65536",
        "https://foo.example.org:123456",
        "https://foo.example.org:443x",
        "https://foo.example.org:443:80",
        "https://foo.example.org/hello#evil",
        "https://foo.example.org\x5c@evil.example/",
        "https://foo.example.org/path\nHeader: x",
        "https://foo.example.org\r\nHost: evil.example",
        "https://foo.example.org%40evil.example/login",
        "https://.foo.example/",
        "https://foo.example./",
        "https://foo..example/",
        "https://-foo.example/",
        "https://foo-.example/",
        "https://foo_name.example/",
        "https://[::1",
        "https://[::1]evil.test/",
        "https://[::1]:",
        "https://[::1]:65536",
        "https://[fe80::1%25eth0]/",
        "https://2001:db8::1/",
        "https://[xyz]/",
        "https://example.org\\evil.test/"
    };
    for(const auto uri:forbidden_uris)
        if(is_valid_http_uri(uri))
        {
            std::cerr<<"invalid credential-bearing URI accepted: "<<uri<<"\n";
            return 12;
        }
    // The parser cannot insert an injected login URI into the cache
    // and the login service must refuse it before sending credentials.
    for(const auto uri:forbidden_uris)
    {
        const auto crafted="<gridinfo><login>"+
            std::string{uri}+"</login></gridinfo>";
        // '>' and '&' inside XML values are not relevant to a rejected
        // URL and may also cause XML parsing to fail, both acceptable.
        if(parse_grid_info_xml(crafted).ok())return 13;
    }
    std::cout << "OGL_GRID_HTTP_CREDENTIAL_URI_AUTHORITY_SPOOF_GUARD_PASS\n";
    std::cout << "GridInfo tests passed\n";
    return 0;
}
