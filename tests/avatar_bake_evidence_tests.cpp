/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/avatar_bake_evidence.hpp"
#include <iostream>
#include <unordered_set>

using namespace nexverse::protocol;
int main()
{
    AvatarBakedTextures bakes;
    const std::unordered_set<std::string> gpu{"head","eyes","stale"};
    auto empty=audit_avatar_bake_evidence(bakes,gpu);
    if(empty.server_references||empty.gpu_resident||empty.visible_body_resident||
       empty.complete()||empty.upload_authorized()||
       empty.regions[0]!=AvatarBakeTextureState::missing)return 1;

    bakes.ids[0]="head"; // original TE Face 8 / Head
    bakes.ids[1]="upper"; // original TE Face 9 / Upper
    bakes.ids[3]="eyes"; // original TE Face 11 / Eyes
    const auto partial=audit_avatar_bake_evidence(bakes,gpu);
    if(partial.server_references!=3U||partial.gpu_resident!=2U||
       partial.visible_body_resident!=1U||partial.complete()||
       partial.upload_authorized()||partial.regions[0]!=AvatarBakeTextureState::gpu_resident||
       partial.regions[1]!=AvatarBakeTextureState::server_reference||
       partial.regions[2]!=AvatarBakeTextureState::missing||
       partial.regions[3]!=AvatarBakeTextureState::gpu_resident||
       avatar_bake_state_symbol(partial.regions[0])!='G'||
       avatar_bake_state_symbol(partial.regions[1])!='S'||
       avatar_bake_state_symbol(partial.regions[2])!='-')
        return 2;

    const auto label=describe_avatar_bake_evidence(17U,17U,partial);
    if(label.find("KEIN BAKE")==std::string::npos||
       label.find("COF 17/17 OK")==std::string::npos||
       label.find("TE 3/11 GPU 2/11 Body 1/3")==std::string::npos||
       label.find("H:G U:S L:-")==std::string::npos||label.size()>95U)
        return 5;
    const auto live=describe_avatar_bake_evidence(17U,17U,partial,true);
    if(live.find("KEIN BAKE LIVE | COF 17/17 OK")!=0U ||
       live.size()>95U)return 9;
    const auto stale=describe_avatar_bake_evidence(17U,18U,partial);
    const auto unknown=describe_avatar_bake_evidence(0U,0U,partial);
    if(stale.find("COF 17/18 OFFEN")==std::string::npos||
       unknown.find("COF 0/0 OFFEN")==std::string::npos)
        return 6;

    // A full set of server references is still NOT permission to bake/upload.
    const std::unordered_set<std::string> all_gpu{
        "head","upper","eyes","leg","skirt","hair","leftarm","leftleg",
        "aux1","aux2","aux3"};
    const std::array<std::string,11U> ids{
        "head","upper","leg","eyes","skirt","hair","leftarm","leftleg",
        "aux1","aux2","aux3"};
    bakes.ids=ids;
    const auto full=audit_avatar_bake_evidence(bakes,all_gpu);
    if(!full.complete()||full.server_references!=11U||
       full.gpu_resident!=11U||full.visible_body_resident!=3U||
       full.upload_authorized()||avatar_bake_region_names[10]!="Aux3")
       return 3;
    const auto no_residency=audit_avatar_bake_evidence(bakes,{});
    if(no_residency.gpu_resident||no_residency.visible_body_resident||
       no_residency.server_references!=11U||no_residency.complete())
       return 4;
    std::cout<<"Firestorm 11-region simulator/GPU provenance verified (no upload)\\n";
    return 0;
}
