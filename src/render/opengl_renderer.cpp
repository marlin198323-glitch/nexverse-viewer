/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/render/renderer.hpp"

#include "nexverse/core/logger.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>

namespace
{
using nexverse::render::RenderCamera;
using nexverse::render::RenderVec3;

constexpr GLenum gl_gpu_memory_info_total_available_memory_nvx = 0x9048;
constexpr GLenum gl_gpu_memory_info_current_available_vidmem_nvx = 0x9049;

struct Mat4
{
    std::array<float, 16> value{};
};

float dot(RenderVec3 a, RenderVec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

RenderVec3 cross(RenderVec3 a, RenderVec3 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

RenderVec3 normalize(RenderVec3 v)
{
    const float length = std::sqrt(dot(v, v));
    if (length <= 0.000001f)
    {
        return {};
    }
    return {v.x / length, v.y / length, v.z / length};
}

RenderVec3 add(RenderVec3 a, RenderVec3 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

RenderVec3 scale(RenderVec3 v, float amount)
{
    return {v.x * amount, v.y * amount, v.z * amount};
}

Mat4 identity()
{
    Mat4 result;
    result.value[0] = 1.0f;
    result.value[5] = 1.0f;
    result.value[10] = 1.0f;
    result.value[15] = 1.0f;
    return result;
}

Mat4 multiply(const Mat4& a, const Mat4& b)
{
    Mat4 result;
    for (std::size_t column = 0U; column < 4U; ++column)
    {
        for (std::size_t row = 0U; row < 4U; ++row)
        {
            float sum = 0.0f;
            for (std::size_t k = 0U; k < 4U; ++k)
            {
                sum +=
                    a.value[k * 4U + row] *
                    b.value[column * 4U + k];
            }
            result.value[column * 4U + row] = sum;
        }
    }
    return result;
}

Mat4 translation(RenderVec3 position)
{
    Mat4 result = identity();
    result.value[12] = position.x;
    result.value[13] = position.y;
    result.value[14] = position.z;
    return result;
}

Mat4 rotation_z(float angle)
{
    Mat4 result = identity();
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    result.value[0] = c;
    result.value[1] = s;
    result.value[4] = -s;
    result.value[5] = c;
    return result;
}

Mat4 rotation_x(float angle)
{
    Mat4 result = identity();
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    result.value[5] = c;
    result.value[6] = s;
    result.value[9] = -s;
    result.value[10] = c;
    return result;
}

Mat4 perspective(
    float vertical_fov_degrees,
    float aspect,
    float near_clip,
    float far_clip)
{
    const float safe_aspect = std::max(aspect, 0.001f);
    const float safe_near = std::max(near_clip, 0.001f);
    const float safe_far = std::max(far_clip, safe_near + 0.001f);
    const float fov =
        std::clamp(vertical_fov_degrees, 20.0f, 120.0f) *
        std::numbers::pi_v<float> / 180.0f;
    const float f = 1.0f / std::tan(fov * 0.5f);

    Mat4 result;
    result.value[0] = f / safe_aspect;
    result.value[5] = f;
    result.value[10] =
        (safe_far + safe_near) / (safe_near - safe_far);
    result.value[11] = -1.0f;
    result.value[14] =
        (2.0f * safe_far * safe_near) / (safe_near - safe_far);
    return result;
}

Mat4 view_matrix(const RenderCamera& camera)
{
    RenderVec3 forward = normalize(camera.forward);
    RenderVec3 up = normalize(camera.up);
    if (dot(forward, forward) <= 0.0f)
    {
        forward = {1.0f, 0.0f, 0.0f};
    }
    if (dot(up, up) <= 0.0f)
    {
        up = {0.0f, 0.0f, 1.0f};
    }

    RenderVec3 right = normalize(cross(forward, up));
    if (dot(right, right) <= 0.0f)
    {
        right = {0.0f, -1.0f, 0.0f};
    }
    up = normalize(cross(right, forward));

    Mat4 result = identity();
    result.value[0] = right.x;
    result.value[1] = up.x;
    result.value[2] = -forward.x;

    result.value[4] = right.y;
    result.value[5] = up.y;
    result.value[6] = -forward.y;

    result.value[8] = right.z;
    result.value[9] = up.z;
    result.value[10] = -forward.z;

    result.value[12] = -dot(right, camera.position);
    result.value[13] = -dot(up, camera.position);
    result.value[14] = dot(forward, camera.position);
    return result;
}

constexpr const char* vertex_shader_source = R"GLSL(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform mat4 uMvp;

out vec3 vColor;

void main()
{
    gl_Position = uMvp * vec4(aPosition, 1.0);
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

        constexpr std::array<float, 48> vertices{
            -0.75f, -0.75f, -0.75f, 0.05f, 0.85f, 1.00f,
             0.75f, -0.75f, -0.75f, 0.65f, 0.15f, 1.00f,
             0.75f,  0.75f, -0.75f, 1.00f, 0.25f, 0.55f,
            -0.75f,  0.75f, -0.75f, 0.10f, 1.00f, 0.55f,
            -0.75f, -0.75f,  0.75f, 1.00f, 0.65f, 0.10f,
             0.75f, -0.75f,  0.75f, 0.20f, 0.55f, 1.00f,
             0.75f,  0.75f,  0.75f, 1.00f, 0.15f, 0.70f,
            -0.75f,  0.75f,  0.75f, 0.35f, 0.95f, 1.00f
        };

        constexpr std::array<std::uint32_t, 36> indices{
            0U, 1U, 2U, 2U, 3U, 0U,
            4U, 6U, 5U, 6U, 4U, 7U,
            0U, 4U, 5U, 5U, 1U, 0U,
            3U, 2U, 6U, 6U, 7U, 3U,
            1U, 5U, 6U, 6U, 2U, 1U,
            0U, 3U, 7U, 7U, 4U, 0U
        };

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(vertices)),
            vertices.data(),
            GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(indices)),
            indices.data(),
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

        glBindVertexArray(0U);
        glBindBuffer(GL_ARRAY_BUFFER, 0U);

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

    void set_camera(const RenderCamera& camera) override
    {
        camera_ = camera;

        if (!test_anchor_initialized_)
        {
            RenderVec3 forward = normalize(camera_.forward);
            if (dot(forward, forward) <= 0.0f)
            {
                forward = {1.0f, 0.0f, 0.0f};
            }

            test_anchor_ = add(
                camera_.position,
                scale(forward, 6.0f));
            test_anchor_initialized_ = true;
        }
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
        if (program_ == 0U || vao_ == 0U || !test_anchor_initialized_)
        {
            return;
        }

        const float aspect =
            static_cast<float>(width_) /
            static_cast<float>(height_ > 0 ? height_ : 1);
        const float angle =
            static_cast<float>(
                std::fmod(time_seconds * 0.45, 2.0 * std::numbers::pi));

        const Mat4 projection = perspective(
            camera_.vertical_fov_degrees,
            aspect,
            camera_.near_clip,
            camera_.far_clip);
        const Mat4 view = view_matrix(camera_);
        const Mat4 model = multiply(
            translation(test_anchor_),
            multiply(
                rotation_z(angle),
                rotation_x(angle * 0.6f)));
        const Mat4 mvp =
            multiply(projection, multiply(view, model));

        glUseProgram(program_);
        glUniformMatrix4fv(
            glGetUniformLocation(program_, "uMvp"),
            1,
            GL_FALSE,
            mvp.value.data());

        glBindVertexArray(vao_);
        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            nullptr);
        glBindVertexArray(0U);
        glUseProgram(0U);

        diagnostics_.draw_calls = 1U;
        diagnostics_.triangles = 12U;
    }

    void end_frame() override
    {
        refresh_vram();
    }

    void shutdown() override
    {
        if (ebo_ != 0U)
        {
            glDeleteBuffers(1, &ebo_);
            ebo_ = 0U;
        }
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
    GLuint ebo_ = 0U;
    bool nvx_memory_info_ = false;
    bool test_anchor_initialized_ = false;
    RenderCamera camera_;
    RenderVec3 test_anchor_;
    RenderDiagnostics diagnostics_;
};

std::unique_ptr<Renderer> create_legacy_opengl_renderer()
{
    return std::make_unique<LegacyOpenGlRenderer>();
}
}
