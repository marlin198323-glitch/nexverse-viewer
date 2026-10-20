/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/terrain_codec.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/avatar_animation.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::world
{
using nexverse::protocol::Quaternion;
using nexverse::protocol::Vec3;

struct GridState
{
    std::string name;
    std::string login_uri;
};

struct RegionState
{
    std::uint64_t handle = 0U;
    std::string name;
    Vec3 agent_position;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0U;
    std::string seed_capability;
    std::string region_id;
    std::uint32_t region_flags = 0U;
    std::uint64_t region_flags_extended = 0U;
    std::uint64_t region_protocols = 0U;
    std::uint8_t sim_access = 0U;
    float water_height = 0.0f;
    std::string channel_version;
    std::uint32_t size_x = 256U;
    std::uint32_t size_y = 256U;
    std::uint32_t generation = 0U;
    bool connected = false;
};

struct AgentState
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Quaternion rotation;
    bool present = false;
};

struct AgentProfileState
{
    std::string first_name;
    std::string last_name;
    std::string group_title;
    std::string active_group_id;
    std::uint64_t group_powers = 0U;
    std::string group_name;
};

struct AvatarState
{
    std::string id;
    std::string name;
    Vec3 position;
    Quaternion rotation;
    std::uint32_t local_id = 0U;
    bool position_known = false;
    // Motion is server-position-derived; used only for visual animation.
    std::chrono::steady_clock::time_point last_translation_at{};
    bool trial_appearance = false;
    std::vector<std::uint8_t> texture_entry;
    std::vector<std::uint8_t> visual_params;
    std::vector<protocol::AvatarAppearanceData> appearance_data;
    std::vector<Vec3> hover_heights;
    std::vector<protocol::AvatarAttachmentData> attachments;
    std::optional<protocol::AvatarAnimationMessage> animations;
};

struct ObjectState
{
    std::uint32_t local_id = 0U;
    std::string id;
    std::uint8_t pcode = 0U;
    std::uint8_t state=0U;
    std::uint32_t parent_id = 0U;
    Vec3 position;
    Quaternion rotation;
    Vec3 scale{1.0f, 1.0f, 1.0f};
    protocol::PrimitiveShapeData shape;
    std::string sculpt_asset_id;
    std::uint8_t sculpt_type = 0U;
    std::optional<protocol::PointLightData> point_light;
    std::optional<protocol::ParticleSystemData> particle_system;
    [[nodiscard]] bool is_mesh() const noexcept
    {
        return !sculpt_asset_id.empty() && (sculpt_type & 0x07U) == 0x05U;
    }
    std::vector<std::uint8_t> texture_entry;
    std::optional<protocol::TextureEntry> parsed_texture_entry;
    std::optional<protocol::TextureEntryDefaultFace> default_texture;
};

struct TerrainMaterials
{
    std::uint32_t revision = 0U;
    bool available = false;
    std::array<std::string, 4> base_ids;
    std::array<std::string, 4> detail_ids;
    std::array<float, 4> start_heights{};
    std::array<float, 4> height_ranges{};
};

struct TerrainState
{
    std::uint32_t revision = 0U;
    bool available = false;
    std::array<float, 256U * 256U> heights{};
    std::array<bool, 16U * 16U> valid_patches{};
    std::uint16_t valid_patch_count = 0U;
    TerrainMaterials materials;
};

struct ParcelState
{
    std::uint32_t local_id = 0U;
    std::string name;
    std::string description;
    std::string owner_id;
    std::int32_t area = 0;
    std::uint32_t parcel_flags = 0U;
    std::int32_t sale_price = 0;
    std::string music_url;
    std::string media_url;
    bool available = false;
};

