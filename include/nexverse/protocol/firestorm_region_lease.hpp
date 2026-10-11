/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
// A source-bound, thread-safe lease for optional Firestorm region CAPS.
// A CAP URL is *not* exposed to logs, UI or snapshots.
class FirestormRegionLease
{
public:
    // 0 means no active region. Same identity is idempotent; new
    // region/handle/generation/seed revokes every older in-flight GET.
    [[nodiscard]] std::uint64_t enter(
        std::uint64_t handle,std::uint32_t generation,
        std::string_view seed)
    {
        std::scoped_lock lock{mutex_};
        if(handle==handle_&&generation==generation_&&seed==seed_)
            return epoch_;
        handle_=handle;
        generation_=generation;
        seed_=seed;
        ++epoch_;
        return epoch_;
    }
    [[nodiscard]] std::uint64_t epoch() const
    {
        std::scoped_lock lock{mutex_};
        return epoch_;
    }
    [[nodiscard]] bool matches(std::uint64_t issued) const
    {
        std::scoped_lock lock{mutex_};
        return issued!=0U && issued==epoch_;
    }
private:
    mutable std::mutex mutex_;
    std::uint64_t handle_=0U;
    std::uint32_t generation_=0U;
    std::string seed_;
    std::uint64_t epoch_=0U;
};
struct ExperienceUiState
{
    enum class Tab { owned, allowed, blocked };
    bool open=false;
    Tab tab=Tab::owned;
    void toggle(bool logged_in,bool region_ready) noexcept
    {
        if(!logged_in || !region_ready){open=false;return;}
        open=!open;
    }
    void close() noexcept {open=false;}
    void select(Tab next) noexcept {if(open)tab=next;}
};
}
