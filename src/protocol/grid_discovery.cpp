/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/protocol/grid_discovery.hpp"
#include "nexverse/core/logger.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <system_error>
#include <utility>

namespace
{
std::filesystem::path home_directory()
{
#if defined(_WIN32)
    if (const char* value = std::getenv("USERPROFILE"); value != nullptr && *value != '\0') return value;
#else
    if (const char* value = std::getenv("HOME"); value != nullptr && *value != '\0') return value;
#endif
    return std::filesystem::current_path();
}

std::optional<nexverse::protocol::GridInfoDocument> read_grid_info_file(
    const std::filesystem::path& path)
{
    std::error_code error;
    const auto size = std::filesystem::file_size(path, error);
    if (error || size > nexverse::protocol::max_grid_info_bytes) return std::nullopt;

    std::ifstream input{path, std::ios::binary};
    if (!input) return std::nullopt;

    std::string xml{
        std::istreambuf_iterator<char>{input},
        std::istreambuf_iterator<char>{}};

    auto parsed = nexverse::protocol::parse_grid_info_xml(xml);
    if (!parsed.ok()) return std::nullopt;

    return std::move(parsed.document);
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
        return std::filesystem::path{local_app_data} / "OpenGenesisLINK Viewer" / "Cache" / "grid_info.xml";
    }
    return home_directory() / "AppData" / "Local" / "OpenGenesisLINK Viewer" / "Cache" / "grid_info.xml";
#elif defined(__APPLE__)
    return home_directory() / "Library" / "Caches" / "OpenGenesisLINK Viewer" / "grid_info.xml";
#else
    if (const char* xdg_cache_home = std::getenv("XDG_CACHE_HOME");
        xdg_cache_home != nullptr && *xdg_cache_home != '\0')
    {
        return std::filesystem::path{xdg_cache_home} / "opengenesislink-viewer" / "grid_info.xml";
    }
    return home_directory() / ".cache" / "opengenesislink-viewer" / "grid_info.xml";
#endif
}

GridDiscoveryService::GridDiscoveryService(
    std::shared_ptr<net::IHttpTransport> transport,
    std::filesystem::path cache_path)
    : transport_{std::move(transport)},
      cache_path_{std::move(cache_path)}
{
}

GridDiscoveryResult GridDiscoveryService::refresh()
{
    const std::optional<GridInfoDocument> cached = load_cache();

    if (transport_ != nullptr)
    {
        constexpr auto connect_timeout = std::chrono::seconds{5};
        constexpr auto total_timeout = std::chrono::seconds{10};

        const net::HttpResponse response = transport_->get(
            nexverse_grid_info_endpoint,
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

                core::info(changed ? "NexVerse GridInfo changed" : "NexVerse GridInfo unchanged");
                return {
                    true,
                    std::move(live.info),
                    GridInfoSource::live,
                    changed,
                    cache_saved ? "Current GridInfo loaded from NexVerse"
                                : "Current GridInfo loaded; cache update failed"};
            }

            core::warning(parsed.error);
        }
        else
        {
            core::warning(response.error.empty() ? "GridInfo request failed" : response.error);
        }
    }

    if (cached.has_value())
    {
        return {
            true,
            cached->info,
            GridInfoSource::cache,
            false,
            "Using last valid cached NexVerse GridInfo"};
    }

    return {
        false,
        {},
        GridInfoSource::none,
        false,
        "NexVerse GridInfo unavailable and no valid cache exists"};
}

std::optional<GridInfoDocument> GridDiscoveryService::load_cache() const
{
    return read_grid_info_file(cache_path_);
}

bool GridDiscoveryService::save_cache(std::string_view xml) const
{
    std::error_code error;
    const auto parent = cache_path_.parent_path();
    if (!parent.empty())
    {
        std::filesystem::create_directories(parent, error);
        if (error) return false;
    }

    std::filesystem::path temporary = cache_path_;
    temporary += ".tmp";

    {
        std::ofstream output{temporary, std::ios::binary | std::ios::trunc};
        if (!output) return false;
        output.write(xml.data(), static_cast<std::streamsize>(xml.size()));
        output.flush();
        if (!output)
        {
            std::filesystem::remove(temporary, error);
            return false;
        }
    }

    error.clear();
    std::filesystem::rename(temporary, cache_path_, error);
    if (!error) return true;

    error.clear();
    if (!std::filesystem::exists(cache_path_, error) || error)
    {
        std::filesystem::remove(temporary, error);
        return false;
    }

    std::filesystem::path backup = cache_path_;
    backup += ".bak";
    std::filesystem::remove(backup, error);

    error.clear();
    std::filesystem::rename(cache_path_, backup, error);
    if (error)
    {
        std::filesystem::remove(temporary, error);
        return false;
    }

    error.clear();
    std::filesystem::rename(temporary, cache_path_, error);
    if (!error)
    {
        std::error_code cleanup;
        std::filesystem::remove(backup, cleanup);
        return true;
    }

    std::error_code restore;
    std::filesystem::rename(backup, cache_path_, restore);

    std::error_code cleanup;
    std::filesystem::remove(temporary, cleanup);

    if (restore)
    {
        core::error("GridInfo cache replacement and backup restoration failed");
    }
    else
    {
        core::warning("GridInfo cache replacement failed; previous cache restored");
    }
    return false;
}
}
