/* MPL-2.0: no stale avatars, attachments, terrain or prim meshes on a
 * direct RegionUpdate / LLUDP circuit drop / re-authenticated session. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/world/attachment_state.hpp"
#include <iostream>
using namespace nexverse;
static world::RegionUpdate region(std::uint64_t handle,bool connected)
{
    return {handle,"NexVerse",{128.0f,128.0f,25.0f},
        "192.0.2.1",9000U,"https://caps.invalid/seed",
        "11111111-2222-3333-4444-555555555555",
        0x10U,0U,0U,13U,20.0f,"OpenGenesisLINK",256U,256U,
        connected};
}
static void populate(world::WorldModel& w)
{
    world::ObjectState old;
    old.local_id=42U;
    old.id="old-avatar-attachment";
    old.pcode=protocol::pcode_prim;
    old.parent_id=51U;
    old.state=0x10U;
    old.sculpt_asset_id="old-mesh";
    old.sculpt_type=0x05U;
    old.texture_entry={1U,2U,3U};
    w.upsert_object(old);
    world::AvatarState other;
    other.id="someone-in-old-region";
    other.local_id=60U;
    w.upsert_avatar(other);
    world::TerrainState terrain;
    terrain.available=true;
    terrain.valid_patch_count=1U;
    terrain.valid_patches[0]=true;
    w.set_terrain(terrain);
    world::ParcelState parcel;
    parcel.available=true;
    parcel.name="Old parcel";
    w.set_parcel(parcel);
    world::EnvironmentState environment;
    environment.available=true;
    environment.sky_name="Old sky";
    w.set_environment(environment);
}
static bool clean(const world::WorldModel& w)
{
    return w.objects().empty()&&w.avatars().empty()&&
        w.terrain().valid_patch_count==0U&&!w.terrain().available&&
        !w.parcel().available&&!w.environment().available&&
        !w.self_appearance()&&!w.self_object_bakes()&&
        !w.self_animations()&&
        world::infer_avatar_attachment_points(
            w.objects(),51U).empty();
}
int main()
{
    world::WorldModel w;
    constexpr std::uint64_t r1=0x0000010000000200ULL;
    constexpr std::uint64_t r2=0x0000020000000300ULL;
    w.apply_region(region(r1,true));
    w.apply_agent({"own-agent","first-session",{1.0f,2.0f,3.0f},{},true});
    populate(w);
    const auto first_revision=w.object_revision();
    const auto first_gen=w.region().generation;
    if(first_revision!=1U||!w.scene_ready()||
       world::infer_avatar_attachment_points(
           w.objects(),51U).size()!=1U)return 1;
    // Same-region AgentMovementComplete and RegionHandshake are merely
    // metadata refreshes: no world flicker, no skin or scene eviction.
    auto same=region(r1,true);
    same.name="New region title";
    same.water_height=24.0f;
    w.apply_region(same);
    if(w.object_revision()!=first_revision||
       w.region().generation!=first_gen||
       w.objects().size()!=1U||w.avatars().size()!=1U||
       !w.terrain().available||!w.parcel().available||
       !w.environment().available)return 2;
    // Real application updates Region.connected=false from circuit state.
    w.apply_region(region(r1,false));
    if(!clean(w)||w.region().generation!=first_gen+1U||
       w.object_revision()!=first_revision+1U||
       w.agent().present||w.scene_ready())return 3;
    // Repeated disconnect frames must not keep changing scene revision
    // or session generation, as the UI polls circuit state each frame.
    const auto cleared_revision=w.object_revision();
    const auto cleared_gen=w.region().generation;
    w.apply_region(region(r1,false));
    if(w.object_revision()!=cleared_revision||
       w.region().generation!=cleared_gen)return 4;
    // Reconnecting must never restore previous objects. A valid complete
    // ObjectUpdate is required before the prim can render.
    w.apply_region(region(r1,true));
    w.apply_agent({"own-agent","first-session",{0,0,0},{},true});
    if(!clean(w)||w.object_revision()!=cleared_revision||
       w.objects().contains(42U))return 5;
    populate(w);
    const auto next_gen=w.region().generation;
    const auto next_revision=w.object_revision();
    // Some software paths directly apply a different RegionUpdate rather
    // than first calling begin_region_transition().
    w.apply_region(region(r2,true));
    if(!clean(w)||w.region().handle!=r2||
       w.region().generation!=next_gen+1U||
       w.object_revision()!=next_revision+1U||
       w.agent().present)return 6;
    w.apply_agent({"own-agent","first-session",{0,0,0},{},true});
    populate(w);
    const auto identity_gen=w.region().generation;
    const auto identity_revision=w.object_revision();
    // Different session on the same region reuses local IDs, but must
    // never inherit prior account avatar, outfit, scene or attachments.
    w.apply_agent({"own-agent","second-session",{0,0,0},{},true});
    if(!clean(w)||w.region().generation!=identity_gen+1U||
       w.object_revision()!=identity_revision+1U||
       !w.agent().present||w.agent().session_id!="second-session")
        return 7;
    const auto current_revision=w.object_revision();
    w.apply_agent({"own-agent","second-session",{1,0,0},{},true});
    if(w.object_revision()!=current_revision)return 8;
    populate(w);
    const auto other_gen=w.region().generation;
    w.apply_agent({"different-agent","second-session",{0,0,0},{},true});
    if(!clean(w)||w.region().generation!=other_gen+1U||
       w.objects().contains(42U))return 9;
    std::cout<<"DIRECT_REGION_CIRCUIT_AND_REAUTH_GHOST_SCENE_INVALIDATION_PASS\n";
}
