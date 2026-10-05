/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"

#include "nexverse/core/logger.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <string>

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

void field(UiRenderer& renderer, float x, float y, float width, std::string_view label, std::string_view value)
{
    renderer.text(x, y, 13.0f, label, theme::text_muted);
    renderer.rect(x, y + 28.0f, width, 48.0f, theme::surface_soft, 10.0f);
    renderer.rect(x, y + 75.0f, width, 1.0f, with_alpha(theme::cyan, 0.28f), 0.0f);
    renderer.text(x + 16.0f, y + 37.0f, 16.0f, value, theme::text);
}

void status_row(UiRenderer& renderer, float x, float y, std::string_view label, std::string_view value, Color state)
{
    renderer.rect(x, y + 6.0f, 8.0f, 8.0f, state, 4.0f);
    renderer.text(x + 18.0f, y, 13.0f, label, theme::text_muted);

    const float value_width = renderer.text_width(13.0f, value);
    renderer.text(x + 205.0f - value_width, y, 13.0f, value, state);
}

void render_world_preview(UiRenderer& renderer, float x, float y, float width, float height)
{
    renderer.rect(x, y, width, height, theme::surface_glass, 18.0f);

    const std::array<Color, 6> sky{
        Color{0.045f, 0.060f, 0.125f, 1.0f},
        Color{0.055f, 0.075f, 0.160f, 1.0f},
        Color{0.075f, 0.095f, 0.200f, 1.0f},
        Color{0.100f, 0.100f, 0.220f, 1.0f},
        Color{0.120f, 0.085f, 0.210f, 1.0f},
        Color{0.115f, 0.065f, 0.170f, 1.0f}
    };

    const float band_height = (height - 128.0f) / static_cast<float>(sky.size());
    for (std::size_t i = 0; i < sky.size(); ++i)
    {
        renderer.rect(
            x + 1.0f,
            y + 70.0f + static_cast<float>(i) * band_height,
            width - 2.0f,
            band_height + 1.0f,
            sky[i],
            0.0f);
    }

    renderer.text(x + 28.0f, y + 20.0f, 14.0f, "NEXVERSE WORLD PREVIEW", theme::text_muted);
    renderer.text(x + 28.0f, y + 43.0f, 22.0f, "A native OpenSim world is coming online.", theme::text);

    const float horizon = y + height * 0.67f;

    renderer.rect(x + width * 0.67f, y + 112.0f, 94.0f, 94.0f, with_alpha(theme::magenta, 0.20f), 47.0f);
    renderer.rect(x + width * 0.70f, y + 132.0f, 54.0f, 54.0f, with_alpha(theme::cyan, 0.32f), 27.0f);

    const std::array<float, 12> building_widths{48, 70, 42, 92, 56, 66, 38, 82, 52, 74, 45, 62};
    float city_x = x + 40.0f;

    for (std::size_t i = 0; i < building_widths.size(); ++i)
    {
        const float building_width = building_widths[i];
        const float max_height = 190.0f;
        const float building_height =
            72.0f + std::fmod(static_cast<float>((i + 3U) * 47U), max_height);

        if (city_x + building_width > x + width - 30.0f)
        {
            break;
        }

        const Color building{
            0.035f + static_cast<float>(i % 3U) * 0.012f,
            0.050f,
            0.095f + static_cast<float>(i % 4U) * 0.018f,
            0.96f
        };

        renderer.rect(city_x, horizon - building_height, building_width, building_height, building, 3.0f);
        renderer.rect(city_x + 7.0f, horizon - building_height + 16.0f, building_width - 14.0f, 2.0f,
                      with_alpha((i % 2U == 0U) ? theme::cyan : theme::violet, 0.66f), 1.0f);

        const int windows = std::max(1, static_cast<int>((building_width - 16.0f) / 14.0f));
        for (int w = 0; w < windows; ++w)
        {
            renderer.rect(
                city_x + 8.0f + static_cast<float>(w) * 14.0f,
                horizon - building_height + 38.0f,
                5.0f,
                12.0f,
                with_alpha(theme::cyan, 0.20f),
                1.0f);
        }

        city_x += building_width + 11.0f;
    }

    renderer.rect(x + 1.0f, horizon, width - 2.0f, height - (horizon - y), Color{0.018f, 0.033f, 0.060f, 1.0f}, 0.0f);
    renderer.rect(x + 1.0f, horizon, width - 2.0f, 2.0f, with_alpha(theme::cyan, 0.48f), 1.0f);

    for (int i = 0; i < 8; ++i)
    {
        const float py = horizon + 28.0f + static_cast<float>(i) * 28.0f;
        renderer.rect(x + 32.0f, py, width - 64.0f, 1.0f, with_alpha(theme::cyan, 0.055f), 0.0f);
    }

    const float status_width = 245.0f;
    const float status_x = x + width - status_width - 24.0f;
    const float status_y = y + 92.0f;

    renderer.rect(status_x, status_y, status_width, 188.0f, Color{0.025f, 0.045f, 0.080f, 0.88f}, 14.0f);
    renderer.text(status_x + 18.0f, status_y + 16.0f, 13.0f, "SYSTEM STATUS", theme::text_muted);

    status_row(renderer, status_x + 18.0f, status_y + 52.0f, "Desktop shell", "READY", theme::success);
    status_row(renderer, status_x + 18.0f, status_y + 80.0f, "Renderer", "READY", theme::success);
    status_row(renderer, status_x + 18.0f, status_y + 108.0f, "OpenSim protocol", "NEXT", theme::warning);
    status_row(renderer, status_x + 18.0f, status_y + 136.0f, "World stream", "PENDING", theme::text_muted);

    renderer.rect(x + 24.0f, y + height - 58.0f, width - 48.0f, 38.0f, Color{0.025f, 0.043f, 0.073f, 0.92f}, 10.0f);
    renderer.text(x + 40.0f, y + height - 49.0f, 13.0f,
                  "Phase 0 preview - no OpenSim connection is attempted yet.", theme::text_muted);
}

