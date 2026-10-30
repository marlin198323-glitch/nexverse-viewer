/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/ui/asset_stream_proof.hpp"
#include <array>
#include <iostream>
#include <string>
struct Asset {std::string id;};
int main()
{
    using namespace nexverse::ui;
    const std::array<Asset,5U> requested{{
        {"texture1"},{"texture2"},{"texture1"},{""},
        {"00000000-0000-0000-0000-000000000000"}}};
    const auto first=asset_gpu_coverage(requested,{"texture1","unrelated"});
    if(first.requested!=2U||first.gpu_resident!=1U)return 1;
    if(asset_gpu_coverage(std::array<Asset,0U>{},{"texture1"}).requested!=0U)
        return 2;
    const AssetStreamProof proof{first,asset_gpu_coverage(
        std::array<Asset,2U>{{{"mesh1"},{"mesh2"}}},{"mesh2"})};
    if(describe_asset_stream(proof)!="Tex-IDs 1/2 GPU | Mesh-IDs 1/2 GPU")
        return 3;
    std::cout<<"World GPU texture and mesh asset residency provenance PASS\n";
    return 0;
}
