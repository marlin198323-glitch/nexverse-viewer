/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/renderer.hpp"

#include "nexverse/core/logger.hpp"

#include <glad/glad.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

namespace
{
constexpr GLenum gl_gpu_memory_info_total_available_memory_nvx = 0x9048;
constexpr GLenum gl_gpu_memory_info_current_available_vidmem_nvx = 0x9049;

constexpr const char* vertex_shader_source = R"GLSL(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform float uAngle;
uniform float uAspect;

out vec3 vColor;

void main()
{
    float c = cos(uAngle);
    float s = sin(uAngle);
    mat2 rotation = mat2(c, -s, s, c);

    vec2 rotated = rotation * aPosition.xy;
    rotated.x /= max(uAspect, 0.001);

    gl_Position = vec4(rotated, aPosition.z, 1.0);
    vColor = aColor;
}
)GLSL";

constexpr const char* fragment_shader_source = R"GLSL(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
)GLSL";

std::string shader_log(GLuint shader)
{
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    return log;
}

std::string program_log(GLuint program)
{
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    return log;
}

GLuint compile_shader(GLenum type, const char* source)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        nexverse::core::error(
            "Legacy renderer shader compile failed: " + shader_log(shader));
        glDeleteShader(shader);
        return 0U;
    }

    return shader;
}

GLuint create_program()
{
    const GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    if (vertex == 0U)
    {
        return 0U;
    }

    const GLuint fragment =
        compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if (fragment == 0U)
    {
        glDeleteShader(vertex);
        return 0U;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        nexverse::core::error(
            "Legacy renderer program link failed: " + program_log(program));
        glDeleteProgram(program);
        return 0U;
    }

    return program;
}

bool has_extension(std::string_view name)
{
    GLint count = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);

    for (GLint index = 0; index < count; ++index)
    {
        const auto* value = reinterpret_cast<const char*>(
            glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(index)));
        if (value != nullptr && name == value)
        {
            return true;
        }
    }

    return false;
}
}

namespace nexverse::render
{
class LegacyOpenGlRenderer final : public Renderer
{
public:
    ~LegacyOpenGlRenderer() override
    {
        shutdown();
    }

    bool initialize(int width, int height) override
    {
        width_ = width > 0 ? width : 1;
        height_ = height > 0 ? height : 1;

        program_ = create_program();
        if (program_ == 0U)
        {
            return false;
        }

        constexpr std::array<float, 18> vertices{
             0.00f,  0.38f, 0.0f, 0.05f, 0.85f, 1.00f,
            -0.38f, -0.32f, 0.0f, 0.65f, 0.15f, 1.00f,
             0.38f, -0.32f, 0.0f, 1.00f, 0.25f, 0.55f
        };

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(vertices)),
            vertices.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0U);
        glVertexAttribPointer(
            0U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(6U * sizeof(float)),
            nullptr);

        glEnableVertexAttribArray(1U);
        glVertexAttribPointer(
            1U,
            3,
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLsizei>(6U * sizeof(float)),
            reinterpret_cast<void*>(3U * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0U);
        glBindVertexArray(0U);

        diagnostics_.backend = "Legacy OpenGL 3.3";
        if (const auto* gpu =
                reinterpret_cast<const char*>(glGetString(GL_RENDERER));
            gpu != nullptr)
        {
            diagnostics_.gpu = gpu;
        }

        nvx_memory_info_ = has_extension("GL_NVX_gpu_memory_info");
        refresh_vram();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        core::info("OGLRender legacy OpenGL backend initialized");
        return true;
    }

    void resize(int width, int height) override
    {
        width_ = width > 0 ? width : 1;
        height_ = height > 0 ? height : 1;
    }

    void begin_frame() override
    {
        diagnostics_.draw_calls = 0U;
        diagnostics_.triangles = 0U;

        glViewport(0, 0, width_, height_);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.008f, 0.018f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render_test_geometry(double time_seconds) override
    {
        if (program_ == 0U || vao_ == 0U)
        {
            return;
        }

        const float aspect =
            static_cast<float>(width_) /
            static_cast<float>(height_ > 0 ? height_ : 1);
        const float angle =
            static_cast<float>(std::fmod(time_seconds * 0.55, 6.283185307179586));

        glUseProgram(program_);
        glUniform1f(glGetUniformLocation(program_, "uAngle"), angle);
        glUniform1f(glGetUniformLocation(program_, "uAspect"), aspect);

        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0U);
        glUseProgram(0U);

        diagnostics_.draw_calls = 1U;
        diagnostics_.triangles = 1U;
    }

    void end_frame() override
    {
        refresh_vram();
    }

    void shutdown() override
    {
        if (vbo_ != 0U)
        {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0U;
        }
        if (vao_ != 0U)
        {
            glDeleteVertexArrays(1, &vao_);
            vao_ = 0U;
        }
        if (program_ != 0U)
        {
            glDeleteProgram(program_);
            program_ = 0U;
        }
    }

    [[nodiscard]] RenderDiagnostics diagnostics() const override
    {
        return diagnostics_;
    }

private:
    void refresh_vram()
    {
        if (!nvx_memory_info_)
        {
            return;
        }

        GLint total_kib = 0;
        GLint available_kib = 0;
        glGetIntegerv(
            gl_gpu_memory_info_total_available_memory_nvx,
            &total_kib);
        glGetIntegerv(
            gl_gpu_memory_info_current_available_vidmem_nvx,
            &available_kib);

        if (total_kib > 0)
        {
            diagnostics_.vram_total_bytes =
                static_cast<std::uint64_t>(total_kib) * 1024U;
        }
        if (available_kib > 0)
        {
            diagnostics_.vram_available_bytes =
                static_cast<std::uint64_t>(available_kib) * 1024U;
        }
    }

    int width_ = 1;
    int height_ = 1;
    GLuint program_ = 0U;
    GLuint vao_ = 0U;
    GLuint vbo_ = 0U;
    bool nvx_memory_info_ = false;
    RenderDiagnostics diagnostics_;
};

std::unique_ptr<Renderer> create_legacy_opengl_renderer()
{
    return std::make_unique<LegacyOpenGlRenderer>();
}
}
