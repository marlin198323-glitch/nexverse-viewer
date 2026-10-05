/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace nexverse::ui
{
enum class LoginField : std::uint8_t
{
    username = 0,
    password = 1,
    start_region = 2
};

enum class GridDiscoveryState : std::uint8_t
{
    checking,
    live,
    cache,
    failed
};

class LoginModel
{
public:
    LoginModel();

    [[nodiscard]] LoginField focused_field() const noexcept;
    void focus(LoginField field) noexcept;
    void focus_next(bool reverse = false) noexcept;

    void append_codepoint(char32_t codepoint);
    void backspace();

    [[nodiscard]] const std::string& username() const noexcept;
    [[nodiscard]] const std::string& password() const noexcept;
    [[nodiscard]] const std::string& start_region() const noexcept;
    [[nodiscard]] std::string masked_password() const;

    void set_grid_discovery(
        GridDiscoveryState state,
        std::string display_name,
        std::string message);

    [[nodiscard]] GridDiscoveryState grid_discovery_state() const noexcept;
    [[nodiscard]] bool grid_ready() const noexcept;
    [[nodiscard]] std::string_view grid_display_name() const noexcept;
    [[nodiscard]] std::string_view grid_status_label() const noexcept;
    [[nodiscard]] std::string_view grid_status_message() const noexcept;

    [[nodiscard]] bool submit_enabled() const noexcept;
    bool attempt_submit();

    [[nodiscard]] std::string_view notice() const noexcept;

private:
    [[nodiscard]] std::string& active_value() noexcept;
    [[nodiscard]] std::size_t active_limit() const noexcept;

    static void append_utf8(std::string& target, char32_t codepoint);
    static void erase_last_utf8(std::string& target);

    LoginField focused_field_ = LoginField::username;
    std::string username_;
    std::string password_;
    std::string start_region_ = "Home";

    GridDiscoveryState grid_discovery_state_ = GridDiscoveryState::checking;
    std::string grid_display_name_ = "hg.stadt-nexverse.de";
    std::string grid_status_message_ = "Grid-Konfiguration wird beim Start aktualisiert.";
    std::string notice_ = "Grid-Konfiguration wird beim Start aktualisiert.";
};
}
