/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"

#include "nexverse/core/logger.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/ui/login_model.hpp"
#include "nexverse/ui/login_screen.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace
{
struct InputContext
{
    nexverse::ui::LoginModel* model = nullptr;
    nexverse::ui::LoginScreen* screen = nullptr;
};

InputContext* input_context(GLFWwindow* window)
{
    return static_cast<InputContext*>(glfwGetWindowUserPointer(window));
}

void glfw_error_callback(int error, const char* description)
{
    nexverse::core::error(
        "GLFW error " + std::to_string(error) + ": " +
        (description != nullptr ? description : "unknown"));
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    InputContext* context = input_context(window);
    if (context != nullptr && context->model != nullptr)
    {
        context->model->append_codepoint(static_cast<char32_t>(codepoint));
    }
}

void key_callback(GLFWwindow* window, int key, int, int action, int mods)
{
    InputContext* context = input_context(window);
    if (context == nullptr || context->model == nullptr)
    {
        return;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        context->model->focus_next((mods & GLFW_MOD_SHIFT) != 0);
        return;
    }

    if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        context->model->backspace();
        return;
    }

    if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) && action == GLFW_PRESS)
    {
        context->model->attempt_submit();
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
    {
        return;
    }

    InputContext* context = input_context(window);
    if (context == nullptr || context->model == nullptr || context->screen == nullptr)
    {
        return;
    }

    double mouse_x = 0.0;
    double mouse_y = 0.0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);

    const auto field = context->screen->field_at(width, height, mouse_x, mouse_y);
    if (field.has_value())
    {
        context->model->focus(*field);
        return;
    }

    if (context->screen->submit_at(width, height, mouse_x, mouse_y))
    {
        context->model->attempt_submit();
    }
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

    ui::LoginModel login_model;
    ui::LoginScreen login_screen;
    InputContext context{&login_model, &login_screen};

    glfwSetWindowUserPointer(window, &context);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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

            login_screen.render(
                renderer,
                login_model,
                window_width,
                window_height,
                mouse_x,
                mouse_y);

            glfwSwapBuffers(window);
        }
    }

    glfwSetWindowUserPointer(window, nullptr);
    core::info("OpenGenesisLINK Viewer shutdown");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
}
