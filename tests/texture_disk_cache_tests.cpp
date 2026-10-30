#include "nexverse/assets/texture_disk_cache.hpp"
#include <chrono>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>

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
        if(cache.contains(a)||cache.contains("../escape"))error=34;
        if(!error && !cache.store(a,red)) error=1;
        if(!error && !cache.contains(a))error=35;
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
    // Firestorm-style independent disk budget: no baked-in GPU memory
    // assumption, and oversized/invalid environment input falls back.
    constexpr std::uint64_t mib=1024ULL*1024ULL;
    const auto default_size=TextureDiskCache::default_budget_bytes();
    if(default_size<128ULL*mib||default_size>2048ULL*mib)error=30;
#if defined(__unix__) || defined(__APPLE__)
    const char* current=std::getenv("OGL_TEXTURE_CACHE_MIB");
    const bool had_value=current!=nullptr;
    const std::string original=had_value?std::string{current}:std::string{};
    if(!error)
    {
        ::setenv("OGL_TEXTURE_CACHE_MIB","128",1);
        if(TextureDiskCache::default_budget_bytes()!=128ULL*mib)error=31;
        ::setenv("OGL_TEXTURE_CACHE_MIB","2048",1);
        if(TextureDiskCache::default_budget_bytes()!=2048ULL*mib)error=32;
        for(const char* malformed:{"0","99999","garbage","1x2","2049"})
        {
            ::setenv("OGL_TEXTURE_CACHE_MIB",malformed,1);
            if(TextureDiskCache::default_budget_bytes()!=512ULL*mib)
                error=33;
        }
    }
    if(had_value)::setenv("OGL_TEXTURE_CACHE_MIB",original.c_str(),1);
    else ::unsetenv("OGL_TEXTURE_CACHE_MIB");
#endif
    // Typical world tile: lossless 1024x1024 RGBA used to consume
    // 4 MiB of the 256 MiB persistent cache per single UUID.
    // OGLTEX02 must survive restart and preserve EVERY RGBA byte.
    constexpr const char* large="33333333-3333-3333-3333-333333333333";
    constexpr const char* legacy="44444444-4444-4444-4444-444444444444";
    constexpr const char* corrupted="55555555-5555-5555-5555-555555555555";
    ImageRgba8 scene_tile;
    scene_tile.width=1024U;scene_tile.height=1024U;
    scene_tile.pixels.resize(1024U*1024U*4U);
    for(std::size_t i=0U;i<scene_tile.pixels.size();i+=4U)
    {
        scene_tile.pixels[i]=84U;
        scene_tile.pixels[i+1U]=static_cast<std::uint8_t>((i/4096U)%4U*16U);
        scene_tile.pixels[i+2U]=90U;
        scene_tile.pixels[i+3U]=255U;
    }
    if(!error)
    {
        TextureDiskCache cache{root,8U*1024U*1024U};
        if(!cache.store(large,scene_tile))error=20;
        else
        {
            const auto path=root/(std::string{large}+".rgba");
            std::ifstream file{path,std::ios::binary};
            std::array<char,8> header{};
            file.read(header.data(),8);
            if(header!=std::array<char,8>{'O','G','L','T','E','X','0','2'}||
               std::filesystem::file_size(path)>=scene_tile.pixels.size()/3U)
                error=21;
        }
    }
    if(!error)
    {
        const TextureDiskCache reopened{root,8U*1024U*1024U};
        AssetCacheLookup status=AssetCacheLookup::not_checked;
        const auto decoded=reopened.load(large,&status);
        if(!decoded||status!=AssetCacheLookup::hit||
           decoded->width!=scene_tile.width||
           decoded->height!=scene_tile.height||
           decoded->pixels!=scene_tile.pixels)error=22;
    }
    if(!error)
    {
        // Build a genuine legacy OGLTEX01 using the existing known-good
        // raw record. On load it must migrate to compact v2, not merely
        // tolerate it or throw away the old global user cache.
        const TextureDiskCache cache{root,8U*1024U*1024U};
        const auto old=root/(std::string{legacy}+".rgba");
        if(!cache.store(legacy,scene_tile))error=23;
        else
        {
            // Current store wrote a v2 file. Replace with a compliant
            // v1 header + full plain RGBA payload while retaining
            // checksum, dimensions and known-good pixel content.
            std::ifstream src{old,std::ios::binary};
            std::array<char,24> record{};
            src.read(record.data(),static_cast<std::streamsize>(record.size()));
            src.close();
            record[6]='0';record[7]='1';
            {
                std::ofstream file{old,std::ios::binary|std::ios::trunc};
                file.write(record.data(),24);
                file.write(reinterpret_cast<const char*>(scene_tile.pixels.data()),
                    static_cast<std::streamsize>(scene_tile.pixels.size()));
            }
            auto decoded=cache.load(legacy);
            std::ifstream check{old,std::ios::binary};
            std::array<char,8> marker{};
            check.read(marker.data(),8);
            if(!decoded||decoded->pixels!=scene_tile.pixels||
               marker!=std::array<char,8>{'O','G','L','T','E','X','0','2'}||
               std::filesystem::file_size(old)>=scene_tile.pixels.size()/3U)
                error=24;
        }
    }
    if(!error)
    {
        // Even a valid zlib stream with trailing attacker-controlled
        // bytes is not considered a valid cache record.
        const TextureDiskCache cache{root,8U*1024U*1024U};
        const auto good=root/(std::string{large}+".rgba");
        const auto broken=root/(std::string{corrupted}+".rgba");
        std::filesystem::copy_file(good,broken,
            std::filesystem::copy_options::overwrite_existing);
        {
            std::ofstream out{broken,std::ios::binary|std::ios::app};
            out.put('X');
        }
        AssetCacheLookup status=AssetCacheLookup::not_checked;
        if(cache.load(corrupted,&status)||status!=AssetCacheLookup::corrupt)
            error=25;
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
