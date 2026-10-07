/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/viewer_messages.hpp"

#include <array>
#include <bit>
#include <cmath>
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

std::int16_t read_le_i16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return static_cast<std::int16_t>(read_le_u16(bytes, offset));
}

std::uint32_t read_le_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return
        static_cast<std::uint32_t>(bytes[offset]) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 8U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 3U]) << 24U);
}

std::uint64_t read_le_u64(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    std::uint64_t value = 0U;
    for (unsigned int shift = 0U; shift < 64U; shift += 8U)
    {
        value |= static_cast<std::uint64_t>(bytes[offset++]) << shift;
    }
    return value;
}

float unpack_u16_float(
    std::span<const std::uint8_t> bytes,
    std::size_t offset,
    float lower,
    float upper)
{
    const float normalized =
        static_cast<float>(read_le_u16(bytes, offset)) / 65535.0f;
    return lower + normalized * (upper - lower);
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

bool read_variable2_bytes(
    std::span<const std::uint8_t> bytes,
    std::size_t& offset,
    std::vector<std::uint8_t>& value)
{
    if (offset + 2U > bytes.size()) return false;
    const std::size_t count = read_le_u16(bytes, offset);
    offset += 2U;
    if (offset + count > bytes.size()) return false;

    value.assign(
        bytes.begin() + static_cast<std::ptrdiff_t>(offset),
        bytes.begin() + static_cast<std::ptrdiff_t>(offset + count));
    offset += count;
    return true;
}

bool skip_variable1(std::span<const std::uint8_t> bytes, std::size_t& offset)
{
    if (offset >= bytes.size()) return false;
    const std::size_t count = bytes[offset++];
    if (offset + count > bytes.size()) return false;
    offset += count;
    return true;
}

bool skip_variable2(std::span<const std::uint8_t> bytes, std::size_t& offset)
{
    if (offset + 2U > bytes.size()) return false;
    const std::size_t count = read_le_u16(bytes, offset);
    offset += 2U;
    if (offset + count > bytes.size()) return false;
    offset += count;
    return true;
}

bool decode_object_transform(
    std::span<const std::uint8_t> data,
    nexverse::protocol::Vec3& position,
    nexverse::protocol::Quaternion& rotation)
{
    std::size_t offset = 0U;
    if (data.size() == 76U || data.size() == 48U)
    {
        offset = 16U;
    }

    if (data.size() == 60U || data.size() == 76U)
    {
        position.x = read_f32(data, offset);
        position.y = read_f32(data, offset + 4U);
        position.z = read_f32(data, offset + 8U);
        offset += 36U;
        rotation.x = read_f32(data, offset);
        rotation.y = read_f32(data, offset + 4U);
        rotation.z = read_f32(data, offset + 8U);
        const float ww = std::max(
            0.0f,
            1.0f - rotation.x * rotation.x -
            rotation.y * rotation.y -
            rotation.z * rotation.z);
        rotation.w = std::sqrt(ww);
        return true;
    }

    if (data.size() == 32U || data.size() == 48U)
    {
        position.x = unpack_u16_float(data, offset, -128.0f, 384.0f);
        position.y = unpack_u16_float(data, offset + 2U, -128.0f, 384.0f);
        position.z = unpack_u16_float(data, offset + 4U, -256.0f, 768.0f);
        offset += 18U;
        rotation.x = unpack_u16_float(data, offset, -1.0f, 1.0f);
        rotation.y = unpack_u16_float(data, offset + 2U, -1.0f, 1.0f);
        rotation.z = unpack_u16_float(data, offset + 4U, -1.0f, 1.0f);
        rotation.w = unpack_u16_float(data, offset + 6U, -1.0f, 1.0f);
        return true;
    }

    if (data.size() == 16U)
    {
        auto byte_float = [](std::uint8_t value, float low, float high)
        {
            return low + (static_cast<float>(value) / 255.0f) * (high - low);
        };
        position.x = byte_float(data[0U], -256.0f, 256.0f);
        position.y = byte_float(data[1U], -256.0f, 256.0f);
        position.z = byte_float(data[2U], -256.0f, 256.0f);
        rotation.x = byte_float(data[9U], -1.0f, 1.0f);
        rotation.y = byte_float(data[10U], -1.0f, 1.0f);
        rotation.z = byte_float(data[11U], -1.0f, 1.0f);
        rotation.w = byte_float(data[12U], -1.0f, 1.0f);
        return true;
    }

    return false;
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

std::optional<LayerDataMessage> decode_layer_data(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 3U)
    {
        return std::nullopt;
    }

    LayerDataMessage message;
    message.type = payload[0U];

    const std::size_t data_size = read_le_u16(payload, 1U);
    if (data_size == 0U || data_size > lludp_max_payload)
    {
        return std::nullopt;
    }

    if (3U + data_size != payload.size())
    {
        return std::nullopt;
    }

    message.data.assign(
        payload.begin() + 3,
        payload.end());
    return message;
}

std::optional<CoarseLocationUpdate> decode_coarse_location_update(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 6U)
    {
        return std::nullopt;
    }

    std::size_t offset = 0U;
    const std::size_t location_count = payload[offset++];
    if (offset + location_count * 3U + 5U > payload.size())
    {
        return std::nullopt;
    }

    std::vector<Vec3> locations;
    locations.reserve(location_count);
    for (std::size_t index = 0U; index < location_count; ++index)
    {
        Vec3 position;
        position.x = static_cast<float>(payload[offset++]);
        position.y = static_cast<float>(payload[offset++]);
        position.z = static_cast<float>(payload[offset++]) * 4.0f;
        locations.push_back(position);
    }

    CoarseLocationUpdate update;
    update.you_index = read_le_i16(payload, offset);
    offset += 2U;
    update.prey_index = read_le_i16(payload, offset);
    offset += 2U;

    if (offset >= payload.size())
    {
        return std::nullopt;
    }

    const std::size_t agent_count = payload[offset++];
    if (agent_count != location_count ||
        offset + agent_count * 16U != payload.size())
    {
        return std::nullopt;
    }

    update.avatars.reserve(agent_count);
    for (std::size_t index = 0U; index < agent_count; ++index)
    {
        CoarseAvatarLocation avatar;
        avatar.agent_id = uuid_from_bytes(payload, offset);
        if (avatar.agent_id.empty())
        {
            return std::nullopt;
        }
        offset += 16U;
        avatar.position = locations[index];
        update.avatars.push_back(std::move(avatar));
    }

    return update;
}

