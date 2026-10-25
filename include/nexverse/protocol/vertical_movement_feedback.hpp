/* This Source Code Form is subject to the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include <chrono>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <optional>
namespace nexverse::protocol
{
// Verifies only server-origin SELF vertical position changes after an
// actual socket-sent AgentUpdate. It does not infer flight eligibility,
// collision outcome, or server ACK from a local animation/camera preview.
struct VerticalMovementSnapshot
{
    bool queued=false, sent=false, confirmed=false, stalled=false;
    int requested_sign=0;
    std::uint32_t server_samples=0U, contradictory_samples=0U;
    bool awaiting_fresh_udp_after_jump=false;
};
class VerticalMovementFeedbackMonitor
{
public:
    using TimePoint=std::chrono::steady_clock::time_point;
    void reset()
    {
        std::scoped_lock lock{mutex_};
        latest_.reset();last_sample_at_.reset();baseline_.reset();
        queued_=false;queued_seen_=false;sign_=0;sent_=false;
        confirmed_=false;stalled_=false;samples_=0U;
        contradictory_samples_=0U;last_fly_=false;
        awaiting_fresh_udp_after_jump_=false;
    }
    void queued_controls(std::uint32_t flags)
    {
        std::scoped_lock lock{mutex_};
        queued_seen_=true;
        queued_=direction(flags)!=0;
        if(!queued_)
        {
            clear_active();
            awaiting_fresh_udp_after_jump_=false;
        }
    }
    void outbound_controls(std::uint32_t flags,TimePoint now)
    {
        std::scoped_lock lock{mutex_};
        const auto requested=direction(flags);
        if(queued_seen_&&!queued_)return; // superseded queue entry
        if(requested==0)
        {
            clear_active();
            return;
        }
        const bool fly=(flags&control_fly)!=0U;
        if(!sent_||requested!=sign_||fly!=last_fly_)
        {
            sign_=requested;last_fly_=fly;sent_=true;
            confirmed_=false;stalled_=false;samples_=0U;
            contradictory_samples_=0U;baseline_=latest_;
            sent_at_=now;
            awaiting_fresh_udp_after_jump_=false;
        }
    }
    [[nodiscard]] VerticalMovementSnapshot snapshot() const
    {
        std::scoped_lock lock{mutex_};
        return {queued_,sent_,confirmed_,stalled_,sign_,
                samples_,contradictory_samples_,
                awaiting_fresh_udp_after_jump_};
    }
    [[nodiscard]] const char* status() const
    {
        std::scoped_lock lock{mutex_};
        if(!sent_)
        {
            if(awaiting_fresh_udp_after_jump_)
                return "Hoehe: Positionssprung/Teleport, neuer UDP-Versand ausstehend";
            return queued_?
                "Hoehe: Befehl wartet auf UDP":
                "Hoehe: kein Steig-/Sinkbefehl";
        }
        if(confirmed_)return sign_>0?
            "Steigen/Sprung: Simulatorhoehe bestaetigt":
            "Sinken: Simulatorhoehe bestaetigt";
        if(stalled_)return "Hoehe: Simulatorantwort bleibt aus";
        if(contradictory_samples_>0U)
            return "Hoehe: Simulator bewegt entgegen Steig-/Sinkbefehl";
        return sign_>0?"Steigen/Sprung: Simulatorantwort ausstehend":
            "Sinken: Simulatorantwort ausstehend";
    }
    [[nodiscard]] bool server_position(Vec3 position,TimePoint now)
    {
        if(!std::isfinite(position.x)||!std::isfinite(position.y)||
           !std::isfinite(position.z))return false;
        std::scoped_lock lock{mutex_};
        if((last_sample_at_&&now<*last_sample_at_)||
           (sent_&&now<sent_at_))return false;
        if(latest_)
        {
            const double dx=static_cast<double>(position.x)-latest_->x;
            const double dy=static_cast<double>(position.y)-latest_->y;
            const double dz=static_cast<double>(position.z)-latest_->z;
            const double dist_sq=dx*dx+dy*dy+dz*dz;
            // A teleport, region transfer, or invalid large self-pose
            // cannot count as a jump/fly acknowledgement.
            if(!std::isfinite(dist_sq)||dist_sq>=1024.0)
            {
                latest_=position;last_sample_at_=now;
                baseline_=position;
                // Old jump/fly request was sent on the pre-teleport
                // position lease. Disarm proof until another UDP datagram
                // is actually sent; a post-teleport Z correction alone
                // is not a verified response to an earlier command.
                sent_=false;sign_=0;last_fly_=false;
                awaiting_fresh_udp_after_jump_=true;
                confirmed_=false;stalled_=false;samples_=0U;
                contradictory_samples_=0U;
                return false;
            }
        }
        latest_=position;last_sample_at_=now;
        if(!sent_)return false;
        if(!baseline_)baseline_=position;
        if(samples_!=UINT32_MAX)++samples_;
        const double dz=static_cast<double>(position.z)-baseline_->z;
        const double projected=dz*sign_;
        if(std::isfinite(projected)&&projected>=0.25)
        {
            if(confirmed_)return false;
            confirmed_=true;stalled_=false;
            return true;
        }
        if(std::isfinite(projected)&&projected<=-0.25&&
           contradictory_samples_!=UINT32_MAX)
            ++contradictory_samples_;
        if(!confirmed_&&!stalled_&&samples_>=3U&&
           now-sent_at_>=std::chrono::seconds{4})
            stalled_=true;
        return false;
    }
private:
    static int direction(std::uint32_t flags) noexcept
    {
        const bool up=(flags&control_up_pos)!=0U;
        const bool down=(flags&control_up_neg)!=0U;
        return up==down?0:up?1:-1;
    }
    void clear_active() noexcept
    {
        sent_=false;sign_=0;confirmed_=false;stalled_=false;
        baseline_.reset();samples_=0U;contradictory_samples_=0U;
    }
    mutable std::mutex mutex_;
    std::optional<Vec3> latest_,baseline_;
    std::optional<TimePoint> last_sample_at_;
    TimePoint sent_at_{};
    bool queued_=false,queued_seen_=false,last_fly_=false;
    bool sent_=false,confirmed_=false,stalled_=false;
    bool awaiting_fresh_udp_after_jump_=false;
    int sign_=0;
    std::uint32_t samples_=0U,contradictory_samples_=0U;
};
}
