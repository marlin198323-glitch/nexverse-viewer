#include "nexverse/protocol/region_material_scope.hpp"
#include "nexverse/assets/mesh_cache.hpp"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <utility>
namespace fs=std::filesystem;
int main()
{
    using namespace nexverse;
    const auto temp=fs::temp_directory_path()/(
        "ogl76-region-epoch-contract-"+std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    const std::string uuid="66666666-6666-6666-6666-666666666666";
    assets::MeshGeometry mesh{
        {0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1},
        {0,1,2},{{0,0,3}}};
    protocol::RegionMaterialScope scope;
    if(!scope.update(100U,1U,"seed-A"))return 1;
    const auto epochA=scope.epoch();
    if(scope.update(100U,1U,"seed-A")||!scope.matches(epochA))
        return 2;
    std::atomic<std::uint64_t> epoch{epochA};
    assets::MeshMemoryCache memory;
    assets::MeshDiskCache disk{temp};
    std::promise<void> fetch_entered;
    std::promise<void> allow_reply;
    auto reply_ready=allow_reply.get_future();
    auto job=std::async(std::launch::async,[&]
    {
        return assets::load_mesh_cached(uuid,memory,disk,
            [&]
            {
                fetch_entered.set_value();
                reply_ready.wait();
                return assets::MeshAssetResult{true,uuid,mesh,"mocked HTTP 200"};
            },
            assets::MeshLod::high,[&]
            {return epoch.load(std::memory_order_acquire)==epochA;});
    });
    if(fetch_entered.get_future().wait_for(std::chrono::seconds{3})!=
        std::future_status::ready)
    {
        allow_reply.set_value();
        (void)job.get();
        return 3;
    }
    if(!scope.update(200U,1U,"seed-B"))return 4;
    epoch.store(scope.epoch(),std::memory_order_release);
    allow_reply.set_value();
    const auto discarded=job.get();
    if(discarded.asset.success || memory.load(uuid).has_value() ||
       disk.load(uuid)||scope.matches(epochA))
        return 5;
    const auto epochB=scope.epoch();
    int fetches=0;
    const auto accepted=assets::load_mesh_cached(uuid,memory,disk,
        [&]
        {
            ++fetches;
            return assets::MeshAssetResult{true,uuid,mesh,"new HTTP result"};
        },assets::MeshLod::high,[&]
        {return epoch.load(std::memory_order_acquire)==epochB;});
    if(!accepted.asset.success||fetches!=1 ||!memory.load(uuid) ||
       !disk.load(uuid))return 6;
    if(!scope.update(100U,1U,"seed-A")||scope.matches(epochA)||
       scope.matches(epochB))return 7;
    epoch.store(scope.epoch(),std::memory_order_release);
    const auto cached=assets::load_mesh_cached(uuid,memory,disk,[] {
        return assets::MeshAssetResult{};},assets::MeshLod::high,[&]
        {return epoch.load(std::memory_order_acquire)==scope.epoch();});
    if(!cached.asset.success ||cached.tier!=assets::MeshCacheTier::memory)
        return 8;
    fs::remove_all(temp);
    std::cout<<"Region A->B->A epoch: delayed mocked HTTP response rejected,"
        " no poisoned cache, new request accepted\n";
    return 0;
}
