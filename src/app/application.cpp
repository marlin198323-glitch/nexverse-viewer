/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"

#include "nexverse/core/logger.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/caps_event_dispatcher.hpp"
#include "nexverse/protocol/caps_world_events.hpp"
#include "nexverse/protocol/circuit.hpp"
#include "nexverse/protocol/grid_discovery.hpp"
#include "nexverse/protocol/login.hpp"
#include "nexverse/protocol/packet_dispatcher.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"
#include "nexverse/world/world_model.hpp"
#include "nexverse/ui/welcome_page.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <chrono>
#include <exception>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>

namespace
{
using nexverse::ui::Color;
using nexverse::ui::UiRenderer;
namespace theme = nexverse::ui::theme;

std::string_view circuit_state_label(nexverse::protocol::CircuitState state)
{
    using nexverse::protocol::CircuitState;
    switch (state)
    {
        case CircuitState::disconnected: return "OFFLINE";
        case CircuitState::opening_socket: return "SOCKET";
        case CircuitState::awaiting_use_circuit_ack: return "CIRCUIT";
        case CircuitState::awaiting_movement_ack: return "MOVEMENT";
        case CircuitState::awaiting_region_ready: return "REGION";
        case CircuitState::active: return "UDP LIVE";
        case CircuitState::failed: return "UDP ERROR";
    }
    return "UNKNOWN";
}

Color circuit_state_color(nexverse::protocol::CircuitState state)
{
    using nexverse::protocol::CircuitState;
    switch (state)
    {
        case CircuitState::active: return theme::success;
        case CircuitState::failed: return theme::magenta;
        case CircuitState::opening_socket:
        case CircuitState::awaiting_use_circuit_ack:
        case CircuitState::awaiting_movement_ack:
        case CircuitState::awaiting_region_ready:
            return theme::cyan;
        case CircuitState::disconnected:
            return theme::text_muted;
    }
    return theme::text_muted;
}

enum class LoginField
{
    username,
    password
};

struct SimulationLoadStats
{
    std::uint32_t terrain_packets = 0U;
    std::uint32_t object_packets = 0U;
    std::uint32_t avatar_packets = 0U;
    std::uint32_t other_packets = 0U;
};

struct CapsRuntimeState
{
    std::string status = "WAITING";
    std::uint32_t capability_count = 0U;
    std::uint32_t event_count = 0U;
    bool simulator_features_loaded = false;
    bool event_queue_available = false;
};

struct LoginUiState
{
    LoginField focused = LoginField::username;
    std::string username;
    std::string password;
    std::string status = "READY";
    std::string notice = "Avatarname und Passwort eingeben.";
    bool submit_requested = false;
    bool busy = false;
    bool authenticated = false;
};

struct InputContext
{
    LoginUiState* login = nullptr;
};

std::string masked_password(std::string_view password)
{
    std::string masked;
    for (const char raw : password)
    {
        const auto byte = static_cast<unsigned char>(raw);
        if ((byte & 0xC0U) != 0x80U)
        {
            masked.push_back('*');
        }
    }
    return masked;
}

void append_utf8(std::string& target, char32_t codepoint, std::size_t limit)
{
    if (codepoint < U' ' || codepoint == 0x7FU) return;

    std::string encoded;
    if (codepoint <= 0x7FU)
    {
        encoded.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FFU)
    {
        encoded.push_back(static_cast<char>(0xC0U | ((codepoint >> 6U) & 0x1FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0xFFFFU)
    {
        encoded.push_back(static_cast<char>(0xE0U | ((codepoint >> 12U) & 0x0FU)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0x10FFFFU)
    {
        encoded.push_back(static_cast<char>(0xF0U | ((codepoint >> 18U) & 0x07U)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 12U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }

    if (target.size() + encoded.size() <= limit)
    {
        target += encoded;
    }
}

void erase_last_utf8(std::string& target)
{
    if (target.empty()) return;
    auto index = target.size() - 1U;
    while (index > 0U &&
           (static_cast<unsigned char>(target[index]) & 0xC0U) == 0x80U)
    {
        --index;
    }
    target.erase(index);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if (context == nullptr || context->login == nullptr || context->login->busy) return;

    LoginUiState& state = *context->login;
    if (state.focused == LoginField::username)
    {
        append_utf8(state.username, static_cast<char32_t>(codepoint), 96U);
    }
    else
    {
        append_utf8(state.password, static_cast<char32_t>(codepoint), 255U);
    }
}

void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (context == nullptr || context->login == nullptr) return;
    LoginUiState& state = *context->login;

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        state.focused = state.focused == LoginField::username
            ? LoginField::password
            : LoginField::username;
        return;
    }

    if (state.busy) return;

    if (key == GLFW_KEY_BACKSPACE &&
        (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (state.focused == LoginField::username) erase_last_utf8(state.username);
        else erase_last_utf8(state.password);
        return;
    }

    if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) &&
        action == GLFW_PRESS)
    {
        state.submit_requested = true;
    }
}

bool point_inside(double px, double py, float x, float y, float width, float height)
{
    return px >= static_cast<double>(x) &&
           py >= static_cast<double>(y) &&
           px <= static_cast<double>(x + width) &&
           py <= static_cast<double>(y + height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if (context == nullptr || context->login == nullptr) return;

    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);

    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1536.0f, h / 960.0f),
        0.72f,
        1.55f);

    const float left_w = 252.0f * scale;
    const float bottom_h = 106.0f * scale;
    const float bottom_y = h - bottom_h;
    const float login_x = left_w + 20.0f * scale;
    const float login_y = bottom_y + 20.0f * scale;
    const float field_w = 184.0f * scale;
    const float field_h = 48.0f * scale;
    const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
    const float button_w = 164.0f * scale;

    double mouse_x = 0.0;
    double mouse_y = 0.0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    LoginUiState& state = *context->login;
    if (point_inside(mouse_x, mouse_y, login_x, login_y, field_w, field_h))
    {
        state.focused = LoginField::username;
    }
    else if (point_inside(
                 mouse_x,
                 mouse_y,
                 login_x + field_w + 8.0f * scale,
                 login_y,
                 field_w,
                 field_h))
    {
        state.focused = LoginField::password;
    }
    else if (point_inside(mouse_x, mouse_y, button_x, login_y, button_w, field_h) &&
             !state.busy)
    {
        state.submit_requested = true;
    }
}

std::optional<std::pair<std::string, std::string>> split_avatar_name(std::string_view name)
{
    const std::size_t first = name.find_first_not_of(' ');
    const std::size_t last = name.find_last_not_of(' ');
    if (first == std::string_view::npos || last == std::string_view::npos)
    {
        return std::nullopt;
    }

    const std::string_view trimmed = name.substr(first, last - first + 1U);
    const std::size_t separator = trimmed.find(' ');
    if (separator == std::string_view::npos)
    {
        return std::nullopt;
    }

    const std::size_t last_first = trimmed.find_first_not_of(' ', separator);
    if (last_first == std::string_view::npos)
    {
        return std::nullopt;
    }

    std::string first_name{trimmed.substr(0, separator)};
    std::string last_name{trimmed.substr(last_first)};
    if (first_name.empty() || last_name.empty())
    {
        return std::nullopt;
    }

    return std::pair<std::string, std::string>{
        std::move(first_name),
        std::move(last_name)};
}

Color with_alpha(Color color, float alpha)
{
    color.a = alpha;
    return color;
}

void status_row(UiRenderer& renderer, float x, float y, std::string_view label, std::string_view value, Color state)
{
    renderer.rect(x, y + 6.0f, 8.0f, 8.0f, state, 4.0f);
    renderer.text(x + 18.0f, y, 13.0f, label, theme::text_muted);

    const float value_width = renderer.text_width(13.0f, value);
    renderer.text(x + 205.0f - value_width, y, 13.0f, value, state);
}

void nav_item(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float scale,
    std::string_view title,
    std::string_view subtitle,
    bool active)
{
    const Color background = active
        ? Color{0.045f, 0.115f, 0.205f, 0.96f}
        : Color{0.026f, 0.045f, 0.078f, 0.76f};

    renderer.rect(x, y, width, 56.0f * scale, background, 10.0f * scale);
    if (active)
    {
        renderer.rect(x, y + 5.0f * scale, 3.0f * scale, 46.0f * scale, theme::cyan, 1.5f * scale);
    }

    renderer.rect(
        x + 12.0f * scale,
        y + 14.0f * scale,
        34.0f * scale,
        34.0f * scale,
        active ? with_alpha(theme::cyan, 0.22f) : with_alpha(theme::blue, 0.13f),
        17.0f * scale);

    renderer.text(
        x + 58.0f * scale,
        y + 9.0f * scale,
        14.0f * scale,
        title,
        active ? theme::text : Color{0.80f, 0.86f, 0.94f, 1.0f});

    renderer.text(
        x + 58.0f * scale,
        y + 34.0f * scale,
        10.0f * scale,
        subtitle,
        theme::text_muted);
}

void status_chip(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float scale,
    std::string_view label,
    std::string_view value,
    Color state)
{
    renderer.rect(
        x, y, width, 48.0f * scale,
        Color{0.027f, 0.050f, 0.085f, 0.92f},
        12.0f * scale);

    renderer.rect(
        x + 12.0f * scale,
        y + 18.0f * scale,
        8.0f * scale,
        8.0f * scale,
        state,
        4.0f * scale);

    renderer.text(
        x + 28.0f * scale,
        y + 7.0f * scale,
        9.5f * scale,
        label,
        theme::text_muted);

    renderer.text(
        x + 28.0f * scale,
        y + 25.0f * scale,
        12.5f * scale,
        value,
        theme::text);
}

void render_world_preview(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float height,
    float scale,
    std::string_view grid_status,
    Color grid_status_color,
    const nexverse::protocol::LoginSession* session)
{
    renderer.rect(
        x, y, width, height,
        Color{0.025f, 0.045f, 0.085f, 1.0f},
        14.0f * scale);

    const std::array<Color, 7> sky{
        Color{0.030f, 0.055f, 0.105f, 1.0f},
        Color{0.036f, 0.075f, 0.135f, 1.0f},
        Color{0.045f, 0.095f, 0.160f, 1.0f},
        Color{0.052f, 0.110f, 0.180f, 1.0f},
        Color{0.065f, 0.105f, 0.185f, 1.0f},
        Color{0.075f, 0.085f, 0.165f, 1.0f},
        Color{0.064f, 0.070f, 0.140f, 1.0f}
    };

    const float header_height = 66.0f * scale;
    const float footer_height = 62.0f * scale;
    const float scene_height = std::max(160.0f * scale, height - header_height - footer_height);
    const float band_height = scene_height / static_cast<float>(sky.size());

    for (std::size_t i = 0; i < sky.size(); ++i)
    {
        renderer.rect(
            x + 1.0f,
            y + header_height + static_cast<float>(i) * band_height,
            width - 2.0f,
            band_height + 1.0f,
            sky[i],
            0.0f);
    }

    renderer.rect(
        x + 1.0f, y + 1.0f, width - 2.0f, header_height,
        Color{0.020f, 0.038f, 0.070f, 0.98f},
        14.0f * scale);

    renderer.text(
        x + 22.0f * scale,
        y + 13.0f * scale,
        10.0f * scale,
        "WELT-VORSCHAU",
        theme::cyan);

    renderer.text(
        x + 22.0f * scale,
        y + 32.0f * scale,
        16.0f * scale,
        "NexVerse verbindet sich mit der OpenSim-Welt",
        theme::text);

    renderer.text(
        x + 22.0f * scale,
        y + 54.0f * scale,
        9.0f * scale,
        session != nullptr
            ? "Login erfolgreich. LLUDP-Weltverbindung folgt in Phase 3."
            : "Die echte Region ersetzt diese Vorschau nach erfolgreichem Login.",
        theme::text_muted);

    const float horizon = y + header_height + scene_height * 0.66f;

    renderer.rect(
        x + width * 0.68f,
        y + header_height + 38.0f * scale,
        86.0f * scale,
        86.0f * scale,
        with_alpha(theme::violet, 0.16f),
        43.0f * scale);

    renderer.rect(
        x + width * 0.72f,
        y + header_height + 58.0f * scale,
        52.0f * scale,
        52.0f * scale,
        with_alpha(theme::cyan, 0.30f),
        26.0f * scale);

    const std::array<float, 11> building_widths{42, 56, 34, 72, 50, 62, 38, 68, 45, 58, 40};
    float city_x = x + 34.0f * scale;

    for (std::size_t i = 0; i < building_widths.size(); ++i)
    {
        const float building_width = building_widths[i] * scale;
        const float building_height =
            (68.0f + std::fmod(static_cast<float>((i + 4U) * 41U), 155.0f)) * scale;

        if (city_x + building_width > x + width - 28.0f * scale)
        {
            break;
        }

        renderer.rect(
            city_x,
            horizon - building_height,
            building_width,
            building_height,
            Color{
                0.025f + static_cast<float>(i % 3U) * 0.008f,
                0.040f,
                0.082f + static_cast<float>(i % 4U) * 0.014f,
                0.98f},
            3.0f * scale);

        renderer.rect(
            city_x + 6.0f * scale,
            horizon - building_height + 14.0f * scale,
            building_width - 12.0f * scale,
            2.0f * scale,
            with_alpha((i % 2U == 0U) ? theme::cyan : theme::violet, 0.72f),
            1.0f * scale);

        city_x += building_width + 9.0f * scale;
    }

    renderer.rect(
        x + 1.0f,
        horizon,
        width - 2.0f,
        y + header_height + scene_height - horizon,
        Color{0.012f, 0.026f, 0.050f, 1.0f},
        0.0f);

    for (int i = 0; i < 5; ++i)
    {
        const float px = x + (120.0f + static_cast<float>(i) * 150.0f) * scale;
        const float pw = 92.0f * scale;
        if (px + pw > x + width - 30.0f * scale) break;

        renderer.rect(
            px,
            horizon + 30.0f * scale,
            pw,
            8.0f * scale,
            with_alpha(theme::cyan, 0.10f + static_cast<float>(i % 2) * 0.06f),
            4.0f * scale);

        renderer.rect(
            px + 18.0f * scale,
            horizon + 54.0f * scale,
            56.0f * scale,
            3.0f * scale,
            with_alpha(theme::violet, 0.22f),
            1.5f * scale);
    }

    renderer.rect(
        x + 1.0f,
        horizon,
        width - 2.0f,
        2.0f * scale,
        with_alpha(theme::cyan, 0.55f),
        0.0f);

    const float badge_width = 174.0f * scale;
    renderer.rect(
        x + width - badge_width - 18.0f * scale,
        y + header_height + 18.0f * scale,
        badge_width,
        76.0f * scale,
        Color{0.016f, 0.032f, 0.060f, 0.90f},
        12.0f * scale);

    renderer.text(
        x + width - badge_width,
        y + header_height + 31.0f * scale,
        9.0f * scale,
        "GRID DISCOVERY",
        theme::text_muted);

    renderer.text(
        x + width - badge_width,
        y + header_height + 51.0f * scale,
        13.0f * scale,
        grid_status,
        grid_status_color);

    renderer.rect(
        x + 1.0f,
        y + height - footer_height - 1.0f,
        width - 2.0f,
        footer_height,
        Color{0.016f, 0.030f, 0.055f, 0.98f},
        0.0f);

    renderer.text(
        x + 20.0f * scale,
        y + height - footer_height + 10.0f * scale,
        9.5f * scale,
        "WORLD STREAM",
        theme::text_muted);

    renderer.text(
        x + 20.0f * scale,
        y + height - footer_height + 29.0f * scale,
        12.0f * scale,
        session != nullptr
            ? "Login erfolgreich - bereit fuer LLUDP / Phase 3"
            : "Noch nicht verbunden - Anmeldung erforderlich",
        theme::text);

    renderer.rect(
        x + width - 146.0f * scale,
        y + height - footer_height + 13.0f * scale,
        136.0f * scale,
        34.0f * scale,
        with_alpha(theme::blue, 0.15f),
        9.0f * scale);

    renderer.text(
        x + width - 124.0f * scale,
        y + height - footer_height + 20.0f * scale,
        10.0f * scale,
        session != nullptr ? "SESSION" : "VORSCHAU",
        session != nullptr ? theme::success : theme::cyan);
}

void render_shell(
    UiRenderer& renderer,
    int width,
    int height,
    double mouse_x,
    double mouse_y,
    std::string_view grid_name,
    std::string_view grid_status,
    Color grid_status_color,
    const LoginUiState& login,
    const nexverse::protocol::LoginSession* session,
    const nexverse::protocol::CircuitStatus& circuit,
    const SimulationLoadStats& load_stats,
    const CapsRuntimeState& caps,
    bool simulation_ready,
    unsigned int welcome_texture)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1536.0f, h / 960.0f),
        0.72f,
        1.55f);

