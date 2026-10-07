/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/core/version.hpp"

#include <iostream>
#include <string_view>

int main()
{
    const std::string_view product = nexverse::core::product_name();
    const std::string_view version = nexverse::core::version();

    if (product != "OpenGenesisLINK Viewer")
    {
        std::cerr << "Unexpected product name: " << product << '\n';
        return 1;
    }

    if (version.empty() || version.front() != '0')
    {
        std::cerr << "Invalid phase-zero version: " << version << '\n';
        return 2;
    }

    std::cout << product << ' ' << version << " core tests passed\n";
    return 0;
}
