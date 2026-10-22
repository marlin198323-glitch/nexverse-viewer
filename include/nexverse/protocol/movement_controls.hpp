/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/movement_frame_safety.hpp"
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
        yaw_ = valid_initial_agent_yaw(heading);
        fly_ = false;
        previous_fly_key_ = false;
    }

    // A simulator can rotate the avatar independently (region handoff,
    // collision or scripted action). Resync the yaw baseline without
    // clearing flight/toggle edge state; never adopt an unconfirmed local
    // preview as server authority.
    void adopt_server_heading(const Quaternion& heading) noexcept
    {
        yaw_ = valid_initial_agent_yaw(heading);
    }

    [[nodiscard]] AgentUpdateData step(
        const MovementKeys& keys, float seconds)
    {
        if (keys.toggle_fly && !previous_fly_key_)
            fly_ = !fly_;
        previous_fly_key_ = keys.toggle_fly;
        const float duration = bounded_agent_update_seconds(seconds);
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
        const bool rotate_left=keys.turn_left&&!keys.turn_right;
        const bool rotate_right=keys.turn_right&&!keys.turn_left;
        // Holding opposite translation keys must not emit mutually
        // exclusive simulator motion flags. Keep valid combinations
        // (for example W+A) for standard diagonal navigation.
        const bool move_forward=keys.forward&&!keys.backward;
        const bool move_backward=keys.backward&&!keys.forward;
        const bool strafe_left=keys.left&&!keys.right;
        const bool strafe_right=keys.right&&!keys.left;
        out.control_flags =
            (move_forward ? control_at_pos : 0U) |
            (move_backward ? control_at_neg : 0U) |
            (strafe_left ? control_left_pos : 0U) |
            (strafe_right ? control_left_neg : 0U) |
            (keys.jump ? control_up_pos : 0U) |
            (keys.descend ? control_up_neg : 0U) |
            // OpenGenesisLINK ScenePresence.cs: TURN_LEFT is paired
            // with YAW_POS, TURN_RIGHT with YAW_NEG. Sending the opposite
            // yaw sign makes the server's flying-roll/rotation disagree
            // with the local heading even if our quaternion is correct.
            (rotate_left ? control_yaw_pos | control_turn_left : 0U) |
            (rotate_right ? control_yaw_neg | control_turn_right : 0U) |
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
