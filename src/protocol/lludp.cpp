/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/lludp.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstring>

namespace
{
void append_be_u32(std::vector<std::uint8_t>& out, std::uint32_t value)
{
    out.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
}

void append_le_u32(std::vector<std::uint8_t>& out, std::uint32_t value)
{
    out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
    out.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
}

std::uint32_t read_be_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return
        (static_cast<std::uint32_t>(bytes[offset]) << 24U) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 8U) |
        static_cast<std::uint32_t>(bytes[offset + 3U]);
}

std::uint32_t read_le_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return
        static_cast<std::uint32_t>(bytes[offset]) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 8U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 3U]) << 24U);
}

std::optional<std::uint8_t> parse_hex_byte(std::string_view value)
{
    if (value.size() != 2U) return std::nullopt;
    unsigned int result = 0;
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result, 16);
    if (parsed.ec != std::errc{} || result > 0xFFU) return std::nullopt;
    return static_cast<std::uint8_t>(result);
}

std::vector<std::uint8_t> zero_decode(std::span<const std::uint8_t> bytes)
{
    std::vector<std::uint8_t> decoded;
    decoded.reserve(bytes.size() * 2U);

    for (std::size_t i = 0; i < bytes.size(); ++i)
    {
        if (bytes[i] != 0U)
        {
            decoded.push_back(bytes[i]);
            continue;
        }

        if (i + 1U >= bytes.size()) return {};
        const std::uint8_t count = bytes[++i];
        if (count == 0U) return {};
        decoded.insert(decoded.end(), count, 0U);
    }
    return decoded;
}

std::optional<std::pair<nexverse::protocol::MessageId, std::size_t>> decode_message_id(
    std::span<const std::uint8_t> bytes,
    std::size_t offset)
{
    if (offset >= bytes.size()) return std::nullopt;

    if (bytes[offset] != 0xFFU)
    {
        return std::pair{
            nexverse::protocol::MessageId{
                nexverse::protocol::MessageFrequency::high,
                bytes[offset]},
            1U};
    }

    if (offset + 1U >= bytes.size()) return std::nullopt;
    if (bytes[offset + 1U] != 0xFFU)
    {
        return std::pair{
            nexverse::protocol::MessageId{
                nexverse::protocol::MessageFrequency::medium,
                bytes[offset + 1U]},
            2U};
    }

    if (offset + 3U >= bytes.size()) return std::nullopt;
    const std::uint32_t full =
        0xFFFF0000U |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 8U) |
        static_cast<std::uint32_t>(bytes[offset + 3U]);

    if (bytes[offset + 2U] == 0xFFU)
    {
        return std::pair{
            nexverse::protocol::MessageId{
                nexverse::protocol::MessageFrequency::fixed,
                full},
            4U};
    }

    return std::pair{
        nexverse::protocol::MessageId{
            nexverse::protocol::MessageFrequency::low,
            full & 0xFFFFU},
        4U};
}
}

