#include "nexverse/protocol/viewer_messages.hpp"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>

int main()
{
    const auto id_bytes = nexverse::protocol::uuid_to_bytes(
        "11111111-2222-3333-4444-555555555555");
    std::vector<std::uint8_t> mesh_extra{
        1U, 0x30U, 0x00U, 17U, 0U, 0U, 0U};
    mesh_extra.insert(mesh_extra.end(), id_bytes->begin(), id_bytes->end());
    mesh_extra.push_back(0x85U); // Mesh type with reserved high bit.
    const auto mesh = nexverse::protocol::parse_sculpt_extra_params(mesh_extra);
    if (!mesh || !mesh->is_mesh() ||
        mesh->asset_id != "11111111-2222-3333-4444-555555555555")
    {
        std::cerr << "Sculpt/Mesh ExtraParams decode failed\n";
        return 90;
    }
    mesh_extra.back() = 0x01U;
    const auto sculpt = nexverse::protocol::parse_sculpt_extra_params(mesh_extra);
    if (!sculpt || sculpt->is_mesh())
    {
        std::cerr << "Sculpt detected incorrectly as mesh\n";
        return 91;
    }
    mesh_extra[3U] = 200U;
    if (nexverse::protocol::parse_sculpt_extra_params(mesh_extra).has_value())
    {
        std::cerr << "Malformed sculpt block accepted\n";
        return 92;
    }

    using namespace nexverse::protocol;

    LoginSession session;
    session.agent_id = "11111111-2222-3333-4444-555555555555";
    session.session_id = "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";

    AgentUpdateData update;
    update.camera_center = {128.0f, 128.0f, 30.0f};
    update.control_flags = control_at_pos | control_fly;

    const auto agent_packet = encode_agent_update(10U, session, update);
    const auto decoded_agent = decode_lludp_packet(agent_packet);
    if (!decoded_agent.ok() ||
        decoded_agent.packet->header.message != msg_agent_update ||
        decoded_agent.packet->payload.size() != 122U)
    {
        std::cerr << "AgentUpdate encoding failed\n";
        return 1;
    }

    LocalChatMessage outgoing;
    outgoing.message = "Hallo NexVerse";
    outgoing.type = LocalChatType::normal;
    outgoing.channel = 0;

    const auto chat_packet = encode_chat_from_viewer(11U, session, outgoing);
    const auto decoded_chat_packet = decode_lludp_packet(chat_packet);
    if (!decoded_chat_packet.ok() ||
        decoded_chat_packet.packet->header.message != msg_chat_from_viewer ||
        (decoded_chat_packet.packet->header.flags & packet_reliable) == 0U)
    {
        std::cerr << "ChatFromViewer encoding failed\n";
        return 2;
    }

    std::vector<std::uint8_t> layer_payload{
        layer_type_land,
        0x04U, 0x00U,
        0x10U, 0x20U, 0x30U, 0x40U};

    const auto layer = decode_layer_data(layer_payload);
    if (!layer.has_value() ||
        layer->type != layer_type_land ||
        !layer->is_terrain() ||
        layer->data.size() != 4U ||
        layer->data[0] != 0x10U ||
        layer->data[3] != 0x40U)
    {
        std::cerr << "LayerData decode failed\n";
        return 3;
    }

    std::vector<std::uint8_t> wind_layer{
        0x37U,
        0x01U, 0x00U,
        0xAAU};

    const auto wind = decode_layer_data(wind_layer);
    if (!wind.has_value() || wind->is_terrain())
    {
        std::cerr << "Non-terrain LayerData classification failed\n";
        return 4;
    }

    std::vector<std::uint8_t> invalid_layer{
        layer_type_land,
        0x05U, 0x00U,
        0x10U, 0x20U, 0x30U, 0x40U};

    if (decode_layer_data(invalid_layer).has_value())
    {
        std::cerr << "Invalid LayerData unexpectedly accepted\n";
        return 5;
    }

    const auto coarse_agent_a =
        uuid_to_bytes("11111111-1111-1111-1111-111111111111");
    const auto coarse_agent_b =
        uuid_to_bytes("22222222-2222-2222-2222-222222222222");

    std::vector<std::uint8_t> coarse_payload{
        2U,
        10U, 20U, 5U,
        30U, 40U, 7U,
        0U, 0U,
        0xFFU, 0xFFU,
        2U};
    coarse_payload.insert(
        coarse_payload.end(),
        coarse_agent_a->begin(),
        coarse_agent_a->end());
    coarse_payload.insert(
        coarse_payload.end(),
        coarse_agent_b->begin(),
        coarse_agent_b->end());

    const auto coarse = decode_coarse_location_update(coarse_payload);
    if (!coarse.has_value() ||
        coarse->avatars.size() != 2U ||
        coarse->you_index != 0 ||
        coarse->prey_index != -1 ||
        coarse->avatars[0].position.x != 10.0f ||
        coarse->avatars[0].position.y != 20.0f ||
        coarse->avatars[0].position.z != 20.0f ||
        coarse->avatars[1].position.z != 28.0f)
    {
        std::cerr << "CoarseLocationUpdate decode failed\n";
        return 6;
    }

    std::vector<std::uint8_t> invalid_coarse = coarse_payload;
    invalid_coarse.pop_back();
    if (decode_coarse_location_update(invalid_coarse).has_value())
    {
        std::cerr << "Invalid CoarseLocationUpdate unexpectedly accepted\n";
        return 7;
    }

    std::vector<std::uint8_t> kill_payload{
        2U,
        0x2AU, 0x00U, 0x00U, 0x00U,
        0x78U, 0x56U, 0x34U, 0x12U};

    const auto kill = decode_kill_object(kill_payload);
    if (!kill.has_value() ||
        kill->local_ids.size() != 2U ||
        kill->local_ids[0] != 42U ||
        kill->local_ids[1] != 0x12345678U)
    {
        std::cerr << "KillObject decode failed\n";
        return 8;
    }

    std::vector<std::uint8_t> invalid_kill = kill_payload;
    invalid_kill.pop_back();
    if (decode_kill_object(invalid_kill).has_value())
    {
        std::cerr << "Invalid KillObject unexpectedly accepted\n";
        return 9;
    }

    std::vector<std::uint8_t> terse_payload{
        0x88U, 0x77U, 0x66U, 0x55U, 0x44U, 0x33U, 0x22U, 0x11U,
        0x34U, 0x12U,
        1U,
        44U};

    std::vector<std::uint8_t> terse_data(44U, 0U);
    terse_data[0] = 0x2AU;
    terse_data[4] = 3U;
    terse_data[5] = 0U;

    auto put_u32 = [&](std::vector<std::uint8_t>& target, std::size_t offset, std::uint32_t value)
    {
        target[offset] = static_cast<std::uint8_t>(value & 0xFFU);
        target[offset + 1U] = static_cast<std::uint8_t>((value >> 8U) & 0xFFU);
        target[offset + 2U] = static_cast<std::uint8_t>((value >> 16U) & 0xFFU);
        target[offset + 3U] = static_cast<std::uint8_t>((value >> 24U) & 0xFFU);
    };
    auto put_f32 = [&](std::vector<std::uint8_t>& target, std::size_t offset, float value)
    {
        put_u32(target, offset, std::bit_cast<std::uint32_t>(value));
    };

    put_f32(terse_data, 6U, 10.0f);
    put_f32(terse_data, 10U, 20.0f);
    put_f32(terse_data, 14U, 30.0f);

    terse_payload.insert(
        terse_payload.end(),
        terse_data.begin(),
        terse_data.end());
    terse_payload.push_back(0U);
    terse_payload.push_back(0U);

    const auto terse = decode_improved_terse_object_update(terse_payload);
    if (!terse.has_value() ||
        terse->region_handle != 0x1122334455667788ULL ||
        terse->time_dilation != 0x1234U ||
        terse->updates.size() != 1U ||
        terse->updates[0].local_id != 42U ||
        terse->updates[0].avatar ||
        terse->updates[0].position.x != 10.0f ||
        terse->updates[0].position.y != 20.0f ||
        terse->updates[0].position.z != 30.0f)
    {
        std::cerr << "ImprovedTerseObjectUpdate decode failed\n";
        return 10;
    }

    std::vector<std::uint8_t> invalid_terse = terse_payload;
    invalid_terse.pop_back();
    if (decode_improved_terse_object_update(invalid_terse).has_value())
    {
        std::cerr << "Invalid ImprovedTerseObjectUpdate unexpectedly accepted\n";
        return 11;
    }

    auto append_u32_vec = [](std::vector<std::uint8_t>& out, std::uint32_t value)
    {
        out.push_back(static_cast<std::uint8_t>(value & 0xFFU));
        out.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
        out.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
        out.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
    };
    auto append_f32_vec = [&](std::vector<std::uint8_t>& out, float value)
    {
        append_u32_vec(out, std::bit_cast<std::uint32_t>(value));
    };

    std::vector<std::uint8_t> full_update_payload{
        0x88U, 0x77U, 0x66U, 0x55U, 0x44U, 0x33U, 0x22U, 0x11U,
        0x34U, 0x12U,
        1U};

    append_u32_vec(full_update_payload, 42U);
    full_update_payload.push_back(0U);

    const auto full_object_id =
        uuid_to_bytes("33333333-3333-3333-3333-333333333333");
    full_update_payload.insert(
        full_update_payload.end(),
        full_object_id->begin(),
        full_object_id->end());

    append_u32_vec(full_update_payload, 0U);
    full_update_payload.push_back(pcode_prim);
    full_update_payload.push_back(3U);
    full_update_payload.push_back(0U);
    append_f32_vec(full_update_payload, 2.0f);
    append_f32_vec(full_update_payload, 3.0f);
    append_f32_vec(full_update_payload, 4.0f);

    std::vector<std::uint8_t> full_object_data(60U, 0U);
    put_f32(full_object_data, 0U, 101.0f);
    put_f32(full_object_data, 4U, 102.0f);
    put_f32(full_object_data, 8U, 25.0f);
    put_f32(full_object_data, 36U, 0.0f);
    put_f32(full_object_data, 40U, 0.0f);
    put_f32(full_object_data, 44U, 0.0f);

    full_update_payload.push_back(60U);
    full_update_payload.insert(
        full_update_payload.end(),
        full_object_data.begin(),
        full_object_data.end());

    append_u32_vec(full_update_payload, 7U);
    append_u32_vec(full_update_payload, 0U);
    full_update_payload.push_back(0x10U);
    full_update_payload.push_back(0x01U);
    full_update_payload.insert(full_update_payload.end(), 21U, 0U);

    std::vector<std::uint8_t> object_texture_entry;
    const auto default_texture_id =
        uuid_to_bytes("89556747-24cb-43ed-920b-47caed15465f");
    object_texture_entry.insert(
        object_texture_entry.end(),
        default_texture_id->begin(),
        default_texture_id->end());
    object_texture_entry.push_back(0U); // texture overrides

    object_texture_entry.insert(
        object_texture_entry.end(),
        {64U, 128U, 191U, 51U});
    object_texture_entry.push_back(0U); // color overrides

    append_f32_vec(object_texture_entry, 1.0f);
    object_texture_entry.push_back(0U); // repeat U overrides
    append_f32_vec(object_texture_entry, 1.0f);
    object_texture_entry.push_back(0U); // repeat V overrides

    object_texture_entry.insert(
        object_texture_entry.end(),
        {0U, 0U, 0U}); // offset U + overrides
    object_texture_entry.insert(
        object_texture_entry.end(),
        {0U, 0U, 0U}); // offset V + overrides
    object_texture_entry.insert(
        object_texture_entry.end(),
        {0U, 0U, 0U}); // rotation + overrides

    object_texture_entry.push_back(0x60U);
    object_texture_entry.push_back(0U); // material overrides

    full_update_payload.push_back(
        static_cast<std::uint8_t>(
            object_texture_entry.size() & 0xFFU));
    full_update_payload.push_back(
        static_cast<std::uint8_t>(
            (object_texture_entry.size() >> 8U) & 0xFFU));
    full_update_payload.insert(
        full_update_payload.end(),
        object_texture_entry.begin(),
        object_texture_entry.end());

    full_update_payload.push_back(0U);
    full_update_payload.insert(full_update_payload.end(), {0U, 0U});
    full_update_payload.insert(full_update_payload.end(), {0U, 0U});
    full_update_payload.push_back(0U);
    full_update_payload.insert(full_update_payload.end(), 4U, 0U);
    full_update_payload.push_back(0U);
    full_update_payload.push_back(0U);
    full_update_payload.push_back(0U);
    full_update_payload.insert(full_update_payload.end(), 16U, 0U);
    full_update_payload.insert(full_update_payload.end(), 16U, 0U);
    full_update_payload.insert(full_update_payload.end(), 4U, 0U);
    full_update_payload.push_back(0U);
    full_update_payload.insert(full_update_payload.end(), 4U, 0U);
    full_update_payload.push_back(0U);
    full_update_payload.insert(full_update_payload.end(), 12U, 0U);
    full_update_payload.insert(full_update_payload.end(), 12U, 0U);

    const auto full_update = decode_object_update(full_update_payload);
    if (!full_update.has_value() ||
        full_update->objects.size() != 1U ||
        full_update->objects[0].local_id != 42U ||
        full_update->objects[0].id != "33333333-3333-3333-3333-333333333333" ||
        full_update->objects[0].is_avatar() ||
        full_update->objects[0].position.x != 101.0f ||
        full_update->objects[0].position.y != 102.0f ||
        full_update->objects[0].position.z != 25.0f ||
        full_update->objects[0].parent_id != 7U ||
        full_update->objects[0].scale.x != 2.0f ||
        full_update->objects[0].scale.y != 3.0f ||
        full_update->objects[0].scale.z != 4.0f ||
        full_update->objects[0].shape.path_curve != 0x10U ||
        full_update->objects[0].shape.profile_shape() != 0x01U ||
        !full_update->objects[0].default_texture.has_value() ||
        full_update->objects[0].default_texture->texture_id !=
            "89556747-24cb-43ed-920b-47caed15465f" ||
        std::abs(
            full_update->objects[0].default_texture->color[0] -
            (191.0f / 255.0f)) > 0.0001f ||
        std::abs(
            full_update->objects[0].default_texture->color[3] -
            (204.0f / 255.0f)) > 0.0001f ||
        full_update->objects[0].default_texture->material != 0x60U)
    {
        std::cerr << "ObjectUpdate decode failed\n";
        return 12;
    }

    std::vector<std::uint8_t> invalid_full_update = full_update_payload;
    invalid_full_update.pop_back();
    if (decode_object_update(invalid_full_update).has_value())
    {
        std::cerr << "Invalid ObjectUpdate unexpectedly accepted\n";
        return 13;
    }

    std::vector<std::uint8_t> compressed_payload{
        0x88U,0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,
        0x34U,0x12U,1U};
    append_u32_vec(compressed_payload, 0U);
    std::vector<std::uint8_t> compressed_data(64U, 0U);
    const auto compressed_id =
        uuid_to_bytes("44444444-4444-4444-4444-444444444444");
    std::copy(compressed_id->begin(), compressed_id->end(), compressed_data.begin());
    put_u32(compressed_data, 16U, 77U);
    compressed_data[20U] = pcode_prim;
    put_f32(compressed_data, 40U, 12.0f);
    put_f32(compressed_data, 44U, 34.0f);
    put_f32(compressed_data, 48U, 56.0f);
    compressed_payload.push_back(64U);
    compressed_payload.push_back(0U);
    compressed_payload.insert(
        compressed_payload.end(),
        compressed_data.begin(),
        compressed_data.end());

    const auto compressed = decode_object_update_compressed(compressed_payload);
    if (!compressed.has_value() ||
        compressed->objects.size() != 1U ||
        compressed->objects[0].local_id != 77U ||
        compressed->objects[0].position.z != 56.0f)
    {
        std::cerr << "ObjectUpdateCompressed decode failed\n";
        return 14;
    }

    std::vector<std::uint8_t> cached_payload{
        0x88U,0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,
        0x34U,0x12U,1U};
    append_u32_vec(cached_payload, 77U);
    append_u32_vec(cached_payload, 0xAABBCCDDU);
    append_u32_vec(cached_payload, 0x01020304U);

    const auto cached = decode_object_update_cached(cached_payload);
    if (!cached.has_value() ||
        cached->objects.size() != 1U ||
        cached->objects[0].local_id != 77U ||
        cached->objects[0].crc != 0xAABBCCDDU)
    {
        std::cerr << "ObjectUpdateCached decode failed\n";
        return 15;
    }

    std::vector<std::uint8_t> agent_data_payload;
    const auto agent_data_id =
        uuid_to_bytes("55555555-5555-5555-5555-555555555555");
    const auto active_group_id =
        uuid_to_bytes("66666666-6666-6666-6666-666666666666");
    agent_data_payload.insert(
        agent_data_payload.end(),
        agent_data_id->begin(),
        agent_data_id->end());
    agent_data_payload.push_back(3U);
    agent_data_payload.insert(agent_data_payload.end(), {'J','a','m'});
    agent_data_payload.push_back(6U);
    agent_data_payload.insert(agent_data_payload.end(), {'V','i','e','w','e','r'});
    agent_data_payload.push_back(5U);
    agent_data_payload.insert(agent_data_payload.end(), {'A','d','m','i','n'});
    agent_data_payload.insert(
        agent_data_payload.end(),
        active_group_id->begin(),
        active_group_id->end());
    for (unsigned int i = 0U; i < 8U; ++i)
    {
        agent_data_payload.push_back(
            static_cast<std::uint8_t>((0x1122334455667788ULL >> (i * 8U)) & 0xFFU));
    }
    agent_data_payload.push_back(8U);
    agent_data_payload.insert(
        agent_data_payload.end(),
        {'N','e','x','V','e','r','s','e'});

    const auto agent_data = decode_agent_data_update(agent_data_payload);
    if (!agent_data.has_value() ||
        agent_data->first_name != "Jam" ||
        agent_data->last_name != "Viewer" ||
        agent_data->group_title != "Admin" ||
        agent_data->group_name != "NexVerse" ||
        agent_data->group_powers != 0x1122334455667788ULL)
    {
        std::cerr << "AgentDataUpdate decode failed\n";
        return 16;
    }

    std::vector<std::uint8_t> appearance_payload;
    const auto appearance_id =
        uuid_to_bytes("77777777-7777-7777-7777-777777777777");
    appearance_payload.insert(
        appearance_payload.end(),
        appearance_id->begin(),
        appearance_id->end());
    appearance_payload.push_back(1U);
    appearance_payload.push_back(3U);
    appearance_payload.push_back(0U);
    appearance_payload.insert(
        appearance_payload.end(),
        {0xAAU,0xBBU,0xCCU});
    appearance_payload.push_back(3U);
    appearance_payload.insert(
        appearance_payload.end(),
        {10U,20U,30U});
    appearance_payload.push_back(1U);
    appearance_payload.push_back(2U);
    append_u32_vec(appearance_payload, 123U);
    append_u32_vec(appearance_payload, 456U);

    const auto appearance =
        decode_avatar_appearance(appearance_payload);
    if (!appearance.has_value() ||
        appearance->avatar_id != "77777777-7777-7777-7777-777777777777" ||
        !appearance->trial ||
        appearance->texture_entry.size() != 3U ||
        appearance->visual_params.size() != 3U ||
        appearance->appearance_data.size() != 1U ||
        appearance->appearance_data[0].appearance_version != 2U ||
        appearance->appearance_data[0].cof_version != 123U ||
        appearance->appearance_data[0].flags != 456U)
    {
        std::cerr << "AvatarAppearance decode failed\n";
        return 17;
    }

    auto append_u64_vec = [](std::vector<std::uint8_t>& out, std::uint64_t value)
    {
        for (unsigned int i = 0U; i < 8U; ++i)
        {
            out.push_back(static_cast<std::uint8_t>(
                (value >> (i * 8U)) & 0xFFU));
        }
    };

    std::vector<std::uint8_t> handshake_payload;
    append_u32_vec(handshake_payload, 0x12345678U);
    handshake_payload.push_back(13U);
    handshake_payload.push_back(8U);
    handshake_payload.insert(
        handshake_payload.end(),
        {'N','e','x','V','e','r','s','e'});

    const auto handshake_owner =
        uuid_to_bytes("99999999-9999-9999-9999-999999999999");
    handshake_payload.insert(
        handshake_payload.end(),
        handshake_owner->begin(),
        handshake_owner->end());
    handshake_payload.push_back(1U);
    append_f32_vec(handshake_payload, 21.5f);
    append_f32_vec(handshake_payload, 1.0f);
    handshake_payload.insert(handshake_payload.end(), 16U, 0U);

    const std::array<std::string_view, 8> terrain_ids{
        "10000000-0000-0000-0000-000000000001",
        "10000000-0000-0000-0000-000000000002",
        "10000000-0000-0000-0000-000000000003",
        "10000000-0000-0000-0000-000000000004",
        "20000000-0000-0000-0000-000000000001",
        "20000000-0000-0000-0000-000000000002",
        "20000000-0000-0000-0000-000000000003",
        "20000000-0000-0000-0000-000000000004"
    };

    for (const auto id : terrain_ids)
    {
        const auto bytes = uuid_to_bytes(id);
        handshake_payload.insert(
            handshake_payload.end(),
            bytes->begin(),
            bytes->end());
    }

    append_f32_vec(handshake_payload, 18.0f);
    append_f32_vec(handshake_payload, 19.0f);
    append_f32_vec(handshake_payload, 20.0f);
    append_f32_vec(handshake_payload, 21.0f);
    append_f32_vec(handshake_payload, 8.0f);
    append_f32_vec(handshake_payload, 9.0f);
    append_f32_vec(handshake_payload, 10.0f);
    append_f32_vec(handshake_payload, 11.0f);

    const auto handshake_region =
        uuid_to_bytes("aaaaaaaa-9999-8888-7777-bbbbbbbbbbbb");
    handshake_payload.insert(
        handshake_payload.end(),
        handshake_region->begin(),
        handshake_region->end());
    append_u32_vec(handshake_payload, 7U);
    append_u32_vec(handshake_payload, 8U);
    handshake_payload.push_back(0U);
    handshake_payload.push_back(0U);
    handshake_payload.push_back(0U);
    handshake_payload.push_back(1U);
    append_u64_vec(handshake_payload, 0x1122334455667788ULL);
    append_u64_vec(handshake_payload, 0x8877665544332211ULL);

    const auto handshake = decode_region_handshake(handshake_payload);
    if (!handshake.has_value() ||
        handshake->region_flags != 0x12345678U ||
        handshake->sim_access != 13U ||
        handshake->sim_name != "NexVerse" ||
        handshake->water_height != 21.5f ||
        !handshake->estate_manager ||
        handshake->terrain_base[0] !=
            "10000000-0000-0000-0000-000000000001" ||
        handshake->terrain_detail[3] !=
            "20000000-0000-0000-0000-000000000004" ||
        handshake->terrain_start_height[2] != 20.0f ||
        handshake->terrain_height_range[3] != 11.0f ||
        handshake->region_id != "aaaaaaaa-9999-8888-7777-bbbbbbbbbbbb" ||
        handshake->region_flags_extended != 0x1122334455667788ULL ||
        handshake->region_protocols != 0x8877665544332211ULL)
    {
        std::cerr << "RegionHandshake decode failed\n";
        return 20;
    }

    std::vector<std::uint8_t> movement_payload;
    const auto movement_agent =
        uuid_to_bytes("11111111-2222-3333-4444-555555555555");
    const auto movement_session =
        uuid_to_bytes("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    movement_payload.insert(
        movement_payload.end(),
        movement_agent->begin(),
        movement_agent->end());
    movement_payload.insert(
        movement_payload.end(),
        movement_session->begin(),
        movement_session->end());
    append_f32_vec(movement_payload, 128.0f);
    append_f32_vec(movement_payload, 129.0f);
    append_f32_vec(movement_payload, 30.0f);
    append_f32_vec(movement_payload, 1.0f);
    append_f32_vec(movement_payload, 0.0f);
    append_f32_vec(movement_payload, 0.0f);
    append_u64_vec(movement_payload, 0x0000010000000200ULL);
    append_u32_vec(movement_payload, 123456U);
    const std::string channel = "OpenSim 0.9";
    movement_payload.push_back(
        static_cast<std::uint8_t>(channel.size() & 0xFFU));
    movement_payload.push_back(
        static_cast<std::uint8_t>((channel.size() >> 8U) & 0xFFU));
    movement_payload.insert(
        movement_payload.end(),
        channel.begin(),
        channel.end());

    const auto movement =
        decode_agent_movement_complete(movement_payload);
    if (!movement.has_value() ||
        movement->agent_id != "11111111-2222-3333-4444-555555555555" ||
        movement->session_id != "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee" ||
        movement->position.x != 128.0f ||
        movement->position.y != 129.0f ||
        movement->position.z != 30.0f ||
        movement->region_handle != 0x0000010000000200ULL ||
        movement->timestamp != 123456U ||
        movement->channel_version != "OpenSim 0.9")
    {
        std::cerr << "AgentMovementComplete decode failed\n";
        return 21;
    }

    std::vector<std::uint8_t> parcel_payload;
    append_u32_vec(parcel_payload, 0U);
    append_u32_vec(parcel_payload, 1U);
    parcel_payload.push_back(0U);
    append_u32_vec(parcel_payload, 0U);
    append_u32_vec(parcel_payload, 0U);
    append_u32_vec(parcel_payload, 0U);
    append_u32_vec(parcel_payload, 99U);

    const auto parcel_owner =
        uuid_to_bytes("88888888-8888-8888-8888-888888888888");
    parcel_payload.insert(
        parcel_payload.end(),
        parcel_owner->begin(),
        parcel_owner->end());

    parcel_payload.push_back(0U);
    for (int i = 0; i < 4; ++i)
    {
        append_u32_vec(parcel_payload, 0U);
    }
    parcel_payload.insert(parcel_payload.end(), 24U, 0U);
    parcel_payload.push_back(0U);
    parcel_payload.push_back(0U);

    append_u32_vec(parcel_payload, 4096U);
    parcel_payload.push_back(1U);
    for (int i = 0; i < 8; ++i)
    {
        append_u32_vec(parcel_payload, 0U);
    }
    append_f32_vec(parcel_payload, 1.0f);
    append_u32_vec(parcel_payload, 0U);
    append_u32_vec(parcel_payload, 0x12U);
    append_u32_vec(parcel_payload, 500U);

    auto append_var1 = [&](std::string_view value)
    {
        parcel_payload.push_back(static_cast<std::uint8_t>(value.size()));
        parcel_payload.insert(
            parcel_payload.end(),
            value.begin(),
            value.end());
    };

    append_var1("Nex Parcel");
    append_var1("Parcel Description");
    append_var1("https://music.example/");
    append_var1("https://media.example/");

    parcel_payload.insert(parcel_payload.end(), 16U, 0U);
    parcel_payload.push_back(0U);
    parcel_payload.insert(parcel_payload.end(), 16U, 0U);
    append_u32_vec(parcel_payload, 0U);
    append_f32_vec(parcel_payload, 0.0f);
    parcel_payload.push_back(0U);
    parcel_payload.insert(parcel_payload.end(), 16U, 0U);
    parcel_payload.insert(parcel_payload.end(), 16U, 0U);
    parcel_payload.insert(parcel_payload.end(), 12U, 0U);
    parcel_payload.insert(parcel_payload.end(), 12U, 0U);
    parcel_payload.push_back(0U);
    parcel_payload.insert(parcel_payload.end(), 5U, 0U);

    const auto parcel = decode_parcel_properties(parcel_payload);
    if (!parcel.has_value() ||
        parcel->local_id != 99U ||
        parcel->area != 4096 ||
        parcel->parcel_flags != 0x12U ||
        parcel->sale_price != 500 ||
        parcel->name != "Nex Parcel" ||
        parcel->description != "Parcel Description")
    {
        std::cerr << "ParcelProperties decode failed\n";
        return 18;
    }

    std::vector<std::uint8_t> invalid_parcel = parcel_payload;
    invalid_parcel.pop_back();
    if (decode_parcel_properties(invalid_parcel).has_value())
    {
        std::cerr << "Invalid ParcelProperties unexpectedly accepted\n";
        return 19;
    }

    std::vector<std::uint8_t> incoming_payload;
    incoming_payload.push_back(3U);
    incoming_payload.insert(incoming_payload.end(), {'J','a','m'});

    const auto source = uuid_to_bytes("aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb");
    const auto owner = uuid_to_bytes("cccccccc-1111-2222-3333-dddddddddddd");
    incoming_payload.insert(incoming_payload.end(), source->begin(), source->end());
    incoming_payload.insert(incoming_payload.end(), owner->begin(), owner->end());
    incoming_payload.push_back(1U);
    incoming_payload.push_back(1U);
    incoming_payload.push_back(1U);

    auto append_u32 = [&](std::uint32_t value)
    {
        incoming_payload.push_back(static_cast<std::uint8_t>(value & 0xFFU));
        incoming_payload.push_back(static_cast<std::uint8_t>((value >> 8U) & 0xFFU));
        incoming_payload.push_back(static_cast<std::uint8_t>((value >> 16U) & 0xFFU));
        incoming_payload.push_back(static_cast<std::uint8_t>((value >> 24U) & 0xFFU));
    };

    append_u32(0x42F60000U);
    append_u32(0x43010000U);
    append_u32(0x41C80000U);

    const std::string message = "Test";
    incoming_payload.push_back(4U);
    incoming_payload.push_back(0U);
    incoming_payload.insert(incoming_payload.end(), message.begin(), message.end());

    const auto incoming = decode_chat_from_simulator(incoming_payload);
    if (!incoming.has_value() ||
        incoming->from_name != "Jam" ||
        incoming->message != "Test" ||
        incoming->source_type != 1U)
    {
        std::cerr << "ChatFromSimulator decode failed\n";
        return 5;
    }

    TeleportLocation teleport;
    teleport.region_handle = 0x1122334455667788ULL;
    teleport.position = {128.0f, 64.0f, 30.0f};

    const auto teleport_packet =
        encode_teleport_location_request(12U, session, teleport);
    const auto decoded_teleport = decode_lludp_packet(teleport_packet);
    if (!decoded_teleport.ok() ||
        decoded_teleport.packet->header.message != msg_teleport_location_request ||
        decoded_teleport.packet->payload.size() != 64U)
    {
        std::cerr << "TeleportLocationRequest encoding failed\n";
        return 6;
    }

    std::vector<std::uint8_t> progress_payload(16U, 0U);
    progress_payload.push_back(0x04U);
    progress_payload.push_back(0x03U);
    progress_payload.push_back(0x02U);
    progress_payload.push_back(0x01U);
    progress_payload.push_back(5U);
    progress_payload.insert(progress_payload.end(), {'M','o','v','i','n','g'});

    const auto progress = decode_teleport_progress(progress_payload);
    if (!progress.has_value() ||
        progress->flags != 0x01020304U ||
        progress->message != "Movin")
    {
        std::cerr << "TeleportProgress decode failed\n";
        return 7;
    }

    std::cout << "Viewer message codec tests passed\n";
    return 0;
}