    const Color login_color = login.authenticated
        ? theme::success
        : login.busy
            ? theme::cyan
            : login.status == "ERROR"
                ? theme::magenta
                : theme::warning;

    renderer.resize(width, height);
    renderer.rect(0.0f, 0.0f, w, h, Color{0.008f, 0.018f, 0.035f, 1.0f}, 0.0f);

    const float top_h = 86.0f * scale;
    const float left_w = 252.0f * scale;
    const float right_w = 354.0f * scale;
    const float bottom_h = 106.0f * scale;
    const float gap = 10.0f * scale;

    renderer.rect(
        0.0f, 0.0f, w, top_h,
        Color{0.012f, 0.030f, 0.058f, 0.99f},
        0.0f);
    renderer.rect(
        0.0f, top_h - 1.0f, w, 1.0f,
        with_alpha(theme::cyan, 0.24f),
        0.0f);

    renderer.rect(
        18.0f * scale,
        14.0f * scale,
        56.0f * scale,
        56.0f * scale,
        Color{0.025f, 0.120f, 0.235f, 1.0f},
        14.0f * scale);
    renderer.rect(
        27.0f * scale,
        23.0f * scale,
        38.0f * scale,
        38.0f * scale,
        with_alpha(theme::cyan, 0.24f),
        9.0f * scale);

