/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_disk_cache.hpp"
#include "nexverse/assets/texture_memory_cache.hpp"
#include <atomic>
#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>

namespace nexverse::assets
{
// Disk-only prefetch. Never invokes HTTP or handles capability credentials.
// A completed old-region disk read cannot enter the live RAM cache.
[[nodiscard]] inline bool prefetch_texture_disk_to_memory_scoped(
    std::string_view id,TextureMemoryCache& ram,
    const std::atomic<std::uint64_t>& active_epoch,
    std::uint64_t expected_epoch,
    const std::function<std::optional<ImageRgba8>()>& load_disk)
{
    if(active_epoch.load(std::memory_order_acquire)!=expected_epoch)
        return false;
    auto image=load_disk();
    if(!image || !image->valid() ||
       active_epoch.load(std::memory_order_acquire)!=expected_epoch)
        return false;
    return ram.store(id,*image);
}

[[nodiscard]] inline bool prefetch_texture_disk_to_memory(
    std::string_view id, const TextureDiskCache& disk,
    TextureMemoryCache& ram,
    const std::atomic<std::uint64_t>& active_epoch,
    std::uint64_t expected_epoch)
{
    return prefetch_texture_disk_to_memory_scoped(
        id,ram,active_epoch,expected_epoch,
        [&]{return disk.load(id);});
}
}
