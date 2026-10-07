/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/core/logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace
{
std::mutex g_log_mutex;
nexverse::core::LogLevel g_level = nexverse::core::LogLevel::info;

constexpr int rank(nexverse::core::LogLevel level) noexcept
{
    return static_cast<int>(level);
}

const char* label(nexverse::core::LogLevel level) noexcept
{
    using nexverse::core::LogLevel;
    switch (level)
    {
        case LogLevel::debug: return "DEBUG";
        case LogLevel::info: return "INFO";
        case LogLevel::warning: return "WARN";
        case LogLevel::error: return "ERROR";
    }
    return "LOG";
}
}

namespace nexverse::core
{
void set_log_level(LogLevel level) noexcept
{
    g_level = level;
}

void log(LogLevel level, std::string_view message)
{
    if (rank(level) < rank(g_level))
    {
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm local{};

#if defined(_WIN32)
    localtime_s(&local, &time);
#else
    localtime_r(&time, &local);
#endif

    std::scoped_lock lock(g_log_mutex);
    std::clog << std::put_time(&local, "%H:%M:%S")
              << " [" << label(level) << "] "
              << message << '\n';
}

void debug(std::string_view message) { log(LogLevel::debug, message); }
void info(std::string_view message) { log(LogLevel::info, message); }
void warning(std::string_view message) { log(LogLevel::warning, message); }
void error(std::string_view message) { log(LogLevel::error, message); }
}
