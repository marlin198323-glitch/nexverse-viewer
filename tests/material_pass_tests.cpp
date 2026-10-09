/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/render/material_pass.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

int main()
{
    using namespace nexverse::render;
    const auto check = [](bool ok, const char* msg) {
        if (!ok) { std::cerr << msg << '\n'; return false; }
        return true;
    };
    RenderImageRgba8 image{2U, 1U, {255U, 10U, 0U, 255U, 0U, 1U, 2U, 255U}};
    if (!check(!texture_has_alpha(image), "Fully opaque RGBA detected as blended")) return 1;
    image.pixels.back() = 128U;
    if (!check(texture_has_alpha(image), "Partial texture alpha ignored")) return 2;
    image.pixels.back() = 0U;
    if (!check(texture_has_alpha(image), "Binary transparency ignored")) return 3;
    image.pixels.pop_back();
    if (!check(!texture_has_alpha(image), "Invalid image treated as textured")) return 4;

    if (!check(!face_requires_blending(1.0f, false), "Opaque face incorrectly blended")) return 5;
    if (!check(face_requires_blending(0.5f, false), "Tint transparency ignored")) return 6;
    if (!check(face_requires_blending(1.0f, true), "Texture transparency ignored")) return 7;
    if (!check(face_requires_blending(std::numeric_limits<float>::quiet_NaN(), false),
               "Invalid alpha treated as opaque")) return 8;

    std::vector<VisibleObjectDistance> objects{{3U, 5.0f}, {1U, 25.0f}, {2U, 25.0f}, {4U, 9.0f}};
    sort_transparent_objects(objects);
    if (!check(objects[0].index == 1U && objects[1].index == 2U &&
               objects[2].index == 4U && objects[3].index == 3U,
               "Back-to-front order or stable tie broken")) return 9;
    std::cout << "Material pass classification / stable alpha ordering passed\n";
}
