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
    }
    if (!error)
    {
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
