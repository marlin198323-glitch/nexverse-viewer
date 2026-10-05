/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/udp_socket.hpp"
#include "nexverse/protocol/lludp.hpp"

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
    std::uint32_t packets_resent = 0;
    std::uint32_t acks_sent = 0;
    std::uint32_t acks_received = 0;
};

class CircuitConnection
{
public:
    explicit CircuitConnection(LoginSession session);

    using StatusCallback = std::function<void(const CircuitStatus&)>;
    using PacketCallback = std::function<void(const DecodedPacket&)>;

    [[nodiscard]] CircuitStatus run(
        std::atomic_bool& stop_requested,
        StatusCallback on_status = {},
        PacketCallback on_packet = {},
        std::chrono::seconds startup_timeout = std::chrono::seconds{15});

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
};
}
