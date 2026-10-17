#include "nexverse/protocol/lludp.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    using namespace nexverse::protocol;

    LoginSession session;
    session.agent_id = "11111111-2222-3333-4444-555555555555";
    session.session_id = "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
    session.circuit_code = 0x12345678U;

    const auto uuid = uuid_to_bytes(session.agent_id);
    if (!uuid.has_value() || uuid->size() != 16U ||
        (*uuid)[0] != 0x11U || (*uuid)[15] != 0x55U)
    {
        std::cerr << "UUID conversion failed\n";
        return 1;
    }

    const auto use = encode_use_circuit_code(42U, session);
    if (use.size() != 46U ||
        use[0] != packet_reliable ||
        use[1] != 0x00U || use[2] != 0x00U || use[3] != 0x00U || use[4] != 0x2AU ||
        use[6] != 0xFFU || use[7] != 0xFFU || use[8] != 0x00U || use[9] != 0x03U ||
        use[10] != 0x78U || use[11] != 0x56U || use[12] != 0x34U || use[13] != 0x12U)
    {
        std::cerr << "UseCircuitCode encoding failed\n";
        return 2;
    }

    const auto decoded_use = decode_lludp_packet(use);
    if (!decoded_use.ok() ||
        decoded_use.packet->header.sequence != 42U ||
        decoded_use.packet->header.message != msg_use_circuit_code ||
        decoded_use.packet->payload.size() != 36U)
    {
        std::cerr << "UseCircuitCode decode failed\n";
        return 3;
    }

    const std::array<std::uint32_t, 2> ack_ids{42U, 99U};
    const auto ack = encode_packet_ack(7U, ack_ids);
    const auto decoded_ack = decode_lludp_packet(ack);
    if (!decoded_ack.ok() ||
        decoded_ack.packet->header.message != msg_packet_ack ||
        decoded_ack.packet->payload.size() != 9U ||
        decoded_ack.packet->payload[0] != 2U)
    {
        std::cerr << "PacketAck encoding failed\n";
        return 4;
    }

    const std::array<std::uint8_t, 2> payload{0xAAU, 0xBBU};
    const std::array<std::uint32_t, 2> appended{0x01020304U, 0xAABBCCDDU};
    const auto with_acks = encode_packet(
        packet_reliable,
        100U,
        MessageId{MessageFrequency::high, 5U},
        payload,
        appended);
    const auto decoded_with_acks = decode_lludp_packet(with_acks);

    if (!decoded_with_acks.ok() ||
        decoded_with_acks.packet->payload.size() != 2U ||
        decoded_with_acks.packet->appended_acks != std::vector<std::uint32_t>(appended.begin(), appended.end()) ||
        with_acks.size() < 18U ||
        with_acks[9] != 0x04U || with_acks[10] != 0x03U ||
        with_acks[11] != 0x02U || with_acks[12] != 0x01U ||
        with_acks[13] != 0xDDU || with_acks[14] != 0xCCU ||
        with_acks[15] != 0xBBU || with_acks[16] != 0xAAU)
    {
        std::cerr << "Appended ACK encoding/decoding failed\n";
        return 5;
    }

    ReliableTracker tracker{std::chrono::milliseconds{10}, 2U};
    tracker.track(10U, use);
    if (!tracker.contains(10U) || tracker.pending_count() != 1U)
    {
        std::cerr << "Reliable tracker insert failed\n";
        return 6;
    }

    const auto future = std::chrono::steady_clock::now() + std::chrono::milliseconds{20};
    auto due = tracker.due_for_resend(future);
    if (due.size() != 1U || (due.front().bytes[0] & packet_resent) == 0U)
    {
        std::cerr << "Reliable resend scheduling failed\n";
        return 7;
    }

    if (!tracker.acknowledge(10U) || tracker.pending_count() != 0U)
    {
        std::cerr << "Reliable ACK handling failed\n";
        return 8;
    }

    for(const auto id:{msg_remove_inventory_item,
                       msg_remove_inventory_folder,
                       msg_remove_inventory_objects})
    {
        const auto entry=known_message_template(id);
        const auto id_bytes=encode_message_id(id);
        if(!entry || id_bytes.size()!=4U || id_bytes[0]!=0xFFU ||
           id_bytes[1]!=0xFFU || id_bytes[2]!=(id.value>>8U) ||
           id_bytes[3]!=(id.value&0xFFU))
        {
            std::cerr<<"Legacy inventory removal LLUDP ID mismatch\n";
            return 71;
        }
    }

    const auto movement = encode_complete_agent_movement(43U, session);
    const auto decoded_movement = decode_lludp_packet(movement);
    if (!decoded_movement.ok() ||
        decoded_movement.packet->header.message != msg_complete_agent_movement ||
        decoded_movement.packet->payload.size() != 36U)
    {
        std::cerr << "CompleteAgentMovement encoding failed\n";
        return 9;
    }

    const auto pong = encode_complete_ping_check(44U, 17U);
    const auto decoded_pong = decode_lludp_packet(pong);
    if (!decoded_pong.ok() ||
        decoded_pong.packet->header.message != msg_complete_ping_check ||
        decoded_pong.packet->payload.size() != 1U ||
        decoded_pong.packet->payload[0] != 17U)
    {
        std::cerr << "CompletePingCheck encoding failed\n";
        return 10;
    }

    const auto use_template = known_message_template(msg_use_circuit_code);
    if (!use_template.has_value() ||
        use_template->name != "UseCircuitCode" ||
        !use_template->reliable)
    {
        std::cerr << "Message template registry failed\n";
        return 11;
    }

    const auto region_reply = encode_region_handshake_reply(45U, session, 0U);
    const auto decoded_region_reply = decode_lludp_packet(region_reply);
    if (!decoded_region_reply.ok() ||
        decoded_region_reply.packet->header.message != msg_region_handshake_reply ||
        decoded_region_reply.packet->payload.size() != 36U)
    {
        std::cerr << "RegionHandshakeReply encoding failed\n";
        return 12;
    }

    // Golden wire: UUID(16)+session(16)+U32 LittleEndian. Firestorm
    // RH_SUPPORTS_SELF_APPEARANCE is bit2=0x00000004; bits 0/1
    // MUST remain zero without a real object cache.
    if(ogl_region_handshake_flags!=4U||
       !advertises_self_appearance(ogl_region_handshake_flags) ||
       (ogl_region_handshake_flags&3U)!=0U)return 52;
    const auto ssa=decode_lludp_packet(
        encode_region_handshake_reply(46U,session));
    if(!ssa.ok()||ssa.packet->payload.size()!=36U||
       ssa.packet->header.message!=msg_region_handshake_reply||
       (ssa.packet->header.flags&packet_reliable)==0U||
       ssa.packet->payload[32U]!=4U||
       ssa.packet->payload[33U]!=0U||
       ssa.packet->payload[34U]!=0U||
       ssa.packet->payload[35U]!=0U)return 53;
    // Explicit legacy opt-out must still serialize exactly zero.
    if(decoded_region_reply.packet->payload[32U]!=0U||
       decoded_region_reply.packet->payload[33U]!=0U)return 54;

    const auto movement_complete_template =
        known_message_template(msg_agent_movement_complete);
    if (!movement_complete_template.has_value() ||
        movement_complete_template->name != "AgentMovementComplete")
    {
        std::cerr << "AgentMovementComplete template lookup failed\n";
        return 13;
    }

    PacketThrottle throttle{100U, 100U};
    const auto throttle_now = std::chrono::steady_clock::now();
    if (!throttle.consume(100U, throttle_now) ||
        throttle.consume(1U, throttle_now))
    {
        std::cerr << "Packet throttle initial budget failed\n";
        return 14;
    }

    if (!throttle.consume(
            10U,
            throttle_now + std::chrono::milliseconds{100}))
    {
        std::cerr << "Packet throttle refill failed\n";
        return 15;
    }

    std::cout << "LLUDP codec, reliability and throttle tests passed\n";
    return 0;
}
