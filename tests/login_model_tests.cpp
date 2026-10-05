/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/ui/login_model.hpp"

#include <iostream>

int main()
{
    using nexverse::ui::LoginField;
    using nexverse::ui::LoginModel;

    LoginModel model;

    if (model.start_region() != "Home")
    {
        std::cerr << "Default region must be Home\n";
        return 1;
    }

    model.append_codepoint(U'N');
    model.append_codepoint(U'e');
    model.append_codepoint(U'x');

    if (model.username() != "Nex")
    {
        std::cerr << "Username input failed\n";
        return 2;
    }

    model.focus_next();
    model.append_codepoint(U'x');
    model.append_codepoint(U'y');

    if (model.password() != "xy" || model.masked_password() != "**")
    {
        std::cerr << "Password input or masking failed\n";
        return 3;
    }

    if (!model.submit_enabled())
    {
        std::cerr << "Submit should be enabled\n";
        return 4;
    }

    model.backspace();
    if (model.password() != "x")
    {
        std::cerr << "Backspace failed\n";
        return 5;
    }

    model.focus_next();
    if (model.focused_field() != LoginField::start_region)
    {
        std::cerr << "Focus navigation failed\n";
        return 6;
    }

    model.focus_next();
    if (model.focused_field() != LoginField::username)
    {
        std::cerr << "Focus wrap failed\n";
        return 7;
    }

    LoginModel incomplete;
    if (incomplete.attempt_submit())
    {
        std::cerr << "Incomplete form must not submit\n";
        return 8;
    }

    std::cout << "Login model tests passed\n";
    return 0;
}
