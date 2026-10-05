/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/login.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
inline constexpr std::size_t lludp_mtu = 1400U;
inline constexpr std::size_t lludp_max_payload = 1200U;

enum class MessageFrequency
{
    high,
    medium,
    low,
    fixed
};

struct MessageId
{
    MessageFrequency frequency = MessageFrequency::high;
    std::uint32_t value = 0;

    bool operator==(const MessageId&) const = default;
};

struct MessageTemplateEntry
{
    MessageId id;
    std::string_view name;
    bool reliable = false;
};

inline constexpr MessageId msg_start_ping_check{MessageFrequency::high, 1U};
inline constexpr MessageId msg_complete_ping_check{MessageFrequency::high, 2U};
inline constexpr MessageId msg_layer_data{MessageFrequency::high, 11U};
inline constexpr MessageId msg_object_update{MessageFrequency::high, 12U};
inline constexpr MessageId msg_object_update_compressed{MessageFrequency::high, 13U};
inline constexpr MessageId msg_object_update_cached{MessageFrequency::high, 14U};
inline constexpr MessageId msg_improved_terse_object_update{MessageFrequency::high, 15U};
inline constexpr MessageId msg_kill_object{MessageFrequency::high, 16U};
inline constexpr MessageId msg_parcel_properties{MessageFrequency::high, 23U};
inline constexpr MessageId msg_coarse_location_update{MessageFrequency::medium, 6U};
inline constexpr MessageId msg_use_circuit_code{MessageFrequency::low, 3U};
inline constexpr MessageId msg_region_handshake{MessageFrequency::low, 148U};
inline constexpr MessageId msg_region_handshake_reply{MessageFrequency::low, 149U};
inline constexpr MessageId msg_avatar_appearance{MessageFrequency::low, 158U};
inline constexpr MessageId msg_complete_agent_movement{MessageFrequency::low, 249U};
inline constexpr MessageId msg_agent_data_update{MessageFrequency::low, 387U};
inline constexpr MessageId msg_agent_movement_complete{MessageFrequency::low, 250U};
inline constexpr MessageId msg_packet_ack{MessageFrequency::fixed, 0xFFFFFFFBU};

enum PacketFlags : std::uint8_t
{
    packet_zerocoded = 0x80U,
    packet_reliable = 0x40U,
    packet_resent = 0x20U,
    packet_appended_acks = 0x10U
};

struct PacketHeader
{
    std::uint8_t flags = 0;
    std::uint32_t sequence = 0;
    std::uint8_t extra_header_length = 0;
    MessageId message;
};

struct DecodedPacket
{
    PacketHeader header;
    std::vector<std::uint8_t> payload;
    std::vector<std::uint32_t> appended_acks;
    std::vector<std::uint8_t> raw;
};

struct DecodeResult
{
    std::optional<DecodedPacket> packet;
    std::string error;

    [[nodiscard]] bool ok() const noexcept
    {
        return packet.has_value() && error.empty();
    }
};

struct ReliablePacket
{
    std::uint32_t sequence = 0;
    std::vector<std::uint8_t> bytes;
    std::chrono::steady_clock::time_point last_sent{};
    std::uint32_t resend_count = 0;
};

class PacketThrottle
{
public:
    explicit PacketThrottle(
        std::size_t bytes_per_second = 256U * 1024U,
        std::size_t burst_bytes = lludp_mtu * 4U);

    [[nodiscard]] bool consume(
        std::size_t bytes,
        std::chrono::steady_clock::time_point now);

private:
    std::size_t bytes_per_second_;
    std::size_t burst_bytes_;
    double tokens_;
    std::chrono::steady_clock::time_point last_refill_;
};

class ReliableTracker
{
public:
    explicit ReliableTracker(
        std::chrono::milliseconds resend_after = std::chrono::milliseconds{1000},
        std::uint32_t max_resends = 5U);

    void track(std::uint32_t sequence, std::vector<std::uint8_t> bytes);
    [[nodiscard]] bool acknowledge(std::uint32_t sequence);
    [[nodiscard]] bool contains(std::uint32_t sequence) const;
    [[nodiscard]] std::size_t pending_count() const noexcept;

    [[nodiscard]] std::vector<ReliablePacket> due_for_resend(
        std::chrono::steady_clock::time_point now);
    [[nodiscard]] bool exhausted() const noexcept;

private:
    std::map<std::uint32_t, ReliablePacket> pending_;
    std::chrono::milliseconds resend_after_;
    std::uint32_t max_resends_;
    bool exhausted_ = false;
};

[[nodiscard]] std::optional<MessageTemplateEntry> known_message_template(MessageId id);
[[nodiscard]] std::optional<std::vector<std::uint8_t>> uuid_to_bytes(std::string_view uuid);
[[nodiscard]] std::vector<std::uint8_t> encode_message_id(MessageId id);
[[nodiscard]] DecodeResult decode_lludp_packet(std::span<const std::uint8_t> bytes);
[[nodiscard]] std::vector<std::uint8_t> encode_packet(
    std::uint8_t flags,
    std::uint32_t sequence,
    MessageId message,
    std::span<const std::uint8_t> payload,
    std::span<const std::uint32_t> appended_acks = {});
[[nodiscard]] std::vector<std::uint8_t> encode_packet_ack(
    std::uint32_t sequence,
    std::span<const std::uint32_t> acknowledged_sequences);
[[nodiscard]] std::vector<std::uint8_t> encode_use_circuit_code(
    std::uint32_t sequence,
    const LoginSession& session);
[[nodiscard]] std::vector<std::uint8_t> encode_complete_agent_movement(
    std::uint32_t sequence,
    const LoginSession& session);
[[nodiscard]] std::vector<std::uint8_t> encode_region_handshake_reply(
    std::uint32_t sequence,
    const LoginSession& session,
    std::uint32_t flags = 0U);
[[nodiscard]] std::vector<std::uint8_t> encode_complete_ping_check(
    std::uint32_t sequence,
    std::uint8_t ping_id);
}
