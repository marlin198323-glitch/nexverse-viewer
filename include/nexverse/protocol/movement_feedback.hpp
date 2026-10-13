/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"
#include <chrono>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <optional>

namespace nexverse::protocol
{
// Independent, server-authoritative evidence of avatar travel.
// Called from both the render/input thread and the LLUDP receive worker;
// never predicts or changes the WorldModel's authoritative position.
enum class MovementEvidence
{
    none, server_position_changed, server_position_stalled
};
class MovementFeedbackMonitor
{
public:
    using TimePoint=std::chrono::steady_clock::time_point;
    void reset()
    {
        std::scoped_lock lock{mutex_};
        latest_.reset();
        baseline_.reset();
        active_=false;
        confirmed_=false;
        stalled_reported_=false;
        samples_=0U;
    }
    void outbound_controls(std::uint32_t flags,TimePoint now)
    {
        constexpr auto moving=control_at_pos|control_at_neg|
                              control_left_pos|control_left_neg;
        const auto direction=flags&moving;
        const bool requested=
            ((direction&control_at_pos)!=0U)!=
            ((direction&control_at_neg)!=0U) ||
            ((direction&control_left_pos)!=0U)!=
            ((direction&control_left_neg)!=0U);
        std::scoped_lock lock{mutex_};
        if(requested && !active_)
        {
            active_=true;
            active_since_=now;
            baseline_=latest_;
            confirmed_=false;
            stalled_reported_=false;
            samples_=0U;
        }
        else if(!requested && active_)
        {
            active_=false;
            baseline_.reset();
            confirmed_=false;
            stalled_reported_=false;
            samples_=0U;
        }
    }
    [[nodiscard]] MovementEvidence server_position(Vec3 position,TimePoint now)
    {
        if(!std::isfinite(position.x)||!std::isfinite(position.y)||
           !std::isfinite(position.z))
            return MovementEvidence::none;
        std::scoped_lock lock{mutex_};
        latest_=position;
        if(!active_)return MovementEvidence::none;
        if(!baseline_)baseline_=position;
        ++samples_;
        const float dx=position.x-baseline_->x;
        const float dy=position.y-baseline_->y;
        const float dz=position.z-baseline_->z;
        // Prevent a single quantization tick from claiming real movement.
        const float distance_squared=dx*dx+dy*dy+dz*dz;
        if(std::isfinite(distance_squared)&&distance_squared>=0.0625f)
        {
            if(confirmed_)return MovementEvidence::none;
            confirmed_=true;
            return MovementEvidence::server_position_changed;
        }
        if(!confirmed_&&!stalled_reported_&&samples_>=3U&&
           now-active_since_>=std::chrono::seconds{4})
        {
            stalled_reported_=true;
            return MovementEvidence::server_position_stalled;
        }
        return MovementEvidence::none;
    }
private:
    std::mutex mutex_;
    std::optional<Vec3> latest_;
    std::optional<Vec3> baseline_;
    TimePoint active_since_{};
    std::uint32_t samples_=0U;
    bool active_=false;
    bool confirmed_=false;
    bool stalled_reported_=false;
};
}
