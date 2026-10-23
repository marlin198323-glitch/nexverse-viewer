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
    // Alpha.1035-1044: full-size server bake is denied by the SAFE
    // transient GPU peak, but the already decoded RGBA can fit at mip2
    // with no second HTTP request and no extra GPU-memory allocation.
    constexpr std::size_t mib=1024U*1024U;
    const std::vector<std::string> preferred{"head","world"};
    const std::unordered_map<std::string,std::size_t> resident{
        {"world",100U*mib}};
    const auto extra=assets::choose_self_avatar_budget_mip_rescue(
        4096U,4096U,0U,100U*mib,260U*mib,384U*mib,256U*mib,
        preferred,resident,"head");
    if(!extra||*extra!=2U)return 11;
    // A small image already fits: never reduce visual quality.
    if(assets::choose_self_avatar_budget_mip_rescue(
        256U,256U,0U,100U*mib,260U*mib,384U*mib,256U*mib,
        preferred,resident,"head"))return 12;
    // If even mip3 cannot be transactionally staged, defer safely;
    // never bypass the existing staging/byte admission.
    if(assets::choose_self_avatar_budget_mip_rescue(
        4096U,4096U,0U,370U*mib,20U*mib,384U*mib,256U*mib,
        preferred,resident,"head"))return 13;
    // Only the remaining part of the three-step ladder is permitted.
    if(assets::choose_self_avatar_budget_mip_rescue(
        4096U,4096U,3U,100U*mib,260U*mib,384U*mib,256U*mib,
        preferred,resident,"head"))return 14;
    if(assets::choose_self_avatar_budget_mip_rescue(
        4096U,4096U,0U,100U*mib,260U*mib,384U*mib,256U*mib,
        preferred,resident,""))return 15;
    // Never poison next login's or region's full-quality desire.
    using Clock=assets::SelfAvatarMipRescueCooldown::Clock;
    const auto now=Clock::time_point{}+std::chrono::hours{2};
    assets::SelfAvatarMipRescueCooldown floor;
    floor.enter(41U);
    floor.admitted("head",2U,now);
    if(floor.minimum_mip("head",now)!=2U||
       floor.minimum_mip("world",now)!=0U||
       floor.minimum_mip("head",now+std::chrono::seconds{59})!=2U||
       floor.minimum_mip("head",now+std::chrono::seconds{60})!=0U)
        return 16;
    floor.enter(41U);
    if(floor.minimum_mip("head",now)!=2U)return 17;
    floor.enter(42U);
    if(floor.minimum_mip("head",now)!=0U)return 18;
    floor.admitted("head",0U,now);
    if(floor.minimum_mip("head",now)!=0U)return 19;
    // No duplicate outgoing request while the lowered mip remains
    // resident and the cooldown is active.
    floor.admitted("head",3U,now);
    if(std::max(0U,static_cast<unsigned>(
           floor.minimum_mip("head",now)))!=3U)return 20;
    std::cout<<"Self Avatar true-mip0 request priority, bounded PASS\n";
    return 0;
}