namespace nexverse::protocol
{
PacketThrottle::PacketThrottle(
    std::size_t bytes_per_second,
    std::size_t burst_bytes)
    : bytes_per_second_{std::max<std::size_t>(bytes_per_second, 1U)},
      burst_bytes_{std::max<std::size_t>(burst_bytes, 1U)},
      tokens_{static_cast<double>(std::max<std::size_t>(burst_bytes, 1U))},
      last_refill_{std::chrono::steady_clock::now()}
{
}

bool PacketThrottle::consume(
    std::size_t bytes,
    std::chrono::steady_clock::time_point now)
{
    const auto elapsed = std::chrono::duration<double>(now - last_refill_).count();
    if (elapsed > 0.0)
    {
        tokens_ = std::min(
            static_cast<double>(burst_bytes_),
            tokens_ + elapsed * static_cast<double>(bytes_per_second_));
        last_refill_ = now;
    }

    if (static_cast<double>(bytes) > tokens_)
    {
        return false;
    }

    tokens_ -= static_cast<double>(bytes);
    return true;
}

std::optional<MessageTemplateEntry> known_message_template(MessageId id)
{
    constexpr std::array<MessageTemplateEntry, 25> entries{
        MessageTemplateEntry{msg_start_ping_check, "StartPingCheck", false},
        MessageTemplateEntry{msg_complete_ping_check, "CompletePingCheck", false},
        MessageTemplateEntry{msg_layer_data, "LayerData", false},
        MessageTemplateEntry{msg_object_update, "ObjectUpdate", false},
        MessageTemplateEntry{msg_object_update_compressed, "ObjectUpdateCompressed", false},
        MessageTemplateEntry{msg_object_update_cached, "ObjectUpdateCached", false},
        MessageTemplateEntry{msg_improved_terse_object_update, "ImprovedTerseObjectUpdate", false},
        MessageTemplateEntry{msg_kill_object, "KillObject", false},
        MessageTemplateEntry{msg_parcel_properties, "ParcelProperties", false},
        MessageTemplateEntry{msg_coarse_location_update, "CoarseLocationUpdate", false},
        MessageTemplateEntry{msg_use_circuit_code, "UseCircuitCode", true},
        MessageTemplateEntry{msg_region_handshake, "RegionHandshake", true},
        MessageTemplateEntry{msg_region_handshake_reply, "RegionHandshakeReply", true},
        MessageTemplateEntry{msg_avatar_appearance, "AvatarAppearance", false},
        MessageTemplateEntry{msg_agent_is_now_wearing, "AgentIsNowWearing", true},
        MessageTemplateEntry{msg_agent_set_appearance, "AgentSetAppearance", true},
        MessageTemplateEntry{msg_agent_wearables_request, "AgentWearablesRequest", true},
        MessageTemplateEntry{msg_agent_wearables_update, "AgentWearablesUpdate", true},
        MessageTemplateEntry{msg_complete_agent_movement, "CompleteAgentMovement", true},
        MessageTemplateEntry{msg_agent_movement_complete, "AgentMovementComplete", false},
        MessageTemplateEntry{msg_agent_data_update, "AgentDataUpdate", false},
        MessageTemplateEntry{msg_remove_inventory_item, "RemoveInventoryItem", false},
        MessageTemplateEntry{msg_remove_inventory_folder, "RemoveInventoryFolder", false},
        MessageTemplateEntry{msg_remove_inventory_objects, "RemoveInventoryObjects", false},
        MessageTemplateEntry{msg_packet_ack, "PacketAck", false}
    };

    for (const auto& entry : entries)
    {
        if (entry.id == id) return entry;
    }
    return std::nullopt;
}

ReliableTracker::ReliableTracker(
    std::chrono::milliseconds resend_after,
    std::uint32_t max_resends)
    : resend_after_{resend_after},
      max_resends_{max_resends}
{
}

void ReliableTracker::track(std::uint32_t sequence, std::vector<std::uint8_t> bytes)
{
    ReliablePacket packet;
    packet.sequence = sequence;
    packet.bytes = std::move(bytes);
    packet.last_sent = std::chrono::steady_clock::now();
    pending_[sequence] = std::move(packet);
}

bool ReliableTracker::acknowledge(std::uint32_t sequence)
{
    return pending_.erase(sequence) > 0U;
}

bool ReliableTracker::contains(std::uint32_t sequence) const
{
    return pending_.contains(sequence);
}

std::size_t ReliableTracker::pending_count() const noexcept
{
    return pending_.size();
}

std::vector<ReliablePacket> ReliableTracker::due_for_resend(
    std::chrono::steady_clock::time_point now)
{
    std::vector<ReliablePacket> due;

    for (auto& [sequence, packet] : pending_)
    {
        (void)sequence;
        if (now - packet.last_sent < resend_after_) continue;

        if (packet.resend_count >= max_resends_)
        {
            exhausted_ = true;
            continue;
        }

        ++packet.resend_count;
        packet.last_sent = now;
        if (!packet.bytes.empty())
        {
            packet.bytes[0] = static_cast<std::uint8_t>(packet.bytes[0] | packet_resent);
        }
        due.push_back(packet);
    }
    return due;
}

bool ReliableTracker::exhausted() const noexcept
{
    return exhausted_;
}

std::optional<std::vector<std::uint8_t>> uuid_to_bytes(std::string_view uuid)
{
    std::array<std::size_t, 4> hyphens{8U, 13U, 18U, 23U};
    if (uuid.size() != 36U) return std::nullopt;
    for (const auto index : hyphens)
    {
        if (uuid[index] != '-') return std::nullopt;
    }

    std::string compact;
    compact.reserve(32U);
    for (const char ch : uuid)
    {
        if (ch != '-') compact.push_back(ch);
    }
    if (compact.size() != 32U) return std::nullopt;

    std::vector<std::uint8_t> bytes;
    bytes.reserve(16U);
    for (std::size_t i = 0; i < compact.size(); i += 2U)
    {
        const auto parsed = parse_hex_byte(std::string_view{compact}.substr(i, 2U));
        if (!parsed.has_value()) return std::nullopt;
        bytes.push_back(*parsed);
    }
    return bytes;
}

std::vector<std::uint8_t> encode_message_id(MessageId id)
{
    switch (id.frequency)
    {
        case MessageFrequency::high:
            return {static_cast<std::uint8_t>(id.value & 0xFFU)};
        case MessageFrequency::medium:
            return {
                0xFFU,
                static_cast<std::uint8_t>(id.value & 0xFFU)};
        case MessageFrequency::low:
            return {
                0xFFU,
                0xFFU,
                static_cast<std::uint8_t>((id.value >> 8U) & 0xFFU),
                static_cast<std::uint8_t>(id.value & 0xFFU)};
        case MessageFrequency::fixed:
            return {
                static_cast<std::uint8_t>((id.value >> 24U) & 0xFFU),
                static_cast<std::uint8_t>((id.value >> 16U) & 0xFFU),
                static_cast<std::uint8_t>((id.value >> 8U) & 0xFFU),
                static_cast<std::uint8_t>(id.value & 0xFFU)};
    }
    return {};
}

std::vector<std::uint8_t> encode_packet(
    std::uint8_t flags,
    std::uint32_t sequence,
    MessageId message,
    std::span<const std::uint8_t> payload,
    std::span<const std::uint32_t> appended_acks)
{
    if (!appended_acks.empty())
    {
        flags = static_cast<std::uint8_t>(flags | packet_appended_acks);
    }

    std::vector<std::uint8_t> bytes;
    bytes.reserve(6U + 4U + payload.size() + appended_acks.size() * 4U + 1U);
    bytes.push_back(flags);
    append_be_u32(bytes, sequence);
    bytes.push_back(0U);

    const auto message_id = encode_message_id(message);
    bytes.insert(bytes.end(), message_id.begin(), message_id.end());
    bytes.insert(bytes.end(), payload.begin(), payload.end());

    for (const auto ack : appended_acks)
    {
        append_le_u32(bytes, ack);
    }
    if (!appended_acks.empty())
    {
        bytes.push_back(static_cast<std::uint8_t>(
            std::min<std::size_t>(appended_acks.size(), 255U)));
    }

    return bytes;
}

DecodeResult decode_lludp_packet(std::span<const std::uint8_t> input)
{
    if (input.size() < 7U)
    {
        return {std::nullopt, "LLUDP packet is shorter than minimum header"};
    }
    if (input.size() > lludp_mtu)
    {
        return {std::nullopt, "LLUDP packet exceeds MTU"};
    }

    std::vector<std::uint8_t> working{input.begin(), input.end()};
    const std::uint8_t original_flags = working[0];

    if ((original_flags & packet_zerocoded) != 0U)
    {
        if (working.size() < 6U)
        {
            return {std::nullopt, "Invalid zerocoded LLUDP header"};
        }

        std::vector<std::uint8_t> decoded;
        decoded.insert(decoded.end(), working.begin(), working.begin() + 6);
        const auto body = zero_decode(std::span<const std::uint8_t>{working}.subspan(6U));
        if (body.empty() && working.size() > 6U)
        {
            return {std::nullopt, "Invalid LLUDP zero encoding"};
        }
        decoded.insert(decoded.end(), body.begin(), body.end());
        working = std::move(decoded);
    }

    std::span<const std::uint8_t> bytes{working};
    const std::uint8_t flags = bytes[0];
    const std::uint32_t sequence = read_be_u32(bytes, 1U);
    const std::uint8_t extra = bytes[5];

    const std::size_t message_offset = 6U + static_cast<std::size_t>(extra);
    if (message_offset >= bytes.size())
    {
        return {std::nullopt, "LLUDP extra header exceeds packet size"};
    }

    const auto id = decode_message_id(bytes, message_offset);
    if (!id.has_value())
    {
        return {std::nullopt, "LLUDP message ID is truncated"};
    }

    std::size_t payload_begin = message_offset + id->second;
    std::size_t payload_end = bytes.size();
    std::vector<std::uint32_t> appended;

    if ((flags & packet_appended_acks) != 0U)
    {
        if (payload_end <= payload_begin)
        {
            return {std::nullopt, "LLUDP ACK flag set without ACK count"};
        }

        const std::size_t ack_count = bytes[payload_end - 1U];
        const std::size_t ack_bytes = ack_count * 4U;
        if (payload_end < payload_begin + 1U + ack_bytes)
        {
            return {std::nullopt, "LLUDP appended ACK section is truncated"};
        }

        const std::size_t ack_start = payload_end - 1U - ack_bytes;
        for (std::size_t i = 0; i < ack_count; ++i)
        {
            appended.push_back(read_le_u32(bytes, ack_start + i * 4U));
        }
        payload_end = ack_start;
    }

    DecodedPacket packet;
    packet.header.flags = flags;
    packet.header.sequence = sequence;
    packet.header.extra_header_length = extra;
    packet.header.message = id->first;
    packet.payload.assign(bytes.begin() + static_cast<std::ptrdiff_t>(payload_begin),
                          bytes.begin() + static_cast<std::ptrdiff_t>(payload_end));
    packet.appended_acks = std::move(appended);
    packet.raw = std::move(working);
    return {std::move(packet), {}};
}

std::vector<std::uint8_t> encode_packet_ack(
    std::uint32_t sequence,
    std::span<const std::uint32_t> acknowledged_sequences)
{
    const std::size_t count = std::min<std::size_t>(acknowledged_sequences.size(), 255U);
    std::vector<std::uint8_t> payload;
    payload.reserve(1U + count * 4U);
    payload.push_back(static_cast<std::uint8_t>(count));
    for (std::size_t i = 0; i < count; ++i)
    {
        append_le_u32(payload, acknowledged_sequences[i]);
    }
    return encode_packet(0U, sequence, msg_packet_ack, payload);
}

std::vector<std::uint8_t> encode_use_circuit_code(
    std::uint32_t sequence,
    const LoginSession& session)
{
    const auto session_id = uuid_to_bytes(session.session_id);
    const auto agent_id = uuid_to_bytes(session.agent_id);
    if (!session_id.has_value() || !agent_id.has_value()) return {};

    std::vector<std::uint8_t> payload;
    payload.reserve(36U);
    append_le_u32(payload, session.circuit_code);
    payload.insert(payload.end(), session_id->begin(), session_id->end());
    payload.insert(payload.end(), agent_id->begin(), agent_id->end());

    return encode_packet(packet_reliable, sequence, msg_use_circuit_code, payload);
}

std::vector<std::uint8_t> encode_complete_agent_movement(
    std::uint32_t sequence,
    const LoginSession& session)
{
    const auto agent_id = uuid_to_bytes(session.agent_id);
    const auto session_id = uuid_to_bytes(session.session_id);
    if (!agent_id.has_value() || !session_id.has_value()) return {};

    std::vector<std::uint8_t> payload;
    payload.reserve(36U);
    payload.insert(payload.end(), agent_id->begin(), agent_id->end());
    payload.insert(payload.end(), session_id->begin(), session_id->end());
    append_le_u32(payload, session.circuit_code);

    return encode_packet(packet_reliable, sequence, msg_complete_agent_movement, payload);
}

std::vector<std::uint8_t> encode_region_handshake_reply(
    std::uint32_t sequence,
    const LoginSession& session,
    std::uint32_t flags)
{
    const auto agent_id = uuid_to_bytes(session.agent_id);
    const auto session_id = uuid_to_bytes(session.session_id);
    if (!agent_id.has_value() || !session_id.has_value()) return {};

    std::vector<std::uint8_t> payload;
    payload.reserve(36U);
    payload.insert(payload.end(), agent_id->begin(), agent_id->end());
    payload.insert(payload.end(), session_id->begin(), session_id->end());
    append_le_u32(payload, flags);

    return encode_packet(packet_reliable, sequence, msg_region_handshake_reply, payload);
}

std::vector<std::uint8_t> encode_complete_ping_check(
    std::uint32_t sequence,
    std::uint8_t ping_id)
{
    const std::array<std::uint8_t, 1> payload{ping_id};
    return encode_packet(0U, sequence, msg_complete_ping_check, payload);
}
}