std::optional<KillObjectMessage> decode_kill_object(
    std::span<const std::uint8_t> payload)
{
    if (payload.empty())
    {
        return std::nullopt;
    }

    const std::size_t count = payload[0U];
    if (count == 0U || 1U + count * 4U != payload.size())
    {
        return std::nullopt;
    }

    KillObjectMessage message;
    message.local_ids.reserve(count);
    std::size_t offset = 1U;
    for (std::size_t index = 0U; index < count; ++index)
    {
        message.local_ids.push_back(read_le_u32(payload, offset));
        offset += 4U;
    }
    return message;
}

std::optional<ObjectUpdateMessage> decode_object_update(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 11U)
    {
        return std::nullopt;
    }

    ObjectUpdateMessage message;
    std::size_t offset = 0U;
    message.region_handle = read_le_u64(payload, offset);
    offset += 8U;
    message.time_dilation = read_le_u16(payload, offset);
    offset += 2U;

    const std::size_t count = payload[offset++];
    message.objects.reserve(count);

    for (std::size_t index = 0U; index < count; ++index)
    {
        auto need = [&](std::size_t amount)
        {
            return offset + amount <= payload.size();
        };

        if (!need(4U + 1U + 16U + 4U + 3U + 12U))
        {
            return std::nullopt;
        }

        ObjectUpdateEntry object;
        object.local_id = read_le_u32(payload, offset);
        offset += 4U;
        offset += 1U; // state
        object.id = uuid_from_bytes(payload, offset);
        offset += 16U;
        offset += 4U; // CRC
        object.pcode = payload[offset++];
        offset += 2U; // material + click action

        object.scale.x = read_f32(payload, offset);
        object.scale.y = read_f32(payload, offset + 4U);
        object.scale.z = read_f32(payload, offset + 8U);
        offset += 12U;

        if (!std::isfinite(object.scale.x) ||
            !std::isfinite(object.scale.y) ||
            !std::isfinite(object.scale.z) ||
            object.scale.x <= 0.0f ||
            object.scale.y <= 0.0f ||
            object.scale.z <= 0.0f)
        {
            return std::nullopt;
        }

        if (offset >= payload.size())
        {
            return std::nullopt;
        }

        const std::size_t object_data_size = payload[offset++];
        if (!need(object_data_size))
        {
            return std::nullopt;
        }
        const auto object_data = payload.subspan(offset, object_data_size);
        offset += object_data_size;

        if (!decode_object_transform(
                object_data,
                object.position,
                object.rotation))
        {
            return std::nullopt;
        }

        if (!need(8U))
        {
            return std::nullopt;
        }
        object.parent_id = read_le_u32(payload, offset);
        offset += 4U;
        offset += 4U; // update flags

        if (!need(23U))
        {
            return std::nullopt;
        }

        object.shape.path_curve = payload[offset++];
        object.shape.profile_curve = payload[offset++];
        object.shape.path_begin = read_le_u16(payload, offset);
        offset += 2U;
        object.shape.path_end = read_le_u16(payload, offset);
        offset += 2U;
        object.shape.path_scale_x = payload[offset++];
        object.shape.path_scale_y = payload[offset++];
        object.shape.path_shear_x = payload[offset++];
        object.shape.path_shear_y = payload[offset++];
        object.shape.path_twist =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.path_twist_begin =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.path_radius_offset =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.path_taper_x =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.path_taper_y =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.path_revolutions = payload[offset++];
        object.shape.path_skew =
            static_cast<std::int8_t>(payload[offset++]);
        object.shape.profile_begin = read_le_u16(payload, offset);
        offset += 2U;
        object.shape.profile_end = read_le_u16(payload, offset);
        offset += 2U;
        object.shape.profile_hollow = read_le_u16(payload, offset);
        offset += 2U;

        if (!read_variable2_bytes(
                payload,
                offset,
                object.texture_entry))
        {
            return std::nullopt;
        }

        if (!object.texture_entry.empty())
        {
            object.default_texture =
                parse_texture_entry_default(
                    object.texture_entry);
        }

        if (!skip_variable1(payload, offset) ||
            !skip_variable2(payload, offset) ||
            !skip_variable2(payload, offset) ||
            !skip_variable1(payload, offset))
        {
            return std::nullopt;
        }

        if (!need(4U))
        {
            return std::nullopt;
        }
        offset += 4U; // text color

        if (!skip_variable1(payload, offset) ||
            !skip_variable1(payload, offset) ||
            !skip_variable1(payload, offset))
        {
            return std::nullopt;
        }

        if (!need(66U))
        {
            return std::nullopt;
        }
        offset += 66U;

        if (object.local_id == 0U || object.id.empty())
        {
            return std::nullopt;
        }

        message.objects.push_back(std::move(object));
    }

    if (offset != payload.size())
    {
        return std::nullopt;
    }

    return message;
}

