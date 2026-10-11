/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_compat.hpp"
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
struct FirestormRegionRead
{
    std::uint64_t issued_epoch=0U;
    bool discovered=false;
    FirestormCapsAvailability available;
    std::optional<OwnedExperiences> owned;
    std::optional<ResidentExperiences> resident;
    std::map<std::string,std::string,std::less<>> display_names;
};
[[nodiscard]] std::optional<std::string> experience_profile_query(
    std::string_view cap_uri,const std::vector<std::string>& ids);
[[nodiscard]] std::map<std::string,std::string,std::less<>>
parse_experience_profiles(const LlsdValue& response);
[[nodiscard]] FirestormRegionRead fetch_firestorm_region(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view seed,std::uint64_t issued_epoch,
    const FirestormRegionLease& lease);
}
