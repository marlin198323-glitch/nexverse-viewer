/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/lludp.hpp"
#include <algorithm>
#include <bit>
#include <cstddef>
#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
// Firestorm message_template.msg: simulator->viewer, High 20 Trusted Unencoded.
inline constexpr MessageId msg_avatar_animation{MessageFrequency::high,20U};

struct AvatarAnimationItem
{
    std::string id;
    std::int32_t sequence=0;
};
struct AvatarAnimationMessage
{
    std::string sender;
    std::vector<AvatarAnimationItem> animations;
    std::vector<std::string> sources;
    std::vector<std::vector<std::uint8_t>> physical_events;
};

// A full authoritative snapshot: AnimationList, AnimationSourceList and
// PhysicalAvatarEventList each start with U8 variable-block count.
[[nodiscard]] inline std::optional<AvatarAnimationMessage>
decode_avatar_animation(std::span<const std::uint8_t> bytes)
{
    if(bytes.size()<19U||bytes.size()>12000U)return std::nullopt;
    std::size_t cursor=0U;
    auto uuid=[&](std::string& value)->bool
    {
        if(bytes.size()-cursor<16U)return false;
        static constexpr char hex[]="0123456789abcdef";
        value.clear();
        value.reserve(36U);
        for(std::size_t j=0U;j<16U;++j)
        {
            if(j==4U||j==6U||j==8U||j==10U)value.push_back('-');
            value.push_back(hex[bytes[cursor+j]>>4U]);
            value.push_back(hex[bytes[cursor+j]&0x0fU]);
        }
        cursor+=16U;
        return !value.empty();
    };
    AvatarAnimationMessage out;
    if(!uuid(out.sender)||out.sender==
       "00000000-0000-0000-0000-000000000000")return std::nullopt;
    const std::size_t anim_count=bytes[cursor++];
    if(anim_count>96U||anim_count>(bytes.size()-cursor)/20U)
        return std::nullopt;
    out.animations.reserve(anim_count);
    for(std::size_t i=0U;i<anim_count;++i)
    {
        AvatarAnimationItem item;
        if(!uuid(item.id))return std::nullopt;
        if(item.id=="00000000-0000-0000-0000-000000000000")
            return std::nullopt;
        const std::uint32_t raw=static_cast<std::uint32_t>(bytes[cursor])|
            (static_cast<std::uint32_t>(bytes[cursor+1U])<<8U)|
            (static_cast<std::uint32_t>(bytes[cursor+2U])<<16U)|
            (static_cast<std::uint32_t>(bytes[cursor+3U])<<24U);
        item.sequence=std::bit_cast<std::int32_t>(raw);
        cursor+=4U;
        const auto found=std::find_if(out.animations.begin(),
            out.animations.end(),[&](const auto& existing)
            {return existing.id==item.id;});
        if(found==out.animations.end())
            out.animations.push_back(std::move(item));
        else if(item.sequence>found->sequence)
            found->sequence=item.sequence;
    }
    if(cursor>=bytes.size())return std::nullopt;
    const std::size_t source_count=bytes[cursor++];
    if(source_count>96U||source_count>(bytes.size()-cursor)/16U)
        return std::nullopt;
    out.sources.reserve(source_count);
    for(std::size_t i=0U;i<source_count;++i)
    {
        std::string id;
        if(!uuid(id))return std::nullopt;
        out.sources.push_back(std::move(id));
    }
    if(cursor>=bytes.size())return std::nullopt;
    const std::size_t event_count=bytes[cursor++];
    if(event_count>32U)return std::nullopt;
    out.physical_events.reserve(event_count);
    for(std::size_t i=0U;i<event_count;++i)
    {
        if(cursor==bytes.size())return std::nullopt;
        const std::size_t length=bytes[cursor++];
        if(length>bytes.size()-cursor)return std::nullopt;
        out.physical_events.emplace_back(
            bytes.begin()+static_cast<std::ptrdiff_t>(cursor),
            bytes.begin()+static_cast<std::ptrdiff_t>(cursor+length));
        cursor+=length;
    }
    if(cursor!=bytes.size())return std::nullopt;
    return out;
}

enum class AvatarMotion : std::uint8_t
{
    idle,walk,run,jump,fly,sit,hover
};
// UUIDs from Firestorm indra/llcharacter/llanimationstates.cpp.
// These are built-in playback state IDENTITIES, not custom animation assets.
[[nodiscard]] inline std::optional<AvatarMotion> detect_avatar_motion(
    const std::vector<AvatarAnimationItem>& anims)
{
    bool walk=false,run=false,jump=false,fly=false,sit=false,hover=false;
    for(const auto& animation:anims)
    {
        const auto& id=animation.id;
        if(id=="6ed24bd8-91aa-4b12-ccc7-c97c857ab4e0"||
           id=="33339176-7ddc-9397-94a4-bf3403cbc8f5")walk=true;
        else if(id=="05ddbff8-aaa9-92a1-2b74-8fe77a29b445"||
                id=="1ab1b236-cd08-21e6-0cbc-0d923fc6eca2")run=true;
        else if(id=="2305bd75-1ca9-b03b-1faa-b176b8a8c49e"||
                id=="7a4e87fe-de39-6fcb-6223-024b00893244"||
                id=="7a17b059-12b2-41b1-570a-186368b6aa6f")jump=true;
        else if(id=="aec4610c-757f-bc4e-c092-c6e9caf18daf"||
                id=="2b5a38b2-5e00-3a97-a495-4c826bc443e6")fly=true;
        else if(id=="1a5fe8ac-a804-8a5d-7cbd-56bd83184568"||
                id=="b1709c8d-ecd3-54a1-4f28-d55ac0840782"||
                id=="1c7600d6-661f-b87b-efe2-d7421eb93c86")sit=true;
        else if(id=="4ae8016b-31b9-03bb-c401-b1ea941db41d")hover=true;
    }
    // The wire carries no animation priority; this only chooses a local
    // built-in locomotion pose. Never claim priority blending of assets.
    if(sit)return AvatarMotion::sit;
    if(jump)return AvatarMotion::jump;
    if(fly)return AvatarMotion::fly;
    if(hover)return AvatarMotion::hover;
    if(run)return AvatarMotion::run;
    if(walk)return AvatarMotion::walk;
    return std::nullopt;
}
[[nodiscard]] inline AvatarMotion select_avatar_motion(
    const std::vector<AvatarAnimationItem>& anims,
    AvatarMotion fallback=AvatarMotion::idle)
{
    return detect_avatar_motion(anims).value_or(fallback);
}
}
