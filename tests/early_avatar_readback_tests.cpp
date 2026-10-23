/* MPL-2.0. FastRez: authenticated Low158 / Low382 early-circuit replay. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/avatar_cof_revision.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace nexverse;
using namespace nexverse::world;
static constexpr std::uint64_t region=0x0000010000000200ULL;
static constexpr auto self="11111111-2222-3333-4444-555555555555";
static constexpr auto session="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
static RegionUpdate area(std::uint64_t handle,bool connected)
{
    return {handle,"Test",{},"127.0.0.1",9000U,"","",
            0U,0U,0U,13U,0.0f,"",256U,256U,connected};
}
static std::vector<std::uint8_t> texture_entry()
{
    // Fully parsed Firestorm TextureEntry, empty valid bake references.
    // Valid Low158 does not imply baked skin or avatar visualization.
    std::vector<std::uint8_t> te(16U,0x11U);
    te.push_back(0U);
    const std::vector<std::uint8_t> fields{
        255U,255U,255U,255U,0U,
        0U,0U,128U,63U,0U,
        0U,0U,128U,63U,0U,
        0U,0U,0U, 0U,0U,0U, 0U,0U,0U,
        0U,0U, 0U,0U, 0U,0U};
    te.insert(te.end(),fields.begin(),fields.end());
    return te;
}
static protocol::AvatarAppearanceMessage appearance(unsigned version=17U)
{
    protocol::AvatarAppearanceMessage a;
    a.avatar_id=self;
    a.visual_params={125U,126U};
    a.texture_entry=texture_entry();
    a.appearance_data={{2U,version,0U},{2U,0U,0U}};
    return a;
}
static protocol::AgentWearablesUpdateMessage worn(std::uint32_t serial)
{
    protocol::AgentWearablesUpdateMessage w;
    w.agent_id=self;
    w.session_id=session;
    w.serial=serial;
    return w;
}
int main()
{
    WorldModel model;
    model.apply_region(area(region,false));
    model.apply_agent({self,session,{},{},false});
    // Both 1-second FastRez responses arrive before the UI thread marks
    // the circuit active. Old behavior DROPPED them irrecoverably.
    if(!model.stage_early_self_appearance(appearance())||
       !model.stage_early_self_wearables(worn(7))||
       model.self_appearance()||model.self_wearables())return 1;
    if(model.flush_early_self_readbacks().appearance||
       model.self_appearance())return 2;
    // No untrusted foreign / trial / invalid-TE update may replace stage.
    auto foreign=appearance();foreign.avatar_id="other";
    if(model.stage_early_self_appearance(foreign))return 3;
    auto trial=appearance();trial.trial=true;
    if(model.stage_early_self_appearance(trial))return 4;
    auto invalid=appearance();invalid.texture_entry={2U,3U,4U};
    if(model.stage_early_self_appearance(invalid))return 5;
    if(model.stage_early_self_appearance(appearance(5)))return 6;
    if(model.stage_early_self_wearables(worn(7))||
       model.stage_early_self_wearables(worn(6)))return 7;
    auto bad_wear=worn(8);bad_wear.session_id="bad";
    if(model.stage_early_self_wearables(bad_wear))return 8;
    if(!model.stage_early_self_wearables(worn(8)))return 9;
    // Main thread activates same region and session: consume ONCE.
    model.apply_region(area(region,true));
    model.apply_agent({self,session,{},{},true});
    const auto released=model.flush_early_self_readbacks();
    if(!released.appearance||!released.wearables||
       !model.self_appearance()||
       protocol::avatar_cof_version_for_display(
           *model.self_appearance())!=17U||
       !model.self_wearables()||model.self_wearables()->serial!=8U)
        return 10;
    const auto repeated=model.flush_early_self_readbacks();
    if(repeated.appearance||repeated.wearables)return 11;
    if(model.stage_early_self_appearance(appearance())||
       model.stage_early_self_wearables(worn(9)))return 12;
    // Old temporary data from a failed circuit must NOT be replayed
    // after a region transition / new user.
    model.apply_region(area(region,false));
    if(!model.stage_early_self_appearance(appearance())||
       !model.stage_early_self_wearables(worn(12)))return 13;
    model.apply_agent({self,"changed-session",{},{},false});
    model.apply_region(area(region,true));
    auto changed=model.flush_early_self_readbacks();
    if(changed.appearance||changed.wearables||
       model.self_appearance()||model.self_wearables())return 14;
    model.apply_region(area(region,false));
    model.apply_agent({self,session,{},{},false});
    if(!model.stage_early_self_appearance(appearance())||
       !model.stage_early_self_wearables(worn(13)))return 15;
    model.begin_region_transition(area(region+1U,false));
    model.apply_agent({self,session,{},{},false});
    model.apply_region(area(region+1U,true));
    changed=model.flush_early_self_readbacks();
    if(changed.appearance||changed.wearables||
       model.self_appearance()||model.self_wearables())return 16;
    // A live disconnect also clears any accepted snapshot.
    model.reset();
    if(model.self_appearance()||model.self_wearables())return 17;
    std::cout<<"OGL_FASTREZ_EARLY_LO158_LO382_SESSION_REGION_REPLAY_PASS\n";
    return 0;
}
