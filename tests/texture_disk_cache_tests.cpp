#include "nexverse/assets/texture_disk_cache.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

using nexverse::assets::TextureDiskCache;
using nexverse::assets::AssetCacheLookup;
using nexverse::assets::ImageRgba8;
int main()
{
    const auto root=std::filesystem::temp_directory_path()/
        ("ogl-cache-test-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    constexpr const char* a="11111111-1111-1111-1111-111111111111";
    constexpr const char* b="22222222-2222-2222-2222-222222222222";
    const ImageRgba8 red{1U,1U,{255U,0U,0U,128U}};
    const ImageRgba8 blue{1U,1U,{0U,0U,255U,255U}};
    int error=0;
    {
        TextureDiskCache cache{root,128U};
        AssetCacheLookup status=AssetCacheLookup::not_checked;
        if(cache.load(a,&status) || status!=AssetCacheLookup::miss)error=10;
        if(!error && !cache.store(a,red)) error=1;
        else if(!error)
        {
            const auto got=cache.load(a,&status);
            if(!got || got->pixels!=red.pixels ||
               status!=AssetCacheLookup::hit)error=2;
        }
        if(!error && cache.load("../escape"))error=3;
        if(!error && !cache.store(b,blue))error=4;
        if(!error)
        {
            std::ofstream f{root/(std::string{a}+".rgba"),
                std::ios::binary|std::ios::app};
            f.put('X'); // Wrong size must reject, never read unchecked.
        }
        if(!error)
        {
            if(cache.load(a,&status) ||
               status!=AssetCacheLookup::corrupt)error=5;
        }
    }
    // The same directory is readable by a brand new cache instance.
    if(!error)
    {
        TextureDiskCache cache{root,28U}; // single 1x1 RGBA entry
        if(const auto got=cache.load(b);
            !got || got->pixels!=blue.pixels)error=6;
        else if(cache.prune()!=1U || cache.load(a) || !cache.load(b))
            error=7;
    }
    std::error_code ec;
    std::filesystem::remove_all(root,ec);
    if(error)
    {
        std::cerr<<"Texture disk cache test failed code "<<error<<'\n';
        return error;
    }
    std::cout<<"Disk cache integrity, bounds, restart and LRU OK\n";
    return 0;
}
