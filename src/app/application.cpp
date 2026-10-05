/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"

#include "nexverse/core/logger.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/grid_discovery.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <chrono>
#include <exception>
#include <future>
#include <memory>
#include <string>
#include <utility>

namespace
{
using nexverse::ui::Color;
using nexverse::ui::UiRenderer;
namespace theme = nexverse::ui::theme;

bool point_inside(double px, double py, float x, float y, float width, float height)
{
    return px >= static_cast<double>(x) &&
           py >= static_cast<double>(y) &&
           px <= static_cast<double>(x + width) &&
           py <= static_cast<double>(y + height);
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
    Color grid_status_color)
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
        "Die echte Region ersetzt diese Vorschau nach erfolgreichem Login.",
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
        "Noch nicht verbunden - Login folgt in Phase 2",
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
        "VORSCHAU",
        theme::cyan);
}

void render_shell(
    UiRenderer& renderer,
    int width,
    int height,
    double mouse_x,
    double mouse_y,
    std::string_view grid_name,
    std::string_view grid_status,
    Color grid_status_color)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1536.0f, h / 960.0f),
        0.72f,
        1.55f);

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
    status_chip(renderer, chip_start + chip_w + chip_gap, 14.0f * scale, chip_w, scale, "PROTOCOL", "PHASE 1", theme::warning);
    status_chip(renderer, chip_start + (chip_w + chip_gap) * 2.0f, 14.0f * scale, chip_w, scale, "WORLD", "OFFLINE", theme::text_muted);

    const float content_y = top_h + gap;
    const float content_h = std::max(380.0f * scale, h - top_h - bottom_h - gap * 2.0f);

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
    renderer.text(
        86.0f * scale,
        profile_y + 16.0f * scale,
        13.5f * scale,
        "Nicht angemeldet",
        theme::text);
    renderer.text(
        86.0f * scale,
        profile_y + 43.0f * scale,
        9.5f * scale,
        "OpenSim Login folgt in Phase 2",
        theme::text_muted);

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
        grid_status_color);

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
        "Phase 2",
        theme::warning);

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
        "Noch keine Weltverbindung",
        theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 68.0f * scale,
        9.0f * scale,
        "Avatare und Regionen werden",
        theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 85.0f * scale,
        9.0f * scale,
        "nach dem Login live geladen.",
        theme::text_muted);

    panel_y += 156.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        136.0f * scale,
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
        Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    renderer.text(
        login_x + 14.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        "Avatar Name",
        theme::text_muted);

    renderer.rect(
        login_x + field_w + 8.0f * scale,
        login_y,
        field_w,
        field_h,
        Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    renderer.text(
        login_x + field_w + 22.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        "Passwort",
        theme::text_muted);

    const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
    const float button_w = 164.0f * scale;
    const bool hover = point_inside(
        mouse_x,
        mouse_y,
        button_x,
        login_y,
        button_w,
        field_h);

    renderer.rect(
        button_x,
        login_y,
        button_w,
        field_h,
        hover ? Color{0.080f, 0.690f, 0.960f, 1.0f} : theme::blue,
        11.0f * scale);
    renderer.text(
        button_x + 29.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        "WELT BETRETEN",
        theme::text);

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
    bool discovery_applied = false;

    {
        ui::UiRenderer renderer;
        if (!renderer.initialize(window_width, window_height))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            return 13;
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

                        core::info(
                            std::string{"Active OpenSim login URI: "} +
                            std::string{result.info.login_uri()});
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

            int framebuffer_width = 0;
            int framebuffer_height = 0;
            glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
            glfwGetWindowSize(window, &window_width, &window_height);

            if (framebuffer_width <= 0 || framebuffer_height <= 0 ||
                window_width <= 0 || window_height <= 0)
            {
                continue;
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

            render_shell(
                renderer, window_width, window_height, mouse_x, mouse_y,
                grid_name, grid_status, grid_status_color);

            glfwSwapBuffers(window);
        }
    }

    core::info("OpenGenesisLINK Viewer shutdown");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
}
