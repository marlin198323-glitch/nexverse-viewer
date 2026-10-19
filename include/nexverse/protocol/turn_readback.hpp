/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <chrono>
#include <cmath>
#include <mutex>
#include <optional>
#include <string_view>

namespace nexverse::protocol
{
// A turn is only confirmed by a simulator-sourced SELF quaternion change
// AFTER a real AgentUpdate socket send. The local predicted heading and
// queued input are never passed to observe_server_rotation().
class TurnReadbackMonitor
{
public:
    using TimePoint=std::chrono::steady_clock::time_point;
    void reset()
    {
        std::scoped_lock lock{mutex_};
        latest_.reset();baseline_.reset();last_sample_at_.reset();
        active_=false;confirmed_=false;direction_=0;
    }
    void outbound_controls(std::uint32_t flags,TimePoint now)
    {
        std::scoped_lock lock{mutex_};
        const int direction=
            (flags&(control_turn_left|control_yaw_pos))==
                (control_turn_left|control_yaw_pos)&&
            (flags&(control_turn_right|control_yaw_neg))==0U ?1:
            (flags&(control_turn_right|control_yaw_neg))==
                (control_turn_right|control_yaw_neg)&&
            (flags&(control_turn_left|control_yaw_pos))==0U ?-1:0;
        if(direction==0)
        {
            active_=false;confirmed_=false;direction_=0;
            baseline_.reset();
            return;
        }
        if(!active_||direction_!=direction)
        {
            active_=true;confirmed_=false;direction_=direction;
            sent_at_=now;baseline_=latest_;
        }
    }
    [[nodiscard]] bool observe_server_rotation(Quaternion q,TimePoint now)
    {
        if(!std::isfinite(q.x)||!std::isfinite(q.y)||
           !std::isfinite(q.z)||!std::isfinite(q.w))return false;
        const float norm_sq=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
        if(!std::isfinite(norm_sq)||norm_sq<0.25f||norm_sq>4.0f)
            return false;
        const float inv=1.0f/std::sqrt(norm_sq);
        q.x*=inv;q.y*=inv;q.z*=inv;q.w*=inv;
        const float yaw=std::atan2(
            2.0f*(q.w*q.z+q.x*q.y),
            1.0f-2.0f*(q.y*q.y+q.z*q.z));
        if(!std::isfinite(yaw))return false;
        std::scoped_lock lock{mutex_};
        if(last_sample_at_&&now<*last_sample_at_)return false;
        last_sample_at_=now;latest_=yaw;
        if(!active_||now<sent_at_)return false;
        if(!baseline_){baseline_=yaw;return false;}
        const float delta=std::atan2(
            std::sin(yaw-*baseline_),std::cos(yaw-*baseline_));
        // 5 degrees; direction as well as magnitude must match the
        // transmitted yaw bit (counter-clockwise is positive).
        if(!confirmed_&&delta*static_cast<float>(direction_)>
                          0.08726646f)
        {
            confirmed_=true;
            return true;
        }
        return false;
    }
    [[nodiscard]] bool turn_active() const
    {
        std::scoped_lock lock{mutex_};return active_;
    }
    [[nodiscard]] bool server_confirmed() const
    {
        std::scoped_lock lock{mutex_};return active_&&confirmed_;
    }
    [[nodiscard]] const char* status() const
    {
        std::scoped_lock lock{mutex_};
        if(!active_)return "Drehung: kein Befehl";
        if(confirmed_)return direction_>0
            ?"Links: Serverrotation bestaetigt"
            :"Rechts: Serverrotation bestaetigt";
        return direction_>0
            ?"Links: Serverrotation ausstehend"
            :"Rechts: Serverrotation ausstehend";
    }
private:
    mutable std::mutex mutex_;
    std::optional<float> latest_,baseline_;
    std::optional<TimePoint> last_sample_at_;
    TimePoint sent_at_{};
    int direction_=0;
    bool active_=false,confirmed_=false;
};
}
