#include "nexverse/assets/texture_prefetch.hpp"
#include "nexverse/assets/asset_stream_scheduler.hpp"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>

using namespace nexverse::assets;
int main()
{
    namespace fs=std::filesystem;
    constexpr auto id1="aaaaaaaa-1111-2222-3333-444444444444";
    constexpr auto id2="bbbbbbbb-1111-2222-3333-444444444444";
    constexpr auto id3="cccccccc-1111-2222-3333-444444444444";
    const auto root=fs::temp_directory_path()/
        ("ogl-prefetch-test-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    const ImageRgba8 image{2U,2U,{
        200,0,0,255, 0,200,0,255,
        0,0,200,255, 200,200,0,255}};
    int error=0;
    {
        TextureDiskCache disk{root};
        TextureMemoryCache ram{256U};
        std::atomic<std::uint64_t> epoch{3U};
        if(!disk.store(id1,image))error=1;
        if(!error && !prefetch_texture_disk_to_memory(
                id1,disk,ram,epoch,3U))error=2;
        if(!error && (!ram.contains(id1) ||
                      !ram.load(id1) ||
                      ram.load(id1)->pixels!=image.pixels))error=3;
        epoch.store(4U);
        if(!error && prefetch_texture_disk_to_memory(
                id1,disk,ram,epoch,3U))error=4;
        if(!error && (!disk.store(id2,image) ||
                      prefetch_texture_disk_to_memory(
                          id2,disk,ram,epoch,3U) ||
                      ram.contains(id2)))error=5;
        if(!error && prefetch_texture_disk_to_memory(
                id3,disk,ram,epoch,4U))error=6;
        if(!error && (!prefetch_texture_disk_to_memory(
                id2,disk,ram,epoch,4U) ||
                      !ram.contains(id2)))error=7;
        // Tampered disk entry must never be admitted to the RAM cache.
        if(!error && disk.store(id3,image))
        {
            std::ofstream file{root/(std::string{id3}+".rgba"),
                std::ios::binary|std::ios::app};
            file.put('X');
            file.close();
            if (prefetch_texture_disk_to_memory(
                    id3,disk,ram,epoch,4U) || ram.contains(id3))error=8;
        }
        // Real async worker: disk read finishes AFTER new region
        // activation; stale decoded pixels may not enter RAM.
        std::promise<void> reader_started,release_reader;
        auto release=release_reader.get_future();
        auto old_region=std::async(std::launch::async,[&]
        {
            return prefetch_texture_disk_to_memory_scoped(id3,ram,
                epoch,4U,[&]() -> std::optional<ImageRgba8>
                {
                    reader_started.set_value();
                    release.wait();
                    return image;
                });
        });
        if(reader_started.get_future().wait_for(std::chrono::seconds{3})!=
           std::future_status::ready)
        {
            release_reader.set_value();
            (void)old_region.get();
            error=10;
        }
        else
        {
            epoch.store(5U,std::memory_order_release);
            release_reader.set_value();
            if(old_region.get()||ram.contains(id3))error=11;
        }
        TexturePrefetchScope scope;
        scope.enter(4U);
        scope.dispatched(id2);
        scope.enter(5U);
        if(!error && (scope.attempted()!=0U || scope.current(4U)))error=9;
    }
    std::error_code ec;
    fs::remove_all(root,ec);
    if(error)
    {
        std::cerr<<"Texture prefetch integration failed: "<<error<<'\n';
        return error;
    }
    std::cout<<"Disk-only texture prefetch, epoch guards and corrupt asset rejection OK\n";
    return 0;
}
