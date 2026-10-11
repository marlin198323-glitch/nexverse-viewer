/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include <cstddef>
namespace nexverse::assets
{
struct AssetMemoryCacheStats
{
    std::size_t hits=0U;
    std::size_t misses=0U;
    std::size_t evictions=0U;
    std::size_t bytes=0U;
    std::size_t entries=0U;
};
}
