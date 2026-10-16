/* This Source Code Form is subject to Mozilla Public License, v. 2.0. */
#include "nexverse/ui/mesh_quality_evidence.hpp"
#include <iostream>
int main()
{
    using nexverse::assets::MeshLod;
    using nexverse::ui::inspect_mesh_quality;
    const std::unordered_map<std::string,MeshLod> desired{
        {"near",MeshLod::high},{"mid",MeshLod::medium},
        {"far",MeshLod::low},{"distant",MeshLod::lowest}};
    const std::unordered_map<std::string,MeshLod> gpu{
        {"near",MeshLod::low},{"mid",MeshLod::medium},
        {"far",MeshLod::low},{"old",MeshLod::high}};
    const auto e=inspect_mesh_quality(desired,gpu);
    if(e.high_requested!=1U||e.medium_requested!=1U||
       e.low_requested!=1U||e.lowest_requested!=1U)return 1;
    if(e.total_requested()!=4U||e.resident_gpu_lod!=3U||
       e.matched_gpu_lod!=2U)return 2;
    if(nexverse::ui::describe_mesh_quality(e).find("GPU 2/4")==
       std::string::npos)return 3;
    std::cout<<"Actual mesh UUID+LOD residency matching vs merely downloaded assets PASS\n";
    return 0;
}