std::optional<ObjectUpdateCompressedMessage>
decode_object_update_compressed(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 11U)
    {
        return std::nullopt;
    }

    ObjectUpdateCompressedMessage message;
    std::size_t offset = 0U;
    message.region_handle = read_le_u64(payload, offset);
    offset += 8U;
    message.time_dilation = read_le_u16(payload, offset);
    offset += 2U;

    const std::size_t count = payload[offset++];
    message.objects.reserve(count);

    for (std::size_t index = 0U; index < count; ++index)
    {
        if (offset + 6U > payload.size())
        {
            return std::nullopt;
        }

        offset += 4U; // update flags
        const std::size_t data_size = read_le_u16(payload, offset);
        offset += 2U;

        if (data_size < 64U || offset + data_size > payload.size())
        {
            return std::nullopt;
        }

        const auto data = payload.subspan(offset, data_size);
        offset += data_size;

        ObjectUpdateCompressedEntry object;
        object.id = uuid_from_bytes(data, 0U);
        object.local_id = read_le_u32(data, 16U);
        object.pcode = data[20U];
        object.position.x = read_f32(data, 40U);
        object.position.y = read_f32(data, 44U);
        object.position.z = read_f32(data, 48U);
        object.rotation.x = read_f32(data, 52U);
        object.rotation.y = read_f32(data, 56U);
        object.rotation.z = read_f32(data, 60U);
        const float ww = std::max(
            0.0f,
            1.0f - object.rotation.x * object.rotation.x -
            object.rotation.y * object.rotation.y -
            object.rotation.z * object.rotation.z);
        object.rotation.w = std::sqrt(ww);

        if (object.local_id == 0U || object.id.empty())
        {
            return std::nullopt;
        }

        message.objects.push_back(std::move(object));
    }

    if (offset != payload.size())
    {
        return std::nullopt;
    }

    return message;
}

