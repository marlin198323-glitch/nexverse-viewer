/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "nexverse/ui/login_model.hpp"

#include <optional>

namespace nexverse::ui
{
class UiRenderer;

class LoginScreen
{
public:
    void render(
        UiRenderer& renderer,
        const LoginModel& model,
        int width,
        int height,
        double mouse_x,
        double mouse_y) const;

    [[nodiscard]] std::optional<LoginField> field_at(
        int width,
        int height,
        double mouse_x,
        double mouse_y) const noexcept;

    [[nodiscard]] bool submit_at(
        int width,
        int height,
        double mouse_x,
        double mouse_y) const noexcept;
};
}
