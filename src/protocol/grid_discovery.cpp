/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/protocol/grid_discovery.hpp"

#include "nexverse/core/logger.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <system_error>

namespace
{
std::filesystem::path home_directory()
{
#if defined(_WIN32)
    if (const char* value = std::getenv("USERPROFILE"); value != nullptr && *value != '\0')
    {
        return value;
    }
#else
    if (const char* value = std::getenv("HOME"); value != nullptr && *value != '\0')
    {
        return value;
    }
#endif
    return std::filesystem::current_path();
}
}

namespace nexverse::protocol
{
std::filesystem::path default_grid_info_cache_path()
{
#if defined(_WIN32)
    if (const char* local_app_data = std::getenv("LOCALAPPDATA");
        local_app_data != nullptr && *local_app_data != '\0')
    {
        return std::filesystem::path{local_app_data} /
               "OpenGenesisLINK Viewer" /
               "Cache" /
               "grid_info.xml";
    }

    return home_directory() /
           "AppData" /
           "Local" /
           "OpenGenesisLINK Viewer" /
           "Cache" /
           "grid_info.xml";
#elif defined(__APPLE__)
    return home_directory() /
           "Library" /
           "Caches" /
           "OpenGenesisLINK Viewer" /
           "grid_info.xml";
#else
    if (const char* xdg_cache_home = std::getenv("XDG_CACHE_HOME");
        xdg_cache_home != nullptr && *xdg_cache_home != '\0')
    {
        return std::filesystem::path{xdg_cache_home} /
               "opengenesislink-viewer" /
               "grid_info.xml";
    }

    return home_directory() /
           ".cache" /
           "opengenesislink-viewer" /
           "grid_info.xml";
#endif
}

GridDiscoveryService::GridDiscoveryService(
    std::shared_ptr<net::IHttpTransport> transport,
    std::filesystem::path cache_path,
    std::string endpoint)
    : transport_{std::move(transport)},
      cache_path_{std::move(cache_path)},
      endpoint_{std::move(endpoint)}
{
}

GridDiscoveryResult GridDiscoveryService::refresh()
{
    const std::optional<GridInfoDocument> cached = load_cache();

    if (transport_ == nullptr)
    {
        if (cached.has_value())
        {
            return GridDiscoveryResult{
                true,
                cached->info,
                GridInfoSource::cache,
                false,
                "HTTP transport unavailable; using last valid GridInfo cache"};
        }

        return GridDiscoveryResult{
            false,
            {},
            GridInfoSource::none,
            false,
            "HTTP transport unavailable and no valid GridInfo cache exists"};
    }

    constexpr auto connect_timeout = std::chrono::seconds{5};
    constexpr auto total_timeout = std::chrono::seconds{10};

    const net::HttpResponse response = transport_->get(
        endpoint_,
        max_grid_info_bytes,
        connect_timeout,
        total_timeout);

    if (response.ok())
    {
        GridInfoParseResult parsed = parse_grid_info_xml(response.body);
        if (parsed.ok())
        {
            GridInfoDocument live = std::move(*parsed.document);
            const bool changed = !cached.has_value() || !(cached->info == live.info);

            const bool cache_saved = save_cache(live.raw_xml);
            if (changed)
            {
                core::info("NexVerse GridInfo changed; using current server configuration");
            }
            else
            {
                core::info("NexVerse GridInfo unchanged");
            }

            GridDiscoveryResult result{
                true,
                std::move(live.info),
                GridInfoSource::live,
                changed,
                cache_saved
                    ? "Current GridInfo loaded from NexVerse"
                    : "Current GridInfo loaded; local cache could not be updated"};

            return result;
        }

        core::warning(parsed.error);

        if (cached.has_value())
        {
            return GridDiscoveryResult{
                true,
                cached->info,
                GridInfoSource::cache,
                false,
                "Live GridInfo invalid; using last valid cached configuration"};
        }

        return GridDiscoveryResult{
            false,
            {},
            GridInfoSource::none,
            false,
            parsed.error};
    }

    core::warning(response.error);

    if (cached.has_value())
    {
        return GridDiscoveryResult{
            true,
            cached->info,
            GridInfoSource::cache,
            false,
            "NexVerse GridInfo unavailable; using last valid cached configuration"};
    }

    return GridDiscoveryResult{
        false,
        {},
        GridInfoSource::none,
        false,
        response.error.empty()
            ? "NexVerse GridInfo request failed"
            : response.error};
}

const std::filesystem::path& GridDiscoveryService::cache_path() const noexcept
{
    return cache_path_;
}

std::string_view GridDiscoveryService::endpoint() const noexcept
{
    return endpoint_;
}

std::optional<GridInfoDocument> GridDiscoveryService::load_cache() const
{
    std::error_code size_error;
    const auto size = std::filesystem::file_size(cache_path_, size_error);
    if (size_error || size > max_grid_info_bytes)
    {
        return std::nullopt;
    }

    std::ifstream input{cache_path_, std::ios::binary};
    if (!input)
    {
        return std::nullopt;
    }

    std::string xml{
        std::istreambuf_iterator<char>{input},
        std::istreambuf_iterator<char>{}};

    GridInfoParseResult parsed = parse_grid_info_xml(xml);
    if (!parsed.ok())
    {
        core::warning("Ignoring invalid cached GridInfo");
        return std::nullopt;
    }

    return std::move(parsed.document);
}

bool GridDiscoveryService::save_cache(std::string_view xml) const
{
    std::error_code error;
    const std::filesystem::path parent = cache_path_.parent_path();

    if (!parent.empty())
    {
        std::filesystem::create_directories(parent, error);
        if (error)
        {
            core::warning("Could not create GridInfo cache directory");
            return false;
        }
    }

    std::filesystem::path temporary = cache_path_;
    temporary += ".tmp";

    {
        std::ofstream output{temporary, std::ios::binary | std::ios::trunc};
        if (!output)
        {
            core::warning("Could not open temporary GridInfo cache file");
            return false;
        }

        output.write(xml.data(), static_cast<std::streamsize>(xml.size()));
        output.flush();

        if (!output)
        {
            core::warning("Could not write temporary GridInfo cache file");
            std::filesystem::remove(temporary, error);
            return false;
        }
    }

    std::filesystem::rename(temporary, cache_path_, error);
    if (!error)
    {
        return true;
    }

    error.clear();
    std::filesystem::remove(cache_path_, error);
    error.clear();
    std::filesystem::rename(temporary, cache_path_, error);

    if (error)
    {
        core::warning("Could not replace GridInfo cache file");
        std::filesystem::remove(temporary, error);
        return false;
    }

    return true;
}
