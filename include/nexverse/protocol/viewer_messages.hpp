/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/texture_entry.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
inline constexpr MessageId msg_agent_update{MessageFrequency::high, 4U};
inline constexpr MessageId msg_teleport_location_request{MessageFrequency::low, 63U};
inline constexpr MessageId msg_teleport_local{MessageFrequency::low, 64U};
inline constexpr MessageId msg_teleport_progress{MessageFrequency::low, 66U};
inline constexpr MessageId msg_teleport_finish{MessageFrequency::low, 69U};
inline constexpr MessageId msg_teleport_start{MessageFrequency::low, 73U};
inline constexpr MessageId msg_teleport_failed{MessageFrequency::low, 74U};
inline constexpr MessageId msg_chat_from_viewer{MessageFrequency::low, 80U};
inline constexpr MessageId msg_chat_from_simulator{MessageFrequency::low, 139U};

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Quaternion
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};

enum class LocalChatType : std::uint8_t
{
    whisper = 0U,
    normal = 1U,
    shout = 2U,
    start_typing = 4U,
    stop_typing = 5U
};

enum AgentControlFlags : std::uint32_t
{
    control_at_pos = 1U << 0U,
    control_at_neg = 1U << 1U,
    control_left_pos = 1U << 2U,
    control_left_neg = 1U << 3U,
    control_up_pos = 1U << 4U,
    control_up_neg = 1U << 5U,
    control_pitch_pos = 1U << 6U,
    control_pitch_neg = 1U << 7U,
    control_yaw_pos = 1U << 8U,
    control_yaw_neg = 1U << 9U,
    control_fast_at = 1U << 10U,
    control_fast_left = 1U << 11U,
    control_fast_up = 1U << 12U,
    control_fly = 1U << 13U,
    control_stop = 1U << 14U,
    control_finish_anim = 1U << 15U,
    control_stand_up = 1U << 16U,
    control_sit_on_ground = 1U << 17U,
    control_mouse_look = 1U << 18U,
    control_nudge_at_pos = 1U << 19U,
    control_nudge_at_neg = 1U << 20U,
    control_nudge_left_pos = 1U << 21U,
    control_nudge_left_neg = 1U << 22U,
    control_nudge_up_pos = 1U << 23U,
    control_nudge_up_neg = 1U << 24U,
    control_turn_left = 1U << 25U,
    control_turn_right = 1U << 26U,
    control_away = 1U << 27U,
    control_lbutton_down = 1U << 28U,
    control_lbutton_up = 1U << 29U,
    control_mouselook_lbutton_down = 1U << 30U
};

struct AgentUpdateData
{
    Quaternion body_rotation;
    Quaternion head_rotation;
    std::uint8_t state = 0U;
    Vec3 camera_center;
    Vec3 camera_at_axis{1.0f, 0.0f, 0.0f};
    Vec3 camera_left_axis{0.0f, 1.0f, 0.0f};
    Vec3 camera_up_axis{0.0f, 0.0f, 1.0f};
    float far_clip = 128.0f;
    std::uint32_t control_flags = 0U;
    std::uint8_t flags = 0U;
};

struct LocalChatMessage
{
    std::string message;
    LocalChatType type = LocalChatType::normal;
    std::int32_t channel = 0;
};

inline constexpr std::uint8_t layer_type_land = 0x4CU;
inline constexpr std::uint8_t layer_type_land_extended = 0x4DU;

struct LayerDataMessage
{
    std::uint8_t type = 0U;
    std::vector<std::uint8_t> data;

    [[nodiscard]] bool is_terrain() const noexcept
    {
        return type == layer_type_land || type == layer_type_land_extended;
    }
};

struct CoarseAvatarLocation
{
    std::string agent_id;
    Vec3 position;
};

struct CoarseLocationUpdate
{
    std::int16_t you_index = -1;
    std::int16_t prey_index = -1;
    std::vector<CoarseAvatarLocation> avatars;
};

struct KillObjectMessage
{
    std::vector<std::uint32_t> local_ids;
};

inline constexpr std::uint8_t pcode_prim = 9U;
inline constexpr std::uint8_t pcode_avatar = 47U;

struct PrimitiveShapeData
{
    std::uint8_t path_curve = 0U;
    std::uint8_t profile_curve = 0U;
    std::uint16_t path_begin = 0U;
    std::uint16_t path_end = 0U;
    std::uint8_t path_scale_x = 100U;
    std::uint8_t path_scale_y = 100U;
    std::uint8_t path_shear_x = 0U;
    std::uint8_t path_shear_y = 0U;
    std::int8_t path_twist = 0;
    std::int8_t path_twist_begin = 0;
    std::int8_t path_radius_offset = 0;
    std::int8_t path_taper_x = 0;
    std::int8_t path_taper_y = 0;
    std::uint8_t path_revolutions = 0U;
    std::int8_t path_skew = 0;
    std::uint16_t profile_begin = 0U;
    std::uint16_t profile_end = 0U;
    std::uint16_t profile_hollow = 0U;

