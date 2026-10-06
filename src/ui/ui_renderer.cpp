/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/ui/ui_renderer.hpp"

#include "nexverse/core/logger.hpp"

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <array>
#include <filesystem>
#include <string>
#include <vector>

namespace
{
constexpr const char* shape_vertex_shader = R"GLSL(
#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aLocal;

uniform vec2 uViewport;

out vec2 vLocal;

void main()
{
    vec2 ndc;
    ndc.x = (aPosition.x / uViewport.x) * 2.0 - 1.0;
    ndc.y = 1.0 - (aPosition.y / uViewport.y) * 2.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
    vLocal = aLocal;
}
)GLSL";

constexpr const char* shape_fragment_shader = R"GLSL(
#version 330 core
in vec2 vLocal;

uniform vec4 uColor;
uniform vec2 uSize;
uniform float uRadius;

out vec4 FragColor;

void main()
{
    vec2 point = vLocal * uSize;
    vec2 halfSize = uSize * 0.5;
    float radius = min(uRadius, min(halfSize.x, halfSize.y));
    vec2 q = abs(point - halfSize) - (halfSize - vec2(radius));
    float distanceToEdge =
        min(max(q.x, q.y), 0.0) +
        length(max(q, vec2(0.0))) -
        radius;

    float alpha = 1.0 - smoothstep(-0.75, 0.75, distanceToEdge);
    FragColor = vec4(uColor.rgb, uColor.a * alpha);
}
)GLSL";

constexpr const char* image_vertex_shader = R"GLSL(
#version 330 core
layout (location = 0) in vec4 aVertex;

uniform vec2 uViewport;

out vec2 vTexCoord;

void main()
{
    vec2 ndc;
    ndc.x = (aVertex.x / uViewport.x) * 2.0 - 1.0;
    ndc.y = 1.0 - (aVertex.y / uViewport.y) * 2.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
    vTexCoord = aVertex.zw;
}
)GLSL";

constexpr const char* image_fragment_shader = R"GLSL(
#version 330 core
in vec2 vTexCoord;

uniform sampler2D uImage;
uniform float uAlpha;

out vec4 FragColor;

void main()
{
    vec4 sampleColor = texture(uImage, vTexCoord);
    FragColor = vec4(sampleColor.rgb, sampleColor.a * uAlpha);
}
)GLSL";

constexpr const char* text_vertex_shader = R"GLSL(
#version 330 core
layout (location = 0) in vec4 aVertex;

uniform vec2 uViewport;

out vec2 vTexCoord;

void main()
{
    vec2 ndc;
    ndc.x = (aVertex.x / uViewport.x) * 2.0 - 1.0;
    ndc.y = 1.0 - (aVertex.y / uViewport.y) * 2.0;
    gl_Position = vec4(ndc, 0.0, 1.0);
    vTexCoord = aVertex.zw;
}
)GLSL";

constexpr const char* text_fragment_shader = R"GLSL(
#version 330 core
in vec2 vTexCoord;

uniform sampler2D uText;
uniform vec4 uColor;

out vec4 FragColor;

void main()
{
    float coverage = texture(uText, vTexCoord).r;
    FragColor = vec4(uColor.rgb, uColor.a * coverage);
}
)GLSL";

std::vector<std::filesystem::path> font_candidates()
{
#if defined(_WIN32)
    return {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };
#elif defined(__APPLE__)
    return {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttf"
    };
#else
    return {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
    };
#endif
}

std::string shader_log(unsigned int shader)
{
    int length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    return log;
}

std::string program_log(unsigned int program)
{
    int length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    return log;
}
}

namespace nexverse::ui
{
UiRenderer::UiRenderer() = default;

UiRenderer::~UiRenderer()
{
    destroy();
}

bool UiRenderer::initialize(int width, int height)
{
    width_ = width > 0 ? width : 1;
    height_ = height > 0 ? height : 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!initialize_shapes())
    {
        core::error("UI shape renderer initialization failed");
        return false;
    }

    if (!initialize_images())
    {
        core::error("UI image renderer initialization failed");
        return false;
    }

    if (!initialize_text())
    {
        core::error("UI font renderer initialization failed");
        return false;
    }

    core::info("NexVerse UI renderer initialized");
    return true;
}

void UiRenderer::resize(int width, int height) noexcept
{
    width_ = width > 0 ? width : 1;
    height_ = height > 0 ? height : 1;
}

