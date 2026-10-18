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
    std::cout<<"OGL_SELF_VALID_TE_PRESERVED_PASS\n";
}
