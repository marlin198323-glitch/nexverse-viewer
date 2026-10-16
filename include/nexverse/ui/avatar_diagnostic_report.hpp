/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#pragma once
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include "nexverse/ui/world_entry_stage.hpp"
#include "nexverse/ui/world_entry_trace.hpp"
#include "nexverse/ui/loading_circuit_retry_policy.hpp"
namespace nexverse::ui
{
// Strictly whitelisted, locally observed facts. Never store agent, region,
// session, simulator endpoint, avatar name, chat or capability URL.
struct AvatarDiagnosticReport
{
    bool authenticated=false;
    bool world_ready=false;
    bool agent_present=false;
    bool region_connected=false;
    bool simulator_circuit_active=false;
    bool region_handle_present=false;
    bool agent_identity_present=false;
    bool appearance_received=false;
    std::uint32_t appearance_packets=0U;
    std::uint32_t appearance_malformed=0U;
    std::uint32_t appearance_other_avatar=0U;
    std::uint32_t appearance_self_accepted=0U;
    bool camera_valid=false;
    bool center_in_frustum=false;
    bool gpu_fragment_result_known=false;
    bool gpu_fragments_passed=false;
    std::uint64_t original_mesh_draws=0;
    std::uint64_t proxy_draws=0;
    std::uint64_t baked_texture_draws=0;
    std::uint64_t objects_drawn=0;
    std::uint64_t missing_mesh_objects=0;
    // Whitelisted CURRENT-FRAME rendering facts, not server strings.
    std::uint32_t terrain_real_layers=0U;
    bool terrain_textured_gl=false;
    bool terrain_fallback_gl=false;
    std::uint32_t mesh_lod_total=0U,mesh_lod_matched=0U;
    std::uint32_t mesh_lod_high_requested=0U;
    std::uint32_t avatar_body_bakes_gpu=0U,avatar_body_bakes_gl=0U;
    std::uint32_t avatar_self_keyframe_draws=0U;
    std::uint32_t avatar_self_server_motion_draws=0U;
    bool avatar_server_animation_received=false;
    WorldEntryStage world_entry_stage=WorldEntryStage::login;
    std::uint32_t world_entry_wait_seconds=0;
    LoadingCircuitRetryReason retry_reason=
        LoadingCircuitRetryReason::login_missing;
    std::uint32_t manual_reconnect_count=0;
    std::array<WorldEntryTraceItem,WorldEntryTrace::capacity> entry_trace{};
    std::size_t entry_trace_count=0U;
    std::uint64_t terrain_packets=0,object_packets=0;
    std::uint64_t avatar_packets=0,other_packets=0;
    bool caps_event_queue_available=false;
    std::uint32_t frame_width=0,frame_height=0;
};
[[nodiscard]] std::string avatar_diagnostic_text(
    const AvatarDiagnosticReport& report);
[[nodiscard]] std::string avatar_diagnostic_trace_text(
    const AvatarDiagnosticReport& report);
// Writes one private O_CREAT|O_EXCL file (0600 on POSIX). 'folder'
// should be the current user's local-state diagnostics directory.
[[nodiscard]] std::optional<std::filesystem::path> save_avatar_diagnostic(
    const std::filesystem::path& folder,
    const AvatarDiagnosticReport& report);
}