std::optional<ObjectUpdateCachedMessage> decode_object_update_cached(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 11U)
    {
        return std::nullopt;
    }

    ObjectUpdateCachedMessage message;
    std::size_t offset = 0U;
    message.region_handle = read_le_u64(payload, offset);
    offset += 8U;
    message.time_dilation = read_le_u16(payload, offset);
    offset += 2U;

    const std::size_t count = payload[offset++];
    if (offset + count * 12U != payload.size())
    {
        return std::nullopt;
    }

    message.objects.reserve(count);
    for (std::size_t index = 0U; index < count; ++index)
    {
        ObjectUpdateCachedEntry object;
        object.local_id = read_le_u32(payload, offset);
        offset += 4U;
        object.crc = read_le_u32(payload, offset);
        offset += 4U;
        object.update_flags = read_le_u32(payload, offset);
        offset += 4U;

        if (object.local_id == 0U)
        {
            return std::nullopt;
        }

        message.objects.push_back(object);
    }

    return message;
}

std::optional<ImprovedTerseObjectUpdateMessage>
decode_improved_terse_object_update(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 11U)
    {
        return std::nullopt;
    }

    ImprovedTerseObjectUpdateMessage message;
    std::size_t offset = 0U;
    message.region_handle = read_le_u64(payload, offset);
    offset += 8U;
    message.time_dilation = read_le_u16(payload, offset);
    offset += 2U;

    const std::size_t count = payload[offset++];
    message.updates.reserve(count);

    for (std::size_t index = 0U; index < count; ++index)
    {
        if (offset >= payload.size())
        {
            return std::nullopt;
        }

        const std::size_t data_size = payload[offset++];
        if (data_size != 44U && data_size != 60U)
        {
            return std::nullopt;
        }
        if (offset + data_size + 2U > payload.size())
        {
            return std::nullopt;
        }

        const std::span<const std::uint8_t> data =
            payload.subspan(offset, data_size);
        offset += data_size;

        TerseObjectUpdate update;
        update.local_id = read_le_u32(data, 0U);
        update.avatar = data[5U] != 0U;

        if ((update.avatar && data_size != 60U) ||
            (!update.avatar && data_size != 44U))
        {
            return std::nullopt;
        }

        std::size_t data_offset = update.avatar ? 22U : 6U;
        update.position.x = read_f32(data, data_offset);
        update.position.y = read_f32(data, data_offset + 4U);
        update.position.z = read_f32(data, data_offset + 8U);
        data_offset += 12U;

        data_offset += 6U; // velocity
        data_offset += 6U; // acceleration

        update.rotation.x =
            unpack_u16_float(data, data_offset, -1.0f, 1.0f);
        update.rotation.y =
            unpack_u16_float(data, data_offset + 2U, -1.0f, 1.0f);
        update.rotation.z =
            unpack_u16_float(data, data_offset + 4U, -1.0f, 1.0f);
        update.rotation.w =
            unpack_u16_float(data, data_offset + 6U, -1.0f, 1.0f);

        const std::size_t texture_size = read_le_u16(payload, offset);
        offset += 2U;
        if (offset + texture_size > payload.size())
        {
            return std::nullopt;
        }
        offset += texture_size;

        message.updates.push_back(update);
    }

    if (offset != payload.size())
    {
        return std::nullopt;
    }

    return message;
}

