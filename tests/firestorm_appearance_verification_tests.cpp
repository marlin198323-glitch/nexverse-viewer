/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/firestorm_appearance_verification.hpp"
#include <chrono>
#include <iostream>
using namespace nexverse::protocol;
int main()
{
    constexpr auto agent="11111111-1111-1111-1111-111111111111";
    constexpr auto other="22222222-2222-2222-2222-222222222222";
    using Clock=CofAppearanceVerifier::Clock;
    const auto t=Clock::time_point{std::chrono::seconds{1000}};
    CofAppearanceVerifier proof;
    if(proof.phase()!=CofAppearancePhase::idle)return 1;
    proof.begin(0U,4U,t);
    if(proof.phase()!=CofAppearancePhase::idle)return 2;
    proof.begin(8U,17U,t);
    if(proof.phase()!=CofAppearancePhase::cof_pending)return 3;
    if(proof.confirm_cof(9U,42U,t)||proof.confirm_cof(8U,0U,t)||
       proof.confirm_cof(8U,UINT32_MAX,t))return 4;
    AvatarAppearanceMessage a;
    a.avatar_id=agent;
    a.texture_entry={0xaaU,0xbbU};
    a.visual_params={0xffU};
    a.appearance_data.push_back({1U,42U,0U});
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{2});
    if(proof.phase()!=CofAppearancePhase::cof_pending)return 5;
    if(!proof.confirm_cof(8U,42U,t+std::chrono::seconds{3})||
       proof.target_version()!=42U)return 6;
    proof.observe(8U,17U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_pending)return 7;
    proof.observe(8U,18U,other,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_pending)return 8;
    a.trial=true;
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_pending)return 9;
    a.trial=false;a.appearance_data.clear();
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_pending)return 10;
    a.appearance_data.push_back({1U,41U,0U});
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_stale)return 11;
    a.appearance_data[0].cof_version=43U;
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_newer)return 12;
    a.appearance_data[0].cof_version=42U;
    a.texture_entry.clear();
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_stale)return 13;
    a.texture_entry={0xaaU,0xbbU};
    proof.observe(8U,18U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_matched||
       proof.active()||proof.status().find("Bake")==
         std::string::npos)return 14;
    proof.begin(9U,20U,t);
    if(!proof.confirm_cof(9U,44U,t+std::chrono::seconds{1}))return 15;
    proof.observe(9U,21U,agent,a,t+std::chrono::seconds{32});
    if(proof.phase()!=CofAppearancePhase::expired)return 16;
    proof.begin(10U,21U,t);
    proof.observe(10U,22U,agent,a,t+std::chrono::seconds{121});
    if(proof.phase()!=CofAppearancePhase::expired)return 17;
    proof.begin(11U,22U,t);
    if(!proof.confirm_cof(11U,42U,t+std::chrono::seconds{2}))return 18;
    proof.observe(12U,23U,agent,a,t+std::chrono::seconds{3});
    if(proof.phase()!=CofAppearancePhase::region_invalid)return 19;
    proof.begin(13U,23U,t);
    if(!proof.confirm_cof(13U,42U,t+std::chrono::seconds{3}))return 20;
    proof.observe(13U,24U,agent,a,t+std::chrono::seconds{4});
    if(proof.phase()!=CofAppearancePhase::appearance_matched)return 21;
    std::cout<<"Firestorm AvatarAppearance Low158 CofVersion plus new authenticated self-packet serial, trial/epoch/stale/newer/expiry and bake-not-proven OK\n";
    return 0;
}
