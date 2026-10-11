#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <iostream>
#include <string>
using namespace nexverse::protocol;
int main()
{
    constexpr auto A="11111111-1111-1111-1111-111111111111";
    constexpr auto B="22222222-2222-2222-2222-222222222222";
    CapabilityMap caps{
        {"FetchInventory2","https://caps.example.test/inv"},
        {"AgentExperiences","https://caps.example.test/owned"},
        {"ExperiencePreferences","https://caps.example.test/permissions"}
    };
    const auto support=firestorm_caps_availability(caps);
    if(!support.inventory_fetch||support.inventory_folder_fetch||
       support.inventory_thumbnail_upload||!support.experience_owned||
       !support.experience_permissions||support.experience_lists||
       support.advertised()!=3U)return 1;
    caps.emplace("InventoryThumbnailUpload","https://caps.example.test/thumb");
    caps.emplace("FetchInventoryDescendents2","https://caps.example.test/folders");
    if(firestorm_caps_availability(caps).advertised()!=5U)return 2;
    const auto owned=parse_agent_experiences(LlsdValue::map({
        {"experience_ids",LlsdValue::array({LlsdValue::uuid(A)})},
        {"purchase",LlsdValue::integer(0)}
    }));
    if(!owned||owned->ids.size()!=1U||!owned->purchase_price ||
       *owned->purchase_price!=0)return 3;
    const auto owned_no_purchase=parse_agent_experiences(LlsdValue::map({
        {"experience_ids",LlsdValue::array({})}
    }));
    if(!owned_no_purchase||owned_no_purchase->purchase_price)return 4;
    if(parse_agent_experiences(LlsdValue::map({
        {"experience_ids",LlsdValue::array({LlsdValue::string("invalid")})}
    }))||parse_agent_experiences(LlsdValue::map({
        {"experience_ids",LlsdValue::array({})},
        {"purchase",LlsdValue::integer(-1)}
    })))return 5;
    const auto resident=parse_resident_experiences(LlsdValue::map({
        {"experiences",LlsdValue::array({LlsdValue::uuid(A)})},
        {"blocked",LlsdValue::array({LlsdValue::uuid(B)})}
    }));
    if(!resident||resident->allowed.size()!=1U||
       resident->blocked.size()!=1U)return 6;
    if(parse_resident_experiences(LlsdValue::map({
        {"experiences",LlsdValue::array({})}
    })))return 7;
    auto oversize=std::vector<LlsdValue>(1025U,LlsdValue::uuid(A));
    if(parse_agent_experiences(LlsdValue::map({
        {"experience_ids",LlsdValue::array(std::move(oversize))}
    })))return 8;
    const auto llsd=parse_llsd_xml(serialize_llsd_xml(LlsdValue::map({
        {"experience_ids",LlsdValue::array({LlsdValue::uuid(A)})},
        {"purchase",LlsdValue::integer(0)}
    })));
    if(!llsd.ok()||!parse_agent_experiences(*llsd.value))return 9;
    constexpr auto thumb="33333333-3333-3333-3333-333333333333";
    const auto begin_item=build_thumbnail_begin_request(
        ThumbnailTarget::item,thumb);
    const auto begin_folder=build_thumbnail_begin_request(
        ThumbnailTarget::folder,thumb);
    if(!begin_item||!begin_folder)return 10;
    const auto item_llsd=parse_llsd_xml(*begin_item);
    const auto folder_llsd=parse_llsd_xml(*begin_folder);
    if(!item_llsd.ok()||!folder_llsd.ok()||
       !item_llsd.value->find("item_id")||
       item_llsd.value->find("category_id")||
       !folder_llsd.value->find("category_id")||
       folder_llsd.value->find("item_id")||
       build_thumbnail_begin_request(
           ThumbnailTarget::item,"bad-uuid")||
       build_thumbnail_begin_request(
           ThumbnailTarget::folder,
           "00000000-0000-0000-0000-000000000000"))
        return 11;
    const auto upload=parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("upload")},
        {"uploader",LlsdValue::string(
            "https://caps.example.test/private-one-time-token")}
    }));
    if(!upload ||upload->state!=ThumbnailUploadStage::State::upload ||
       upload->uploader.empty()||!upload->new_asset.empty())
        return 12;
    const auto complete=parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("complete")},
        {"new_asset",LlsdValue::uuid(thumb)}
    }));
    if(!complete||complete->state!=ThumbnailUploadStage::State::complete ||
       complete->new_asset!=thumb||!complete->uploader.empty())
        return 13;
    if(parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("upload")},
        {"uploader",LlsdValue::string("file:///tmp/escape")}
    }))||parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("complete")},
        {"new_asset",LlsdValue::string("invalid")}
    }))||parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("unexpected")}
    }))||parse_thumbnail_stage(LlsdValue::map({
        {"state",LlsdValue::string("complete")},
        {"new_asset",LlsdValue::uuid(
            "00000000-0000-0000-0000-000000000000")}
    })))return 14;
    std::cout<<"Firestorm inventory+Experiences advertisement and bounded LLSD reader OK\n";
    return 0;
}
