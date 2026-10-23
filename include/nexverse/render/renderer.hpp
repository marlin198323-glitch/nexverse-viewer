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
#include <unordered_map>
#include "nexverse/protocol/avatar_animation.hpp"
#include "nexverse/assets/firestorm_keyframe_motion.hpp"

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
    // Only original .llm avatar body meshes carry skin data.
    std::vector<float> skin_weights;
    // Each weight's integral index is a Firestorm JointRenderData palette
    // entry; fractional part blends its successor.
    std::vector<std::uint16_t> skin_palette;
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
    std::string object_id; // server object UUID; attachment identification
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
    // From server AvatarAppearance TextureEntry, only Head/Upper/Lower.
    // Empty slot uses the provisional flat proxy tint.
    std::array<std::string,3U> baked_texture_ids{};
    // Local simulator ObjectUpdate avatar ID, for children/attachments.
    std::uint32_t local_id = 0U;
    // Strictly server-provided AttachmentBlock ID -> point mapping.
    std::unordered_map<std::string,std::uint8_t> attachments{};
    bool moving=false; // drive provisional local locomotion, not server truth
    protocol::AvatarMotion motion=protocol::AvatarMotion::idle;
    bool animation_from_server=false;
    // Optional decoded LLKeyframeMotion asset. Populated only when a
    // verified animation asset is available; High20 UUID alone is not
    // proof that the actual keyframe bytes were downloaded.
    std::shared_ptr<const assets::FirestormKeyframeMotion> keyframe_clip;
    // Seconds since this server animation sequence began, not app uptime.
    // Negative = procedural fallback / no known server clip start.
    float keyframe_elapsed_seconds=-1.0f;
    // Authenticated AvatarAppearance baked Hair face, NOT a fabricated
    // head bake or client-generated mesh clothing texture.
    std::string hair_bake_texture_id;
    // Original Firestorm eyeBallLeft/Right pair shares real BAKED_EYES
    // (TE face 11). Never alias head skin as an eyeball texture.
    std::string eyes_bake_texture_id;
};

