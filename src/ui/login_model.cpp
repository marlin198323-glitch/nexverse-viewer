/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/ui/login_model.hpp"

namespace nexverse::ui
{
LoginModel::LoginModel() = default;

LoginField LoginModel::focused_field() const noexcept
{
    return focused_field_;
}

void LoginModel::focus(LoginField field) noexcept
{
    focused_field_ = field;
}

void LoginModel::focus_next(bool reverse) noexcept
{
    constexpr int count = 3;
    int index = static_cast<int>(focused_field_);
    index = reverse ? (index + count - 1) % count : (index + 1) % count;
    focused_field_ = static_cast<LoginField>(index);
}

std::string& LoginModel::active_value() noexcept
{
    switch (focused_field_)
    {
        case LoginField::username: return username_;
        case LoginField::password: return password_;
        case LoginField::start_region: return start_region_;
    }
    return username_;
}

std::size_t LoginModel::active_limit() const noexcept
{
    switch (focused_field_)
    {
        case LoginField::username: return 96U;
        case LoginField::password: return 255U;
        case LoginField::start_region: return 128U;
    }
    return 96U;
}

void LoginModel::append_codepoint(char32_t codepoint)
{
    if (codepoint < U' ' || codepoint == 0x7FU)
    {
        return;
    }

    std::string encoded;
    append_utf8(encoded, codepoint);

    std::string& value = active_value();
    if (value.size() + encoded.size() > active_limit())
    {
        return;
    }

    value += encoded;
    notice_.clear();
}

void LoginModel::backspace()
{
    std::string& value = active_value();
    erase_last_utf8(value);
    notice_.clear();
}

const std::string& LoginModel::username() const noexcept
{
    return username_;
}

const std::string& LoginModel::password() const noexcept
{
    return password_;
}

const std::string& LoginModel::start_region() const noexcept
{
    return start_region_;
}

std::string LoginModel::masked_password() const
{
    std::string masked;
    for (const char raw_byte : password_)
    {
        const auto byte = static_cast<unsigned char>(raw_byte);
        if ((byte & 0xC0U) != 0x80U)
        {
            masked.push_back('*');
        }
    }
    return masked;
}

void LoginModel::set_grid_discovery(
    GridDiscoveryState state,
    std::string display_name,
    std::string message)
{
    grid_discovery_state_ = state;

    if (!display_name.empty())
    {
        grid_display_name_ = std::move(display_name);
    }

    grid_status_message_ = std::move(message);
    notice_ = grid_status_message_;
}

GridDiscoveryState LoginModel::grid_discovery_state() const noexcept
{
    return grid_discovery_state_;
}

bool LoginModel::grid_ready() const noexcept
{
    return grid_discovery_state_ == GridDiscoveryState::live ||
           grid_discovery_state_ == GridDiscoveryState::cache;
}

std::string_view LoginModel::grid_display_name() const noexcept
{
    return grid_display_name_;
}

std::string_view LoginModel::grid_status_label() const noexcept
{
    switch (grid_discovery_state_)
    {
        case GridDiscoveryState::checking: return "CHECKING";
        case GridDiscoveryState::live: return "LIVE";
        case GridDiscoveryState::cache: return "CACHE";
        case GridDiscoveryState::failed: return "ERROR";
    }
    return "ERROR";
}

std::string_view LoginModel::grid_status_message() const noexcept
{
    return grid_status_message_;
}

bool LoginModel::submit_enabled() const noexcept
{
    return grid_ready() && !username_.empty() && !password_.empty();
}

bool LoginModel::attempt_submit()
{
    if (!grid_ready())
    {
        notice_ = "Keine gueltige NexVerse GridInfo verfuegbar.";
        return false;
    }

    if (username_.empty() || password_.empty())
    {
        notice_ = "Bitte Benutzername und Passwort eingeben.";
        return false;
    }

    notice_ = "OpenSim-Login wird in Phase 2 aktiviert.";
    return true;
}

std::string_view LoginModel::notice() const noexcept
{
    return notice_;
}

void LoginModel::append_utf8(std::string& target, char32_t codepoint)
{
    if (codepoint <= 0x7FU)
    {
        target.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FFU)
    {
        target.push_back(static_cast<char>(0xC0U | ((codepoint >> 6U) & 0x1FU)));
        target.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0xFFFFU)
    {
        target.push_back(static_cast<char>(0xE0U | ((codepoint >> 12U) & 0x0FU)));
        target.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        target.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0x10FFFFU)
    {
        target.push_back(static_cast<char>(0xF0U | ((codepoint >> 18U) & 0x07U)));
        target.push_back(static_cast<char>(0x80U | ((codepoint >> 12U) & 0x3FU)));
        target.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        target.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
}

void LoginModel::erase_last_utf8(std::string& target)
{
    if (target.empty())
    {
        return;
    }

    auto index = target.size() - 1U;
    while (index > 0U && (static_cast<unsigned char>(target[index]) & 0xC0U) == 0x80U)
    {
        --index;
    }
    target.erase(index);
}
}