    renderer.text(
        88.0f * scale,
        15.0f * scale,
        24.0f * scale,
        "OpenGenesisLINK",
        theme::text);
    renderer.text(
        89.0f * scale,
        48.0f * scale,
        9.0f * scale,
        "VIEWER FOR NEXVERSE",
        theme::cyan);

    const float location_x = 346.0f * scale;
    const float location_w = 360.0f * scale;
    renderer.rect(
        location_x,
        14.0f * scale,
        location_w,
        54.0f * scale,
        Color{0.026f, 0.052f, 0.090f, 0.95f},
        12.0f * scale);
    renderer.text(
        location_x + 18.0f * scale,
        18.0f * scale,
        10.0f * scale,
        "NEXVERSE GRID",
        theme::text_muted);
    renderer.text(
        location_x + 18.0f * scale,
        36.0f * scale,
        13.0f * scale,
        grid_name,
        theme::text);

    const float chip_w = 122.0f * scale;
    const float chip_gap = 8.0f * scale;
    const float chips_total = chip_w * 3.0f + chip_gap * 2.0f;
    const float chip_start = std::max(location_x + location_w + 16.0f * scale, w - chips_total - 26.0f * scale);

    status_chip(renderer, chip_start, 14.0f * scale, chip_w, scale, "GRID", grid_status, grid_status_color);
    status_chip(renderer, chip_start + chip_w + chip_gap, 14.0f * scale, chip_w, scale, "LOGIN", login.status, login_color);
    status_chip(
        renderer,
        chip_start + (chip_w + chip_gap) * 2.0f,
        14.0f * scale,
        chip_w,
        scale,
        "WORLD",
        simulation_ready
            ? circuit_state_label(circuit.state)
            : (login.authenticated ? "LOADING" : "OFFLINE"),
        simulation_ready
            ? circuit_state_color(circuit.state)
            : (login.authenticated ? theme::cyan : theme::text_muted));

    const float content_y = top_h + gap;
    const float content_h = std::max(380.0f * scale, h - top_h - bottom_h - gap * 2.0f);

