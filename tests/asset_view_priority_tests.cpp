#include "nexverse/assets/asset_view_priority.hpp"
#include "nexverse/assets/texture_distance.hpp"
#include "nexverse/assets/mesh_residency.hpp"
#include <cmath>
#include <iostream>
#include <unordered_set>
int main()
{
    using namespace nexverse::assets;
    const AssetCameraHint camera{0,0,0,1,0,0};
    if(view_priority_multiplier(camera,20,0,0)!=1.0 ||
       view_priority_multiplier(camera,1,0,0)!=1.0 ||
       view_priority_multiplier(camera,-100,0,0)<=2.0)
        return 1;
    const auto front=rank_texture_distances(
        {{"behind",-30,0,0},{"visible",40,0,0}},
        0,0,0,{},8.0f,&camera);
    if(front.size()!=2U || front[0]!="visible")return 2;
    const auto near=rank_texture_distances(
        {{"near",-6,0,0},{"far",60,0,0}},
        0,0,0,{},8.0f,&camera);
    if(near.size()!=2U || near[0]!="near")return 3;
    const auto mesh=plan_mesh_residency(
        {{"behind",-30,0,0,1},{"visible",40,0,0,1}},
        0,0,0,{},2U,24.0f,&camera);
    if(mesh.prioritized_ids.size()!=2U ||
       mesh.prioritized_ids[0]!="visible")return 4;
    const auto legacy=rank_texture_distances(
        {{"behind",-30,0,0},{"visible",40,0,0}},0,0,0,{});
    if(legacy[0]!="behind")return 5;
    const AssetCameraHint invalid{0,0,0,NAN,0,0};
    if(view_priority_multiplier(invalid,20,0,0)!=1.0)return 6;
    std::cout<<"Camera-aware soft asset priority tests passed\n";
    return 0;
}
