/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/ui/login_screen.hpp"

#include "nexverse/core/version.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <string_view>

namespace
{
using nexverse::ui::Color;
using nexverse::ui::LoginField;
using nexverse::ui::LoginModel;
using nexverse::ui::UiRenderer;
namespace theme = nexverse::ui::theme;

struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

struct Layout
{
    float scale;
    Rect username;
    Rect password;
    Rect region;
    Rect submit;
    float margin;
    float content_y;
    float content_height;
    float left_width;
    float preview_x;
    float preview_width;
};

bool contains(const Rect& rect, double x, double y) noexcept
{
    return x >= rect.x &&
           y >= rect.y &&
           x <= rect.x + rect.width &&
           y <= rect.y + rect.height;
}

Color with_alpha(Color color, float alpha)
{
    color.a = alpha;
    return color;
}

Layout make_layout(int width, int height)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1600.0f, h / 900.0f),
        0.78f,
        2.20f);

    const float margin = 32.0f * scale;
    const float content_y = 96.0f * scale;
    const float left_width = std::clamp(w * 0.30f, 360.0f * scale, 460.0f * scale);
    const float gap = 22.0f * scale;
    const float content_height = std::max(520.0f * scale, h - content_y - 38.0f * scale);
    const float preview_x = margin + left_width + gap;
    const float preview_width = std::max(420.0f * scale, w - preview_x - margin);

    const float field_x = margin + 26.0f * scale;
    const float field_width = left_width - 52.0f * scale;
    const float first_y = content_y + 250.0f * scale;
    const float field_height = 50.0f * scale;
    const float step = 92.0f * scale;

    return Layout{
        scale,
        Rect{field_x, first_y, field_width, field_height},
        Rect{field_x, first_y + step, field_width, field_height},
        Rect{field_x, first_y + step * 2.0f, field_width, field_height},
        Rect{field_x, first_y + step * 3.0f + 10.0f * scale, field_width, 54.0f * scale},
        margin,
        content_y,
        content_height,
        left_width,
        preview_x,
        preview_width
    };
}

std::string field_value(const LoginModel& model, LoginField field, std::string_view placeholder)
{
    switch (field)
    {
        case LoginField::username:
            return model.username().empty() ? std::string{placeholder} : model.username();
        case LoginField::password:
        {
            const std::string masked = model.masked_password();
            return masked.empty() ? std::string{placeholder} : masked;
        }
        case LoginField::start_region:
            return model.start_region().empty() ? std::string{placeholder} : model.start_region();
    }
    return {};
}

void draw_field(
    UiRenderer& renderer,
    const LoginModel& model,
    LoginField field,
    const Rect& rect,
    float scale,
    std::string_view label,
    std::string_view placeholder)
{
    const bool focused = model.focused_field() == field;
    const Color fill = focused
        ? Color{0.055f, 0.090f, 0.145f, 0.96f}
        : theme::surface_soft;
    const Color line = focused ? theme::cyan : with_alpha(theme::cyan, 0.24f);

    renderer.text(rect.x, rect.y - 22.0f * scale, 12.0f * scale, label, focused ? theme::cyan : theme::text_muted);
    renderer.rect(rect.x, rect.y, rect.width, rect.height, fill, 10.0f * scale);
    renderer.rect(rect.x, rect.y + rect.height - 2.0f * scale, rect.width, 2.0f * scale, line, 0.0f);

    bool empty = false;
    if (field == LoginField::username) empty = model.username().empty();
    if (field == LoginField::password) empty = model.password().empty();

    renderer.text(
        rect.x + 16.0f * scale,
        rect.y + 9.0f * scale,
        15.0f * scale,
        field_value(model, field, placeholder),
        empty ? theme::text_muted : theme::text);
}

void status_row(
    UiRenderer& renderer,
    float x,
    float y,
    float scale,
    std::string_view label,
    std::string_view value,
    Color state)
{
    renderer.rect(x, y + 6.0f * scale, 8.0f * scale, 8.0f * scale, state, 4.0f * scale);
    renderer.text(x + 18.0f * scale, y, 12.0f * scale, label, theme::text_muted);

    const float value_width = renderer.text_width(12.0f * scale, value);
    renderer.text(x + 205.0f * scale - value_width, y, 12.0f * scale, value, state);
}

