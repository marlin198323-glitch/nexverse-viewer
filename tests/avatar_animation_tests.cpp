/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/avatar_animation.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

int main()
{
    using namespace nexverse::protocol;
    if(msg_avatar_animation.frequency!=MessageFrequency::high ||
       msg_avatar_animation.value!=20U)return 1;
    const auto sender="11111111-2222-3333-4444-555555555555";
    const auto walk="6ed24bd8-91aa-4b12-ccc7-c97c857ab4e0";
    const auto run="05ddbff8-aaa9-92a1-2b74-8fe77a29b445";
    const auto sit="1a5fe8ac-a804-8a5d-7cbd-56bd83184568";
    auto append_uuid=[](std::vector<std::uint8_t>& data,
                        std::string_view id)
    {
        const auto raw=uuid_to_bytes(id);
        if(!raw)return false;
        data.insert(data.end(),raw->begin(),raw->end());
        return true;
    };
    std::vector<std::uint8_t> data;
    if(!append_uuid(data,sender))return 2;
    data.push_back(3U);
    if(!append_uuid(data,walk))return 3;
    data.insert(data.end(),{42U,0U,0U,0U});
    if(!append_uuid(data,sit))return 4;
    data.insert(data.end(),{255U,255U,255U,255U});
    if(!append_uuid(data,walk))return 5;
    data.insert(data.end(),{43U,0U,0U,0U});
    data.push_back(1U);
    if(!append_uuid(data,"aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"))
        return 6;
    data.push_back(2U); // two variable-length physical-event blocks
    data.insert(data.end(),{3U,1U,2U,3U,0U});
    // Full LLUDP High20 packet (not only directly-invoked payload
    // parser), with the exact wire header frequency and payload.
    const auto encoded=encode_packet(packet_reliable,140U,
        msg_avatar_animation,data);
    const auto wire=decode_lludp_packet(encoded);
    if(!wire.ok() ||
       wire.packet->header.message!=msg_avatar_animation ||
       wire.packet->header.sequence!=140U ||
       wire.packet->payload!=data ||
       encoded.size()<7U || encoded[6U]!=20U)
        return 21;
    const auto parsed=decode_avatar_animation(
        wire.packet->payload);
    if(!parsed || parsed->sender!=sender ||
       parsed->animations.size()!=2U ||
       parsed->animations.front().id!=walk ||
       parsed->animations.front().sequence!=43 ||
       parsed->animations[1U].id!=sit ||
       parsed->animations[1U].sequence!=-1 ||
       parsed->sources.size()!=1U ||
       parsed->physical_events.size()!=2U ||
       parsed->physical_events.front().size()!=3U ||
       !parsed->physical_events[1U].empty())return 7;
    if(select_avatar_motion(parsed->animations)!=AvatarMotion::sit)
        return 8;
    if(select_avatar_motion({{run,2}},AvatarMotion::walk)!=
       AvatarMotion::run ||
       select_avatar_motion({{"33339176-7ddc-9397-94a4-bf3403cbc8f5",5}})!=
       AvatarMotion::walk ||
       select_avatar_motion({{"2305bd75-1ca9-b03b-1faa-b176b8a8c49e",6}})!=
       AvatarMotion::jump ||
       select_avatar_motion({{"aec4610c-757f-bc4e-c092-c6e9caf18daf",7}})!=
       AvatarMotion::fly ||
       select_avatar_motion({{"4ae8016b-31b9-03bb-c401-b1ea941db41d",8}})!=
       AvatarMotion::hover ||
       select_avatar_motion({{"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa",1}},
                            AvatarMotion::run)!=AvatarMotion::run)
        return 9;
    auto invalid=data;
    invalid.pop_back();
    if(decode_avatar_animation(invalid))return 10;
    invalid=data;
    invalid.push_back(0xaaU);
    if(decode_avatar_animation(invalid))return 11;
    invalid=data;
    invalid[16U]=97U; // count > budget
    if(decode_avatar_animation(invalid))return 12;
    invalid=data;
    invalid[16U]=96U; // count <= budget but truncated data
    if(decode_avatar_animation(invalid))return 13;
    invalid=data;
    std::fill(invalid.begin(),invalid.begin()+16,0U);
    if(decode_avatar_animation(invalid))return 14;
    invalid=data;
    // after sender and 3*20 animation items, source-count offset is 77.
    invalid[77U]=97U;
    if(decode_avatar_animation(invalid))return 15;
    invalid=data;
    // source ID 16 bytes, one count byte; event count at 94.
    invalid[94U]=33U;
    if(decode_avatar_animation(invalid))return 16;
    invalid=data;
    invalid[95U]=200U; // oversized variable1 event block
    if(decode_avatar_animation(invalid))return 17;
    invalid=data;
    invalid.resize(16U+1U+3U*20U);
    if(decode_avatar_animation(invalid))return 18;
    // A real valid empty full replacement ends active animations.
    invalid.assign(data.begin(),data.begin()+16U);
    invalid.insert(invalid.end(),{0U,0U,0U});
    const auto empty=decode_avatar_animation(invalid);
    if(!empty||!empty->animations.empty()||!empty->sources.empty()||
       !empty->physical_events.empty())return 19;
    std::cout<<"Firestorm AvatarAnimation High20 full-snapshot tests passed\n";
    return 0;
}
