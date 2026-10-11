/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include "nexverse/net/http_transport.hpp"
#include <cstdint>
#include <memory>
#include <map>
#include <optional>
#include <string_view>

namespace nexverse::protocol
{
// Additional simulator region, separate from the login-region EventQueue.
// Only read-only caps are requested. Caller must recheck lease on commit.
struct ExperienceProfile
{
    std::string name;
    std::string description;
    std::int32_t maturity=13;
    bool enabled=true;
};
struct FirestormRegionRead
{
    std::uint64_t issued_epoch=0U;
    bool discovered=false;
    FirestormCapsAvailability available;
    std::optional<OwnedExperiences> owned;
    std::optional<ResidentExperiences> resident;
    std::map<std::string,std::string,std::less<>> display_names;
    std::map<std::string,ExperienceProfile,std::less<>> profiles;
    OutfitInventoryRead outfit_inventory;
    // Secret CAPS URI: runtime-only. Never serialize or log.
    std::string get_texture_uri;
    std::string inventory_folder_uri;
    std::string experience_permissions_uri;
};
[[nodiscard]] std::optional<std::string> experience_profile_query(
    std::string_view cap_uri,const std::vector<std::string>& ids);
[[nodiscard]] std::map<std::string,std::string,std::less<>>
parse_experience_profiles(const LlsdValue& response);
[[nodiscard]] std::map<std::string,ExperienceProfile,std::less<>>
parse_experience_profile_details(const LlsdValue& response);
[[nodiscard]] FirestormRegionRead fetch_firestorm_region(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view seed,std::uint64_t issued_epoch,
    const FirestormRegionLease& lease,
    std::string_view agent_id={},
    const std::vector<OutfitGalleryFolder>& outfits={},
    std::string_view current_outfit_folder={});
}
