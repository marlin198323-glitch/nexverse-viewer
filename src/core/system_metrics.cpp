/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */

#include "nexverse/core/system_metrics.hpp"

#if defined(__linux__)
#include <sys/sysinfo.h>
#elif defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#endif

namespace nexverse::core
{
SystemMemoryMetrics system_memory_metrics() noexcept
{
    SystemMemoryMetrics metrics;

#if defined(__linux__)
    struct sysinfo info {};
    if (sysinfo(&info) == 0)
    {
        const std::uint64_t unit =
            static_cast<std::uint64_t>(info.mem_unit);
        metrics.total_bytes =
            static_cast<std::uint64_t>(info.totalram) * unit;
        metrics.available_bytes =
            static_cast<std::uint64_t>(info.freeram + info.bufferram) * unit;
    }
#elif defined(_WIN32)
    MEMORYSTATUSEX status {};
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status) != FALSE)
    {
        metrics.total_bytes =
            static_cast<std::uint64_t>(status.ullTotalPhys);
        metrics.available_bytes =
            static_cast<std::uint64_t>(status.ullAvailPhys);
    }
#elif defined(__APPLE__)
    std::uint64_t total = 0U;
    std::size_t size = sizeof(total);
    if (sysctlbyname("hw.memsize", &total, &size, nullptr, 0) == 0)
    {
        metrics.total_bytes = total;
    }
#endif

    return metrics;
}
}
