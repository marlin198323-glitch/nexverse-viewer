/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/protocol/lludp.hpp"
#include <cstdint>
#include <vector>
namespace nexverse::protocol
{
// Firestorm scripts/messages/message_template.msg Low 117/119.
// SurfaceInfo is a *variable block*: zero entries are permitted when
// the viewer has not computed a precise mesh ray/face hit.
inline constexpr MessageId msg_object_grab{MessageFrequency::low,117U};
inline constexpr MessageId msg_object_degrab{MessageFrequency::low,119U};
enum class ObjectTouchPhase : std::uint8_t { begin, end };
struct ObjectTouchCommand
{
    std::uint32_t local_id=0U;
    ObjectTouchPhase phase=ObjectTouchPhase::begin;
};
[[nodiscard]] inline std::vector<std::uint8_t> encode_object_touch(
    std::uint32_t sequence,const LoginSession& session,
    ObjectTouchCommand command)
{
    if(command.local_id==0U)return {};
    const auto agent=uuid_to_bytes(session.agent_id);
    const auto login=uuid_to_bytes(session.session_id);
    if(!agent||!login)return {};
    std::vector<std::uint8_t> bytes;
    bytes.reserve(command.phase==ObjectTouchPhase::begin?49U:37U);
    bytes.insert(bytes.end(),agent->begin(),agent->end());
    bytes.insert(bytes.end(),login->begin(),login->end());
    for(unsigned i=0U;i<4U;++i)
        bytes.push_back(static_cast<std::uint8_t>(
            command.local_id>>(i*8U)));
    if(command.phase==ObjectTouchPhase::begin)
    {
        // GrabOffset vector3 (0,0,0) is intentionally approximate
        // and never claims a face pick.
        bytes.insert(bytes.end(),12U,0U);
    }
    bytes.push_back(0U); // SurfaceInfo array count
    return encode_packet(packet_reliable,sequence,
        command.phase==ObjectTouchPhase::begin
            ?msg_object_grab:msg_object_degrab,
        bytes);
}
}
