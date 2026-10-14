/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_inventory_items.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <chrono>
#include <set>
#include <limits>

namespace nexverse::protocol
{
std::optional<std::string> build_fetch_inventory2_request(
    std::string_view owner,const std::vector<std::string>& ids)
{
    if(!uuid_to_bytes(owner)||ids.empty()||ids.size()>32U)
        return std::nullopt;
    std::set<std::string> seen;
    std::vector<LlsdValue> list;
    for(const auto& id:ids)
    {
        if(!uuid_to_bytes(id)||!seen.insert(id).second)return std::nullopt;
        list.push_back(LlsdValue::map({
            {"item_id",LlsdValue::uuid(id)},
            {"owner_id",LlsdValue::uuid(std::string{owner})}
        }));
    }
    return serialize_llsd_xml(LlsdValue::map({
        {"items",LlsdValue::array(std::move(list))}
    }));
}
InventoryItemFetch parse_fetch_inventory2(
    const LlsdValue& response,std::string_view owner,
    const std::vector<std::string>& ids,std::string_view expected_parent)
{
    InventoryItemFetch output;
    if(!uuid_to_bytes(owner)||!response.is_map()||
       (!expected_parent.empty()&&!uuid_to_bytes(expected_parent)))
        return output;
    const auto* agent=response.find("agent_id");
    const auto* items=response.find("items");
    if(!agent||uuid_to_bytes(agent->as_string())!=uuid_to_bytes(owner)||
       !items||!items->is_array()||items->array_value.size()>32U)
        return output;
    const std::set<std::string> requested(ids.begin(),ids.end());
    std::set<std::string> seen;
    for(const auto& item:items->array_value)
    {
        if(!item.is_map())continue;
        const auto* id=item.find("item_id");
        const auto* name=item.find("name");
        const auto* type=item.find("type");
        const auto* inv=item.find("inv_type");
        const auto* perm=item.find("permissions");
        if(!id||!uuid_to_bytes(id->as_string())||
           !requested.contains(std::string{id->as_string()})||
           seen.contains(std::string{id->as_string()}) ||
           !name||name->type!=LlsdType::string||
           name->string_value.empty()||name->string_value.size()>128U ||
           !type||type->type!=LlsdType::integer ||
           type->integer_value<std::numeric_limits<std::int32_t>::min()||
           type->integer_value>std::numeric_limits<std::int32_t>::max()||
           !inv||inv->type!=LlsdType::integer ||
           inv->integer_value<std::numeric_limits<std::int32_t>::min()||
           inv->integer_value>std::numeric_limits<std::int32_t>::max()||
           !perm||!perm->is_map())
            continue;
        const auto* item_owner=perm->find("owner_id");
        if(!item_owner||
           uuid_to_bytes(item_owner->as_string())!=uuid_to_bytes(owner))
            continue;
        if(!expected_parent.empty())
        {
            const auto* parent=item.find("parent_id");
            if(!parent||
               uuid_to_bytes(parent->as_string())!=
                   uuid_to_bytes(expected_parent))
                continue;
        }
        std::string clean{name->string_value};
        clean.erase(std::remove_if(clean.begin(),clean.end(),
            [](unsigned char ch){return ch<32U||ch==127U;}),
            clean.end());
        if(clean.empty())continue;
        seen.insert(std::string{id->as_string()});
        const auto* asset=item.find("asset_id");
        const auto* flags=item.find("flags");
        const std::optional<std::uint32_t> item_flags=
            flags&&flags->type==LlsdType::integer&&
            flags->integer_value>=0&&
            static_cast<std::uint64_t>(flags->integer_value)<=UINT32_MAX
            ?std::optional<std::uint32_t>{
                static_cast<std::uint32_t>(flags->integer_value)}
            :std::nullopt;
        output.items.push_back({std::string{id->as_string()},
            asset&&uuid_to_bytes(asset->as_string())
                ?std::string{asset->as_string()}:std::string{},
            std::move(clean),
            static_cast<std::int32_t>(type->integer_value),
            static_cast<std::int32_t>(inv->integer_value),
            item_flags});
    }
    output.valid=true;
    return output;
}
InventoryItemFetch fetch_inventory2(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view uri,std::string_view owner,
    const std::vector<std::string>& ids,
    std::uint64_t epoch,const FirestormRegionLease& lease,
    std::string_view expected_parent)
{
    if(!transport||!lease.matches(epoch)||!is_valid_http_uri(uri)||
       uri.size()>4096U||uri.find('#')!=std::string_view::npos||
       (!expected_parent.empty()&&!uuid_to_bytes(expected_parent)))
        return {};
    const auto body=build_fetch_inventory2_request(owner,ids);
    if(!body)return {};
    const auto response=transport->post(uri,"application/llsd+xml",
        *body,256U*1024U,std::chrono::seconds{4},
        std::chrono::seconds{12});
    if(!lease.matches(epoch)||!response.ok())return {};
    const auto parsed=parse_llsd_xml(response.body);
    if(!parsed.ok())return {};
    return parse_fetch_inventory2(*parsed.value,owner,ids,expected_parent);
}
}