    if (!simulation_ready)
    {
        if (welcome_texture != 0U)
        {
            renderer.image(
                welcome_texture,
                0.0f,
                content_y,
                w,
                std::max(1.0f, h - content_y - bottom_h),
                1.0f);
            renderer.rect(
                0.0f,
                content_y,
                w,
                std::max(1.0f, h - content_y - bottom_h),
                Color{0.004f, 0.012f, 0.028f, 0.56f},
                0.0f);
        }

        const float panel_w = std::min(760.0f * scale, w - 80.0f * scale);
        const float panel_h = login.authenticated ? 286.0f * scale : 230.0f * scale;
        const float panel_x = (w - panel_w) * 0.5f;
        const float panel_y = top_h + std::max(44.0f * scale, (h - top_h - bottom_h - panel_h) * 0.5f);

        renderer.rect(
            panel_x,
            panel_y,
            panel_w,
            panel_h,
            Color{0.016f, 0.034f, 0.063f, 0.98f},
            16.0f * scale);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 24.0f * scale,
            10.0f * scale,
            login.authenticated ? "SIMULATION" : "NEXVERSE LOGIN",
            theme::cyan);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 52.0f * scale,
            22.0f * scale,
            login.authenticated
                ? "Simulation wird geladen"
                : "Willkommen bei NexVerse",
            theme::text);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 88.0f * scale,
            11.0f * scale,
            login.authenticated
                ? "Die Viewer-Oberflaeche wird erst eingeblendet, wenn Region und WorldModel bereit sind."
                : "Melde dich mit deinem NexVerse-Avatar an.",
            theme::text_muted);

        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 126.0f * scale,
            "Grid",
            grid_status,
            grid_status_color);
        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 154.0f * scale,
            "Login",
            login.status,
            login_color);
        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 182.0f * scale,
            "Simulation",
            login.authenticated
                ? (circuit.state == nexverse::protocol::CircuitState::active ? "WORLD DATA" : circuit_state_label(circuit.state))
                : "WARTET",
            login.authenticated
                ? (circuit.state == nexverse::protocol::CircuitState::active ? theme::cyan : circuit_state_color(circuit.state))
                : theme::text_muted);

        if (login.authenticated)
        {
            const std::string load_line =
                "Terrain " + std::to_string(load_stats.terrain_packets) +
                "  |  Objekte " + std::to_string(load_stats.object_packets) +
                "  |  Avatar " + std::to_string(load_stats.avatar_packets) +
                "  |  Sonstige " + std::to_string(load_stats.other_packets);

            renderer.text(
                panel_x + 28.0f * scale,
                panel_y + 222.0f * scale,
                10.0f * scale,
                load_line,
                theme::text_muted);

            const std::string caps_line =
                "CAPS " + caps.status +
                "  |  Endpunkte " + std::to_string(caps.capability_count) +
                "  |  Features " + (caps.simulator_features_loaded ? "OK" : "-") +
                "  |  Events " + std::to_string(caps.event_count);

            renderer.text(
                panel_x + 28.0f * scale,
                panel_y + 246.0f * scale,
                9.5f * scale,
                caps_line,
                caps.status == "ERROR" ? theme::magenta : theme::text_muted);
        }

        const float bottom_y = h - bottom_h;
        renderer.rect(
            0.0f,
            bottom_y,
            w,
            bottom_h,
            Color{0.010f, 0.026f, 0.050f, 0.99f},
            0.0f);
        renderer.rect(
            0.0f,
            bottom_y,
            w,
            1.0f,
            with_alpha(theme::cyan, 0.20f),
            0.0f);

        const float login_x = left_w + 20.0f * scale;
        const float login_y = bottom_y + 20.0f * scale;
        const float field_w = 184.0f * scale;
        const float field_h = 48.0f * scale;

        if (!login.authenticated)
        {
            renderer.text(
                login_x,
                bottom_y + 7.0f * scale,
                8.5f * scale,
                "ANMELDUNG",
                theme::text_muted);

            renderer.rect(
                login_x,
                login_y,
                field_w,
                field_h,
                login.focused == LoginField::username
                    ? Color{0.035f, 0.085f, 0.145f, 0.98f}
                    : Color{0.024f, 0.052f, 0.090f, 0.96f},
                11.0f * scale);
            if (login.focused == LoginField::username)
            {
                renderer.rect(
                    login_x,
                    login_y + field_h - 2.0f * scale,
                    field_w,
                    2.0f * scale,
                    theme::cyan,
                    1.0f * scale);
            }
            renderer.text(
                login_x + 14.0f * scale,
                login_y + 14.0f * scale,
                11.0f * scale,
                login.username.empty() ? "Avatar Name" : login.username,
                login.username.empty() ? theme::text_muted : theme::text);

            const float password_x = login_x + field_w + 8.0f * scale;
            renderer.rect(
                password_x,
                login_y,
                field_w,
                field_h,
                login.focused == LoginField::password
                    ? Color{0.035f, 0.085f, 0.145f, 0.98f}
                    : Color{0.024f, 0.052f, 0.090f, 0.96f},
                11.0f * scale);
            if (login.focused == LoginField::password)
            {
                renderer.rect(
                    password_x,
                    login_y + field_h - 2.0f * scale,
                    field_w,
                    2.0f * scale,
                    theme::cyan,
                    1.0f * scale);
            }

            const std::string password_mask = masked_password(login.password);
            renderer.text(
                password_x + 14.0f * scale,
                login_y + 14.0f * scale,
                11.0f * scale,
                password_mask.empty() ? "Passwort" : password_mask,
                password_mask.empty() ? theme::text_muted : theme::text);

            const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
            const float button_w = 164.0f * scale;
            const bool hover = point_inside(
                mouse_x,
                mouse_y,
                button_x,
                login_y,
                button_w,
                field_h);

            const bool can_submit =
                !login.busy &&
                !login.username.empty() &&
                !login.password.empty() &&
                (grid_status == "LIVE" || grid_status == "CACHE");

            Color button_color = can_submit
                ? theme::blue
                : Color{0.075f, 0.120f, 0.185f, 1.0f};
            if (can_submit && hover)
            {
                button_color = Color{0.080f, 0.690f, 0.960f, 1.0f};
            }

            renderer.rect(
                button_x,
                login_y,
                button_w,
                field_h,
                button_color,
                11.0f * scale);

            const std::string_view button_label = login.busy
                ? "VERBINDE..."
                : "WELT BETRETEN";
            const float button_label_w = renderer.text_width(10.5f * scale, button_label);
            renderer.text(
                button_x + (button_w - button_label_w) * 0.5f,
                login_y + 14.0f * scale,
                10.5f * scale,
                button_label,
                can_submit ? theme::text : theme::text_muted);

            renderer.text(
                login_x,
                bottom_y + 76.0f * scale,
                8.5f * scale,
                login.notice,
                theme::text_muted);
        }
        else
        {
            renderer.text(
                24.0f * scale,
                bottom_y + 20.0f * scale,
                10.0f * scale,
                "ANGEMELDET",
                theme::success);
            renderer.text(
                24.0f * scale,
                bottom_y + 44.0f * scale,
                13.0f * scale,
                login.username,
                theme::text);
            renderer.text(
                24.0f * scale,
                bottom_y + 68.0f * scale,
                9.5f * scale,
                circuit.state == nexverse::protocol::CircuitState::active
                    ? "Verbindung steht. Warte auf Region, WorldModel und Renderer."
                    : circuit.message,
                theme::text_muted);
        }

        const std::string version =
            std::string{"v"} + std::string{nexverse::core::version()};
        const float version_w = renderer.text_width(9.0f * scale, version);
        renderer.text(
            w - version_w - 18.0f * scale,
            bottom_y + bottom_h - 20.0f * scale,
            9.0f * scale,
            version,
            theme::text_muted);
        return;
    }

    renderer.rect(
        0.0f, content_y, left_w, content_h,
        Color{0.011f, 0.025f, 0.048f, 0.98f},
        0.0f);

    renderer.text(
        18.0f * scale,
        content_y + 16.0f * scale,
        9.0f * scale,
        "NAVIGATION",
        theme::text_muted);

    const float nav_x = 8.0f * scale;
    const float nav_w = left_w - 16.0f * scale;
    float nav_y = content_y + 40.0f * scale;

    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Start", "NexVerse Zugang", true);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Welt", "Regionen erkunden", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Orte", "Favoriten und Ziele", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Freunde", "Kontakte und Nähe", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Gruppen", "Communities", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Events", "Veranstaltungen", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Inventar", "Objekte und Assets", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Marketplace", "NexVerse entdecken", false);

    const float profile_h = 86.0f * scale;
    const float profile_y = content_y + content_h - profile_h - 12.0f * scale;
    renderer.rect(
        10.0f * scale,
        profile_y,
        left_w - 20.0f * scale,
        profile_h,
        Color{0.020f, 0.050f, 0.086f, 0.94f},
        14.0f * scale);
    renderer.rect(
        22.0f * scale,
        profile_y + 16.0f * scale,
        52.0f * scale,
        52.0f * scale,
        with_alpha(theme::blue, 0.18f),
        26.0f * scale);
    const std::string profile_name = login.authenticated
        ? login.username
        : "Nicht angemeldet";
    renderer.text(
        86.0f * scale,
        profile_y + 16.0f * scale,
        13.5f * scale,
        profile_name,
        theme::text);
    renderer.text(
        86.0f * scale,
        profile_y + 43.0f * scale,
        9.5f * scale,
        login.authenticated
            ? "OpenSim Session aktiv"
            : "NexVerse Login bereit",
        login.authenticated ? theme::success : theme::text_muted);

    const float center_x = left_w + gap;
    const float center_w = std::max(
        420.0f * scale,
        w - left_w - right_w - gap * 3.0f);
    const float right_x = center_x + center_w + gap;

    render_world_preview(
        renderer,
        center_x,
        content_y,
        center_w,
        content_h,
        scale,
        grid_status,
        grid_status_color,
        session);

    renderer.rect(
        right_x,
        content_y,
        right_w,
        content_h,
        Color{0.011f, 0.025f, 0.048f, 0.98f},
        0.0f);

    renderer.rect(
        right_x + 10.0f * scale,
        content_y + 10.0f * scale,
        right_w - 20.0f * scale,
        46.0f * scale,
        Color{0.020f, 0.050f, 0.086f, 0.94f},
        10.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "GRID",
        theme::cyan);
    renderer.text(
        right_x + 104.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "REGION",
        theme::text_muted);
    renderer.text(
        right_x + 190.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "ORTE",
        theme::text_muted);

    float panel_y = content_y + 64.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        184.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);

    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 19.0f * scale,
        14.0f * scale,
        "NexVerse Verbindung",
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 45.0f * scale,
        9.0f * scale,
        "Grid",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 45.0f * scale,
        10.5f * scale,
        grid_name,
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 70.0f * scale,
        9.0f * scale,
        "Discovery",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 70.0f * scale,
        10.5f * scale,
        grid_status,
        grid_status_color);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 95.0f * scale,
        9.0f * scale,
        "Login",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 95.0f * scale,
        10.5f * scale,
        login.status,
        login_color);

    renderer.rect(
        right_x + 24.0f * scale,
        panel_y + 136.0f * scale,
        right_w - 48.0f * scale,
        34.0f * scale,
        with_alpha(theme::blue, 0.17f),
        9.0f * scale);
    renderer.text(
        right_x + 38.0f * scale,
        panel_y + 133.0f * scale,
        10.0f * scale,
        "Server-Konfiguration live geladen",
        theme::cyan);

    panel_y += 196.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        144.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 17.0f * scale,
        13.0f * scale,
        "In der Nähe",
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 46.0f * scale,
        10.0f * scale,
        login.authenticated ? "Session erfolgreich aufgebaut" : "Noch keine Weltverbindung",
        login.authenticated ? theme::success : theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 68.0f * scale,
        9.0f * scale,
        session != nullptr
            ? ("Simulator: " + session->simulator_ip + ":" + std::to_string(session->simulator_port))
            : "Avatare und Regionen werden",
        theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 85.0f * scale,
        9.0f * scale,
        session != nullptr
            ? ("Inventar " + std::to_string(session->inventory_skeleton.size()) +
               " | Freunde " + std::to_string(session->buddies.size()) +
               " | Gesten " + std::to_string(session->gestures.size()))
            : "nach dem Login live geladen.",
        theme::text_muted);

    panel_y += 156.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        166.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 17.0f * scale,
        13.0f * scale,
        "Systemstatus",
        theme::text);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 49.0f * scale,
        "Renderer",
        "READY",
        theme::success);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 77.0f * scale,
        "Grid",
        grid_status,
        grid_status_color);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 105.0f * scale,
        "LLUDP",
        circuit_state_label(circuit.state),
        circuit_state_color(circuit.state));

    const float bottom_y = h - bottom_h;
    renderer.rect(
        0.0f,
        bottom_y,
        w,
        bottom_h,
        Color{0.010f, 0.026f, 0.050f, 0.99f},
        0.0f);
    renderer.rect(
        0.0f,
        bottom_y,
        w,
        1.0f,
        with_alpha(theme::cyan, 0.20f),
        0.0f);

    const float login_x = left_w + 20.0f * scale;
    const float login_y = bottom_y + 20.0f * scale;
    const float field_w = 184.0f * scale;
    const float field_h = 48.0f * scale;

    renderer.text(
        login_x,
        bottom_y + 7.0f * scale,
        8.5f * scale,
        "ANMELDUNG",
        theme::text_muted);

    renderer.rect(
        login_x,
        login_y,
        field_w,
        field_h,
        login.focused == LoginField::username
            ? Color{0.035f, 0.085f, 0.145f, 0.98f}
            : Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    if (login.focused == LoginField::username)
    {
        renderer.rect(
            login_x,
            login_y + field_h - 2.0f * scale,
            field_w,
            2.0f * scale,
            theme::cyan,
            1.0f * scale);
    }
    renderer.text(
        login_x + 14.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        login.username.empty() ? "Avatar Name" : login.username,
        login.username.empty() ? theme::text_muted : theme::text);

    const float password_x = login_x + field_w + 8.0f * scale;
    renderer.rect(
        password_x,
        login_y,
        field_w,
        field_h,
        login.focused == LoginField::password
            ? Color{0.035f, 0.085f, 0.145f, 0.98f}
            : Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    if (login.focused == LoginField::password)
    {
        renderer.rect(
            password_x,
            login_y + field_h - 2.0f * scale,
            field_w,
            2.0f * scale,
            theme::cyan,
            1.0f * scale);
    }
    const std::string password_mask = masked_password(login.password);
    renderer.text(
        password_x + 14.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        password_mask.empty() ? "Passwort" : password_mask,
        password_mask.empty() ? theme::text_muted : theme::text);

    const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
    const float button_w = 164.0f * scale;
    const bool hover = point_inside(
        mouse_x,
        mouse_y,
        button_x,
        login_y,
        button_w,
        field_h);

    const bool can_submit =
        !login.busy &&
        !login.authenticated &&
        !login.username.empty() &&
        !login.password.empty() &&
        (grid_status == "LIVE" || grid_status == "CACHE");

    Color button_color = can_submit
        ? theme::blue
        : Color{0.075f, 0.120f, 0.185f, 1.0f};
    if (can_submit && hover)
    {
        button_color = Color{0.080f, 0.690f, 0.960f, 1.0f};
    }

    renderer.rect(
        button_x,
        login_y,
        button_w,
        field_h,
        button_color,
        11.0f * scale);

    const std::string_view button_label = login.busy
        ? "VERBINDE..."
        : login.authenticated
            ? "ANGEMELDET"
            : "WELT BETRETEN";
    const float button_label_w = renderer.text_width(10.5f * scale, button_label);
    renderer.text(
        button_x + (button_w - button_label_w) * 0.5f,
        login_y + 14.0f * scale,
        10.5f * scale,
        button_label,
        can_submit || login.authenticated ? theme::text : theme::text_muted);

    renderer.text(
        login_x,
        bottom_y + 76.0f * scale,
        8.5f * scale,
        login.notice,
        login.authenticated ? theme::success : theme::text_muted);

    const float dock_x = button_x + button_w + 20.0f * scale;
    const std::array<std::string_view, 6> dock{
        "Chat", "Sprechen", "Bewegen", "Teleport", "Inventar", "Bauen"};

    float dx = dock_x;
    for (const auto label : dock)
    {
        const float item_w = 80.0f * scale;
        renderer.rect(
            dx,
            login_y,
            item_w,
            field_h,
            Color{0.018f, 0.042f, 0.074f, 0.90f},
            11.0f * scale);
        const float text_w = renderer.text_width(9.5f * scale, label);
        renderer.text(
            dx + (item_w - text_w) * 0.5f,
            login_y + 15.0f * scale,
            9.5f * scale,
            label,
            theme::text_muted);
        dx += item_w + 6.0f * scale;
    }

    const std::string version =
        std::string{"v"} + std::string{nexverse::core::version()};
    const float version_w = renderer.text_width(9.0f * scale, version);
    renderer.text(
        w - version_w - 18.0f * scale,
        bottom_y + bottom_h - 20.0f * scale,
        9.0f * scale,
        version,
        theme::text_muted);
}

void glfw_error_callback(int error, const char* description)
{
    nexverse::core::error(
        "GLFW error " + std::to_string(error) + ": " +
        (description != nullptr ? description : "unknown"));
}
}

