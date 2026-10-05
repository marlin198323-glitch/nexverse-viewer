#include "nexverse/protocol/packet_dispatcher.hpp"

#include <cstdint>
#include <iostream>

int main()
{
    using namespace nexverse::protocol;

    PacketDispatcher dispatcher;

    int ping_calls = 0;
    int movement_calls = 0;
    std::uint32_t last_sequence = 0;

    dispatcher.register_handler(
        msg_start_ping_check,
        [&](const DecodedPacket& packet)
        {
            ++ping_calls;
            last_sequence = packet.header.sequence;
        });

    dispatcher.register_handler(
        msg_agent_movement_complete,
        [&](const DecodedPacket&)
        {
            ++movement_calls;
        });

    if (!dispatcher.has_handler(msg_start_ping_check) ||
        !dispatcher.has_handler(msg_agent_movement_complete) ||
        dispatcher.handler_count() != 2U)
    {
        std::cerr << "Dispatcher registration failed\n";
        return 1;
    }

    DecodedPacket ping;
    ping.header.message = msg_start_ping_check;
    ping.header.sequence = 77U;

    if (!dispatcher.dispatch(ping) || ping_calls != 1 || last_sequence != 77U)
    {
        std::cerr << "Dispatcher ping route failed\n";
        return 2;
    }

    DecodedPacket movement;
    movement.header.message = msg_agent_movement_complete;

    if (!dispatcher.dispatch(movement) || movement_calls != 1)
    {
        std::cerr << "Dispatcher movement route failed\n";
        return 3;
    }

    DecodedPacket unknown;
    unknown.header.message = MessageId{MessageFrequency::low, 999U};
    if (dispatcher.dispatch(unknown))
    {
        std::cerr << "Dispatcher unexpectedly handled unknown message\n";
        return 4;
    }

    int fallback_calls = 0;
    dispatcher.set_fallback_handler(
        [&](const DecodedPacket& packet)
        {
            if (packet.header.message == unknown.header.message)
            {
                ++fallback_calls;
            }
        });

    if (!dispatcher.dispatch(unknown) || fallback_calls != 1)
    {
        std::cerr << "Dispatcher fallback failed\n";
        return 5;
    }

    dispatcher.clear_handler(msg_start_ping_check);
    if (dispatcher.has_handler(msg_start_ping_check) ||
        dispatcher.handler_count() != 1U)
    {
        std::cerr << "Dispatcher clear failed\n";
        return 5;
    }

    std::cout << "Packet dispatcher tests passed\n";
    return 0;
}
