#include "nexverse/assets/asset_streaming_snapshot.hpp"
#include <iostream>
#include <string>
int main()
{
    using namespace nexverse::assets;
    AssetStreamingSnapshot snapshot;
    snapshot.region_epoch=77U;
    snapshot.texture_scene_samples=42U;
    snapshot.mesh_scene_samples=11U;
    snapshot.textures={4U,3U,1U};
    snapshot.meshes={2U,1U,1U};
    snapshot.texture_retry_blocked=2U;
    snapshot.mesh_ready_retries=1U;
    snapshot.gpu_texture_bytes=240U;
    snapshot.gpu_mesh_bytes=80U;
    snapshot.gpu_shared_budget_bytes=384U;
    snapshot.gpu_upload_cpu_samples=12U;
    snapshot.gpu_upload_cpu_p95_us=1300U;
    snapshot.gpu_upload_pacing_ms=30U;
    snapshot.mesh_priority_churn=2U;
    snapshot.texture_gpu_upload_failures=3U;
    snapshot.mesh_gpu_upload_failures=4U;
    snapshot.sequence=7U;
    snapshot.session_started_unix_ms=1800000000000ULL;
    snapshot.emitted_unix_ms=1800000090000ULL;
    snapshot.texture_gpu_staging_waiting=1U;
    snapshot.texture_gpu_staging_released=2U;
    snapshot.mesh_gpu_budget_waiting=3U;
    snapshot.mesh_gpu_budget_released=4U;
    snapshot.texture_cpu_cached_region_reuse=5U;
    snapshot.texture_cpu_cached_lod_reuse=6U;
    snapshot.mesh_cpu_cached_region_reuse=7U;
    snapshot.mesh_cpu_cached_lod_reuse=8U;
    snapshot.texture_ram_hits=41U;
    snapshot.texture_ram_misses=21U;
    snapshot.texture_ram_evictions=7U;
    snapshot.mesh_ram_hits=31U;
    snapshot.mesh_ram_misses=11U;
    snapshot.mesh_ram_evictions=5U;
    snapshot.mesh_fail_missing=6U;
    snapshot.mesh_fail_transient=7U;
    snapshot.mesh_fail_invalid=8U;
    snapshot.mesh_fail_configuration=9U;
    snapshot.texture_cpu_cached_region_ram=9U;
    snapshot.texture_cpu_cached_region_disk=10U;
    snapshot.mesh_cpu_cached_lod_ram=11U;
    snapshot.mesh_cpu_cached_lod_disk=12U;
    snapshot.mesh_cpu_reuse_history_evictions=13U;
    snapshot.previous_frame_scene_missing_mesh_draw_calls=3U;
    snapshot.previous_frame_scene_missing_mesh_objects=2U;
    snapshot.previous_frame_scene_missing_mesh_triangles=36U;
    snapshot.texture_gpu_staging_deferrals=5U;
    snapshot.previous_frame_draw_calls=20U;
    snapshot.previous_frame_scene_draw_calls=9U;
    snapshot.previous_frame_scene_mesh_draw_calls=7U;
    snapshot.previous_frame_scene_prim_draw_calls=2U;
    const auto json=snapshot.json();
    if(json.rfind("{\"schema\":\"ogl.asset.streaming.v1\"",0U)!=0U ||
       json.back()!='}' ||
       json.find("\"region_epoch\":77")==std::string::npos ||
       json.find("\"textures_wanted\":4")==std::string::npos ||
       json.find("\"texture_coverage_pct\":75")==std::string::npos ||
       json.find("\"mesh_coverage_pct\":50")==std::string::npos ||
       json.find("\"textures_pending\":1")==std::string::npos ||
       json.find("\"mesh_retry_blocked\":0")==std::string::npos ||
       json.find("\"gpu_shared_budget_bytes\":384")==std::string::npos ||
       json.find("\"gpu_upload_cpu_p95_us\":1300")==std::string::npos ||
       json.find("\"mesh_priority_churn\":2")==std::string::npos ||
       json.find("\"texture_gpu_upload_failures\":3")==std::string::npos ||
       json.find("\"mesh_gpu_upload_failures\":4")==std::string::npos ||
       json.find("\"sequence\":7")==std::string::npos ||
       json.find("\"texture_gpu_staging_waiting\":1")==std::string::npos ||
       json.find("\"texture_gpu_staging_released\":2")==std::string::npos ||
       json.find("\"mesh_gpu_budget_waiting\":3")==std::string::npos ||
       json.find("\"mesh_gpu_budget_released\":4")==std::string::npos ||
       json.find("\"texture_cpu_cached_region_reuse\":5")==std::string::npos ||
       json.find("\"mesh_cpu_cached_lod_reuse\":8")==std::string::npos ||
       json.find("\"texture_ram_hits\":41")==std::string::npos ||
       json.find("\"texture_ram_evictions\":7")==std::string::npos ||
       json.find("\"mesh_ram_misses\":11")==std::string::npos ||
       json.find("\"mesh_ram_evictions\":5")==std::string::npos ||
       json.find("\"mesh_fail_missing\":6")==std::string::npos ||
       json.find("\"mesh_fail_configuration\":9")==std::string::npos ||
       json.find("\"texture_cpu_cached_region_ram\":9")==std::string::npos ||
       json.find("\"texture_cpu_cached_region_disk\":10")==std::string::npos ||
       json.find("\"mesh_cpu_cached_lod_ram\":11")==std::string::npos ||
       json.find("\"mesh_cpu_cached_lod_disk\":12")==std::string::npos ||
       json.find("\"mesh_cpu_reuse_history_evictions\":13")==std::string::npos ||
       json.find("\"previous_frame_scene_missing_mesh_draw_calls\":3")==std::string::npos ||
       json.find("\"previous_frame_scene_missing_mesh_objects\":2")==std::string::npos ||
       json.find("\"previous_frame_scene_missing_mesh_triangles\":36")==std::string::npos ||
       json.find("\"texture_gpu_staging_deferrals\":5")==std::string::npos ||
       json.find("\"previous_frame_scene_mesh_draw_calls\":7")==std::string::npos ||
       json.find("\"previous_frame_scene_prim_draw_calls\":2")==std::string::npos)
        return 1;
    AssetStreamingSnapshot none;
    const auto blank=none.json();
    if(blank.find("\"texture_coverage_pct\":100")==std::string::npos||
       blank.find("\"mesh_coverage_pct\":100")==std::string::npos)
        return 2;
    std::cout<<"Fixed-schema numeric streaming snapshot and empty scene OK\n";
    return 0;
}
