#include "nexverse/protocol/outbound_queue.hpp"
#include "nexverse/protocol/movement_controls.hpp"

#include <iostream>
#include <limits>

int main()
{
    using namespace nexverse::protocol;

    OutboundCommandQueue queue{2U};

    AgentUpdateData movement;
    movement.control_flags = control_at_pos;

    LocalChatMessage chat;
    chat.message = "Hallo";

    TeleportLocation teleport;
    teleport.region_handle=0x0000010000000200ULL;

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
    // No malformed internal teleport may enter the reliable queue.
    // Rejection must preserve already-enqueued chat/agent commands and
    // must NOT turn a routine bad target into a disconnected circuit.
    queue.clear();
    if(!queue.push(chat)||queue.size()!=1U)return 40;
    for(const float bad:{
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()})
    {
        auto invalid=teleport;
        invalid.position.x=bad;
        if(queue.push(invalid)||queue.size()!=1U)return 41;
        invalid=teleport;
        invalid.look_at.z=bad;
        if(queue.push(invalid)||queue.size()!=1U)return 42;
    }
    auto no_region=teleport;
    no_region.region_handle=0U;
    if(queue.push(no_region)||queue.size()!=1U)return 43;
    const auto preserved=queue.try_pop();
    if(!preserved||!std::holds_alternative<LocalChatMessage>(*preserved)||
       queue.try_pop())return 44;
    auto valid_zero_look=teleport;
    valid_zero_look.look_at={0.0f,0.0f,0.0f};
    if(!queue.push(valid_zero_look)||queue.size()!=1U)return 45;
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
    // When the real 512-capacity outbound queue is full of
    // reliable-only messages, a local TURN/F toggle may not be
    // enqueued. Undo speculative local controller state so the
    // next successful delivery does not jump in yaw or flying.
    OutboundCommandQueue blocked{1U};
    MovementController controller;
    controller.reset();
    MovementKeys turn{};
    turn.turn_left=true;
    if(!blocked.push(chat))return 15;
    const auto old_controller=controller;
    const auto rejected=controller.step(turn,0.25f);
    if(blocked.push(rejected))return 16;
    controller=old_controller;
    blocked.clear();
    const auto requeued=controller.step(turn,0.1f);
    if(!blocked.push(requeued)||
       requeued.body_rotation.z<=0.0f||
       requeued.body_rotation.z>=rejected.body_rotation.z)
        return 17;
    const auto first_valid=blocked.try_pop();
    if(!first_valid||!std::holds_alternative<AgentUpdateData>(*first_valid))
        return 18;
    // A full queue also must not arm F-flight or consume the key edge.
    MovementController fly_controller;
    fly_controller.reset();
    MovementKeys flight{};
    flight.toggle_fly=true;
    OutboundCommandQueue flight_blocked{1U};
    if(!flight_blocked.push(chat))return 19;
    const auto before_flight=fly_controller;
    const auto blocked_flight=fly_controller.step(flight,0.05f);
    if((blocked_flight.control_flags&control_fly)==0U||
       flight_blocked.push(blocked_flight))return 20;
    fly_controller=before_flight;
    if(fly_controller.flying())return 21;
    flight_blocked.clear();
    const auto accepted_flight=fly_controller.step(flight,0.05f);
    if(!flight_blocked.push(accepted_flight)||
       (accepted_flight.control_flags&control_fly)==0U)return 22;
    std::cout << "Outbound command queue tests passed\n";
    return 0;
}
