/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_wearable_slots.hpp"
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <span>
#include <vector>
namespace nexverse::protocol
{
// Firestorm_7.2.4 message_template.msg:
// AgentIsNowWearing Low383 NotTrusted Zerocoded Reliable,
// AgentSetAppearance Low84 NotTrusted Zerocoded.
// These are BOUNDED FORMAT ENCODERS, deliberately NOT wired to
// outbound_queue until wearable assets, cached textures and baked
// TextureEntry/VisualParams match the avatar's actual body.
struct FirestormWearableSlot
{
    std::uint8_t type=0U;
    std::string item_id;
};
[[nodiscard]] inline std::vector<std::uint8_t> encode_agent_is_now_wearing(
    std::uint32_t sequence,const LoginSession& session,
    std::span<const FirestormWearableSlot> slots)
{
    const auto agent=uuid_to_bytes(session.agent_id);
    const auto sid=uuid_to_bytes(session.session_id);
    const auto real=[](const auto& id){
        return id&&std::any_of(id->begin(),id->end(),
            [](std::uint8_t c){return c!=0U;});
    };
    if(!real(agent)||!real(sid)||slots.size()!=17U)return {};
    std::array<bool,17> seen{};
    std::vector<std::uint8_t> payload;
    payload.insert(payload.end(),agent->begin(),agent->end());
    payload.insert(payload.end(),sid->begin(),sid->end());
    payload.push_back(static_cast<std::uint8_t>(slots.size()));
    for(const auto& slot:slots)
    {
        if(slot.type>=17U||seen[slot.type])return {};
        seen[slot.type]=true;
        const auto id=uuid_to_bytes(slot.item_id);
        if(!id)return {};
        if(slot.type<4U&&!real(id))return {};
        payload.insert(payload.end(),id->begin(),id->end());
        payload.push_back(slot.type);
    }
    // Reliable LLUDP without zerocoded bit is a valid non-compressed
    // representation of this message; setting the bit without
    // encoding would corrupt all UUIDs containing zeros.
    return encode_packet(packet_reliable,sequence,
                         msg_agent_is_now_wearing,payload);
}
struct AppearanceCacheSlot
{
    std::string cache_id;
    std::uint8_t texture_index=0U;
};
struct AgentSetAppearanceCommand
{
    std::uint32_t serial=0U;
    std::array<float,3> avatar_size{};
    std::vector<AppearanceCacheSlot> wearable_cache;
    std::vector<std::uint8_t> texture_entry;
    std::vector<std::uint8_t> visual_params;
};
[[nodiscard]] inline std::vector<std::uint8_t> encode_agent_set_appearance(
    std::uint32_t sequence,const LoginSession& session,
    const AgentSetAppearanceCommand& command)
{
    const auto agent=uuid_to_bytes(session.agent_id);
    const auto sid=uuid_to_bytes(session.session_id);
    const auto real=[](const auto& id){
        return id&&std::any_of(id->begin(),id->end(),
            [](std::uint8_t c){return c!=0U;});
    };
    if(!real(agent)||!real(sid)||command.serial==0U||
       command.wearable_cache.size()>32U||
       command.texture_entry.empty()||command.texture_entry.size()>640U||
       command.visual_params.empty()||command.visual_params.size()>240U)
        return {};
    for(const float v:command.avatar_size)
        if(!std::isfinite(v)||v<0.1f||v>10.0f)return {};
    std::array<bool,256> seen{};
    std::vector<std::uint8_t> payload;
    const auto u32=[&](std::uint32_t value){
        for(unsigned i=0;i<4;++i)
            payload.push_back(static_cast<std::uint8_t>(value>>(8U*i)));
    };
    payload.insert(payload.end(),agent->begin(),agent->end());
    payload.insert(payload.end(),sid->begin(),sid->end());
    u32(command.serial);
    for(const float v:command.avatar_size)
        u32(std::bit_cast<std::uint32_t>(v));
    payload.push_back(static_cast<std::uint8_t>(command.wearable_cache.size()));
    for(const auto& entry:command.wearable_cache)
    {
        const auto id=uuid_to_bytes(entry.cache_id);
        if(!real(id)||seen[entry.texture_index])return {};
        seen[entry.texture_index]=true;
        payload.insert(payload.end(),id->begin(),id->end());
        payload.push_back(entry.texture_index);
    }
    const auto n=static_cast<std::uint16_t>(command.texture_entry.size());
    payload.push_back(static_cast<std::uint8_t>(n&0xffU));
    payload.push_back(static_cast<std::uint8_t>(n>>8U));
    payload.insert(payload.end(),command.texture_entry.begin(),
                   command.texture_entry.end());
    payload.push_back(static_cast<std::uint8_t>(command.visual_params.size()));
    payload.insert(payload.end(),command.visual_params.begin(),
                   command.visual_params.end());
    if(payload.size()>lludp_max_payload)return {};
    return encode_packet(packet_reliable,sequence,
                         msg_agent_set_appearance,payload);
}
}
