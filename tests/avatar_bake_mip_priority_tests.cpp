/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/avatar_bake_mip_priority.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace nexverse;
int main()
{
    std::unordered_map<std::string,std::uint8_t> mips{
        {"head",3U},{"upper",1U},{"lower",0U},{"world",3U}};
    const std::array<std::string,3U> ids{"head","upper","lower"};
    if(assets::pin_self_avatar_body_full_mip(mips,ids,false)!=0U||
       mips["head"]!=3U)return 1;
    if(assets::pin_self_avatar_body_full_mip(mips,ids,true)!=2U||
       mips["head"]!=0U||mips["upper"]!=0U||
       mips["lower"]!=0U||mips["world"]!=3U)return 2;
    if(assets::pin_self_avatar_body_full_mip(mips,ids,true)!=0U)
       return 3;
    const std::array<std::string,3U> not_present{
        "missing","00000000-0000-0000-0000-000000000000",
        std::string{protocol::baked_placeholders[0]}};
    if(assets::pin_self_avatar_body_full_mip(mips,not_present,true)!=0U||
       mips.contains("missing")||
       mips.contains(not_present[1])||
       mips.contains(not_present[2]))return 4;
    std::cout<<"Self Avatar true-mip0 request priority, bounded PASS\n";
    return 0;
}
