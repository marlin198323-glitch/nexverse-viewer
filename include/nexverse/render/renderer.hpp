/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace nexverse::render
{
struct RenderDiagnostics
{
    std::string backend;
    std::string gpu;
    std::uint64_t vram_total_bytes = 0U;
    std::uint64_t vram_available_bytes = 0U;
    std::uint64_t draw_calls = 0U;
    std::uint64_t triangles = 0U;
};

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual bool initialize(int width, int height) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void begin_frame() = 0;
    virtual void render_test_geometry(double time_seconds) = 0;
    virtual void end_frame() = 0;
    virtual void shutdown() = 0;

    [[nodiscard]] virtual RenderDiagnostics diagnostics() const = 0;
};

[[nodiscard]] std::unique_ptr<Renderer> create_legacy_opengl_renderer();
}
