/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace nexverse::assets
{
// Measures CPU wall time spent submitting mesh / texture assets to OpenGL.
// This does NOT measure GPU completion time (which is asynchronous).
class AssetUploadTelemetry
{
public:
    void observe(std::chrono::microseconds duration) noexcept
    {
        const auto raw=duration.count();
        const auto micros=raw>0 ?
            static_cast<std::uint64_t>(raw) : 0U;
        samples_[next_] = micros;
        next_=(next_+1U)%samples_.size();
        if(count_<samples_.size()) ++count_;
        if(total_<=std::numeric_limits<std::uint64_t>::max()-micros)
            total_+=micros;
        else total_=std::numeric_limits<std::uint64_t>::max();
        ++lifetime_count_;
    }
    [[nodiscard]] std::size_t recent_count() const noexcept{return count_;}
    [[nodiscard]] std::size_t total_count() const noexcept{return lifetime_count_;}
    [[nodiscard]] std::uint64_t recent_avg_us() const noexcept
    {
        if(count_==0U)return 0U;
        std::uint64_t sum=0U;
        for(std::size_t i=0;i<count_;++i)
            sum+=samples_[i];
        return sum/count_;
    }
    [[nodiscard]] std::uint64_t recent_max_us() const noexcept
    {
        std::uint64_t maximum=0U;
        for(std::size_t i=0;i<count_;++i)
            maximum=std::max(maximum,samples_[i]);
        return maximum;
    }
    [[nodiscard]] std::uint64_t recent_p95_us() const noexcept
    {
        if(count_==0U)return 0U;
        auto sorted=samples_;
        std::sort(sorted.begin(),sorted.begin()+static_cast<std::ptrdiff_t>(count_));
        return sorted[((count_*95U+99U)/100U)-1U];
    }
    void reset() noexcept
    {
        samples_.fill(0U);
        next_=count_=0U;
        total_=0U;
        lifetime_count_=0U;
    }
private:
    std::array<std::uint64_t,64U> samples_{};
    std::size_t next_=0U;
    std::size_t count_=0U;
    std::uint64_t total_=0U;
    std::size_t lifetime_count_=0U;
};
}
