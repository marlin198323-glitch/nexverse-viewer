/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstdint>
#include <string_view>
namespace nexverse::assets
{
enum class AssetCacheLookup : std::uint8_t
{
    not_checked, hit, miss, corrupt, io_error
};
[[nodiscard]] constexpr std::string_view asset_cache_lookup_name(
    AssetCacheLookup value) noexcept
{
    switch(value)
    {
    case AssetCacheLookup::not_checked:return "not_checked";
    case AssetCacheLookup::hit:return "hit";
    case AssetCacheLookup::miss:return "miss";
    case AssetCacheLookup::corrupt:return "corrupt";
    case AssetCacheLookup::io_error:return "io_error";
    }
    return "unknown";
}
}
