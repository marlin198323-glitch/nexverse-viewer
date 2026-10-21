/* MPL-2.0: coarse map positions must not become visible 3D avatars,
 * suppress full ObjectUpdate transforms or erase received appearance. */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <iostream>
#include <cmath>
#include <vector>
using namespace nexverse;
static world::RegionUpdate region(std::uint64_t h,bool connected)
{
    return {h,"NexVerse",{128.0f,128.0f,25.0f},"127.0.0.1",9000U,
        "https://caps.invalid/seed","11111111-2222-3333-4444-555555555555",
        0x10U,0U,0U,13U,20.0f,"OpenGenesisLINK",256U,256U,connected};
}
int main()
{
    constexpr auto a="aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb";
    constexpr auto b="bbbbbbbb-1111-2222-3333-cccccccccccc";
    constexpr auto c="cccccccc-1111-2222-3333-dddddddddddd";
    world::WorldModel w;
    w.apply_region(region(0x0000010000000200ULL,true));
    w.apply_agent({"self","session",{0,0,0},{},true});
    protocol::CoarseLocationUpdate coarse;
    coarse.avatars={{"self",{10.0f,20.0f,8.0f}},
                    {a,{30.0f,40.0f,12.0f}},
                    {a,{31.0f,41.0f,16.0f}},
                    {b,{70.0f,80.0f,24.0f}},
                    {"00000000-0000-0000-0000-000000000000",{0,0,0}}};
    if(!w.apply_coarse_avatar_locations(coarse)||
       w.avatars().size()!=2U||
       w.avatars().at(a).position.x!=30.0f||
       !w.avatars().at(a).coarse_position_only||
       w.avatars().at(a).simulator_object_update_seen||
       w.avatars().contains("self")||
       w.avatars().contains("00000000-0000-0000-0000-000000000000"))
        return 1;
    // This is the actual ObjectUpdate admission for a remote avatar;
    // no fabricated mesh or Low158 is introduced by a coarse packet.
    w.update_avatar_transform(a,{36.75f,40.25f,15.5f},
                              {0,0,0,1},123U,true);
    if(!w.avatars().at(a).simulator_object_update_seen||
       w.avatars().at(a).coarse_position_only||
       w.avatars().at(a).local_id!=123U)return 2;
    // An authentic remote Low158 with outfit/hover data can arrive
    // between coarse reports; never erase it.
    auto full=w.avatars().at(a);
    full.texture_entry={1U,2U,3U};
    full.visual_params={4U,5U};
    full.appearance_data={{2U,412U,0U}};
    full.attachments={{"worn-prim",5U}};
    w.upsert_avatar(full);
    // A following coarse update may move map-only B, but NEVER downgrade
    // the fractional current ObjectUpdate of A or delete A when omitted.
    protocol::CoarseLocationUpdate next;
    next.avatars={{a,{99,99,80}},{b,{72,82,28}},{c,{10,10,4}}};
    if(!w.apply_coarse_avatar_locations(next)||
       w.avatars().at(a).position.x!=36.75f||
       w.avatars().at(a).position.y!=40.25f||
       w.avatars().at(a).appearance_data.size()!=1U||
       w.avatars().at(a).attachments.size()!=1U||
       w.avatars().at(b).position.x!=72.0f||
       w.avatars().at(c).simulator_object_update_seen||
       w.avatars().at(c).local_id!=0U)return 3;
    next.avatars={{c,{11,11,4}}};
    if(!w.apply_coarse_avatar_locations(next)||
       !w.avatars().contains(a)||w.avatars().contains(b)||
       !w.avatars().contains(c)||
       !w.avatars().at(a).simulator_object_update_seen)
        return 4;
    // A Coarse-only avatar which already received appearance metadata
    // can keep it without becoming a falsely rendered ObjectUpdate body.
    auto pending=w.avatars().at(c);
    pending.visual_params={15U,19U};
    w.upsert_avatar(pending);
    next.avatars.clear();
    if(!w.apply_coarse_avatar_locations(next)||
       !w.avatars().contains(a)||!w.avatars().contains(c)||
       w.avatars().at(c).simulator_object_update_seen||
       w.avatars().at(c).visual_params.size()!=2U)
        return 5;
    // Unknown coarse report and disconnected circuit cannot mutate world.
    const auto original_count=w.avatars().size();
    w.apply_region(region(0x0000010000000200ULL,false));
    if(!w.avatars().empty()||
       w.apply_coarse_avatar_locations(coarse)||!w.avatars().empty())
        return 6;
    w.apply_region(region(0x0000020000000300ULL,true));
    w.apply_agent({"new-self","new-session",{0,0,0},{},true});
    if(!w.avatars().empty()||
       w.avatars().size()==original_count)
       return 7;
    // Out-of-order invalid packet cannot introduce NaN coordinates.
    protocol::CoarseLocationUpdate invalid;
    invalid.avatars={{c,{NAN,0,0}},{a,{10,10,4}}};
    if(!w.apply_coarse_avatar_locations(invalid)||
       w.avatars().size()!=1U||!w.avatars().contains(a))
        return 8;
    // Decode the actual CoarseLocationUpdate on-wire payload, not a
    // synthetic world-only object. Positions are coarse byte-quantized;
    // UUID is full 16-byte. No LocalID, appearance, rig or COF exists.
    const auto uuid=protocol::uuid_to_bytes(b);
    if(!uuid)return 9;
    std::vector<std::uint8_t> payload{
        1U,42U,87U,5U, 0U,0U, 0xFFU,0xFFU, 1U};
    payload.insert(payload.end(),uuid->begin(),uuid->end());
    const auto decoded=protocol::decode_coarse_location_update(payload);
    if(!decoded||decoded->avatars.size()!=1U||
       decoded->avatars[0].agent_id!=b||
       decoded->avatars[0].position.x!=42.0f||
       decoded->avatars[0].position.z!=20.0f||
       !w.apply_coarse_avatar_locations(*decoded)||
       w.avatars().at(b).simulator_object_update_seen||
       !w.avatars().at(b).coarse_position_only)return 10;
    // Malformed payload is rejected by decoder before it can alter world.
    payload.pop_back();
    if(protocol::decode_coarse_location_update(payload)||
       w.avatars().at(b).simulator_object_update_seen)return 11;
    std::cout<<"COARSE_IS_MAP_ONLY_FULL_OBJECT_ADMITS_3D_AND_PRESERVES_OUTFIT_PASS\n";
}