struct EnvironmentState
{
    std::uint32_t revision = 0U;
    bool available = false;
    Vec3 ambient{0.35f, 0.35f, 0.35f};
    Vec3 blue_density{0.12f, 0.22f, 0.38f};
    Vec3 blue_horizon{0.25f, 0.25f, 0.32f};
    Vec3 sunlight_color{0.72f, 0.76f, 0.90f};
    Vec3 cloud_color{0.41f, 0.41f, 0.41f};
    Vec3 water_fog_color{0.0156f, 0.149f, 0.2509f};
    float haze_density = 0.70f;
    float haze_horizon = 0.19f;
    float density_multiplier = 0.00018f;
    float distance_multiplier = 0.8f;
    float gamma = 1.0f;
    float star_brightness = 0.0f;
    float sun_angle = 0.317f;
    float east_angle = 0.0f;
    float cloud_shadow = 0.27f;
    float water_fog_density = 10.0f;
    std::string sky_name;
    std::string water_normal_map;
};

struct RegionUpdate
{
    std::uint64_t handle = 0U;
    std::string name;
    Vec3 agent_position;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0U;
    std::string seed_capability;
    std::string region_id;
    std::uint32_t region_flags = 0U;
    std::uint64_t region_flags_extended = 0U;
    std::uint64_t region_protocols = 0U;
    std::uint8_t sim_access = 0U;
    float water_height = 0.0f;
    std::string channel_version;
    std::uint32_t size_x = 256U;
    std::uint32_t size_y = 256U;
    bool connected = false;
};

struct AgentUpdate
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Quaternion rotation;
    bool present = false;
};

// A current simulator ObjectUpdate for the authenticated self agent can
// carry bona fide baked TE faces even before a Low158 AvatarAppearance.
// Strictly provisional, never an authoritative body/COF appearance.
struct SelfAvatarObjectBakes
{
    std::array<std::string,3U> visible_body;
    std::uint32_t local_id=0U;
};
// Per-face precedence: a real Low158 face is NEVER replaced. A still-current
// self ObjectUpdate face can be shown provisionally only where Low158 lacks
// an asset ID. These flags are not proof of a worn COF or visible GPU pixels.
struct SelfRenderBakeSelection
{
    std::array<std::string,3U> ids{};
    std::array<bool,3U> from_low158{};
    std::array<bool,3U> from_object_update{};
};

struct WorldSnapshot
{
    GridState grid;
    RegionState region;
    AgentState agent;
    AgentProfileState agent_profile;
    std::optional<protocol::AvatarAppearanceMessage> self_appearance;
    std::optional<protocol::AgentWearablesUpdateMessage> self_wearables;
    std::optional<SelfAvatarObjectBakes> self_object_bakes;
    std::optional<protocol::AvatarAnimationMessage> self_animations;
    std::map<std::string, AvatarState, std::less<>> avatars;
    std::map<std::uint32_t, ObjectState> objects;
    TerrainState terrain;
    ParcelState parcel;
    EnvironmentState environment;
    bool ready = false;
};

class WorldModel
{
public:
    void reset();

