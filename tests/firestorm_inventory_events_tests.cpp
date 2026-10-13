#include "nexverse/protocol/firestorm_inventory_events.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
using namespace nexverse::protocol;
constexpr auto OWNER="11111111-1111-1111-1111-111111111111";
constexpr auto SESSION="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
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

    // Firestorm Linden Unencoded, Low 270/276/284: exact AgentData
    // (agent/session UUID) and bounded InventoryData/FolderData counts.
    const auto removal=[&](MessageId type,std::string_view agent=OWNER,
                           std::string_view session=SESSION)
    {
        std::vector<std::uint8_t> payload;
        for(const auto id:{agent,session})
        {
            const auto raw=uuid_to_bytes(id);
            if(raw)payload.insert(payload.end(),raw->begin(),raw->end());
        }
        payload.push_back(1U);
        const auto record=uuid_to_bytes(ITEM);
        payload.insert(payload.end(),record->begin(),record->end());
        if(type==msg_remove_inventory_objects)
            payload.push_back(0U);
        const auto packet=encode_packet(0U,44U,type,payload);
        const auto decoded=decode_lludp_packet(packet);
        return decoded.ok()?*decoded.packet:DecodedPacket{};
    };
    for(const auto id:{msg_remove_inventory_item,
                       msg_remove_inventory_folder,
                       msg_remove_inventory_objects})
    {
        const auto expected=known_message_template(id);
        if(!expected || !parse_inventory_remove_hint(
                removal(id),OWNER,SESSION))return 21;
    }
    if(parse_inventory_remove_hint(removal(msg_remove_inventory_item,OTHER),
                                   OWNER,SESSION) ||
       parse_inventory_remove_hint(removal(msg_remove_inventory_item,OWNER,OTHER),
                                   OWNER,SESSION))
        return 22;
    auto truncated=removal(msg_remove_inventory_folder);
    truncated.payload.pop_back();
    if(parse_inventory_remove_hint(truncated,OWNER,SESSION))return 23;
    auto trailing=removal(msg_remove_inventory_item);
    trailing.payload.push_back(0x44U);
    if(parse_inventory_remove_hint(trailing,OWNER,SESSION))return 24;
    auto oversized=removal(msg_remove_inventory_item);
    oversized.payload[32U]=65U;
    if(parse_inventory_remove_hint(oversized,OWNER,SESSION))return 25;
    auto empty=removal(msg_remove_inventory_item);
    empty.payload[32U]=0U;
    empty.payload.resize(33U);
    if(parse_inventory_remove_hint(empty,OWNER,SESSION))return 26;
    auto zero=removal(msg_remove_inventory_item);
    std::fill(zero.payload.begin()+33,zero.payload.end(),0U);
    if(parse_inventory_remove_hint(zero,OWNER,SESSION))return 27;
    auto malformed_objects=removal(msg_remove_inventory_objects);
    malformed_objects.payload.pop_back(); // mandatory second variable count
    if(parse_inventory_remove_hint(malformed_objects,OWNER,SESSION))return 28;
    // Boundaries for 64 actual UUIDs, including a dual-block packet.
    // The server may send many removals but never unbounded data.
    auto boundary=removal(msg_remove_inventory_item);
    boundary.payload[32U]=64U;
    const auto one=uuid_to_bytes(ITEM);
    for(unsigned i=1U;i<64U;++i)
        boundary.payload.insert(boundary.payload.end(),
                                one->begin(),one->end());
    if(!parse_inventory_remove_hint(boundary,OWNER,SESSION))return 33;
    boundary.payload[32U]=65U;
    if(parse_inventory_remove_hint(boundary,OWNER,SESSION))return 34;
    auto two_blocks=removal(msg_remove_inventory_objects);
    two_blocks.payload[32U]=63U;
    two_blocks.payload.resize(33U);
    for(unsigned i=0U;i<63U;++i)
        two_blocks.payload.insert(two_blocks.payload.end(),
                                  one->begin(),one->end());
    two_blocks.payload.push_back(1U);
    two_blocks.payload.insert(two_blocks.payload.end(),
                              one->begin(),one->end());
    if(!parse_inventory_remove_hint(two_blocks,OWNER,SESSION))return 35;
    two_blocks.payload[two_blocks.payload.size()-17U]=2U;
    if(parse_inventory_remove_hint(two_blocks,OWNER,SESSION))return 36;
    auto unrelated=removal(msg_remove_inventory_item);
    unrelated.header.message={MessageFrequency::high,4U};
    if(parse_inventory_remove_hint(unrelated,OWNER,SESSION))return 37;
    // Full invalidation supersedes pending targeted folders, coalesces
    // repeated delete hints and never survives an epoch transition.
    const auto removal_epoch=lease.enter(7U,2U,"https://example.test/cap3");
    if(!inbox.enqueue(ordinary,OWNER,removal_epoch,lease))
        return 29;
    if(!inbox.enqueue_full_refresh(removal_epoch,lease) ||
       !inbox.enqueue_full_refresh(removal_epoch,lease))
        return 30;
    const auto full=inbox.drain(removal_epoch,lease);
    if(full.epoch!=removal_epoch || !full.full_refresh ||
       !full.folders.empty())return 31;
    const auto newer=lease.enter(8U,2U,"https://example.test/cap4");
    if(inbox.enqueue_full_refresh(removal_epoch,lease) ||
       !inbox.drain(newer,lease).empty())return 32;
    std::cout<<"Firestorm OpenSim CAPS BulkUpdateInventory owner,"
                " folder and epoch invalidation passed\\n";
    return 0;
}
