/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string_view>

namespace nexverse::core
{
enum class LogLevel
{
    debug,
    info,
    warning,
    error
};

void set_log_level(LogLevel level) noexcept;
void log(LogLevel level, std::string_view message);
void debug(std::string_view message);
void info(std::string_view message);
void warning(std::string_view message);
void error(std::string_view message);
}