    void set_grid(GridState grid);
    void apply_region(const RegionUpdate& update);
    void begin_region_transition(const RegionUpdate& update);
    void apply_agent(const AgentUpdate& update);
    void set_agent_profile(AgentProfileState profile);
    // Authenticated self appearance is not a second avatar entity.
    [[nodiscard]] bool apply_self_appearance(
        protocol::AvatarAppearanceMessage appearance);
    // Read-only legacy wearable references (no wear/set-appearance writes).
    [[nodiscard]] bool apply_self_wearables(
        protocol::AgentWearablesUpdateMessage update);
    [[nodiscard]] const std::optional<protocol::AgentWearablesUpdateMessage>&
        self_wearables() const noexcept;
    // Low158 remains authoritative; this may only rescue genuine TE bake
    // face IDs from the region-matched self ObjectUpdate visual path.
    [[nodiscard]] bool observe_self_object_bakes(
        std::string_view avatar_id,std::uint64_t region_handle,
        std::uint32_t local_id,std::span<const std::uint8_t> texture_entry);
    [[nodiscard]] const std::optional<SelfAvatarObjectBakes>&
        self_object_bakes() const noexcept;
    // Low158 always wins PER FACE; valid self ObjectUpdate may fill only
    // missing body faces with explicitly provisional server references.
    [[nodiscard]] SelfRenderBakeSelection self_bake_selection() const;
    [[nodiscard]] std::array<std::string,3U>
        self_render_bakes() const;
    // High20 AvatarAnimation is an authoritative complete per-avatar
    // animation snapshot, not a requested viewer-side Wear/Play command.
    [[nodiscard]] bool apply_avatar_animation(
        protocol::AvatarAnimationMessage animation);
    [[nodiscard]] const std::optional<protocol::AvatarAnimationMessage>&
        self_animations() const noexcept;
    void upsert_avatar(AvatarState avatar);
    // Pose-only packets must not erase AppearanceData, COF versions,
    // baked texture entries or attachment blocks already received.
    void update_avatar_transform(const std::string& id,Vec3 position,
                                 Quaternion rotation,std::uint32_t local_id);
    void remove_avatar(const std::string& id);
    void upsert_object(ObjectState object);
    // A High9 ImprovedTerseObjectUpdate has ONLY pose, never an object
    // identity or a complete TextureEntry. Do not create phantom prims or
    // erase preexisting shape, attachment point, materials and meshes.
    [[nodiscard]] bool apply_object_terse(
        std::uint32_t local_id,Vec3 position,Quaternion rotation);
    // Compressed ObjectData may have only the first 64 bytes. Such a packet
    // MUST match an existing prim by BOTH LocalID and UUID before it can
    // update State/pose; missing mesh/TE/ParentID are never fabricated.
    [[nodiscard]] bool apply_object_compressed_partial(
        const protocol::ObjectUpdateCompressedEntry& partial);
    void remove_object(std::uint32_t local_id);
    void set_terrain(TerrainState terrain);
    void set_terrain_materials(TerrainMaterials materials);
    void apply_terrain_patch(const protocol::TerrainPatch& patch);
    void set_parcel(ParcelState parcel);
    void set_environment(EnvironmentState environment);

    [[nodiscard]] const GridState& grid() const noexcept;
    [[nodiscard]] const RegionState& region() const noexcept;
    [[nodiscard]] const AgentState& agent() const noexcept;
    [[nodiscard]] const AgentProfileState& agent_profile() const noexcept;
    [[nodiscard]] const std::optional<protocol::AvatarAppearanceMessage>&
        self_appearance() const noexcept;
    [[nodiscard]] const TerrainState& terrain() const noexcept;
    [[nodiscard]] const ParcelState& parcel() const noexcept;
    [[nodiscard]] const EnvironmentState& environment() const noexcept;
    [[nodiscard]] const std::map<std::string, AvatarState, std::less<>>& avatars() const noexcept;
    [[nodiscard]] const std::map<std::uint32_t, ObjectState>& objects() const noexcept;
    [[nodiscard]] std::uint32_t object_revision() const noexcept;
    [[nodiscard]] WorldSnapshot snapshot() const;

    // Simulator scene may become interactive before terrain LayerData
    // arrives. WorldModel::ready() is the stricter terrain-ready contract.
    [[nodiscard]] bool scene_ready() const noexcept;
    [[nodiscard]] bool ready() const noexcept;

private:
    GridState grid_;
    RegionState region_;
    AgentState agent_;
    AgentProfileState agent_profile_;
    std::optional<protocol::AvatarAppearanceMessage> self_appearance_;
    std::optional<protocol::AgentWearablesUpdateMessage> self_wearables_;
    std::optional<SelfAvatarObjectBakes> self_object_bakes_;
    std::optional<protocol::AvatarAnimationMessage> self_animations_;
    // Animation may precede ObjectUpdate. Bounded pending messages do not
    // create renderable ghost avatars and expire at region transitions.
    std::map<std::string, protocol::AvatarAnimationMessage, std::less<>>
        pending_avatar_animations_;
    std::map<std::string, AvatarState, std::less<>> avatars_;
    std::map<std::uint32_t, ObjectState> objects_;
    std::uint32_t object_revision_ = 0U;
    TerrainState terrain_;
    ParcelState parcel_;
    EnvironmentState environment_;
};
}
