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

    std::cout << "GridInfo tests passed\n";
    return 0;
}
