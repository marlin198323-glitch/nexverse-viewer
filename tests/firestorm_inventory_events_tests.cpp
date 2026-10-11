#include "nexverse/protocol/firestorm_inventory_events.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
using namespace nexverse::protocol;
constexpr auto OWNER="11111111-1111-1111-1111-111111111111";
constexpr auto OTHER="22222222-2222-2222-2222-222222222222";
constexpr auto FOLDER="33333333-3333-3333-3333-333333333333";
constexpr auto PARENT="44444444-4444-4444-4444-444444444444";
constexpr auto ITEM="55555555-5555-5555-5555-555555555555";
constexpr auto ZERO="00000000-0000-0000-0000-000000000000";

EventQueueEvent inventory_event(std::string_view owner=OWNER,
                                std::string_view item_owner=OWNER,
                                std::string_view folder=FOLDER)
{
    return {"BulkUpdateInventory",LlsdValue::map({
        {"AgentData",LlsdValue::array({LlsdValue::map({
            {"AgentID",LlsdValue::uuid(std::string{owner})},
            {"TransactionID",LlsdValue::uuid(ITEM)}
        })})},
        {"FolderData",LlsdValue::array({LlsdValue::map({
            {"FolderID",LlsdValue::uuid(ZERO)},
            {"ParentID",LlsdValue::uuid(ZERO)}
        })})},
        {"ItemData",LlsdValue::array({LlsdValue::map({
            {"ItemID",LlsdValue::uuid(ITEM)},
            {"FolderID",LlsdValue::uuid(std::string{folder})},
            {"OwnerID",LlsdValue::uuid(std::string{item_owner})}
        })})}
    })};
}
int main()
{
    auto ordinary=inventory_event();
    auto parsed=parse_bulk_inventory_dirty(ordinary,OWNER);
    if(!parsed ||parsed->size()!=1U||!parsed->contains(FOLDER))return 1;
    if(parse_bulk_inventory_dirty(ordinary,OTHER))return 2;
    if(parse_bulk_inventory_dirty(inventory_event(OTHER),OWNER))return 3;
    if(!parse_bulk_inventory_dirty(inventory_event(OWNER,OTHER),OWNER)
            ->empty())return 4;
    if(!parse_bulk_inventory_dirty(inventory_event(OWNER,OWNER,ZERO),OWNER)
            ->empty())return 5;
    auto folders=inventory_event(OWNER,OTHER);
    auto* dummy=&folders.body.map_value.at("FolderData");
    *dummy=LlsdValue::array({LlsdValue::map({
        {"FolderID",LlsdValue::uuid(FOLDER)},
        {"ParentID",LlsdValue::uuid(PARENT)}
    })});
    auto dirty=parse_bulk_inventory_dirty(folders,OWNER);
    if(!dirty||dirty->size()!=2U||!dirty->contains(FOLDER)||
       !dirty->contains(PARENT))return 6;
    auto malformed=inventory_event();
    malformed.body.map_value.erase("AgentData");
    if(parse_bulk_inventory_dirty(malformed,OWNER))return 7;
    malformed=inventory_event();
    malformed.body.map_value["ItemData"]=LlsdValue::array(
        std::vector<LlsdValue>(513U,LlsdValue::map()));
    if(parse_bulk_inventory_dirty(malformed,OWNER))return 8;
    FirestormRegionLease lease;
    const auto epoch=lease.enter(4U,1U,"https://example.test/cap1");
    InventoryEventInbox inbox;
    if(!inbox.enqueue(ordinary,OWNER,epoch,lease))return 9;
    if(!inbox.enqueue(folders,OWNER,epoch,lease))return 10;
    auto batch=inbox.drain(epoch,lease);
    if(batch.epoch!=epoch||batch.full_refresh||
       batch.folders.size()!=2U||!batch.folders.contains(PARENT))
        return 11;
    if(!inbox.drain(epoch,lease).empty())return 12;
    if(inbox.enqueue(inventory_event(OTHER),OWNER,epoch,lease))return 13;
    if(!inbox.drain(epoch,lease).empty())return 14;
    if(!inbox.enqueue(ordinary,OWNER,epoch,lease))return 15;
    const auto next=lease.enter(5U,1U,"https://example.test/cap2");
    if(!inbox.drain(next,lease).empty()||
       inbox.enqueue(ordinary,OWNER,epoch,lease))return 16;
    if(!inbox.enqueue(ordinary,OWNER,next,lease))return 17;
    inbox.reset();
    if(!inbox.drain(next,lease).empty())return 18;
    // More than 256 valid distinct folders must trigger a bounded
    // full read-only resync rather than unbounded retention.
    auto many=inventory_event();
    std::vector<LlsdValue> items;
    for(unsigned i=1;i<=300U;++i)
    {
        char uuid[37];
        std::snprintf(uuid,sizeof uuid,"33333333-3333-3333-3333-%012x",i);
        items.push_back(LlsdValue::map({
            {"ItemID",LlsdValue::uuid(ITEM)},
            {"FolderID",LlsdValue::uuid(uuid)},
            {"OwnerID",LlsdValue::uuid(OWNER)}
        }));
    }
    many.body.map_value["ItemData"]=LlsdValue::array(std::move(items));
    if(!inbox.enqueue(many,OWNER,next,lease))return 19;
    const auto overflow=inbox.drain(next,lease);
    if(!overflow.full_refresh||!overflow.folders.empty())return 20;
    std::cout<<"Firestorm OpenSim CAPS BulkUpdateInventory owner,"
                " folder and epoch invalidation passed\\n";
    return 0;
}
