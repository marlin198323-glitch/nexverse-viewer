/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
inline constexpr std::size_t max_grid_info_bytes = 256U * 1024U;

struct GridInfo
{
    std::map<std::string, std::string, std::less<>> fields;

    [[nodiscard]] const std::string* get(std::string_view key) const noexcept;
    [[nodiscard]] std::string_view login_uri() const noexcept;
    [[nodiscard]] std::string_view grid_name() const noexcept;
    [[nodiscard]] std::string_view grid_nick() const noexcept;

    bool operator==(const GridInfo&) const = default;
};

struct GridInfoDocument
{
    GridInfo info;
    std::string raw_xml;
};

struct GridInfoParseResult
{
    std::optional<GridInfoDocument> document;
    std::string error;

    [[nodiscard]] bool ok() const noexcept
    {
        return document.has_value() && error.empty();
    }
};

[[nodiscard]] bool is_valid_http_uri(std::string_view uri) noexcept;
[[nodiscard]] GridInfoParseResult parse_grid_info_xml(std::string_view xml);
}
