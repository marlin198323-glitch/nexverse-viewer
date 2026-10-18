#include "nexverse/protocol/movement_controls.hpp"
#include "nexverse/protocol/movement_feedback.hpp"

#include <cmath>
#include <iostream>
#include <numbers>

int main()
{
    using namespace nexverse::protocol;
    MovementController c;
    c.reset();
    MovementKeys k;
    k.forward = true;
    auto forward = c.step(k, 0.1f);
    if ((forward.control_flags & control_at_pos) == 0U ||
        (forward.control_flags & control_fly) != 0U ||
        forward.body_rotation.w != 1.0f)
    {
        std::cerr << "W movement control not initialized\n";
        return 1;
    }
    k = {};
    k.left = true;
    k.sprint = true;
    auto strafe = c.step(k, 0.1f);
    if ((strafe.control_flags & control_left_pos) == 0U ||
        (strafe.control_flags & control_fast_left) == 0U)
    {
        std::cerr << "Strafe/sprint protocol bits incorrect\n";
        return 2;
    }
    k = {};
    k.toggle_fly = true;
    auto flying = c.step(k, 0.1f);
    if (!c.flying() || (flying.control_flags & control_fly) == 0U)
    {
        std::cerr << "Fly toggle did not arm\n";
        return 3;
    }
    (void)c.step(k, 0.1f); // holding F should not toggle a second time
    if (!c.flying())
    {
        std::cerr << "Fly key bounced while held\n";
        return 4;
    }
    k.toggle_fly = false;
    (void)c.step(k, 0.1f);
    k.toggle_fly = true;
    flying = c.step(k, 0.1f);
    if (c.flying() || (flying.control_flags & control_fly) != 0U)
    {
        std::cerr << "Flight cannot be disabled on new press\n";
        return 5;
    }
    k = {};
    k.turn_left = true;
    auto left = c.step(k, 0.2f);
    if ((left.control_flags & control_turn_left) == 0U ||
        (left.control_flags & control_yaw_neg) == 0U ||
        left.body_rotation.z <= 0.0f ||
        std::abs(left.body_rotation.z * left.body_rotation.z +
                 left.body_rotation.w * left.body_rotation.w - 1.0f) > 0.0001f ||
        left.body_rotation.z != left.head_rotation.z)
    {
        std::cerr << "Left rotation quaternion / control flags incorrect\n";
        return 6;
    }
    k = {};
    k.jump = true;
    k.descend = true;
    auto vertical = c.step(k, 0.1f);
    if ((vertical.control_flags & control_up_pos) == 0U ||
        (vertical.control_flags & control_up_neg) == 0U)
    {
        std::cerr << "Jump/down input missing\n";
        return 7;
    }
    c.reset();
    if (c.flying())
    {
        std::cerr << "Region reset did not clear fly mode\n";
        return 8;
    }
    c.reset({0.0f, 0.0f, std::sin(std::numbers::pi_v<float>/4.0f),
            std::cos(std::numbers::pi_v<float>/4.0f)});
    const auto heading = c.step({}, 0.1f);
    if (std::abs(heading.body_rotation.z -
                 std::sin(std::numbers::pi_v<float>/4.0f)) > 0.0001f)
    {
        std::cerr << "Initial simulator heading not preserved\n";
        return 9;
    }
    // Server-side motion verification: input bits are not sufficient.
    // Only an actual server position delta >=0.25m can confirm movement.
    using Clock=std::chrono::steady_clock;
    const auto t=Clock::time_point{};
    MovementFeedbackMonitor feedback;
    if(feedback.server_position({10.0f,20.0f,30.0f},t)!=
       MovementEvidence::none)return 10;
    feedback.outbound_controls(control_turn_left,t); // rotate only
    if(feedback.server_position({10.0f,20.0f,30.0f},
                                t+std::chrono::seconds{5})!=
       MovementEvidence::none)return 11;
    // Use monotonic arrivals: a turn-only simulator sample was observed
    // at +5s, so the subsequent WASD send cannot predate it. This guards
    // against a test fixture falsely simulating time travel on the wire.
    feedback.outbound_controls(control_at_pos,t+std::chrono::seconds{6});
    for(int i=7;i<=8;++i)
        if(feedback.server_position({10.0f,20.0f,30.0f},
                      t+std::chrono::seconds{i})!=MovementEvidence::none)
            return 12;
    if(feedback.server_position({10.0f,20.0f,30.0f},
               t+std::chrono::seconds{11})!=
       MovementEvidence::server_position_stalled)return 13;
    if(feedback.server_position({10.0f,20.0f,30.0f},
               t+std::chrono::seconds{12})!=MovementEvidence::none)
        return 14; // stalled report deduplicated
    if(feedback.server_position({10.0f,20.3f,30.0f},
               t+std::chrono::seconds{13})!=
       MovementEvidence::server_position_changed)return 15;
    if(feedback.server_position({10.0f,20.6f,30.0f},
               t+std::chrono::seconds{14})!=MovementEvidence::none)
        return 16; // confirmed report deduplicated
    feedback.outbound_controls(0U,t+std::chrono::seconds{15});
    feedback.outbound_controls(control_at_pos,t+std::chrono::seconds{16});
    if(feedback.server_position({10.3f,20.6f,30.0f},
               t+std::chrono::seconds{17})!=
       MovementEvidence::server_position_changed)return 17;
    feedback.reset();
    if(feedback.server_position({20.0f,20.0f,30.0f},
               t+std::chrono::seconds{30})!=MovementEvidence::none)
        return 18; // teleports are not WASD evidence
    feedback.outbound_controls(control_at_pos|control_at_neg,
               t+std::chrono::seconds{32});
    if(feedback.server_position({20.0f,20.0f,30.0f},
               t+std::chrono::seconds{40})!=MovementEvidence::none)
        return 19; // opposing controls cancel
    std::cout << "Avatar controls and rotation tests passed\n";
    return 0;
}
