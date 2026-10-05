/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/grid_info.hpp"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
inline constexpr std::string_view nexverse_grid_host = "hg.stadt-nexverse.de";
inline constexpr std::string_view nexverse_grid_info_endpoint =
    "http://hg.stadt-nexverse.de/get_grid_info";

enum class GridInfoSource
{
    none,
    live,
    cache
};

struct GridDiscoveryResult
{
    bool success = false;
    GridInfo info;
    GridInfoSource source = GridInfoSource::none;
    bool changed = false;
    std::string message;
};

[[nodiscard]] std::filesystem::path default_grid_info_cache_path();

class GridDiscoveryService
{
public:
    explicit GridDiscoveryService(
        std::shared_ptr<net::IHttpTransport> transport,
        std::filesystem::path cache_path = default_grid_info_cache_path());

    [[nodiscard]] GridDiscoveryResult refresh();

private:
    [[nodiscard]] std::optional<GridInfoDocument> load_cache() const;
    [[nodiscard]] bool save_cache(std::string_view xml) const;

    std::shared_ptr<net::IHttpTransport> transport_;
    std::filesystem::path cache_path_;
};
}
