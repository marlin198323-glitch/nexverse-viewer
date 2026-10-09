/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/render/renderer.hpp"

#include <cstddef>
#include <vector>

namespace nexverse::render
{
// CPU-side classification; a texture with any nonopaque texel needs blending.
// The texture is only inspected once, on upload, never once per rendered face.
[[nodiscard]] bool texture_has_alpha(const RenderImageRgba8& image) noexcept;
[[nodiscard]] bool face_requires_blending(float tint_alpha, bool texture_alpha) noexcept;

struct VisibleObjectDistance
{
    std::size_t index = 0U;
    float squared_distance = 0.0f;
};

// Stable, furthest first. Render-only ordering never changes the WorldModel.
void sort_transparent_objects(std::vector<VisibleObjectDistance>& objects);
}
