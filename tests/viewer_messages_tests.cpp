#include "nexverse/protocol/viewer_messages.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <iostream>

int main()
{
    // Legacy ParticleSystem: 68-byte system + 18-byte particle data.
    std::vector<std::uint8_t> ps(86U, 0U);
    ps[8U] = 0x02U; // explode pattern
    ps[15U] = 0x80U; // burst rate: 0.5s (LE /256)
    ps[17U] = 0x00U;
    ps[18U] = 0x01U; // burst radius 1.0
    ps[19U] = 0x00U;
    ps[20U] = 0x01U; // speed minimum 1.0
    ps[21U] = 0x00U;
    ps[22U] = 0x02U; // speed maximum 2.0
    ps[23U] = 4U;
    ps[72U] = 0x00U;
    ps[73U] = 0x03U; // max age 3.0
    ps[74U] = 220U;
    ps[75U] = 120U;
    ps[76U] = 50U;
    ps[77U] = 255U;
    ps[78U] = 70U;
    ps[79U] = 70U;
    ps[80U] = 70U;
    ps[81U] = 0U;
    ps[82U] = 16U;
    ps[83U] = 16U; // scale 0.5m
    ps[84U] = 32U;
    ps[85U] = 32U; // scale 1m
    const auto target_uuid =
        nexverse::protocol::uuid_to_bytes(
            "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    ps.insert(ps.begin() + 52, target_uuid->begin(), target_uuid->end());
    ps.erase(ps.begin() + 68, ps.begin() + 84);
    const auto decoded_ps =
        nexverse::protocol::parse_particle_system(ps);
    if (!decoded_ps || decoded_ps->pattern != 0x02U ||
        decoded_ps->burst_count != 4U ||
        decoded_ps->burst_rate != 0.5f ||
        decoded_ps->burst_radius != 1.0f ||
        decoded_ps->particle_max_age != 3.0f ||
        decoded_ps->start_scale != 0.5f ||
        decoded_ps->end_scale != 1.0f ||
        decoded_ps->start_color[0] != 220.0f / 255.0f ||
        decoded_ps->target_id !=
            "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee" ||
        decoded_ps->blend_source != 7U ||
        decoded_ps->blend_destination != 9U)
    {
        std::cerr << "Legacy OpenSim PSBlock decode failed\n";
        return 98;
    }
    auto invalid_ps = ps;
    invalid_ps.pop_back();
    if (nexverse::protocol::parse_particle_system(invalid_ps))
    {
        std::cerr << "Truncated particle system accepted\n";
        return 99;
    }
    // Extended PSBlock with system/particle length headers.
    std::vector<std::uint8_t> extended;
    extended.insert(extended.end(), {68U,0U,0U,0U});
    extended.insert(extended.end(), ps.begin(), ps.begin()+68);
    extended.insert(extended.end(), {18U,0U,0U,0U});
    extended.insert(extended.end(), ps.begin()+68, ps.end());
    if (!nexverse::protocol::parse_particle_system(extended))
    {
        std::cerr << "Extended OpenSim PSBlock decode failed\n";
        return 100;
    }
    extended[72U] = 22U;
    extended[76U] = 0x00U;
    extended[77U] = 0x00U;
    extended[78U] = 0x03U; // DataGlow | DataBlend
    extended[79U] = 0x00U;
    extended.insert(extended.end(), {20U,150U,7U,8U});
    const auto extended_ps =
        nexverse::protocol::parse_particle_system(extended);
    if (!extended_ps ||
        extended_ps->end_glow != 150.0f / 255.0f ||
        extended_ps->blend_source != 7U ||
        extended_ps->blend_destination != 8U)
    {
        std::cerr << "Glow/blend extended PSBlock decode failed\n";
        return 101;
    }
    extended.back() = 255U;
    if (nexverse::protocol::parse_particle_system(extended))
    {
        std::cerr << "Invalid particle blend enum accepted\n";
        return 103;
    }
    extended.back() = 8U;
    extended[72U] = 25U; // invalid size (must be <=22)
    if (nexverse::protocol::parse_particle_system(extended))
    {
        std::cerr << "Oversized PSBlock accepted\n";
        return 102;
    }

    // OpenSim ExtraParams 0x20, canonical 16-byte LightData wire format.
    // RGB and intensity are UNORM8; radius/cutoff/falloff are LE float32.
    const std::vector<std::uint8_t> light_extra{
        1U, 0x20U, 0U, 16U, 0U, 0U, 0U,
        255U, 128U, 64U, 192U,
        0U, 0U, 0x20U, 0x41U,  // radius 10
        0U, 0U, 0x80U, 0x3eU,  // cutoff .25
        0U, 0U, 0U, 0x3fU};    // falloff .5
    const auto point_light =
        nexverse::protocol::parse_point_light_extra_params(light_extra);
    if (!point_light ||
        point_light->red != 1.0f ||
        std::abs(point_light->green - 128.0f / 255.0f) > 0.0001f ||
        std::abs(point_light->intensity - 192.0f / 255.0f) > 0.0001f ||
        point_light->radius != 10.0f ||
        point_light->cutoff != 0.25f ||
        point_light->falloff != 0.5f)
    {
        std::cerr << "OpenSim point light ExtraParams decode failed\n";
        return 95;
    }
    auto truncated_light = light_extra;
    truncated_light.pop_back();
    if (nexverse::protocol::parse_point_light_extra_params(
            truncated_light).has_value())
    {
        std::cerr << "Malformed point light ExtraParams accepted\n";
        return 96;
    }
    auto invalid_light = light_extra;
    invalid_light[13U] = 0xc0U;
    invalid_light[14U] = 0x7fU; // NaN radius
    if (nexverse::protocol::parse_point_light_extra_params(
            invalid_light).has_value())
    {
        std::cerr << "Nonfinite point-light radius accepted\n";
        return 97;
    }

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
        decoded_agent.packet->payload.size() != 114U)
    {
        std::cerr << "AgentUpdate encoding failed\n";
        return 1;
    }

    // AgentUpdate High 4: 2*UUID=32, 2*LLQuaternion=24,
    // State=1, four LLVector3=48, Far=4, ControlFlags=4, Flags=1.
    // Catch byte alignment bugs that would make server-side motion
    // remain static even though WASD key events are emitted locally.
    const auto& data=decoded_agent.packet->payload;
    const auto control_offset=109U;
    const std::uint32_t flags=
        static_cast<std::uint32_t>(data[control_offset]) |
        (static_cast<std::uint32_t>(data[control_offset+1U])<<8U) |
        (static_cast<std::uint32_t>(data[control_offset+2U])<<16U) |
        (static_cast<std::uint32_t>(data[control_offset+3U])<<24U);
    if(flags!=(control_at_pos|control_fly) || data[56U]!=0U ||
       data[113U]!=0U)
    {
        std::cerr<<"AgentUpdate quaternion layout shifted ControlFlags\n";
        return 104;
    }

    // Golden field map from Firestorm message_template.msg for High 4:
    // two quaternion vectors (3 * F32 each), state U8, four camera vec3,
    // Far F32, ControlFlags U32, Flags U8. Every offset is independent
    // of the viewer decoder: a parse-only test would miss symmetry bugs.
    update.body_rotation={0.25f,-0.5f,0.75f,1.0f};
    update.head_rotation={0.1f,0.2f,0.3f,1.0f};
    update.state=0x5aU;
    update.camera_at_axis={0.7f,0.0f,0.0f};
    update.far_clip=192.5f;
    update.flags=0x81U;
    const auto exact=decode_lludp_packet(
        encode_agent_update(42U,session,update));
    if(!exact.ok() || exact.packet->payload.size()!=114U)return 105;
    const auto& golden=exact.packet->payload;
    const auto f32=[&](std::size_t offset)
    {
        const std::uint32_t raw=
            static_cast<std::uint32_t>(golden[offset]) |
            (static_cast<std::uint32_t>(golden[offset+1U])<<8U) |
            (static_cast<std::uint32_t>(golden[offset+2U])<<16U) |
            (static_cast<std::uint32_t>(golden[offset+3U])<<24U);
        return std::bit_cast<float>(raw);
    };
    if(f32(32U)!=0.25f || f32(36U)!=-0.5f ||
       f32(40U)!=0.75f || f32(44U)!=0.1f ||
       f32(48U)!=0.2f || f32(52U)!=0.3f ||
       golden[56U]!=0x5aU ||
       f32(57U)!=128.0f || f32(61U)!=128.0f ||
       f32(65U)!=30.0f || f32(69U)!=0.7f ||
       f32(105U)!=192.5f || golden[113U]!=0x81U)
    {
        std::cerr<<"AgentUpdate field offsets mismatch Firestorm wire\n";
        return 106;
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

    // OpenGenesisLINK's latest cross-region llListen fix requires no
    // special viewer packet. Standard ChatFromViewer type bytes remain
    // whisper=0, say=1, shout=2 (channel 0); server decides distance.
    for(const auto& [type,wire_byte]:std::array{
        std::pair{LocalChatType::whisper,std::uint8_t{0}},
        std::pair{LocalChatType::normal,std::uint8_t{1}},
        std::pair{LocalChatType::shout,std::uint8_t{2}}})
    {
        outgoing.type=type;
        const auto raw=encode_chat_from_viewer(15U,session,outgoing);
        const auto decoded=decode_lludp_packet(raw);
        if(!decoded.ok()||
           decoded.packet->header.message!=msg_chat_from_viewer||
           decoded.packet->payload.size()<5U)
            return 122;
        const auto& chat_wire=decoded.packet->payload;
        if(chat_wire[chat_wire.size()-5U]!=wire_byte||
           chat_wire[chat_wire.size()-4U]!=0U||
           chat_wire[chat_wire.size()-3U]!=0U||
           chat_wire[chat_wire.size()-2U]!=0U||
           chat_wire[chat_wire.size()-1U]!=0U)
            return 123;
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
    full_update_payload.push_back(0U); // MediaURL
    full_update_payload.push_back(86U); // PSBlock (Variable1)
    full_update_payload.insert(
        full_update_payload.end(), ps.begin(), ps.end());
    full_update_payload.push_back(0U); // ExtraParams
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
        !full_update->objects[0].particle_system.has_value() ||
        full_update->objects[0].particle_system->burst_count != 4U ||
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

    // A complete ObjectUpdateCompressed must read variable ExtraParams
    // before 23-byte prim shape and 32-bit TextureEntry length.
    std::vector<std::uint8_t> mesh_data = compressed_data;
    mesh_data.resize(84U, 0U); // compressed flags + owner UUID
    put_f32(mesh_data, 28U, 2.0f);
    put_f32(mesh_data, 32U, 3.0f);
    put_f32(mesh_data, 36U, 4.0f);
    put_u32(mesh_data, 64U, 0x28U); // HasParent + PSBlock
    append_u32_vec(mesh_data, 123U);
    mesh_data.insert(mesh_data.end(), ps.begin(), ps.end());
    mesh_data.push_back(1U); // one ExtraParam
    mesh_data.insert(mesh_data.end(), {0x30U, 0U}); // Sculpt type
    append_u32_vec(mesh_data, 17U);
    const auto mesh_asset_id =
        uuid_to_bytes("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");
    mesh_data.insert(
        mesh_data.end(), mesh_asset_id->begin(), mesh_asset_id->end());
    mesh_data.push_back(0x05U); // mesh sculpt type
    std::vector<std::uint8_t> compressed_shape(23U, 0U);
    compressed_shape[0U] = 0x10U; // straight-path profile
    compressed_shape[16U] = 0x01U; // square profile
    mesh_data.insert(
        mesh_data.end(), compressed_shape.begin(), compressed_shape.end());
    append_u32_vec(mesh_data,
        static_cast<std::uint32_t>(object_texture_entry.size()));
    mesh_data.insert(
        mesh_data.end(), object_texture_entry.begin(), object_texture_entry.end());

    std::vector<std::uint8_t> compressed_mesh_payload{
        0x88U,0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,
        0x34U,0x12U,1U};
    append_u32_vec(compressed_mesh_payload, 0U);
    compressed_mesh_payload.push_back(
        static_cast<std::uint8_t>(mesh_data.size() & 0xFFU));
    compressed_mesh_payload.push_back(
        static_cast<std::uint8_t>((mesh_data.size() >> 8U) & 0xFFU));
    compressed_mesh_payload.insert(
        compressed_mesh_payload.end(), mesh_data.begin(), mesh_data.end());

    const auto mesh_update =
        decode_object_update_compressed(compressed_mesh_payload);
    if (!mesh_update || mesh_update->objects.size() != 1U ||
        !mesh_update->objects[0].has_complete_prim_data ||
        !mesh_update->objects[0].is_mesh() ||
        mesh_update->objects[0].sculpt_asset_id !=
            "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee" ||
        mesh_update->objects[0].parent_id != 123U ||
        mesh_update->objects[0].scale.x != 2.0f ||
        mesh_update->objects[0].scale.y != 3.0f ||
        mesh_update->objects[0].scale.z != 4.0f ||
        mesh_update->objects[0].shape.path_curve != 0x10U ||
        mesh_update->objects[0].shape.profile_shape() != 0x01U ||
        !mesh_update->objects[0].particle_system.has_value() ||
        mesh_update->objects[0].particle_system->burst_count != 4U ||
        !mesh_update->objects[0].default_texture ||
        mesh_update->objects[0].default_texture->texture_id !=
            "89556747-24cb-43ed-920b-47caed15465f")
    {
        std::cerr << "Full compressed mesh object decode failed\n";
        return 93;
    }
    auto truncated_compressed_mesh = compressed_mesh_payload;
    truncated_compressed_mesh.pop_back();
    if (decode_object_update_compressed(truncated_compressed_mesh))
    {
        std::cerr << "Truncated compressed mesh object accepted\n";
        return 94;
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

    // Firestorm modern Trusted Low 158 adds AppearanceHover Variable
    // (one LLVector3 per block) and AttachmentBlock Variable (UUID+U8).
    auto complete_appearance=appearance_payload;
    complete_appearance.push_back(1U); // one hover record
    append_u32_vec(complete_appearance,0x00000000U);
    append_u32_vec(complete_appearance,0x00000000U);
    append_u32_vec(complete_appearance,0x3e800000U); // +0.25f visual hover
    complete_appearance.push_back(1U);
    const auto attachment_id=uuid_to_bytes(
        "66666666-6666-6666-6666-666666666666");
    complete_appearance.insert(
        complete_appearance.end(),attachment_id->begin(),attachment_id->end());
    complete_appearance.push_back(5U);
    const auto modern=decode_avatar_appearance(complete_appearance);
    if(!modern || modern->hover_heights.size()!=1U ||
       modern->hover_heights.front().z!=0.25f ||
       modern->attachments.size()!=1U ||
       modern->attachments.front().object_id!=
           "66666666-6666-6666-6666-666666666666" ||
       modern->attachments.front().attachment_point!=5U)
        return 107;
    auto empty_tail=appearance_payload;
    empty_tail.push_back(0U); // zero hover
    empty_tail.push_back(0U); // zero attachments
    const auto empty_appearance=decode_avatar_appearance(empty_tail);
    if(!empty_appearance || !empty_appearance->attachments.empty() ||
       !empty_appearance->hover_heights.empty())return 108;
    auto truncated=complete_appearance;
    truncated.pop_back();
    if(decode_avatar_appearance(truncated))return 109;
    truncated=complete_appearance;
    truncated.resize(appearance_payload.size()+1U); // missing second block
    if(decode_avatar_appearance(truncated))return 110;
    auto nan_hover=complete_appearance;
    const auto hover_begin=appearance_payload.size()+1U;
    nan_hover[hover_begin+8U]=0U;
    nan_hover[hover_begin+9U]=0U;
    nan_hover[hover_begin+10U]=0xc0U;
    nan_hover[hover_begin+11U]=0x7fU; // quiet NaN
    if(decode_avatar_appearance(nan_hover))return 111;
    auto excessive=complete_appearance;
    excessive[appearance_payload.size()]=17U;
    if(decode_avatar_appearance(excessive))return 112;
    excessive=complete_appearance;
    excessive[appearance_payload.size()+1U+12U]=65U;
    if(decode_avatar_appearance(excessive))return 113;
    excessive=complete_appearance;
    excessive.push_back(0x5aU);
    if(decode_avatar_appearance(excessive))return 114;

    AvatarAppearanceFailure why{};
    if(decode_avatar_appearance({},&why)||why!=AvatarAppearanceFailure::header)return 115;
    auto broken=appearance_payload;
    broken[17]=255;broken[18]=127;
    if(decode_avatar_appearance(broken,&why)||why!=AvatarAppearanceFailure::texture_entry)return 116;
    broken=appearance_payload;broken[22]=255;
    if(decode_avatar_appearance(broken,&why)||why!=AvatarAppearanceFailure::visual_params)return 117;
    broken=appearance_payload;broken[26]=255;
    if(decode_avatar_appearance(broken,&why)||why!=AvatarAppearanceFailure::appearance_data)return 118;
    if(!decode_avatar_appearance(complete_appearance,&why)||why!=AvatarAppearanceFailure::none)return 119;
    if(decode_avatar_appearance(nan_hover,&why)||why!=AvatarAppearanceFailure::hover)return 120;
    if(decode_avatar_appearance(excessive,&why)||why!=AvatarAppearanceFailure::attachments)return 121;

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
