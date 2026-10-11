/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_inventory_events.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

namespace nexverse::protocol
{
namespace
{
bool same_uuid(std::string_view a,std::string_view b)
{
    const auto left=uuid_to_bytes(a);
    const auto right=uuid_to_bytes(b);
    return left &&right&&*left==*right;
}
bool nonzero_uuid(std::string_view id)
{
    const auto raw=uuid_to_bytes(id);
    if(!raw)return false;
    for(const auto byte:*raw)if(byte!=0U)return true;
    return false;
}
}
std::optional<std::set<std::string,std::less<>>>
parse_bulk_inventory_dirty(const EventQueueEvent& event,
                           std::string_view authenticated_owner)
{
    if(event.message!="BulkUpdateInventory"||
       !nonzero_uuid(authenticated_owner)||!event.body.is_map())
        return std::nullopt;
    const auto* agent=event.body.find("AgentData");
    if(!agent||!agent->is_array()||agent->array_value.size()!=1U||
       !agent->array_value.front().is_map())
        return std::nullopt;
    const auto* agent_id=agent->array_value.front().find("AgentID");
    if(!agent_id||!same_uuid(agent_id->as_string(),authenticated_owner))
        return std::nullopt;
    const auto* items=event.body.find("ItemData");
    const auto* categories=event.body.find("FolderData");
    if((!items&&!categories)||
       (items&&(!items->is_array()||items->array_value.size()>512U))||
       (categories&&(!categories->is_array()||
                    categories->array_value.size()>512U)))
        return std::nullopt;
    std::set<std::string,std::less<>> dirty;
    if(items)
    {
        for(const auto& record:items->array_value)
        {
            if(!record.is_map())continue;
            const auto* item_id=record.find("ItemID");
            const auto* owner=record.find("OwnerID");
            const auto* folder=record.find("FolderID");
            if(item_id&&owner&&folder&&nonzero_uuid(item_id->as_string())&&
               nonzero_uuid(folder->as_string())&&
               same_uuid(owner->as_string(),authenticated_owner))
                dirty.insert(std::string{folder->as_string()});
        }
    }
    if(categories)
    {
        for(const auto& record:categories->array_value)
        {
            if(!record.is_map())continue;
            const auto* folder=record.find("FolderID");
            const auto* parent=record.find("ParentID");
            // OpenSimulator emits a synthetic all-zero FolderData record
            // for item-only updates. It is not an inventory category.
            if(!folder||!nonzero_uuid(folder->as_string()))continue;
            dirty.insert(std::string{folder->as_string()});
            if(parent&&nonzero_uuid(parent->as_string()))
                dirty.insert(std::string{parent->as_string()});
        }
    }
    return dirty;
}
bool InventoryEventInbox::enqueue(
    const EventQueueEvent& event,std::string_view owner,
    std::uint64_t issued_epoch,const FirestormRegionLease& lease)
{
    if(!lease.matches(issued_epoch))return false;
    auto parsed=parse_bulk_inventory_dirty(event,owner);
    if(!parsed||parsed->empty()||!lease.matches(issued_epoch))
        return false;
    std::scoped_lock lock{mutex_};
    if(!lease.matches(issued_epoch))return false;
    if(pending_.epoch!=issued_epoch)
        pending_={issued_epoch,false,{}};
    for(const auto& id:*parsed)
    {
        if(pending_.full_refresh)break;
        pending_.folders.insert(id);
        if(pending_.folders.size()>256U)
        {
            pending_.full_refresh=true;
            pending_.folders.clear();
        }
    }
    return true;
}
InventoryDirtyBatch InventoryEventInbox::drain(
    std::uint64_t current_epoch,const FirestormRegionLease& lease)
{
    std::scoped_lock lock{mutex_};
    if(!lease.matches(current_epoch)||pending_.epoch!=current_epoch)
    {
        pending_={};
        return {};
    }
    InventoryDirtyBatch out=std::move(pending_);
    pending_={};
    return out;
}
void InventoryEventInbox::reset()
{
    std::scoped_lock lock{mutex_};
    pending_={};
}
}
