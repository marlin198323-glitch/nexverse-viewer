#include "nexverse/protocol/viewer_messages.hpp"

#include <cstdint>
#include <iostream>

int main()
{
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
        return 4;
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
