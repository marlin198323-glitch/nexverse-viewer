/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/firestorm_inventory_items.hpp"
#include "nexverse/protocol/lludp.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::protocol
{
// Low382 is simulator-authenticated, but *not* proof that the robust grid
// CurrentOutfitFolder is populated. Inventory2 can independently verify
// ORIGINAL item/asset/type/flags without modifying COF or avatar appearance.
struct LegacyWearableInventoryProbe
{
    std::vector<std::string> ids;
    std::vector<AgentWearableEntry> worn;
    bool eligible=false;
};
[[nodiscard]] inline LegacyWearableInventoryProbe plan_legacy_wearable_inventory_probe(
    const AgentWearablesUpdateMessage* report,
    std::string_view authenticated_agent,
    std::string_view authenticated_session)
{
    LegacyWearableInventoryProbe out;
    if(!report || !uuid_to_bytes(authenticated_agent) ||
       !uuid_to_bytes(authenticated_session) ||
       report->agent_id!=authenticated_agent ||
       report->session_id!=authenticated_session ||
       report->wearables.empty()||report->wearables.size()>17U)
        return out;
    std::set<std::string> ids;
    std::array<bool,17> types{};
    constexpr std::string_view zero="00000000-0000-0000-0000-000000000000";
    for(const auto& w:report->wearables)
    {
        if(w.wearable_type>=17U)return {};
        if(w.item_id.empty()||w.item_id==zero)
            continue;
        if(!uuid_to_bytes(w.item_id)||!uuid_to_bytes(w.asset_id)||
           w.asset_id==zero||types[w.wearable_type]||
           !ids.insert(w.item_id).second)
            return {};
        types[w.wearable_type]=true;
        out.ids.push_back(w.item_id);
        out.worn.push_back(w);
    }
    out.eligible=!out.ids.empty();
    return out;
}
struct LegacyWearableInventoryEvidence
{
    std::size_t requested=0U;
    std::size_t matched=0U;
    std::size_t missing=0U;
    std::size_t bodyparts=0U;
    bool valid=false;
    bool complete_primary=false;
};
[[nodiscard]] inline LegacyWearableInventoryEvidence
audit_legacy_wearable_inventory_probe(
    const LegacyWearableInventoryProbe& expected,
    const InventoryItemFetch& result)
{
    LegacyWearableInventoryEvidence out;
    out.requested=expected.ids.size();
    if(!expected.eligible||!result.valid)return out;
    out.valid=true;
    std::array<bool,4> body{};
    for(const auto& w:expected.worn)
    {
        const auto item=std::find_if(result.items.begin(),result.items.end(),
            [&](const OutfitInventoryItem& x){return x.item_id==w.item_id;});
        if(item==result.items.end()||item->asset_id!=w.asset_id||
           item->inventory_type!=18||!item->flags||
           (*item->flags&0xffU)!=w.wearable_type||
           item->asset_type!=(w.wearable_type<4U?13:5))
        {
            ++out.missing;
            continue;
        }
        ++out.matched;
        if(w.wearable_type<4U)body[w.wearable_type]=true;
    }
    out.bodyparts=std::count(body.begin(),body.end(),true);
    out.complete_primary=out.bodyparts==4U&&out.missing==0U;
    return out;
}
}
