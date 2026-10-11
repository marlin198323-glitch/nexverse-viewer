#include "nexverse/assets/asset_missing_id_counter.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::assets;
    AssetMissingIdCounter tracker{2U};
    tracker.observe("mesh-a",AssetFailure::transient);
    if(tracker.events()!=0U||tracker.unique()!=0U)return 1;
    tracker.observe("mesh-a",AssetFailure::missing);
    tracker.observe("mesh-a",AssetFailure::missing);
    tracker.observe("mesh-b",AssetFailure::missing);
    tracker.observe("mesh-c",AssetFailure::missing);
    tracker.observe("mesh-c",AssetFailure::missing);
    if(tracker.events()!=5U||tracker.unique()!=2U ||
       tracker.overflow()!=2U)return 2;
    tracker.reset();
    if(tracker.events()!=0U||tracker.unique()!=0U ||
       tracker.overflow()!=0U)return 3;
    tracker.observe("mesh-c",AssetFailure::missing);
    if(tracker.unique()!=1U)return 4;
    AssetMissingIdCounter disabled{0U};
    disabled.observe("id",AssetFailure::missing);
    if(disabled.unique()!=0U||disabled.overflow()!=1U)return 5;
    std::cout<<"Missing asset unique-ID tracking, cap and epoch reset OK\n";
    return 0;
}
