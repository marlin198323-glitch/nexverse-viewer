/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/avatar_bake_evidence.hpp"
#include "nexverse/protocol/avatar_cof_revision.hpp"
#include <iostream>
#include <unordered_set>

using namespace nexverse::protocol;
int main()
{
    if(!self_bake_owner_is_current("my-agent","my-agent","my-agent",true)||
       self_bake_owner_is_current("other","my-agent","my-agent",true)||
       self_bake_owner_is_current("my-agent","my-agent","other",true)||
       self_bake_owner_is_current("my-agent","my-agent","my-agent",false)||
       self_bake_owner_is_current("","","",true))
        return 10;
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

    // Both F9 manual and F9 live readback must select EXACTLY the
    // same meaningful Firestorm Low158 COF revision as WorldModel, even
    // if a trailing row is merely COF=0 (unknown/legacy).
    AvatarAppearanceMessage appearance{};
    if(avatar_cof_version_for_display(appearance)!=0U||
       latest_nonzero_avatar_cof(appearance))return 12;
    appearance.appearance_data={{2U,17U,0U},{2U,0U,0U}};
    const auto selected=latest_nonzero_avatar_cof(appearance);
    if(!selected||selected->appearance_version!=2U||
       avatar_cof_version_for_display(appearance)!=17U||
       describe_avatar_bake_evidence(
           avatar_cof_version_for_display(appearance),17U,partial).
           find("COF 17/17 OK")==std::string::npos)return 13;
    appearance.appearance_data.push_back({3U,42U,0U});
    appearance.appearance_data.push_back({3U,0U,0U});
    if(avatar_cof_version_for_display(appearance)!=42U||
       latest_nonzero_avatar_cof(appearance)->appearance_version!=3U)
        return 14;
    appearance.appearance_data={{2U,0U,0U},{3U,0U,0U}};
    if(avatar_cof_version_for_display(appearance)!=0U||
       latest_nonzero_avatar_cof(appearance))return 15;

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
    const auto large_version=
        describe_avatar_bake_evidence(4294967298ULL,4294967298ULL,partial);
    if(large_version.find("4294967298/4294967298 OK")==
       std::string::npos)return 11;
    const auto no_residency=audit_avatar_bake_evidence(bakes,{});
    if(no_residency.gpu_resident||no_residency.visible_body_resident||
       no_residency.server_references!=11U||no_residency.complete())
       return 4;
    std::cout<<"Firestorm 11-region simulator/GPU provenance verified (no upload)\\n";
    return 0;
}