void render_shell(UiRenderer& renderer, int width, int height, double mouse_x, double mouse_y)
{
    const float w = static_cast<float>(width);
    const float h = static_cast<float>(height);

    renderer.resize(width, height);
    renderer.rect(0.0f, 0.0f, w, h, theme::background, 0.0f);

    for (float gx = 0.0f; gx < w; gx += 64.0f)
    {
        renderer.rect(gx, 0.0f, 1.0f, h, with_alpha(theme::cyan, 0.025f), 0.0f);
    }

    for (float gy = 0.0f; gy < h; gy += 64.0f)
    {
        renderer.rect(0.0f, gy, w, 1.0f, with_alpha(theme::blue, 0.025f), 0.0f);
    }

    renderer.rect(0.0f, 0.0f, w, 76.0f, Color{0.025f, 0.038f, 0.072f, 0.96f}, 0.0f);
    renderer.rect(0.0f, 75.0f, w, 1.0f, with_alpha(theme::cyan, 0.22f), 0.0f);

    renderer.text(34.0f, 16.0f, 27.0f, "NEXVERSE", theme::text);
    renderer.text(185.0f, 23.0f, 12.0f, "VIEWER", theme::cyan);

    const std::string version =
        std::string{nexverse::core::version()} + "  |  C++23";
    const float version_width = renderer.text_width(12.0f, version);
    renderer.text(w - version_width - 34.0f, 25.0f, 12.0f, version, theme::text_muted);

    const float margin = 34.0f;
    const float content_y = 108.0f;
    const float content_height = std::max(580.0f, h - 150.0f);
    const float left_width = std::clamp(w * 0.30f, 350.0f, 430.0f);
    const float gap = 24.0f;
    const float preview_x = margin + left_width + gap;
    const float preview_width = std::max(520.0f, w - preview_x - margin);

    renderer.rect(margin, content_y, left_width, content_height, theme::surface, 18.0f);
    renderer.rect(margin, content_y, 4.0f, content_height, with_alpha(theme::cyan, 0.72f), 2.0f);

    renderer.text(margin + 28.0f, content_y + 30.0f, 13.0f, "NEXVERSE ACCESS", theme::cyan);
    renderer.text(margin + 28.0f, content_y + 58.0f, 28.0f, "Willkommen im NexVerse", theme::text);
    renderer.text(margin + 28.0f, content_y + 102.0f, 14.0f,
                  "Native OpenSim Viewer - Phase 0", theme::text_muted);

    const float field_x = margin + 28.0f;
    const float field_width = left_width - 56.0f;
    float field_y = content_y + 154.0f;

    field(renderer, field_x, field_y, field_width, "GRID / SERVER URL", "https://...");
    field_y += 90.0f;
    field(renderer, field_x, field_y, field_width, "BENUTZERNAME", "Avatar Name");
    field_y += 90.0f;
    field(renderer, field_x, field_y, field_width, "PASSWORT", "********");
    field_y += 90.0f;
    field(renderer, field_x, field_y, field_width, "STARTREGION", "Home");

    const float button_y = field_y + 93.0f;
    const bool hover = point_inside(mouse_x, mouse_y, field_x, button_y, field_width, 52.0f);
    const Color button_color = hover ? Color{0.100f, 0.720f, 0.940f, 1.0f} : theme::blue;

    renderer.rect(field_x, button_y, field_width, 52.0f, button_color, 12.0f);
    const std::string_view button_text = "WELT BETRETEN";
    const float button_text_width = renderer.text_width(15.0f, button_text);
    renderer.text(field_x + (field_width - button_text_width) * 0.5f, button_y + 8.0f, 15.0f, button_text, theme::text);

    renderer.text(field_x, button_y + 72.0f, 12.0f,
                  "Login wird in Phase 2 aktiviert.", theme::text_muted);

    render_world_preview(renderer, preview_x, content_y, preview_width, content_height);

    renderer.text(34.0f, h - 26.0f, 11.0f,
                  "NexVerse Viewer | MPL 2.0 | Phase 0 native desktop foundation", theme::text_muted);
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

    GLFWwindow* window = glfwCreateWindow(1440, 900, "NexVerse Viewer", nullptr, nullptr);
    if (window == nullptr)
    {
        core::error("Viewer window creation failed");
        glfwTerminate();
        return 11;
    }

    glfwSetWindowSizeLimits(window, 1100, 700, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    const GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK)
    {
        const auto* message = glewGetErrorString(glew_status);
        core::error(
            std::string{"GLEW initialization failed: "} +
            reinterpret_cast<const char*>(message));
        glfwDestroyWindow(window);
        glfwTerminate();
        return 12;
    }

    glGetError();

    int window_width = 0;
    int window_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);

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

            render_shell(renderer, window_width, window_height, mouse_x, mouse_y);

            glfwSwapBuffers(window);
        }
    }

    core::info("NexVerse Viewer shutdown");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
}
