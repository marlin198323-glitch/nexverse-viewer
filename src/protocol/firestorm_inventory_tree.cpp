/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_inventory_tree.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include <chrono>
#include <algorithm>
#include <set>
#include <limits>

namespace nexverse::protocol
{
namespace
{
std::optional<std::string> valid_title(std::string_view raw)
{
    if(raw.empty()||raw.size()>128U)return std::nullopt;
    std::string value{raw};
    value.erase(std::remove_if(value.begin(),value.end(),
        [](unsigned char c){return c<32U||c==127U;}),value.end());
    if(value.empty())return std::nullopt;
    return value;
}
}
InventoryTreeSnapshot inventory_tree_from_login(
    const std::vector<InventoryFolder>& skeleton)
{
    InventoryTreeSnapshot out;
    for(const auto& folder:skeleton)
    {
        const auto id=uuid_to_bytes(folder.folder_id);
        const auto parent=uuid_to_bytes(folder.parent_id);
        const auto name=valid_title(folder.name);
        if(!id||!name||(!parent&&!folder.parent_id.empty()))
            continue;
        out.folders.emplace(folder.folder_id,InventoryFolderNode{
            folder.folder_id,folder.parent_id,*name,
            folder.type_default,
            static_cast<std::uint64_t>(
                std::max<std::int32_t>(0,folder.version))});
        if(out.count()>=4096U)break;
    }
    return out;
}
InventoryFolderRead fetch_inventory_folder_children(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view cap,std::string_view owner,std::string_view folder,
    std::uint64_t epoch,const FirestormRegionLease& lease)
{
    InventoryFolderRead result;
    result.epoch=epoch;
    result.folder_id=std::string{folder};
    if(!transport||!lease.matches(epoch)||!uuid_to_bytes(owner)||
       !uuid_to_bytes(folder)||!is_valid_http_uri(cap)||
       cap.size()>4096U||cap.find('#')!=std::string_view::npos)
        return result;
    const auto request=serialize_llsd_xml(LlsdValue::map({
        {"folders",LlsdValue::array({LlsdValue::map({
            {"folder_id",LlsdValue::uuid(std::string{folder})},
            {"owner_id",LlsdValue::uuid(std::string{owner})},
            {"sort_order",LlsdValue::integer(0)},
            {"fetch_folders",LlsdValue::boolean(true)},
            {"fetch_items",LlsdValue::boolean(true)}
        })})}
    }));
    const auto response=transport->post(cap,"application/llsd+xml",request,
        1U*1024U*1024U,std::chrono::seconds{4},
        std::chrono::seconds{12});
    if(!lease.matches(epoch)||!response.ok())return result;
    const auto parsed=parse_llsd_xml(response.body);
    if(!parsed.ok()||!parsed.value->is_map())return result;
    const auto* parents=parsed.value->find("folders");
    if(!parents||!parents->is_array()||
       parents->array_value.size()>64U)return result;
    for(const auto& candidate:parents->array_value)
    {
        if(!candidate.is_map())continue;
        const auto* id=candidate.find("folder_id");
        const auto* owner_id=candidate.find("owner_id");
        const auto* version=candidate.find("version");
        const auto* items=candidate.find("items");
        if(!id||!owner_id||!version||!items||
           uuid_to_bytes(id->as_string())!=uuid_to_bytes(folder)||
           uuid_to_bytes(owner_id->as_string())!=uuid_to_bytes(owner)||
           version->type!=LlsdType::integer||version->integer_value<0||
           !items->is_array()||items->array_value.size()>512U)
            continue;
        std::set<std::string> ids;
        for(const auto& item:items->array_value)
        {
            if(!item.is_map()||result.items.size()>=256U)continue;
            const auto* iid=item.find("item_id");
            const auto* parent=item.find("parent_id");
            const auto* permissions=item.find("permissions");
            const auto* name=item.find("name");
            const auto* type=item.find("type");
            const auto* inventory_type=item.find("inv_type");
            if(!iid||!parent||!permissions||!permissions->is_map()||
               !name||name->type!=LlsdType::string||
               !type||type->type!=LlsdType::integer||
               type->integer_value<std::numeric_limits<std::int32_t>::min()||
               type->integer_value>std::numeric_limits<std::int32_t>::max()||
               !inventory_type||inventory_type->type!=LlsdType::integer||
               inventory_type->integer_value<std::numeric_limits<std::int32_t>::min()||
               inventory_type->integer_value>std::numeric_limits<std::int32_t>::max()||
               !uuid_to_bytes(iid->as_string())||
               uuid_to_bytes(parent->as_string())!=uuid_to_bytes(folder)||
               !ids.insert(std::string{iid->as_string()}).second)
                continue;
            const auto* item_owner=permissions->find("owner_id");
            if(!item_owner||
               uuid_to_bytes(item_owner->as_string())!=uuid_to_bytes(owner))
                continue;
            const auto title=valid_title(name->as_string());
            if(!title)continue;
            const auto* asset=item.find("asset_id");
            result.items.push_back(OutfitInventoryItem{
                std::string{iid->as_string()},
                asset&&uuid_to_bytes(asset->as_string())
                    ?std::string{asset->as_string()}:std::string{},
                *title,
                static_cast<std::int32_t>(type->integer_value),
                static_cast<std::int32_t>(inventory_type->integer_value)});
        }
        result.version=static_cast<std::uint64_t>(version->integer_value);
        result.valid=true;
        break;
    }
    if(!result.valid)return result;
    (void)apply_inventory_categories(result.tree,*parsed.value,
                                      owner,{std::string{folder}});
    return result;
}
bool merge_inventory_folder_read(
    InventoryTreeSnapshot& tree,
    std::map<std::string,InventoryFolderContents,std::less<>>& contents,
    const InventoryFolderRead& read)
{
    if(!read.valid || !uuid_to_bytes(read.folder_id))return false;
    if(const auto it=contents.find(read.folder_id);it!=contents.end() &&
       it->second.version>read.version)return false;
    for(const auto& [id,node]:read.tree.folders)
    {
        const auto it=tree.folders.find(id);
        if(it==tree.folders.end())
        {
            if(tree.count()>=4096U)continue;
            tree.folders.emplace(id,node);
        }
        else if(node.version>=it->second.version)
            it->second=node;
    }
    contents.insert_or_assign(read.folder_id,
        InventoryFolderContents{read.version,read.items});
    return true;
}
std::size_t merge_inventory_categories(
    InventoryTreeSnapshot& tree,const OutfitInventoryRead& result)
{
    if(!result.valid)return 0U;
    std::size_t changed=0U;
    for(const auto& [id,child]:result.categories)
    {
        const auto found=tree.folders.find(id);
        const auto next=static_cast<std::uint64_t>(
            std::max<std::int32_t>(0,child.version));
        if(found!=tree.folders.end()&&found->second.version>next)
            continue;
        if(found==tree.folders.end()&&tree.count()>=4096U)
            continue;
        if(found==tree.folders.end()||
           found->second.version!=next||
           found->second.name!=child.name||
           found->second.parent_id!=child.parent_id)
        {
            tree.folders.insert_or_assign(id,InventoryFolderNode{
                id,child.parent_id,child.name,child.type_default,next});
            ++changed;
        }
        if(tree.count()>=4096U)break;
    }
    return changed;
}
std::size_t apply_inventory_categories(
    InventoryTreeSnapshot& tree,const LlsdValue& response,
    std::string_view owner_id,const std::vector<std::string>& parents)
{
    if(!uuid_to_bytes(owner_id)||!response.is_map())return 0U;
    const auto* data=response.find("folders");
    if(!data||!data->is_array()||data->array_value.size()>64U)return 0U;
    std::set<std::string> allowed(parents.begin(),parents.end());
    std::size_t added=0;
    for(const auto& folder:data->array_value)
    {
        if(!folder.is_map())continue;
        const auto* id=folder.find("folder_id");
        const auto* owner=folder.find("owner_id");
        const auto* children=folder.find("categories");
        if(!id||!owner||!uuid_to_bytes(id->as_string())||
           uuid_to_bytes(owner->as_string())!=uuid_to_bytes(owner_id)||
           !allowed.contains(std::string{id->as_string()})||
           !children||!children->is_array()||
           children->array_value.size()>1024U)
            continue;
        for(const auto& child:children->array_value)
        {
            if(!child.is_map())continue;
            const auto* cid=child.find("category_id");
            const auto* parent=child.find("parent_id");
            const auto* name=child.find("name");
            const auto* type=child.find("type_default");
            const auto* version=child.find("version");
            if(!cid||!parent||!name||!type||!version||
               !uuid_to_bytes(cid->as_string())||
               parent->as_string()!=id->as_string()||
               type->type!=LlsdType::integer||
               version->type!=LlsdType::integer||
               version->integer_value<0||
               type->integer_value<std::numeric_limits<std::int32_t>::min()||
               type->integer_value>std::numeric_limits<std::int32_t>::max())
                continue;
            const auto title=valid_title(name->as_string());
            if(!title)continue;
            const auto key=std::string{cid->as_string()};
            if(tree.count()>=4096U&&!tree.folders.contains(key))
                continue;
            const auto v=static_cast<std::uint64_t>(version->integer_value);
            const auto found=tree.folders.find(key);
            if(found!=tree.folders.end()&&found->second.version>v)
                continue;
            tree.folders.insert_or_assign(key,InventoryFolderNode{
                key,std::string{id->as_string()},*title,
                static_cast<std::int32_t>(type->integer_value),v});
            ++added;
        }
    }
    return added;
}
}
