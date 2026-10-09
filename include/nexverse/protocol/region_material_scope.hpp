/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// Monotonically scoped response token; immutable for the lifetime of a
// region. Async HTTP responses may only be applied to their original region.
class RegionMaterialScope
{
public:
    [[nodiscard]] bool update(std::uint64_t handle,
                              std::uint32_t generation,
                              std::string_view seed_capability)
    {
        if (initialized_ && handle_ == handle &&
            generation_ == generation &&
            seed_capability_ == seed_capability)
            return false;

        initialized_ = true;
        handle_ = handle;
        generation_ = generation;
        seed_capability_.assign(seed_capability);
        ++epoch_;
        return true;
    }

    [[nodiscard]] std::uint64_t epoch() const noexcept { return epoch_; }
    [[nodiscard]] bool matches(std::uint64_t request_epoch) const noexcept
    {
        return initialized_ && request_epoch == epoch_;
    }
    [[nodiscard]] std::string_view seed_capability() const noexcept
    {
        return seed_capability_;
    }

private:
    std::uint64_t handle_ = 0U;
    std::uint32_t generation_ = 0U;
    std::string seed_capability_;
    std::uint64_t epoch_ = 0U;
    bool initialized_ = false;
};

}
