/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include "nexverse/net/http_transport.hpp"
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace nexverse::protocol
{
enum class ExperiencePermissionDecision { allow, block };
struct ExperiencePermissionRequest
{
    std::uint64_t epoch=0U;
    std::string experience_id;
    ExperiencePermissionDecision decision=ExperiencePermissionDecision::allow;
};
class ExperiencePermissionConsent
{
public:
    // First keypress stages only. Enter confirms. ESC cancels.
    bool propose(std::string_view id,ExperiencePermissionDecision decision,
                 std::uint64_t epoch,bool capability_advertised);
    [[nodiscard]] std::optional<ExperiencePermissionRequest>
    confirm(std::uint64_t current_epoch);
    void cancel() noexcept {pending_.reset();}
    [[nodiscard]] const auto& pending() const noexcept {return pending_;}
private:
    std::optional<ExperiencePermissionRequest> pending_;
};
[[nodiscard]] std::optional<std::string> build_experience_permission_request(
    const ExperiencePermissionRequest& request);
struct ExperiencePermissionResponse
{
    std::uint64_t epoch=0U;
    bool confirmed=false;
    std::optional<ResidentExperiences> resident;
};
[[nodiscard]] ExperiencePermissionResponse write_experience_permission(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view capability_uri,
    const ExperiencePermissionRequest& request,
    const FirestormRegionLease& lease);
}
