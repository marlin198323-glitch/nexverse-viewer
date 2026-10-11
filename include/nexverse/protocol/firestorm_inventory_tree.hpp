/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include <map>

namespace nexverse::protocol
{
struct InventoryFolderNode
{
    std::string id;
    std::string parent_id;
    std::string name;
    std::int32_t type=0;
    std::uint64_t version=0;
};
struct InventoryFolderContents
{
    std::uint64_t version=0;
    std::vector<OutfitInventoryItem> items;
};
struct InventoryTreeSnapshot
{
    std::map<std::string,InventoryFolderNode,std::less<>> folders;
    [[nodiscard]] std::size_t count() const noexcept {return folders.size();}
};
[[nodiscard]] InventoryTreeSnapshot inventory_tree_from_login(
    const std::vector<InventoryFolder>& skeleton);
// Overlays direct child category records from verified Descendents2; only
// queried parents may contribute children, and only a live owner scope.
struct InventoryFolderRead
{
    std::uint64_t epoch=0U;
    std::uint64_t invalidation_revision=0U;
    std::string folder_id;
    bool valid=false;
    std::uint64_t version=0;
    std::vector<OutfitInventoryItem> items;
    InventoryTreeSnapshot tree;
};
[[nodiscard]] InventoryFolderRead fetch_inventory_folder_children(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view cap,std::string_view owner,std::string_view folder,
    std::uint64_t epoch,const FirestormRegionLease& lease);
// Merge only newer owner-validated snapshots; an empty items[] clears a
// folder on the same/newer version. Callers must first check the region lease.
[[nodiscard]] bool merge_inventory_folder_read(
    InventoryTreeSnapshot& tree,
    std::map<std::string,InventoryFolderContents,std::less<>>& contents,
    const InventoryFolderRead& read);
[[nodiscard]] std::size_t merge_inventory_categories(
    InventoryTreeSnapshot& tree,const OutfitInventoryRead& result);
[[nodiscard]] std::size_t apply_inventory_categories(
    InventoryTreeSnapshot& tree,const LlsdValue& response,
    std::string_view owner_id,const std::vector<std::string>& parents);
}
