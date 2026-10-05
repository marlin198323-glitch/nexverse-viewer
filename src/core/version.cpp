/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/core/version.hpp"

namespace nexverse::core
{
std::string_view version() noexcept
{
    return NEXVERSE_VERSION;
}

std::string_view product_name() noexcept
{
    return "OpenGenesisLINK Viewer";
}
}