namespace nexverse::app
{
int Application::run()
{
    core::info(
        std::string{"Starting "} +
        std::string{core::product_name()} +
        " " +
        std::string{core::version()});

    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == GLFW_FALSE)
    {
        core::error("GLFW initialization failed");
        return 10;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1440, 900, "OpenGenesisLINK Viewer", nullptr, nullptr);
    if (window == nullptr)
    {
        core::error("Viewer window creation failed");
        glfwTerminate();
        return 11;
    }

    glfwSetWindowSizeLimits(window, 1100, 700, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        core::error("GLAD OpenGL loader initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 12;
    }

    int window_width = 0;
    int window_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);

    LoginUiState login_ui;
    InputContext input_context{&login_ui};
    glfwSetWindowUserPointer(window, &input_context);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    auto transport = std::make_shared<net::CurlHttpTransport>();
    auto discovery_future = std::async(
        std::launch::async,
        [transport]
        {
            protocol::GridDiscoveryService service{transport};
            return service.refresh();
        });

    std::string grid_name{protocol::nexverse_grid_host};
    std::string grid_status = "CHECKING";
    Color grid_status_color = theme::cyan;
    std::string active_login_uri;
    bool discovery_applied = false;

    std::future<protocol::LoginResult> login_future;
    std::optional<protocol::LoginSession> active_session;

    protocol::OutboundCommandQueue outbound_queue{512U};
    world::WorldModel world_model;
    std::mutex world_mutex;
    bool simulation_ready = false;
    protocol::PacketDispatcher packet_dispatcher;
    protocol::CapsEventDispatcher caps_event_dispatcher;

    caps_event_dispatcher.register_handler(
        "EnableSimulator",
        [](const protocol::EventQueueEvent& event)
        {
            if (protocol::parse_enable_simulator_event(event).has_value())
            {
                core::info("CAPS EnableSimulator world event validated");
            }
        });

    caps_event_dispatcher.register_handler(
        "EstablishAgentCommunication",
        [](const protocol::EventQueueEvent& event)
        {
            if (protocol::parse_establish_agent_communication_event(event).has_value())
            {
                core::info(
                    "CAPS EstablishAgentCommunication world event validated");
            }
        });

