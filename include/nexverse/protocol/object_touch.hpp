/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/protocol/lludp.hpp"
#include <cstdint>
#include <vector>
#include <array>
#include <optional>
#include <bit>
#include <cmath>
namespace nexverse::protocol
{
// Firestorm scripts/messages/message_template.msg Low 117/119.
// SurfaceInfo is a *variable block*: zero entries are permitted when
// the viewer has not computed a precise mesh ray/face hit.
inline constexpr MessageId msg_object_grab{MessageFrequency::low,117U};
inline constexpr MessageId msg_object_degrab{MessageFrequency::low,119U};
enum class ObjectTouchPhase : std::uint8_t { begin, end };
// Firestorm ObjectGrab/DeGrab SurfaceInfo one full variable block.
// All coords are simulator-region local, vectors are world orientation.
struct TouchSurfaceInfo
{
    std::array<float,3> uv{};
    std::array<float,3> st{};
    std::int32_t face=-1;
    std::array<float,3> position{};
    std::array<float,3> normal{};
    std::array<float,3> binormal{};
    [[nodiscard]] bool valid() const noexcept
    {
        const auto finite3=[](const auto& v) {
            return std::isfinite(v[0])&&std::isfinite(v[1])&&
                   std::isfinite(v[2]);};
        const auto norm2=[](const auto& v) {
            return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];};
        return face>=0&&face<64&&finite3(uv)&&finite3(st)&&
               finite3(position)&&finite3(normal)&&finite3(binormal)&&
               std::isfinite(norm2(normal))&&norm2(normal)>0.5f&&
               std::isfinite(norm2(binormal))&&norm2(binormal)>0.5f;
    }
};
struct ObjectTouchCommand
{
    std::uint32_t local_id=0U;
    ObjectTouchPhase phase=ObjectTouchPhase::begin;
    std::optional<TouchSurfaceInfo> surface;
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
    if(command.surface&&!command.surface->valid())return {};
    bytes.reserve((command.phase==ObjectTouchPhase::begin?49U:37U)+
                  (command.surface?64U:0U));
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
    bytes.push_back(command.surface?1U:0U); // SurfaceInfo variable count
    if(command.surface)
    {
        const auto append_f32=[&](float f)
        {
            const auto bits=std::bit_cast<std::uint32_t>(f);
            for(unsigned i=0U;i<4U;++i)
                bytes.push_back(static_cast<std::uint8_t>(
                    bits>>(8U*i)));
        };
        const auto append_vec=[&](const std::array<float,3>& v)
        {for(const auto f:v)append_f32(f);};
        // Firestorm template order, not renderer struct layout.
        append_vec(command.surface->uv);
        append_vec(command.surface->st);
        const auto face=static_cast<std::uint32_t>(command.surface->face);
        for(unsigned i=0U;i<4U;++i)
            bytes.push_back(static_cast<std::uint8_t>(face>>(8U*i)));
        append_vec(command.surface->position);
        append_vec(command.surface->normal);
        append_vec(command.surface->binormal);
    }
    return encode_packet(packet_reliable,sequence,
        command.phase==ObjectTouchPhase::begin
            ?msg_object_grab:msg_object_degrab,
        bytes);
}
}
