#include "nexverse/assets/asset_gpu_budget.hpp"
#include "nexverse/assets/gpu_staging_budget.hpp"
#include "nexverse/assets/gpu_staging_recovery.hpp"
#include "nexverse/assets/asset_request_fairness.hpp"
#include "nexverse/assets/asset_reuse_history.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    constexpr std::size_t mib=1024U*1024U;
    const auto now=GpuStagingRecovery::Clock::time_point{seconds{500}};
    GpuStagingRecovery textures,meshes;
    // Shared capacity can have room only after the other family shrinks.
    const auto mesh_budget=effective_gpu_family_budget(
        256U*mib,384U*mib,230U*mib);
    if(mesh_budget!=154U*mib)return 1;
    if(can_replace_gpu_bytes(140U*mib,0U,20U*mib,mesh_budget))
        return 2;
    const auto mesh_key=std::string{"building-mesh.medium"};
    meshes.defer(mesh_key,now,230U*mib,140U*mib);
    if(meshes.pending()!=1U ||
       !meshes.ready(now+seconds{1},230U*mib,140U*mib).empty())
        return 3;
    // Texture eviction frees 12 MiB, so the mesh may be retried NOW
    // instead of waiting for the full 2-second retry deadline.
    auto unblocked=meshes.ready(now+seconds{1},218U*mib,140U*mib);
    if(unblocked.size()!=1U||unblocked[0]!=mesh_key)return 4;
    const auto after=effective_gpu_family_budget(
        256U*mib,384U*mib,218U*mib);
    if(!can_replace_gpu_bytes(140U*mib,0U,20U*mib,after))return 5;
    // Full texture staging must not risk losing old GPU assets.
    if(gpu_staging_budget(300U*mib,70U*mib,8U*mib,384U*mib).fits)
        return 6;
    textures.defer("texture-1",now,300U*mib,70U*mib);
    if(!textures.ready(now+seconds{1},300U*mib,70U*mib).empty())
        return 7;
    if(textures.ready(now+seconds{2},300U*mib,70U*mib).size()!=1U)
        return 8;
    const auto texture_allowed=gpu_staging_budget(
        280U*mib,70U*mib,8U*mib,384U*mib);
    if(!texture_allowed.fits)return 9;
    // Cache metadata intentionally survives region epoch changes, but
    // GPU deferrals must not.
    AssetReuseHistory cache;
    (void)cache.uploaded("building-mesh",9U,2U,false);
    if(cache.uploaded("building-mesh",10U,2U,true)!=
       AssetReuseHistory::Kind::cached_region_reuse)return 10;
    meshes.defer(mesh_key,now,218U*mib,160U*mib);
    meshes.reset();
    textures.reset();
    if(meshes.pending()!=0U||textures.pending()!=0U ||
       cache.region_cache_hits()!=1U)return 11;
    // Retry queue remains eligible without a stale deferred key.
    AssetRetrySchedule failed;
    failed.enter(10U);
    AssetRequestFairness fairness;
    const auto choice=fairness.choose({mesh_key},{},{},failed,now);
    if(!choice||choice->id!=mesh_key)return 12;
    std::cout<<"Combined mesh/texture GPU pressure, cooldown release, cached epoch reuse OK\n";
    return 0;
}