    caps_event_dispatcher.register_handler(
        "TeleportFinish",
        [&world_model, &world_mutex](const protocol::EventQueueEvent& event)
        {
            const auto teleport =
                protocol::parse_teleport_finish_caps_event(event);
            if (!teleport.has_value())
            {
                core::warning("CAPS TeleportFinish world event rejected");
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (teleport->agent_id != world_model.agent().agent_id)
            {
                return;
            }

            world_model.begin_region_transition({
                teleport->simulator.region_handle,
                {},
                {},
                teleport->simulator.simulator_ip,
                teleport->simulator.simulator_port,
                teleport->simulator.seed_capability,
                teleport->simulator.region_size_x,
                teleport->simulator.region_size_y,
                false});

            core::info("WorldModel region transition started from TeleportFinish");
        });

    caps_event_dispatcher.register_handler(
        "CrossedRegion",
        [&world_model, &world_mutex](const protocol::EventQueueEvent& event)
        {
            const auto crossed =
                protocol::parse_crossed_region_caps_event(event);
            if (!crossed.has_value())
            {
                core::warning("CAPS CrossedRegion world event rejected");
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (crossed->agent_id != world_model.agent().agent_id ||
                crossed->session_id != world_model.agent().session_id)
            {
                return;
            }

            world_model.begin_region_transition({
                crossed->simulator.region_handle,
                {},
                crossed->position,
                crossed->simulator.simulator_ip,
                crossed->simulator.simulator_port,
                crossed->simulator.seed_capability,
                crossed->simulator.region_size_x,
                crossed->simulator.region_size_y,
                false});

            core::info("WorldModel region transition started from CrossedRegion");
        });

    caps_event_dispatcher.set_fallback_handler(
        [](const protocol::EventQueueEvent&)
        {
        });

    std::atomic_uint32_t local_chat_received{0U};
    std::atomic_uint32_t terrain_packets{0U};
    std::atomic_bool terrain_live_logged{false};
    std::atomic_bool parcel_live_logged{false};
    bool world_ready_logged = false;
    std::atomic_uint32_t object_packets{0U};
    std::atomic_uint32_t avatar_packets{0U};
    std::atomic_bool coarse_avatar_live_logged{false};
    std::atomic_bool full_object_live_logged{false};
    std::atomic_bool compressed_object_live_logged{false};
    std::atomic_bool cached_object_live_logged{false};
    std::atomic_bool terse_object_live_logged{false};
    std::atomic_uint32_t other_packets{0U};

    packet_dispatcher.register_handler(
        protocol::msg_chat_from_simulator,
        [&local_chat_received](const protocol::DecodedPacket& packet)
        {
            if (protocol::decode_chat_from_simulator(packet.payload).has_value())
            {
                local_chat_received.fetch_add(1U, std::memory_order_relaxed);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_layer_data,
        [&terrain_packets,
         &terrain_live_logged,
         &world_model,
         &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto layer = protocol::decode_layer_data(packet.payload);
            if (!layer.has_value() || !layer->is_terrain())
            {
                return;
            }

            const auto terrain = protocol::decode_terrain_layer(
                layer->data,
                layer->type);
            if (!terrain.success)
            {
                core::warning(
                    "Terrain LayerData decode failed: " +
                    terrain.message);
                return;
            }

            terrain_packets.fetch_add(1U, std::memory_order_relaxed);

            if (!terrain_live_logged.exchange(true))
            {
                core::info("WorldModel terrain stream validated");
            }

            std::scoped_lock lock{world_mutex};
            for (const auto& patch : terrain.patches)
            {
                world_model.apply_terrain_patch(patch);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_parcel_properties,
        [&parcel_live_logged, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto parcel =
                protocol::decode_parcel_properties(packet.payload);
            if (!parcel.has_value())
            {
                return;
            }

            if (!parcel_live_logged.exchange(true))
            {
                core::info("WorldModel parcel stream validated");
            }

            std::scoped_lock lock{world_mutex};
            world_model.set_parcel({
                parcel->local_id,
                parcel->name,
                parcel->description,
                parcel->owner_id,
                parcel->area,
                parcel->parcel_flags,
                parcel->sale_price,
                parcel->music_url,
                parcel->media_url,
                true});
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update,
        [&object_packets,
         &avatar_packets,
         &full_object_live_logged,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto update = protocol::decode_object_update(packet.payload);
            if (!update.has_value())
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!full_object_live_logged.exchange(true))
            {
                core::info("WorldModel full object stream validated");
            }

            std::scoped_lock lock{world_mutex};
            const std::string self_id = world_model.agent().agent_id;

            for (const auto& entry : update->objects)
            {
                if (entry.is_avatar())
                {
                    avatar_packets.fetch_add(1U, std::memory_order_relaxed);

                    if (entry.id == self_id)
                    {
                        const world::AgentState current = world_model.agent();
                        world_model.apply_agent({
                            current.agent_id,
                            current.session_id,
                            entry.position,
                            entry.rotation,
                            current.present});
                        continue;
                    }

                    world::AvatarState avatar;
                    avatar.id = entry.id;
                    avatar.position = entry.position;
                    avatar.rotation = entry.rotation;
                    avatar.local_id = entry.local_id;

                    if (const auto existing = world_model.avatars().find(entry.id);
                        existing != world_model.avatars().end())
                    {
                        avatar.name = existing->second.name;
                    }

                    world_model.upsert_avatar(std::move(avatar));
                    continue;
                }

                world_model.upsert_object({
                    entry.local_id,
                    entry.id,
                    entry.position,
                    entry.rotation});
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update_compressed,
        [&object_packets,
         &avatar_packets,
         &compressed_object_live_logged,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto update =
                protocol::decode_object_update_compressed(packet.payload);
            if (!update.has_value())
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!compressed_object_live_logged.exchange(true))
            {
                core::info("WorldModel compressed object stream validated");
            }

            std::scoped_lock lock{world_mutex};
            const std::string self_id = world_model.agent().agent_id;

            for (const auto& entry : update->objects)
            {
                if (entry.pcode == protocol::pcode_avatar)
                {
                    avatar_packets.fetch_add(1U, std::memory_order_relaxed);

                    if (entry.id == self_id)
                    {
                        const world::AgentState current = world_model.agent();
                        world_model.apply_agent({
                            current.agent_id,
                            current.session_id,
                            entry.position,
                            entry.rotation,
                            current.present});
                        continue;
                    }

                    world::AvatarState avatar;
                    avatar.id = entry.id;
                    avatar.position = entry.position;
                    avatar.rotation = entry.rotation;
                    avatar.local_id = entry.local_id;

                    if (const auto existing = world_model.avatars().find(entry.id);
                        existing != world_model.avatars().end())
                    {
                        avatar.name = existing->second.name;
                    }

                    world_model.upsert_avatar(std::move(avatar));
                    continue;
                }

                world_model.upsert_object({
                    entry.local_id,
                    entry.id,
                    entry.position,
                    entry.rotation});
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update_cached,
        [&object_packets, &cached_object_live_logged](
            const protocol::DecodedPacket& packet)
        {
            const auto cached =
                protocol::decode_object_update_cached(packet.payload);
            if (!cached.has_value())
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!cached_object_live_logged.exchange(true))
            {
                core::info("WorldModel cached object stream validated");
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_improved_terse_object_update,
        [&object_packets,
         &terse_object_live_logged,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto terse =
                protocol::decode_improved_terse_object_update(packet.payload);
            if (!terse.has_value())
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!terse_object_live_logged.exchange(true))
            {
                core::info("WorldModel terse object stream validated");
            }

            std::scoped_lock lock{world_mutex};
            for (const auto& update : terse->updates)
            {
                if (update.avatar)
                {
                    for (const auto& [id, existing] : world_model.avatars())
                    {
                        if (existing.local_id != update.local_id)
                        {
                            continue;
                        }

                        world::AvatarState avatar = existing;
                        avatar.position = update.position;
                        avatar.rotation = update.rotation;
                        world_model.upsert_avatar(std::move(avatar));
                        break;
                    }
                    continue;
                }

                world::ObjectState object;
                object.local_id = update.local_id;
                object.position = update.position;
                object.rotation = update.rotation;

                if (const auto existing = world_model.objects().find(update.local_id);
                    existing != world_model.objects().end())
                {
                    object.id = existing->second.id;
                }

                world_model.upsert_object(std::move(object));
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_kill_object,
        [&object_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto kill = protocol::decode_kill_object(packet.payload);
            if (!kill.has_value())
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);

            std::scoped_lock lock{world_mutex};
            for (const std::uint32_t local_id : kill->local_ids)
            {
                world_model.remove_object(local_id);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_coarse_location_update,
        [&avatar_packets,
         &coarse_avatar_live_logged,
         &world_model,
         &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto coarse =
                protocol::decode_coarse_location_update(packet.payload);
            if (!coarse.has_value())
            {
                return;
            }

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!coarse_avatar_live_logged.exchange(true))
            {
                core::info("WorldModel coarse avatar stream validated");
            }

            std::scoped_lock lock{world_mutex};
            const std::string self_id = world_model.agent().agent_id;
            std::vector<std::string> seen;
            seen.reserve(coarse->avatars.size());

            for (const auto& coarse_avatar : coarse->avatars)
            {
                if (coarse_avatar.agent_id == self_id)
                {
                    continue;
                }

                seen.push_back(coarse_avatar.agent_id);

                world::AvatarState avatar;
                avatar.id = coarse_avatar.agent_id;
                avatar.position = coarse_avatar.position;

                if (const auto existing =
                        world_model.avatars().find(coarse_avatar.agent_id);
                    existing != world_model.avatars().end())
                {
                    avatar.name = existing->second.name;
                    avatar.rotation = existing->second.rotation;
                }

                world_model.upsert_avatar(std::move(avatar));
            }

            std::vector<std::string> stale;
            for (const auto& [id, avatar] : world_model.avatars())
            {
                (void)avatar;
                if (std::find(seen.begin(), seen.end(), id) == seen.end())
                {
                    stale.push_back(id);
                }
            }

            for (const auto& id : stale)
            {
                world_model.remove_avatar(id);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_avatar_appearance,
        [&avatar_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto appearance =
                protocol::decode_avatar_appearance(packet.payload);
            if (!appearance.has_value())
            {
                return;
            }

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);

            std::scoped_lock lock{world_mutex};
            world::AvatarState avatar;
            avatar.id = appearance->avatar_id;

            if (const auto existing =
                    world_model.avatars().find(appearance->avatar_id);
                existing != world_model.avatars().end())
            {
                avatar = existing->second;
            }

            avatar.trial_appearance = appearance->trial;
            avatar.texture_entry = appearance->texture_entry;
            avatar.visual_params = appearance->visual_params;
            avatar.appearance_data = appearance->appearance_data;
            world_model.upsert_avatar(std::move(avatar));
        });

    packet_dispatcher.register_handler(
        protocol::msg_agent_data_update,
        [&avatar_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto data = protocol::decode_agent_data_update(packet.payload);
            if (!data.has_value())
            {
                return;
            }

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);

            std::scoped_lock lock{world_mutex};
            if (data->agent_id != world_model.agent().agent_id)
            {
                return;
            }

            world_model.set_agent_profile({
                data->first_name,
                data->last_name,
                data->group_title,
                data->active_group_id,
                data->group_powers,
                data->group_name});
        });

    packet_dispatcher.set_fallback_handler(
        [&other_packets](const protocol::DecodedPacket&)
        {
            other_packets.fetch_add(1U, std::memory_order_relaxed);
        });

    auto last_agent_update = std::chrono::steady_clock::now();

    std::atomic_bool circuit_stop{false};
    std::future<protocol::CircuitStatus> circuit_future;
    std::mutex circuit_mutex;
    protocol::CircuitStatus circuit_status;
    bool circuit_started = false;

    std::atomic_bool caps_stop{false};
    std::future<void> caps_future;
    std::mutex caps_mutex;
    CapsRuntimeState caps_state;
    bool caps_started = false;

    {
        ui::UiRenderer renderer;
        if (!renderer.initialize(window_width, window_height))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            return 13;
        }

        ui::WelcomePage welcome_page;
        unsigned int welcome_texture = 0U;
        int welcome_texture_width = 0;
        int welcome_texture_height = 0;
        bool welcome_failure_logged = false;

        if (!welcome_page.initialize(window_width, window_height))
        {
            core::warning("Embedded NexVerse welcome page initialization failed");
        }

        const auto* gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        const auto* gl_renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        if (gl_version != nullptr)
        {
            core::info(std::string{"OpenGL: "} + gl_version);
        }
        if (gl_renderer != nullptr)
        {
            core::info(std::string{"GPU: "} + gl_renderer);
        }

        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            glfwPollEvents();

            if (!discovery_applied &&
                discovery_future.valid() &&
                discovery_future.wait_for(std::chrono::milliseconds{0}) == std::future_status::ready)
            {
                discovery_applied = true;
                try
                {
                    auto result = discovery_future.get();
                    if (result.success)
                    {
                        if (!result.info.grid_name().empty())
                        {
                            grid_name = std::string{result.info.grid_name()};
                        }
                        else if (!result.info.grid_nick().empty())
                        {
                            grid_name = std::string{result.info.grid_nick()};
                        }

                        if (result.source == protocol::GridInfoSource::live)
                        {
                            grid_status = "LIVE";
                            grid_status_color = theme::success;
                        }
                        else
                        {
                            grid_status = "CACHE";
                            grid_status_color = theme::warning;
                        }

                        active_login_uri = std::string{result.info.login_uri()};
                        core::info(
                            std::string{"Active OpenSim login URI: "} +
                            active_login_uri);
                    }
                    else
                    {
                        grid_status = "ERROR";
                        grid_status_color = theme::magenta;
                        core::warning(result.message);
                    }
                }
                catch (const std::exception& error)
                {
                    grid_status = "ERROR";
                    grid_status_color = theme::magenta;
                    core::error(std::string{"Grid discovery worker failed: "} + error.what());
                }
            }

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            if (login_ui.busy &&
                login_future.valid() &&
                login_future.wait_for(std::chrono::milliseconds{0}) == std::future_status::ready)
            {
                login_ui.busy = false;

                try
                {
                    protocol::LoginResult result = login_future.get();
                    if (result.success && result.session.has_value())
                    {
                        active_session = std::move(result.session);
                        login_ui.authenticated = true;
                        login_ui.status = "CONNECTED";
                        login_ui.notice = result.message.empty()
                            ? "OpenSim-Anmeldung erfolgreich."
                            : result.message;
                        login_ui.password.clear();
                        core::info("OpenSim login succeeded; simulator session established");

                        const std::uint64_t region_handle =
                            (static_cast<std::uint64_t>(active_session->region_x) << 32U) |
                            static_cast<std::uint64_t>(active_session->region_y);

                        std::string region_name = active_session->start_location;
                        if (const auto found = active_session->scalar_fields.find("sim_name");
                            found != active_session->scalar_fields.end() && !found->second.empty())
                        {
                            region_name = found->second;
                        }

                        {
                            std::scoped_lock lock{world_mutex};
                            world_model.reset();
                            world_model.set_grid({grid_name, active_login_uri});
                            world_model.apply_region({
                                region_handle,
                                std::move(region_name),
                                {},
                                active_session->simulator_ip,
                                active_session->simulator_port,
                                active_session->seed_capability,
                                256U,
                                256U,
                                false});
                            world_model.apply_agent({
                                active_session->agent_id,
                                active_session->session_id,
                                {},
                                {},
                                false});
                            simulation_ready = world_model.ready();
                        }

                        if (!caps_started &&
                            active_session.has_value() &&
                            !active_session->seed_capability.empty())
                        {
                            caps_started = true;
                            const std::string seed_capability =
                                active_session->seed_capability;

                            caps_future = std::async(
                                std::launch::async,
                                [transport,
                                 seed_capability,
                                 &caps_stop,
                                 &caps_mutex,
                                 &caps_state,
                                 &caps_event_dispatcher,
                                 &world_model,
                                 &world_mutex]()
                                {
                                    protocol::CapabilityService service{transport};

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.status = "DISCOVERY";
                                    }

                                    protocol::CapabilityDiscoveryResult discovery =
                                        service.discover(seed_capability);

                                    if (!discovery.success)
                                    {
                                        {
                                            std::scoped_lock lock{caps_mutex};
                                            caps_state.status = "ERROR";
                                        }
                                        core::error(
                                            "CAPS seed discovery failed; no capability endpoints accepted");
                                        return;
                                    }

                                    core::info(
                                        "CAPS seed discovery succeeded; endpoints=" +
                                        std::to_string(discovery.capabilities.size()));

                                    std::string event_queue_uri;
                                    std::string simulator_features_uri;
                                    std::string environment_settings_uri;
                                    std::string simulator_features_message;

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.capability_count =
                                            static_cast<std::uint32_t>(
                                                discovery.capabilities.size());
                                        caps_state.status = "CAPS READY";
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("SimulatorFeatures");
                                        found != discovery.capabilities.end())
                                    {
                                        simulator_features_uri = found->second;
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("EventQueueGet");
                                        found != discovery.capabilities.end())
                                    {
                                        event_queue_uri = found->second;
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("EnvironmentSettings");
                                        found != discovery.capabilities.end())
                                    {
                                        environment_settings_uri = found->second;
                                    }

                                    if (!simulator_features_uri.empty())
                                    {
                                        const auto features =
                                            service.fetch_simulator_features(
                                                simulator_features_uri);

                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.simulator_features_loaded =
                                            features.success;
                                        simulator_features_message = features.message;
                                        if (!features.success)
                                        {
                                            caps_state.status = "CAPS PARTIAL";
                                        }
                                    }

                                    if (!simulator_features_uri.empty())
                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        if (caps_state.simulator_features_loaded)
                                        {
                                            core::info("CAPS SimulatorFeatures loaded successfully");
                                        }
                                        else
                                        {
                                            core::warning(
                                                "CAPS SimulatorFeatures request failed: " +
                                                simulator_features_message);
                                        }
                                    }
                                    else
                                    {
                                        core::warning("CAPS SimulatorFeatures endpoint not advertised");
                                    }

                                    if (!environment_settings_uri.empty())
                                    {
                                        const auto environment =
                                            service.fetch_llsd(environment_settings_uri);
                                        if (environment.success)
                                        {
                                            {
                                                std::scoped_lock lock{world_mutex};
                                                world::EnvironmentState state =
                                                    world_model.environment();
                                                ++state.revision;
                                                state.available = true;
                                                world_model.set_environment(
                                                    std::move(state));
                                            }
                                            core::info(
                                                "CAPS EnvironmentSettings loaded successfully");
                                        }
                                        else
                                        {
                                            core::warning(
                                                "CAPS EnvironmentSettings request failed: " +
                                                environment.message);
                                        }
                                    }
                                    else
                                    {
                                        core::warning(
                                            "CAPS EnvironmentSettings endpoint not advertised");
                                    }

                                    if (event_queue_uri.empty())
                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.event_queue_available = false;
                                        if (caps_state.status != "ERROR")
                                        {
                                            caps_state.status = "CAPS READY";
                                        }
                                        core::warning("CAPS EventQueueGet endpoint not advertised");
                                        return;
                                    }

                                    core::info("CAPS EventQueueGet endpoint available; long-poll starting");

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.event_queue_available = true;
                                        caps_state.status = "EVENTQUEUE";
                                    }

                                    std::optional<std::int32_t> acknowledgement;
                                    std::uint32_t failures = 0U;
                                    bool event_queue_live_logged = false;

                                    while (!caps_stop.load())
                                    {
                                        const auto batch = service.poll_event_queue(
                                            event_queue_uri,
                                            acknowledgement,
                                            false);

                                        if (caps_stop.load())
                                        {
                                            break;
                                        }

                                        if (batch.success)
                                        {
                                            acknowledgement = batch.id;
                                            failures = 0U;

                                            {
                                                std::scoped_lock lock{caps_mutex};
                                                caps_state.event_count +=
                                                    static_cast<std::uint32_t>(
                                                        batch.events.size());
                                                caps_state.status = "CAPS LIVE";
                                            }

                                            if (!event_queue_live_logged)
                                            {
                                                event_queue_live_logged = true;
                                                core::info(
                                                    "CAPS EventQueue live; first successful batch received");
                                            }

                                            for (const auto& event : batch.events)
                                            {
                                                (void)caps_event_dispatcher.dispatch(event);
                                            }
                                            continue;
                                        }

                                        ++failures;
                                        {
                                            std::scoped_lock lock{caps_mutex};
                                            caps_state.status =
                                                failures >= 5U
                                                    ? "ERROR"
                                                    : "CAPS RETRY";
                                        }

                                        core::warning(
                                            "CAPS EventQueue poll failed; retry=" +
                                            std::to_string(failures));

                                        std::this_thread::sleep_for(
                                            std::chrono::seconds{
                                                std::min<std::uint32_t>(
                                                    failures,
                                                    5U)});
                                    }
                                });

                            core::info(
                                "CAPS discovery started from login seed capability");
                        }

                        if (!circuit_started && active_session.has_value())
                        {
                            circuit_started = true;
                            protocol::LoginSession circuit_session = *active_session;

                            circuit_future = std::async(
                                std::launch::async,
                                [circuit_session = std::move(circuit_session),
                                 &circuit_stop,
                                 &circuit_mutex,
                                 &circuit_status,
                                 &packet_dispatcher,
                                 &outbound_queue]() mutable
                                {
                                    protocol::CircuitStatus final_status;
                                    protocol::CircuitState last_reported_state =
                                        protocol::CircuitState::disconnected;
                                    constexpr int max_attempts = 3;

                                    for (int attempt = 1;
                                         attempt <= max_attempts && !circuit_stop.load();
                                         ++attempt)
                                    {
                                        protocol::CircuitConnection connection{circuit_session};
                                        final_status = connection.run(
                                            circuit_stop,
                                            [&circuit_mutex,
                                             &circuit_status,
                                             &last_reported_state](
                                                const protocol::CircuitStatus& update)
                                            {
                                                {
                                                    std::scoped_lock lock{circuit_mutex};
                                                    circuit_status = update;
                                                }

                                                if (update.state != last_reported_state)
                                                {
                                                    last_reported_state = update.state;
                                                    core::info(
                                                        std::string{"LLUDP state: "} +
                                                        std::string{circuit_state_label(update.state)} +
                                                        " - " + update.message);
                                                }
                                            },
                                            [&packet_dispatcher](const protocol::DecodedPacket& packet)
                                            {
                                                (void)packet_dispatcher.dispatch(packet);
                                            },
                                            &outbound_queue);

                                        if (circuit_stop.load() ||
                                            final_status.state != protocol::CircuitState::failed)
                                        {
                                            break;
                                        }

                                        if (attempt < max_attempts)
                                        {
                                            {
                                                std::scoped_lock lock{circuit_mutex};
                                                circuit_status.message =
                                                    "LLUDP reconnect scheduled";
                                            }
                                            std::this_thread::sleep_for(std::chrono::seconds{1});
                                        }
                                    }

                                    return final_status;
                                });
                        }
                    }
                    else
                    {
                        login_ui.status = "ERROR";
                        login_ui.notice = result.message.empty()
                            ? "OpenSim-Anmeldung fehlgeschlagen."
                            : result.message;
                        core::warning(
                            std::string{"OpenSim login failed: "} +
                            (result.reason.empty() ? "unknown" : result.reason));
                    }
                }
                catch (const std::exception& error)
                {
                    login_ui.status = "ERROR";
                    login_ui.notice = "Anmeldevorgang konnte nicht abgeschlossen werden.";
                    core::error(std::string{"OpenSim login worker failed: "} + error.what());
                }
            }

            if (login_ui.submit_requested)
            {
                login_ui.submit_requested = false;

                if (login_ui.busy || login_ui.authenticated)
                {
                    // Ignore duplicate submit while a login is active or already complete.
                }
                else if (active_login_uri.empty() ||
                         (grid_status != "LIVE" && grid_status != "CACHE"))
                {
                    login_ui.status = "ERROR";
                    login_ui.notice = "Keine gueltige Grid-Konfiguration verfuegbar.";
                }
                else if (login_ui.username.empty() || login_ui.password.empty())
                {
                    login_ui.status = "READY";
                    login_ui.notice = "Avatarname und Passwort eingeben.";
                }
                else
                {
                    const auto avatar_name = split_avatar_name(login_ui.username);
                    if (!avatar_name.has_value())
                    {
                        login_ui.status = "READY";
                        login_ui.notice = "Avatarname als Vorname Nachname eingeben.";
                    }
                    else
                    {
                        protocol::LoginCredentials credentials;
                        credentials.first_name = avatar_name->first;
                        credentials.last_name = avatar_name->second;
                        credentials.password = std::move(login_ui.password);
                        login_ui.password.clear();
                        credentials.start = "last";

                        const std::string login_uri = active_login_uri;
                        const std::string viewer_version{core::version()};

                        login_ui.busy = true;
                        login_ui.status = "CONNECTING";
                        login_ui.notice = "Anmeldung bei NexVerse wird ausgefuehrt...";

                        login_future = std::async(
                            std::launch::async,
                            [transport, login_uri, credentials = std::move(credentials), viewer_version]() mutable
                            {
                                protocol::LoginService service{transport};
                                protocol::LoginResult result = service.login(
                                    login_uri,
                                    credentials,
                                    "OpenGenesisLINK Viewer",
                                    viewer_version);

                                std::fill(
                                    credentials.password.begin(),
                                    credentials.password.end(),
                                    '\0');
                                credentials.password.clear();

                                return result;
                            });
                    }
                }
            }

            protocol::CircuitState current_circuit_state;
            {
                std::scoped_lock lock{circuit_mutex};
                current_circuit_state = circuit_status.state;
            }

            if (login_ui.authenticated && active_session.has_value())
            {
                const bool circuit_active =
                    current_circuit_state == protocol::CircuitState::active;

                std::scoped_lock lock{world_mutex};
                const world::RegionState region_snapshot = world_model.region();
                const bool circuit_matches_region =
                    region_snapshot.simulator_ip == active_session->simulator_ip &&
                    region_snapshot.simulator_port == active_session->simulator_port;
                const bool region_circuit_active =
                    circuit_active && circuit_matches_region;

                world_model.apply_region({
                    region_snapshot.handle,
                    region_snapshot.name,
                    region_snapshot.agent_position,
                    region_snapshot.simulator_ip,
                    region_snapshot.simulator_port,
                    region_snapshot.seed_capability,
                    region_snapshot.size_x,
                    region_snapshot.size_y,
                    region_circuit_active});

                const world::AgentState agent_snapshot = world_model.agent();
                world_model.apply_agent({
                    agent_snapshot.agent_id,
                    agent_snapshot.session_id,
                    agent_snapshot.position,
                    agent_snapshot.rotation,
                    region_circuit_active});

                simulation_ready = world_model.ready();
            }
            else
            {
                simulation_ready = false;
            }

            if (!simulation_ready)
            {
                world_ready_logged = false;
            }
            else if (!world_ready_logged)
            {
                world_ready_logged = true;
                core::info("WorldModel READY; simulation UI may unlock");
            }

            const auto now = std::chrono::steady_clock::now();
            if (simulation_ready &&
                login_ui.authenticated &&
                current_circuit_state == protocol::CircuitState::active &&
                now - last_agent_update >= std::chrono::milliseconds{100})
            {
                protocol::AgentUpdateData update;
                std::uint32_t controls = 0U;

                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    controls |= protocol::control_at_pos;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    controls |= protocol::control_at_neg;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    controls |= protocol::control_left_pos;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    controls |= protocol::control_left_neg;
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                    controls |= protocol::control_up_pos;
                if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
                    controls |= protocol::control_up_neg;
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    controls |= protocol::control_turn_left;
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    controls |= protocol::control_turn_right;
                if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
                    controls |= protocol::control_fly;

                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
                {
                    controls |= protocol::control_fast_at |
                                protocol::control_fast_left |
                                protocol::control_fast_up;
                }

                update.control_flags = controls;
                update.camera_center = {128.0f, 128.0f, 30.0f};
                update.camera_at_axis = {1.0f, 0.0f, 0.0f};
                update.camera_left_axis = {0.0f, 1.0f, 0.0f};
                update.camera_up_axis = {0.0f, 0.0f, 1.0f};
                update.far_clip = 256.0f;

                (void)outbound_queue.push(std::move(update));
                last_agent_update = now;
            }

            int framebuffer_width = 0;
            int framebuffer_height = 0;
            glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
            glfwGetWindowSize(window, &window_width, &window_height);

            if (framebuffer_width <= 0 || framebuffer_height <= 0 ||
                window_width <= 0 || window_height <= 0)
            {
                continue;
            }

            welcome_page.resize(window_width, window_height);
            welcome_page.pump();
            glfwMakeContextCurrent(window);

            const ui::WelcomeFrame& welcome_frame = welcome_page.frame();
            if (welcome_frame.ready &&
                !welcome_frame.rgba.empty() &&
                welcome_frame.width > 0 &&
                welcome_frame.height > 0)
            {
                if (welcome_texture == 0U)
                {
                    welcome_texture = renderer.create_rgba_texture(
                        welcome_frame.width,
                        welcome_frame.height,
                        welcome_frame.rgba.data());
                    welcome_texture_width = welcome_frame.width;
                    welcome_texture_height = welcome_frame.height;
                    if (welcome_texture != 0U)
                    {
                        core::info(
                            std::string{"NexVerse welcome background loaded: "} +
                            std::string{ui::nexverse_welcome_url});
                    }
                }
                else if (welcome_texture_width != welcome_frame.width ||
                         welcome_texture_height != welcome_frame.height)
                {
                    if (renderer.update_rgba_texture(
                            welcome_texture,
                            welcome_frame.width,
                            welcome_frame.height,
                            welcome_frame.rgba.data()))
                    {
                        welcome_texture_width = welcome_frame.width;
                        welcome_texture_height = welcome_frame.height;
                    }
                }
            }
            else if (welcome_frame.failed && !welcome_failure_logged)
            {
                welcome_failure_logged = true;
                core::warning(
                    std::string{"NexVerse welcome background unavailable: "} +
                    welcome_frame.status);
            }

            glViewport(0, 0, framebuffer_width, framebuffer_height);
            glClearColor(
                ui::theme::background.r,
                ui::theme::background.g,
                ui::theme::background.b,
                ui::theme::background.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            double mouse_x = 0.0;
            double mouse_y = 0.0;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);

            protocol::CircuitStatus circuit_snapshot;
            {
                std::scoped_lock lock{circuit_mutex};
                circuit_snapshot = circuit_status;
            }

            const SimulationLoadStats load_stats{
                terrain_packets.load(std::memory_order_relaxed),
                object_packets.load(std::memory_order_relaxed),
                avatar_packets.load(std::memory_order_relaxed),
                other_packets.load(std::memory_order_relaxed)};

            CapsRuntimeState caps_snapshot;
            {
                std::scoped_lock lock{caps_mutex};
                caps_snapshot = caps_state;
            }

            render_shell(
                renderer, window_width, window_height, mouse_x, mouse_y,
                grid_name, grid_status, grid_status_color, login_ui,
                active_session.has_value() ? &*active_session : nullptr,
                circuit_snapshot,
                load_stats,
                caps_snapshot,
                simulation_ready,
                welcome_texture);

            glfwSwapBuffers(window);
        }

        renderer.destroy_texture(welcome_texture);
    }

    circuit_stop.store(true);
    caps_stop.store(true);

    if (circuit_future.valid())
    {
        circuit_future.wait();
    }
    if (caps_future.valid())
    {
        caps_future.wait();
    }

    glfwSetWindowUserPointer(window, nullptr);
    core::info("OpenGenesisLINK Viewer shutdown");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
}
