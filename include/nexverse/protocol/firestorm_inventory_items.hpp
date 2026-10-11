/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"

namespace nexverse::protocol
{
struct InventoryItemFetch
{
    bool valid=false;
    std::vector<OutfitInventoryItem> items;
};
[[nodiscard]] std::optional<std::string> build_fetch_inventory2_request(
    std::string_view authenticated_owner,
    const std::vector<std::string>& item_ids);
[[nodiscard]] InventoryItemFetch parse_fetch_inventory2(
    const LlsdValue& response,std::string_view authenticated_owner,
    const std::vector<std::string>& item_ids);
[[nodiscard]] InventoryItemFetch fetch_inventory2(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view capability_uri,std::string_view authenticated_owner,
    const std::vector<std::string>& item_ids,
    std::uint64_t issued_epoch,const FirestormRegionLease& lease);
}
