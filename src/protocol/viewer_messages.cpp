/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/viewer_messages.hpp"

#include <array>
#include <bit>
#include <cstring>
#include <limits>

namespace
{
void append_le_u16(std::vector<std::uint8_t>& out, std::uint16_t value)
{
    out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
}

void append_le_u32(std::vector<std::uint8_t>& out, std::uint32_t value)
{
    out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
}

void append_le_u64(std::vector<std::uint8_t>& out, std::uint64_t value)
{
    for (unsigned int shift = 0; shift < 64U; shift += 8U)
    {
        out.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFFU));
    }
}

void append_f32(std::vector<std::uint8_t>& out, float value)
{
    append_le_u32(out, std::bit_cast<std::uint32_t>(value));
}

void append_vec3(std::vector<std::uint8_t>& out, const nexverse::protocol::Vec3& value)
{
    append_f32(out, value.x);
    append_f32(out, value.y);
    append_f32(out, value.z);
}

void append_quaternion(
    std::vector<std::uint8_t>& out,
    const nexverse::protocol::Quaternion& value)
{
    append_f32(out, value.x);
    append_f32(out, value.y);
    append_f32(out, value.z);
    append_f32(out, value.w);
}

bool append_uuid(std::vector<std::uint8_t>& out, std::string_view uuid)
{
    const auto bytes = nexverse::protocol::uuid_to_bytes(uuid);
    if (!bytes.has_value()) return false;
    out.insert(out.end(), bytes->begin(), bytes->end());
    return true;
}

void append_variable2(std::vector<std::uint8_t>& out, std::string_view value)
{
    const auto count = std::min<std::size_t>(value.size(), 65535U);
    append_le_u16(out, static_cast<std::uint16_t>(count));
    out.insert(out.end(), value.begin(), value.begin() + static_cast<std::ptrdiff_t>(count));
}

std::uint16_t read_le_u16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(bytes[offset]) |
        (static_cast<std::uint16_t>(bytes[offset + 1U]) << 8U));
}

std::uint32_t read_le_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return
        static_cast<std::uint32_t>(bytes[offset]) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 8U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 3U]) << 24U);
}

float read_f32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return std::bit_cast<float>(read_le_u32(bytes, offset));
}

std::string uuid_from_bytes(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    static constexpr char hex[] = "0123456789abcdef";
    if (offset + 16U > bytes.size()) return {};

    std::string out;
    out.reserve(36U);
    for (std::size_t i = 0; i < 16U; ++i)
    {
        if (i == 4U || i == 6U || i == 8U || i == 10U) out.push_back('-');
        const std::uint8_t byte = bytes[offset + i];
        out.push_back(hex[(byte >> 4U) & 0x0FU]);
        out.push_back(hex[byte & 0x0FU]);
    }
    return out;
}

bool read_variable1(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::string& value)
{
    if (offset >= bytes.size()) return false;
    const std::size_t count = bytes[offset++];
    if (offset + count > bytes.size()) return false;
    value.assign(
        reinterpret_cast<const char*>(bytes.data() + offset),
        count);
    offset += count;
    return true;
}

bool read_variable2(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::string& value)
{
    if (offset + 2U > bytes.size()) return false;
    const std::size_t count = read_le_u16(bytes, offset);
    offset += 2U;
    if (offset + count > bytes.size()) return false;
    value.assign(
        reinterpret_cast<const char*>(bytes.data() + offset),
        count);
    offset += count;
    return true;
}
}

namespace nexverse::protocol
{
std::vector<std::uint8_t> encode_agent_update(
    std::uint32_t sequence,
    const LoginSession& session,
    const AgentUpdateData& update)
{
    std::vector<std::uint8_t> payload;
    payload.reserve(137U);

    if (!append_uuid(payload, session.agent_id) ||
        !append_uuid(payload, session.session_id))
    {
        return {};
    }

    append_quaternion(payload, update.body_rotation);
    append_quaternion(payload, update.head_rotation);
    payload.push_back(update.state);
    append_vec3(payload, update.camera_center);
    append_vec3(payload, update.camera_at_axis);
    append_vec3(payload, update.camera_left_axis);
    append_vec3(payload, update.camera_up_axis);
    append_f32(payload, update.far_clip);
    append_le_u32(payload, update.control_flags);
    payload.push_back(update.flags);

    return encode_packet(0U, sequence, msg_agent_update, payload);
}

std::vector<std::uint8_t> encode_chat_from_viewer(
    std::uint32_t sequence,
    const LoginSession& session,
    const LocalChatMessage& chat)
{
    std::vector<std::uint8_t> payload;
    if (!append_uuid(payload, session.agent_id) ||
        !append_uuid(payload, session.session_id))
    {
        return {};
    }

    append_variable2(payload, chat.message);
    payload.push_back(static_cast<std::uint8_t>(chat.type));
    append_le_u32(payload, static_cast<std::uint32_t>(chat.channel));

    return encode_packet(packet_reliable, sequence, msg_chat_from_viewer, payload);
}

std::optional<IncomingLocalChat> decode_chat_from_simulator(
    std::span<const std::uint8_t> payload)
{
    IncomingLocalChat chat;
    std::size_t offset = 0U;

    if (!read_variable1(payload, offset, chat.from_name)) return std::nullopt;
    if (offset + 16U + 16U + 3U + 12U > payload.size()) return std::nullopt;

    chat.source_id = uuid_from_bytes(payload, offset);
    offset += 16U;
    chat.owner_id = uuid_from_bytes(payload, offset);
    offset += 16U;

    chat.source_type = payload[offset++];
    chat.chat_type = payload[offset++];
    chat.audible = static_cast<std::int8_t>(payload[offset++]);

    chat.position.x = read_f32(payload, offset);
    offset += 4U;
    chat.position.y = read_f32(payload, offset);
    offset += 4U;
    chat.position.z = read_f32(payload, offset);
    offset += 4U;

    if (!read_variable2(payload, offset, chat.message)) return std::nullopt;
    return chat;
}

std::vector<std::uint8_t> encode_teleport_location_request(
    std::uint32_t sequence,
    const LoginSession& session,
    const TeleportLocation& target)
{
    std::vector<std::uint8_t> payload;
    if (!append_uuid(payload, session.agent_id) ||
        !append_uuid(payload, session.session_id))
    {
        return {};
    }

    append_le_u64(payload, target.region_handle);
    append_vec3(payload, target.position);
    append_vec3(payload, target.look_at);

    return encode_packet(
        packet_reliable,
        sequence,
        msg_teleport_location_request,
        payload);
}

std::optional<TeleportProgressEvent> decode_teleport_progress(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 20U) return std::nullopt;

    TeleportProgressEvent event;
    std::size_t offset = 16U;
    event.flags = read_le_u32(payload, offset);
    offset += 4U;

    if (!read_variable1(payload, offset, event.message)) return std::nullopt;
    return event;
}

std::optional<TeleportFailureEvent> decode_teleport_failed(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 17U) return std::nullopt;

    TeleportFailureEvent event;
    std::size_t offset = 16U;
    if (!read_variable1(payload, offset, event.reason)) return std::nullopt;
    return event;
}
}
