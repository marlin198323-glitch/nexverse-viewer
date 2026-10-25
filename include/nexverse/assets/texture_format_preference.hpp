/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
#include <mutex>
namespace nexverse::assets
{
// Region-scoped GetTexture converter negotiation. Do not store URLs or IDs.
// Only 3 confirmed failures of format=png WITH successfully decoded native
// J2C may avoid a PNG GET; periodically probe converter recovery.
// A native failure immediately returns to two-format negotiation next asset.
class TextureFormatPreference
{
public:
    struct Statistics
    {
        unsigned png_gets_avoided=0U;
        unsigned png_converter_reprobes=0U;
        bool native_preferred=false;
    };
    [[nodiscard]] Statistics statistics() const
    {
        std::scoped_lock lock{mutex_};
        return {png_avoided_,png_reprobes_,native_streak_>=3U};
    }
    void enter(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        if(epoch_==epoch)return;
        epoch_=epoch;
        native_streak_=0U;
        skips_=0U;
        png_avoided_=0U;
        png_reprobes_=0U;
    }
    [[nodiscard]] bool skip_png(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        if(epoch==0U||epoch_!=epoch||native_streak_<3U)return false;
        // Every 16th eligible transfer probes format=png once more.
        ++skips_;
        if(skips_%16U==0U)
        {
            ++png_reprobes_;
            return false;
        }
        ++png_avoided_;
        return true;
    }
    void png_fallback_requested(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        if(epoch_==epoch&&epoch!=0U&&png_avoided_>0U)
            --png_avoided_;
    }
    void native_succeeded(std::uint64_t epoch,bool converter_failed)
    {
        std::scoped_lock lock{mutex_};
        if(epoch_!=epoch||epoch==0U)return;
        if(converter_failed)
        {
            if(native_streak_<3U)++native_streak_;
        }
    }
    void native_failed(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        if(epoch_!=epoch||epoch==0U)return;
        native_streak_=0U;
        skips_=0U;
    }
    void png_succeeded(std::uint64_t epoch)
    {
        native_failed(epoch);
    }
private:
    mutable std::mutex mutex_;
    std::uint64_t epoch_=0U;
    unsigned native_streak_=0U;
    unsigned skips_=0U;
    unsigned png_avoided_=0U;
    unsigned png_reprobes_=0U;
};
} // namespace nexverse::assets
