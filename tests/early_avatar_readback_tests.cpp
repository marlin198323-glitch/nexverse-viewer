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

    // A newer DIRECT live Low158 might have arrived after the circuit
    // became ready but before the UI thread flushes an OLD staged packet.
    // Even different COF format (or a legacy unversioned live packet)
    // must not allow a previously buffered body to override that readback.
    WorldModel live;
    live.apply_region(area(region,false));
    live.apply_agent({self,session,{},{},false});
    if(!live.stage_early_self_appearance(appearance(17))||
       !live.stage_early_self_wearables(worn(7)))return 18;
    const auto still_pending=live.early_self_readbacks_pending();
    if(!still_pending[0]||!still_pending[1])return 19;
    live.apply_region(area(region,true));
    live.apply_agent({self,session,{},{},true});
    const auto newer=appearance(42);
    if(!live.apply_self_appearance(newer)||
       !live.apply_self_wearables(worn(9)))return 20;
    const auto late_flush=live.flush_early_self_readbacks();
    if(late_flush.appearance||late_flush.wearables||
       !live.self_appearance()||
       protocol::avatar_cof_version_for_display(
           *live.self_appearance())!=42U||
       !live.self_wearables()||live.self_wearables()->serial!=9U)
        return 21;
    const auto consumed=live.early_self_readbacks_pending();
    if(consumed[0]||consumed[1])return 22;

    WorldModel legacy_direct;
    legacy_direct.apply_region(area(region,false));
    legacy_direct.apply_agent({self,session,{},{},false});
    if(!legacy_direct.stage_early_self_appearance(appearance(17)))
        return 23;
    legacy_direct.apply_region(area(region,true));
    legacy_direct.apply_agent({self,session,{},{},true});
    auto live_legacy=appearance(0);
    live_legacy.appearance_data.clear();
    live_legacy.visual_params={201U,202U};
    if(!legacy_direct.apply_self_appearance(live_legacy))return 24;
    if(legacy_direct.flush_early_self_readbacks().appearance||
       !legacy_direct.self_appearance()||
       legacy_direct.self_appearance()->visual_params!=
           live_legacy.visual_params)return 25;

    // Same RegionHandle is NOT the same simulator socket. A different
    // port on reconnect must not reuse a prior circuit's early outfit.
    WorldModel endpoint;
    endpoint.apply_region(area(region,false));
    endpoint.apply_agent({self,session,{},{},false});
    if(!endpoint.stage_early_self_appearance(appearance(23))||
       !endpoint.stage_early_self_wearables(worn(23)))return 26;
    auto other_socket=area(region,true);
    other_socket.simulator_port=9001U;
    endpoint.apply_region(other_socket);
    endpoint.apply_agent({self,session,{},{},true});
    const auto endpoint_flush=endpoint.flush_early_self_readbacks();
    if(endpoint_flush.appearance||endpoint_flush.wearables||
       endpoint.self_appearance()||endpoint.self_wearables())return 27;
    if(endpoint.early_self_readbacks_pending()[0]||
       endpoint.early_self_readbacks_pending()[1])return 28;
    // A live world on the original socket MUST lose its scene, outfit,
    // self pose and local IDs on a reconnect to another simulator socket
    // EVEN WHEN RegionHandle and agent/session stay identical.
    WorldModel socket_region;
    socket_region.apply_region(area(region,true));
    socket_region.apply_agent({self,session,{},{},true});
    if(!socket_region.apply_self_appearance(appearance(31))||
       !socket_region.apply_self_wearables(worn(31))||
       !socket_region.accept_simulator_self_pose(
           self,session,region,{128.0f,128.0f,28.0f},{}))
        return 29;
    AvatarState remote_avatar;
    remote_avatar.id="other-avatar";
    socket_region.upsert_avatar(std::move(remote_avatar));
    const auto stable_epoch=socket_region.region().generation;
    // Identical known socket is a harmless RegionHandshake refresh.
    socket_region.apply_region(area(region,true));
    if(!socket_region.self_appearance()||
       !socket_region.self_wearables()||
       !socket_region.simulator_self_pose_confirmed()||
       socket_region.avatars().empty()||
       socket_region.region().generation!=stable_epoch)
        return 30;
    // An incomplete interim handshake must neither revoke a healthy
    // circuit nor ERASE its socket identity; otherwise the next known
    // endpoint change could evade the security/appearance reset.
    auto incomplete=area(region,true);
    incomplete.simulator_ip.clear();
    incomplete.simulator_port=0U;
    socket_region.apply_region(incomplete);
    if(socket_region.region().generation!=stable_epoch||
       socket_region.region().simulator_ip!="127.0.0.1"||
       socket_region.region().simulator_port!=9000U||
       !socket_region.self_avatar_renderable()||
       !socket_region.self_appearance())return 35;
    // A genuinely different known socket MUST revoke all old scene data.
    auto switched=area(region,true);
    switched.simulator_port=9001U;
    socket_region.apply_region(switched);
    if(socket_region.region().generation==stable_epoch||
       socket_region.self_appearance()||
       socket_region.self_wearables()||
       socket_region.simulator_self_pose_confirmed()||
       socket_region.self_avatar_renderable()||
       !socket_region.avatars().empty())
        return 31;
    // Never silently revive old COF / pose after a socket switch.
    socket_region.apply_agent({self,session,{},{},true});
    if(socket_region.self_appearance()||
       socket_region.self_avatar_renderable())return 32;
    if(!socket_region.apply_self_appearance(appearance(32))||
       !socket_region.accept_simulator_self_pose(
           self,session,region,{127.0f,126.0f,33.0f},{})||
       !socket_region.self_avatar_renderable())
        return 33;
    // IP-only change with an unchanged UDP port must be equally fatal
    // for old simulator state, including local prim references.
    const auto changed_epoch=socket_region.region().generation;
    switched.simulator_ip="127.0.0.2";
    socket_region.apply_region(switched);
    if(socket_region.region().generation==changed_epoch||
       socket_region.self_avatar_renderable()||
       socket_region.self_appearance())return 34;
    std::cout<<"OGL_FASTREZ_EARLY_LO158_LO382_SESSION_REGION_REPLAY_PASS\\n";
    return 0;
}
