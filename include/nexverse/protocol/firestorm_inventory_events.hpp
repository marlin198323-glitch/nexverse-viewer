/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include <cstdint>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
// OpenSimulator EventQueueGet BulkUpdateInventory is a *hint* that one or
// more folders changed. It is not a trusted replacement for CAPS inventory
// readback and cannot mutate inventory or current outfit contents directly.
struct InventoryDirtyBatch
{
    std::uint64_t epoch=0U;
    bool full_refresh=false;
    std::set<std::string,std::less<>> folders;
    [[nodiscard]] bool empty() const noexcept
    {return !full_refresh&&folders.empty();}
};
[[nodiscard]] std::optional<std::set<std::string,std::less<>>>
parse_bulk_inventory_dirty(const EventQueueEvent& event,
                           std::string_view authenticated_owner);

class InventoryEventInbox
{
public:
    [[nodiscard]] bool enqueue(const EventQueueEvent& event,
        std::string_view authenticated_owner,std::uint64_t issued_epoch,
        const FirestormRegionLease& lease);
    [[nodiscard]] InventoryDirtyBatch drain(std::uint64_t current_epoch,
        const FirestormRegionLease& lease);
    void reset();
private:
    std::mutex mutex_;
    InventoryDirtyBatch pending_;
};
}
