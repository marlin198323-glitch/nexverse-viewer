/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/assets/mesh_loader.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace nexverse::assets
{
// Validate CPU geometry before loading from disk or forwarding to OpenGL.
[[nodiscard]] bool cacheable_mesh(const MeshGeometry& geometry) noexcept;
[[nodiscard]] std::size_t mesh_byte_size(const MeshGeometry& geometry) noexcept;

class MeshDiskCache
{
public:
    explicit MeshDiskCache(std::filesystem::path directory,
        std::uint64_t budget_bytes = 512ULL * 1024ULL * 1024ULL);
    [[nodiscard]] std::optional<MeshGeometry> load(std::string_view id,
        MeshLod lod = MeshLod::high) const;
    [[nodiscard]] bool store(std::string_view id, const MeshGeometry& mesh,
        MeshLod lod = MeshLod::high) const;
    [[nodiscard]] std::size_t prune() const;
    [[nodiscard]] static std::filesystem::path default_directory();
private:
    std::filesystem::path directory_;
    std::uint64_t budget_bytes_;
};

class MeshMemoryCache
{
public:
    explicit MeshMemoryCache(std::size_t budget_bytes = 64U * 1024U * 1024U);
    [[nodiscard]] std::optional<MeshGeometry> load(std::string_view id,
        MeshLod lod = MeshLod::high);
    [[nodiscard]] bool store(std::string_view id, const MeshGeometry& mesh,
        MeshLod lod = MeshLod::high);
    [[nodiscard]] std::size_t bytes() const;
    [[nodiscard]] std::size_t count() const;
private:
    struct Entry
    {
        MeshGeometry mesh;
        std::size_t bytes;
        std::list<std::string>::iterator age;
    };
    std::size_t budget_;
    std::size_t used_ = 0U;
    mutable std::mutex mutex_;
    std::list<std::string> lru_;
    std::unordered_map<std::string, Entry> items_;
};

enum class MeshCacheTier { memory, disk, network };
struct CachedMeshResult
{
    MeshAssetResult asset;
    MeshCacheTier tier = MeshCacheTier::network;
};

// Uses RAM -> disk -> authenticated GetMesh callback. Network errors never
// enter the caches. Only immutable UUID assets are cached, not CAPS URLs.
[[nodiscard]] CachedMeshResult load_mesh_cached(
    std::string_view id, MeshMemoryCache& memory, const MeshDiskCache& disk,
    const std::function<MeshAssetResult()>& downloader,
    MeshLod preferred_lod = MeshLod::high,
    const std::function<bool()>& accept = {});
}
