/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
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

struct RenderLegacyMap
{
    std::string texture_id;
    std::array<float, 2> repeat{1.0f, 1.0f};
    std::array<float, 2> offset{0.0f, 0.0f};
    float rotation = 0.0f;
};

struct RenderLegacyAppearance
{
    RenderLegacyMap normal;
    RenderLegacyMap specular;
    std::array<float, 3> specular_color{1.0f, 1.0f, 1.0f};
    std::uint8_t specular_exponent = 51U;
    std::uint8_t environment_intensity = 0U;
};

struct RenderFaceMaterial
{
    std::string texture_id;
    std::array<float, 4> color{
        1.0f, 1.0f, 1.0f, 1.0f};
    float repeat_u = 1.0f;
    float repeat_v = 1.0f;
    float offset_u = 0.0f;
    float offset_v = 0.0f;
    float rotation = 0.0f;
    bool fullbright = false;
    std::uint8_t shiny = 0U;
    std::uint8_t bumpiness = 0U;
    bool media_present = false;
    bool texgen_planar = false;
    float glow = 0.0f;
    std::string material_id;
    std::string render_material_id;
    // Legacy RenderMaterials cap data; defaults preserve TextureEntry alpha.
    std::uint8_t diffuse_alpha_mode = 1U;
    std::uint8_t alpha_mask_cutoff = 0U;
    RenderLegacyAppearance legacy;
};

struct RenderMeshFace
{
    std::uint32_t face = 0U;
    std::uint32_t first_index = 0U;
    std::uint32_t index_count = 0U;
};

struct RenderMeshGeometry
{
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<RenderMeshFace> faces;
};

struct RenderPointLight
{
    RenderVec3 position;
    RenderVec3 color{1.0f, 1.0f, 1.0f};
    float intensity = 0.0f;
    float radius = 0.0f;
    float falloff = 0.0f;
    float cutoff = 0.0f;
};

struct RenderParticleEmitter
{
    std::uint32_t local_id = 0U;
    std::uint32_t crc = 0U;
    RenderVec3 position;
    RenderQuaternion rotation;
    RenderVec3 acceleration;
    std::string texture_id;
    std::array<float, 4> start_color{1.0f,1.0f,1.0f,1.0f};
    std::array<float, 4> end_color{1.0f,1.0f,1.0f,0.0f};
    float burst_rate = 0.0f;
    float burst_radius = 0.0f;
    float speed_min = 0.0f;
    float speed_max = 0.0f;
    float max_age = 0.0f;
    float source_max_age = 0.0f;
    float start_glow = 0.0f;
    float end_glow = 0.0f;
    float start_scale = 0.2f;
    float end_scale = 0.2f;
    std::uint8_t burst_count = 0U;
    std::uint8_t pattern = 0U;
    std::uint32_t flags = 0U;
    std::uint32_t source_flags = 0U;
    bool target_available = false;
    RenderVec3 target_position;
    std::uint8_t blend_source = 7U;
    std::uint8_t blend_destination = 9U;
};

struct RenderSceneObject
{
    std::uint32_t local_id = 0U;
    std::uint32_t parent_id = 0U;
    RenderPrimitiveKind kind = RenderPrimitiveKind::box;
    std::string mesh_id;
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
    std::uint8_t bumpiness = 0U;
    bool media_present = false;
    bool texgen_planar = false;
    float glow = 0.0f;
    // Identifiers are authoritative OpenSim references, NOT resolved PBR data.
    std::string material_id;
    std::string render_material_id;
    // Legacy RenderMaterials cap data; defaults preserve TextureEntry alpha.
    std::uint8_t diffuse_alpha_mode = 1U;
    std::uint8_t alpha_mask_cutoff = 0U;
    RenderLegacyAppearance legacy;

    std::array<std::optional<RenderFaceMaterial>, 64>
        face_materials;
};

// Transitional humanoid marker until full OpenSim appearance
// (skeleton, baked skin, wearables and attachments) is implemented.
struct RenderAvatarProxy
{
    RenderVec3 position;
    RenderQuaternion rotation;
    bool is_self = false;
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
    // Actual per-frame glDrawElements from world-scene objects only.
    // Sky, terrain, water, UI and avatar proxies are not counted here.
    std::uint64_t scene_draw_calls = 0U;
    std::uint64_t scene_mesh_draw_calls = 0U;
    std::uint64_t scene_prim_draw_calls = 0U;
};

enum class GpuUploadFailure
{
    none, invalid_input, capacity, unsupported_dimensions,
    allocation, driver_error
};
[[nodiscard]] constexpr const char* gpu_upload_failure_name(
    GpuUploadFailure failure) noexcept
{
    switch(failure)
    {
    case GpuUploadFailure::none:return "none";
    case GpuUploadFailure::invalid_input:return "invalid_input";
    case GpuUploadFailure::capacity:return "capacity";
    case GpuUploadFailure::unsupported_dimensions:return "unsupported_dimensions";
    case GpuUploadFailure::allocation:return "allocation";
    case GpuUploadFailure::driver_error:return "driver_error";
    }
    return "unknown";
}
struct GpuUploadResult
{
    bool success=false;
    GpuUploadFailure failure=GpuUploadFailure::none;
    unsigned gl_error=0U;
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
    // True only after the GPU accepted this texture. A failed replacement
    // must preserve the previously resident texture for that UUID.
    [[nodiscard]] virtual bool set_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image) = 0;
    // Upload into a separate OpenGL name. If this fails, ALL designated
    // victim textures and the existing UUID binding must remain unchanged.
    [[nodiscard]] virtual GpuUploadResult replace_object_texture(
        const std::string& texture_id,
        const RenderImageRgba8& image,
        const std::vector<std::string>& evict_after_success) = 0;
    virtual void remove_object_texture(const std::string& texture_id) = 0;
    virtual void clear_object_textures() = 0;
    // GPU uploads and removals must happen on the render thread.
    [[nodiscard]] virtual bool set_mesh_asset(
        const std::string& mesh_id,
        const RenderMeshGeometry& geometry) = 0;
    [[nodiscard]] virtual GpuUploadResult upload_mesh_asset(
        const std::string& mesh_id,
        const RenderMeshGeometry& geometry) = 0;
    virtual void remove_mesh_asset(const std::string& mesh_id) = 0;
    virtual void clear_mesh_assets() = 0;
    virtual void set_environment(
        const RenderEnvironment& environment) = 0;
    virtual void set_scene_objects(
        const std::vector<RenderSceneObject>& objects) = 0;
    virtual void set_avatar_proxies(
        const std::vector<RenderAvatarProxy>& avatars) = 0;
    virtual void set_point_lights(
        const std::vector<RenderPointLight>& lights) = 0;
    virtual void set_particle_emitters(
        const std::vector<RenderParticleEmitter>& emitters) = 0;
    virtual void begin_frame() = 0;
    virtual void render_world(double time_seconds) = 0;
    virtual void end_frame() = 0;
    virtual void shutdown() = 0;

    [[nodiscard]] virtual RenderDiagnostics diagnostics() const = 0;
};

[[nodiscard]] std::unique_ptr<Renderer> create_legacy_opengl_renderer();
}
