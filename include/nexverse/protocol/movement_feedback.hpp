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
    none, server_position_changed, server_position_stalled,
    server_position_discontinuity
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
    std::uint32_t discontinuities=0U;
    std::uint32_t stale_samples_ignored=0U;
    bool awaiting_after_discontinuity=false;
    MovementPositionSource confirmed_source=MovementPositionSource::improved_terse;
    // Real simulator moved, but not in the projected direction of
    // the socket-sent command. This is NOT a WASD confirmation.
    std::uint32_t offaxis_server_samples=0U;
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
        last_sample_at_.reset();
        discontinuities_=0U;
        stale_samples_ignored_=0U;
        awaiting_after_discontinuity_=false;
        active_=false;
        confirmed_=false;
        stalled_reported_=false;
        samples_=0U;
        offaxis_server_samples_=0U;
        queued_signals_seen_=false;
        queued_direction_=false;
        expected_direction_.reset();
        last_direction_bits_=0U;
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
    // Historical synthetic unit tests can supply no heading, but a
    // REAL circuit socket callback always gives us the exact BodyRotation
    // that was serialized in this AgentUpdate datagram.
    void outbound_controls(std::uint32_t flags,TimePoint now)
    {
        outbound_controls_impl(flags,now,std::nullopt);
    }
    void outbound_controls(std::uint32_t flags,TimePoint now,
                           Quaternion transmitted_body)
    {
        std::optional<Vec3> expected;
        if(std::isfinite(transmitted_body.x)&&
           std::isfinite(transmitted_body.y)&&
           std::isfinite(transmitted_body.z)&&
           std::isfinite(transmitted_body.w))
        {
            const double x=transmitted_body.x,y=transmitted_body.y,
                z=transmitted_body.z,w=transmitted_body.w;
            const double norm=x*x+y*y+z*z+w*w;
            if(std::isfinite(norm)&&norm>=0.25&&norm<=4.0)
            {
                const double siny=2.0*(w*z+x*y)/norm;
                const double cosy=1.0-2.0*(y*y+z*z)/norm;
                const double angle=std::atan2(siny,cosy);
                const int forward=((flags&control_at_pos)!=0U)-
                    ((flags&control_at_neg)!=0U);
                const int left=((flags&control_left_pos)!=0U)-
                    ((flags&control_left_neg)!=0U);
                const double magnitude=std::hypot(
                    static_cast<double>(forward),static_cast<double>(left));
                if(magnitude>0.0&&std::isfinite(angle))
                {
                    const double vx=(std::cos(angle)*forward-
                                     std::sin(angle)*left)/magnitude;
                    const double vy=(std::sin(angle)*forward+
                                     std::cos(angle)*left)/magnitude;
                    if(std::isfinite(vx)&&std::isfinite(vy))
                        expected=Vec3{static_cast<float>(vx),
                                      static_cast<float>(vy),0.0f};
                }
            }
        }
        // An invalid quaternion cannot authorize directional proof.
        // The send is still counted, but it remains unconfirmed.
        if(!expected&&has_direction(flags))
            expected=Vec3{0.0f,0.0f,0.0f};
        outbound_controls_impl(flags,now,expected);
    }
    [[nodiscard]] MovementFeedbackSnapshot snapshot() const
    {
        std::scoped_lock lock{mutex_};
        return {queued_direction_,active_,confirmed_,stalled_reported_,
                samples_,discontinuities_,stale_samples_ignored_,
                awaiting_after_discontinuity_,confirmed_source_,
                offaxis_server_samples_};
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
        if(offaxis_server_samples_>0U)
            return "WASD: Server bewegt sich quer/entgegen Steuerbefehl";
        if(awaiting_after_discontinuity_)
            return "WASD: Positionssprung/Teleport, neue Serverantwort ausstehend";
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
        // Out-of-order or pre-send position observations must not confirm
        // an input sent later. Keep the last valid baseline unchanged.
        if((last_sample_at_&&now<*last_sample_at_)||
           (active_&&now<active_since_))
        {
            if(stale_samples_ignored_!=UINT32_MAX)++stale_samples_ignored_;
            return MovementEvidence::none;
        }
        // Abrupt location transfers (including teleport) are not evidence
        // of walking. Detect between successive server position samples,
        // independently of the original motion-start baseline.
        if(active_&&latest_)
        {
            const float dx=position.x-latest_->x;
            const float dy=position.y-latest_->y;
            const float dz=position.z-latest_->z;
            const float step_sq=dx*dx+dy*dy+dz*dz;
            constexpr float teleport_like_sq=32.0f*32.0f;
            if(!std::isfinite(step_sq)||step_sq>=teleport_like_sq)
            {
                latest_=position;
                last_sample_at_=now;
                baseline_=position;
                active_since_=now;
                confirmed_=false;
                stalled_reported_=false;
                awaiting_after_discontinuity_=true;
                samples_=0U;
                offaxis_server_samples_=0U;
                if(discontinuities_!=UINT32_MAX)++discontinuities_;
                return MovementEvidence::server_position_discontinuity;
            }
        }
        latest_=position;
        last_sample_at_=now;
        if(!active_)return MovementEvidence::none;
        if(!baseline_)baseline_=position;
        if(samples_!=UINT32_MAX)++samples_;
        const float dx=position.x-baseline_->x;
        const float dy=position.y-baseline_->y;
        // WASD left/right/forward/backward is horizontal. Vertical-only
        // terrain corrections, jumps or gravity do not confirm WASD.
        // Preserve 0.25m horizontal displacement threshold.
        const float distance_squared=dx*dx+dy*dy;
        // For ACTUAL datagrams the server must move at least 0.25 m
        // along the requested world heading/strafe. Perpendicular drift,
        // reverse movement, and a fall in Z are not a WASD ACK.
        bool matches=std::isfinite(distance_squared)&&
                     distance_squared>=0.0625f;
        if(matches&&expected_direction_)
        {
            const float projection=
                dx*expected_direction_->x+dy*expected_direction_->y;
            matches=std::isfinite(projection)&&projection>=0.25f;
        }
        if(!matches&&expected_direction_&&
           (expected_direction_->x!=0.0f||
            expected_direction_->y!=0.0f)&&
           std::isfinite(distance_squared)&&
           distance_squared>=0.0625f&&
           offaxis_server_samples_!=UINT32_MAX)
            ++offaxis_server_samples_;
        if(matches)
        {
            if(confirmed_)return MovementEvidence::none;
            confirmed_=true;
            awaiting_after_discontinuity_=false;
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
    void outbound_controls_impl(
        std::uint32_t flags,TimePoint now,
        std::optional<Vec3> expected)
    {
        const bool requested=has_direction(flags);
        std::scoped_lock lock{mutex_};
        if(queued_signals_seen_&&!queued_direction_)return;
        constexpr auto directional_bits=control_at_pos|control_at_neg|
                         control_left_pos|control_left_neg;
        const auto current_direction_bits=flags&directional_bits;
        bool direction_changed=active_&&requested&&
            current_direction_bits!=last_direction_bits_;
        if(active_&&requested)
        {
            // A reversed/strafe bit requires a new baseline, but tiny
            // normal yaw adjustments DURING W must not reset evidence
            // on every 100-ms datagram and permanently prevent a proof.
            if(expected_direction_.has_value()!=expected.has_value())
                direction_changed=true;
            else if(expected&&expected_direction_ &&
                    expected->x*expected_direction_->x+
                    expected->y*expected_direction_->y<0.98f)
                direction_changed=true;
        }
        if(requested&&(!active_||direction_changed))
        {
            active_=true;
            active_since_=now;
            baseline_=latest_;
            confirmed_=false;
            stalled_reported_=false;
            awaiting_after_discontinuity_=false;
            samples_=0U;
            offaxis_server_samples_=0U;
            expected_direction_=expected;
            last_direction_bits_=current_direction_bits;
        }
        else if(!requested&&active_)clear_motion_locked();
    }
    void clear_motion_locked() noexcept
    {
        active_=false;
        baseline_.reset();
        confirmed_=false;
        stalled_reported_=false;
        samples_=0U;
        offaxis_server_samples_=0U;
        awaiting_after_discontinuity_=false;
        expected_direction_.reset();
        last_direction_bits_=0U;
    }
    mutable std::mutex mutex_;
    std::optional<Vec3> latest_;
    std::optional<Vec3> baseline_;
    std::optional<Vec3> expected_direction_;
    std::uint32_t last_direction_bits_=0U;
    std::optional<TimePoint> last_sample_at_;
    TimePoint active_since_{};
    std::uint32_t samples_=0U;
    std::uint32_t offaxis_server_samples_=0U;
    std::uint32_t discontinuities_=0U;
    std::uint32_t stale_samples_ignored_=0U;
    bool awaiting_after_discontinuity_=false;
    bool active_=false;
    bool queued_signals_seen_=false;
    bool queued_direction_=false;
    bool confirmed_=false;
    bool stalled_reported_=false;
    MovementPositionSource confirmed_source_=
        MovementPositionSource::improved_terse;
};
}
