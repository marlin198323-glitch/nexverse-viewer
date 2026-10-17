/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/avatar_appearance_runtime.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::ui;
    AvatarAppearanceRuntime state{};
    if(describe_appearance_runtime(state,false)!=
       "Appearance: kein Self-Paket vom Simulator")return 1;
    state.other_avatar=5;
    if(describe_appearance_runtime(state,false).find("fremde Avatare")==
        std::string::npos)return 2;
    state.ignored_region=1;
    if(describe_appearance_runtime(state,false).find("Regionwechsel")==
        std::string::npos)return 3;
    state.malformed=2;
    if(describe_appearance_runtime(state,false).find("ungueltige")==
        std::string::npos)return 4;
    if(describe_appearance_runtime(state,true).find("bestaetigt")==
        std::string::npos)return 5;
    if(describe_self_appearance_handshake(false,true,true).find(
        "ausstehend")==std::string::npos)return 6;
    if(describe_self_appearance_handshake(true,false,false).find(
        "nicht gesendet")==std::string::npos)return 7;
    if(describe_self_appearance_handshake(true,true,false).find(
        "fehlt")==std::string::npos)return 8;
    if(describe_self_appearance_handshake(true,true,true).find(
        "empfangen")==std::string::npos)return 9;
    if(describe_self_appearance_handshake(true,true,true,false).find(
        "quittiert")==std::string::npos)return 10;
    if(describe_self_appearance_handshake(true,true,false,false).find(
        "ACK ausstehend")==std::string::npos)return 11;
    if(describe_self_appearance_handshake(true,true,false,true).find(
        "empfangen")==std::string::npos)return 12;
    if(describe_self_appearance_handshake(true,true,true,false,
         std::chrono::seconds{14}).find("fehlt")==std::string::npos)
        return 13;
    if(describe_self_appearance_handshake(true,true,true,false,
         std::chrono::seconds{15}).find("15s")==std::string::npos)
        return 14;
    if(describe_self_appearance_handshake(true,true,true,true,
         std::chrono::seconds{100}).find("empfangen")==std::string::npos)
        return 15;
    std::cout<<"Authenticated appearance reception stages are truthful and identity-free PASS\n";
    return 0;
}
