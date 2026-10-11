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
    const auto json=snapshot.json();
    if(json.rfind("{\"schema\":\"ogl.asset.streaming.v1\"",0U)!=0U ||
       json.back()!='}' ||
       json.find("\"region_epoch\":77")==std::string::npos ||
       json.find("\"textures_wanted\":4")==std::string::npos ||
       json.find("\"texture_coverage_pct\":75")==std::string::npos ||
       json.find("\"mesh_coverage_pct\":50")==std::string::npos ||
       json.find("\"textures_pending\":1")==std::string::npos ||
       json.find("\"mesh_retry_blocked\":0")==std::string::npos ||
       json.find("\"gpu_shared_budget_bytes\":384")==std::string::npos)
        return 1;
    AssetStreamingSnapshot none;
    const auto blank=none.json();
    if(blank.find("\"texture_coverage_pct\":100")==std::string::npos||
       blank.find("\"mesh_coverage_pct\":100")==std::string::npos)
        return 2;
    std::cout<<"Fixed-schema numeric streaming snapshot and empty scene OK\n";
    return 0;
}
