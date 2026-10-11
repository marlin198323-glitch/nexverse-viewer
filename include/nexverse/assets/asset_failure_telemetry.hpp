/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/asset_failure.hpp"
#include <cstddef>
namespace nexverse::assets
{
// Numeric-only counters for failure categories. No UUID, CAPS URL, or HTTP
// body is stored here. Existing retry schedule owns failure backoff.
struct AssetFailureTelemetry
{
    std::size_t missing=0U;
    std::size_t transient=0U;
    std::size_t invalid=0U;
    std::size_t configuration=0U;
    void observe(AssetFailure kind) noexcept
    {
        switch(kind)
        {
        case AssetFailure::missing:++missing;break;
        case AssetFailure::transient:++transient;break;
        case AssetFailure::invalid:++invalid;break;
        case AssetFailure::configuration:++configuration;break;
        case AssetFailure::none:break;
        }
    }
    [[nodiscard]] std::size_t total() const noexcept
    {return missing+transient+invalid+configuration;}
};
}
