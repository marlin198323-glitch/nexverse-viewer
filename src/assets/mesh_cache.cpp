/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/assets/mesh_cache.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <iterator>
#include <new>
#include <vector>

namespace nexverse::assets
{
namespace
{
constexpr std::array<std::uint8_t, 8> magic{'O','G','L','M','E','S','H','1'};
constexpr std::size_t header_bytes = 28U;
constexpr std::size_t max_vertices = 200000U;
constexpr std::size_t max_indices = 1200000U;
constexpr std::size_t max_faces = 64U;
constexpr std::size_t max_payload = 200000U * 8U * 4U +
    1200000U * 4U + 64U * 12U;
bool safe_uuid(std::string_view id)
{
    if (id.size() != 36U) return false;
    for (std::size_t i = 0; i < id.size(); ++i)
    {
        if (i == 8U || i == 13U || i == 18U || i == 23U)
        {
            if (id[i] != '-') return false;
        }
        else if (!((id[i] >= '0' && id[i] <= '9') ||
                   (id[i] >= 'a' && id[i] <= 'f') ||
                   (id[i] >= 'A' && id[i] <= 'F'))) return false;
    }
    return true;
}
std::string canonical(std::string_view id)
{
    std::string s{id};
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
    { return static_cast<char>(c >= 'A' && c <= 'F' ? c - 'A' + 'a' : c); });
    return s;
}
std::string cache_key(std::string_view id, MeshLod lod)
{
    auto key = canonical(id);
    if (lod != MeshLod::high)
    {
        key += ".";
        key += mesh_lod_name(lod);
    }
    return key;
}
void push32(std::vector<std::uint8_t>& data, std::uint32_t value)
{
    for (unsigned i = 0U; i < 4U; ++i)
        data.push_back(static_cast<std::uint8_t>((value >> (8U*i)) & 0xffU));
}
void push64(std::vector<std::uint8_t>& data, std::uint64_t value)
{
    for (unsigned i = 0U; i < 8U; ++i)
        data.push_back(static_cast<std::uint8_t>((value >> (8U*i)) & 0xffU));
}
std::uint32_t get32(const std::vector<std::uint8_t>& bytes, std::size_t offset)
{
    std::uint32_t value = 0U;
    for (unsigned i = 0U; i < 4U; ++i)
        value |= static_cast<std::uint32_t>(bytes[offset+i]) << (8U*i);
    return value;
}
std::uint64_t get64(const std::vector<std::uint8_t>& bytes, std::size_t offset)
{
    std::uint64_t value = 0U;
    for (unsigned i = 0U; i < 8U; ++i)
        value |= static_cast<std::uint64_t>(bytes[offset+i]) << (8U*i);
    return value;
}
std::uint64_t checksum(std::string_view id, MeshLod lod,
    const std::vector<std::uint8_t>& bytes)
{
    std::uint64_t hash = 14695981039346656037ULL;
    const auto fold = [&](std::uint8_t value)
    { hash = (hash ^ value) * 1099511628211ULL; };
    for (const char c : cache_key(id, lod))
        fold(static_cast<std::uint8_t>(c));
    // Include header counts, but exclude the embedded checksum at [20,28).
    for (std::size_t i = 8U; i < 20U; ++i) fold(bytes[i]);
    for (std::size_t i = header_bytes; i < bytes.size(); ++i)
        fold(bytes[i]);
    return hash;
}
}
std::size_t mesh_byte_size(const MeshGeometry& m) noexcept
{
    return m.vertices.size() * sizeof(float) +
        m.indices.size() * sizeof(std::uint32_t) +
        m.faces.size() * 3U * sizeof(std::uint32_t);
}
bool cacheable_mesh(const MeshGeometry& m) noexcept
{
    if (!m.valid() || m.vertices.size()/8U > max_vertices ||
        m.indices.size() > max_indices || m.faces.size() > max_faces ||
        m.faces.empty()) return false;
    for (const auto v : m.vertices)
        if (!std::isfinite(v)) return false;
    for (const auto index : m.indices)
        if (index >= m.vertices.size()/8U) return false;
    for (const auto& face : m.faces)
        if (face.face >= max_faces || face.index_count == 0U ||
            face.index_count % 3U != 0U ||
            face.first_index > m.indices.size() ||
            face.index_count > m.indices.size()-face.first_index)
            return false;
    return true;
}
MeshDiskCache::MeshDiskCache(std::filesystem::path directory,
    std::uint64_t budget_bytes)
    : directory_(std::move(directory)), budget_bytes_(budget_bytes)
{}
std::filesystem::path MeshDiskCache::default_directory()
{
    if (const char* xdg = std::getenv("XDG_CACHE_HOME");
        xdg && *xdg)
    {
        const std::filesystem::path p{xdg};
        if (p.is_absolute()) return p/"opengenesislink"/"meshes-v1";
    }
    if (const char* home = std::getenv("HOME"); home && *home)
        return std::filesystem::path{home}/".cache"/"opengenesislink"/"meshes-v1";
    return {};
}
std::optional<MeshGeometry> MeshDiskCache::load(std::string_view id, MeshLod lod) const
{
    if (!safe_uuid(id) || directory_.empty()) return std::nullopt;
    try
    {
        const auto path = directory_/(cache_key(id,lod)+".mesh");
        // Do not follow symlinked entries in a cache directory.
        if (!std::filesystem::is_regular_file(
                std::filesystem::symlink_status(path)))
            return std::nullopt;
        const auto size = std::filesystem::file_size(path);
        if (size < header_bytes || size > header_bytes+max_payload)
            return std::nullopt;
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
        std::ifstream f{path, std::ios::binary};
        if (!f.read(reinterpret_cast<char*>(bytes.data()),
            static_cast<std::streamsize>(bytes.size()))) return std::nullopt;
        if (!std::equal(magic.begin(), magic.end(), bytes.begin()))
            return std::nullopt;
        const auto floats = get32(bytes, 8U);
        const auto indices = get32(bytes, 12U);
        const auto faces = get32(bytes, 16U);
        if (floats == 0U || floats % 8U != 0U ||
            floats/8U > max_vertices || indices == 0U ||
            indices % 3U != 0U || indices > max_indices ||
            faces == 0U || faces > max_faces)
            return std::nullopt;
        const std::uint64_t expected = header_bytes +
            static_cast<std::uint64_t>(floats)*4U +
            static_cast<std::uint64_t>(indices)*4U +
            static_cast<std::uint64_t>(faces)*12U;
        if (expected != size || checksum(id,lod,bytes) != get64(bytes,20U))
            return std::nullopt;
        MeshGeometry geometry;
        geometry.vertices.reserve(floats);
        geometry.indices.reserve(indices);
        geometry.faces.reserve(faces);
        std::size_t offset = header_bytes;
        for (std::uint32_t i = 0U; i < floats; ++i, offset += 4U)
            geometry.vertices.push_back(std::bit_cast<float>(get32(bytes,offset)));
        for (std::uint32_t i = 0U; i < indices; ++i, offset += 4U)
            geometry.indices.push_back(get32(bytes,offset));
        for (std::uint32_t i = 0U; i < faces; ++i, offset += 12U)
            geometry.faces.push_back({
                get32(bytes,offset), get32(bytes,offset+4U),
                get32(bytes,offset+8U)});
        if (!cacheable_mesh(geometry)) return std::nullopt;
        std::error_code ec;
        std::filesystem::last_write_time(path,
            std::filesystem::file_time_type::clock::now(), ec);
        return geometry;
    }
    catch (const std::filesystem::filesystem_error&) { return std::nullopt; }
    catch (const std::ios_base::failure&) { return std::nullopt; }
    catch (const std::bad_alloc&) { return std::nullopt; }
}
bool MeshDiskCache::store(std::string_view id, const MeshGeometry& geometry, MeshLod lod) const
{
    if (!safe_uuid(id) || directory_.empty() || !cacheable_mesh(geometry) ||
        mesh_byte_size(geometry)+header_bytes > budget_bytes_) return false;
    try
    {
        std::vector<std::uint8_t> bytes;
        bytes.reserve(header_bytes+mesh_byte_size(geometry));
        bytes.insert(bytes.end(), magic.begin(), magic.end());
        push32(bytes,static_cast<std::uint32_t>(geometry.vertices.size()));
        push32(bytes,static_cast<std::uint32_t>(geometry.indices.size()));
        push32(bytes,static_cast<std::uint32_t>(geometry.faces.size()));
        push64(bytes,0U);
        for (float value : geometry.vertices)
            push32(bytes,std::bit_cast<std::uint32_t>(value));
        for (auto index : geometry.indices) push32(bytes,index);
        for (const auto& face : geometry.faces)
        {
            push32(bytes,face.face);
            push32(bytes,face.first_index);
            push32(bytes,face.index_count);
        }
        const auto hash = checksum(id, lod, bytes);
        for (unsigned i = 0U; i < 8U; ++i)
            bytes[20U+i] = static_cast<std::uint8_t>(hash >> (8U*i));
        std::filesystem::create_directories(directory_);
        const auto dest = directory_/(cache_key(id,lod)+".mesh");
        auto temp = dest;
        temp += ".part";
        {
            std::ofstream f{temp,std::ios::binary|std::ios::trunc};
            if (!f) return false;
            f.write(reinterpret_cast<const char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()));
            f.flush();
            if (!f) return false;
        }
        std::filesystem::rename(temp,dest);
        (void)prune();
        return true;
    }
    catch (const std::filesystem::filesystem_error&) { return false; }
    catch (const std::ios_base::failure&) { return false; }
    catch (const std::bad_alloc&) { return false; }
}
std::size_t MeshDiskCache::prune() const
{
    if (directory_.empty()) return 0U;
    struct Entry {std::filesystem::path path;
        std::filesystem::file_time_type time; std::uint64_t size;};
    try
    {
        if (!std::filesystem::exists(directory_)) return 0U;
        std::vector<Entry> entries;
        std::uint64_t total = 0U;
        for (const auto& file : std::filesystem::directory_iterator(directory_))
        {
            if (file.path().extension() != ".mesh" ||
                !std::filesystem::is_regular_file(file.symlink_status()))
                continue;
            const auto name = file.path().stem().string();
            const auto dot = name.find('.');
            if (!safe_uuid(name.substr(0U,dot))) continue;
            if (dot != std::string::npos &&
                name.substr(dot+1U)!="low" &&
                name.substr(dot+1U)!="medium" &&
                name.substr(dot+1U)!="lowest") continue;
            const auto size = file.file_size();
            entries.push_back({file.path(),file.last_write_time(),size});
            total += size;
        }
        if (total <= budget_bytes_) return 0U;
        std::sort(entries.begin(),entries.end(),[](const Entry& a,const Entry& b)
        {return a.time != b.time ? a.time < b.time : a.path < b.path;});
        std::size_t evicted = 0U;
        for (const auto& entry : entries)
        {
            if (total <= budget_bytes_) break;
            std::filesystem::remove(entry.path);
            total -= entry.size;
            ++evicted;
        }
        return evicted;
    }
    catch (const std::filesystem::filesystem_error&) { return 0U; }
}
MeshMemoryCache::MeshMemoryCache(std::size_t budget_bytes) : budget_(budget_bytes)
{}
std::optional<MeshGeometry> MeshMemoryCache::load(std::string_view id, MeshLod lod)
{
    if (!safe_uuid(id)) return std::nullopt;
    const auto key = cache_key(id,lod);
    std::lock_guard lock{mutex_};
    const auto it = items_.find(key);
    if (it == items_.end()) return std::nullopt;
    lru_.splice(lru_.end(),lru_,it->second.age);
    return it->second.mesh;
}
bool MeshMemoryCache::store(std::string_view id, const MeshGeometry& geometry, MeshLod lod)
{
    if (!safe_uuid(id) || !cacheable_mesh(geometry)) return false;
    const auto size = mesh_byte_size(geometry);
    if (size > budget_) return false;
    const auto key = cache_key(id,lod);
    std::lock_guard lock{mutex_};
    if (const auto it = items_.find(key); it != items_.end())
    {
        used_ -= it->second.bytes;
        lru_.erase(it->second.age);
        items_.erase(it);
    }
    while (used_+size > budget_ && !lru_.empty())
    {
        const auto oldest = lru_.front();
        used_ -= items_.at(oldest).bytes;
        items_.erase(oldest);
        lru_.pop_front();
    }
    lru_.push_back(key);
    const auto age = std::prev(lru_.end());
    items_.emplace(key,Entry{geometry,size,age});
    used_ += size;
    return true;
}
std::size_t MeshMemoryCache::bytes() const
{
    std::lock_guard lock{mutex_};
    return used_;
}
std::size_t MeshMemoryCache::count() const
{
    std::lock_guard lock{mutex_};
    return items_.size();
}
CachedMeshResult load_mesh_cached(std::string_view id, MeshMemoryCache& memory,
    const MeshDiskCache& disk,
    const std::function<MeshAssetResult()>& downloader,
    MeshLod preferred_lod)
{
    CachedMeshResult result;
    result.asset.asset_id = std::string{id};
    if (!safe_uuid(id))
    {
        result.asset.message = "Invalid mesh asset identifier";
        return result;
    }
    if (auto hit = memory.load(id,preferred_lod))
    {
        result.tier = MeshCacheTier::memory;
        result.asset.success = true;
        result.asset.geometry = std::move(*hit);
        result.asset.message = "RAM mesh cache hit";
        result.asset.decoded_lod = preferred_lod;
        return result;
    }
    if (auto hit = disk.load(id,preferred_lod))
    {
        result.tier = MeshCacheTier::disk;
        result.asset.success = true;
        result.asset.geometry = std::move(*hit);
        result.asset.message = "Disk mesh cache hit";
        result.asset.decoded_lod = preferred_lod;
        (void)memory.store(id,result.asset.geometry,preferred_lod);
        return result;
    }
    result.asset = downloader();
    if (result.asset.success && cacheable_mesh(result.asset.geometry) &&
        canonical(result.asset.asset_id) == canonical(id))
    {
        (void)memory.store(id,result.asset.geometry,preferred_lod);
        (void)disk.store(id,result.asset.geometry,preferred_lod);
    }
    return result;
}
}
