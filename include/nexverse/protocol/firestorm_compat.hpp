/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/caps.hpp"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace nexverse::protocol
{
// Discovery is evidence of a capability URL, not proof that the simulator
// implements the complete Firestorm feature or grants any authorization.
struct FirestormCapsAvailability
{
    bool inventory_fetch=false;
    bool inventory_folder_fetch=false;
    bool inventory_thumbnail_upload=false;
    bool experience_owned=false;
    bool experience_permissions=false;
    bool experience_lists=false;
    bool experience_admin=false;
    bool experience_update=false;
    [[nodiscard]] std::size_t advertised() const noexcept;
};
[[nodiscard]] FirestormCapsAvailability firestorm_caps_availability(
    const CapabilityMap& capabilities);

// Minimal read-only Firestorm AgentExperiences response.  An absent
// purchase key means acquire is NOT offered; zero is a valid price.
// UUID values never enter structured health logs.
struct OwnedExperiences
{
    std::vector<std::string> ids;
    std::optional<std::int64_t> purchase_price;
};
[[nodiscard]] std::optional<OwnedExperiences> parse_agent_experiences(
    const LlsdValue& response);

struct ResidentExperiences
{
    std::vector<std::string> allowed;
    std::vector<std::string> blocked;
};
[[nodiscard]] std::optional<ResidentExperiences> parse_resident_experiences(
    const LlsdValue& response);

// OpenGenesisLINK InventoryThumbnailUpload is a two-stage, owner-scoped
// JPEG2000 upload CAP. These functions only handle the verified LLSD
// control contract. They DO NOT upload binaries or grant permissions.
enum class ThumbnailTarget { item, folder };
struct ThumbnailUploadStage
{
    enum class State { upload, complete } state=State::upload;
    // Secret one-use URL: never copy into logs, snapshots, or error text.
    std::string uploader;
    std::string new_asset;
};
[[nodiscard]] std::optional<std::string> build_thumbnail_begin_request(
    ThumbnailTarget target,std::string_view inventory_uuid);
[[nodiscard]] std::optional<ThumbnailUploadStage> parse_thumbnail_stage(
    const LlsdValue& response);
}
