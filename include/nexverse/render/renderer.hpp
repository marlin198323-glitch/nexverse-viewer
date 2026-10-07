/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace nexverse::render
{
struct RenderVec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct RenderQuaternion
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};

enum class RenderPrimitiveKind : std::uint8_t
{
    box = 0U,
    cylinder = 1U,
    prism = 2U,
    sphere = 3U,
    torus = 4U
};

enum class RenderProfileKind : std::uint8_t
{
    same = 0U,
    circle = 1U,
    square = 2U,
    triangle = 3U
};

struct RenderSceneObject
{
    std::uint32_t local_id = 0U;
    std::uint32_t parent_id = 0U;
    RenderPrimitiveKind kind = RenderPrimitiveKind::box;
    RenderVec3 position;
    RenderQuaternion rotation;
    RenderVec3 scale{1.0f, 1.0f, 1.0f};

    bool linear_path_deform = false;
    bool curved_path = false;
    float path_begin = 0.0f;
    float path_end = 1.0f;
    float profile_begin = 0.0f;
    float profile_end = 1.0f;
    float profile_hollow = 0.0f;
    RenderProfileKind hollow_profile = RenderProfileKind::same;
    float path_hole_x = 1.0f;
    float path_hole_y = 1.0f;
    float radius_offset = 0.0f;
    float revolutions = 1.0f;
    float skew = 0.0f;
    float taper_x = 0.0f;
    float taper_y = 0.0f;
    float shear_x = 0.0f;
    float shear_y = 0.0f;
    float twist_begin_radians = 0.0f;
    float twist_end_radians = 0.0f;

    std::string texture_id;
    std::array<float, 4> color{
        0.58f, 0.74f, 0.88f, 1.0f};
    float repeat_u = 1.0f;
    float repeat_v = 1.0f;
    float offset_u = 0.0f;
    float offset_v = 0.0f;
    float texture_rotation = 0.0f;
    bool fullbright = false;
    std::uint8_t shiny = 0U;
    float glow = 0.0f;
};

struct RenderCamera
{
    RenderVec3 position;
    RenderVec3 forward{1.0f, 0.0f, 0.0f};
    RenderVec3 left{0.0f, 1.0f, 0.0f};
    RenderVec3 up{0.0f, 0.0f, 1.0f};
    float vertical_fov_degrees = 60.0f;
    float near_clip = 0.1f;
    float far_clip = 256.0f;
};

struct RenderTerrain
{
    std::uint32_t generation = 0U;
    std::uint32_t revision = 0U;
    std::uint32_t width = 256U;
    std::uint32_t height = 256U;
    std::uint32_t patch_size = 16U;
    float water_height = 20.0f;
    std::uint32_t material_revision = 0U;
    std::array<float, 4> terrain_start_height{};
    std::array<float, 4> terrain_height_range{};
    std::vector<float> heights;
    std::vector<std::uint8_t> valid_patches;
};

struct RenderImageRgba8
{
    std::uint32_t width = 0U;
    std::uint32_t height = 0U;
    std::vector<std::uint8_t> pixels;

    [[nodiscard]] bool valid() const noexcept
    {
        return width > 0U &&
               height > 0U &&
               pixels.size() ==
                   static_cast<std::size_t>(width) *
                   static_cast<std::size_t>(height) * 4U;
    }
};

struct RenderTerrainTextures
{
    std::uint32_t generation = 0U;
    std::uint32_t material_revision = 0U;
    std::array<RenderImageRgba8, 4> detail;
};

struct RenderEnvironment
{
    std::uint32_t revision = 0U;
    bool available = false;
    RenderVec3 ambient{0.35f, 0.35f, 0.35f};
    RenderVec3 blue_density{0.12f, 0.22f, 0.38f};
    RenderVec3 blue_horizon{0.25f, 0.25f, 0.32f};
    RenderVec3 sunlight_color{0.72f, 0.76f, 0.90f};
    RenderVec3 water_fog_color{0.0156f, 0.149f, 0.2509f};
    float haze_density = 0.70f;
    float haze_horizon = 0.19f;
    float gamma = 1.0f;
    float sun_angle = 0.317f;
    float east_angle = 0.0f;
};

struct RenderDiagnostics
{
    std::string backend;
    std::string gpu;
    std::uint64_t vram_total_bytes = 0U;
    std::uint64_t vram_available_bytes = 0U;
    std::uint64_t draw_calls = 0U;
    std::uint64_t triangles = 0U;
    std::uint64_t visible_objects = 0U;
};

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual bool initialize(int width, int height) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void set_camera(const RenderCamera& camera) = 0;
    virtual void set_terrain(const RenderTerrain& terrain) = 0;
    virtual void set_terrain_textures(
        const RenderTerrainTextures& textures) = 0;
    virtual void set_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image) = 0;
    virtual void set_environment(
        const RenderEnvironment& environment) = 0;
    virtual void set_scene_objects(
        const std::vector<RenderSceneObject>& objects) = 0;
    virtual void begin_frame() = 0;
    virtual void render_world(double time_seconds) = 0;
    virtual void end_frame() = 0;
    virtual void shutdown() = 0;

    [[nodiscard]] virtual RenderDiagnostics diagnostics() const = 0;
};

[[nodiscard]] std::unique_ptr<Renderer> create_legacy_opengl_renderer();
}