    [[nodiscard]] std::uint8_t profile_shape() const noexcept
    {
        return profile_curve & 0x0FU;
    }

    [[nodiscard]] std::uint8_t hollow_shape() const noexcept
    {
        return profile_curve & 0xF0U;
    }
};

// ExtraParams type 0x20: RGBA UNORM8 (alpha = intensity), then
// radius/cutoff/falloff as three little-endian float32 values.
struct PointLightData
{
    float red = 1.0f;
    float green = 1.0f;
    float blue = 1.0f;
    float intensity = 0.0f;
    float radius = 0.0f;
    float cutoff = 0.0f;
    float falloff = 0.0f;
};

[[nodiscard]] std::optional<PointLightData> parse_point_light_extra_params(
    std::span<const std::uint8_t> extra);

// OpenSim PSBlock legacy payload (86 bytes), or extended 94..98.
struct ParticleSystemData
{
    std::uint32_t crc = 0U;
    std::uint32_t flags = 0U;
    std::uint32_t particle_flags = 0U;
    std::uint8_t pattern = 0U;
    std::uint8_t burst_count = 0U;
    float source_max_age = 0.0f;
    float burst_rate = 0.0f;
    float burst_radius = 0.0f;
    float speed_min = 0.0f;
    float speed_max = 0.0f;
    float particle_max_age = 0.0f;
    Vec3 acceleration;
    std::string texture_id;
    std::string target_id;
    std::uint8_t blend_source = 7U; // OpenMetaverse SrcAlpha
    std::uint8_t blend_destination = 9U; // OneMinusSrcAlpha
    std::array<float, 4> start_color{1.0f,1.0f,1.0f,1.0f};
    std::array<float, 4> end_color{1.0f,1.0f,1.0f,0.0f};
    float start_scale = 0.2f;
    float end_scale = 0.2f;
    float start_glow = 0.0f;
    float end_glow = 0.0f;
};

// Malformed or unsupported PSBlocks are ignored without rejecting the prim.
[[nodiscard]] std::optional<ParticleSystemData> parse_particle_system(
    std::span<const std::uint8_t> bytes);

struct ObjectUpdateEntry
{
    std::uint32_t local_id = 0U;
    std::string id;
    std::uint8_t pcode = 0U;
    std::uint32_t parent_id = 0U;
    Vec3 position;
    Quaternion rotation;
    Vec3 scale{1.0f, 1.0f, 1.0f};
    PrimitiveShapeData shape;
    // Mesh/Sculpt identity from ObjectUpdate ExtraParams (type 0x30).
    std::string sculpt_asset_id;
    std::uint8_t sculpt_type = 0U;
    std::optional<PointLightData> point_light;
    std::optional<ParticleSystemData> particle_system;
    [[nodiscard]] bool is_mesh() const noexcept
    {
        return !sculpt_asset_id.empty() && (sculpt_type & 0x07U) == 0x05U;
    }
    std::vector<std::uint8_t> texture_entry;
    std::optional<TextureEntry> parsed_texture_entry;
    std::optional<TextureEntryDefaultFace> default_texture;

    [[nodiscard]] bool is_avatar() const noexcept
    {
        return pcode == pcode_avatar;
    }
};

struct SculptExtraParam
{
    std::string asset_id;
    std::uint8_t type = 0U;
    [[nodiscard]] bool is_mesh() const noexcept
    {
        return !asset_id.empty() && (type & 0x07U) == 0x05U;
    }
};

// Returns nullopt for no Sculpt block OR malformed ExtraParams.
// Caller retains the regular object even when optional extra data is absent.
[[nodiscard]] std::optional<SculptExtraParam> parse_sculpt_extra_params(
    std::span<const std::uint8_t> extra);

struct ObjectUpdateMessage
{
    std::uint64_t region_handle = 0U;
    std::uint16_t time_dilation = 0U;
    std::vector<ObjectUpdateEntry> objects;
};

struct ObjectUpdateCachedEntry
{
    std::uint32_t local_id = 0U;
    std::uint32_t crc = 0U;
    std::uint32_t update_flags = 0U;
};

struct ObjectUpdateCachedMessage
{
    std::uint64_t region_handle = 0U;
    std::uint16_t time_dilation = 0U;
    std::vector<ObjectUpdateCachedEntry> objects;
};

struct ObjectUpdateCompressedEntry
{
    std::uint32_t local_id = 0U;
    std::string id;
    std::uint8_t pcode = 0U;
    std::uint32_t parent_id = 0U;
    Vec3 position;
    Quaternion rotation;
    Vec3 scale{1.0f, 1.0f, 1.0f};
    PrimitiveShapeData shape;
    std::string sculpt_asset_id;
    std::uint8_t sculpt_type = 0U;
    std::optional<PointLightData> point_light;
    std::optional<ParticleSystemData> particle_system;
    std::vector<std::uint8_t> texture_entry;
    std::optional<TextureEntry> parsed_texture_entry;
    std::optional<TextureEntryDefaultFace> default_texture;
    bool has_complete_prim_data = false;
    [[nodiscard]] bool is_mesh() const noexcept
    {
        return !sculpt_asset_id.empty() &&
               (sculpt_type & 0x07U) == 0x05U;
    }
};

