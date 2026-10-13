/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/object_touch.hpp"
#include "nexverse/protocol/chat_channel_command.hpp"
#include <cstdint>
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    LoginSession login;
    login.agent_id="11111111-2222-3333-4444-555555555555";
    login.session_id="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
    for(const auto phase:{ObjectTouchPhase::begin,ObjectTouchPhase::end})
    {
        const auto wire=encode_object_touch(123U,login,{0x12345678U,phase});
        const auto result=decode_lludp_packet(wire);
        if(!result.ok()||result.packet->header.sequence!=123U||
           !(result.packet->header.flags&packet_reliable)||
           result.packet->header.message!=(phase==ObjectTouchPhase::begin
             ?msg_object_grab:msg_object_degrab))return 1;
        const auto& p=result.packet->payload;
        const auto agent=uuid_to_bytes(login.agent_id);
        const auto session=uuid_to_bytes(login.session_id);
        if(!agent||!session||p.size()!=(phase==ObjectTouchPhase::begin?49U:37U))
            return 2;
        if(!std::equal(agent->begin(),agent->end(),p.begin())||
           !std::equal(session->begin(),session->end(),p.begin()+16))
            return 3;
        if(p[32U]!=0x78U||p[33U]!=0x56U||
           p[34U]!=0x34U||p[35U]!=0x12U)return 4;
        if(p.back()!=0U)return 5;
        if(phase==ObjectTouchPhase::begin)
            for(std::size_t i=36U;i<48U;++i)
                if(p[i]!=0U)return 6;
    }
    if(!encode_object_touch(4U,login,{0U,ObjectTouchPhase::begin}).empty())
        return 7;
    login.agent_id="bad";
    if(!encode_object_touch(4U,login,{15U,ObjectTouchPhase::end}).empty())
        return 8;
    const auto chat=parse_local_chat_command(" /7 Beep ");
    if(!chat.valid||!chat.explicit_channel||chat.channel!=7||
       chat.message!="Beep")return 9;
    const auto negative=parse_local_chat_command("/-1729 Private");
    if(!negative.valid||negative.channel!=-1729||
       negative.message!="Private")return 10;
    const auto ordinary=parse_local_chat_command("Hallo Welt");
    if(!ordinary.valid||ordinary.channel!=0||
       ordinary.explicit_channel)return 11;
    for(const auto invalid:{"","  \t ","/7","/7 ",
        "/99999999999999999999999 Text","/-8","/7\t"})
        if(parse_local_chat_command(invalid).valid)return 12;
    const auto verbatim=parse_local_chat_command("/help");
    if(!verbatim.valid||verbatim.message!="/help"||
       verbatim.channel!=0)return 13;
    std::cout<<"OpenGenesisLINK LLUDP ObjectGrab/DeGrab and Firestorm /7 chat OK\n";
    return 0;
}
