/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/ui/viewer_shell.hpp"
#include "nexverse/ui/ui_renderer.hpp"
#include <array>
#include <cstdint>
#include <utility>
#include <string>
#include <vector>
namespace nexverse::ui
{
struct ViewerShellView
{
    std::string region_name;
    std::string avatar_name;
    bool region_connected=false;
    bool terrain_ready=false;
    bool parcel_ready=false;
    bool avatar_present=false;
    bool appearance_ready=false;
    std::string avatar_view_status;
    std::string avatar_fragment_status;
    std::string avatar_bake_status;
    std::string avatar_report_notice;
    std::array<float,3> agent_position{};
    std::uint32_t region_width=256U,region_height=256U;
    std::size_t objects=0U,terrain_patches=0U;
    std::size_t remote_avatars=0U,animations=0U,attachments=0U;
    // Server-reported full active snapshot, not an available clip library.
    std::vector<std::pair<std::string,std::int32_t>> animation_entries;
    std::vector<std::string> animation_asset_status;
    std::size_t animation_cache_size=0U;
    bool selected_animation_retryable=false;
    struct NearbyObjectRow
    {
        std::uint32_t local_id=0U;
        float distance_metres=0.0f;
        std::string object_id;
    };
    std::vector<NearbyObjectRow> nearby_objects;

    std::vector<std::array<float,2>> avatar_positions;
    std::vector<std::string> chat_lines;
};
// Drawn strictly when a real simulator is ready. UI geometry corresponds
// to ViewerShellLayout::hit; actions are handled by the host app.
void render_viewer_shell(UiRenderer& renderer,int width,int height,
                         const ViewerShellState& state,
                         const ViewerShellView& view);
}
