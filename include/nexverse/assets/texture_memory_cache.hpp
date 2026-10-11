/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/texture_loader.hpp"
#include <cstddef>
#include <list>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
namespace nexverse::assets
{
// Retains full decoded RGBA images across GPU-mip swaps and region changes.
// The byte limit is independent of GPU residency and persistent disk cache.
class TextureMemoryCache
{
public:
    explicit TextureMemoryCache(
        std::size_t byte_budget = 96U*1024U*1024U);
    [[nodiscard]] std::optional<ImageRgba8> load(std::string_view id);
    [[nodiscard]] bool store(std::string_view id,const ImageRgba8& image);
    [[nodiscard]] std::size_t bytes() const;
    [[nodiscard]] std::size_t count() const;
private:
    struct Entry {
        ImageRgba8 image;
        std::size_t bytes;
        std::list<std::string>::iterator age;
    };
    std::size_t budget_;
    std::size_t used_ = 0U;
    mutable std::mutex mutex_;
    std::list<std::string> age_;
    std::unordered_map<std::string,Entry> entries_;
};
}
