/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/assets/firestorm_wearable_asset.hpp"
#include <algorithm>
#include <cstdint>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
// RAM-only parsed ORIGINAL wearable cache. An asset UUID alone is never
// sufficient to authorize reuse; every hit must match the *current* owner,
// authenticated session and independently verified inventory item / flags.
// No skin, credentials, wearable bytes or inventory IDs are written to disk.
class WearableOriginalSessionCache
{
public:
    static constexpr std::size_t max_entries=24U;
    void clear() noexcept {entries_.clear();}
    [[nodiscard]] std::size_t size() const noexcept {return entries_.size();}
    [[nodiscard]] std::shared_ptr<const assets::FirestormWearableAsset>
    find(std::string_view owner,std::string_view session,
         const OutfitInventoryItem& item) const
    {
        if(!valid(owner,session,item))return {};
        for(const auto& entry:entries_)
            if(entry.owner==owner&&entry.session==session&&
               entry.item_id==item.item_id&&
               entry.asset_id==item.asset_id&&
               entry.asset_type==item.asset_type&&
               entry.inventory_type==item.inventory_type&&
               entry.flags==*item.flags&&
               entry.original&&
               entry.original->type==static_cast<int>(*item.flags&0xffU))
                return entry.original;
        return {};
    }
    [[nodiscard]] bool insert(std::string_view owner,std::string_view session,
         const OutfitInventoryItem& item,
         const assets::FirestormWearableAsset& original)
    {
        if(!valid(owner,session,item)||
           original.type!=static_cast<int>(*item.flags&0xffU)||
           (original.type<4)!=(item.asset_type==13)||
           original.visual_params.size()>assets::max_firestorm_visual_params||
           original.textures.size()>assets::max_firestorm_local_textures)
            return false;
        // One owner/session/item may change to a different asset/type;
        // never retain its previously authenticated appearance in RAM.
        std::erase_if(entries_,[&](const Entry& e){
            return e.owner==owner&&e.session==session&&
                   e.item_id==item.item_id;
        });
        if(entries_.size()>=max_entries)entries_.pop_front();
        entries_.push_back({
            std::string{owner},std::string{session},item.item_id,
            item.asset_id,item.asset_type,item.inventory_type,*item.flags,
            std::make_shared<const assets::FirestormWearableAsset>(original)});
        return true;
    }
private:
    struct Entry
    {
        std::string owner,session,item_id,asset_id;
        std::int32_t asset_type=0,inventory_type=0;
        std::uint32_t flags=0U;
        std::shared_ptr<const assets::FirestormWearableAsset> original;
    };
    [[nodiscard]] static bool valid(
        std::string_view owner,std::string_view session,
        const OutfitInventoryItem& item)
    {
        const auto uuid_valid=[](std::string_view x){
            const auto bytes=uuid_to_bytes(x);
            return bytes&&std::any_of(bytes->begin(),bytes->end(),
                [](std::uint8_t b){return b!=0U;});
        };
        return uuid_valid(owner)&&uuid_valid(session)&&
            uuid_valid(item.item_id)&&uuid_valid(item.asset_id)&&
            item.inventory_type==18&&item.flags&&
            (item.asset_type==5||item.asset_type==13)&&
            (*item.flags&0xffU)<17U&&
            ((item.asset_type==13)==((*item.flags&0xffU)<4U));
    }
    std::deque<Entry> entries_;
};
}