std::optional<RegionHandshakeMessage> decode_region_handshake(
    std::span<const std::uint8_t> payload)
{
    std::size_t offset = 0U;
    auto need = [&](std::size_t amount)
    {
        return offset + amount <= payload.size();
    };

    if (!need(5U))
    {
        return std::nullopt;
    }

    RegionHandshakeMessage message;
    message.region_flags = read_le_u32(payload, offset);
    offset += 4U;
    message.sim_access = payload[offset++];

    if (!read_variable1(payload, offset, message.sim_name) ||
        !need(16U + 1U + 4U + 4U + 16U + 128U + 32U + 16U + 8U))
    {
        return std::nullopt;
    }

    message.sim_owner = uuid_from_bytes(payload, offset);
    if (message.sim_owner.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    message.estate_manager = payload[offset++] != 0U;
    message.water_height = read_f32(payload, offset);
    offset += 4U;
    message.billable_factor = read_f32(payload, offset);
    offset += 4U;

    if (!std::isfinite(message.water_height) ||
        !std::isfinite(message.billable_factor))
    {
        return std::nullopt;
    }

    offset += 16U;  // CacheID

    for (std::size_t index = 0U; index < 4U; ++index)
    {
        message.terrain_base[index] = uuid_from_bytes(payload, offset);
        if (message.terrain_base[index].empty())
        {
            return std::nullopt;
        }
        offset += 16U;
    }

    for (std::size_t index = 0U; index < 4U; ++index)
    {
        message.terrain_detail[index] = uuid_from_bytes(payload, offset);
        if (message.terrain_detail[index].empty())
        {
            return std::nullopt;
        }
        offset += 16U;
    }

    for (std::size_t index = 0U; index < 4U; ++index)
    {
        message.terrain_start_height[index] = read_f32(payload, offset);
        offset += 4U;
    }

    for (std::size_t index = 0U; index < 4U; ++index)
    {
        message.terrain_height_range[index] = read_f32(payload, offset);
        offset += 4U;
    }

    for (std::size_t index = 0U; index < 4U; ++index)
    {
        if (!std::isfinite(message.terrain_start_height[index]) ||
            !std::isfinite(message.terrain_height_range[index]))
        {
            return std::nullopt;
        }
    }

    message.region_id = uuid_from_bytes(payload, offset);
    if (message.region_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    offset += 8U; // CPUClassID + CPURatio

    std::string ignored;
    if (!read_variable1(payload, offset, ignored) ||
        !read_variable1(payload, offset, ignored) ||
        !read_variable1(payload, offset, ignored) ||
        offset >= payload.size())
    {
        return std::nullopt;
    }

    const std::size_t extended_count = payload[offset++];
    if (offset + extended_count * 16U != payload.size())
    {
        return std::nullopt;
    }

    for (std::size_t index = 0U; index < extended_count; ++index)
    {
        const std::uint64_t flags = read_le_u64(payload, offset);
        offset += 8U;
        const std::uint64_t protocols = read_le_u64(payload, offset);
        offset += 8U;

        if (index == 0U)
        {
            message.region_flags_extended = flags;
            message.region_protocols = protocols;
        }
    }

    return message;
}

std::optional<AgentMovementCompleteMessage>
decode_agent_movement_complete(
    std::span<const std::uint8_t> payload)
{
    constexpr std::size_t fixed_size =
        16U + 16U + 12U + 12U + 8U + 4U + 2U;
    if (payload.size() < fixed_size)
    {
        return std::nullopt;
    }

    AgentMovementCompleteMessage message;
    std::size_t offset = 0U;

    message.agent_id = uuid_from_bytes(payload, offset);
    if (message.agent_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    message.session_id = uuid_from_bytes(payload, offset);
    if (message.session_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    message.position.x = read_f32(payload, offset);
    message.position.y = read_f32(payload, offset + 4U);
    message.position.z = read_f32(payload, offset + 8U);
    offset += 12U;

    message.look_at.x = read_f32(payload, offset);
    message.look_at.y = read_f32(payload, offset + 4U);
    message.look_at.z = read_f32(payload, offset + 8U);
    offset += 12U;

    if (!std::isfinite(message.position.x) ||
        !std::isfinite(message.position.y) ||
        !std::isfinite(message.position.z) ||
        !std::isfinite(message.look_at.x) ||
        !std::isfinite(message.look_at.y) ||
        !std::isfinite(message.look_at.z))
    {
        return std::nullopt;
    }

    message.region_handle = read_le_u64(payload, offset);
    offset += 8U;
    message.timestamp = read_le_u32(payload, offset);
    offset += 4U;

    if (message.region_handle == 0U ||
        !read_variable2(payload, offset, message.channel_version) ||
        offset != payload.size())
    {
        return std::nullopt;
    }

    return message;
}

std::optional<ParcelPropertiesMessage> decode_parcel_properties(
    std::span<const std::uint8_t> payload)
{
    std::size_t offset = 0U;
    auto need = [&](std::size_t amount)
    {
        return offset + amount <= payload.size();
    };

    if (!need(4U + 4U + 1U + 12U + 4U + 16U + 1U + 16U + 24U + 2U))
    {
        return std::nullopt;
    }

    offset += 4U; // request result
    offset += 4U; // sequence id
    offset += 1U; // snap selection
    offset += 12U; // self/other/public count

    const std::int32_t local_id =
        static_cast<std::int32_t>(read_le_u32(payload, offset));
    offset += 4U;
    if (local_id <= 0)
    {
        return std::nullopt;
    }

    ParcelPropertiesMessage parcel;
    parcel.local_id = static_cast<std::uint32_t>(local_id);
    parcel.owner_id = uuid_from_bytes(payload, offset);
    if (parcel.owner_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    offset += 1U; // is group owned
    offset += 16U; // auction/claim/rent
    offset += 24U; // AABB min/max

    if (!skip_variable2(payload, offset) || !need(4U + 1U + 32U + 4U + 4U + 4U + 4U))
    {
        return std::nullopt;
    }

    parcel.area = static_cast<std::int32_t>(read_le_u32(payload, offset));
    offset += 4U;
    offset += 1U; // status
    offset += 32U; // prim counters
    offset += 4U; // prim bonus
    offset += 4U; // other clean time
    parcel.parcel_flags = read_le_u32(payload, offset);
    offset += 4U;
    parcel.sale_price =
        static_cast<std::int32_t>(read_le_u32(payload, offset));
    offset += 4U;

    if (!read_variable1(payload, offset, parcel.name) ||
        !read_variable1(payload, offset, parcel.description) ||
        !read_variable1(payload, offset, parcel.music_url) ||
        !read_variable1(payload, offset, parcel.media_url))
    {
        return std::nullopt;
    }

    if (!need(16U + 1U + 16U + 4U + 4U + 1U + 16U + 16U + 12U + 12U + 1U + 5U))
    {
        return std::nullopt;
    }

    offset += 16U; // media id
    offset += 1U; // media autoscale
    offset += 16U; // group id
    offset += 4U; // pass price
    offset += 4U; // pass hours
    offset += 1U; // category
    offset += 16U; // auth buyer
    offset += 16U; // snapshot
    offset += 12U; // user location
    offset += 12U; // user look at
    offset += 1U; // landing type
    offset += 5U; // region/age booleans

    if (offset != payload.size())
    {
        return std::nullopt;
    }

    return parcel;
}

std::optional<AgentDataUpdateMessage> decode_agent_data_update(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 16U + 1U + 1U + 1U + 16U + 8U + 1U)
    {
        return std::nullopt;
    }

    AgentDataUpdateMessage message;
    std::size_t offset = 0U;
    message.agent_id = uuid_from_bytes(payload, offset);
    if (message.agent_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    if (!read_variable1(payload, offset, message.first_name) ||
        !read_variable1(payload, offset, message.last_name) ||
        !read_variable1(payload, offset, message.group_title))
    {
        return std::nullopt;
    }

    message.active_group_id = uuid_from_bytes(payload, offset);
    if (message.active_group_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    if (offset + 8U > payload.size())
    {
        return std::nullopt;
    }
    message.group_powers = read_le_u64(payload, offset);
    offset += 8U;

    if (!read_variable1(payload, offset, message.group_name) ||
        offset != payload.size())
    {
        return std::nullopt;
    }

    return message;
}

std::optional<AvatarAppearanceMessage> decode_avatar_appearance(
    std::span<const std::uint8_t> payload)
{
    if (payload.size() < 16U + 1U + 2U + 1U + 1U)
    {
        return std::nullopt;
    }

    AvatarAppearanceMessage message;
    std::size_t offset = 0U;

    message.avatar_id = uuid_from_bytes(payload, offset);
    if (message.avatar_id.empty())
    {
        return std::nullopt;
    }
    offset += 16U;

    message.trial = payload[offset++] != 0U;

    if (offset + 2U > payload.size())
    {
        return std::nullopt;
    }

    const std::size_t texture_size = read_le_u16(payload, offset);
    offset += 2U;
    if (offset + texture_size > payload.size())
    {
        return std::nullopt;
    }

    message.texture_entry.assign(
        payload.begin() + static_cast<std::ptrdiff_t>(offset),
        payload.begin() + static_cast<std::ptrdiff_t>(offset + texture_size));
    offset += texture_size;

    if (offset >= payload.size())
    {
        return std::nullopt;
    }

    const std::size_t visual_count = payload[offset++];
    if (offset + visual_count > payload.size())
    {
        return std::nullopt;
    }

    message.visual_params.assign(
        payload.begin() + static_cast<std::ptrdiff_t>(offset),
        payload.begin() + static_cast<std::ptrdiff_t>(offset + visual_count));
    offset += visual_count;

    if (offset >= payload.size())
    {
        return std::nullopt;
    }

    const std::size_t appearance_count = payload[offset++];
    if (offset + appearance_count * 9U != payload.size())
    {
        return std::nullopt;
    }

    message.appearance_data.reserve(appearance_count);
    for (std::size_t index = 0U; index < appearance_count; ++index)
    {
        AvatarAppearanceData data;
        data.appearance_version = payload[offset++];
        data.cof_version = read_le_u32(payload, offset);
        offset += 4U;
        data.flags = read_le_u32(payload, offset);
        offset += 4U;
        message.appearance_data.push_back(data);
    }

    return message;
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
