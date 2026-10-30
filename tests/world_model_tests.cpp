#include "nexverse/world/world_model.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

int main()
{
    using namespace nexverse::world;

    WorldModel world;
    if (world.ready())
    {
        std::cerr << "Fresh WorldModel unexpectedly ready\n";
        return 1;
    }

    world.set_grid({"NexVerse", "http://hg.stadt-nexverse.de:80/"});
    world.apply_region({
        0x0000010000000200ULL,
        "NexVerse",
        {128.0f, 128.0f, 25.0f},
        "127.0.0.1",
        9000U,
        "https://caps.example/seed",
        "aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb",
        0x10U,
        0U,
        0U,
        13U,
        20.0f,
        "OpenSim Test",
        256U,
        256U,
        true});
    world.apply_agent({
        "11111111-2222-3333-4444-555555555555",
        "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",
        {128.0f, 128.0f, 25.0f},
        {},
        true});

    if (world.ready() || !world.scene_ready())
    {
        std::cerr << "Scene readiness must precede validated terrain data\n";
        return 2;
    }

    nexverse::protocol::TerrainPatch terrain_patch;
    terrain_patch.x = 2U;
    terrain_patch.y = 3U;
    terrain_patch.heights.fill(42.25f);
    world.apply_terrain_patch(terrain_patch);

    const std::size_t terrain_sample =
        (3U * 16U + 5U) * 256U +
        (2U * 16U + 7U);

    if (!world.ready() || !world.scene_ready() ||
        world.terrain().valid_patch_count != 1U ||
        !world.terrain().valid_patches[3U * 16U + 2U] ||
        world.terrain().heights[terrain_sample] != 42.25f)
    {
        std::cerr << "Validated terrain patch did not populate WorldModel\n";
        return 3;
    }

    AvatarState avatar;
    avatar.id = "avatar-1";
    avatar.name = "Test Avatar";
    avatar.position = {130.0f, 128.0f, 25.0f};
    world.upsert_avatar(std::move(avatar));

    // SelfAvatar appearance is a distinct readback, not a second
    // avatar in the list. Changing region invalidates appearance.
    nexverse::protocol::AvatarAppearanceMessage own;
    own.avatar_id=world.agent().agent_id;
    // A valid neutral LLTextureEntry rather than a three-byte placeholder.
    own.texture_entry=std::vector<std::uint8_t>(16U,0x11U);
    const std::vector<std::uint8_t> te_fields{
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
    own.texture_entry.insert(own.texture_entry.end(),
        te_fields.begin(),te_fields.end());
    own.visual_params={2U,3U,4U};
    own.hover_heights={{0.0f,0.0f,0.25f}};
    own.attachments={{"22222222-2222-2222-2222-222222222222",5U}};
    if(!world.apply_self_appearance(own) ||
       !world.self_appearance() ||
       world.self_appearance()->hover_heights.front().z!=0.25f ||
       world.snapshot().self_appearance->attachments.size()!=1U)
        return 15;
    AvatarState fake_self;
    fake_self.id=world.agent().agent_id;
    world.upsert_avatar(std::move(fake_self));
    if(world.avatars().size()!=1U)return 16;
    auto wrong=own;
    wrong.avatar_id="22222222-2222-2222-2222-222222222222";
    if(world.apply_self_appearance(std::move(wrong)))return 17;
    if(world.self_appearance()->avatar_id!=world.agent().agent_id)
        return 18;
    // A trial/preview or partially published packet may never replace
    // the current confirmed server baked body and visual parameters.
    auto trial=own;
    trial.trial=true;
    trial.texture_entry={99U};
    if(world.apply_self_appearance(trial)||
       world.self_appearance()->texture_entry!=own.texture_entry)return 100;
    auto no_texture=own;
    no_texture.texture_entry.clear();
    if(world.apply_self_appearance(no_texture)||
       world.self_appearance()->texture_entry!=own.texture_entry)return 101;
    auto no_params=own;
    no_params.visual_params.clear();
    if(world.apply_self_appearance(no_params)||
       world.self_appearance()->visual_params!=own.visual_params)return 102;

    // Full/Compressed/Terse/Coarse updates share a pose-only WorldModel
    // method; appearance is preserved even if it arrived before pose.
    AvatarState pending;
    pending.id="remote-avatar";
    pending.visual_params={17U,23U};
    pending.texture_entry={0x12U,0x34U};
    pending.appearance_data={{2U,412U,0U}};
    pending.hover_heights={{0.0f,0.0f,0.5f}};
    pending.attachments={{"99999999-9999-9999-9999-999999999999",3U}};
    world.upsert_avatar(std::move(pending));
    if(world.avatars().at("remote-avatar").position_known)return 19;
    world.update_avatar_transform(
        "remote-avatar",{130.0f,129.0f,28.0f},{},901U);
    const auto& populated=world.avatars().at("remote-avatar");
    if(!populated.position_known || populated.local_id!=901U ||
       populated.appearance_data.size()!=1U ||
       populated.appearance_data.front().cof_version!=412U ||
       populated.attachments.size()!=1U ||
       populated.texture_entry.size()!=2U)return 20;
    world.update_avatar_transform(
        "remote-avatar",{131.0f,130.0f,28.0f},{},0U);
    if(world.avatars().at("remote-avatar").local_id!=901U ||
       world.avatars().at("remote-avatar").position.x!=131.0f ||
       world.avatars().at("remote-avatar").visual_params.size()!=2U ||
       world.avatars().at("remote-avatar").last_translation_at==
           std::chrono::steady_clock::time_point{})
        return 21;
    world.update_avatar_transform(
        world.agent().agent_id,{0.0f,0.0f,0.0f},{},999U);
    if(world.avatars().contains(world.agent().agent_id))return 22;
    // AvatarAnimation High20 is a full snapshot per known sender.
    // It must not create ghost avatars or cross region epochs.
    nexverse::protocol::AvatarAnimationMessage self_anim;
    self_anim.sender=world.agent().agent_id;
    self_anim.animations={{"6ed24bd8-91aa-4b12-ccc7-c97c857ab4e0",4}};
    if(!world.apply_avatar_animation(self_anim)||
       !world.self_animations()||
       world.self_animations()->animations.size()!=1U||
       !world.snapshot().self_animations)
        return 24;
    nexverse::protocol::AvatarAnimationMessage wrong_anim=self_anim;
    wrong_anim.sender="33333333-3333-3333-3333-333333333333";
    if(!world.apply_avatar_animation(wrong_anim)||
       world.avatars().contains(wrong_anim.sender))
        return 25;
    // AvatarAnimation can precede ObjectUpdate. Pending animation must
    // not create a visible ghost or be lost on subsequent avatar spawn.
    AvatarState late_avatar;
    late_avatar.id=wrong_anim.sender;
    world.upsert_avatar(std::move(late_avatar));
    if(!world.avatars().at(wrong_anim.sender).animations ||
       world.avatars().at(wrong_anim.sender).animations->animations.size()!=1U)
        return 30;
    world.remove_avatar(wrong_anim.sender);
    nexverse::protocol::AvatarAnimationMessage remote_anim=self_anim;
    remote_anim.sender="remote-avatar";
    if(!world.apply_avatar_animation(remote_anim)||
       !world.avatars().at("remote-avatar").animations ||
       world.avatars().at("remote-avatar").animations->animations.size()!=1U)
        return 26;
    const auto snapshot=world.snapshot();
    if(!snapshot.avatars.at("remote-avatar").animations ||
       snapshot.avatars.at("remote-avatar").animations->animations.size()!=1U)
        return 27;
    remote_anim.animations.clear(); // simulator stop: empty full snapshot
    if(!world.apply_avatar_animation(remote_anim)||
       !world.avatars().at("remote-avatar").animations ||
       !world.avatars().at("remote-avatar").animations->animations.empty())
        return 28;
    // Pose-only updates must retain animation metadata.
    world.update_avatar_transform(
        "remote-avatar",{131.1f,130.0f,28.0f},{},901U);
    if(!world.avatars().at("remote-avatar").animations)
        return 29;
    world.remove_avatar("remote-avatar");
    // The pending map is finite even if an active simulator sends UUIDs
    // for unknown avatars. Such messages must not allocate ghost avatars.
    for(unsigned i=0U;i<64U;++i)
    {
        auto queued=self_anim;
        queued.sender="90000000-0000-0000-0000-000000000000";
        constexpr char hexdigits[]="0123456789abcdef";
        queued.sender[34U]=hexdigits[i/16U];
        queued.sender[35U]=hexdigits[i%16U];
        if(!world.apply_avatar_animation(std::move(queued)))return 31;
    }
    auto overflow=self_anim;
    overflow.sender="90000000-0000-0000-0000-0000000000ff";
    if(world.apply_avatar_animation(std::move(overflow))||
       world.avatars().size()!=1U)return 32;
    nexverse::world::ObjectState object;
    object.local_id = 42U;
    object.id = "object-uuid";
    object.pcode = nexverse::protocol::pcode_prim;
    object.parent_id = 7U;
    object.position = {132.0f, 128.0f, 25.0f};
    object.scale = {2.0f, 3.0f, 4.0f};
    world.upsert_object(object);

    if (world.avatars().size() != 1U ||
        world.objects().size() != 1U ||
        world.object_revision() != 1U ||
        world.objects().at(42U).parent_id != 7U ||
        world.objects().at(42U).scale.z != 4.0f)
    {
        std::cerr << "World entity upsert failed\n";
        return 4;
    }

    const WorldSnapshot stable_snapshot = world.snapshot();
    world.remove_object(42U);

    if (!stable_snapshot.ready ||
        stable_snapshot.objects.size() != 1U ||
        !world.objects().empty() ||
        world.object_revision() != 2U)
    {
        std::cerr << "WorldSnapshot isolation failed\n";
        return 5;
    }

    world.upsert_object(std::move(object));

    world.set_parcel({
        9U,
        "Parcel",
        "Description",
        "owner",
        4096,
        0U,
        0,
        {},
        {},
        true});
    nexverse::world::EnvironmentState environment;
    environment.revision = 1U;
    environment.available = true;
    world.set_environment(environment);

    const std::uint32_t previous_generation =
        world.region().generation;

    world.begin_region_transition({
        0x0000030000000400ULL,
        {},
        {10.0f, 20.0f, 30.0f},
        "192.0.2.10",
        9100U,
        "https://caps.example/new",
        {},
        0U,
        0U,
        0U,
        0U,
        0.0f,
        {},
        256U,
        256U,
        false});

    if (world.ready() || world.scene_ready() ||
        world.region().generation != previous_generation + 1U ||
        world.region().handle != 0x0000030000000400ULL ||
        world.terrain().available ||
        world.parcel().available ||
        world.environment().available ||
        !world.avatars().empty() ||
        !world.objects().empty() ||
        world.self_appearance().has_value() ||
        world.self_animations().has_value() ||
        world.agent().present)
    {
        std::cerr << "WorldModel region transition invalidation failed\n";
        return 6;
    }

    // Same transition may be replayed by multiple EventQueue queues.
    // Identical pending target MUST be idempotent, not clear self
    // readbacks and increment the region generation repeatedly.
    const auto first_transition_generation=world.region().generation;
    const RegionUpdate pending_target{
        0x0000030000000400ULL,{}, {10.0f,20.0f,30.0f},
        "192.0.2.10",9100U,"https://caps.example/new",{},0U,
        0U,0U,0U,0.0f,{},256U,256U,false};
    world.begin_region_transition(pending_target);
    if(world.region().generation!=first_transition_generation ||
       world.region().handle!=pending_target.handle)return 108;
    // A distinct destination agent position is NOT a replay, even
    // within the same simulator/seed. It starts a new handoff.
    auto different_position=pending_target;
    different_position.agent_position.x=128.0f;
    world.begin_region_transition(different_position);
    if(world.region().generation!=first_transition_generation+1U ||
       world.agent().position.x!=128.0f)return 109;
    world.begin_region_transition(different_position);
    if(world.region().generation!=first_transition_generation+1U)
        return 110;
    auto new_seed=different_position;
    new_seed.seed_capability="https://caps.example/other-instance";
    world.begin_region_transition(new_seed);
    if(world.region().generation!=first_transition_generation+2U)
        return 111;
    auto new_socket=new_seed;
    new_socket.simulator_port=9101U;
    world.begin_region_transition(new_socket);
    if(world.region().generation!=first_transition_generation+3U)
        return 112;
    // Partial RegionHandshake with connected=false must NOT clear
    // pending source evidence and allow a duplicate to kill the world.
    world.apply_region(new_socket);
    const auto generation_after_handshake=world.region().generation;
    world.begin_region_transition(new_socket);
    if(world.region().generation!=generation_after_handshake)
        return 115;
    // Handle is part of destination identity, not just the CAPS URI.
    WorldModel another_world;
    another_world.begin_region_transition(pending_target);
    auto other_handle=pending_target;
    other_handle.handle+=0x100ULL;
    another_world.begin_region_transition(other_handle);
    if(another_world.region().generation!=2U ||
       another_world.region().handle!=other_handle.handle)
        return 116;
    // Once the new simulator circuit is connected, a subsequent
    // teleport to the exact same target must remain possible.
    auto live=new_socket;
    live.connected=true;
    world.apply_region(live);
    const auto connected_generation=world.region().generation;
    world.begin_region_transition(new_socket);
    if(world.region().generation!=connected_generation+1U ||
       world.region().connected)return 113;
    world.reset();
    // A stale transition must not survive full logout/reset.
    world.begin_region_transition(new_socket);
    if(world.region().generation!=1U)return 114;

    world.reset();
    if (world.ready() || world.scene_ready() ||
        world.region().connected || world.agent().present ||
        world.self_appearance().has_value() ||
        world.self_animations().has_value())
    {
        std::cerr << "WorldModel reset failed\n";
        return 7;
    }

    // Region/circuit and agent/session remain hard prerequisites for
    // scene_ready even though terrain is a soft streaming prerequisite.
    WorldModel probe;
    probe.apply_region({
        44U,"Test",{0.0f,0.0f,0.0f},"192.0.2.1",9100U,"",{},0U,
        0U,0U,0U,0.0f,{},256U,256U,true});
    probe.apply_agent({"owner","",{}, {},true});
    if(probe.scene_ready())return 30;
    probe.apply_agent({"owner","session",{}, {},true});
    if(!probe.scene_ready()||probe.ready())return 31;
    probe.apply_agent({"owner","session",{}, {},false});
    if(probe.scene_ready())return 32;
    probe.apply_agent({"owner","session",{}, {},true});
    probe.apply_region({
        0U,"Test",{0.0f,0.0f,0.0f},"192.0.2.1",9100U,"",{},0U,
        0U,0U,0U,0.0f,{},256U,256U,true});
    if(probe.scene_ready())return 33;

    std::cout << "WorldModel foundation tests passed\n";
    return 0;
}
