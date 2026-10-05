/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/app/application.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/grid_discovery.hpp"

#include <iostream>
#include <memory>
#include <string_view>

namespace
{
int print_grid_info()
{
    auto transport = std::make_shared<nexverse::net::CurlHttpTransport>();
    nexverse::protocol::GridDiscoveryService discovery{transport};
    const auto result = discovery.refresh();

    if (!result.success)
    {
        std::cerr << "GridInfo: ERROR\n" << result.message << '\n';
        return 2;
    }

    std::cout << "GridInfo: "
              << (result.source == nexverse::protocol::GridInfoSource::live ? "LIVE" : "CACHE")
              << '\n'
              << "Changed: " << (result.changed ? "yes" : "no") << '\n';

    for (const auto& [key, value] : result.info.fields)
    {
        std::cout << key << '=' << value << '\n';
    }
    return 0;
}
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        const std::string_view argument{argv[1]};
        if (argument == "--version" || argument == "-v")
        {
            std::cout << nexverse::core::product_name() << ' '
                      << nexverse::core::version() << '\n';
            return 0;
        }

        if (argument == "--grid-info")
        {
            return print_grid_info();
        }

        if (argument == "--help" || argument == "-h")
        {
            std::cout
                << "OpenGenesisLINK Viewer for NexVerse\n"
                << "Usage: nexverse-viewer [--version|--grid-info|--help]\n\n"
                << "--grid-info  Refresh and print the current NexVerse GridInfo.\n"
                << "Normal startup refreshes GridInfo automatically.\n";
            return 0;
        }
    }

    nexverse::app::Application application;
    return application.run();
}
