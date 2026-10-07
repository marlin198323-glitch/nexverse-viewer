/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/ui/login_model.hpp"

#include <iostream>

int main()
{
    using nexverse::ui::GridDiscoveryState;
    using nexverse::ui::LoginField;
    using nexverse::ui::LoginModel;

    LoginModel model;

    if (model.start_region() != "Home")
    {
        std::cerr << "Default region must be Home\n";
        return 1;
    }

    if (model.grid_ready())
    {
        std::cerr << "Grid must not be ready before discovery completes\n";
        return 2;
    }

    model.append_codepoint(U'N');
    model.append_codepoint(U'e');
    model.append_codepoint(U'x');

    if (model.username() != "Nex")
    {
        std::cerr << "Username input failed\n";
        return 3;
    }

    model.focus_next();
    model.append_codepoint(U'x');
    model.append_codepoint(U'y');

    if (model.password() != "xy" || model.masked_password() != "**")
    {
        std::cerr << "Password input or masking failed\n";
        return 4;
    }

    if (model.submit_enabled())
    {
        std::cerr << "Submit must remain disabled until GridInfo is valid\n";
        return 5;
    }

    model.set_grid_discovery(
        GridDiscoveryState::live,
        "NexVerse",
        "Current GridInfo loaded from NexVerse");

    if (!model.grid_ready() || model.grid_status_label() != "LIVE")
    {
        std::cerr << "Live GridInfo state failed\n";
        return 6;
    }

    if (!model.submit_enabled())
    {
        std::cerr << "Submit should be enabled with GridInfo and credentials\n";
        return 7;
    }

    model.backspace();
    if (model.password() != "x")
    {
        std::cerr << "Backspace failed\n";
        return 8;
    }

    model.focus_next();
    if (model.focused_field() != LoginField::start_region)
    {
        std::cerr << "Focus navigation failed\n";
        return 9;
    }

    model.focus_next();
    if (model.focused_field() != LoginField::username)
    {
        std::cerr << "Focus wrap failed\n";
        return 10;
    }

    LoginModel cache_model;
    cache_model.set_grid_discovery(
        GridDiscoveryState::cache,
        "NexVerse Cache",
        "Using cache");

    if (!cache_model.grid_ready() || cache_model.grid_status_label() != "CACHE")
    {
        std::cerr << "Cache GridInfo state failed\n";
        return 11;
    }

    LoginModel failed;
    failed.set_grid_discovery(
        GridDiscoveryState::failed,
        "hg.stadt-nexverse.de",
        "No GridInfo");

    if (failed.grid_ready() || failed.attempt_submit())
    {
        std::cerr << "Failed GridInfo state must block submit\n";
        return 12;
    }

    std::cout << "Login model tests passed\n";
    return 0;
}
