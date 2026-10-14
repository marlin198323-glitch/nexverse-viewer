/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::protocol
{
// Firestorm_7.2.4 indra/llappearance/llwearabletype.h:
// InventoryFlagsToWearableType(flags) = flags & 0xFF.
// Do NOT infer wearable subtypes from translated inventory item names.
inline constexpr std::array<std::string_view,17> firestorm_wearable_names{
    "Shape","Skin","Hair","Eyes","Shirt","Pants","Shoes","Socks",
    "Jacket","Gloves","Undershirt","Underpants","Skirt",
    "Alpha","Tattoo","Physics","Universal"
};
enum class WearableReadiness
{
    no_cof,missing_target,missing_subtype,invalid_subtype,
    asset_mismatch,multiple_primary,multiwear_requires_bake,
    missing_bodypart,ready_for_appearance_pipeline
};
struct FirestormWearableSlots
{
    WearableReadiness readiness=WearableReadiness::no_cof;
    std::array<std::string,17> worn_items{};
    std::array<std::uint8_t,17> count{};
    std::size_t link_count=0U,wearable_count=0U,
        attachment_count=0U,unresolved_count=0U;
    // AgentIsNowWearing legacy packet can represent only one item per
    // slot. Even a complete snapshot cannot authorize rendering or an
    // AgentSetAppearance upload without actual wearable assets/bakes.
    [[nodiscard]] bool may_send_appearance() const noexcept {return false;}
};
[[nodiscard]] inline FirestormWearableSlots resolve_firestorm_wearables(
    const std::vector<OutfitInventoryItem>* cof,
    const std::vector<OutfitInventoryItem>& fetched_originals)
{
    FirestormWearableSlots result;
    if(!cof||cof->empty())return result;
    const auto good_id=[](std::string_view text) {
        const auto b=uuid_to_bytes(text);
        return b&&std::any_of(b->begin(),b->end(),
            [](std::uint8_t v){return v!=0U;});
    };
    std::map<std::string,const OutfitInventoryItem*,std::less<>> originals;
    for(const auto& item:fetched_originals)
        if(good_id(item.item_id))
            originals.emplace(item.item_id,&item);
    std::set<std::string> targets;
    for(const auto& link:*cof)
    {
        if(link.asset_type!=24||!good_id(link.item_id)||
           !good_id(link.asset_id)||
           !targets.insert(link.asset_id).second)
        {
            result.readiness=WearableReadiness::asset_mismatch;
            return result;
        }
        ++result.link_count;
        const auto it=originals.find(link.asset_id);
        if(it==originals.end())
        {
            ++result.unresolved_count;
            continue;
        }
        const auto& item=*it->second;
        if(item.asset_type==6&&item.inventory_type==6)
        {
            ++result.attachment_count;
            continue;
        }
        if(item.asset_type!=5&&item.asset_type!=13)
            continue; // Scripts, gestures and other COF link targets.
        if(item.inventory_type!=18)
        {
            result.readiness=WearableReadiness::asset_mismatch;
            return result;
        }
        if(!item.flags)
        {
            result.readiness=WearableReadiness::missing_subtype;
            return result;
        }
        const unsigned subtype=*item.flags&0xffU;
        if(subtype>=result.count.size())
        {
            result.readiness=WearableReadiness::invalid_subtype;
            return result;
        }
        if((subtype<4U)!=(item.asset_type==13))
        {
            result.readiness=WearableReadiness::asset_mismatch;
            return result;
        }
        if(result.count[subtype]==UINT8_MAX)
        {
            result.readiness=WearableReadiness::multiwear_requires_bake;
            return result;
        }
        ++result.count[subtype];
        ++result.wearable_count;
        if(result.count[subtype]>1U)
        {
            result.readiness=subtype<4U
                ?WearableReadiness::multiple_primary
                :WearableReadiness::multiwear_requires_bake;
            return result;
        }
        result.worn_items[subtype]=item.item_id;
    }
    if(result.unresolved_count)
    {
        result.readiness=WearableReadiness::missing_target;
        return result;
    }
    for(std::size_t i=0U;i<4U;++i)
        if(result.count[i]!=1U)
        {
            result.readiness=WearableReadiness::missing_bodypart;
            return result;
        }
    result.readiness=WearableReadiness::ready_for_appearance_pipeline;
    return result;
}
[[nodiscard]] inline const char* wearable_readiness_label(
    WearableReadiness state)
{
    switch(state)
    {
    case WearableReadiness::no_cof:return "Kein Current Outfit Folder geladen";
    case WearableReadiness::missing_target:return "COF-Linkziel nicht als Originalitem geladen";
    case WearableReadiness::missing_subtype:return "Wearable-Subtype-Flags fehlen";
    case WearableReadiness::invalid_subtype:return "Wearable-Subtype ausserhalb Firestorm 0-16";
    case WearableReadiness::asset_mismatch:return "Asset-/Inventory-Typ oder COF-Link ungueltig";
    case WearableReadiness::multiple_primary:return "Mehrere primaere Koerperteile";
    case WearableReadiness::multiwear_requires_bake:return "Multiwear braucht Layer/Bake-Synchronisierung";
    case WearableReadiness::missing_bodypart:return "Shape/Skin/Hair/Eyes unvollstaendig";
    case WearableReadiness::ready_for_appearance_pipeline:
        return "17 Wearable-Slots geprüft; Asset/Bake/Simulator-Wear noch offen";
    }
    return "Unbekannte Wearable-Bereitschaft";
}
}