unsigned int UiRenderer::compile_program(const char* vertex_source, const char* fragment_source)
{
    const auto compile = [](unsigned int type, const char* source) -> unsigned int
    {
        const unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            core::error(shader_log(shader));
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    };

    const unsigned int vertex = compile(GL_VERTEX_SHADER, vertex_source);
    if (vertex == 0)
    {
        return 0;
    }

    const unsigned int fragment = compile(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment == 0)
    {
        glDeleteShader(vertex);
        return 0;
    }

    const unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        core::error(program_log(program));
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

bool UiRenderer::initialize_shapes()
{
    shape_program_ = compile_program(shape_vertex_shader, shape_fragment_shader);
    if (shape_program_ == 0)
    {
        return false;
    }

    glGenVertexArrays(1, &shape_vao_);
    glGenBuffers(1, &shape_vbo_);

    glBindVertexArray(shape_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shape_vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * 24), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(float) * 4), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(float) * 4),
        reinterpret_cast<void*>(sizeof(float) * 2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

bool UiRenderer::initialize_images()
{
    image_program_ = compile_program(image_vertex_shader, image_fragment_shader);
    if (image_program_ == 0)
    {
        return false;
    }

    glGenVertexArrays(1, &image_vao_);
    glGenBuffers(1, &image_vbo_);

    glBindVertexArray(image_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, image_vbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(sizeof(float) * 24),
        nullptr,
        GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(float) * 4),
        nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

bool UiRenderer::initialize_text()
{
    text_program_ = compile_program(text_vertex_shader, text_fragment_shader);
    if (text_program_ == 0)
    {
        return false;
    }

    FT_Library library{};
    if (FT_Init_FreeType(&library) != 0)
    {
        core::error("FreeType initialization failed");
        return false;
    }

    std::filesystem::path selected_font;
    for (const auto& candidate : font_candidates())
    {
        if (std::filesystem::exists(candidate))
        {
            selected_font = candidate;
            break;
        }
    }

    if (selected_font.empty())
    {
        FT_Done_FreeType(library);
        core::error("No supported system font found");
        return false;
    }

    FT_Face face{};
    if (FT_New_Face(library, selected_font.string().c_str(), 0, &face) != 0)
    {
        FT_Done_FreeType(library);
        core::error("Could not load system font");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char character = 32; character < 127; ++character)
    {
        if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0)
        {
            continue;
        }

        unsigned int texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            static_cast<GLsizei>(face->glyph->bitmap.width),
            static_cast<GLsizei>(face->glyph->bitmap.rows),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glyphs_.emplace(
            static_cast<char>(character),
            Glyph{
                texture,
                static_cast<int>(face->glyph->bitmap.width),
                static_cast<int>(face->glyph->bitmap.rows),
                face->glyph->bitmap_left,
                face->glyph->bitmap_top,
                static_cast<unsigned int>(face->glyph->advance.x)
            });
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    glGenVertexArrays(1, &text_vao_);
    glGenBuffers(1, &text_vbo_);

    glBindVertexArray(text_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * 24), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(float) * 4), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    font_ready_ = !glyphs_.empty();
    core::info(std::string{"UI font: "} + selected_font.string());
    return font_ready_;
}

void UiRenderer::rect(float x, float y, float width, float height, Color color, float radius)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const std::array<float, 24> vertices{
        x,         y,          0.0f, 0.0f,
        x,         y + height, 0.0f, 1.0f,
        x + width, y + height, 1.0f, 1.0f,

        x,         y,          0.0f, 0.0f,
        x + width, y + height, 1.0f, 1.0f,
        x + width, y,          1.0f, 0.0f
    };

    glUseProgram(shape_program_);
    glUniform2f(glGetUniformLocation(shape_program_, "uViewport"), static_cast<float>(width_), static_cast<float>(height_));
    glUniform4f(glGetUniformLocation(shape_program_, "uColor"), color.r, color.g, color.b, color.a);
    glUniform2f(glGetUniformLocation(shape_program_, "uSize"), width, height);
    glUniform1f(glGetUniformLocation(shape_program_, "uRadius"), radius);

    glBindVertexArray(shape_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shape_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(vertices)), vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void UiRenderer::text(float x, float y, float pixel_size, std::string_view value, Color color)
{
    if (!font_ready_ || pixel_size <= 0.0f)
    {
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float scale = pixel_size / 48.0f;
    float cursor = x;

    glUseProgram(text_program_);
    glUniform2f(glGetUniformLocation(text_program_, "uViewport"), static_cast<float>(width_), static_cast<float>(height_));
    glUniform4f(glGetUniformLocation(text_program_, "uColor"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(text_program_, "uText"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_vao_);

    for (const char character : value)
    {
        const auto found = glyphs_.find(character);
        if (found == glyphs_.end())
        {
            continue;
        }

        const Glyph& glyph = found->second;
        const float xpos = cursor + static_cast<float>(glyph.bearing_x) * scale;
        const float ypos = y + (48.0f - static_cast<float>(glyph.bearing_y)) * scale;
        const float width = static_cast<float>(glyph.width) * scale;
        const float height = static_cast<float>(glyph.height) * scale;

        const std::array<float, 24> vertices{
            xpos,         ypos,          0.0f, 0.0f,
            xpos,         ypos + height, 0.0f, 1.0f,
            xpos + width, ypos + height, 1.0f, 1.0f,

            xpos,         ypos,          0.0f, 0.0f,
            xpos + width, ypos + height, 1.0f, 1.0f,
            xpos + width, ypos,          1.0f, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, glyph.texture);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(vertices)), vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursor += static_cast<float>(glyph.advance >> 6U) * scale;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

float UiRenderer::text_width(float pixel_size, std::string_view value) const noexcept
{
    if (!font_ready_ || pixel_size <= 0.0f)
    {
        return 0.0f;
    }

    const float scale = pixel_size / 48.0f;
    float width = 0.0f;

    for (const char character : value)
    {
        const auto found = glyphs_.find(character);
        if (found != glyphs_.end())
        {
            width += static_cast<float>(found->second.advance >> 6U) * scale;
        }
    }

    return width;
}

unsigned int UiRenderer::create_rgba_texture(
    int width,
    int height,
    const unsigned char* pixels)
{
    if (width <= 0 || height <= 0 || pixels == nullptr)
    {
        return 0U;
    }

    unsigned int texture = 0U;
    glGenTextures(1, &texture);
    if (texture == 0U) return 0U;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

bool UiRenderer::update_rgba_texture(
    unsigned int texture,
    int width,
    int height,
    const unsigned char* pixels)
{
    if (texture == 0U || width <= 0 || height <= 0 || pixels == nullptr)
    {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void UiRenderer::image(
    unsigned int texture,
    float x,
    float y,
    float width,
    float height,
    float alpha)
{
    if (texture == 0U || width <= 0.0f || height <= 0.0f)
    {
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const std::array<float, 24> vertices{
        x,         y,          0.0f, 0.0f,
        x,         y + height, 0.0f, 1.0f,
        x + width, y + height, 1.0f, 1.0f,

        x,         y,          0.0f, 0.0f,
        x + width, y + height, 1.0f, 1.0f,
        x + width, y,          1.0f, 0.0f
    };

    glUseProgram(image_program_);
    glUniform2f(
        glGetUniformLocation(image_program_, "uViewport"),
        static_cast<float>(width_),
        static_cast<float>(height_));
    glUniform1f(
        glGetUniformLocation(image_program_, "uAlpha"),
        std::clamp(alpha, 0.0f, 1.0f));
    glUniform1i(glGetUniformLocation(image_program_, "uImage"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(image_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, image_vbo_);
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        static_cast<GLsizeiptr>(sizeof(vertices)),
        vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void UiRenderer::destroy_texture(unsigned int texture)
{
    if (texture != 0U)
    {
        glDeleteTextures(1, &texture);
    }
}

bool UiRenderer::font_ready() const noexcept
{
    return font_ready_;
}

void UiRenderer::destroy()
{
    for (const auto& [character, glyph] : glyphs_)
    {
        static_cast<void>(character);
        if (glyph.texture != 0)
        {
            glDeleteTextures(1, &glyph.texture);
        }
    }
    glyphs_.clear();

    if (text_vbo_ != 0) glDeleteBuffers(1, &text_vbo_);
    if (text_vao_ != 0) glDeleteVertexArrays(1, &text_vao_);
    if (text_program_ != 0) glDeleteProgram(text_program_);

    if (image_vbo_ != 0) glDeleteBuffers(1, &image_vbo_);
    if (image_vao_ != 0) glDeleteVertexArrays(1, &image_vao_);
    if (image_program_ != 0) glDeleteProgram(image_program_);

    if (shape_vbo_ != 0) glDeleteBuffers(1, &shape_vbo_);
    if (shape_vao_ != 0) glDeleteVertexArrays(1, &shape_vao_);
    if (shape_program_ != 0) glDeleteProgram(shape_program_);

    text_vbo_ = 0;
    text_vao_ = 0;
    text_program_ = 0;
    image_vbo_ = 0;
    image_vao_ = 0;
    image_program_ = 0;
    shape_vbo_ = 0;
    shape_vao_ = 0;
    shape_program_ = 0;
    font_ready_ = false;
}
}
