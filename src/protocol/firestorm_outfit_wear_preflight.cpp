/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_outfit_wear_preflight.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <set>
namespace nexverse::protocol
{
OutfitWearPreflight check_firestorm_outfit_preflight(
    std::string_view outfit,std::string_view cof,
    const std::vector<OutfitInventoryItem>* saved,
    const std::vector<OutfitInventoryItem>* current,
    const std::vector<OutfitInventoryItem>& resolved)
{
    OutfitWearPreflight result;
    if(!uuid_to_bytes(outfit))return result;
    if(!saved){result.readiness=OutfitWearReadiness::no_inventory;return result;}
    if(saved->empty()){result.readiness=OutfitWearReadiness::empty_outfit;return result;}
    if(!uuid_to_bytes(cof)||!current)
    {
        result.readiness=OutfitWearReadiness::missing_cof;
        return result;
    }
    std::set<std::string> known;
    for(const auto& item:resolved)
        if(uuid_to_bytes(item.item_id))known.insert(item.item_id);
    result.inventory_entries=saved->size();
    for(const auto& item:*saved)
    {
        // Firestorm links: LLAssetType::AT_LINK=24, target item in asset_id.
        // Folder links (AT_LINK_FOLDER=25) need a separate hierarchy resolver.
        if(item.asset_type==24 &&uuid_to_bytes(item.asset_id))
        {
            ++result.link_entries;
            if(!known.contains(item.asset_id))++result.unresolved_links;
        }
        else if(item.asset_type==25)
        {
            ++result.unresolved_links;
        }
        else if(item.asset_type!=5 &&item.asset_type!=6 &&
                item.asset_type!=13 &&item.asset_type!=20)
        {
            ++result.unresolved_links;
        }
    }
    if(result.unresolved_links!=0U)
        result.readiness=OutfitWearReadiness::unresolved_links;
    else
        // Even valid inventory IDs are not equivalent to fetched wearable
        // assets + COF changes + AvatarAppearance reconciliation.
        result.readiness=OutfitWearReadiness::incomplete_appearance;
    return result;
}
}
