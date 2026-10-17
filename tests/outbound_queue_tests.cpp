#include "nexverse/protocol/outbound_queue.hpp"

#include <iostream>

int main()
{
    using namespace nexverse::protocol;

    OutboundCommandQueue queue{2U};

    AgentUpdateData movement;
    movement.control_flags = control_at_pos;

    LocalChatMessage chat;
    chat.message = "Hallo";

    TeleportLocation teleport;

    if (!queue.push(movement) ||
        !queue.push(chat) ||
        queue.push(teleport) ||
        queue.size() != 2U)
    {
        std::cerr << "Outbound queue bounds failed\n";
        return 1;
    }

    auto first = queue.try_pop();
    auto second = queue.try_pop();
    if (!first.has_value() ||
        !second.has_value() ||
        !std::holds_alternative<AgentUpdateData>(*first) ||
        !std::holds_alternative<LocalChatMessage>(*second) ||
        queue.try_pop().has_value())
    {
        std::cerr << "Outbound queue order failed\n";
        return 2;
    }

    if (!queue.push(teleport))
    {
        std::cerr << "Outbound queue refill failed\n";
        return 3;
    }
    queue.clear();
    if (queue.size() != 0U)
    {
        std::cerr << "Outbound queue clear failed\n";
        return 3;
    }

    OutboundCommandQueue fast{2U};
    AgentUpdateData old=movement;
    old.control_flags=control_at_neg;
    AgentUpdateData latest=movement;
    latest.control_flags=control_left_pos;
    if(!fast.push(old)||!fast.push(chat)||!fast.push(latest)||
       fast.size()!=2U)return 12;
    const auto reliable=fast.try_pop();
    const auto newest=fast.try_pop();
    if(!reliable||!newest||
       !std::holds_alternative<LocalChatMessage>(*reliable)||
       !std::holds_alternative<AgentUpdateData>(*newest)||
       std::get<AgentUpdateData>(*newest).control_flags!=control_left_pos)
       return 13;
    // No stale AgentUpdate exists in a full reliable-only queue.
    if(!fast.push(chat)||!fast.push(chat)||fast.push(latest))
        return 14;
    std::cout << "Outbound command queue tests passed\n";
    return 0;
}
