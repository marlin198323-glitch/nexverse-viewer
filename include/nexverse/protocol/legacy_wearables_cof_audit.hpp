/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

#include <algorithm>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
// Low382 has NO COF version. Matching IDs is a read-only comparison, not
// authoritative current outfit, complete worn layers or simulator baked skin.
struct LegacyWearableCofAudit
{
    bool legacy_received=false;
    bool cof_originals_available=false;
    std::size_t legacy_items=0U;
    std::size_t cof_wearables=0U;
    std::size_t exact_item_asset_type=0U;
    std::size_t mismatched_asset_or_type=0U;
    std::size_t not_in_cof=0U;
    std::size_t cof_not_in_legacy=0U;
    std::size_t duplicate_legacy_items=0U;
    std::size_t unresolved_cof_links=0U;
    [[nodiscard]] bool can_prove_worn() const noexcept {return false;}
    [[nodiscard]] std::string label() const
    {
        if(!legacy_received)
            return "Legacy/COF: Simulator-Wearables ausstehend";
        if(!cof_originals_available)
            return "Legacy/COF: Originalinventar nicht verifiziert";
        // Keep the explicit non-proof notice inside the actual 99-char
        // F9 floater clipping budget, even with malformed 255-entry data.
        return "Legacy/COF: "+std::to_string(exact_item_asset_type)+
            "/"+std::to_string(cof_wearables)+
            " passend | A:"+std::to_string(mismatched_asset_or_type)+
            " F:"+std::to_string(cof_not_in_legacy)+
            " Z:"+std::to_string(not_in_cof)+
            " D:"+std::to_string(duplicate_legacy_items)+
            " O:"+std::to_string(unresolved_cof_links)+
            " | kein Wear-Beweis";
    }
};
[[nodiscard]] inline LegacyWearableCofAudit compare_legacy_wearables_cof(
    const AgentWearablesUpdateMessage* legacy,
    const std::vector<OutfitInventoryItem>* cof_links,
    const std::vector<OutfitInventoryItem>* fetched_originals)
{
    LegacyWearableCofAudit result;
    if(!legacy)return result;
    result.legacy_received=true;
    result.legacy_items=legacy->wearables.size();
    if(!cof_links||!fetched_originals)return result;
    result.cof_originals_available=true;
    std::map<std::string,const OutfitInventoryItem*,std::less<>> originals;
    for(const auto& item:*fetched_originals)
        if(!item.item_id.empty())
            originals.emplace(item.item_id,&item);
    std::map<std::string,const OutfitInventoryItem*,std::less<>> expected;
    for(const auto& link:*cof_links)
    {
        if(link.asset_type!=24)continue; // COF symlink
        const auto lookup=originals.find(link.asset_id);
        if(lookup==originals.end())
        {
            ++result.unresolved_cof_links;
            continue;
        }
        const auto& item=*lookup->second;
        if(item.asset_type!=5&&item.asset_type!=13)continue; // attachments etc
        if(item.inventory_type!=18||!item.flags||
           (*item.flags&0xffU)>16U)
        {
            ++result.unresolved_cof_links;
            continue;
        }
        if(!expected.emplace(item.item_id,&item).second)
            ++result.unresolved_cof_links;
    }
    result.cof_wearables=expected.size();
    std::set<std::string,std::less<>> seen;
    std::set<std::string,std::less<>> matched;
    constexpr std::string_view zero="00000000-0000-0000-0000-000000000000";
    for(const auto& entry:legacy->wearables)
    {
        // Empty original SL slot placeholders do not count as worn items.
        if(entry.item_id==zero||entry.item_id.empty())continue;
        if(!seen.insert(entry.item_id).second)
        {
            ++result.duplicate_legacy_items;
            continue;
        }
        const auto found=expected.find(entry.item_id);
        if(found==expected.end())
        {
            ++result.not_in_cof;
            continue;
        }
        const auto& original=*found->second;
        if(original.asset_id!=entry.asset_id||
           ((*original.flags&0xffU)!=entry.wearable_type))
        {
            ++result.mismatched_asset_or_type;
            continue;
        }
        matched.insert(entry.item_id);
        ++result.exact_item_asset_type;
    }
    result.cof_not_in_legacy=result.cof_wearables-matched.size();
    // A differing COF source may be stale without proving that clothing
    // is missing from the live avatar. These counts are read-only evidence.
    return result;
}
}