// Firestorm avatar_skeleton.xml and avatar_lad.xml bind transforms. HUD
// attachment points are intentionally excluded from world scene rendering.
struct RenderAvatarBindRig
{
    struct Joint
    {
        std::string name;
        int parent=-1;
        RenderVec3 local{};
        RenderVec3 bind_world{};
    };
    std::vector<Joint> joints;
    std::array<RenderVec3,64U> attachment_offsets{};
    std::array<RenderVec3,64U> attachment_local_offsets{};
    std::array<std::uint16_t,64U> attachment_joint_plus_one{};
    std::array<bool,64U> available{};
    std::array<bool,64U> is_hud{};
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
    std::uint64_t scene_hud_linkset_excluded = 0U;
    std::uint64_t scene_invalid_parent_excluded = 0U;
    std::uint32_t scene_max_visible_parent_depth=0U;
    std::uint64_t scene_deep_visible_linksets=0U;
    std::uint32_t scene_bone_pose_avatars=0U; // frame-local actual joint matrices
    // Actual per-frame glDrawElements from world-scene objects only.
    // Sky, terrain, water, UI and avatar proxies are not counted here.
    // Actual OpenGL shader branch, not simply assets resident.
    std::uint64_t terrain_textured_draw_calls = 0U;
    std::uint64_t terrain_fallback_draw_calls = 0U;
    std::uint32_t terrain_real_detail_layers = 0U;
    std::uint64_t scene_draw_calls = 0U;
    std::uint64_t scene_mesh_draw_calls = 0U;
    std::uint64_t scene_prim_draw_calls = 0U;
    // Actual OpenGL draw routing: opaque/depth-writing vs alpha-blended.
    std::uint64_t scene_opaque_draw_calls = 0U;
    std::uint64_t scene_alpha_draw_calls = 0U;
    std::uint64_t scene_alpha_mask_draw_calls = 0U;
    // Scene object requested a specific mesh UUID, but fallback primitive
    // geometry was drawn instead because the GPU mesh was not resident.
    std::uint64_t scene_missing_mesh_draw_calls = 0U;
    std::uint64_t scene_missing_mesh_objects = 0U;
    std::uint64_t scene_missing_mesh_triangles = 0U;
    // Actual GL proxy draw calls, separate from skinned avatar support
    // and separate from world-object scene mesh draw accounting.
    std::uint64_t avatar_proxy_draw_calls = 0U;
    std::uint64_t self_avatar_proxy_draw_calls = 0U;
    // Actual GPU admission counts for this frame, never inferred from
    // LLUDP object presence or from a previous simulator region.
    std::uint32_t avatar_pose_invalid_dropped=0U;
    std::uint32_t avatar_rotation_repaired=0U;
    bool self_avatar_pose_invalid_dropped=false;
    std::uint64_t avatar_baked_texture_draw_calls = 0U;
    std::uint64_t avatar_anatomical_mesh_draw_calls = 0U;
    std::uint64_t avatar_weighted_skin_draw_calls = 0U;
    std::uint64_t avatar_locally_animated_skin_draw_calls = 0U;
    std::uint64_t avatar_server_selected_skin_draw_calls = 0U;
    std::uint64_t avatar_keyframe_skin_draw_calls = 0U;
    std::uint64_t self_avatar_weighted_skin_draw_calls = 0U;
    // Counts only the real SELF anatomical 3-part GL skin, never remote avatar.
    std::uint64_t self_avatar_keyframe_skin_draw_calls=0U;
    std::uint64_t self_avatar_server_selected_skin_draw_calls=0U;
    std::uint64_t self_avatar_anatomical_mesh_draw_calls = 0U;
    std::uint64_t self_avatar_baked_texture_draw_calls = 0U;
    // GL_ANY_SAMPLES_PASSED on original self head/upper/lower, resolved
    // asynchronously from a preceding frame. Not a completed bake or
    // a post-UI pixel readback. Explicit validity avoids fake 0-pixel
    // claims when the GPU has not returned query results.
    bool self_avatar_visible_samples_known=false;
    bool self_avatar_visible_samples_passed=false;
    // Independent previous-frame GL_ANY_SAMPLES_PASSED results for the
    // original Head/Upper/Lower triangles. Never infer other parts
    // from one positive result or treat a query as a bake upload.
    std::array<bool,3U> self_avatar_part_samples_known{};
    std::array<bool,3U> self_avatar_part_samples_passed{};
    // Actual original three Firestorm .llm body GL submissions with a baked
    // server texture bound, Head/Upper/Lower. Proxy fallback excluded.
    std::array<std::uint64_t,3U> self_avatar_body_bake_gl_draws{};
    // Real Firestorm optional system-hair, never counted as a fourth
    // Head/Upper/Lower skin-proof draw.
    std::uint64_t avatar_hair_bake_gl_draws=0U;
    std::uint64_t self_avatar_hair_bake_gl_draws=0U;
    // Independent original Firestorm left/right eye GL submissions:
    // never counted as anatomical body, false skin or Hair proof.
    std::uint64_t avatar_eyes_bake_gl_draws=0U;
    std::uint64_t self_avatar_eyes_bake_gl_draws=0U;
    // Optional ORIGINAL Firestorm eyelashMesh; only draw with current
    // real GPU Head bake and never as a fourth anatomical skin proof.
    std::uint64_t avatar_eyelash_head_gl_draws=0U;
    std::uint64_t self_avatar_eyelash_head_gl_draws=0U;
    // Actual local original Firestorm head_alpha visibility texture
    // enabled on an admitted Wimpern GL draw, not final makeup COF.
    std::uint64_t self_avatar_eyelash_mask_gl_draws=0U;
    // Actual Texture UUID used by the original LLM slot in this frame.
    // A GPU draw with yesterday's bake must not prove today's Low158 TE.
    std::array<std::string,3U> self_avatar_body_drawn_bake_ids{};
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
    // Render-thread-only read-only GL handle for existing texture UUIDs.
    // Caller may use it for UI in THIS frame only; invalid after eviction.
    [[nodiscard]] virtual unsigned int avatar_bake_preview_handle(
        const std::string& server_bake_id) const = 0;
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
    // Invalidate prior GPU visibility results when the simulator region
    // epoch changes. Outstanding GL queries are discarded on completion.
    virtual void reset_self_avatar_visibility() = 0;
    virtual void set_avatar_bind_rig(
        const RenderAvatarBindRig& rig)=0;
    // Readable Firestorm Linden Binary Mesh 1.0 head/upper/lower bind pose.
    // Three real original UV meshes replace the 7-part geometry proxy once
    // all required files have passed validation and GPU upload.
    [[nodiscard]] virtual bool set_avatar_body_mesh(
        std::size_t slot,const RenderMeshGeometry& geometry)=0;
    // Locally installed Firestorm head_alpha.tga *visibility* mask
    // for the original eyelashMesh, NOT a server downloaded asset.
    // An absent/invalid image leaves the previous GPU mask intact.
    [[nodiscard]] virtual bool set_avatar_eyelash_visibility_mask(
        const RenderImageRgba8& mask)=0;
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
