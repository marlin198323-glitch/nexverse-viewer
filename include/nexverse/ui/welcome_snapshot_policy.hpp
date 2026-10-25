/* OpenGenesisLINK Viewer -- MPL-2.0
 * Pure state machine for asynchronous GTK/WebKit snapshot retries.
 */
#pragma once
#include <chrono>
#include <cstdint>

namespace nexverse::ui
{
class WelcomeSnapshotPolicy
{
public:
    using Clock=std::chrono::steady_clock;
    using Time=Clock::time_point;

    void invalidate() noexcept
    {
        ++revision_;
        dirty_=true;
        failures_=0U;
        retry_after_=Time{};
    }

    [[nodiscard]] bool can_request(Time now) const noexcept
    {
        return !pending_ && failures_<3U &&
               now>=retry_after_ && (dirty_ || now>=refresh_after_);
    }

    [[nodiscard]] std::uint64_t begin(Time now) noexcept
    {
        if(!can_request(now))return 0U;
        pending_=true;
        active_revision_=revision_;
        return active_revision_;
    }

    // Reject results of old viewports, including late failures.
    [[nodiscard]] bool finish(std::uint64_t token,bool success,Time now) noexcept
    {
        if(!pending_||token==0U||token!=active_revision_)return false;
        pending_=false;
        if(token!=revision_)return false;
        if(success)
        {
            dirty_=false;
            failures_=0U;
            refresh_after_=now+std::chrono::seconds{30};
            retry_after_=Time{};
            return true;
        }
        dirty_=true;
        ++failures_;
        retry_after_=now+std::chrono::milliseconds{250*failures_};
        return false;
    }

    [[nodiscard]] bool is_current(std::uint64_t token) const noexcept
    {
        return pending_ && token!=0U &&
               token==active_revision_ && token==revision_;
    }

    [[nodiscard]] bool exhausted() const noexcept
    {
        return failures_>=3U;
    }

private:
    std::uint64_t revision_=1U;
    std::uint64_t active_revision_=0U;
    bool pending_=false,dirty_=true;
    unsigned failures_=0U;
    Time retry_after_{};
    Time refresh_after_{Time::max()};
};
}
