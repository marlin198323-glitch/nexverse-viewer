/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/assets/firestorm_wearable_asset.hpp"
#include <cassert>
#include <iostream>
using namespace nexverse::assets;
int main()
{
    // Legacy Firestorm wearable text format (not handwaved JSON).
    // Local permission fields may contain other numeric settings.
    const std::string fixture=
        "LLWearable version 1\n"
        "Firestorm Shirt\n"
        "Cotton layers\n"
        "permissions 0\n"
        "{\n"
        "base_mask 7fffffff\n"
        "owner_id 11111111-1111-1111-1111-111111111111\n"
        "}\n"
        "sale_info 0\n"
        "{\n"
        "sale_type not\n"
        "sale_price 10\n"
        "}\n"
        "type 4\n"
        "parameters 2\n"
        "101 0.5\n"
        "203 -0.125\n"
        "textures 2\n"
        "1 22222222-2222-2222-2222-222222222222\n"
        "10 33333333-3333-3333-3333-333333333333\n";
    const auto good=parse_firestorm_wearable(fixture,4U);
    if(!good.ok()||good.asset->type!=4||
       good.asset->visual_params.size()!=2U||
       good.asset->visual_params.at(203)!=-0.125f||
       good.asset->textures.size()!=2U||
       good.asset->textures.at(1)!="22222222-2222-2222-2222-222222222222"||
       good.asset->name!="Firestorm Shirt")return 1;
    const auto audit=audit_firestorm_bake(&*good.asset);
    if(!audit.has_asset||audit.params!=2U||audit.texture_refs!=2U||
       !audit.baked_layers[1]||audit.baked_output_refs!=1U||
       audit.bake_ready)return 2;
    if(firestorm_texture_indices!=45U||
       firestorm_local_bake_groups[1U]!=1||
       firestorm_local_bake_groups[2U]!=2||
       firestorm_local_bake_groups[26U]!=0||
       firestorm_local_bake_groups[35U]!=6||
       firestorm_local_bake_groups[44U]!=-1)return 9;
    if(parse_firestorm_wearable(fixture,0U).ok())return 3;
    const auto invalid=[](std::string input,const std::string& old,
                          const std::string& replacement)
    {
        auto pos=input.find(old);
        if(pos==std::string::npos)return false;
        input.replace(pos,old.size(),replacement);
        return !parse_firestorm_wearable(input,4U).ok();
    };
    if(!invalid(fixture,"LLWearable version 1","LLWearable version 999")||
       !invalid(fixture,"permissions 0","permissions 1")||
       !invalid(fixture,"sale_info 0","sale_info 1")||
       !invalid(fixture,"type 4","type 16")||
       !invalid(fixture,"parameters 2","parameters 1001")||
       !invalid(fixture,"parameters 2","parameters -1")||
       !invalid(fixture,"textures 2","textures 101")||
       !invalid(fixture,"textures 2","textures -1")||
       !invalid(fixture,"101 0.5","101 nan")||
       !invalid(fixture,"101 0.5","101 inf")||
       !invalid(fixture,"101 0.5","101 0.5\n101 0.6")||
       !invalid(fixture,"1 22222222-2222-2222-2222-222222222222",
                "47 22222222-2222-2222-2222-222222222222")||
       !invalid(fixture,"1 22222222-2222-2222-2222-222222222222",
                "1 00000000-0000-0000-0000-000000000000")||
       !invalid(fixture,"1 22222222-2222-2222-2222-222222222222",
                "1 bad-id")||
       !invalid(fixture,"1 22222222-2222-2222-2222-222222222222",
                "1 33333333-3333-3333-3333-333333333333\n"
                "1 44444444-4444-4444-4444-444444444444")||
       !invalid(fixture,"textures 2","textures 2\ninjected"))
        return 4;
    if(parse_firestorm_wearable(
        std::string(max_firestorm_wearable_bytes+1U,'a')).ok())
        return 5;
    if(parse_firestorm_wearable(
        fixture+std::string(max_firestorm_wearable_line+2U,'b')).ok())
        return 6;
    if(parse_firestorm_wearable(fixture+"garbage\n").ok())return 7;
    if(audit_firestorm_bake(nullptr).has_asset)return 8;
    std::cout<<"Firestorm v1 Wearable text asset bounded permission/sale/17 type parser, visual params and texture->bake reference audit; no false GPU bake OK\n";
    return 0;
}
