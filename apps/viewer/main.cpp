/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"
#include "nexverse/core/version.hpp"

#include <iostream>
#include <string_view>

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

        if (argument == "--help" || argument == "-h")
        {
            std::cout
                << "NexVerse Viewer\n"
                << "Usage: nexverse-viewer [--version|--help]\n"
                << "\n"
                << "Phase 0 provides the native desktop shell and UI foundation.\n";
            return 0;
        }
    }

    nexverse::app::Application application;
    return application.run();
}
