/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/core/logger.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>

namespace
{
std::mutex g_log_mutex;
nexverse::core::LogLevel g_level = nexverse::core::LogLevel::info;
std::optional<std::ofstream> g_log_file;

std::filesystem::path log_file_path()
{
#if defined(_WIN32)
    if (const char* base = std::getenv("LOCALAPPDATA"); base != nullptr && *base != '\0')
    {
        return std::filesystem::path{base} / "OpenGenesisLINK" / "viewer.log";
    }
#elif defined(__APPLE__)
    if (const char* home = std::getenv("HOME"); home != nullptr && *home != '\0')
    {
        return std::filesystem::path{home} / "Library" / "Logs" /
               "OpenGenesisLINK" / "viewer.log";
    }
#else
    if (const char* state = std::getenv("XDG_STATE_HOME"); state != nullptr && *state != '\0')
    {
        return std::filesystem::path{state} / "opengenesislink" / "viewer.log";
    }
    if (const char* home = std::getenv("HOME"); home != nullptr && *home != '\0')
    {
        return std::filesystem::path{home} / ".local" / "state" /
               "opengenesislink" / "viewer.log";
    }
#endif
    return std::filesystem::current_path() / "viewer.log";
}

std::ofstream* persistent_log_stream()
{
    if (g_log_file.has_value())
    {
        return g_log_file->is_open() ? &*g_log_file : nullptr;
    }

    try
    {
        const std::filesystem::path path = log_file_path();
        std::filesystem::create_directories(path.parent_path());
        g_log_file.emplace(path, std::ios::out | std::ios::app);
    }
    catch (...)
    {
        g_log_file.emplace();
    }

    return g_log_file->is_open() ? &*g_log_file : nullptr;
}

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

    std::ostringstream line;
    line << std::put_time(&local, "%Y-%m-%d %H:%M:%S")
         << " [" << label(level) << "] "
         << message;

    std::clog << line.str() << '\n';

    if (std::ofstream* file = persistent_log_stream(); file != nullptr)
    {
        *file << line.str() << '\n';
        file->flush();
    }
}

void debug(std::string_view message) { log(LogLevel::debug, message); }
void info(std::string_view message) { log(LogLevel::info, message); }
void warning(std::string_view message) { log(LogLevel::warning, message); }
void error(std::string_view message) { log(LogLevel::error, message); }
}
