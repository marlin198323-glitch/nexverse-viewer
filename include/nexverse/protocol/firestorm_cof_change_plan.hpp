/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::protocol
{
enum class CofPlanReadiness
{
    no_selection,missing_inventory,missing_cof,
    unsupported_folder_link,invalid_item,unresolved_target,
    duplicate_target,too_many_changes,preview_ready
};
struct CofLinkAddition
{
    std::string target_id;
    std::string name;
    std::int32_t inventory_type=0;
};
struct CofLinkRemoval
{
    std::string link_id;
    std::string target_id;
};
struct CofChangePlan
{
    CofPlanReadiness readiness=CofPlanReadiness::no_selection;
    std::vector<CofLinkAddition> add;
    std::vector<CofLinkRemoval> remove;
    std::uint64_t cof_version=0U;
    std::size_t unchanged=0U;
    std::set<std::string> desired_targets;
    // There is no atomic "wear outfit" transaction in these UDP calls.
    // A plan is only a preview, NEVER an execution authorization.
    [[nodiscard]] bool executable() const noexcept {return false;}
    [[nodiscard]] bool preview_ready() const noexcept
    {return readiness==CofPlanReadiness::preview_ready;}
};
[[nodiscard]] inline CofChangePlan plan_firestorm_cof_change(
    std::string_view selected_folder,std::string_view cof_folder,
    const std::vector<OutfitInventoryItem>* selected,
    const std::vector<OutfitInventoryItem>* cof,
    const std::vector<OutfitInventoryItem>& resolved,
    std::uint64_t verified_cof_version)
{
    CofChangePlan plan;
    const auto valid_id=[](std::string_view id) {
        const auto uuid=uuid_to_bytes(id);
        return uuid&&std::any_of(uuid->begin(),uuid->end(),
            [](std::uint8_t b){return b!=0U;});
    };
    if(!valid_id(selected_folder)||selected_folder==cof_folder)
        return plan;
    if(!selected) {plan.readiness=CofPlanReadiness::missing_inventory;return plan;}
    if(!valid_id(cof_folder)||!cof||verified_cof_version==0U)
    {plan.readiness=CofPlanReadiness::missing_cof;return plan;}
    if(selected->empty())
    {plan.readiness=CofPlanReadiness::missing_inventory;return plan;}
    plan.cof_version=verified_cof_version;
    std::set<std::string> resolved_ids;
    for(const auto& item:resolved)
        if(valid_id(item.item_id))resolved_ids.insert(item.item_id);
    std::map<std::string,CofLinkAddition,std::less<>> targets;
    for(const auto& item:*selected)
    {
        if(item.asset_type==25)
        {plan.readiness=CofPlanReadiness::unsupported_folder_link;return plan;}
        if(!valid_id(item.item_id)||item.name.empty()||
           item.name.size()>120U)
        {plan.readiness=CofPlanReadiness::invalid_item;return plan;}
        std::string target;
        if(item.asset_type==24)
        {
            if(!valid_id(item.asset_id)||
               !resolved_ids.contains(item.asset_id))
            {plan.readiness=CofPlanReadiness::unresolved_target;return plan;}
            target=item.asset_id;
        }
        else if(item.asset_type==5||item.asset_type==6||
                item.asset_type==13||item.asset_type==20)
            target=item.item_id;
        else
        {plan.readiness=CofPlanReadiness::invalid_item;return plan;}
        if(!targets.emplace(target,CofLinkAddition{
                 target,item.name,item.inventory_type}).second)
        {plan.readiness=CofPlanReadiness::duplicate_target;return plan;}
    }
    std::set<std::string> existing_ids;
    for(const auto& item:*cof)
    {
        if(item.asset_type!=24||!valid_id(item.item_id)||
           !valid_id(item.asset_id)||
           !existing_ids.insert(item.item_id).second)
        {plan.readiness=CofPlanReadiness::invalid_item;return plan;}
        if(targets.erase(item.asset_id)!=0U)++plan.unchanged;
        else plan.remove.push_back({item.item_id,item.asset_id});
    }
    for(const auto& [_,operation]:targets)plan.add.push_back(operation);
    // Immutable desired link targets for a future server-authoritative
    // reconciliation step; no source UUIDs are modified by the preview.
    for(const auto& item:*selected)
        plan.desired_targets.insert(item.asset_type==24
            ?item.asset_id:item.item_id);
    if(plan.add.size()+plan.remove.size()>32U)
    {plan.add.clear();plan.remove.clear();
     plan.readiness=CofPlanReadiness::too_many_changes;return plan;}
    plan.readiness=CofPlanReadiness::preview_ready;
    return plan;
}
enum class CofReconcileState
{
    no_plan,stale_region,stale_snapshot,invalid_server_items,
    different_targets,confirmed
};
// Only new, independently fetched owner-scoped COF inventory can
// establish confirmation; never trust LLUDP ACK or local optimistic state.
[[nodiscard]] inline CofReconcileState reconcile_cof_readback(
    const CofChangePlan& plan,
    std::uint64_t plan_region_epoch,std::uint64_t live_region_epoch,
    std::uint64_t server_cof_version,
    const std::vector<OutfitInventoryItem>* server_items)
{
    if(!plan.preview_ready())return CofReconcileState::no_plan;
    if(plan_region_epoch==0U||plan_region_epoch!=live_region_epoch)
        return CofReconcileState::stale_region;
    if(!server_items||server_cof_version<plan.cof_version+
        (plan.add.empty()&&plan.remove.empty()?0U:1U))
        return CofReconcileState::stale_snapshot;
    std::set<std::string> observed;
    for(const auto& link:*server_items)
    {
        if(link.asset_type!=24||!uuid_to_bytes(link.item_id)||
           !uuid_to_bytes(link.asset_id)||
           !observed.insert(link.asset_id).second)
            return CofReconcileState::invalid_server_items;
    }
    return observed==plan.desired_targets
        ?CofReconcileState::confirmed
        :CofReconcileState::different_targets;
}
[[nodiscard]] inline const char* cof_plan_label(CofPlanReadiness r)
{
    switch(r)
    {
    case CofPlanReadiness::no_selection:return "Kein Outfit ausgewaehlt";
    case CofPlanReadiness::missing_inventory:return "Outfit-Inhalt fehlt oder ist leer";
    case CofPlanReadiness::missing_cof:return "COF-Inhalt/Version nicht verifiziert";
    case CofPlanReadiness::unsupported_folder_link:return "Ordner-Links noch nicht aufloesbar";
    case CofPlanReadiness::invalid_item:return "Itemtyp/ID oder COF-Link ungueltig";
    case CofPlanReadiness::unresolved_target:return "Ziel der Outfit-Verknuepfung nicht aufgeloest";
    case CofPlanReadiness::duplicate_target:return "Outfit enthaelt doppelte Link-Ziele";
    case CofPlanReadiness::too_many_changes:return "Zu viele Mutationen (>32)";
    case CofPlanReadiness::preview_ready:return "COF-Aenderungsplan lesend berechnet";
    }
    return "Unbekannter COF-Zustand";
}
}
