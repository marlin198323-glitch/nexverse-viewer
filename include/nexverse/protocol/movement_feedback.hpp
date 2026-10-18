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
// OpenSim may send the self pose through multiple LLUDP families;
// each is an actual simulator position, NOT a predicted camera location.
enum class MovementPositionSource
{
    improved_terse, full_object, compressed_object
};
struct MovementFeedbackSnapshot
{
    bool queued_direction=false;
    bool socket_direction_sent=false;
    bool server_confirmed=false;
    bool server_stalled=false;
    std::uint32_t server_samples=0U;
    MovementPositionSource confirmed_source=MovementPositionSource::improved_terse;
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
        queued_signals_seen_=false;
        queued_direction_=false;
    }
    // Enqueued controls may be delayed or replaced before UDP send.
    // They are only a pending UI indication, never motion evidence.
    void queued_controls(std::uint32_t flags)
    {
        std::scoped_lock lock{mutex_};
        queued_signals_seen_=true;
        queued_direction_=has_direction(flags);
        if(!queued_direction_)clear_motion_locked();
    }
    // Called exclusively after actual successful socket.send.
    void outbound_controls(std::uint32_t flags,TimePoint now)
    {
        const bool requested=has_direction(flags);
        std::scoped_lock lock{mutex_};
        if(queued_signals_seen_&&!queued_direction_)return;
        if(requested && !active_)
        {
            active_=true;
            active_since_=now;
            baseline_=latest_;
            confirmed_=false;
            stalled_reported_=false;
            samples_=0U;
        }
        else if(!requested && active_)clear_motion_locked();
    }
    [[nodiscard]] MovementFeedbackSnapshot snapshot() const
    {
        std::scoped_lock lock{mutex_};
        return {queued_direction_,active_,confirmed_,stalled_reported_,
                samples_,confirmed_source_};
    }
    [[nodiscard]] const char* status() const
    {
        std::scoped_lock lock{mutex_};
        if(!active_)
            return queued_direction_?
                "WASD: Richtung wartet auf UDP-Versand":
                "WASD: kein Richtungsbefehl";
        if(confirmed_)
        {
            switch(confirmed_source_)
            {
                case MovementPositionSource::full_object:
                    return "WASD: Serverposition geaendert (ObjectUpdate)";
                case MovementPositionSource::compressed_object:
                    return "WASD: Serverposition geaendert (Compressed)";
                case MovementPositionSource::improved_terse:
                    return "WASD: Serverposition geaendert (Terse)";
            }
        }
        if(stalled_reported_)return "WASD: Serverposition bleibt gleich";
        return "WASD: Serverantwort ausstehend";
    }
    [[nodiscard]] MovementEvidence server_position(
        Vec3 position,TimePoint now,
        MovementPositionSource source=MovementPositionSource::improved_terse)
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
            confirmed_source_=source;
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
    [[nodiscard]] static bool has_direction(std::uint32_t flags) noexcept
    {
        constexpr auto moving=control_at_pos|control_at_neg|
                              control_left_pos|control_left_neg;
        const auto dir=flags&moving;
        return (((dir&control_at_pos)!=0U)!=((dir&control_at_neg)!=0U))||
               (((dir&control_left_pos)!=0U)!=((dir&control_left_neg)!=0U));
    }
    void clear_motion_locked() noexcept
    {
        active_=false;
        baseline_.reset();
        confirmed_=false;
        stalled_reported_=false;
        samples_=0U;
    }
    mutable std::mutex mutex_;
    std::optional<Vec3> latest_;
    std::optional<Vec3> baseline_;
    TimePoint active_since_{};
    std::uint32_t samples_=0U;
    bool active_=false;
    bool queued_signals_seen_=false;
    bool queued_direction_=false;
    bool confirmed_=false;
    bool stalled_reported_=false;
    MovementPositionSource confirmed_source_=
        MovementPositionSource::improved_terse;
};
}
