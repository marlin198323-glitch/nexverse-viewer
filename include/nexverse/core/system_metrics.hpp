/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstdint>

namespace nexverse::core
{
struct SystemMemoryMetrics
{
    std::uint64_t total_bytes = 0U;
    std::uint64_t available_bytes = 0U;
};

[[nodiscard]] SystemMemoryMetrics system_memory_metrics() noexcept;
}
