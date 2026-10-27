/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once
#include "nexverse/app/marketplace_hop.hpp"
#include <optional>

namespace nexverse::app
{
class Application
{
public:
    int run(bool enable_developer_modules = false,
            std::optional<MarketplaceHop> startup_hop = std::nullopt);
};
}
