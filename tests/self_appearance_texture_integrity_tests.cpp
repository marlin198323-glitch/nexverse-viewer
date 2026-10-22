/* MPL-2.0: actual TE parser protects previously verified self skin. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
using namespace nexverse;
int main()
{
    world::WorldModel model;
    model.apply_region({0x0000010000000200ULL,"Test",{},
        "127.0.0.1",9000U,"","",0U,0U,0U,13U,0.0f,"",
        256U,256U,true});
    model.apply_agent({"11111111-2222-3333-4444-555555555555",
        "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",{},{},true});
    protocol::AvatarAppearanceMessage appearance;
    appearance.avatar_id=model.agent().agent_id;
    appearance.texture_entry=std::vector<std::uint8_t>(16U,0x11U);
    const std::vector<std::uint8_t> fields{
        0U, // no UUID face overrides
        255U,255U,255U,255U,0U, // default white, no color overrides
        0U,0U,128U,63U,0U, // repeat U = 1.0
        0U,0U,128U,63U,0U, // repeat V = 1.0
        0U,0U,0U, // offset U and mask
        0U,0U,0U, // offset V and mask
        0U,0U,0U, // rotation and mask
        0U,0U, // material
        0U,0U, // media
        0U,0U // glow
    };
    appearance.texture_entry.insert(appearance.texture_entry.end(),
        fields.begin(),fields.end());
    appearance.visual_params={120U,130U,140U};
    if(!protocol::valid_self_appearance_texture_entry(
           appearance.texture_entry))return 1;
    if(!model.apply_self_appearance(appearance))return 2;
    const auto authentic=appearance.texture_entry;
    auto invalid=appearance;
    // Nonempty but truncated core block: never replace real skin.
    invalid.texture_entry={0xAAU,0xBBU,0xCCU};
    if(protocol::valid_self_appearance_texture_entry(invalid.texture_entry)||
       model.apply_self_appearance(invalid))return 3;
    if(model.self_appearance()->texture_entry!=authentic)return 4;
    // A later truncated default UUID with 16 bytes is also invalid.
    invalid.texture_entry=std::vector<std::uint8_t>(16U,0x11U);
    if(protocol::valid_self_appearance_texture_entry(invalid.texture_entry)||
       model.apply_self_appearance(invalid))return 5;
    if(model.self_appearance()->texture_entry!=authentic)return 6;
    // Trial and missing visual params do not replace existing skin.
    invalid=appearance;invalid.trial=true;
    if(model.apply_self_appearance(invalid))return 7;
    invalid=appearance;invalid.visual_params.clear();
    if(model.apply_self_appearance(invalid))return 8;
    if(model.self_appearance()->texture_entry!=authentic)return 9;
    // A new valid appearance is still accepted, and not blocked by
    // baked-slot placeholders or absent nonessential optional tails.
    auto updated=appearance;updated.visual_params={1U,2U,3U};
    if(!model.apply_self_appearance(updated)||
       model.self_appearance()->visual_params!=updated.visual_params)return 10;
    // A verified current modern COF revision protects skin data from
    // delayed but otherwise well-formed earlier Low158 packets.
    auto current=updated;
    current.appearance_data={{2U,900U,0U}};
    if(!model.apply_self_appearance(current)||
       model.stale_self_cof_appearances()!=0U)return 11;
    auto stale=current;
    stale.appearance_data.back().cof_version=899U;
    stale.visual_params={222U,223U,224U};
    if(model.apply_self_appearance(stale)||
       model.self_appearance()->visual_params!=current.visual_params||
       model.stale_self_cof_appearances()!=1U)return 12;
    // No COF version in legacy OpenGenesisLINK reply: still accepted,
    // but the modern high-watermark remains sticky in this login.
    auto legacy=updated;
    legacy.appearance_data.clear();
    legacy.visual_params={99U,98U,97U};
    if(!model.apply_self_appearance(legacy)||
       !model.self_appearance()->appearance_data.empty()||
       model.stale_self_cof_appearances()!=1U)return 13;
    if(model.apply_self_appearance(stale)||
       model.self_appearance()->visual_params!=legacy.visual_params||
       model.stale_self_cof_appearances()!=2U)return 14;
    // Firestorm can include 2+ AppearanceData entries; a trailing
    // unknown COF=0 is NOT an authorization to replay old skin.
    auto padded_stale=stale;
    padded_stale.appearance_data.push_back({2U,0U,0U});
    padded_stale.visual_params={231U,232U};
    if(model.apply_self_appearance(padded_stale)||
       model.self_appearance()->visual_params!=legacy.visual_params||
       model.stale_self_cof_appearances()!=3U)
        return 17;
    auto padded_new=current;
    padded_new.appearance_data.back().cof_version=901U;
    padded_new.appearance_data.push_back({2U,0U,0U});
    padded_new.visual_params={241U,242U};
    if(!model.apply_self_appearance(padded_new)||
       model.self_appearance()->visual_params!=padded_new.visual_params||
       model.stale_self_cof_appearances()!=3U)
        return 18;
    // Another delayed packet with COF 900 cannot displace 901 after
    // the trailing 0 row. Count=0 legacy remains accepted earlier.
    auto replay=current;
    replay.appearance_data.push_back({2U,0U,0U});
    if(model.apply_self_appearance(replay)||
       model.self_appearance()->visual_params!=padded_new.visual_params||
       model.stale_self_cof_appearances()!=4U)return 19;
    // A row with only zero COF numbers has no trustworthy revision:
    // it must neither erase the watermark nor block older OpenSim.
    auto unknown=legacy;
    unknown.appearance_data={{2U,0U,0U},{2U,0U,0U}};
    if(!model.apply_self_appearance(unknown)||
       model.stale_self_cof_appearances()!=4U)return 20;
    if(model.apply_self_appearance(replay)||
       model.stale_self_cof_appearances()!=5U)return 21;
    // New authenticated session establishes independent COF numbering.
    model.apply_agent({"11111111-2222-3333-4444-555555555555",
        "bbbbbbbb-bbbb-cccc-dddd-eeeeeeeeeeee",{},{},true});
    if(model.stale_self_cof_appearances()!=0U||
       model.self_appearance()||
       !model.apply_self_appearance(stale)||
       model.self_appearance()->appearance_data.back().cof_version!=899U)
        return 15;
    model.apply_region({0x0000010000000300ULL,"Another",{},
        "127.0.0.1",9000U,"","",0U,0U,0U,13U,0.0f,"",
        256U,256U,true});
    if(model.stale_self_cof_appearances()!=0U||
       model.self_appearance()||
       !model.apply_self_appearance(current))return 16;
    std::cout<<"OGL_SELF_VALID_TE_PRESERVED_PASS\n";
}
