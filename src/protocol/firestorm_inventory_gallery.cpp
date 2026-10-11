/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_inventory_gallery.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <set>

namespace nexverse::protocol
{
std::vector<OutfitGalleryFolder> outfits_from_login_skeleton(
    const std::vector<InventoryFolder>& skeleton)
{
    std::set<std::string> outfit_roots;
    for(const auto& folder:skeleton)
        if(folder.type_default==48 && uuid_to_bytes(folder.folder_id))
            outfit_roots.insert(folder.folder_id);
    std::vector<OutfitGalleryFolder> result;
    std::set<std::string> seen;
    for(const auto& folder:skeleton)
    {
        if(!uuid_to_bytes(folder.folder_id)||
           !seen.insert(folder.folder_id).second)
            continue;
        if(folder.type_default!=47 &&
           !outfit_roots.contains(folder.parent_id))
            continue;
        // Truncate untrusted display names, preserving the exact
        // server-provided UUID but never generating one locally.
        std::string title=folder.name.substr(0,128U);
        title.erase(std::remove_if(title.begin(),title.end(),
            [](unsigned char c){return c<32U ||c==127U;}),title.end());
        if(title.empty())title="Unbenanntes Outfit";
        result.push_back({folder.folder_id,title,folder.version});
        if(result.size()>=256U)break;
    }
    return result;
}
}
