/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/animation_asset_transfer.hpp"
#include <bit>
#include <cstdint>
#include <iostream>
#include <vector>
#include <array>
using B=std::vector<std::uint8_t>;
namespace
{
void u16(B& b,std::uint16_t n)
{b.push_back(static_cast<std::uint8_t>(n));b.push_back(static_cast<std::uint8_t>(n>>8U));}
void u32(B& b,std::uint32_t n)
{for(int i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(n>>(8*i)));}
void f32(B& b,float n){u32(b,std::bit_cast<std::uint32_t>(n));}
void uuid(B& b,const std::string& name)
{
    const auto bytes=nexverse::protocol::uuid_to_bytes(name);
    if(bytes)b.insert(b.end(),bytes->begin(),bytes->end());
}
B clip()
{
    B b;
    u16(b,1U);u16(b,0U);u32(b,3U);f32(b,2.0f);
    b.push_back(0U); // Empty emote C-string
    f32(b,0.0f);f32(b,2.0f);u32(b,0U); // loopIn/out/flag
    f32(b,0.0f);f32(b,0.0f);u32(b,0U);u32(b,1U);
    for(const char c:std::string{"mPelvis"})b.push_back(static_cast<std::uint8_t>(c));
    b.push_back(0U);
    u32(b,3U);u32(b,1U);
    u16(b,0U);u16(b,32768U);u16(b,32768U);u16(b,32768U);
    u32(b,0U);u32(b,0U);
    return b;
}
B make_info(std::string tid,std::string aid,int size,std::uint32_t type=20U)
{
    B b;uuid(b,tid);u32(b,2U);u32(b,0U);u32(b,0U);
    u32(b,static_cast<std::uint32_t>(size));u16(b,20U);
    uuid(b,aid);u32(b,type);
    return b;
}
B make_packet(std::string tid,std::uint32_t index,bool last,
              std::span<const std::uint8_t> body)
{
    B b;uuid(b,tid);u32(b,2U);u32(b,index);u32(b,last?1U:0U);
    u16(b,static_cast<std::uint16_t>(body.size()));
    b.insert(b.end(),body.begin(),body.end());
    return b;
}
}
int main()
{
    using namespace nexverse::protocol;
    const std::string aid="11111111-2222-3333-4444-555555555555";
    const std::string tid="aaaaaaaa-bbbb-4ccc-8ddd-eeeeeeeeeeee";
    const AnimationAssetRequest request{aid,tid};
    const auto wire=encode_animation_asset_transfer(11U,request);
    const auto decoded=decode_lludp_packet(wire);
    if(!decoded.ok()||decoded.packet->header.message!=msg_transfer_request||
       !(decoded.packet->header.flags&packet_reliable)||
       decoded.packet->payload.size()!=50U||
       transfer_detail::uuid(decoded.packet->payload)!=tid||
       transfer_detail::i32(decoded.packet->payload,16U)!=2||
       transfer_detail::i32(decoded.packet->payload,20U)!=2||
       decoded.packet->payload[28U]!=20U||
       transfer_detail::uuid(std::span{decoded.packet->payload}.subspan(30U))!=aid||
       transfer_detail::i32(decoded.packet->payload,46U)!=20)
        return 1;
    if(!encode_animation_asset_transfer(
       4U,{aid,"invalid"}).empty())return 2;
    const auto id2=random_animation_transfer_id();
    if(id2==aid||id2==tid||!uuid_to_bytes(id2))return 3;
    const B binary=clip();
    if(!nexverse::assets::decode_firestorm_keyframe(binary))return 4;
    const auto info_wire=encode_packet(0U,12U,
        msg_transfer_info,make_info(tid,aid,static_cast<int>(binary.size())));
    const auto info_packet=decode_lludp_packet(info_wire);
    if(!info_packet.ok()||info_packet.packet->header.message!=msg_transfer_info)
        return 5;
    const auto info=decode_animation_transfer_info(info_packet.packet->payload);
    if(!info||info->transfer_id!=tid||info->asset_id!=aid||
       info->size!=static_cast<int>(binary.size())||info->type!=20)
        return 6;
    // Real OpenGenesisLINK LLClientView.SendAssetNotFound uses status
    // -2, size 0, original 20-byte Params and no TransferPacket.
    auto notfound=make_info(tid,aid,0);
    notfound[24U]=0xfeU;notfound[25U]=0xffU;
    notfound[26U]=0xffU;notfound[27U]=0xffU;
    const auto denied=decode_animation_transfer_info(notfound);
    if(!denied||denied->status!=-2||denied->size!=0)
        return 22;
    AnimationTransferInbox denied_inbox;
    denied_inbox.reset(16U);
    if(!denied_inbox.begin(16U,request))return 23;
    denied_inbox.on_info(*denied);
    const auto denied_result=denied_inbox.pop();
    if(!denied_result||denied_result->clip||
       denied_result->error!="Asset absent on simulator"||
       denied_inbox.has_pending(aid))
        return 24;
    auto badinfo=make_info(tid,aid,static_cast<int>(binary.size()),10U);
    if(decode_animation_transfer_info(badinfo))return 7;
    badinfo=make_info(tid,aid,static_cast<int>(binary.size()));
    badinfo.push_back(0U);
    if(decode_animation_transfer_info(badinfo))return 8;
    const std::size_t split=binary.size()/2U;
    const auto wire0=encode_packet(0U,13U,msg_transfer_packet,
        make_packet(tid,0U,false,std::span{binary}.first(split)));
    const auto wire1=encode_packet(0U,14U,msg_transfer_packet,
        make_packet(tid,1U,true,std::span{binary}.subspan(split)));
    const auto packet0=decode_lludp_packet(wire0);
    const auto packet1=decode_lludp_packet(wire1);
    if(!packet0.ok()||!packet1.ok())return 9;
    const auto a=decode_animation_transfer_packet(packet0.packet->payload);
    const auto b=decode_animation_transfer_packet(packet1.packet->payload);
    if(!a||!b||a->index!=0U||a->last||b->index!=1U||!b->last)
    {
        std::cerr<<"Part invalid: a="<<a.has_value()<<" b="<<b.has_value()
                 <<" size0="<<packet0.packet->payload.size()
                 <<" size1="<<packet1.packet->payload.size()<<"\n";
        return 10;
    }
    AnimationTransferInbox inbox;
    inbox.reset(7U);
    if(!inbox.begin(7U,request)||inbox.begin(7U,request)||
       inbox.begin(8U,{aid,id2})||!inbox.has_pending(aid))
        return 11;
    // Out of order, final chunk first, duplicate identical, Info last.
    inbox.on_packet(*b);
    inbox.on_packet(*b);
    inbox.on_packet(*a);
    if(inbox.pop())return 12;
    inbox.on_info(*info);
    auto completed=inbox.pop();
    if(!completed||!completed->clip||!completed->error.empty()||
       completed->asset_id!=aid||completed->clip->joints.size()!=1U||
       completed->clip->joints[0].name!="mPelvis"||
       inbox.pending_count()!=0U)return 13;
    // Mixing a different asset UUID must fail closed.
    const AnimationAssetRequest request2{"ffffffff-1111-2222-3333-444444444444",id2};
    if(!inbox.begin(7U,request2))return 14;
    inbox.on_info(*info); // Old transfer id ignored.
    auto mismatch=*info;
    mismatch.transfer_id=id2; // Requested different asset.
    inbox.on_info(mismatch);
    completed=inbox.pop();
    if(!completed||completed->clip||completed->error.empty())
        return 15;
    // Conflicting duplicates are a protocol failure, not silent overwrite.
    inbox.reset(10U);
    if(!inbox.begin(10U,request))return 16;
    inbox.on_packet(*a);
    auto conflict=*a;conflict.data[0U]^=0x55U;
    inbox.on_packet(conflict);
    completed=inbox.pop();
    if(!completed||completed->clip||
       completed->error.find("duplicate")==std::string::npos)
        return 17;
    inbox.reset(11U);
    if(inbox.has_pending(aid)||inbox.pop())return 18;
    auto badpacket=make_packet(tid,0U,false,std::span{binary});
    badpacket[16U]=7U; // channel type mismatch
    if(decode_animation_transfer_packet(badpacket))return 19;
    badpacket=make_packet(tid,0U,false,std::span{binary});
    badpacket[28U]^=0x80U;
    if(decode_animation_transfer_packet(badpacket))return 20;
    if(decode_animation_transfer_packet(B(3U,0U)))return 21;
    std::cout<<"Firestorm/OpenSim TransferRequest Low153, Info Low154, "
                "Packet High17, bounded decoded Keyframe GPU input OK\n";
    return 0;
}