void render_world_preview(UiRenderer& renderer, const Layout& layout)
{
    const float x = layout.preview_x;
    const float y = layout.content_y;
    const float width = layout.preview_width;
    const float height = layout.content_height;
    const float s = layout.scale;

    renderer.rect(x, y, width, height, theme::surface_glass, 18.0f * s);

    const std::array<Color, 6> sky{
        Color{0.045f, 0.060f, 0.125f, 1.0f},
        Color{0.055f, 0.075f, 0.160f, 1.0f},
        Color{0.075f, 0.095f, 0.200f, 1.0f},
        Color{0.100f, 0.100f, 0.220f, 1.0f},
        Color{0.120f, 0.085f, 0.210f, 1.0f},
        Color{0.115f, 0.065f, 0.170f, 1.0f}
    };

    const float band_height = std::max(
        1.0f,
        (height - 128.0f * s) / static_cast<float>(sky.size()));

    for (std::size_t index = 0; index < sky.size(); ++index)
    {
        renderer.rect(
            x + 1.0f,
            y + 70.0f * s + static_cast<float>(index) * band_height,
            width - 2.0f,
            band_height + 1.0f,
            sky[index],
            0.0f);
    }

    renderer.text(x + 28.0f * s, y + 18.0f * s, 12.0f * s, "NEXVERSE WORLD PREVIEW", theme::text_muted);
    renderer.text(x + 28.0f * s, y + 42.0f * s, 20.0f * s, "OpenSim world connection foundation", theme::text);

    const float horizon = y + height * 0.67f;
    const std::array<float, 12> widths{48, 70, 42, 92, 56, 66, 38, 82, 52, 74, 45, 62};
    float city_x = x + 40.0f * s;

    for (std::size_t index = 0; index < widths.size(); ++index)
    {
        const float building_width = widths[index] * s;
        const float building_height =
            (72.0f + std::fmod(static_cast<float>((index + 3U) * 47U), 190.0f)) * s;

        if (city_x + building_width > x + width - 30.0f * s)
        {
            break;
        }

        const Color building{
            0.035f + static_cast<float>(index % 3U) * 0.012f,
            0.050f,
            0.095f + static_cast<float>(index % 4U) * 0.018f,
            0.96f
        };

        renderer.rect(city_x, horizon - building_height, building_width, building_height, building, 3.0f * s);
        renderer.rect(
            city_x + 7.0f * s,
            horizon - building_height + 16.0f * s,
            building_width - 14.0f * s,
            2.0f * s,
            with_alpha((index % 2U == 0U) ? theme::cyan : theme::violet, 0.66f),
            1.0f * s);

        city_x += building_width + 11.0f * s;
    }

    renderer.rect(
        x + 1.0f,
        horizon,
        width - 2.0f,
        height - (horizon - y),
        Color{0.018f, 0.033f, 0.060f, 1.0f},
        0.0f);
    renderer.rect(x + 1.0f, horizon, width - 2.0f, 2.0f * s, with_alpha(theme::cyan, 0.48f), 1.0f * s);

    const float status_width = 245.0f * s;
    const float status_x = x + width - status_width - 24.0f * s;
    const float status_y = y + 92.0f * s;

    renderer.rect(status_x, status_y, status_width, 188.0f * s, Color{0.025f, 0.045f, 0.080f, 0.88f}, 14.0f * s);
    renderer.text(status_x + 18.0f * s, status_y + 16.0f * s, 12.0f * s, "SYSTEM STATUS", theme::text_muted);

    status_row(renderer, status_x + 18.0f * s, status_y + 52.0f * s, s, "Desktop shell", "READY", theme::success);
    status_row(renderer, status_x + 18.0f * s, status_y + 80.0f * s, s, "Renderer", "READY", theme::success);
    status_row(renderer, status_x + 18.0f * s, status_y + 108.0f * s, s, "Grid discovery", "NEXT", theme::warning);
    status_row(renderer, status_x + 18.0f * s, status_y + 136.0f * s, s, "World stream", "PENDING", theme::text_muted);

    renderer.rect(
        x + 24.0f * s,
        y + height - 58.0f * s,
        width - 48.0f * s,
        38.0f * s,
        Color{0.025f, 0.043f, 0.073f, 0.92f},
        10.0f * s);

    renderer.text(
        x + 40.0f * s,
        y + height - 49.0f * s,
        12.0f * s,
        "Phase 0 - UI interaction only, no OpenSim login is sent.",
        theme::text_muted);
}
}

