/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace nexverse::protocol
{
// Region-epoch-bound Outfit and COF snapshot accumulator. Polling may
// update current item lists without replacing a newer folder version.
class FirestormInventorySync
{
public:
    void enter(std::uint64_t epoch)
    {
        if(epoch_==epoch)return;
        epoch_=epoch;
        versions_.clear();
        items_.clear();
        thumbnails_.clear();
        read_=false;
    }
    [[nodiscard]] bool merge(std::uint64_t epoch,
                              const OutfitInventoryRead& incoming)
    {
        if(epoch==0U ||epoch!=epoch_ ||!incoming.valid)return false;
        bool changed=false;
        for(const auto& [folder,version]:incoming.versions)
        {
            const auto previous=versions_.find(folder);
            if(previous!=versions_.end()&&version<previous->second)continue;
            const bool new_version=previous==versions_.end() ||
                                   version>previous->second;
            if(new_version)
            {
                versions_.insert_or_assign(folder,version);
                changed=true;
            }
            if(const auto items=incoming.items.find(folder);
               items!=incoming.items.end() &&
               (new_version||!items_.contains(folder)))
            {
                items_.insert_or_assign(folder,items->second);
                changed=true;
            }
            if(const auto thumb=incoming.thumbnails.find(folder);
               thumb!=incoming.thumbnails.end() &&
               thumb->second.advertised)
            {
                // Firestorm's gallery may replace a thumbnail UUID without
                // bumping the folder version. An omitted thumbnail is not
                // authoritative evidence of removal on old Robust grids.
                const auto id=thumb->second.asset_id.value_or("");
                if(!thumbnails_.contains(folder)||thumbnails_.at(folder)!=id)
                {
                    thumbnails_.insert_or_assign(folder,id);
                    changed=true;
                }
            }
        }
        read_=true;
        return changed;
    }
    [[nodiscard]] bool read() const noexcept {return read_;}
    [[nodiscard]] const auto& items() const noexcept {return items_;}
    [[nodiscard]] const auto& thumbnails() const noexcept {return thumbnails_;}
    [[nodiscard]] const auto& versions() const noexcept {return versions_;}
private:
    std::uint64_t epoch_=0U;
    bool read_=false;
    std::map<std::string,std::uint64_t,std::less<>> versions_;
    std::map<std::string,std::vector<OutfitInventoryItem>,std::less<>> items_;
    std::map<std::string,std::string,std::less<>> thumbnails_;
};
}
