/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/lludp.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>
namespace nexverse::protocol
{
// Wire protocol verified from Firestorm 7.2.4 message_template.msg
// and llviewerinventory.cpp; OpenGenesisLINK LLClientView handlers.
// This layer NEVER schedules mutations on its own.
inline constexpr MessageId msg_link_inventory_item{MessageFrequency::low,426U};
inline constexpr MessageId msg_remove_inventory_link{MessageFrequency::low,270U};
struct CofLinkCommand
{
    std::string folder_id,linked_item_id,name,description;
    std::uint32_t callback_id=0U;
    std::int8_t inventory_type=0;
};
struct CofUnlinkCommand
{
    std::string link_item_id;
};
// Strictly UUID-valid and bounded, and only LINK(24). The caller
// must have independently verified ownership/COF/consent.
[[nodiscard]] inline std::vector<std::uint8_t> encode_cof_link(
    std::uint32_t sequence,const LoginSession& session,
    const CofLinkCommand& command)
{
    const auto agent=uuid_to_bytes(session.agent_id);
    const auto sid=uuid_to_bytes(session.session_id);
    const auto folder=uuid_to_bytes(command.folder_id);
    const auto linked=uuid_to_bytes(command.linked_item_id);
    const auto good_text=[](const std::string& s) {
        return !s.empty()&&s.size()<=120U&&
            std::all_of(s.begin(),s.end(),[](unsigned char c) {
                return c>=32U&&c!=127U;
            });
    };
    const auto nonzero=[](const auto& x) {
        return x&&std::any_of(x->begin(),x->end(),
            [](std::uint8_t b){return b!=0U;});
    };
    if(!nonzero(agent)||!nonzero(sid)||!nonzero(folder)||
       !nonzero(linked)||folder==linked||
       command.callback_id==0U||
       !good_text(command.name)||!good_text(command.description)||
       command.inventory_type<0)return {};
    std::vector<std::uint8_t> bytes;
    const auto append_id=[&](const auto& id) {
        bytes.insert(bytes.end(),id->begin(),id->end());
    };
    const auto append_u32=[&](std::uint32_t value) {
        for(unsigned i=0;i<4;++i)
            bytes.push_back(static_cast<std::uint8_t>(value>>(8U*i)));
    };
    const auto append_string=[&](const std::string& str) {
        // LLMessageSystem addStringFast to Variable 1 is NUL-terminated.
        bytes.push_back(static_cast<std::uint8_t>(str.size()+1U));
        bytes.insert(bytes.end(),str.begin(),str.end());
        bytes.push_back(0U);
    };
    append_id(agent);append_id(sid);
    append_u32(command.callback_id);
    append_id(folder);
    bytes.insert(bytes.end(),16U,0U); // Firestorm null TransactionID
    append_id(linked);
    bytes.push_back(24U); // LLAssetType::AT_LINK
    bytes.push_back(static_cast<std::uint8_t>(command.inventory_type));
    append_string(command.name);
    append_string(command.description);
    return encode_packet(packet_reliable,sequence,msg_link_inventory_item,bytes);
}
[[nodiscard]] inline std::vector<std::uint8_t> encode_cof_unlink(
    std::uint32_t sequence,const LoginSession& session,
    const CofUnlinkCommand& command)
{
    const auto agent=uuid_to_bytes(session.agent_id);
    const auto sid=uuid_to_bytes(session.session_id);
    const auto item=uuid_to_bytes(command.link_item_id);
    const auto nonzero=[](const auto& x) {
        return x&&std::any_of(x->begin(),x->end(),
            [](std::uint8_t b){return b!=0U;});
    };
    if(!nonzero(agent)||!nonzero(sid)||!nonzero(item))return {};
    std::vector<std::uint8_t> body;
    body.insert(body.end(),agent->begin(),agent->end());
    body.insert(body.end(),sid->begin(),sid->end());
    body.push_back(1U); // InventoryData variable one ItemID
    body.insert(body.end(),item->begin(),item->end());
    return encode_packet(packet_reliable,sequence,msg_remove_inventory_link,body);
}
}
