/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>

namespace nexverse::protocol
{
struct MovementKeys
{
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool turn_left = false;
    bool turn_right = false;
    bool jump = false;
    bool descend = false;
    bool sprint = false;
    bool toggle_fly = false;
};

class MovementController
{
public:
    void reset(const Quaternion& heading = {})
    {
        yaw_ = std::atan2(
            2.0f * (heading.w * heading.z + heading.x * heading.y),
            1.0f - 2.0f *
                (heading.y * heading.y + heading.z * heading.z));
        if (!std::isfinite(yaw_)) yaw_ = 0.0f;
        fly_ = false;
        previous_fly_key_ = false;
    }

    [[nodiscard]] AgentUpdateData step(
        const MovementKeys& keys, float seconds)
    {
        if (keys.toggle_fly && !previous_fly_key_)
            fly_ = !fly_;
        previous_fly_key_ = keys.toggle_fly;
        const float duration = std::clamp(seconds, 0.0f, 0.25f);
        constexpr float turn_speed_radians = 1.65f;
        if (keys.turn_left != keys.turn_right)
            yaw_ += (keys.turn_left ? 1.0f : -1.0f) *
                    duration * turn_speed_radians;
        if (yaw_ > std::numbers::pi_v<float>)
            yaw_ -= 2.0f * std::numbers::pi_v<float>;
        if (yaw_ < -std::numbers::pi_v<float>)
            yaw_ += 2.0f * std::numbers::pi_v<float>;

        AgentUpdateData out;
        out.body_rotation = {
            0.0f, 0.0f,
            std::sin(yaw_ * 0.5f),
            std::cos(yaw_ * 0.5f)};
        out.head_rotation = out.body_rotation;
        out.control_flags =
            (keys.forward ? control_at_pos : 0U) |
            (keys.backward ? control_at_neg : 0U) |
            (keys.left ? control_left_pos : 0U) |
            (keys.right ? control_left_neg : 0U) |
            (keys.jump ? control_up_pos : 0U) |
            (keys.descend ? control_up_neg : 0U) |
            (keys.turn_left ? control_yaw_neg | control_turn_left : 0U) |
            (keys.turn_right ? control_yaw_pos | control_turn_right : 0U) |
            (fly_ ? control_fly : 0U);
        if (keys.sprint)
            out.control_flags |= control_fast_at |
                control_fast_left | control_fast_up;
        return out;
    }

    [[nodiscard]] bool flying() const noexcept { return fly_; }

private:
    float yaw_ = 0.0f;
    bool fly_ = false;
    bool previous_fly_key_ = false;
};
}
