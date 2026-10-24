/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/udp_socket.hpp"
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/protocol/outbound_queue.hpp"
#include "nexverse/protocol/wearables_readback_retry.hpp"
#include "nexverse/protocol/region_handshake_replay.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>

namespace nexverse::protocol
{
enum class CircuitState
{
    disconnected,
    opening_socket,
    awaiting_use_circuit_ack,
    awaiting_movement_ack,
    awaiting_region_ready,
    active,
    failed
};

struct CircuitStatus
{
    CircuitState state = CircuitState::disconnected;
    std::string message;
    std::uint32_t packets_sent = 0;
    std::uint32_t packets_received = 0;
    // UDP socket accepted these AgentUpdate datagrams; NOT a simulator ACK.
    std::uint32_t agent_updates_sent=0U;
    std::uint32_t directional_agent_updates_sent=0U;
    std::uint32_t packets_resent = 0;
    std::uint32_t acks_sent = 0;
    std::uint32_t acks_received = 0;
    // True only after reliable RegionHandshakeReply datagram was accepted
    // for sending. Does not prove ACK or server AvatarAppearance delivery.
    bool self_appearance_support_sent=false;
    // Duplicate inbound RegionHandshake is ACKed but cannot regress
    // current simulator state or invalidate a real SSA reply ACK.
    std::uint32_t region_handshake_duplicates=0U;
    std::uint32_t region_handshake_refreshes=0U;
    std::uint32_t region_handshake_pre_active_ack_preserved=0U;
    // Legacy Low381 read-only bootstrap (socket-send, not ACK/reply).
    // A packet ID is not a simulator handshake. These counters expose
    // real Low15 admission, not a confirmed rendered avatar.
    bool movement_complete_validated=false;
    std::uint32_t movement_complete_rejected=0U;
    std::uint32_t movement_complete_wrong_region=0U;
    bool wearables_read_request_sent=false;
    std::uint32_t wearables_read_retries_sent=0U;
    bool wearables_valid_wire_received=false;
    // True only for an ACK matched by the reliable tracker to the LATEST
    // RegionHandshakeReply sequence. Not a simulator AvatarAppearance.
    bool self_appearance_support_acknowledged=false;
    // Monotonic local timestamp of the exact reliable ACK; never
    // store a simulator address, avatar ID, account or CAP token.
    std::chrono::steady_clock::time_point self_appearance_ack_at{};
};

class CircuitConnection
{
public:
    explicit CircuitConnection(LoginSession session);

    using StatusCallback = std::function<void(const CircuitStatus&)>;
    using PacketCallback = std::function<void(const DecodedPacket&)>;
    // Called only after a real UDP socket successfully sends AgentUpdate.
    // Non-reliable UDP has no server ACK. Flags only, no agent identifiers.
    using AgentUpdateSentCallback =
        std::function<void(std::uint32_t,const Quaternion&)>;

    [[nodiscard]] CircuitStatus run(
        std::atomic_bool& stop_requested,
        StatusCallback on_status = {},
        PacketCallback on_packet = {},
        OutboundCommandQueue* outbound = nullptr,
        std::chrono::seconds startup_timeout = std::chrono::seconds{15},
        AgentUpdateSentCallback on_agent_update_sent = {});

private:
    [[nodiscard]] bool send_datagram(
        net::UdpSocket& socket,
        std::span<const std::uint8_t> bytes,
        CircuitStatus& status);

    [[nodiscard]] bool send_reliable(
        net::UdpSocket& socket,
        ReliableTracker& tracker,
        std::vector<std::uint8_t> bytes,
        std::uint32_t sequence,
        CircuitStatus& status);
    [[nodiscard]] bool handle_packet(
        net::UdpSocket& socket,
        ReliableTracker& tracker,
        const DecodedPacket& packet,
        CircuitStatus& status,
        std::uint32_t& next_sequence,
        std::uint32_t use_circuit_sequence,
        bool& movement_sent,
        std::uint32_t& movement_sequence);

    LoginSession session_;
    PacketThrottle throttle_;
    SelfAppearanceHandshakeReceipt self_appearance_receipt_;
    RegionHandshakeReplay region_handshake_replay_;
    bool wearables_initial_requested_=false;
};
}
