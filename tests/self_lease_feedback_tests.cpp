/* MPL-2.0: confirmed WASD/yaw must not survive a self avatar
   KillObject or implicit simulator LocalID rebinding. */
#include "nexverse/protocol/movement_feedback.hpp"
#include "nexverse/protocol/turn_readback.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <string_view>
using namespace nexverse::protocol;
int main()
{
    using namespace std::chrono_literals;
    const auto t=std::chrono::steady_clock::time_point{};
    MovementFeedbackMonitor motion;
    TurnReadbackMonitor turn;
    const auto q=[](float radians)->Quaternion {
        return {0.0f,0.0f,std::sin(radians*0.5f),
                std::cos(radians*0.5f)};
    };
    (void)motion.server_position({10,20,30},t);
    (void)turn.observe_server_rotation(q(0.0f),t);
    motion.queued_controls(control_at_pos);
    motion.outbound_controls(control_at_pos,t+1s);
    turn.outbound_controls(control_turn_left|control_yaw_pos,t+1s);
    if(motion.server_position({10.6f,20,30},t+2s)!=
       MovementEvidence::server_position_changed ||
       !turn.observe_server_rotation(q(0.3f),t+2s) ||
       !motion.snapshot().server_confirmed || !turn.server_confirmed())
        return 1;
    // The production dispatcher resets BOTH monitors when the known
    // self root is killed or its LocalID changes.
    motion.reset();turn.reset();
    if(motion.snapshot().server_confirmed||turn.server_confirmed()||
       motion.snapshot().socket_direction_sent||turn.turn_active()||
       std::string_view(motion.status()).find("kein Richtungsbefehl")==
           std::string_view::npos)return 2;
    // Old late sample after the reset is not evidence by itself.
    if(motion.server_position({12,20,30},t+3s)!=MovementEvidence::none||
       turn.observe_server_rotation(q(0.5f),t+3s)||
       motion.snapshot().server_confirmed||turn.server_confirmed())
        return 3;
    // A new generation requires a NEW actual socket send AND readback.
    motion.queued_controls(control_at_pos);
    motion.outbound_controls(control_at_pos,t+4s);
    turn.outbound_controls(control_turn_left|control_yaw_pos,t+4s);
    if(motion.server_position({12.5f,20,30},t+5s)!=
       MovementEvidence::server_position_changed ||
       !turn.observe_server_rotation(q(0.8f),t+5s) ||
       !motion.snapshot().server_confirmed || !turn.server_confirmed())
        return 4;
    std::cout<<"SELF_LEASE_KILL_AND_REBIND_MOVEMENT_TURN_RESET_PASS\n";
}
