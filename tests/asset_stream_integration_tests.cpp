#include "nexverse/assets/asset_request_fairness.hpp"
#include "nexverse/assets/asset_scene_coverage.hpp"
#include "nexverse/assets/asset_streaming_snapshot.hpp"
#include "nexverse/assets/asset_gpu_budget.hpp"
#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/assets/texture_residency.hpp"
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
int main()
{
    using namespace nexverse::assets;
    using namespace std::chrono;
    const auto now=AssetRetrySchedule::Clock::time_point{seconds{100}};
    AssetRetrySchedule retries;
    retries.enter(10U);
    (void)retries.failure("a",AssetFailure::transient,now);
    const auto residency=plan_texture_residency(
        {"a"},{"b","c","a"},{"c"},3U);
    if(residency.preferred_ids!=std::vector<std::string>({"a","b","c"}))
        return 1;
    AssetRequestFairness queue;
    const auto selected=queue.choose(residency.preferred_ids,
        {"c"},{},retries,now);
    if(!selected||selected->id!="b"||selected->is_retry)return 2;
    queue.dispatched();
    // Cached b is now GPU resident, while a is in backoff.
    if(queue.choose(residency.preferred_ids,{"b","c"},{},retries,now))
        return 3;
    // After expiry, the formerly blocked a is eligible.
    const auto next=queue.choose(residency.preferred_ids,{"b","c"},{},
        retries,now+seconds{2});
    if(!next||next->id!="a"||!next->is_retry)return 4;
    const std::unordered_map<std::string,std::size_t> gpu_bytes{
        {"b",80U},{"c",50U}};
    const auto byte_plan=plan_texture_byte_budget(
        residency.preferred_ids,gpu_bytes,"a",70U,
        effective_gpu_family_budget(200U,250U,100U));
    // No lower-priority asset may evict a higher-priority b for c.
    if(!byte_plan.fits || byte_plan.eviction_ids.size()!=1U ||
       byte_plan.eviction_ids[0]!="c")return 5;
    const auto health=asset_scene_coverage(
        residency.preferred_ids,{"a","b"});
    if(health.requested!=3U||health.resident!=2U||
       health.pending!=1U)return 6;
    AssetStreamingSnapshot snapshot;
    snapshot.region_epoch=10U;
    snapshot.textures=health;
    snapshot.gpu_shared_budget_bytes=250U;
    if(snapshot.json().find("\"texture_coverage_pct\":66")==
       std::string::npos)return 7;
    retries.enter(11U);
    queue.reset();
    if(retries.count()!=0U || queue.dispatched_count()!=0U ||
       !queue.choose(residency.preferred_ids,{"b","c"},{},retries,now))
        return 8;
    std::cout<<"Streaming queue, retry, GPU admission, region and scene snapshot integration OK\n";
    return 0;
}
