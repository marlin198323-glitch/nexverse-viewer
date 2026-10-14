/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/circuit.hpp"

#include <algorithm>
#include <thread>
#include <type_traits>
#include <utility>

namespace
{
std::uint32_t read_le_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return
        static_cast<std::uint32_t>(bytes[offset]) |
        (static_cast<std::uint32_t>(bytes[offset + 1U]) << 8U) |
        (static_cast<std::uint32_t>(bytes[offset + 2U]) << 16U) |
        (static_cast<std::uint32_t>(bytes[offset + 3U]) << 24U);
}

std::vector<std::uint32_t> parse_packet_ack_payload(std::span<const std::uint8_t> payload)
{
    std::vector<std::uint32_t> result;
    if (payload.empty()) return result;

    const std::size_t count = payload[0];
    if (payload.size() < 1U + count * 4U) return {};

    result.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        result.push_back(read_le_u32(payload, 1U + i * 4U));
    }
    return result;
}
}

namespace nexverse::protocol
{
CircuitConnection::CircuitConnection(LoginSession session)
    : session_{std::move(session)}
{
}

bool CircuitConnection::send_datagram(
    net::UdpSocket& socket,
    std::span<const std::uint8_t> bytes,
    CircuitStatus& status)
{
    while (!throttle_.consume(bytes.size(), std::chrono::steady_clock::now()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }

    std::string error;
    if (!socket.send(bytes, error))
    {
        status.state = CircuitState::failed;
        status.message = error.empty() ? "LLUDP send failed" : error;
        return false;
    }

    ++status.packets_sent;
    return true;
}

bool CircuitConnection::send_reliable(
    net::UdpSocket& socket,
    ReliableTracker& tracker,
    std::vector<std::uint8_t> bytes,
    std::uint32_t sequence,
    CircuitStatus& status)
{
    if (bytes.empty())
    {
        status.state = CircuitState::failed;
        status.message = "LLUDP packet encoding failed";
        return false;
    }

    if (!send_datagram(socket, bytes, status))
    {
        return false;
    }

    tracker.track(sequence, bytes);
    return true;
}

bool CircuitConnection::handle_packet(
    net::UdpSocket& socket,
    ReliableTracker& tracker,
    const DecodedPacket& packet,
    CircuitStatus& status,
    std::uint32_t& next_sequence,
    std::uint32_t use_circuit_sequence,
    bool& movement_sent,
    std::uint32_t& movement_sequence)
{
    ++status.packets_received;

    std::vector<std::uint32_t> acknowledgements = packet.appended_acks;
    if (packet.header.message == msg_packet_ack)
    {
        auto explicit_acks = parse_packet_ack_payload(packet.payload);
        acknowledgements.insert(
            acknowledgements.end(),
            explicit_acks.begin(),
            explicit_acks.end());
    }

    for (const auto ack : acknowledgements)
    {
        if (tracker.acknowledge(ack))
        {
            ++status.acks_received;
        }

        if (ack == use_circuit_sequence && !movement_sent)
        {
            movement_sequence = next_sequence++;
            auto movement = encode_complete_agent_movement(movement_sequence, session_);
            if (!send_reliable(
                    socket,
                    tracker,
                    std::move(movement),
                    movement_sequence,
                    status))
            {
                return false;
            }

            movement_sent = true;
            status.state = CircuitState::awaiting_movement_ack;
            status.message = "UseCircuitCode acknowledged; CompleteAgentMovement sent";
        }

        if (movement_sent && ack == movement_sequence &&
            status.state == CircuitState::awaiting_movement_ack)
        {
            status.state = CircuitState::awaiting_region_ready;
            status.message = "CompleteAgentMovement acknowledged; awaiting region readiness";
        }
    }

    if ((packet.header.flags & packet_reliable) != 0U)
    {
        const std::array<std::uint32_t, 1> ack_ids{packet.header.sequence};
        const auto ack = encode_packet_ack(next_sequence++, ack_ids);

        if (!send_datagram(socket, ack, status))
        {
            status.message = "Failed to ACK simulator packet";
            return false;
        }

        ++status.acks_sent;
    }

    if (packet.header.message == msg_start_ping_check && !packet.payload.empty())
    {
        const auto pong = encode_complete_ping_check(next_sequence++, packet.payload[0]);
        if (!send_datagram(socket, pong, status))
        {
            status.message = "Failed to answer simulator ping";
            return false;
        }
    }

    if (packet.header.message == msg_region_handshake)
    {
        const std::uint32_t reply_sequence = next_sequence++;
        auto reply = encode_region_handshake_reply(reply_sequence, session_);
        if (!send_reliable(
                socket,
                tracker,
                std::move(reply),
                reply_sequence,
                status))
        {
            status.message = "Failed to send RegionHandshakeReply";
            return false;
        }

        status.state = CircuitState::awaiting_region_ready;
        status.message = "RegionHandshake received; reply sent";
    }

    if (packet.header.message == msg_agent_movement_complete)
    {
        status.state = CircuitState::active;
        status.message = "LLUDP circuit active; AgentMovementComplete received";
    }

    return true;
}

CircuitStatus CircuitConnection::run(
    std::atomic_bool& stop_requested,
    StatusCallback on_status,
    PacketCallback on_packet,
    OutboundCommandQueue* outbound,
    std::chrono::seconds startup_timeout)
{
    CircuitStatus status;
    const auto notify = [&]
    {
        if (on_status) on_status(status);
    };

    status.state = CircuitState::opening_socket;
    status.message = "Opening simulator UDP socket";
    notify();

    if (session_.simulator_ip.empty() || session_.simulator_port == 0U)
    {
        status.state = CircuitState::failed;
        status.message = "Login session does not contain a simulator endpoint";
        notify();
        return status;
    }

    net::UdpSocket socket;
    std::string socket_error;
    if (!socket.open(session_.simulator_ip, session_.simulator_port, socket_error))
    {
        status.state = CircuitState::failed;
        status.message = socket_error.empty()
            ? "Could not open simulator UDP socket"
            : socket_error;
        notify();
        return status;
    }

    ReliableTracker tracker{std::chrono::milliseconds{750}, 6U};
    std::uint32_t next_sequence = 1U;

    const std::uint32_t use_circuit_sequence = next_sequence++;
    auto use_circuit = encode_use_circuit_code(use_circuit_sequence, session_);
    if (!send_reliable(
            socket,
            tracker,
            std::move(use_circuit),
            use_circuit_sequence,
            status))
    {
        return status;
    }

    status.state = CircuitState::awaiting_use_circuit_ack;
    status.message = "UseCircuitCode sent";
    notify();

    bool movement_sent = false;
    std::uint32_t movement_sequence = 0U;
    const auto started = std::chrono::steady_clock::now();
    auto last_receive = started;

    while (!stop_requested.load())
    {
        const auto now = std::chrono::steady_clock::now();
        if (status.state != CircuitState::active &&
            now - started > startup_timeout)
        {
            status.state = CircuitState::failed;
            status.message = "LLUDP circuit startup timed out";
            notify();
            return status;
        }

        if (status.state == CircuitState::active &&
            now - last_receive > std::chrono::seconds{30})
        {
            status.state = CircuitState::failed;
            status.message = "LLUDP simulator timed out";
            notify();
            return status;
        }

        for (auto resend : tracker.due_for_resend(now))
        {
            if (!send_datagram(socket, resend.bytes, status))
            {
                status.message = "LLUDP resend failed";
                notify();
                return status;
            }
            ++status.packets_resent;
        }

        if (tracker.exhausted())
        {
            status.state = CircuitState::failed;
            status.message = "LLUDP reliable retry limit reached";
            notify();
            return status;
        }

        if (status.state == CircuitState::active && outbound != nullptr)
        {
            for (std::size_t command_count = 0; command_count < 16U; ++command_count)
            {
                auto command = outbound->try_pop();
                if (!command.has_value()) break;

                const std::uint32_t sequence = next_sequence++;
                bool sent = true;

                std::visit(
                    [&](const auto& value)
                    {
                        using T = std::decay_t<decltype(value)>;

                        if constexpr (std::is_same_v<T, AgentUpdateData>)
                        {
                            const auto packet = encode_agent_update(sequence, session_, value);
                            sent = !packet.empty() && send_datagram(socket, packet, status);
                        }
                        else if constexpr (std::is_same_v<T, LocalChatMessage>)
                        {
                            auto packet = encode_chat_from_viewer(sequence, session_, value);
                            sent = send_reliable(
                                socket,
                                tracker,
                                std::move(packet),
                                sequence,
                                status);
                        }
                        else if constexpr (std::is_same_v<T, ObjectTouchCommand>)
                        {
                            auto packet=encode_object_touch(
                                sequence,session_,value);
                            sent=!packet.empty()&&send_reliable(
                                socket,tracker,std::move(packet),
                                sequence,status);
                        }
                        else if constexpr (std::is_same_v<T, CofLinkCommand>)
                        {
                            auto packet=encode_cof_link(sequence,session_,value);
                            sent=!packet.empty()&&send_reliable(
                                socket,tracker,std::move(packet),sequence,status);
                        }
                        else if constexpr (std::is_same_v<T, CofUnlinkCommand>)
                        {
                            auto packet=encode_cof_unlink(sequence,session_,value);
                            sent=!packet.empty()&&send_reliable(
                                socket,tracker,std::move(packet),sequence,status);
                        }
                        else if constexpr (std::is_same_v<T, AnimationAssetRequest>)
                        {
                            auto packet=encode_animation_asset_transfer(
                                sequence,value);
                            sent=!packet.empty()&&send_reliable(
                                socket,tracker,std::move(packet),
                                sequence,status);
                        }
                        else if constexpr (std::is_same_v<T, TeleportLocation>)
                        {
                            auto packet = encode_teleport_location_request(
                                sequence,
                                session_,
                                value);
                            sent = send_reliable(
                                socket,
                                tracker,
                                std::move(packet),
                                sequence,
                                status);
                        }
                    },
                    *command);

                if (!sent)
                {
                    notify();
                    return status;
                }
            }
        }

        auto received = socket.receive(lludp_mtu, std::chrono::milliseconds{50});
        if (!received.error.empty())
        {
            status.state = CircuitState::failed;
            status.message = received.error;
            notify();
            return status;
        }
        if (!received.received)
        {
            continue;
        }

        last_receive = std::chrono::steady_clock::now();

        const auto decoded = decode_lludp_packet(received.bytes);
        if (!decoded.ok())
        {
            continue;
        }

        if (!handle_packet(
                socket,
                tracker,
                *decoded.packet,
                status,
                next_sequence,
                use_circuit_sequence,
                movement_sent,
                movement_sequence))
        {
            notify();
            return status;
        }

        if (on_packet)
        {
            on_packet(*decoded.packet);
        }

        notify();
    }

    status.state = CircuitState::disconnected;
    status.message = "LLUDP circuit stopped";
    notify();
    return status;
}
}