struct ObjectUpdateCompressedMessage
{
    std::uint64_t region_handle = 0U;
    std::uint16_t time_dilation = 0U;
    std::vector<ObjectUpdateCompressedEntry> objects;
};

struct TerseObjectUpdate
{
    std::uint32_t local_id = 0U;
    bool avatar = false;
    Vec3 position;
    Quaternion rotation;
};

struct ImprovedTerseObjectUpdateMessage
{
    std::uint64_t region_handle = 0U;
    std::uint16_t time_dilation = 0U;
    std::vector<TerseObjectUpdate> updates;
};

struct RegionHandshakeMessage
{
    std::uint32_t region_flags = 0U;
    std::uint8_t sim_access = 0U;
    std::string sim_name;
    std::string sim_owner;
    bool estate_manager = false;
    float water_height = 0.0f;
    float billable_factor = 0.0f;
    std::array<std::string, 4> terrain_base;
    std::array<std::string, 4> terrain_detail;
    std::array<float, 4> terrain_start_height{};
    std::array<float, 4> terrain_height_range{};
    std::string region_id;
    std::uint64_t region_flags_extended = 0U;
    std::uint64_t region_protocols = 0U;
};

struct AgentMovementCompleteMessage
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Vec3 look_at;
    std::uint64_t region_handle = 0U;
    std::uint32_t timestamp = 0U;
    std::string channel_version;
};

struct ParcelPropertiesMessage
{
    std::uint32_t local_id = 0U;
    std::string owner_id;
    std::int32_t area = 0;
    std::uint32_t parcel_flags = 0U;
    std::int32_t sale_price = 0;
    std::string name;
    std::string description;
    std::string music_url;
    std::string media_url;
};

struct AgentDataUpdateMessage
{
    std::string agent_id;
    std::string first_name;
    std::string last_name;
    std::string group_title;
    std::string active_group_id;
    std::uint64_t group_powers = 0U;
    std::string group_name;
};

struct AvatarAppearanceData
{
    std::uint8_t appearance_version = 0U;
    std::uint32_t cof_version = 0U;
    std::uint32_t flags = 0U;
};

struct AvatarAppearanceMessage
{
    std::string avatar_id;
    bool trial = false;
    std::vector<std::uint8_t> texture_entry;
    std::vector<std::uint8_t> visual_params;
    std::vector<AvatarAppearanceData> appearance_data;
};

struct IncomingLocalChat
{
    std::string from_name;
    std::string source_id;
    std::string owner_id;
    std::uint8_t source_type = 0U;
    std::uint8_t chat_type = 0U;
    std::int8_t audible = 0;
    Vec3 position;
    std::string message;
};

struct TeleportLocation
{
    std::uint64_t region_handle = 0U;
    Vec3 position{128.0f, 128.0f, 25.0f};
    Vec3 look_at{1.0f, 0.0f, 0.0f};
};

struct TeleportProgressEvent
{
    std::uint32_t flags = 0U;
    std::string message;
};

struct TeleportFailureEvent
{
    std::string reason;
};

[[nodiscard]] std::vector<std::uint8_t> encode_agent_update(
    std::uint32_t sequence,
    const LoginSession& session,
    const AgentUpdateData& update);

[[nodiscard]] std::vector<std::uint8_t> encode_chat_from_viewer(
    std::uint32_t sequence,
    const LoginSession& session,
    const LocalChatMessage& chat);

[[nodiscard]] std::optional<LayerDataMessage> decode_layer_data(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<CoarseLocationUpdate> decode_coarse_location_update(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<KillObjectMessage> decode_kill_object(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<ObjectUpdateMessage> decode_object_update(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<ObjectUpdateCompressedMessage>
decode_object_update_compressed(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<ObjectUpdateCachedMessage> decode_object_update_cached(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<ImprovedTerseObjectUpdateMessage>
decode_improved_terse_object_update(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<RegionHandshakeMessage> decode_region_handshake(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<AgentMovementCompleteMessage>
decode_agent_movement_complete(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<ParcelPropertiesMessage> decode_parcel_properties(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<AgentDataUpdateMessage> decode_agent_data_update(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<AvatarAppearanceMessage> decode_avatar_appearance(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<IncomingLocalChat> decode_chat_from_simulator(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::vector<std::uint8_t> encode_teleport_location_request(
    std::uint32_t sequence,
    const LoginSession& session,
    const TeleportLocation& target);

[[nodiscard]] std::optional<TeleportProgressEvent> decode_teleport_progress(
    std::span<const std::uint8_t> payload);

[[nodiscard]] std::optional<TeleportFailureEvent> decode_teleport_failed(
    std::span<const std::uint8_t> payload);
}
