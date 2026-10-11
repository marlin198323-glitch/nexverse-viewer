/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/firestorm_inventory_gallery.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include "nexverse/net/http_transport.hpp"
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
// OpenSim FetchInventoryDescendents2 POST. All IDs come from an authenticated
// login session; caps and actual access decisions remain server-authoritative.
struct OutfitInventoryItem
{
    std::string item_id;
    std::string asset_id;
    std::string name;
    std::int32_t asset_type=0;
    std::int32_t inventory_type=0;
};
struct OutfitInventoryRead
{
    std::map<std::string,InventoryThumbnailMetadata,std::less<>> thumbnails;
    std::map<std::string,std::uint64_t,std::less<>> versions;
    std::map<std::string,std::vector<OutfitInventoryItem>,std::less<>> items;
    std::size_t returned_folders=0U;
    bool valid=false;
};
[[nodiscard]] std::optional<std::string> build_outfit_descendents_request(
    std::string_view owner_id,
    const std::vector<OutfitGalleryFolder>& outfits,
    std::string_view current_outfit_folder={});
[[nodiscard]] OutfitInventoryRead parse_outfit_descendents(
    const LlsdValue& response,
    const std::vector<OutfitGalleryFolder>& requested,
    std::string_view current_outfit_folder={},
    std::string_view authenticated_owner={});
[[nodiscard]] OutfitInventoryRead fetch_outfit_descendents(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view capability_uri,std::string_view owner_id,
    const std::vector<OutfitGalleryFolder>& outfits,
    std::uint64_t issued_epoch,const FirestormRegionLease& lease,
    std::string_view current_outfit_folder={});
}
