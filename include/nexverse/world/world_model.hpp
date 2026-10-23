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
#include <deque>
#include <map>
#include <unordered_map>
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
    // A coarse map position is NOT proof that the simulator has provided
    // an avatar ObjectUpdate, mesh pose or a body to draw.
    bool simulator_object_update_seen=false;
    bool coarse_position_only=false;
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
    // A live circuit is not evidence of the self avatar's location.
    // Only an agent/session/region-matching decoded simulator movement
    // or ObjectUpdate may authorize an original Firestorm body to render.
    [[nodiscard]] bool accept_simulator_self_pose(
        std::string_view avatar_id,std::string_view session_id,
        std::uint64_t region_handle,Vec3 position,Quaternion rotation);
    // AgentMovementComplete with no usable horizontal LookAt confirms
    // a server POSITION only. It cannot prove a BodyRotation/High4 yaw.
    [[nodiscard]] bool accept_simulator_self_position(
        std::string_view avatar_id,std::string_view session_id,
        std::uint64_t region_handle,Vec3 position);
    [[nodiscard]] bool simulator_self_pose_confirmed() const noexcept
    { return self_pose_confirmed_; }
    [[nodiscard]] bool self_avatar_renderable() const noexcept
    { return scene_ready()&&self_pose_confirmed_; }
    // SceneReady/UseCircuitCode alone never authorizes a High4
    // AgentUpdate camera or WASD/flight commands from provisional origin.
    [[nodiscard]] bool self_avatar_controls_ready() const noexcept;
    void set_agent_profile(AgentProfileState profile);
    // Authenticated self appearance is not a second avatar entity.
    [[nodiscard]] bool apply_self_appearance(
        protocol::AvatarAppearanceMessage appearance);
    // Simulator may transmit Low158/Low382 before the UI thread observes
    // CircuitState::active. Keep ONE strictly validated readback per type,
    // bound to the current authenticated agent/session/region generation.
    // The packets are NOT authoritative until circuit readiness is true.
    [[nodiscard]] bool stage_early_self_appearance(
        protocol::AvatarAppearanceMessage appearance);
    [[nodiscard]] bool stage_early_self_wearables(
        protocol::AgentWearablesUpdateMessage update);
    struct EarlySelfReadbackResult
    {
        bool appearance=false;
        bool wearables=false;
    };
    [[nodiscard]] EarlySelfReadbackResult flush_early_self_readbacks();
    // Read-only F3 badge: receipt is NOT accepted appearance/COF/Skin.
    [[nodiscard]] std::array<bool,2U>
        early_self_readbacks_pending() const noexcept
    {
        return {early_self_appearance_.has_value(),
                early_self_wearables_.has_value()};
    }
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
    // Valid current simulator self TE withdrawals, not HTTP failures or
    // placeholder uploads. Per authenticated agent/session/region epoch.
    [[nodiscard]] std::uint32_t self_object_bake_withdrawals() const noexcept;
    // A newer authentic Low158 COF outfit invalidated provisional
    // ObjectUpdate TE references from the previous outfit revision.
    [[nodiscard]] std::uint32_t self_object_bake_outfit_invalidations()
        const noexcept;
    // Replayed self ObjectUpdate TE from the retired COF outfit is not
    // evidence that the previous baked skin has become current again.
    [[nodiscard]] std::uint32_t self_object_bake_stale_replays()
        const noexcept;
    [[nodiscard]] std::uint32_t self_object_bake_replay_guards()
        const noexcept;
    // Current simulator explicitly withdrew the own avatar LocalID.
    // Invalidate ONLY ObjectUpdate provisional bakes; genuine Low158
    // remains authoritative until a new simulator appearance arrives.
    [[nodiscard]] bool withdraw_self_object_on_kill(
        std::string_view avatar_id,std::uint64_t region_handle,
        std::uint32_t local_id);
    [[nodiscard]] std::uint32_t self_object_kills() const noexcept;
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
                                 Quaternion rotation,std::uint32_t local_id,
                                 bool from_avatar_object_update=false);
    // Read-only coarse map snapshot. Never evict a detailed avatar
    // just because the server omitted it from its bounded coarse list.
    [[nodiscard]] bool apply_coarse_avatar_locations(
        const protocol::CoarseLocationUpdate& update);
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
    // Simulator KillObject applies to avatar LocalIDs as well as prims.
    // Remove the killed root's attachment/linkset descendants in this
    // region; never leave detached ghost meshes for the rig to reuse.
    [[nodiscard]] bool kill_simulator_local_id(
        std::uint32_t local_id,bool count_kill_object=true);
    // An actual Full/Compressed remote avatar ObjectUpdate may reassign
    // an existing LocalID without an observed KillObject. Preserve only
    // currently verified UUID->LocalID bindings.
    [[nodiscard]] std::uint32_t remote_avatar_local_id_rebinds() const noexcept;
    // Identity collisions handled without a received KillObject; separate
    // from actual simulator kill packet/removal counters.
    [[nodiscard]] std::uint32_t prim_local_id_rebinds() const noexcept;
    // Same real Prim-UUID with a new simulator-local root ID (implicit
    // remap without KillObject). Readable from F7, never raw UUIDs.
    [[nodiscard]] std::uint32_t prim_uuid_relocations() const noexcept;
    [[nodiscard]] std::uint32_t invalid_scene_updates() const noexcept;
    // Counts only successful current-region KillObject removals; no IDs.
    [[nodiscard]] std::uint32_t remote_avatar_kills() const noexcept;
    [[nodiscard]] std::uint32_t removed_prim_kills() const noexcept;
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
    // Count late Lower COF-version Low158 snapshots for this identity
    // and region. Legacy OpenGenesisLINK appearance has no COF version.
    [[nodiscard]] std::uint32_t stale_self_cof_appearances() const noexcept;
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
    bool self_pose_confirmed_=false;
    AgentProfileState agent_profile_;
    struct EarlySelfSource
    {
        std::string agent_id;
        std::string session_id;
        std::uint64_t region_handle=0U;
        std::uint32_t region_generation=0U;
        // Same-region reconnects can target a different simulator socket.
        // Generation alone cannot tie an early packet to that circuit.
        std::string simulator_ip;
        std::uint16_t simulator_port=0U;
    };
    template<class Packet> struct EarlySelfPacket
    {
        Packet packet;
        EarlySelfSource source;
    };
    std::optional<EarlySelfPacket<protocol::AvatarAppearanceMessage>>
        early_self_appearance_;
    std::optional<EarlySelfPacket<protocol::AgentWearablesUpdateMessage>>
        early_self_wearables_;
    std::optional<protocol::AvatarAppearanceMessage> self_appearance_;
    std::uint32_t stale_self_cof_appearances_=0U;
    // Highest authenticated modern COF revision survives interleaved
    // legacy count=0 Low158 messages within this session and region.
    std::uint32_t self_cof_version_highwater_=0U;
    std::uint8_t self_cof_format_highwater_=0U;
    // AppearanceVersion is uint8. Interleaving different format versions
    // must not make an older COF revision from a previous format valid.
    // Each series is scoped to this authenticated agent + region.
    std::array<std::uint32_t,256U> self_cof_by_format_highwater_{};
    std::optional<protocol::AgentWearablesUpdateMessage> self_wearables_;
    std::optional<SelfAvatarObjectBakes> self_object_bakes_;
    // At most eight exact old LocalID + Head/Upper/Lower fingerprints,
    // scoped to the current agent/region. Never exposed to rendering.
    std::deque<SelfAvatarObjectBakes> retired_self_object_bakes_;
    std::uint32_t self_object_bake_stale_replays_=0U;
    std::uint32_t self_object_bake_withdrawals_=0U;
    std::uint32_t self_object_bake_outfit_invalidations_=0U;
    std::uint32_t self_object_kills_=0U;
    std::optional<protocol::AvatarAnimationMessage> self_animations_;
    // Animation may precede ObjectUpdate. Bounded pending messages do not
    // create renderable ghost avatars and expire at region transitions.
    std::map<std::string, protocol::AvatarAnimationMessage, std::less<>>
        pending_avatar_animations_;
    std::map<std::string, AvatarState, std::less<>> avatars_;
    std::map<std::uint32_t, ObjectState> objects_;
    // Secondary UUID index: do not scan every prim per ObjectUpdate.
    // Updated atomically with objects_ on insert, kill and region resets.
    std::unordered_map<std::string,std::uint32_t> prim_uuid_to_local_id_;
    std::uint32_t object_revision_ = 0U;
    std::uint32_t remote_avatar_kills_=0U;
    std::uint32_t removed_prim_kills_=0U;
    std::uint32_t remote_avatar_local_id_rebinds_=0U;
    std::uint32_t prim_local_id_rebinds_=0U;
    std::uint32_t prim_uuid_relocations_=0U;
    std::uint32_t invalid_scene_updates_=0U;
    TerrainState terrain_;
    ParcelState parcel_;
    EnvironmentState environment_;
};
}
