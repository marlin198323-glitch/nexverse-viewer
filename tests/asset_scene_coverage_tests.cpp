#include "nexverse/assets/asset_scene_coverage.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    const auto empty=asset_scene_coverage({},{});
    if (empty.requested != 0U || empty.pending != 0U ||
        empty.percentage() != 100U)return 1;
    const auto scene=asset_scene_coverage(
        {"a","b","a","","c","d"},{"a","c","unrelated"});
    if (scene.requested != 4U || scene.resident != 2U ||
        scene.pending != 2U || scene.percentage() != 50U)return 2;
    const auto pending=asset_scene_coverage(
        {"a","b","c"},{});
    if(pending.pending != 3U || pending.percentage()!=0U)return 3;
    const auto full=asset_scene_coverage(
        {"a","b"},{"a","b","unused"});
    if(full.percentage()!=100U || full.pending!=0U)return 4;
    std::cout<<"In-world streaming coverage and pending-asset calculations OK\n";
    return 0;
}
