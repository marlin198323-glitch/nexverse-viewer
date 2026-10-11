/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/login.hpp"
#include <string>
#include <vector>

namespace nexverse::protocol
{
// Firestorm LLFolderType: FT_MY_OUTFITS=48, FT_OUTFIT=47,
// FT_CURRENT_OUTFIT=46. Skeleton is real login data, not fabricated.
struct OutfitGalleryFolder
{
    std::string id;
    std::string name;
    std::int32_t version=0;
};
[[nodiscard]] std::vector<OutfitGalleryFolder> outfits_from_login_skeleton(
    const std::vector<InventoryFolder>& skeleton);
}
