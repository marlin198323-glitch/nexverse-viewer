#include "nexverse/protocol/movement_controls.hpp"

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
    std::cout << "Avatar controls and rotation tests passed\n";
    return 0;
}
