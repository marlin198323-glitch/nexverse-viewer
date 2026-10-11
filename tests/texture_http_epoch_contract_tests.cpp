#include "nexverse/assets/texture_cached_request.hpp"
#include "nexverse/protocol/region_material_scope.hpp"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
namespace fs=std::filesystem;
int main()
{
    using namespace nexverse;
    const auto base=fs::temp_directory_path()/(
        "ogl77-texture-epoch-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    const std::string id="77777777-7777-7777-7777-777777777777";
    const assets::ImageRgba8 pixels{4U,4U,
        std::vector<std::uint8_t>(4U*4U*4U,190U)};
    assets::TextureMemoryCache ram{128U};
    assets::TextureDiskCache disk{base};
    protocol::RegionMaterialScope region;
    if(!region.update(1U,1U,"A"))return 1;
    std::atomic<std::uint64_t> active{region.epoch()};
    const auto epochA=region.epoch();
    int calls=0;
    const auto result=[&](std::string name)
    {
        ++calls;
        assets::TextureAssetResult r;
        r.success=true;
        r.asset_id=std::move(name);
        r.image=pixels;
        r.message="fixture response";
        return r;
    };
    if(!region.update(2U,1U,"B"))return 2;
    active.store(region.epoch(),std::memory_order_release);
    const auto stale=assets::load_texture_cached_scoped(id,1U,ram,disk,
        [&]{return result(id);},
        [&]{return active.load(std::memory_order_acquire)==epochA;});
    if(stale.success||!stale.cancelled_before_download||calls!=0||
       ram.contains(id)||disk.load(id))return 3;
    const auto epochB=region.epoch();
    std::promise<void> inside_download,finish_download;
    auto allow=finish_download.get_future();
    auto job=std::async(std::launch::async,[&]
    {
        return assets::load_texture_cached_scoped(id,1U,ram,disk,
            [&]
            {
                inside_download.set_value();
                allow.wait();
                return result(id);
            },[&]{return active.load(std::memory_order_acquire)==epochB;});
    });
    if(inside_download.get_future().wait_for(std::chrono::seconds{3})!=
        std::future_status::ready)
    {
        finish_download.set_value();
        (void)job.get();
        return 4;
    }
    if(!region.update(3U,1U,"C"))return 5;
    active.store(region.epoch(),std::memory_order_release);
    finish_download.set_value();
    const auto stale_reply=job.get();
    if(stale_reply.success||!stale_reply.network_requested ||
       calls!=1||ram.contains(id)||disk.load(id))return 6;
    const auto epochC=region.epoch();
    const auto mismatch=assets::load_texture_cached_scoped(id,0U,ram,disk,
        [&]{return result("88888888-8888-8888-8888-888888888888");},
        [&]{return active.load()==epochC;});
    if(mismatch.success||mismatch.failure!=assets::AssetFailure::invalid ||
       ram.contains(id)||disk.load(id))return 7;
    const auto valid=assets::load_texture_cached_scoped(id,1U,ram,disk,
        [&]{return result(id);},[&]{return active.load()==epochC;});
    if(!valid.success||!valid.network_requested ||
       valid.image.width!=2U||valid.image.height!=2U||
       !ram.contains(id)||!disk.load(id)||ram.load(id)->width!=4U)
        return 8;
    const int network_before=calls;
    const auto cache=assets::load_texture_cached_scoped(id,0U,ram,disk,
        [&]{return result(id);},[&]{return active.load()==epochC;});
    if(!cache.success||cache.network_requested||calls!=network_before ||
       cache.message!="RAM texture cache hit"||cache.image.width!=4U)
        return 9;
    assets::TextureMemoryCache fresh{128U};
    const auto disk_hit=assets::load_texture_cached_scoped(id,0U,fresh,disk,
        [&]{return result(id);},[&]{return active.load()==epochC;});
    if(!disk_hit.success||disk_hit.network_requested ||
       disk_hit.message!="Disk texture cache hit"||calls!=network_before)
        return 10;
    fs::remove_all(base);
    std::cout<<"Stale GetTexture HTTP after A/B/C region changes rejected "
        "before RAM/disk commit; UUID and mip isolation OK\n";
    return 0;
}
