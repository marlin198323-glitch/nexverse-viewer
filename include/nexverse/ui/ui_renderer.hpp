/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "nexverse/ui/theme.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace nexverse::ui
{
class UiRenderer
{
public:
    UiRenderer();
    ~UiRenderer();

    UiRenderer(const UiRenderer&) = delete;
    UiRenderer& operator=(const UiRenderer&) = delete;

    bool initialize(int width, int height);
    void resize(int width, int height) noexcept;

    void rect(float x, float y, float width, float height, Color color, float radius = 0.0f);
    void text(float x, float y, float pixel_size, std::string_view value, Color color);
    float text_width(float pixel_size, std::string_view value) const noexcept;

    unsigned int create_rgba_texture(int width, int height, const unsigned char* pixels);
    bool update_rgba_texture(unsigned int texture, int width, int height, const unsigned char* pixels);
    void image(unsigned int texture, float x, float y, float width, float height, float alpha = 1.0f);
    void destroy_texture(unsigned int texture);

    bool font_ready() const noexcept;

private:
    struct Glyph
    {
        unsigned int texture = 0;
        int width = 0;
        int height = 0;
        int bearing_x = 0;
        int bearing_y = 0;
        unsigned int advance = 0;
    };

    unsigned int compile_program(const char* vertex_source, const char* fragment_source);
    bool initialize_shapes();
    bool initialize_images();
    bool initialize_text();
    void destroy();

    int width_ = 1;
    int height_ = 1;

    unsigned int shape_program_ = 0;
    unsigned int shape_vao_ = 0;
    unsigned int shape_vbo_ = 0;

    unsigned int text_program_ = 0;
    unsigned int text_vao_ = 0;
    unsigned int text_vbo_ = 0;

    unsigned int image_program_ = 0;
    unsigned int image_vao_ = 0;
    unsigned int image_vbo_ = 0;

    std::unordered_map<char, Glyph> glyphs_;
    bool font_ready_ = false;
};
}
