/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/lludp.hpp"

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

struct ObjectUpdateEntry
{
    std::uint32_t local_id = 0U;
    std::string id;
    std::uint8_t pcode = 0U;
    std::uint32_t parent_id = 0U;
    Vec3 position;
    Quaternion rotation;

    [[nodiscard]] bool is_avatar() const noexcept
    {
        return pcode == pcode_avatar;
    }
};

struct ObjectUpdateMessage
{
    std::uint64_t region_handle = 0U;
    std::uint16_t time_dilation = 0U;
    std::vector<ObjectUpdateEntry> objects;
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

[[nodiscard]] std::optional<ImprovedTerseObjectUpdateMessage>
decode_improved_terse_object_update(
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
