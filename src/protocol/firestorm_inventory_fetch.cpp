/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <chrono>
#include <set>

namespace nexverse::protocol
{
std::optional<std::string> build_outfit_descendents_request(
    std::string_view owner_id,
    const std::vector<OutfitGalleryFolder>& outfits)
{
    const auto owner=uuid_to_bytes(owner_id);
    if(!owner||outfits.empty()||outfits.size()>16U)
        return std::nullopt;
    std::vector<LlsdValue> folders;
    std::set<std::string> seen;
    const auto add=[&](std::string_view id)
    {
        const auto bytes=uuid_to_bytes(id);
        if(!bytes||std::all_of(bytes->begin(),bytes->end(),
            [](std::uint8_t byte){return byte==0U;}))return false;
        if(!seen.insert(std::string{id}).second)return true;
        folders.push_back(LlsdValue::map({
            {"folder_id",LlsdValue::uuid(std::string{id})},
            {"owner_id",LlsdValue::uuid(std::string{owner_id})},
            {"sort_order",LlsdValue::integer(0)},
            {"fetch_folders",LlsdValue::boolean(true)},
            {"fetch_items",LlsdValue::boolean(true)}
        }));
        return true;
    };
    // Parent folders expose the thumbnails of their outfit children.
    // Requesting only an outfit's own children would miss its asset_id.
    for(const auto& outfit:outfits)
        if(!add(outfit.parent_id)||!add(outfit.id))return std::nullopt;
    return serialize_llsd_xml(LlsdValue::map({
        {"folders",LlsdValue::array(std::move(folders))}
    }));
}
OutfitInventoryRead parse_outfit_descendents(
    const LlsdValue& response,
    const std::vector<OutfitGalleryFolder>& requested)
{
    OutfitInventoryRead result;
    if(!response.is_map())return result;
    const auto* folders=response.find("folders");
    if(!folders||!folders->is_array()||
       folders->array_value.size()>64U)return result;
    std::set<std::string> wanted;
    std::set<std::string> query_folders;
    for(const auto& item:requested)
    {
        wanted.insert(item.id);
        query_folders.insert(item.id);
        query_folders.insert(item.parent_id);
    }
    for(const auto& data:folders->array_value)
    {
        if(!data.is_map())continue;
        const auto* folder_id=data.find("folder_id");
        const auto* version=data.find("version");
        const auto* categories=data.find("categories");
        if(!folder_id||!uuid_to_bytes(folder_id->as_string())||
           !query_folders.contains(std::string{folder_id->as_string()})||
           !version||version->type!=LlsdType::integer||
           version->integer_value<0||
           !categories||!categories->is_array()||
           categories->array_value.size()>1024U)continue;
        ++result.returned_folders;
        result.versions.emplace(std::string{folder_id->as_string()},
                                static_cast<std::uint64_t>(version->integer_value));
        // The request returns category thumbnail metadata in child records.
        // Never trust a category from a different parent/owner.
        for(const auto& child:categories->array_value)
        {
            if(!child.is_map())continue;
            const auto* id=child.find("category_id");
            const auto* parent=child.find("parent_id");
            if(!id||!parent||!uuid_to_bytes(id->as_string())||
               parent->as_string()!=folder_id->as_string()||
               !wanted.contains(std::string{id->as_string()}))
                continue;
            const auto meta=parse_inventory_thumbnail_metadata(child);
            if(meta)
            {
                result.thumbnails.insert_or_assign(
                    std::string{id->as_string()},*meta);
                if(const auto* child_version=child.find("version");
                   child_version &&
                   child_version->type==LlsdType::integer &&
                   child_version->integer_value>=0)
                    result.versions.insert_or_assign(
                        std::string{id->as_string()},
                        static_cast<std::uint64_t>(
                            child_version->integer_value));
            }
        }
    }
    result.valid=true;
    return result;
}
OutfitInventoryRead fetch_outfit_descendents(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view capability_uri,std::string_view owner_id,
    const std::vector<OutfitGalleryFolder>& outfits,
    std::uint64_t issued_epoch,const FirestormRegionLease& lease)
{
    if(!transport||!lease.matches(issued_epoch))return {};
    const auto body=build_outfit_descendents_request(owner_id,outfits);
    if(!body)return {};
    if(capability_uri.size()>4096U ||capability_uri.find('#')!=std::string_view::npos ||
       !is_valid_http_uri(capability_uri))
        return {};
    const auto reply=transport->post(capability_uri,"application/llsd+xml",
        *body,2U*1024U*1024U,std::chrono::seconds{5},
        std::chrono::seconds{15});
    if(!lease.matches(issued_epoch)||!reply.ok())return {};
    const auto parsed=parse_llsd_xml(reply.body);
    if(!parsed.ok())return {};
    return parse_outfit_descendents(*parsed.value,outfits);
}
}
