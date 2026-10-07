/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/protocol/grid_info.hpp"

#include <iostream>
#include <string>

int main()
{
    using nexverse::protocol::max_grid_info_bytes;
    using nexverse::protocol::parse_grid_info_xml;

    constexpr auto valid_xml = R"XML(
<gridinfo>
  <platform>OpenSim</platform>
  <login> http://hg.stadt-nexverse.de:8002/ </login>
  <gridname>NexVerse &amp; Friends</gridname>
  <gridnick>NexVerse</gridnick>
  <welcome>https://stadt-nexverse.de/welcome</welcome>
  <future_field>supported</future_field>
</gridinfo>
)XML";

    const auto valid = parse_grid_info_xml(valid_xml);
    if (!valid.ok())
    {
        std::cerr << "Valid GridInfo failed: " << valid.error << '\n';
        return 1;
    }

    if (valid.document->info.login_uri() != "http://hg.stadt-nexverse.de:8002/")
    {
        std::cerr << "Login URI was not trimmed correctly\n";
        return 2;
    }

    if (valid.document->info.grid_name() != "NexVerse & Friends")
    {
        std::cerr << "XML entity decoding failed\n";
        return 3;
    }

    const std::string* future = valid.document->info.get("future_field");
    if (future == nullptr || *future != "supported")
    {
        std::cerr << "Unknown additive GridInfo field was not preserved\n";
        return 4;
    }

    const auto missing_login = parse_grid_info_xml(
        "<gridinfo><gridname>NexVerse</gridname></gridinfo>");
    if (missing_login.ok())
    {
        std::cerr << "GridInfo without login must be rejected\n";
        return 5;
    }

    const auto wrong_root = parse_grid_info_xml(
        "<notgridinfo><login>http://example.invalid/</login></notgridinfo>");
    if (wrong_root.ok())
    {
        std::cerr << "Wrong XML root must be rejected\n";
        return 6;
    }

    const auto invalid_scheme = parse_grid_info_xml(
        "<gridinfo><login>ftp://example.invalid/</login></gridinfo>");
    if (invalid_scheme.ok())
    {
        std::cerr << "Non-HTTP login URI must be rejected\n";
        return 7;
    }

    const std::string oversized(max_grid_info_bytes + 1U, 'x');
    const auto too_large = parse_grid_info_xml(oversized);
    if (too_large.ok())
    {
        std::cerr << "Oversized GridInfo payload must be rejected\n";
        return 8;
    }

    std::cout << "GridInfo parser tests passed\n";
    return 0;
}