namespace nexverse::ui
{
void LoginScreen::render(
    UiRenderer& renderer,
    const LoginModel& model,
    int width,
    int height,
    double mouse_x,
    double mouse_y) const
{
    const Layout layout = make_layout(width, height);
    const float s = layout.scale;
    const float w = static_cast<float>(width);
    const float h = static_cast<float>(height);

    renderer.resize(width, height);
    renderer.rect(0.0f, 0.0f, w, h, theme::background, 0.0f);

    for (float gx = 0.0f; gx < w; gx += 64.0f * s)
    {
        renderer.rect(gx, 0.0f, 1.0f, h, with_alpha(theme::cyan, 0.025f), 0.0f);
    }

    for (float gy = 0.0f; gy < h; gy += 64.0f * s)
    {
        renderer.rect(0.0f, gy, w, 1.0f, with_alpha(theme::blue, 0.025f), 0.0f);
    }

    renderer.rect(0.0f, 0.0f, w, 76.0f * s, Color{0.025f, 0.038f, 0.072f, 0.96f}, 0.0f);
    renderer.rect(0.0f, 75.0f * s, w, 1.0f * s, with_alpha(theme::cyan, 0.22f), 0.0f);

    renderer.text(32.0f * s, 14.0f * s, 24.0f * s, "OpenGenesisLINK", theme::text);
    renderer.text(32.0f * s, 43.0f * s, 10.0f * s, "VIEWER FOR NEXVERSE", theme::cyan);

    const std::string version = std::string{nexverse::core::version()} + "  |  C++23";
    const float version_width = renderer.text_width(11.0f * s, version);
    renderer.text(w - version_width - 32.0f * s, 25.0f * s, 11.0f * s, version, theme::text_muted);

    renderer.rect(layout.margin, layout.content_y, layout.left_width, layout.content_height, theme::surface, 18.0f * s);
    renderer.rect(layout.margin, layout.content_y, 4.0f * s, layout.content_height, with_alpha(theme::cyan, 0.72f), 2.0f * s);

    const float left_x = layout.margin + 26.0f * s;
    renderer.text(left_x, layout.content_y + 28.0f * s, 12.0f * s, "NEXVERSE ACCESS", theme::cyan);
    renderer.text(left_x, layout.content_y + 58.0f * s, 25.0f * s, "Willkommen im NexVerse", theme::text);
    renderer.text(left_x, layout.content_y + 96.0f * s, 12.0f * s, "OpenGenesisLINK Viewer", theme::text_muted);

    renderer.text(left_x, layout.content_y + 138.0f * s, 11.0f * s, "NEXVERSE GRID", theme::text_muted);
    renderer.rect(left_x, layout.content_y + 160.0f * s, layout.left_width - 52.0f * s, 58.0f * s, theme::surface_soft, 10.0f * s);
    renderer.text(left_x + 14.0f * s, layout.content_y + 170.0f * s, 14.0f * s, "hg.stadt-nexverse.de", theme::text);
    renderer.text(left_x + 14.0f * s, layout.content_y + 193.0f * s, 9.0f * s, "AUTO DISCOVERY ON START", theme::cyan);

    draw_field(renderer, model, LoginField::username, layout.username, s, "BENUTZERNAME", "Avatar Name");
    draw_field(renderer, model, LoginField::password, layout.password, s, "PASSWORT", "Passwort");
    draw_field(renderer, model, LoginField::start_region, layout.region, s, "STARTREGION", "Home");

    const bool hover = contains(layout.submit, mouse_x, mouse_y);
    const bool enabled = model.submit_enabled();
    Color button = enabled ? theme::blue : Color{0.100f, 0.160f, 0.260f, 1.0f};
    if (enabled && hover)
    {
        button = Color{0.100f, 0.720f, 0.940f, 1.0f};
    }

    renderer.rect(layout.submit.x, layout.submit.y, layout.submit.width, layout.submit.height, button, 12.0f * s);

    constexpr std::string_view button_text = "WELT BETRETEN";
    const float button_width = renderer.text_width(14.0f * s, button_text);
    renderer.text(
        layout.submit.x + (layout.submit.width - button_width) * 0.5f,
        layout.submit.y + 9.0f * s,
        14.0f * s,
        button_text,
        enabled ? theme::text : theme::text_muted);

    if (!model.notice().empty())
    {
        renderer.text(
            layout.submit.x,
            layout.submit.y + 70.0f * s,
            10.5f * s,
            model.notice(),
            theme::text_muted);
    }

    render_world_preview(renderer, layout);

    renderer.text(
        32.0f * s,
        h - 24.0f * s,
        10.0f * s,
        "OpenGenesisLINK Viewer | NexVerse | MPL 2.0 | Phase 0",
        theme::text_muted);
}

std::optional<LoginField> LoginScreen::field_at(
    int width,
    int height,
    double mouse_x,
    double mouse_y) const noexcept
{
    const Layout layout = make_layout(width, height);

    if (contains(layout.username, mouse_x, mouse_y)) return LoginField::username;
    if (contains(layout.password, mouse_x, mouse_y)) return LoginField::password;
    if (contains(layout.region, mouse_x, mouse_y)) return LoginField::start_region;

    return std::nullopt;
}

bool LoginScreen::submit_at(
    int width,
    int height,
    double mouse_x,
    double mouse_y) const noexcept
{
    return contains(make_layout(width, height).submit, mouse_x, mouse_y);
}
}
