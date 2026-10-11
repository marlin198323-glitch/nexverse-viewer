#include "nexverse/assets/mesh_cache.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

using namespace nexverse::assets;
namespace fs = std::filesystem;
namespace
{
constexpr std::string_view a = "11111111-1111-1111-1111-111111111111";
constexpr std::string_view b = "22222222-2222-2222-2222-222222222222";
constexpr std::string_view c = "33333333-3333-3333-3333-333333333333";
MeshGeometry triangle(float x)
{
    return {{x,0,0, 0,0,1, 0,0,
             x+1,0,0, 0,0,1, 1,0,
             x,1,0, 0,0,1, 0,1},
            {0,1,2},{{0,0,3}}};
}
}
int main()
{
    const auto root = fs::temp_directory_path()/
        ("ogl-mesh-cache-test-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    int error = 0;
    const auto tri = triangle(0.0f);
    const auto tri2 = triangle(2.0f);
    const auto bytes = mesh_byte_size(tri);
    if (!cacheable_mesh(tri) || bytes != 120U) error = 1;
    auto invalid = tri;
    invalid.indices[1] = 99U;
    if (!error && cacheable_mesh(invalid)) error = 2;
    invalid = tri;
    invalid.vertices[0] = std::numeric_limits<float>::infinity();
    if (!error && cacheable_mesh(invalid)) error = 3;
    if (!error)
    {
        MeshDiskCache disk{root, 2048U};
        if (!disk.store(a,tri) || !disk.store(b,tri2)) error = 4;
        else if (const auto got = disk.load(a);
                 !got || got->vertices != tri.vertices ||
                 got->indices != tri.indices ||
                 got->faces.size() != 1U) error = 5;
        else if (disk.load("../escape") || disk.store("../escape",tri))
            error = 6;
        else if (disk.store(c,invalid)) error = 7;
        else
        {
            fs::copy_file(root/(std::string{a}+".mesh"),
                root/(std::string{c}+".mesh"));
            if (disk.load(c)) error = 8; // UUID-bound checksum
        }
    }
    if (!error)
    {
        // Tampering after download must never reach the renderer.
        std::fstream f{root/(std::string{a}+".mesh"),
            std::ios::in|std::ios::out|std::ios::binary};
        f.seekp(35);
        f.put(static_cast<char>(0x83));
        f.close();
        MeshDiskCache reopened{root};
        if (reopened.load(a) || !reopened.load(b)) error = 9;
    }
    if (!error)
    {
        // Disk prune evicts oldest LRU item and keeps a recently read one.
        MeshDiskCache disk{root, 2U*(bytes+28U)};
        (void)fs::remove(root/(std::string{c}+".mesh"));
        (void)fs::remove(root/(std::string{a}+".mesh"));
        if (!disk.store(a,tri)) error = 10;
        else
        {
            const auto old = fs::file_time_type::clock::now()-
                std::chrono::hours{4};
            fs::last_write_time(root/(std::string{a}+".mesh"),old);
            if (!disk.load(b) || !disk.store(c,tri) ||
                disk.load(a) || !disk.load(b) || !disk.load(c))
                error = 11;
        }
    }
    if (!error)
    {
        MeshMemoryCache ram{bytes*2U};
        if (!ram.store(a,tri) || !ram.store(b,tri2) || !ram.load(a) ||
            !ram.store(c,tri) || ram.load(b) || !ram.load(a) ||
            !ram.load(c) || ram.count()!=2U || ram.bytes()!=bytes*2U)
            error = 12;
        MeshMemoryCache tiny{bytes-1U};
        if (!error && (tiny.store(a,tri) || tiny.count()!=0U))
            error = 13;
        const auto stats=ram.stats();
        if (!error && (stats.hits==0U||stats.misses==0U||
                       stats.evictions==0U||stats.bytes!=ram.bytes()||
                       stats.entries!=ram.count()))
            error=19;
    }
    if (!error)
    {
        // Different LODs of one UUID have isolated RAM and disk entries.
        MeshDiskCache disk{root};
        MeshMemoryCache ram{bytes*3U};
        if (!disk.store(a,tri,MeshLod::high) ||
            !disk.store(a,tri2,MeshLod::medium) ||
            disk.load(a,MeshLod::low) ||
            !disk.load(a,MeshLod::medium) ||
            !ram.store(a,tri,MeshLod::high) ||
            !ram.store(a,tri2,MeshLod::medium) ||
            !ram.load(a,MeshLod::medium) ||
            ram.load(a,MeshLod::low) ||
            ram.load(a,MeshLod::high)->vertices != tri.vertices)
            error = 17;
    }
    if (!error)
    {
        // The multi-LOD isolation fixture stored high; ensure this test
        // starts with the deliberate network cache miss it checks.
        (void)fs::remove(root/(std::string{a}+".mesh"));
        MeshMemoryCache ram{bytes*2U};
        MeshDiskCache disk{root};
        int network_calls = 0;
        const auto fetch = [&] {
            ++network_calls;
            return MeshAssetResult{true,std::string{a},tri,"downloaded"};
        };
        // A was deliberately tampered/evicted; first call is network.
        const auto first = load_mesh_cached(a,ram,disk,fetch);
        const auto second = load_mesh_cached(a,ram,disk,fetch);
        MeshMemoryCache restarted{bytes*2U};
        const auto third = load_mesh_cached(a,restarted,disk,fetch);
        if (first.tier != MeshCacheTier::network ||
            second.tier != MeshCacheTier::memory ||
            third.tier != MeshCacheTier::disk || network_calls != 1 ||
            !third.asset.success) error = 14;
        auto reject = load_mesh_cached("../bad",ram,disk,fetch);
        if (!error && (reject.asset.success || network_calls != 1))
            error = 15;
        const auto failed = load_mesh_cached("44444444-4444-4444-4444-444444444444",
            ram,disk,[] { return MeshAssetResult{
                false, "44444444-4444-4444-4444-444444444444", {}, "404"}; });
        if (!error && (failed.asset.success ||
            disk.load("44444444-4444-4444-4444-444444444444")))
            error = 16;
    }
    if (!error)
    {
        // Cancel before HTTP; a stale region may not populate the cache.
        const std::string id = "55555555-5555-5555-5555-555555555555";
        MeshMemoryCache ram{bytes*2U};
        MeshDiskCache disk{root};
        int downloads=0;
        bool active=false;
        const auto fetch=[&] {
            ++downloads;
            return MeshAssetResult{true,id,tri,"downloaded"};
        };
        const auto skipped=load_mesh_cached(id,ram,disk,fetch,
            MeshLod::high,[&]{return active;});
        if(skipped.asset.success || downloads!=0 || disk.load(id))
            error=18;
        active=true;
        const auto finish_after_expiry=load_mesh_cached(id,ram,disk,
            [&] {
                ++downloads;
                active=false;
                return MeshAssetResult{true,id,tri,"slow response"};
            },MeshLod::high,[&]{return active;});
        if(!error && (finish_after_expiry.asset.success ||
            downloads!=1 || disk.load(id) || ram.load(id)))error=19;
        active=true;
        const auto success=load_mesh_cached(id,ram,disk,fetch,
            MeshLod::high,[&]{return active;});
        if(!error && (!success.asset.success || downloads!=2 ||
            !disk.load(id) || !ram.load(id)))error=20;
    }
    std::error_code ec;
    fs::remove_all(root,ec);
    if (error)
    {
        std::cerr << "Mesh cache test failed code " << error << '\n';
        return error;
    }
    std::cout << "Mesh disk/RAM integrity, LRU, cache tiers and errors OK\n";
    return 0;
}
