/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/render/material_pass.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace nexverse::render
{
bool texture_has_alpha(const RenderImageRgba8& image) noexcept
{
    if (!image.valid()) return false;
    for (std::size_t offset = 3U; offset < image.pixels.size(); offset += 4U)
        if (image.pixels[offset] != UINT8_MAX) return true;
    return false;
}

bool face_requires_blending(float tint_alpha, bool texture_alpha) noexcept
{
    // Nonfinite alpha never becomes an opaque depth writer.
    return !std::isfinite(tint_alpha) || tint_alpha < 0.999f || texture_alpha;
}

bool face_material_requires_blending(
    float tint_alpha,bool texture_alpha,unsigned diffuse_alpha_mode) noexcept
{
    return face_requires_blending(
        tint_alpha,texture_alpha && diffuse_alpha_mode==1U);
}

void sort_transparent_objects(std::vector<VisibleObjectDistance>& objects)
{
    std::stable_sort(objects.begin(), objects.end(),
        [](const VisibleObjectDistance& left, const VisibleObjectDistance& right)
        {
            return left.squared_distance > right.squared_distance;
        });
}
}
