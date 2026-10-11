/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
namespace nexverse::protocol
{
enum class OutfitWearReadiness
{
    no_folder, no_inventory, empty_outfit, missing_cof, unresolved_links,
    incomplete_appearance, ready_for_implementation
};
struct OutfitWearPreflight
{
    OutfitWearReadiness readiness=OutfitWearReadiness::no_folder;
    std::size_t inventory_entries=0U;
    std::size_t link_entries=0U;
    std::size_t unresolved_links=0U;
    // NOTE: ready_for_implementation explicitly does NOT permit wearing.
    [[nodiscard]] bool can_wear() const noexcept {return false;}
};
[[nodiscard]] OutfitWearPreflight check_firestorm_outfit_preflight(
    std::string_view outfit_id,std::string_view current_outfit_id,
    const std::vector<OutfitInventoryItem>* outfit_items,
    const std::vector<OutfitInventoryItem>* cof_items,
    const std::vector<OutfitInventoryItem>& resolved_items);
}
