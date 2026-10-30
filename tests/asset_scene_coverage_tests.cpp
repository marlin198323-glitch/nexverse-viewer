#include "nexverse/assets/asset_scene_coverage.hpp"
#include "nexverse/assets/texture_mip_evidence.hpp"
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
    // GPU-resident UUID count alone is not a visual-quality metric:
    // both base-mip and downscaled-mip IDs count as resident, but must
    // remain distinguishable in UI/streaming evidence.
    const auto mips=texture_mip_evidence(
        {"a","b","a","c","unknown","pending",""},
        {"a","b","c","unknown","unused"},
        {{"a",0U},{"b",2U},{"c",3U}});
    if(mips.base_mip!=1U||mips.reduced_mip!=2U||
       mips.unknown_mip!=1U||mips.counted()!=4U)
        return 6;
    const auto absent=texture_mip_evidence({"a","a","b"}, {}, {});
    if(absent.counted()!=0U)return 7;
    const auto all_base=texture_mip_evidence(
        {"x","y"},{"x","y"},{{"x",0U},{"y",0U}});
    if(all_base.base_mip!=2U||all_base.reduced_mip!=0U)
        return 8;
    std::cout<<"In-world streaming coverage and pending-asset calculations OK\n";
    return 0;
}
