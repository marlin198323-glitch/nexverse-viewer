/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/region_handshake_appearance.hpp"
#include "nexverse/ui/avatar_appearance_runtime.hpp"
#include <chrono>
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    SelfAppearanceHandshakeReceipt receipt{};
    if(receipt.sent||receipt.acknowledged||
       receipt.note_ack(111U,true))return 1;
    receipt.note_sent(111U);
    if(!receipt.sent||receipt.acknowledged||
       receipt.note_ack(111U,false)||
       receipt.note_ack(112U,true))return 2;
    if(!receipt.note_ack(111U,true)||!receipt.acknowledged||
       receipt.note_ack(111U,true))return 3;
    // A new handshake/region invalidates the old pending sequence.
    receipt.note_sent(113U);
    if(receipt.acknowledged||
       receipt.note_ack(111U,true)||
       receipt.note_ack(112U,true)||
       !receipt.note_ack(113U,true))return 4;
    receipt.reset();
    if(receipt.sent||receipt.acknowledged||
       receipt.note_ack(113U,true))return 5;
    receipt.note_sent(0U);
    if(receipt.sent||receipt.note_ack(0U,true))return 6;
    // New, DISTINCT RegionHandshake during initial connection:
    // once SSA bit2 ACK is VERIFIED, a second negotiation cannot
    // rewind the proof solely because AgentMovementComplete is pending.
    receipt.reset();
    receipt.note_sent(120U);
    if(!receipt.note_ack(120U,true))return 12;
    receipt.note_refresh(121U);
    if(!receipt.acknowledged||receipt.sequence!=120U||
       receipt.note_ack(121U,true))return 13;
    // Before ANY receipt is ACKed, the newest sent reply remains
    // the only valid pending ACK (not an untracked old sequence).
    receipt.reset();
    receipt.note_sent(130U);
    receipt.note_refresh(131U);
    if(receipt.sequence!=131U||receipt.acknowledged||
       receipt.note_ack(130U,true)||
       !receipt.note_ack(131U,true))return 14;
    // A fresh login circuit always explicitly resets the old proof.
    receipt.reset();
    if(receipt.acknowledged||receipt.sent||
       receipt.note_ack(120U,true))return 15;

    using nexverse::ui::describe_self_appearance_handshake;
    if(describe_self_appearance_handshake(true,true,false,false).find(
         "ACK ausstehend")==std::string::npos)return 7;
    if(describe_self_appearance_handshake(true,true,true,false,
          std::chrono::seconds{14}).find("fehlt")==std::string::npos)return 8;
    if(describe_self_appearance_handshake(true,true,true,false,
          std::chrono::seconds{15}).find("15s")==std::string::npos)return 9;
    if(describe_self_appearance_handshake(true,true,false,true).find(
          "empfangen")==std::string::npos)return 10;
    if(describe_self_appearance_handshake(false,true,true,false).find(
          "ausstehend")==std::string::npos)return 11;
    std::cout<<"SSA current reliable sequence ACK and truthful live receipt stages PASS\n";
    return 0;
}
