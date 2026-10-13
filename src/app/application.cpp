/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "nexverse/app/application.hpp"

#include "nexverse/assets/texture_loader.hpp"
#include "nexverse/assets/texture_disk_cache.hpp"
#include "nexverse/assets/texture_memory_cache.hpp"
#include "nexverse/assets/texture_request_priority.hpp"
#include "nexverse/assets/texture_residency.hpp"
#include "nexverse/assets/texture_distance.hpp"
#include "nexverse/assets/texture_mip.hpp"
#include "nexverse/assets/mesh_loader.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include "nexverse/assets/mesh_cache.hpp"
#include "nexverse/assets/mesh_residency.hpp"
#include "nexverse/assets/mesh_lod_policy.hpp"
#include "nexverse/assets/asset_gpu_budget.hpp"
#include "nexverse/assets/gpu_staging_budget.hpp"
#include "nexverse/assets/gpu_staging_recovery.hpp"
#include "nexverse/assets/asset_reuse_history.hpp"
#include "nexverse/assets/asset_failure_telemetry.hpp"
#include "nexverse/assets/asset_cache_pressure_window.hpp"
#include "nexverse/assets/asset_missing_id_counter.hpp"
#include "nexverse/assets/asset_request_fairness.hpp"
#include "nexverse/assets/asset_scene_coverage.hpp"
#include "nexverse/assets/asset_streaming_snapshot.hpp"
#include "nexverse/assets/asset_lod_dwell.hpp"
#include "nexverse/assets/asset_gpu_upload_gate.hpp"
#include "nexverse/assets/asset_upload_telemetry.hpp"
#include "nexverse/assets/asset_scene_priority_feedback.hpp"
#include "nexverse/assets/asset_stream_scheduler.hpp"
#include "nexverse/assets/texture_prefetch.hpp"
#include "nexverse/assets/texture_cached_request.hpp"
#include "nexverse/core/logger.hpp"
#include "nexverse/core/system_metrics.hpp"
#include "nexverse/core/version.hpp"
#include "nexverse/modules/module_manager.hpp"
#include "nexverse/net/http_transport.hpp"
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include "nexverse/protocol/firestorm_region_read.hpp"
#include "nexverse/protocol/firestorm_inventory_gallery.hpp"
#include "nexverse/protocol/firestorm_inventory_sync.hpp"
#include "nexverse/protocol/firestorm_inventory_items.hpp"
#include "nexverse/protocol/firestorm_inventory_tree.hpp"
#include "nexverse/protocol/firestorm_inventory_events.hpp"
#include "nexverse/protocol/region_event_queue.hpp"
#include "nexverse/protocol/region_circuit_handoff.hpp"
#include "nexverse/protocol/firestorm_outfit_wear_preflight.hpp"
#include "nexverse/protocol/firestorm_experience_permissions.hpp"
#include "nexverse/protocol/caps_event_dispatcher.hpp"
#include "nexverse/protocol/event_queue_recovery.hpp"
#include "nexverse/protocol/caps_world_events.hpp"
#include "nexverse/protocol/circuit.hpp"
#include "nexverse/protocol/environment_settings.hpp"
#include "nexverse/protocol/grid_discovery.hpp"
#include "nexverse/protocol/login.hpp"
#include "nexverse/protocol/packet_dispatcher.hpp"
#include "nexverse/protocol/render_materials.hpp"
#include "nexverse/protocol/region_material_scope.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/movement_controls.hpp"
#include "nexverse/protocol/movement_feedback.hpp"
#include "nexverse/protocol/avatar_bakes.hpp"
#include "nexverse/render/renderer.hpp"
#include "nexverse/ui/theme.hpp"
#include "nexverse/ui/ui_renderer.hpp"
#include "nexverse/ui/viewer_shell.hpp"
#include "nexverse/ui/viewer_shell_render.hpp"
#include "nexverse/world/world_model.hpp"
#include "nexverse/ui/welcome_page.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <atomic>
#include <array>
#include <cmath>
#include <chrono>
#include <deque>
#include <exception>
#include <future>
#include <limits>
#include <memory>
#include <map>
#include <set>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>

namespace
{
using nexverse::ui::Color;
using nexverse::ui::UiRenderer;
namespace theme = nexverse::ui::theme;

std::string_view circuit_state_label(nexverse::protocol::CircuitState state)
{
    using nexverse::protocol::CircuitState;
    switch (state)
    {
        case CircuitState::disconnected: return "OFFLINE";
        case CircuitState::opening_socket: return "SOCKET";
        case CircuitState::awaiting_use_circuit_ack: return "CIRCUIT";
        case CircuitState::awaiting_movement_ack: return "MOVEMENT";
        case CircuitState::awaiting_region_ready: return "REGION";
        case CircuitState::active: return "UDP LIVE";
        case CircuitState::failed: return "UDP ERROR";
    }
    return "UNKNOWN";
}

Color circuit_state_color(nexverse::protocol::CircuitState state)
{
    using nexverse::protocol::CircuitState;
    switch (state)
    {
        case CircuitState::active: return theme::success;
        case CircuitState::failed: return theme::magenta;
        case CircuitState::opening_socket:
        case CircuitState::awaiting_use_circuit_ack:
        case CircuitState::awaiting_movement_ack:
        case CircuitState::awaiting_region_ready:
            return theme::cyan;
        case CircuitState::disconnected:
            return theme::text_muted;
    }
    return theme::text_muted;
}

enum class LoginField
{
    username,
    password
};

struct SimulationLoadStats
{
    std::uint32_t terrain_packets = 0U;
    std::uint32_t object_packets = 0U;
    std::uint32_t avatar_packets = 0U;
    std::uint32_t other_packets = 0U;
};

struct CapsRuntimeState
{
    std::string status = "WAITING";
    std::uint32_t capability_count = 0U;
    std::uint32_t event_count = 0U;
    bool simulator_features_loaded = false;
    bool event_queue_available = false;
    // Read-only initial login-region CAPS status. Advertised != live-verified.
    nexverse::protocol::FirestormCapsAvailability firestorm_caps;
    bool owned_experiences_read = false;
    std::size_t owned_experiences = 0U;
    bool experience_acquire_offered = false;
    bool resident_experiences_read = false;
    std::size_t allowed_experiences = 0U;
    std::size_t blocked_experiences = 0U;
    // Local display state only; never log or persist Experience UUIDs.
    std::vector<std::string> owned_ids;
    std::vector<std::string> allowed_ids;
    std::vector<std::string> blocked_ids;
    std::map<std::string,std::string,std::less<>> experience_names;
    std::map<std::string,nexverse::protocol::ExperienceProfile,std::less<>>
        experience_profiles;
    bool outfit_inventory_read=false;
    std::size_t outfit_folders_read=0U;
    std::map<std::string,std::string,std::less<>> outfit_thumbnail_assets;
    std::map<std::string,std::vector<nexverse::protocol::OutfitInventoryItem>,
             std::less<>> outfit_folder_items;
    std::string current_outfit_folder;
    std::map<std::string,nexverse::protocol::InventoryFolder,std::less<>>
        inventory_dynamic_folders;
    std::vector<nexverse::protocol::OutfitInventoryItem>
        resolved_outfit_items;
    std::string resolved_outfit_folder;
    std::string experience_permission_status;
};
struct GalleryTextureReply
{
    std::uint64_t epoch=0U;
    std::string folder_id;
    std::string asset_id;
    nexverse::assets::ImageRgba8 image;
};

struct FrameMetrics
{
    double frame_time_ms = 0.0;
    double fps = 0.0;
};

struct CameraState
{
    nexverse::protocol::Vec3 center{128.0f, 128.0f, 30.0f};
    nexverse::protocol::Vec3 at{1.0f, 0.0f, 0.0f};
    nexverse::protocol::Vec3 left{0.0f, 1.0f, 0.0f};
    nexverse::protocol::Vec3 up{0.0f, 0.0f, 1.0f};
    float far_clip = 256.0f;
};

nexverse::protocol::Vec3 rotate_vector(
    const nexverse::protocol::Quaternion& q,
    nexverse::protocol::Vec3 v)
{
    const float xx = q.x * q.x;
    const float yy = q.y * q.y;
    const float zz = q.z * q.z;
    const float xy = q.x * q.y;
    const float xz = q.x * q.z;
    const float yz = q.y * q.z;
    const float wx = q.w * q.x;
    const float wy = q.w * q.y;
    const float wz = q.w * q.z;

    return {
        (1.0f - 2.0f * (yy + zz)) * v.x +
            2.0f * (xy - wz) * v.y +
            2.0f * (xz + wy) * v.z,
        2.0f * (xy + wz) * v.x +
            (1.0f - 2.0f * (xx + zz)) * v.y +
            2.0f * (yz - wx) * v.z,
        2.0f * (xz - wy) * v.x +
            2.0f * (yz + wx) * v.y +
            (1.0f - 2.0f * (xx + yy)) * v.z};
}

CameraState camera_from_agent(const nexverse::world::AgentState& agent)
{
    CameraState camera;
    camera.center = {
        agent.position.x,
        agent.position.y,
        agent.position.z + 1.7f};
    camera.at = rotate_vector(agent.rotation, {1.0f, 0.0f, 0.0f});
    camera.left = rotate_vector(agent.rotation, {0.0f, 1.0f, 0.0f});
    camera.up = rotate_vector(agent.rotation, {0.0f, 0.0f, 1.0f});
    return camera;
}

CameraState render_camera_from_agent(
    const nexverse::world::AgentState& agent)
{
    CameraState camera;

    const auto forward =
        rotate_vector(agent.rotation, {1.0f, 0.0f, 0.0f});

    const nexverse::protocol::Vec3 target{
        agent.position.x,
        agent.position.y,
        agent.position.z + 1.45f};

    camera.center = {
        target.x - forward.x * 8.0f,
        target.y - forward.y * 8.0f,
        target.z + 4.2f};

    const nexverse::protocol::Vec3 delta{
        target.x - camera.center.x,
        target.y - camera.center.y,
        target.z - camera.center.z};

    const float length = std::sqrt(
        delta.x * delta.x +
        delta.y * delta.y +
        delta.z * delta.z);

    if (length > 0.0001f)
    {
        camera.at = {
            delta.x / length,
            delta.y / length,
            delta.z / length};
    }
    else
    {
        camera.at = forward;
    }

    camera.left =
        rotate_vector(agent.rotation, {0.0f, 1.0f, 0.0f});
    camera.up = {0.0f, 0.0f, 1.0f};
    camera.far_clip = 512.0f;
    return camera;
}

nexverse::render::RenderLegacyAppearance render_legacy_appearance(
    const nexverse::protocol::LegacyMaterial& source)
{
    nexverse::render::RenderLegacyAppearance result;
    const auto copy_map = [](
        const std::string& id,
        const nexverse::protocol::LegacyMapTransform& transform,
        nexverse::render::RenderLegacyMap& out)
    {
        if (nexverse::protocol::is_valid_legacy_material_id(id))
            out.texture_id = id;
        out.repeat = {transform.repeat_x, transform.repeat_y};
        out.offset = {transform.offset_x, transform.offset_y};
        out.rotation = transform.rotation;
    };
    copy_map(source.normal_id, source.normal_transform, result.normal);
    copy_map(source.specular_id, source.specular_transform, result.specular);
    result.specular_color = source.specular_color;
    result.specular_exponent = source.specular_exponent;
    result.environment_intensity = source.environment_intensity;
    return result;
}

nexverse::render::RenderPrimitiveKind render_primitive_kind(
    const nexverse::protocol::PrimitiveShapeData& shape)
{
    constexpr std::uint8_t path_straight = 0x10U;
    constexpr std::uint8_t path_curve_1 = 0x20U;
    constexpr std::uint8_t path_curve_2 = 0x30U;
    constexpr std::uint8_t path_flexible = 0x80U;

    constexpr std::uint8_t profile_circle = 0U;
    constexpr std::uint8_t profile_square = 1U;
    constexpr std::uint8_t profile_triangle_min = 2U;
    constexpr std::uint8_t profile_triangle_max = 4U;
    constexpr std::uint8_t profile_half_circle = 5U;

    const std::uint8_t profile = shape.profile_shape();

    if (shape.path_curve == path_straight ||
        shape.path_curve == path_flexible)
    {
        if (profile == profile_circle)
        {
            return nexverse::render::RenderPrimitiveKind::cylinder;
        }
        if (profile >= profile_triangle_min &&
            profile <= profile_triangle_max)
        {
            return nexverse::render::RenderPrimitiveKind::prism;
        }
        return nexverse::render::RenderPrimitiveKind::box;
    }

    if (shape.path_curve == path_curve_1 ||
        shape.path_curve == path_curve_2)
    {
        if (profile == profile_half_circle)
        {
            return nexverse::render::RenderPrimitiveKind::sphere;
        }

        return nexverse::render::RenderPrimitiveKind::torus;
    }

    if (profile == profile_circle)
    {
        return nexverse::render::RenderPrimitiveKind::cylinder;
    }
    if (profile == profile_square)
    {
        return nexverse::render::RenderPrimitiveKind::box;
    }
    if (profile >= profile_triangle_min &&
        profile <= profile_triangle_max)
    {
        return nexverse::render::RenderPrimitiveKind::prism;
    }

    return nexverse::render::RenderPrimitiveKind::box;
}

struct LoginUiState
{
    LoginField focused = LoginField::username;
    std::string username;
    std::string password;
    std::string status = "READY";
    std::string notice = "Avatarname und Passwort eingeben.";
    bool submit_requested = false;
    bool busy = false;
    bool authenticated = false;
};

struct InputContext
{
    LoginUiState* login = nullptr;
    nexverse::ui::ViewerShellState* viewer_shell=nullptr;
    nexverse::protocol::ExperienceUiState* experience = nullptr;
    bool* outfits = nullptr;
    std::size_t* outfit_selection = nullptr;
    bool* inventory_open=nullptr;
    std::string* inventory_parent=nullptr;
    std::size_t* inventory_selection=nullptr;
    nexverse::protocol::InventoryTreeSnapshot* inventory_tree=nullptr;
    bool* inventory_refresh_requested=nullptr;
    std::size_t* inventory_item_selection=nullptr;
    bool* inventory_item_detail_requested=nullptr;
    bool* simulation_ready = nullptr;
    nexverse::protocol::ExperiencePermissionConsent* permission_consent=nullptr;
    nexverse::protocol::FirestormRegionLease* region_lease=nullptr;
    CapsRuntimeState* caps=nullptr;
    std::mutex* caps_mutex=nullptr;
    std::string* permission_cap=nullptr;
    std::optional<nexverse::protocol::ExperiencePermissionRequest>*
        approved_permission=nullptr;
    bool* permission_busy=nullptr;
    bool exclusive_fullscreen = false;
    int windowed_x = 100;
    int windowed_y = 100;
    int windowed_width = 1440;
    int windowed_height = 900;
};

std::string masked_password(std::string_view password)
{
    std::string masked;
    for (const char raw : password)
    {
        const auto byte = static_cast<unsigned char>(raw);
        if ((byte & 0xC0U) != 0x80U)
        {
            masked.push_back('*');
        }
    }
    return masked;
}

void append_utf8(std::string& target, char32_t codepoint, std::size_t limit)
{
    if (codepoint < U' ' || codepoint == 0x7FU) return;

    std::string encoded;
    if (codepoint <= 0x7FU)
    {
        encoded.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FFU)
    {
        encoded.push_back(static_cast<char>(0xC0U | ((codepoint >> 6U) & 0x1FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0xFFFFU)
    {
        encoded.push_back(static_cast<char>(0xE0U | ((codepoint >> 12U) & 0x0FU)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }
    else if (codepoint <= 0x10FFFFU)
    {
        encoded.push_back(static_cast<char>(0xF0U | ((codepoint >> 18U) & 0x07U)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 12U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | ((codepoint >> 6U) & 0x3FU)));
        encoded.push_back(static_cast<char>(0x80U | (codepoint & 0x3FU)));
    }

    if (target.size() + encoded.size() <= limit)
    {
        target += encoded;
    }
}

void erase_last_utf8(std::string& target)
{
    if (target.empty()) return;
    auto index = target.size() - 1U;
    while (index > 0U &&
           (static_cast<unsigned char>(target[index]) & 0xC0U) == 0x80U)
    {
        --index;
    }
    target.erase(index);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if(context && context->viewer_shell &&
       context->viewer_shell->window==nexverse::ui::ViewerWindow::chat)
    {
        append_utf8(context->viewer_shell->chat_draft,
                    static_cast<char32_t>(codepoint),500U);
        return;
    }
    if(context && context->viewer_shell &&
       context->viewer_shell->window==nexverse::ui::ViewerWindow::teleport)
    {
        auto& shell=*context->viewer_shell;
        const char32_t c=static_cast<char32_t>(codepoint);
        if((c>=U'0'&&c<=U'9')||c==U'.'||c==U'-')
            append_utf8(shell.teleport_coords[
                static_cast<std::size_t>(shell.teleport_focus)],c,18U);
        return;
    }
    if (context == nullptr || context->login == nullptr || context->login->busy) return;

    LoginUiState& state = *context->login;
    if (state.authenticated) return;
    if (state.focused == LoginField::username)
    {
        append_utf8(state.username, static_cast<char32_t>(codepoint), 96U);
    }
    else
    {
        append_utf8(state.password, static_cast<char32_t>(codepoint), 255U);
    }
}

void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(context && context->viewer_shell &&
           (context->viewer_shell->window!=
                nexverse::ui::ViewerWindow::none||
            context->viewer_shell->open_menu>=0))
        {
            context->viewer_shell->close();
            return;
        }
        if(context &&context->permission_consent &&
           context->permission_consent->pending())
            context->permission_consent->cancel();
        else if(context && context->experience && context->experience->open)
            context->experience->close();
        else if(context && context->outfits && *context->outfits)
            *context->outfits=false;
        else if(context &&context->inventory_open&&*context->inventory_open)
            *context->inventory_open=false;
        else
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if(context && context->viewer_shell &&
       context->login && context->login->authenticated &&
       context->simulation_ready && *context->simulation_ready &&
       action==GLFW_PRESS)
    {
        auto& shell=*context->viewer_shell;
        if(key==GLFW_KEY_F6||key==GLFW_KEY_F7)
        {
            const auto which=key==GLFW_KEY_F6
                ?nexverse::ui::ViewerWindow::chat
                :nexverse::ui::ViewerWindow::region;
            shell.window=shell.window==which
                ?nexverse::ui::ViewerWindow::none:which;
            shell.open_menu=-1;
            if(shell.window!=nexverse::ui::ViewerWindow::none)
            {
                if(context->outfits)*context->outfits=false;
                if(context->inventory_open)*context->inventory_open=false;
                if(context->experience)context->experience->close();
            }
            return;
        }
        if(shell.window==nexverse::ui::ViewerWindow::chat)
        {
            if(key==GLFW_KEY_ENTER||key==GLFW_KEY_KP_ENTER)
                shell.chat_submit_requested=true;
            else if(key==GLFW_KEY_BACKSPACE)
                erase_last_utf8(shell.chat_draft);
            return;
        }
        if(shell.window==nexverse::ui::ViewerWindow::teleport)
        {
            if(key==GLFW_KEY_TAB)
                shell.teleport_focus=(shell.teleport_focus+1)%3;
            else if(key==GLFW_KEY_ENTER||key==GLFW_KEY_KP_ENTER)
                shell.teleport_submit_requested=true;
            else if(key==GLFW_KEY_BACKSPACE)
                erase_last_utf8(shell.teleport_coords[
                    static_cast<std::size_t>(shell.teleport_focus)]);
            return;
        }
        if(shell.window!=nexverse::ui::ViewerWindow::none &&
           key!=GLFW_KEY_F11)return;
    }
    if(context && context->experience && action==GLFW_PRESS)
    {
        auto& ui=*context->experience;
        if(key==GLFW_KEY_F8)
        {
            ui.toggle(context->login && context->login->authenticated,
                      context->simulation_ready && *context->simulation_ready);
            if(!ui.open &&context->permission_consent)
                context->permission_consent->cancel();
            if(ui.open &&context->outfits)*context->outfits=false;
            if(ui.open &&context->inventory_open)
                *context->inventory_open=false;
            return;
        }
        if(key==GLFW_KEY_F9 &&context->outfits)
        {
            if(context->login &&context->login->authenticated &&
               context->simulation_ready &&*context->simulation_ready)
            {
                *context->outfits=!*context->outfits;
                if(*context->outfits)
                {
                    ui.close();
                    if(context->permission_consent)
                        context->permission_consent->cancel();
                    if(context->inventory_open)*context->inventory_open=false;
                    if(context->outfit_selection)
                        *context->outfit_selection=0U;
                }
            }
            return;
        }
        if(key==GLFW_KEY_F10 &&context->inventory_open &&
           context->inventory_parent&&context->inventory_selection &&
           context->inventory_tree)
        {
            if(context->login &&context->login->authenticated &&
               context->simulation_ready &&*context->simulation_ready)
            {
                *context->inventory_open=!*context->inventory_open;
                if(*context->inventory_open)
                {
                    ui.close();
                    if(context->permission_consent)
                        context->permission_consent->cancel();
                    if(context->outfits)*context->outfits=false;
                    *context->inventory_selection=0U;
                }
            }
            return;
        }
        if(context->inventory_open &&*context->inventory_open &&
           context->inventory_parent&&context->inventory_selection &&
           context->inventory_tree)
        {
            auto& folder=*context->inventory_parent;
            auto& selected=*context->inventory_selection;
            std::vector<std::string> children;
            for(const auto& [id,node]:context->inventory_tree->folders)
                if(node.parent_id==folder)children.push_back(id);
            if(key==GLFW_KEY_R)
            {
                if(context->inventory_refresh_requested)
                    *context->inventory_refresh_requested=true;
                return;
            }
            if(context->inventory_item_selection&&
               (key==GLFW_KEY_J||key==GLFW_KEY_K))
            {
                auto& index=*context->inventory_item_selection;
                if(key==GLFW_KEY_J)index=std::min(index+1U,std::size_t{9U});
                else if(index>0U)--index;
                return;
            }
            if(key==GLFW_KEY_I &&context->inventory_item_detail_requested)
            {
                *context->inventory_item_detail_requested=true;
                return;
            }
            if(key==GLFW_KEY_DOWN&&!children.empty())
                selected=std::min(selected+1U,children.size()-1U);
            else if(key==GLFW_KEY_UP&&selected>0U)--selected;
            else if((key==GLFW_KEY_RIGHT||key==GLFW_KEY_ENTER)&&
                    selected<children.size())
            {
                folder=children[selected];
                selected=0U;
                if(context->inventory_item_selection)
                    *context->inventory_item_selection=0U;
            }
            else if(key==GLFW_KEY_LEFT||key==GLFW_KEY_BACKSPACE)
            {
                const auto it=context->inventory_tree->folders.find(folder);
                if(it!=context->inventory_tree->folders.end()&&
                   !it->second.parent_id.empty())
                {
                    folder=it->second.parent_id;
                    selected=0U;
                    if(context->inventory_item_selection)
                        *context->inventory_item_selection=0U;
                }
            }
            return;
        }
        if(context->outfits &&*context->outfits)
        {
            if(context->outfit_selection)
            {
                auto& selection=*context->outfit_selection;
                if(key==GLFW_KEY_DOWN)selection=
                    std::min(selection+1U,std::size_t{9U});
                else if(key==GLFW_KEY_UP &&selection>0U)--selection;
            }
            return;
        }
        if(ui.open)
        {
            if(context->permission_consent &&
               context->region_lease &&
               context->approved_permission &&
               key==GLFW_KEY_ENTER &&
               context->permission_consent->pending())
            {
                *context->approved_permission=
                    context->permission_consent->confirm(
                        context->region_lease->epoch());
                return;
            }
            if((key==GLFW_KEY_A||key==GLFW_KEY_B||
                key==GLFW_KEY_X) &&
               context->caps &&context->caps_mutex &&
               context->permission_consent &&
               context->region_lease &&context->permission_cap &&
               context->permission_busy &&!*context->permission_busy &&
               !context->permission_cap->empty() &&
               context->login &&context->login->authenticated)
            {
                std::scoped_lock lock{*context->caps_mutex};
                const auto& caps=*context->caps;
                const auto& ids=ui.tab==nexverse::protocol::ExperienceUiState::Tab::owned
                    ?caps.owned_ids:
                    ui.tab==nexverse::protocol::ExperienceUiState::Tab::allowed
                    ?caps.allowed_ids:caps.blocked_ids;
                if(ui.selected_index<ids.size())
                {
                    (void)context->permission_consent->propose(
                        ids[ui.selected_index],
                        key==GLFW_KEY_A
                            ?nexverse::protocol::ExperiencePermissionDecision::allow
                            :key==GLFW_KEY_B
                            ?nexverse::protocol::ExperiencePermissionDecision::block
                            :nexverse::protocol::ExperiencePermissionDecision::forget,
                        context->region_lease->epoch(),
                        caps.firestorm_caps.experience_permissions);
                }
                return;
            }
            if(context->permission_consent &&
               (key==GLFW_KEY_1||key==GLFW_KEY_2 ||
                key==GLFW_KEY_3||key==GLFW_KEY_UP||
                key==GLFW_KEY_DOWN))
                context->permission_consent->cancel();
            if(key==GLFW_KEY_1)
                ui.select(nexverse::protocol::ExperienceUiState::Tab::owned);
            else if(key==GLFW_KEY_2)
                ui.select(nexverse::protocol::ExperienceUiState::Tab::allowed);
            else if(key==GLFW_KEY_3)
                ui.select(nexverse::protocol::ExperienceUiState::Tab::blocked);
            else if(key==GLFW_KEY_DOWN)
                ui.choose(ui.selected_index+1U);
            else if(key==GLFW_KEY_UP)
                ui.choose(ui.selected_index>0U?ui.selected_index-1U:0U);
            return;
        }
    }

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS && context != nullptr)
    {
        if (!context->exclusive_fullscreen)
        {
            glfwGetWindowPos(
                window,
                &context->windowed_x,
                &context->windowed_y);
            glfwGetWindowSize(
                window,
                &context->windowed_width,
                &context->windowed_height);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode =
                monitor != nullptr ? glfwGetVideoMode(monitor) : nullptr;

            if (monitor != nullptr && mode != nullptr)
            {
                glfwSetWindowMonitor(
                    window,
                    monitor,
                    0,
                    0,
                    mode->width,
                    mode->height,
                    mode->refreshRate);
                context->exclusive_fullscreen = true;
            }
        }
        else
        {
            glfwSetWindowMonitor(
                window,
                nullptr,
                context->windowed_x,
                context->windowed_y,
                std::max(context->windowed_width, 1100),
                std::max(context->windowed_height, 700),
                GLFW_DONT_CARE);
            glfwMaximizeWindow(window);
            context->exclusive_fullscreen = false;
        }
        return;
    }

    if (context == nullptr || context->login == nullptr) return;
    LoginUiState& state = *context->login;
    if (state.authenticated) return;

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        state.focused = state.focused == LoginField::username
            ? LoginField::password
            : LoginField::username;
        return;
    }

    if (state.busy) return;

    if (key == GLFW_KEY_BACKSPACE &&
        (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (state.focused == LoginField::username) erase_last_utf8(state.username);
        else erase_last_utf8(state.password);
        return;
    }

    if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) &&
        action == GLFW_PRESS)
    {
        state.submit_requested = true;
    }
}

bool point_inside(double px, double py, float x, float y, float width, float height)
{
    return px >= static_cast<double>(x) &&
           py >= static_cast<double>(y) &&
           px <= static_cast<double>(x + width) &&
           py <= static_cast<double>(y + height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    auto* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if(context &&context->viewer_shell&&context->simulation_ready &&
       *context->simulation_ready&&context->login &&
       context->login->authenticated &&
       !(context->experience&&context->experience->open) &&
       !(context->outfits&&*context->outfits) &&
       !(context->inventory_open&&*context->inventory_open))
    {
        int width=0,height=0;
        glfwGetWindowSize(window,&width,&height);
        double mx=0.0,my=0.0;
        glfwGetCursorPos(window,&mx,&my);
        auto& shell=*context->viewer_shell;
        const nexverse::ui::ViewerShellLayout layout{width,height};
        int menu_change=-2;
        const auto command=layout.hit(mx,my,shell,&menu_change);
        if(menu_change>=0)
        {
            shell.open_menu=shell.open_menu==menu_change
                ?-1:menu_change;
            return;
        }
        if(menu_change==-3)
        {
            shell.open_menu=-1;
            return;
        }
        if(command==nexverse::ui::ViewerAction::close)
        {
            shell.close();return;
        }
        if(command==nexverse::ui::ViewerAction::send_chat)
        {
            shell.chat_submit_requested=true;return;
        }
        if(command==nexverse::ui::ViewerAction::send_teleport)
        {
            shell.teleport_submit_requested=true;return;
        }
        if(command==nexverse::ui::ViewerAction::focus_teleport_x||
           command==nexverse::ui::ViewerAction::focus_teleport_y||
           command==nexverse::ui::ViewerAction::focus_teleport_z)
        {
            shell.teleport_focus=
                command==nexverse::ui::ViewerAction::focus_teleport_x?0:
                command==nexverse::ui::ViewerAction::focus_teleport_y?1:2;
            return;
        }
        if(command==nexverse::ui::ViewerAction::fullscreen)
        {
            shell.open_menu=-1;
            key_callback(window,GLFW_KEY_F11,0,GLFW_PRESS,0);
            return;
        }
        if(command==nexverse::ui::ViewerAction::inventory||
           command==nexverse::ui::ViewerAction::outfits||
           command==nexverse::ui::ViewerAction::experiences)
        {
            shell.close();
            if(command==nexverse::ui::ViewerAction::inventory &&
               context->inventory_open)
            {
                *context->inventory_open=true;
                if(context->inventory_selection)
                    *context->inventory_selection=0U;
            }
            else if(command==nexverse::ui::ViewerAction::outfits &&
                    context->outfits)
            {
                *context->outfits=true;
                if(context->outfit_selection)
                    *context->outfit_selection=0U;
            }
            else if(command==nexverse::ui::ViewerAction::experiences&&
                    context->experience)
                context->experience->toggle(true,true);
            if(context->experience&&
               command!=nexverse::ui::ViewerAction::experiences)
                context->experience->close();
            if(context->inventory_open&&
               command!=nexverse::ui::ViewerAction::inventory)
                *context->inventory_open=false;
            if(context->outfits&&
               command!=nexverse::ui::ViewerAction::outfits)
                *context->outfits=false;
            return;
        }
        if(command==nexverse::ui::ViewerAction::disabled)return;
        const auto open=nexverse::ui::window_for_action(command);
        if(open!=nexverse::ui::ViewerWindow::none)
        {
            shell.window=open;
            shell.open_menu=-1;
            if(context->experience)context->experience->close();
            if(context->inventory_open)*context->inventory_open=false;
            if(context->outfits)*context->outfits=false;
            return;
        }
        if(shell.input_captured()||
           command==nexverse::ui::ViewerAction::focus_chat)
            return;
    }
    if(context && context->experience && context->experience->open &&
       context->login && context->login->authenticated)
    {
        int window_w=0,window_h=0;
        glfwGetWindowSize(window,&window_w,&window_h);
        double px=0.0,py=0.0;
        glfwGetCursorPos(window,&px,&py);
        const float scale=std::clamp(
            static_cast<float>(window_h)/1080.0f,0.75f,1.5f);
        const float panel_w=std::min(670.0f*scale,
                                    static_cast<float>(window_w)-32.0f);
        const float panel_x=(static_cast<float>(window_w)-panel_w)/2.0f;
        const float panel_y=std::max(35.0f,
            (static_cast<float>(window_h)-520.0f*scale)/2.0f);
        if(point_inside(px,py,panel_x+panel_w-55.0f*scale,
                        panel_y+10.0f*scale,45.0f*scale,35.0f))
            context->experience->close();
        else
        {
            const float tabs_x=panel_x+20.0f*scale;
            const float tabs_y=panel_y+80.0f*scale;
            for(int i=0;i<3;++i)
                if(point_inside(px,py,tabs_x+static_cast<float>(i)*203.0f*scale,
                                tabs_y,194.0f*scale,40.0f*scale))
                {
                    context->experience->select(
                        static_cast<nexverse::protocol::ExperienceUiState::Tab>(i));
                    if(context->permission_consent)
                        context->permission_consent->cancel();
                    break;
                }
            const float list_y=panel_y+174.0f*scale;
            const auto offset=context->experience->selected_index>5U
                ? context->experience->selected_index-5U:0U;
            for(std::size_t i=0;i<6U;++i)
                if(point_inside(px,py,panel_x+22.0f*scale,
                      list_y+static_cast<float>(i)*31.0f*scale,
                      panel_w-44.0f*scale,28.0f*scale))
                {
                    context->experience->choose(offset+i);
                    if(context->permission_consent)
                        context->permission_consent->cancel();
                    break;
                }
        }
        return;
    }
    if(context&&context->outfits&&*context->outfits &&
       context->outfit_selection)
    {
        int w=0,h=0;
        glfwGetWindowSize(window,&w,&h);
        double px=0.0,py=0.0;
        glfwGetCursorPos(window,&px,&py);
        const float scale=std::clamp(
            static_cast<float>(h)/1080.0f,0.75f,1.5f);
        const float panel_w=std::min(790.0f*scale,
                                    static_cast<float>(w)-32.0f);
        const float x=(static_cast<float>(w)-panel_w)*0.5f;
        const float y=std::max(30.0f,
            (static_cast<float>(h)-500.0f*scale)*0.5f);
        if(point_inside(px,py,x+panel_w-55.0f*scale,
                        y+10.0f*scale,45.0f*scale,35.0f*scale))
        {
            *context->outfits=false;
            return;
        }
        if(point_inside(px,py,x+22.0f*scale,y+115.0f*scale,
                        425.0f*scale,300.0f*scale))
        {
            const auto index=static_cast<std::size_t>(
                (py-(y+115.0f*scale))/(30.0f*scale));
            *context->outfit_selection=std::min(index,std::size_t{9U});
        }
        return;
    }
    if(context&&context->inventory_open&&*context->inventory_open &&
       context->inventory_tree&&context->inventory_parent&&
       context->inventory_selection)
    {
        int ww=0,hh=0;glfwGetWindowSize(window,&ww,&hh);
        double mx=0,my=0;glfwGetCursorPos(window,&mx,&my);
        const float sc=std::clamp(static_cast<float>(hh)/1080.0f,0.75f,1.5f);
        const float pw=std::min(690.0f*sc,static_cast<float>(ww)-32.0f);
        const float xx=(static_cast<float>(ww)-pw)*0.5f;
        const float yy=std::max(30.0f,(static_cast<float>(hh)-520.0f*sc)*0.5f);
        if(point_inside(mx,my,xx+pw-55.0f*sc,yy+10.0f*sc,
                        45.0f*sc,35.0f*sc))
        {
            *context->inventory_open=false;
            return;
        }
        for(std::size_t idx=0U;idx<3U;++idx)
        {
            if(!point_inside(mx,my,
                xx+(23.0f+145.0f*static_cast<float>(idx))*sc,
                yy+(520.0f-72.0f)*sc,135.0f*sc,35.0f*sc))
                continue;
            if(idx==2U&&context->inventory_refresh_requested)
                *context->inventory_refresh_requested=true;
            else
            {
                auto& parent=*context->inventory_parent;
                auto& selected=*context->inventory_selection;
                if(idx==0U)
                {
                    std::vector<std::string> folders;
                    for(const auto& [id,node]:context->inventory_tree->folders)
                        if(node.parent_id==parent)folders.push_back(id);
                    if(selected<folders.size())
                    {
                        parent=folders[selected];
                        selected=0U;
                        if(context->inventory_item_selection)
                            *context->inventory_item_selection=0U;
                    }
                }
                else
                {
                    const auto current=
                        context->inventory_tree->folders.find(parent);
                    if(current!=context->inventory_tree->folders.end() &&
                       !current->second.parent_id.empty())
                    {
                        parent=current->second.parent_id;
                        selected=0U;
                        if(context->inventory_item_selection)
                            *context->inventory_item_selection=0U;
                    }
                }
            }
            return;
        }
        if(context->inventory_item_selection &&
           point_inside(mx,my,xx+pw*0.51f+12.0f*sc,yy+145.0f*sc,
                        pw*0.48f-20.0f*sc,270.0f*sc))
        {
            *context->inventory_item_selection=static_cast<std::size_t>(
                (my-yy-145.0f*sc)/(27.0f*sc));
            return;
        }
        if(point_inside(mx,my,xx+23.0f*sc,yy+145.0f*sc,
                        pw*0.51f-30.0f*sc,297.0f*sc))
        {
            const auto row=static_cast<std::size_t>(
                (my-yy-145.0f*sc)/(27.0f*sc));
            const auto offset=*context->inventory_selection>10U
                ?*context->inventory_selection-10U:0U;
            *context->inventory_selection=offset+row;
        }
        return;
    }
    if (context == nullptr || context->login == nullptr ||
        context->login->authenticated) return;

    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);

    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1536.0f, h / 960.0f),
        0.72f,
        1.55f);

    const float left_w = 252.0f * scale;
    const float bottom_h = 106.0f * scale;
    const float bottom_y = h - bottom_h;
    const float login_x = left_w + 20.0f * scale;
    const float login_y = bottom_y + 20.0f * scale;
    const float field_w = 184.0f * scale;
    const float field_h = 48.0f * scale;
    const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
    const float button_w = 164.0f * scale;

    double mouse_x = 0.0;
    double mouse_y = 0.0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    LoginUiState& state = *context->login;
    if (point_inside(mouse_x, mouse_y, login_x, login_y, field_w, field_h))
    {
        state.focused = LoginField::username;
    }
    else if (point_inside(
                 mouse_x,
                 mouse_y,
                 login_x + field_w + 8.0f * scale,
                 login_y,
                 field_w,
                 field_h))
    {
        state.focused = LoginField::password;
    }
    else if (point_inside(mouse_x, mouse_y, button_x, login_y, button_w, field_h) &&
             !state.busy)
    {
        state.submit_requested = true;
    }
}

std::optional<std::pair<std::string, std::string>> split_avatar_name(std::string_view name)
{
    const std::size_t first = name.find_first_not_of(' ');
    const std::size_t last = name.find_last_not_of(' ');
    if (first == std::string_view::npos || last == std::string_view::npos)
    {
        return std::nullopt;
    }

    const std::string_view trimmed = name.substr(first, last - first + 1U);
    const std::size_t separator = trimmed.find(' ');
    if (separator == std::string_view::npos)
    {
        return std::nullopt;
    }

    const std::size_t last_first = trimmed.find_first_not_of(' ', separator);
    if (last_first == std::string_view::npos)
    {
        return std::nullopt;
    }

    std::string first_name{trimmed.substr(0, separator)};
    std::string last_name{trimmed.substr(last_first)};
    if (first_name.empty() || last_name.empty())
    {
        return std::nullopt;
    }

    return std::pair<std::string, std::string>{
        std::move(first_name),
        std::move(last_name)};
}

Color with_alpha(Color color, float alpha)
{
    color.a = alpha;
    return color;
}

void status_row(UiRenderer& renderer, float x, float y, std::string_view label, std::string_view value, Color state)
{
    renderer.rect(x, y + 6.0f, 8.0f, 8.0f, state, 4.0f);
    renderer.text(x + 18.0f, y, 13.0f, label, theme::text_muted);

    const float value_width = renderer.text_width(13.0f, value);
    renderer.text(x + 205.0f - value_width, y, 13.0f, value, state);
}

void nav_item(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float scale,
    std::string_view title,
    std::string_view subtitle,
    bool active)
{
    const Color background = active
        ? Color{0.045f, 0.115f, 0.205f, 0.96f}
        : Color{0.026f, 0.045f, 0.078f, 0.76f};

    renderer.rect(x, y, width, 56.0f * scale, background, 10.0f * scale);
    if (active)
    {
        renderer.rect(x, y + 5.0f * scale, 3.0f * scale, 46.0f * scale, theme::cyan, 1.5f * scale);
    }

    renderer.rect(
        x + 12.0f * scale,
        y + 14.0f * scale,
        34.0f * scale,
        34.0f * scale,
        active ? with_alpha(theme::cyan, 0.22f) : with_alpha(theme::blue, 0.13f),
        17.0f * scale);

    renderer.text(
        x + 58.0f * scale,
        y + 9.0f * scale,
        14.0f * scale,
        title,
        active ? theme::text : Color{0.80f, 0.86f, 0.94f, 1.0f});

    renderer.text(
        x + 58.0f * scale,
        y + 34.0f * scale,
        10.0f * scale,
        subtitle,
        theme::text_muted);
}

void status_chip(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float scale,
    std::string_view label,
    std::string_view value,
    Color state)
{
    renderer.rect(
        x, y, width, 48.0f * scale,
        Color{0.027f, 0.050f, 0.085f, 0.92f},
        12.0f * scale);

    renderer.rect(
        x + 12.0f * scale,
        y + 18.0f * scale,
        8.0f * scale,
        8.0f * scale,
        state,
        4.0f * scale);

    renderer.text(
        x + 28.0f * scale,
        y + 7.0f * scale,
        9.5f * scale,
        label,
        theme::text_muted);

    renderer.text(
        x + 28.0f * scale,
        y + 25.0f * scale,
        12.5f * scale,
        value,
        theme::text);
}

void render_world_preview(
    UiRenderer& renderer,
    float x,
    float y,
    float width,
    float height,
    float scale,
    std::string_view grid_status,
    Color grid_status_color,
    const nexverse::protocol::LoginSession* session)
{
    renderer.rect(
        x, y, width, height,
        Color{0.025f, 0.045f, 0.085f, 1.0f},
        14.0f * scale);

    const std::array<Color, 7> sky{
        Color{0.030f, 0.055f, 0.105f, 1.0f},
        Color{0.036f, 0.075f, 0.135f, 1.0f},
        Color{0.045f, 0.095f, 0.160f, 1.0f},
        Color{0.052f, 0.110f, 0.180f, 1.0f},
        Color{0.065f, 0.105f, 0.185f, 1.0f},
        Color{0.075f, 0.085f, 0.165f, 1.0f},
        Color{0.064f, 0.070f, 0.140f, 1.0f}
    };

    const float header_height = 66.0f * scale;
    const float footer_height = 62.0f * scale;
    const float scene_height = std::max(160.0f * scale, height - header_height - footer_height);
    const float band_height = scene_height / static_cast<float>(sky.size());

    for (std::size_t i = 0; i < sky.size(); ++i)
    {
        renderer.rect(
            x + 1.0f,
            y + header_height + static_cast<float>(i) * band_height,
            width - 2.0f,
            band_height + 1.0f,
            sky[i],
            0.0f);
    }

    renderer.rect(
        x + 1.0f, y + 1.0f, width - 2.0f, header_height,
        Color{0.020f, 0.038f, 0.070f, 0.98f},
        14.0f * scale);

    renderer.text(
        x + 22.0f * scale,
        y + 13.0f * scale,
        10.0f * scale,
        "WELT-VORSCHAU",
        theme::cyan);

    renderer.text(
        x + 22.0f * scale,
        y + 32.0f * scale,
        16.0f * scale,
        "NexVerse verbindet sich mit der OpenSim-Welt",
        theme::text);

    renderer.text(
        x + 22.0f * scale,
        y + 54.0f * scale,
        9.0f * scale,
        session != nullptr
            ? "Login erfolgreich. LLUDP-Weltverbindung folgt in Phase 3."
            : "Die echte Region ersetzt diese Vorschau nach erfolgreichem Login.",
        theme::text_muted);

    const float horizon = y + header_height + scene_height * 0.66f;

    renderer.rect(
        x + width * 0.68f,
        y + header_height + 38.0f * scale,
        86.0f * scale,
        86.0f * scale,
        with_alpha(theme::violet, 0.16f),
        43.0f * scale);

    renderer.rect(
        x + width * 0.72f,
        y + header_height + 58.0f * scale,
        52.0f * scale,
        52.0f * scale,
        with_alpha(theme::cyan, 0.30f),
        26.0f * scale);

    const std::array<float, 11> building_widths{42, 56, 34, 72, 50, 62, 38, 68, 45, 58, 40};
    float city_x = x + 34.0f * scale;

    for (std::size_t i = 0; i < building_widths.size(); ++i)
    {
        const float building_width = building_widths[i] * scale;
        const float building_height =
            (68.0f + std::fmod(static_cast<float>((i + 4U) * 41U), 155.0f)) * scale;

        if (city_x + building_width > x + width - 28.0f * scale)
        {
            break;
        }

        renderer.rect(
            city_x,
            horizon - building_height,
            building_width,
            building_height,
            Color{
                0.025f + static_cast<float>(i % 3U) * 0.008f,
                0.040f,
                0.082f + static_cast<float>(i % 4U) * 0.014f,
                0.98f},
            3.0f * scale);

        renderer.rect(
            city_x + 6.0f * scale,
            horizon - building_height + 14.0f * scale,
            building_width - 12.0f * scale,
            2.0f * scale,
            with_alpha((i % 2U == 0U) ? theme::cyan : theme::violet, 0.72f),
            1.0f * scale);

        city_x += building_width + 9.0f * scale;
    }

    renderer.rect(
        x + 1.0f,
        horizon,
        width - 2.0f,
        y + header_height + scene_height - horizon,
        Color{0.012f, 0.026f, 0.050f, 1.0f},
        0.0f);

    for (int i = 0; i < 5; ++i)
    {
        const float px = x + (120.0f + static_cast<float>(i) * 150.0f) * scale;
        const float pw = 92.0f * scale;
        if (px + pw > x + width - 30.0f * scale) break;

        renderer.rect(
            px,
            horizon + 30.0f * scale,
            pw,
            8.0f * scale,
            with_alpha(theme::cyan, 0.10f + static_cast<float>(i % 2) * 0.06f),
            4.0f * scale);

        renderer.rect(
            px + 18.0f * scale,
            horizon + 54.0f * scale,
            56.0f * scale,
            3.0f * scale,
            with_alpha(theme::violet, 0.22f),
            1.5f * scale);
    }

    renderer.rect(
        x + 1.0f,
        horizon,
        width - 2.0f,
        2.0f * scale,
        with_alpha(theme::cyan, 0.55f),
        0.0f);

    const float badge_width = 174.0f * scale;
    renderer.rect(
        x + width - badge_width - 18.0f * scale,
        y + header_height + 18.0f * scale,
        badge_width,
        76.0f * scale,
        Color{0.016f, 0.032f, 0.060f, 0.90f},
        12.0f * scale);

    renderer.text(
        x + width - badge_width,
        y + header_height + 31.0f * scale,
        9.0f * scale,
        "GRID DISCOVERY",
        theme::text_muted);

    renderer.text(
        x + width - badge_width,
        y + header_height + 51.0f * scale,
        13.0f * scale,
        grid_status,
        grid_status_color);

    renderer.rect(
        x + 1.0f,
        y + height - footer_height - 1.0f,
        width - 2.0f,
        footer_height,
        Color{0.016f, 0.030f, 0.055f, 0.98f},
        0.0f);

    renderer.text(
        x + 20.0f * scale,
        y + height - footer_height + 10.0f * scale,
        9.5f * scale,
        "WORLD STREAM",
        theme::text_muted);

    renderer.text(
        x + 20.0f * scale,
        y + height - footer_height + 29.0f * scale,
        12.0f * scale,
        session != nullptr
            ? "Login erfolgreich - bereit fuer LLUDP / Phase 3"
            : "Noch nicht verbunden - Anmeldung erforderlich",
        theme::text);

    renderer.rect(
        x + width - 146.0f * scale,
        y + height - footer_height + 13.0f * scale,
        136.0f * scale,
        34.0f * scale,
        with_alpha(theme::blue, 0.15f),
        9.0f * scale);

    renderer.text(
        x + width - 124.0f * scale,
        y + height - footer_height + 20.0f * scale,
        10.0f * scale,
        session != nullptr ? "SESSION" : "VORSCHAU",
        session != nullptr ? theme::success : theme::cyan);
}

void render_loading_screen(
    UiRenderer& renderer,
    int width,
    int height,
    std::string_view grid_name,
    const nexverse::protocol::CircuitStatus& circuit,
    const SimulationLoadStats& load_stats,
    const CapsRuntimeState& caps,
    unsigned int welcome_texture)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1920.0f, h / 1080.0f),
        0.78f,
        1.75f);

    renderer.resize(width, height);

    if (welcome_texture != 0U)
    {
        renderer.image(welcome_texture, 0.0f, 0.0f, w, h, 1.0f);
        renderer.rect(
            0.0f, 0.0f, w, h,
            Color{0.004f, 0.010f, 0.024f, 0.72f},
            0.0f);
    }
    else
    {
        renderer.rect(
            0.0f, 0.0f, w, h,
            Color{0.008f, 0.018f, 0.035f, 1.0f},
            0.0f);
    }

    const float panel_w = std::min(720.0f * scale, w - 64.0f * scale);
    const float panel_h = 336.0f * scale;
    const float panel_x = (w - panel_w) * 0.5f;
    const float panel_y = (h - panel_h) * 0.5f;

    renderer.rect(
        panel_x - 2.0f * scale,
        panel_y - 2.0f * scale,
        panel_w + 4.0f * scale,
        panel_h + 4.0f * scale,
        with_alpha(theme::cyan, 0.18f),
        18.0f * scale);
    renderer.rect(
        panel_x,
        panel_y,
        panel_w,
        panel_h,
        Color{0.010f, 0.020f, 0.032f, 0.96f},
        16.0f * scale);

    renderer.text(
        panel_x + 30.0f * scale,
        panel_y + 26.0f * scale,
        19.0f * scale,
        "OpenGenesisLINK",
        theme::text);
    renderer.text(
        panel_x + 30.0f * scale,
        panel_y + 58.0f * scale,
        10.0f * scale,
        "WELT WIRD GELADEN",
        theme::cyan);

    renderer.text(
        panel_x + 30.0f * scale,
        panel_y + 88.0f * scale,
        12.0f * scale,
        std::string{"Willkommen in "} + std::string{grid_name},
        theme::text_muted);

    const bool udp_ready =
        circuit.state == nexverse::protocol::CircuitState::active;
    const bool caps_ready =
        caps.simulator_features_loaded && caps.event_queue_available;
    const bool terrain_ready = load_stats.terrain_packets > 0U;
    const bool object_ready = load_stats.object_packets > 0U;

    const int completed =
        1 +
        (udp_ready ? 1 : 0) +
        (caps_ready ? 1 : 0) +
        (terrain_ready ? 1 : 0) +
        (object_ready ? 1 : 0);
    const float progress =
        std::clamp(static_cast<float>(completed) / 5.0f, 0.04f, 1.0f);

    const float bar_x = panel_x + 30.0f * scale;
    const float bar_y = panel_y + 126.0f * scale;
    const float bar_w = panel_w - 60.0f * scale;
    const float bar_h = 14.0f * scale;

    renderer.rect(
        bar_x, bar_y, bar_w, bar_h,
        Color{0.06f, 0.08f, 0.10f, 1.0f},
        7.0f * scale);
    renderer.rect(
        bar_x, bar_y, bar_w * progress, bar_h,
        theme::cyan,
        7.0f * scale);

    const auto step = [&](float y, std::string_view label, bool done)
    {
        renderer.rect(
            panel_x + 32.0f * scale,
            y + 4.0f * scale,
            8.0f * scale,
            8.0f * scale,
            done ? theme::success : theme::text_muted,
            4.0f * scale);
        renderer.text(
            panel_x + 50.0f * scale,
            y,
            10.0f * scale,
            label,
            done ? theme::text : theme::text_muted);
    };

    step(panel_y + 162.0f * scale, "Grid-Verbindung hergestellt", true);
    step(panel_y + 186.0f * scale, "Simulator / LLUDP verbunden", udp_ready);
    step(panel_y + 210.0f * scale, "Region-Capabilities geladen", caps_ready);
    step(panel_y + 234.0f * scale, "Terrain wird geladen", terrain_ready);
    step(panel_y + 258.0f * scale, "Objekte und Umgebung werden geladen", object_ready);

    // A capability merely advertises a server endpoint; it is not a
    // working feature or an approved upload/Experience consent action.
    const std::string parity_caps =
        std::string{"Startregion: Outfit-Vorschau "} +
        (caps.firestorm_caps.inventory_thumbnail_upload ? "CAP" : "-") +
        " | Experiences " +
        (caps.firestorm_caps.experience_owned ? "CAP" : "-") +
        (caps.owned_experiences_read
            ? " | Eigene " + std::to_string(caps.owned_experiences)
            : "") +
        (caps.resident_experiences_read
            ? " | Erlaubt " + std::to_string(caps.allowed_experiences)
            : "");
    renderer.text(panel_x+30.0f*scale,panel_y+298.0f*scale,
                  8.2f*scale,parity_caps,theme::text_muted);

    const std::string version =
        std::string{"v"} + std::string{nexverse::core::version()};
    renderer.text(
        22.0f * scale,
        h - 28.0f * scale,
        9.0f * scale,
        version,
        theme::text_muted);
}

void render_world_hud(
    UiRenderer& renderer,
    int width,
    int height,
    std::string_view grid_name,
    const LoginUiState& login,
    const nexverse::protocol::CircuitStatus& circuit,
    const FrameMetrics& frame_metrics,
    const nexverse::render::RenderDiagnostics& render_diagnostics,
    const nexverse::core::SystemMemoryMetrics& memory_metrics,
    const nexverse::ui::ViewerShellState& shell,
    bool inventory_open,bool outfit_open,bool experiences_open)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));

    // Vertical scaling keeps the world viewport wide on 21:9 and 32:9.
    const float scale = std::clamp(h / 1080.0f, 0.74f, 1.50f);
    const float top_h = 98.0f * scale;
    const float bottom_h = 58.0f * scale;
    const float left_w = 190.0f * scale;
    const float right_w = 268.0f * scale;
    const float margin = 12.0f * scale;

    renderer.resize(width, height);

    // NexVerse top branding bar.
    renderer.rect(
        0.0f, 0.0f, w, top_h,
        Color{0.006f, 0.015f, 0.028f, 0.93f},
        0.0f);
    renderer.rect(
        0.0f, top_h - 1.0f * scale, w, 1.0f * scale,
        with_alpha(theme::cyan, 0.55f),
        0.0f);

    renderer.rect(
        14.0f * scale,
        12.0f * scale,
        42.0f * scale,
        42.0f * scale,
        Color{0.025f, 0.120f, 0.205f, 0.96f},
        10.0f * scale);
    renderer.rect(
        22.0f * scale,
        20.0f * scale,
        26.0f * scale,
        26.0f * scale,
        with_alpha(theme::cyan, 0.52f),
        6.0f * scale);

    renderer.text(
        70.0f * scale,
        13.0f * scale,
        17.0f * scale,
        "OpenGenesisLINK",
        theme::text);
    renderer.text(
        70.0f * scale,
        40.0f * scale,
        8.5f * scale,
        "VIEWER FOR NEXVERSE",
        theme::cyan);

    const float grid_card_x = 280.0f * scale;
    const float grid_card_w = 270.0f * scale;
    renderer.rect(
        grid_card_x,
        11.0f * scale,
        grid_card_w,
        44.0f * scale,
        Color{0.020f, 0.040f, 0.068f, 0.82f},
        9.0f * scale);
    renderer.text(
        grid_card_x + 14.0f * scale,
        18.0f * scale,
        8.0f * scale,
        "NEXVERSE GRID",
        theme::text_muted);
    renderer.text(
        grid_card_x + 14.0f * scale,
        35.0f * scale,
        10.5f * scale,
        grid_name,
        theme::text);

    const bool udp_live =
        circuit.state == nexverse::protocol::CircuitState::active;
    const Color live_color = udp_live ? theme::success : theme::warning;

    const float chip_w = 96.0f * scale;
    const float chip_gap = 8.0f * scale;
    float chip_x = w - margin - chip_w;
    status_chip(
        renderer,
        chip_x,
        10.0f * scale,
        chip_w,
        scale,
        "WORLD",
        udp_live ? "UDP LIVE" : "WAIT",
        live_color);
    chip_x -= chip_w + chip_gap;
    status_chip(
        renderer,
        chip_x,
        10.0f * scale,
        chip_w,
        scale,
        "LOGIN",
        "CONNECTED",
        theme::success);
    chip_x -= chip_w + chip_gap;
    status_chip(
        renderer,
        chip_x,
        10.0f * scale,
        chip_w,
        scale,
        "GRID",
        "LIVE",
        theme::success);

    // Compact NexVerse navigation rail. It preserves the original visual
    // language without hiding the world viewport.
    const float rail_y = top_h + margin;
    const float rail_h = h - top_h - bottom_h - margin * 2.0f;
    renderer.rect(
        margin,
        rail_y,
        left_w,
        rail_h,
        Color{0.006f, 0.015f, 0.028f, 0.78f},
        10.0f * scale);

    renderer.text(
        margin + 12.0f * scale,
        rail_y + 12.0f * scale,
        8.0f * scale,
        "NAVIGATION",
        theme::text_muted);

    const std::array<std::pair<std::string_view, std::string_view>, 7> navigation{{
        {"Welt", "Region ansehen"},
        {"Orte", "Teleport-Ziel"},
        {"Freunde", "Noch nicht bereit"},
        {"Gruppen", "Noch nicht bereit"},
        {"Events", "Noch nicht bereit"},
        {"Inventar", "Ordner und Items"},
        {"Marketplace", "Noch nicht bereit"}
    }};

    float nav_y = rail_y + 34.0f * scale;
    for (std::size_t index = 0U; index < navigation.size(); ++index)
    {
        nav_item(
            renderer,
            margin + 6.0f * scale,
            nav_y,
            left_w - 12.0f * scale,
            scale * 0.84f,
            navigation[index].first,
            navigation[index].second,
            (index==0U&&shell.window==nexverse::ui::ViewerWindow::region)||
            (index==1U&&shell.window==nexverse::ui::ViewerWindow::teleport)||
            (index==5U&&inventory_open));
        if(nexverse::ui::navigation_commands[index]==nexverse::ui::ViewerAction::disabled)
        {
            renderer.rect(margin+6.0f*scale,nav_y,
                          left_w-12.0f*scale,47.0f*scale,
                          Color{0.008f,0.018f,0.035f,0.56f},
                          7.0f*scale);
            renderer.text(margin+18.0f*scale,nav_y+31.0f*scale,
                          7.7f*scale,"IN ARBEIT",theme::warning);
        }
        nav_y += 50.0f * scale;
    }

    const std::string session_name =
        login.username.empty() ? "NexVerse" : login.username;
    renderer.rect(
        margin + 6.0f * scale,
        rail_y + rail_h - 68.0f * scale,
        left_w - 12.0f * scale,
        56.0f * scale,
        Color{0.020f, 0.040f, 0.068f, 0.88f},
        9.0f * scale);
    renderer.rect(
        margin + 16.0f * scale,
        rail_y + rail_h - 55.0f * scale,
        30.0f * scale,
        30.0f * scale,
        with_alpha(theme::blue, 0.30f),
        15.0f * scale);
    renderer.text(
        margin + 56.0f * scale,
        rail_y + rail_h - 54.0f * scale,
        10.5f * scale,
        session_name,
        theme::text);
    renderer.text(
        margin + 56.0f * scale,
        rail_y + rail_h - 34.0f * scale,
        8.5f * scale,
        "OpenSim Session aktiv",
        theme::success);

    // Right status cards, matching the original NexVerse dashboard style.
    const float right_x = w - right_w - margin;
    renderer.rect(
        right_x,
        rail_y,
        right_w,
        rail_h,
        Color{0.006f, 0.015f, 0.028f, 0.74f},
        10.0f * scale);

    const float card_x = right_x + 10.0f * scale;
    const float card_w = right_w - 20.0f * scale;

    renderer.rect(
        card_x,
        rail_y + 10.0f * scale,
        card_w,
        134.0f * scale,
        Color{0.018f, 0.036f, 0.062f, 0.90f},
        9.0f * scale);
    renderer.text(
        card_x + 14.0f * scale,
        rail_y + 23.0f * scale,
        12.0f * scale,
        "NexVerse Verbindung",
        theme::text);
    status_row(
        renderer,
        card_x + 14.0f * scale,
        rail_y + 52.0f * scale,
        "Grid",
        "LIVE",
        theme::success);
    status_row(
        renderer,
        card_x + 14.0f * scale,
        rail_y + 76.0f * scale,
        "Login",
        "CONNECTED",
        theme::success);
    status_row(
        renderer,
        card_x + 14.0f * scale,
        rail_y + 100.0f * scale,
        "LLUDP",
        udp_live ? "LIVE" : "WAIT",
        live_color);

    renderer.rect(
        card_x,
        rail_y + 154.0f * scale,
        card_w,
        104.0f * scale,
        Color{0.018f, 0.036f, 0.062f, 0.88f},
        9.0f * scale);
    renderer.text(
        card_x + 14.0f * scale,
        rail_y + 168.0f * scale,
        11.0f * scale,
        "Systemstatus",
        theme::text);

    const auto to_mib = [](std::uint64_t bytes)
    {
        return bytes / (1024U * 1024U);
    };
    const std::uint64_t ram_used =
        memory_metrics.total_bytes >= memory_metrics.available_bytes
            ? memory_metrics.total_bytes - memory_metrics.available_bytes
            : 0U;

    std::string vram = "n/a";
    if (render_diagnostics.vram_total_bytes > 0U)
    {
        const std::uint64_t used =
            render_diagnostics.vram_total_bytes -
            std::min(
                render_diagnostics.vram_total_bytes,
                render_diagnostics.vram_available_bytes);
        vram = std::to_string(to_mib(used)) + " MiB";
    }

    renderer.text(
        card_x + 14.0f * scale,
        rail_y + 194.0f * scale,
        8.5f * scale,
        "Renderer  READY",
        theme::success);
    renderer.text(
        card_x + 14.0f * scale,
        rail_y + 214.0f * scale,
        8.5f * scale,
        "RAM  " + std::to_string(to_mib(ram_used)) + " MiB",
        theme::text_muted);
    renderer.text(
        card_x + 14.0f * scale,
        rail_y + 234.0f * scale,
        8.5f * scale,
        "VRAM  " + vram,
        theme::text_muted);

    const std::string debug_line =
        "FPS " + std::to_string(static_cast<int>(std::round(frame_metrics.fps))) +
        " | Frame " +
        std::to_string(static_cast<int>(std::round(frame_metrics.frame_time_ms))) +
        " ms | Draw " + std::to_string(render_diagnostics.draw_calls) +
        " | Tri " + std::to_string(render_diagnostics.triangles) +
        " | Obj " + std::to_string(render_diagnostics.visible_objects);

    renderer.rect(
        card_x,
        rail_y + rail_h - 58.0f * scale,
        card_w,
        44.0f * scale,
        Color{0.018f, 0.036f, 0.062f, 0.88f},
        9.0f * scale);
    renderer.text(
        card_x + 12.0f * scale,
        rail_y + rail_h - 44.0f * scale,
        8.3f * scale,
        debug_line,
        theme::cyan);
    renderer.text(card_x+12.0f*scale,
                  rail_y+rail_h-24.0f*scale,
                  8.0f*scale,
                  "Erlebnisse [F8]   Outfits [F9]   Inventar [F10]",
                  theme::text_muted);

    // Bottom action bar, visually aligned with the original NexVerse UI.
    renderer.rect(
        0.0f,
        h - bottom_h,
        w,
        bottom_h,
        Color{0.006f, 0.015f, 0.028f, 0.91f},
        0.0f);
    renderer.rect(
        0.0f,
        h - bottom_h,
        w,
        1.0f * scale,
        with_alpha(theme::cyan, 0.45f),
        0.0f);

    float action_x = 214.0f * scale;
    for(std::size_t index=0U;
        index<nexverse::ui::action_labels.size();++index)
    {
        const auto action=nexverse::ui::action_labels[index];
        const auto command=nexverse::ui::action_commands[index];
        const bool selected=
            (command==nexverse::ui::ViewerAction::inventory&&inventory_open)||
            (command==nexverse::ui::ViewerAction::outfits&&outfit_open)||
            (command==nexverse::ui::ViewerAction::experiences&&experiences_open)||
            (shell.window!=nexverse::ui::ViewerWindow::none&&
             nexverse::ui::window_for_action(command)==shell.window);
        const float action_w=96.0f*scale;

        renderer.rect(
            action_x,
            h - bottom_h + 11.0f * scale,
            action_w,
            34.0f * scale,
            selected?Color{0.055f,0.170f,0.235f,0.96f}
                    :Color{0.020f, 0.045f, 0.080f, 0.90f},
            8.0f * scale);
        renderer.text(
            action_x + 14.0f * scale,
            h - bottom_h + 20.0f * scale,
            9.0f * scale,
            action,
            selected?theme::cyan:theme::text_muted);
        action_x += 104.0f * scale;
    }
}

// Firestorm Experiences: real read-only floater, available only after a
// live simulator scene is ready. No accidental Purchase/Allow/Block actions.
// The inventory skeleton is populated by the authenticated OpenSim
// login reply. Thumbnail images require a later region inventory fetch,
// not a made-up placeholder, and outfit editing remains disabled.
void render_outfit_gallery(
    UiRenderer& renderer,int width,int height,
    const std::vector<nexverse::protocol::OutfitGalleryFolder>& folders,
    const CapsRuntimeState& caps,
    const std::map<std::string,std::pair<std::string,unsigned int>,std::less<>>&
        image_textures,
    std::size_t selected_outfit)
{
    const float w=static_cast<float>(std::max(width,1));
    const float h=static_cast<float>(std::max(height,1));
    const float scale=std::clamp(h/1080.0f,0.75f,1.5f);
    const float pw=std::min(790.0f*scale,w-32.0f);
    const float ph=500.0f*scale;
    const float x=(w-pw)*0.5f, y=std::max(30.0f,(h-ph)*0.5f);
    renderer.rect(0,0,w,h,Color{0,0.009f,0.025f,0.57f},0.0f);
    renderer.rect(x,y,pw,ph,Color{0.014f,0.032f,0.057f,0.98f},11.0f*scale);
    renderer.text(x+22.0f*scale,y+24.0f*scale,
                  17.0f*scale,"Outfit-Galerie",theme::text);
    renderer.rect(x+pw-55.0f*scale,y+10.0f*scale,
                  45.0f*scale,35.0f*scale,
                  Color{0.16f,0.050f,0.087f,1.0f},8.0f*scale);
    renderer.text(x+pw-40.0f*scale,y+17.0f*scale,
                  12.0f*scale,"X",theme::text);
    renderer.text(x+22.0f*scale,y+59.0f*scale,
                  9.3f*scale,"Firestorm-Inventarordner (nur lesend)",
                  theme::cyan);
    renderer.text(x+22.0f*scale,y+86.0f*scale,
                  9.5f*scale,folders.empty()?
                  "Keine Outfit-Ordner im Login-Inventar":
                  std::to_string(folders.size())+" Outfit-Ordner erkannt",
                  theme::text_muted);
    for(std::size_t i=0U;i<std::min(folders.size(),std::size_t{10U});++i)
    {
        const float yy=y+(115.0f+static_cast<float>(i)*30.0f)*scale;
        renderer.rect(x+22.0f*scale,yy,425.0f*scale,
                      27.0f*scale,
                      i==selected_outfit?Color{0.055f,0.19f,0.26f,0.98f}
                                        :Color{0.021f,0.050f,0.082f,0.98f},
                      5.0f*scale);
        renderer.text(x+31.0f*scale,yy+7.0f*scale,
                      9.5f*scale,folders[i].name,theme::text);
        if(const auto image=image_textures.find(folders[i].id);
           image!=image_textures.end() &&
           caps.outfit_thumbnail_assets.contains(folders[i].id) &&
           caps.outfit_thumbnail_assets.at(folders[i].id)==image->second.first)
            renderer.image(image->second.second,
                           x+421.0f*scale,yy+1.5f*scale,
                           24.0f*scale,24.0f*scale,1.0f);
        else
            renderer.text(x+347.0f*scale,yy+7.0f*scale,
                          7.5f*scale,"Vorschau -",theme::text_muted);
    }
    if(selected_outfit<folders.size())
    {
        const auto& selected=folders[selected_outfit];
        renderer.text(x+477.0f*scale,y+116.0f*scale,10.2f*scale,
            "Outfit-Inhalt",theme::cyan);
        renderer.text(x+477.0f*scale,y+146.0f*scale,8.6f*scale,
            selected.name.substr(0U,35U),theme::text);
        const auto it=caps.outfit_folder_items.find(selected.id);
        if(it!=caps.outfit_folder_items.end())
        {
            renderer.text(x+477.0f*scale,y+170.0f*scale,8.1f*scale,
                std::to_string(it->second.size())+" Inventargegenstaende",
                theme::text_muted);
            for(std::size_t i=0U;
                i<std::min(it->second.size(),std::size_t{6U});++i)
                renderer.text(x+477.0f*scale,
                    y+(198.0f+static_cast<float>(i)*29.0f)*scale,
                    8.1f*scale,
                    std::to_string(i+1U)+". "+
                    it->second[i].name.substr(0U,33U),
                    theme::text_muted);
        }
        else
            renderer.text(x+477.0f*scale,y+177.0f*scale,
                8.4f*scale,"Inhalt noch nicht geladen",theme::text_muted);
        const auto cof=caps.outfit_folder_items.find(
            caps.current_outfit_folder);
        const std::vector<nexverse::protocol::OutfitInventoryItem> unresolved;
        const auto& resolved=caps.resolved_outfit_folder==selected.id
            ?caps.resolved_outfit_items:unresolved;
        const auto preview=nexverse::protocol::check_firestorm_outfit_preflight(
            selected.id,caps.current_outfit_folder,
            it==caps.outfit_folder_items.end()?nullptr:&it->second,
            cof==caps.outfit_folder_items.end()?nullptr:&cof->second,
            resolved);
        const std::string readiness=
            preview.readiness==nexverse::protocol::OutfitWearReadiness::no_inventory
              ?"Outfit-Inventar noch nicht geladen"
            :preview.readiness==nexverse::protocol::OutfitWearReadiness::empty_outfit
              ?"Outfit-Ordner ist leer"
            :preview.readiness==nexverse::protocol::OutfitWearReadiness::missing_cof
              ?"COF ist noch nicht geladen"
            :preview.readiness==nexverse::protocol::OutfitWearReadiness::unresolved_links
              ?"Link-Ziele/Anhaenge nicht aufgeloest"
            :"Wearables/Avatar-Appearance noch nicht bereit";
        renderer.text(x+477.0f*scale,y+390.0f*scale,
                      8.0f*scale,readiness,theme::warning);
        if(caps.resolved_outfit_folder==selected.id)
            renderer.text(x+477.0f*scale,y+370.0f*scale,
                7.8f*scale,
                std::to_string(caps.resolved_outfit_items.size())+
                " Items durch FetchInventory2 geprueft",
                theme::text_muted);
    }
    if(!caps.current_outfit_folder.empty())
    {
        const auto found=caps.outfit_folder_items.find(
            caps.current_outfit_folder);
        const std::string cof_status=found==caps.outfit_folder_items.end()
            ?"COF: Inhalt noch nicht geladen"
            :"COF: "+std::to_string(found->second.size())+
              " aktuell verknuepfte Eintraege";
        renderer.text(x+477.0f*scale,y+419.0f*scale,
                      8.0f*scale,cof_status,theme::text_muted);
    }
    renderer.text(x+22.0f*scale,y+ph-44.0f*scale,
                  8.4f*scale,
                  caps.outfit_inventory_read
                    ? "Inventar gelesen; Tragen/Bearbeiten noch gesperrt"
                    : "Inventar-CAP-Antwort nicht verfuegbar",
                  theme::warning);
    renderer.text(x+22.0f*scale,y+ph-24.0f*scale,
                  8.3f*scale,"F9 / ESC schließen",theme::text_muted);
}

void render_inventory_tree(
    UiRenderer& renderer,int width,int height,
    const nexverse::protocol::InventoryTreeSnapshot& tree,
    const std::map<std::string,nexverse::protocol::InventoryFolderContents,
                   std::less<>>& contents,
    std::string_view current_parent,std::size_t selected_index,
    std::size_t item_index,
    const std::optional<nexverse::protocol::OutfitInventoryItem>& detail,
    std::string_view detail_folder,std::string_view detail_id,
    std::string_view detail_status)
{
    const float w=static_cast<float>(std::max(width,1));
    const float h=static_cast<float>(std::max(height,1));
    const float scale=std::clamp(h/1080.0f,0.75f,1.5f);
    const float pw=std::min(690.0f*scale,w-32.0f);
    const float ph=520.0f*scale;
    const float x=(w-pw)*0.5f,y=std::max(30.0f,(h-ph)*0.5f);
    renderer.rect(0,0,w,h,Color{0,0.009f,0.025f,0.57f},0.0f);
    renderer.rect(x,y,pw,ph,Color{0.014f,0.032f,0.057f,0.99f},11.0f*scale);
    renderer.text(x+23.0f*scale,y+25.0f*scale,
        18.0f*scale,"Inventar / Inventory",theme::text);
    renderer.rect(x+pw-55.0f*scale,y+10.0f*scale,
                  45.0f*scale,35.0f*scale,
                  Color{0.16f,0.050f,0.087f,1.0f},8.0f*scale);
    renderer.text(x+pw-40.0f*scale,y+17.0f*scale,
                  12.0f*scale,"X",theme::text);
    renderer.text(x+23.0f*scale,y+60.0f*scale,
        9.0f*scale,"Firestorm-Struktur aus authentifiziertem Login und CAPS",
        theme::cyan);
    const auto parent=tree.folders.find(current_parent);
    renderer.text(x+23.0f*scale,y+89.0f*scale,10.0f*scale,
        parent==tree.folders.end()?"Inventar-Wurzel":
            parent->second.name.substr(0U,52U),theme::text);
    std::vector<const nexverse::protocol::InventoryFolderNode*> children;
    for(const auto& [id,node]:tree.folders)
    {
        (void)id;
        if(node.parent_id==current_parent)
            children.push_back(&node);
    }
    const auto data=contents.find(std::string{current_parent});
    renderer.text(x+23.0f*scale,y+114.0f*scale,8.4f*scale,
        std::to_string(children.size())+" Ordner / "+
        (data==contents.end()?"Gegenstaende nicht geladen":
         std::to_string(data->second.items.size())+" Gegenstaende (v"+
         std::to_string(data->second.version)+")"),theme::text_muted);
    const float divider_x=x+pw*0.51f;
    renderer.rect(divider_x,y+139.0f*scale,1.0f*scale,
        294.0f*scale,with_alpha(theme::cyan,0.22f),0.0f);
    renderer.text(divider_x+12.0f*scale,y+127.0f*scale,
        8.5f*scale,"Inhalt dieses Ordners",theme::cyan);
    const auto offset=selected_index>10U?selected_index-10U:0U;
    for(std::size_t i=offset;i<std::min(children.size(),offset+11U);++i)
    {
        const float yy=y+(145.0f+static_cast<float>(i-offset)*27.0f)*scale;
        renderer.rect(x+23.0f*scale,yy,pw*0.51f-30.0f*scale,25.0f*scale,
            i==selected_index?Color{0.07f,0.20f,0.29f,0.96f}:
                              Color{0.023f,0.052f,0.078f,0.96f},
            5.0f*scale);
        renderer.text(x+33.0f*scale,yy+5.0f*scale,9.2f*scale,
            children[i]->name.substr(0U,29U),theme::text);

    }
    if(data!=contents.end())
    {
        const auto& items=data->second.items;
        for(std::size_t i=0U;i<std::min(items.size(),std::size_t{10U});++i)
        {
            const float yy=y+(145.0f+static_cast<float>(i)*27.0f)*scale;
            if(i==item_index)
                renderer.rect(divider_x+9.0f*scale,yy,
                    pw*0.48f-16.0f*scale,25.0f*scale,
                    Color{0.07f,0.20f,0.29f,0.96f},5.0f*scale);
            renderer.text(divider_x+13.0f*scale,yy+5.0f*scale,9.0f*scale,
                items[i].name.substr(0U,30U),theme::text_muted);
        }
        if(items.size()>10U)
            renderer.text(divider_x+13.0f*scale,y+418.0f*scale,
                8.0f*scale,"Weitere Gegenstaende vorhanden",theme::cyan);
        if(item_index<items.size()&&
           detail_folder==current_parent &&
           detail_id==items[item_index].item_id)
        {
            const std::string line=detail?
                "Itemdetails: "+detail->name.substr(0U,30U):
                std::string{detail_status};
            renderer.text(x+23.0f*scale,y+435.0f*scale,
                8.5f*scale,line,theme::cyan);
        }
    }
    const std::array<std::string_view,3U> buttons{
        "Ordner oeffnen","Zurueck","Aktualisieren"};
    for(std::size_t i=0U;i<buttons.size();++i)
    {
        const float bx=x+(23.0f+145.0f*static_cast<float>(i))*scale;
        renderer.rect(bx,y+ph-72.0f*scale,135.0f*scale,35.0f*scale,
                      Color{0.030f,0.092f,0.155f,0.95f},6.0f*scale);
        renderer.text(bx+9.0f*scale,y+ph-60.0f*scale,8.8f*scale,
                      buttons[i],theme::text);
    }
    renderer.text(x+23.0f*scale,y+ph-24.0f*scale,8.0f*scale,
        "F10 / ESC schliessen | J/K Items | I Details | Read-only",
        theme::text_muted);
}

void render_experience_floater(
    UiRenderer& renderer,int width,int height,
    const nexverse::protocol::ExperienceUiState& ui,
    const CapsRuntimeState& caps,
    const nexverse::protocol::ExperiencePermissionConsent& permission_consent)
{
    if(!ui.open)return;
    const float w=static_cast<float>(std::max(width,1));
    const float h=static_cast<float>(std::max(height,1));
    const float scale=std::clamp(h/1080.0f,0.75f,1.5f);
    const float pw=std::min(670.0f*scale,w-32.0f);
    const float ph=520.0f*scale;
    const float x=(w-pw)*0.5f;
    const float y=std::max(35.0f,(h-ph)*0.5f);
    renderer.rect(0.0f,0.0f,w,h,Color{0.0f,0.009f,0.025f,0.52f},0.0f);
    renderer.rect(x-2.0f*scale,y-2.0f*scale,
                  pw+4.0f*scale,ph+4.0f*scale,
                  with_alpha(theme::cyan,0.27f),13.0f*scale);
    renderer.rect(x,y,pw,ph,Color{0.013f,0.030f,0.053f,0.98f},
                  12.0f*scale);
    renderer.text(x+22.0f*scale,y+22.0f*scale,
                  18.0f*scale,"Erlebnisse / Experiences",theme::text);
    renderer.text(x+22.0f*scale,y+53.0f*scale,
                  9.0f*scale,"Firestorm-kompatible Ansicht (nur lesend)",
                  theme::cyan);
    renderer.rect(x+pw-55.0f*scale,y+10.0f*scale,
                  45.0f*scale,35.0f*scale,
                  Color{0.16f,0.050f,0.087f,1.0f},8.0f*scale);
    renderer.text(x+pw-40.0f*scale,y+17.0f*scale,
                  12.0f*scale,"X",theme::text);
    const std::array<std::string_view,3U> tabs{
        "1 Eigene","2 Erlaubt","3 Blockiert"};
    const auto selected=static_cast<std::size_t>(ui.tab);
    for(std::size_t i=0U;i<tabs.size();++i)
    {
        const auto xx=x+(20.0f+static_cast<float>(i)*203.0f)*scale;
        renderer.rect(xx,y+80.0f*scale,194.0f*scale,40.0f*scale,
                      i==selected ? Color{0.06f,0.21f,0.30f,1.0f}
                                  : Color{0.027f,0.069f,0.11f,1.0f},
                      6.0f*scale);
        renderer.text(xx+14.0f*scale,y+91.0f*scale,11.0f*scale,
                      tabs[i],i==selected?theme::text:theme::text_muted);
    }
    const bool ready=selected==0U ? caps.owned_experiences_read
                                 : caps.resident_experiences_read;
    const bool advertised=selected==0U ?
        caps.firestorm_caps.experience_owned:
        caps.firestorm_caps.experience_lists;
    const auto& ids=selected==0U ? caps.owned_ids :
                    selected==1U ? caps.allowed_ids : caps.blocked_ids;
    const std::string state=!advertised
        ? "Vom aktuellen Grid nicht angeboten"
        : !ready ? "Serverantwort noch nicht verfügbar"
                 : ids.empty() ? "Keine Einträge" :
                     std::to_string(ids.size())+" Einträge vom Simulator";
    renderer.text(x+25.0f*scale,y+145.0f*scale,10.0f*scale,
                  state,ready?theme::text:theme::warning);
    if(ready)
    {
        const auto active=std::min(ui.selected_index,
                                   ids.empty()?0U:ids.size()-1U);
        const auto offset=active>=6U?active-5U:0U;
        for(std::size_t i=offset;i<std::min(ids.size(),offset+6U);++i)
        {
            const float yy=(174.0f+static_cast<float>(i-offset)*31.0f)*scale;
            renderer.rect(x+22.0f*scale,y+yy,
                          pw-44.0f*scale,28.0f*scale,
                          i==active?Color{0.060f,0.21f,0.29f,0.96f}
                                   :Color{0.020f,0.051f,0.083f,0.94f},
                          5.0f*scale);
            renderer.text(x+32.0f*scale,y+yy+7.0f*scale,
                          9.2f*scale,
                          std::to_string(i+1U)+". "+
                          (caps.experience_names.contains(ids[i])
                              ? caps.experience_names.at(ids[i])
                              : ("ID "+ids[i].substr(0U,8U)+
                                 " (Name nicht geladen)")),
                          theme::text_muted);
        }
        if(!ids.empty())
        {
            if(const auto found=caps.experience_profiles.find(ids[active]);
               found!=caps.experience_profiles.end())
            {
                const auto& profile=found->second;
                const std::string maturity=profile.maturity==42?"Adult":
                    profile.maturity==21?"Moderate":"General";
                renderer.text(x+27.0f*scale,y+372.0f*scale,9.5f*scale,
                    profile.name+" | "+maturity+
                    (profile.enabled?" | Aktiv":" | Deaktiviert"),
                    theme::cyan);
                renderer.text(x+27.0f*scale,y+400.0f*scale,8.5f*scale,
                    profile.description.substr(0U,90U),
                    theme::text_muted);
            }
            else
                renderer.text(x+27.0f*scale,y+380.0f*scale,9.0f*scale,
                    "Profildetails vom Simulator nicht verfuegbar",
                    theme::text_muted);
        }
    }
    const std::string action_note=permission_consent.pending()
        ? (permission_consent.pending()->decision==
            nexverse::protocol::ExperiencePermissionDecision::allow
              ?"Erlauben: ENTER bestaetigen, ESC abbrechen"
              :permission_consent.pending()->decision==
                nexverse::protocol::ExperiencePermissionDecision::block
              ?"Blockieren: ENTER bestaetigen, ESC abbrechen"
              :"Vergessen: ENTER bestaetigen, ESC abbrechen")
        :caps.firestorm_caps.experience_permissions
          ?"A Erlauben | B Blockieren | X Vergessen (ENTER bestaetigen)"
          :"Keine Berechtigungs-CAP | Nur Anzeige";
    renderer.text(x+22.0f*scale,y+ph-63.0f*scale,
                  8.6f*scale,action_note,theme::warning);
    renderer.text(x+22.0f*scale,y+ph-43.0f*scale,
                  8.0f*scale,
                  caps.experience_permission_status.empty()
                      ?"Kaufen und Experience-Editor: gesperrt"
                      :caps.experience_permission_status,
                  theme::text_muted);
    renderer.text(x+22.0f*scale,y+ph-25.0f*scale,
                  8.0f*scale,"F8 schliessen | 1-3 Reiter | ESC zurueck",
                  theme::text_muted);
}

void render_shell(
    UiRenderer& renderer,
    int width,
    int height,
    double mouse_x,
    double mouse_y,
    std::string_view grid_name,
    std::string_view grid_status,
    Color grid_status_color,
    const LoginUiState& login,
    const nexverse::protocol::LoginSession* session,
    const nexverse::protocol::CircuitStatus& circuit,
    const SimulationLoadStats& load_stats,
    const CapsRuntimeState& caps,
    const FrameMetrics& frame_metrics,
    const nexverse::render::RenderDiagnostics& render_diagnostics,
    const nexverse::core::SystemMemoryMetrics& memory_metrics,
    bool simulation_ready,
    unsigned int welcome_texture,
    const nexverse::ui::ViewerShellState& shell,
    bool inventory_open,bool outfit_open,bool experiences_open)
{
    const float w = static_cast<float>(std::max(width, 1));
    const float h = static_cast<float>(std::max(height, 1));
    const float scale = std::clamp(
        std::min(w / 1536.0f, h / 960.0f),
        0.72f,
        1.55f);

    const Color login_color = login.authenticated
        ? theme::success
        : login.busy
            ? theme::cyan
            : login.status == "ERROR"
                ? theme::magenta
                : theme::warning;

    renderer.resize(width, height);

    if (simulation_ready)
    {
        render_world_hud(
            renderer,
            width,
            height,
            grid_name,
            login,
            circuit,
            frame_metrics,
            render_diagnostics,
            memory_metrics,shell,inventory_open,outfit_open,experiences_open);
        return;
    }

    if (login.authenticated)
    {
        render_loading_screen(
            renderer,
            width,
            height,
            grid_name,
            circuit,
            load_stats,
            caps,
            welcome_texture);
        return;
    }

    renderer.rect(
        0.0f,
        0.0f,
        w,
        h,
        Color{0.008f, 0.018f, 0.035f, 1.0f},
        0.0f);

    const float top_h = 86.0f * scale;
    const float left_w = 252.0f * scale;
    const float right_w = 354.0f * scale;
    const float bottom_h = 106.0f * scale;
    const float gap = 10.0f * scale;

    renderer.rect(
        0.0f, 0.0f, w, top_h,
        Color{0.012f, 0.030f, 0.058f, 0.99f},
        0.0f);
    renderer.rect(
        0.0f, top_h - 1.0f, w, 1.0f,
        with_alpha(theme::cyan, 0.24f),
        0.0f);

    renderer.rect(
        18.0f * scale,
        14.0f * scale,
        56.0f * scale,
        56.0f * scale,
        Color{0.025f, 0.120f, 0.235f, 1.0f},
        14.0f * scale);
    renderer.rect(
        27.0f * scale,
        23.0f * scale,
        38.0f * scale,
        38.0f * scale,
        with_alpha(theme::cyan, 0.24f),
        9.0f * scale);

    renderer.text(
        88.0f * scale,
        15.0f * scale,
        24.0f * scale,
        "OpenGenesisLINK",
        theme::text);
    renderer.text(
        89.0f * scale,
        48.0f * scale,
        9.0f * scale,
        "VIEWER FOR NEXVERSE",
        theme::cyan);

    const float location_x = 346.0f * scale;
    const float location_w = 360.0f * scale;
    renderer.rect(
        location_x,
        14.0f * scale,
        location_w,
        54.0f * scale,
        Color{0.026f, 0.052f, 0.090f, 0.95f},
        12.0f * scale);
    renderer.text(
        location_x + 18.0f * scale,
        18.0f * scale,
        10.0f * scale,
        "NEXVERSE GRID",
        theme::text_muted);
    renderer.text(
        location_x + 18.0f * scale,
        36.0f * scale,
        13.0f * scale,
        grid_name,
        theme::text);

    const float chip_w = 122.0f * scale;
    const float chip_gap = 8.0f * scale;
    const float chips_total = chip_w * 3.0f + chip_gap * 2.0f;
    const float chip_start = std::max(location_x + location_w + 16.0f * scale, w - chips_total - 26.0f * scale);

    status_chip(renderer, chip_start, 14.0f * scale, chip_w, scale, "GRID", grid_status, grid_status_color);
    status_chip(renderer, chip_start + chip_w + chip_gap, 14.0f * scale, chip_w, scale, "LOGIN", login.status, login_color);
    status_chip(
        renderer,
        chip_start + (chip_w + chip_gap) * 2.0f,
        14.0f * scale,
        chip_w,
        scale,
        "WORLD",
        simulation_ready
            ? circuit_state_label(circuit.state)
            : (login.authenticated ? "LOADING" : "OFFLINE"),
        simulation_ready
            ? circuit_state_color(circuit.state)
            : (login.authenticated ? theme::cyan : theme::text_muted));

    const float content_y = top_h + gap;
    const float content_h = std::max(380.0f * scale, h - top_h - bottom_h - gap * 2.0f);

    if (!simulation_ready)
    {
        if (welcome_texture != 0U)
        {
            renderer.image(
                welcome_texture,
                0.0f,
                content_y,
                w,
                std::max(1.0f, h - content_y - bottom_h),
                1.0f);
            renderer.rect(
                0.0f,
                content_y,
                w,
                std::max(1.0f, h - content_y - bottom_h),
                Color{0.004f, 0.012f, 0.028f, 0.56f},
                0.0f);
        }

        const float panel_w = std::min(760.0f * scale, w - 80.0f * scale);
        const float panel_h = login.authenticated ? 286.0f * scale : 230.0f * scale;
        const float panel_x = (w - panel_w) * 0.5f;
        const float panel_y = top_h + std::max(44.0f * scale, (h - top_h - bottom_h - panel_h) * 0.5f);

        renderer.rect(
            panel_x,
            panel_y,
            panel_w,
            panel_h,
            Color{0.016f, 0.034f, 0.063f, 0.98f},
            16.0f * scale);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 24.0f * scale,
            10.0f * scale,
            login.authenticated ? "SIMULATION" : "NEXVERSE LOGIN",
            theme::cyan);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 52.0f * scale,
            22.0f * scale,
            login.authenticated
                ? "Simulation wird geladen"
                : "Willkommen bei NexVerse",
            theme::text);

        renderer.text(
            panel_x + 28.0f * scale,
            panel_y + 88.0f * scale,
            11.0f * scale,
            login.authenticated
                ? "Die Viewer-Oberflaeche wird erst eingeblendet, wenn Region und WorldModel bereit sind."
                : "Melde dich mit deinem NexVerse-Avatar an.",
            theme::text_muted);

        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 126.0f * scale,
            "Grid",
            grid_status,
            grid_status_color);
        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 154.0f * scale,
            "Login",
            login.status,
            login_color);
        status_row(
            renderer,
            panel_x + 28.0f * scale,
            panel_y + 182.0f * scale,
            "Simulation",
            login.authenticated
                ? (circuit.state == nexverse::protocol::CircuitState::active ? "WORLD DATA" : circuit_state_label(circuit.state))
                : "WARTET",
            login.authenticated
                ? (circuit.state == nexverse::protocol::CircuitState::active ? theme::cyan : circuit_state_color(circuit.state))
                : theme::text_muted);

        if (login.authenticated)
        {
            const std::string load_line =
                "Terrain " + std::to_string(load_stats.terrain_packets) +
                "  |  Objekte " + std::to_string(load_stats.object_packets) +
                "  |  Avatar " + std::to_string(load_stats.avatar_packets) +
                "  |  Sonstige " + std::to_string(load_stats.other_packets);

            renderer.text(
                panel_x + 28.0f * scale,
                panel_y + 222.0f * scale,
                10.0f * scale,
                load_line,
                theme::text_muted);

            const std::string caps_line =
                "CAPS " + caps.status +
                "  |  Endpunkte " + std::to_string(caps.capability_count) +
                "  |  Features " + (caps.simulator_features_loaded ? "OK" : "-") +
                "  |  Events " + std::to_string(caps.event_count);

            renderer.text(
                panel_x + 28.0f * scale,
                panel_y + 246.0f * scale,
                9.5f * scale,
                caps_line,
                caps.status == "ERROR" ? theme::magenta : theme::text_muted);
        }

        const float bottom_y = h - bottom_h;
        renderer.rect(
            0.0f,
            bottom_y,
            w,
            bottom_h,
            Color{0.010f, 0.026f, 0.050f, 0.99f},
            0.0f);
        renderer.rect(
            0.0f,
            bottom_y,
            w,
            1.0f,
            with_alpha(theme::cyan, 0.20f),
            0.0f);

        const float login_x = left_w + 20.0f * scale;
        const float login_y = bottom_y + 20.0f * scale;
        const float field_w = 184.0f * scale;
        const float field_h = 48.0f * scale;

        if (!login.authenticated)
        {
            renderer.text(
                login_x,
                bottom_y + 7.0f * scale,
                8.5f * scale,
                "ANMELDUNG",
                theme::text_muted);

            renderer.rect(
                login_x,
                login_y,
                field_w,
                field_h,
                login.focused == LoginField::username
                    ? Color{0.035f, 0.085f, 0.145f, 0.98f}
                    : Color{0.024f, 0.052f, 0.090f, 0.96f},
                11.0f * scale);
            if (login.focused == LoginField::username)
            {
                renderer.rect(
                    login_x,
                    login_y + field_h - 2.0f * scale,
                    field_w,
                    2.0f * scale,
                    theme::cyan,
                    1.0f * scale);
            }
            renderer.text(
                login_x + 14.0f * scale,
                login_y + 14.0f * scale,
                11.0f * scale,
                login.username.empty() ? "Avatar Name" : login.username,
                login.username.empty() ? theme::text_muted : theme::text);

            const float password_x = login_x + field_w + 8.0f * scale;
            renderer.rect(
                password_x,
                login_y,
                field_w,
                field_h,
                login.focused == LoginField::password
                    ? Color{0.035f, 0.085f, 0.145f, 0.98f}
                    : Color{0.024f, 0.052f, 0.090f, 0.96f},
                11.0f * scale);
            if (login.focused == LoginField::password)
            {
                renderer.rect(
                    password_x,
                    login_y + field_h - 2.0f * scale,
                    field_w,
                    2.0f * scale,
                    theme::cyan,
                    1.0f * scale);
            }

            const std::string password_mask = masked_password(login.password);
            renderer.text(
                password_x + 14.0f * scale,
                login_y + 14.0f * scale,
                11.0f * scale,
                password_mask.empty() ? "Passwort" : password_mask,
                password_mask.empty() ? theme::text_muted : theme::text);

            const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
            const float button_w = 164.0f * scale;
            const bool hover = point_inside(
                mouse_x,
                mouse_y,
                button_x,
                login_y,
                button_w,
                field_h);

            const bool can_submit =
                !login.busy &&
                !login.username.empty() &&
                !login.password.empty() &&
                (grid_status == "LIVE" || grid_status == "CACHE");

            Color button_color = can_submit
                ? theme::blue
                : Color{0.075f, 0.120f, 0.185f, 1.0f};
            if (can_submit && hover)
            {
                button_color = Color{0.080f, 0.690f, 0.960f, 1.0f};
            }

            renderer.rect(
                button_x,
                login_y,
                button_w,
                field_h,
                button_color,
                11.0f * scale);

            const std::string_view button_label = login.busy
                ? "VERBINDE..."
                : "WELT BETRETEN";
            const float button_label_w = renderer.text_width(10.5f * scale, button_label);
            renderer.text(
                button_x + (button_w - button_label_w) * 0.5f,
                login_y + 14.0f * scale,
                10.5f * scale,
                button_label,
                can_submit ? theme::text : theme::text_muted);

            renderer.text(
                login_x,
                bottom_y + 76.0f * scale,
                8.5f * scale,
                login.notice,
                theme::text_muted);
        }
        else
        {
            renderer.text(
                24.0f * scale,
                bottom_y + 20.0f * scale,
                10.0f * scale,
                "ANGEMELDET",
                theme::success);
            renderer.text(
                24.0f * scale,
                bottom_y + 44.0f * scale,
                13.0f * scale,
                login.username,
                theme::text);
            renderer.text(
                24.0f * scale,
                bottom_y + 68.0f * scale,
                9.5f * scale,
                circuit.state == nexverse::protocol::CircuitState::active
                    ? "Verbindung steht. Warte auf Region, WorldModel und Renderer."
                    : circuit.message,
                theme::text_muted);
        }

        const std::string version =
            std::string{"v"} + std::string{nexverse::core::version()};
        const float version_w = renderer.text_width(9.0f * scale, version);
        renderer.text(
            w - version_w - 18.0f * scale,
            bottom_y + bottom_h - 20.0f * scale,
            9.0f * scale,
            version,
            theme::text_muted);
        return;
    }

    renderer.rect(
        0.0f, content_y, left_w, content_h,
        Color{0.011f, 0.025f, 0.048f, 0.98f},
        0.0f);

    renderer.text(
        18.0f * scale,
        content_y + 16.0f * scale,
        9.0f * scale,
        "NAVIGATION",
        theme::text_muted);

    const float nav_x = 8.0f * scale;
    const float nav_w = left_w - 16.0f * scale;
    float nav_y = content_y + 40.0f * scale;

    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Start", "NexVerse Zugang", true);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Welt", "Regionen erkunden", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Orte", "Favoriten und Ziele", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Freunde", "Kontakte und Nähe", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Gruppen", "Communities", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Events", "Veranstaltungen", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Inventar", "Objekte und Assets", false);
    nav_y += 68.0f * scale;
    nav_item(renderer, nav_x, nav_y, nav_w, scale, "Marketplace", "NexVerse entdecken", false);

    const float profile_h = 86.0f * scale;
    const float profile_y = content_y + content_h - profile_h - 12.0f * scale;
    renderer.rect(
        10.0f * scale,
        profile_y,
        left_w - 20.0f * scale,
        profile_h,
        Color{0.020f, 0.050f, 0.086f, 0.94f},
        14.0f * scale);
    renderer.rect(
        22.0f * scale,
        profile_y + 16.0f * scale,
        52.0f * scale,
        52.0f * scale,
        with_alpha(theme::blue, 0.18f),
        26.0f * scale);
    const std::string profile_name = login.authenticated
        ? login.username
        : "Nicht angemeldet";
    renderer.text(
        86.0f * scale,
        profile_y + 16.0f * scale,
        13.5f * scale,
        profile_name,
        theme::text);
    renderer.text(
        86.0f * scale,
        profile_y + 43.0f * scale,
        9.5f * scale,
        login.authenticated
            ? "OpenSim Session aktiv"
            : "NexVerse Login bereit",
        login.authenticated ? theme::success : theme::text_muted);

    const float center_x = left_w + gap;
    const float center_w = std::max(
        420.0f * scale,
        w - left_w - right_w - gap * 3.0f);
    const float right_x = center_x + center_w + gap;

    render_world_preview(
        renderer,
        center_x,
        content_y,
        center_w,
        content_h,
        scale,
        grid_status,
        grid_status_color,
        session);

    renderer.rect(
        right_x,
        content_y,
        right_w,
        content_h,
        Color{0.011f, 0.025f, 0.048f, 0.98f},
        0.0f);

    renderer.rect(
        right_x + 10.0f * scale,
        content_y + 10.0f * scale,
        right_w - 20.0f * scale,
        46.0f * scale,
        Color{0.020f, 0.050f, 0.086f, 0.94f},
        10.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "GRID",
        theme::cyan);
    renderer.text(
        right_x + 104.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "REGION",
        theme::text_muted);
    renderer.text(
        right_x + 190.0f * scale,
        content_y + 23.0f * scale,
        11.5f * scale,
        "ORTE",
        theme::text_muted);

    float panel_y = content_y + 64.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        184.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);

    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 19.0f * scale,
        14.0f * scale,
        "NexVerse Verbindung",
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 45.0f * scale,
        9.0f * scale,
        "Grid",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 45.0f * scale,
        10.5f * scale,
        grid_name,
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 70.0f * scale,
        9.0f * scale,
        "Discovery",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 70.0f * scale,
        10.5f * scale,
        grid_status,
        grid_status_color);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 95.0f * scale,
        9.0f * scale,
        "Login",
        theme::text_muted);
    renderer.text(
        right_x + 96.0f * scale,
        panel_y + 95.0f * scale,
        10.5f * scale,
        login.status,
        login_color);

    renderer.rect(
        right_x + 24.0f * scale,
        panel_y + 136.0f * scale,
        right_w - 48.0f * scale,
        34.0f * scale,
        with_alpha(theme::blue, 0.17f),
        9.0f * scale);
    renderer.text(
        right_x + 38.0f * scale,
        panel_y + 133.0f * scale,
        10.0f * scale,
        "Server-Konfiguration live geladen",
        theme::cyan);

    panel_y += 196.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        144.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 17.0f * scale,
        13.0f * scale,
        "In der Nähe",
        theme::text);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 46.0f * scale,
        10.0f * scale,
        login.authenticated ? "Session erfolgreich aufgebaut" : "Noch keine Weltverbindung",
        login.authenticated ? theme::success : theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 68.0f * scale,
        9.0f * scale,
        session != nullptr
            ? ("Simulator: " + session->simulator_ip + ":" + std::to_string(session->simulator_port))
            : "Avatare und Regionen werden",
        theme::text_muted);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 85.0f * scale,
        9.0f * scale,
        session != nullptr
            ? ("Inventar " + std::to_string(session->inventory_skeleton.size()) +
               " | Freunde " + std::to_string(session->buddies.size()) +
               " | Gesten " + std::to_string(session->gestures.size()))
            : "nach dem Login live geladen.",
        theme::text_muted);

    panel_y += 156.0f * scale;
    renderer.rect(
        right_x + 10.0f * scale,
        panel_y,
        right_w - 20.0f * scale,
        166.0f * scale,
        Color{0.018f, 0.040f, 0.072f, 0.94f},
        12.0f * scale);
    renderer.text(
        right_x + 24.0f * scale,
        panel_y + 17.0f * scale,
        13.0f * scale,
        "Systemstatus",
        theme::text);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 49.0f * scale,
        "Renderer",
        "READY",
        theme::success);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 77.0f * scale,
        "Grid",
        grid_status,
        grid_status_color);
    status_row(
        renderer,
        right_x + 24.0f * scale,
        panel_y + 105.0f * scale,
        "LLUDP",
        circuit_state_label(circuit.state),
        circuit_state_color(circuit.state));

    const float bottom_y = h - bottom_h;
    renderer.rect(
        0.0f,
        bottom_y,
        w,
        bottom_h,
        Color{0.010f, 0.026f, 0.050f, 0.99f},
        0.0f);
    renderer.rect(
        0.0f,
        bottom_y,
        w,
        1.0f,
        with_alpha(theme::cyan, 0.20f),
        0.0f);

    const float login_x = left_w + 20.0f * scale;
    const float login_y = bottom_y + 20.0f * scale;
    const float field_w = 184.0f * scale;
    const float field_h = 48.0f * scale;

    renderer.text(
        login_x,
        bottom_y + 7.0f * scale,
        8.5f * scale,
        "ANMELDUNG",
        theme::text_muted);

    renderer.rect(
        login_x,
        login_y,
        field_w,
        field_h,
        login.focused == LoginField::username
            ? Color{0.035f, 0.085f, 0.145f, 0.98f}
            : Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    if (login.focused == LoginField::username)
    {
        renderer.rect(
            login_x,
            login_y + field_h - 2.0f * scale,
            field_w,
            2.0f * scale,
            theme::cyan,
            1.0f * scale);
    }
    renderer.text(
        login_x + 14.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        login.username.empty() ? "Avatar Name" : login.username,
        login.username.empty() ? theme::text_muted : theme::text);

    const float password_x = login_x + field_w + 8.0f * scale;
    renderer.rect(
        password_x,
        login_y,
        field_w,
        field_h,
        login.focused == LoginField::password
            ? Color{0.035f, 0.085f, 0.145f, 0.98f}
            : Color{0.024f, 0.052f, 0.090f, 0.96f},
        11.0f * scale);
    if (login.focused == LoginField::password)
    {
        renderer.rect(
            password_x,
            login_y + field_h - 2.0f * scale,
            field_w,
            2.0f * scale,
            theme::cyan,
            1.0f * scale);
    }
    const std::string password_mask = masked_password(login.password);
    renderer.text(
        password_x + 14.0f * scale,
        login_y + 14.0f * scale,
        11.0f * scale,
        password_mask.empty() ? "Passwort" : password_mask,
        password_mask.empty() ? theme::text_muted : theme::text);

    const float button_x = login_x + (field_w + 8.0f * scale) * 2.0f;
    const float button_w = 164.0f * scale;
    const bool hover = point_inside(
        mouse_x,
        mouse_y,
        button_x,
        login_y,
        button_w,
        field_h);

    const bool can_submit =
        !login.busy &&
        !login.authenticated &&
        !login.username.empty() &&
        !login.password.empty() &&
        (grid_status == "LIVE" || grid_status == "CACHE");

    Color button_color = can_submit
        ? theme::blue
        : Color{0.075f, 0.120f, 0.185f, 1.0f};
    if (can_submit && hover)
    {
        button_color = Color{0.080f, 0.690f, 0.960f, 1.0f};
    }

    renderer.rect(
        button_x,
        login_y,
        button_w,
        field_h,
        button_color,
        11.0f * scale);

    const std::string_view button_label = login.busy
        ? "VERBINDE..."
        : login.authenticated
            ? "ANGEMELDET"
            : "WELT BETRETEN";
    const float button_label_w = renderer.text_width(10.5f * scale, button_label);
    renderer.text(
        button_x + (button_w - button_label_w) * 0.5f,
        login_y + 14.0f * scale,
        10.5f * scale,
        button_label,
        can_submit || login.authenticated ? theme::text : theme::text_muted);

    renderer.text(
        login_x,
        bottom_y + 76.0f * scale,
        8.5f * scale,
        login.notice,
        login.authenticated ? theme::success : theme::text_muted);

    const float dock_x = button_x + button_w + 20.0f * scale;
    const std::array<std::string_view, 6> dock{
        "Chat", "Sprechen", "Bewegen", "Teleport", "Inventar", "Bauen"};

    float dx = dock_x;
    for (const auto label : dock)
    {
        const float item_w = 80.0f * scale;
        renderer.rect(
            dx,
            login_y,
            item_w,
            field_h,
            Color{0.018f, 0.042f, 0.074f, 0.90f},
            11.0f * scale);
        const float text_w = renderer.text_width(9.5f * scale, label);
        renderer.text(
            dx + (item_w - text_w) * 0.5f,
            login_y + 15.0f * scale,
            9.5f * scale,
            label,
            theme::text_muted);
        dx += item_w + 6.0f * scale;
    }

    const std::string version =
        std::string{"v"} + std::string{nexverse::core::version()};
    const float version_w = renderer.text_width(9.0f * scale, version);
    renderer.text(
        w - version_w - 18.0f * scale,
        bottom_y + bottom_h - 20.0f * scale,
        9.0f * scale,
        version,
        theme::text_muted);

    if (simulation_ready)
    {
        const std::string debug_line =
            "FPS " + std::to_string(static_cast<int>(std::round(frame_metrics.fps))) +
            "  |  Frame " +
            std::to_string(static_cast<int>(std::round(frame_metrics.frame_time_ms))) +
            " ms  |  Draw " +
            std::to_string(render_diagnostics.draw_calls) +
            "  |  Tri " +
            std::to_string(render_diagnostics.triangles);

        const auto to_mib = [](std::uint64_t bytes)
        {
            return bytes / (1024U * 1024U);
        };

        const std::uint64_t ram_used =
            memory_metrics.total_bytes >= memory_metrics.available_bytes
                ? memory_metrics.total_bytes - memory_metrics.available_bytes
                : 0U;

        const std::string vram_text =
            render_diagnostics.vram_total_bytes > 0U
                ? std::to_string(to_mib(
                      render_diagnostics.vram_total_bytes -
                      std::min(
                          render_diagnostics.vram_total_bytes,
                          render_diagnostics.vram_available_bytes))) +
                      "/" +
                      std::to_string(to_mib(render_diagnostics.vram_total_bytes)) +
                      " MiB"
                : "n/a";

        const std::string memory_line =
            "RAM " + std::to_string(to_mib(ram_used)) +
            "/" + std::to_string(to_mib(memory_metrics.total_bytes)) +
            " MiB  |  VRAM " + vram_text +
            "  |  " + render_diagnostics.backend;

        const float debug_w =
            std::max(
                renderer.text_width(9.5f * scale, debug_line),
                renderer.text_width(9.0f * scale, memory_line)) +
            24.0f * scale;
        const float debug_x = w - debug_w - 18.0f * scale;
        const float debug_y = top_h + 12.0f * scale;

        renderer.rect(
            debug_x,
            debug_y,
            debug_w,
            52.0f * scale,
            Color{0.010f, 0.025f, 0.045f, 0.88f},
            8.0f * scale);
        renderer.text(
            debug_x + 12.0f * scale,
            debug_y + 7.0f * scale,
            9.5f * scale,
            debug_line,
            theme::cyan);
        renderer.text(
            debug_x + 12.0f * scale,
            debug_y + 27.0f * scale,
            9.0f * scale,
            memory_line,
            theme::text_muted);
    }
}

void glfw_error_callback(int error, const char* description)
{
    nexverse::core::error(
        "GLFW error " + std::to_string(error) + ": " +
        (description != nullptr ? description : "unknown"));
}
}

namespace nexverse::app
{
int Application::run(bool enable_developer_modules)
{
    core::info(
        std::string{"Starting "} +
        std::string{core::product_name()} +
        " " +
        std::string{core::version()});

    // In-process modules are disabled unless separately enabled and trusted.
    auto module_options = modules::default_module_options();
    if (enable_developer_modules)
    {
        core::warning("[OGL-MODULE] Developer mode explicitly enabled: "
                      "unsigned native modules run with process privileges");
        module_options.developer_mode = true;
        module_options.allowed_capabilities.insert("developer");
    }
    modules::ModuleManager module_manager(std::move(module_options));
    module_manager.discover();
    std::string module_error;
    if (!module_manager.start_enabled(module_error))
        core::warning(std::string{"[OGL-MODULE] Startup: "} + module_error);

    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == GLFW_FALSE)
    {
        core::error("GLFW initialization failed");
        return 10;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* primary_mode =
        primary_monitor != nullptr
            ? glfwGetVideoMode(primary_monitor)
            : nullptr;

    const int initial_width =
        primary_mode != nullptr
            ? std::max(1280, primary_mode->width * 3 / 4)
            : 1440;
    const int initial_height =
        primary_mode != nullptr
            ? std::max(800, primary_mode->height * 3 / 4)
            : 900;

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(
        initial_width,
        initial_height,
        "OpenGenesisLINK Viewer",
        nullptr,
        nullptr);

    if (window == nullptr)
    {
        core::error("Viewer window creation failed");
        glfwTerminate();
        return 11;
    }

    glfwSetWindowSizeLimits(
        window,
        1100,
        700,
        GLFW_DONT_CARE,
        GLFW_DONT_CARE);
    glfwMaximizeWindow(window);

    if (primary_mode != nullptr)
    {
        core::info(
            "Viewer maximized window enabled; desktop=" +
            std::to_string(primary_mode->width) + "x" +
            std::to_string(primary_mode->height) +
            "; F11 toggles exclusive fullscreen");
    }
    else
    {
        core::info(
            "Viewer started maximized; F11 toggles fullscreen when available");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        core::error("GLAD OpenGL loader initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 12;
    }

    int window_width = 0;
    int window_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);

    LoginUiState login_ui;
    ui::ViewerShellState viewer_shell;
    protocol::ExperienceUiState experience_ui;
    protocol::ExperiencePermissionConsent experience_consent;
    std::optional<protocol::ExperiencePermissionRequest>
        approved_experience_permission;
    bool experience_permission_busy=false;
    bool outfit_gallery_open=false;
    std::size_t outfit_selection=0U;
    bool inventory_tree_open=false;
    protocol::InventoryTreeSnapshot inventory_tree;
    std::string inventory_parent;
    std::size_t inventory_selection=0U;
    std::future<protocol::InventoryFolderRead> inventory_folder_future;
    std::set<std::string> inventory_fetched_folders;
    std::map<std::string,protocol::InventoryFolderContents,std::less<>>
        inventory_folder_contents;
    std::map<std::string,std::chrono::steady_clock::time_point,std::less<>>
        inventory_last_requested;
    bool inventory_refresh_requested=false;
    std::uint64_t inventory_dirty_revision=0U;
    std::size_t inventory_item_selection=0U;
    bool inventory_item_detail_requested=false;
    std::future<protocol::InventoryItemFetch> inventory_item_detail_future;
    std::optional<protocol::OutfitInventoryItem> inventory_item_detail;
    std::uint64_t inventory_item_detail_epoch=0U;
    std::string inventory_item_detail_folder;
    std::string inventory_item_detail_id;
    std::string inventory_item_detail_status;
    InputContext input_context{&login_ui};
    glfwSetWindowUserPointer(window, &input_context);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    auto transport = std::make_shared<net::CurlHttpTransport>();
    auto discovery_future = std::async(
        std::launch::async,
        [transport]
        {
            protocol::GridDiscoveryService service{transport};
            return service.refresh();
        });

    std::string grid_name{protocol::nexverse_grid_host};
    std::string grid_status = "CHECKING";
    Color grid_status_color = theme::cyan;
    std::string active_login_uri;
    bool discovery_applied = false;

    std::future<protocol::LoginResult> login_future;
    std::optional<protocol::LoginSession> active_session;

    protocol::OutboundCommandQueue outbound_queue{512U};
    world::WorldModel world_model;
    std::mutex world_mutex;
    std::uint32_t self_avatar_local_id = 0U;
    std::uint64_t self_avatar_region_handle = 0U;
    std::chrono::steady_clock::time_point last_self_movement_log{};
    protocol::MovementFeedbackMonitor movement_feedback;
    bool simulation_ready = false;
    input_context.viewer_shell=&viewer_shell;
    input_context.experience=&experience_ui;
    input_context.outfits=&outfit_gallery_open;
    input_context.outfit_selection=&outfit_selection;
    input_context.inventory_open=&inventory_tree_open;
    input_context.inventory_parent=&inventory_parent;
    input_context.inventory_selection=&inventory_selection;
    input_context.inventory_tree=&inventory_tree;
    input_context.inventory_refresh_requested=&inventory_refresh_requested;
    input_context.inventory_item_selection=&inventory_item_selection;
    input_context.inventory_item_detail_requested=&inventory_item_detail_requested;
    input_context.permission_consent=&experience_consent;
    input_context.approved_permission=&approved_experience_permission;
    input_context.permission_busy=&experience_permission_busy;
    input_context.simulation_ready=&simulation_ready;
    protocol::PacketDispatcher packet_dispatcher;
    protocol::CapsEventDispatcher caps_event_dispatcher;
    protocol::InventoryEventInbox inventory_events;

    caps_event_dispatcher.register_handler(
        "EnableSimulator",
        [](const protocol::EventQueueEvent& event)
        {
            if (protocol::parse_enable_simulator_event(event).has_value())
            {
                core::info("CAPS EnableSimulator world event validated");
            }
        });

    caps_event_dispatcher.register_handler(
        "EstablishAgentCommunication",
        [](const protocol::EventQueueEvent& event)
        {
            if (protocol::parse_establish_agent_communication_event(event).has_value())
            {
                core::info(
                    "CAPS EstablishAgentCommunication world event validated");
            }
        });

    caps_event_dispatcher.register_handler(
        "TeleportFinish",
        [&world_model, &world_mutex](const protocol::EventQueueEvent& event)
        {
            const auto teleport =
                protocol::parse_teleport_finish_caps_event(event);
            if (!teleport.has_value())
            {
                core::warning("CAPS TeleportFinish world event rejected");
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (teleport->agent_id != world_model.agent().agent_id)
            {
                return;
            }

            world_model.begin_region_transition({
                teleport->simulator.region_handle,
                {},
                {},
                teleport->simulator.simulator_ip,
                teleport->simulator.simulator_port,
                teleport->simulator.seed_capability,
                {},
                0U,
                0U,
                0U,
                0U,
                0.0f,
                {},
                teleport->simulator.region_size_x,
                teleport->simulator.region_size_y,
                false});

            core::info("WorldModel region transition started from TeleportFinish");
        });

    caps_event_dispatcher.register_handler(
        "CrossedRegion",
        [&world_model, &world_mutex](const protocol::EventQueueEvent& event)
        {
            const auto crossed =
                protocol::parse_crossed_region_caps_event(event);
            if (!crossed.has_value())
            {
                core::warning("CAPS CrossedRegion world event rejected");
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (crossed->agent_id != world_model.agent().agent_id ||
                crossed->session_id != world_model.agent().session_id)
            {
                return;
            }

            world_model.begin_region_transition({
                crossed->simulator.region_handle,
                {},
                crossed->position,
                crossed->simulator.simulator_ip,
                crossed->simulator.simulator_port,
                crossed->simulator.seed_capability,
                {},
                0U,
                0U,
                0U,
                0U,
                0.0f,
                {},
                crossed->simulator.region_size_x,
                crossed->simulator.region_size_y,
                false});

            core::info("WorldModel region transition started from CrossedRegion");
        });

    caps_event_dispatcher.set_fallback_handler(
        [](const protocol::EventQueueEvent&)
        {
        });

    std::atomic_uint32_t local_chat_received{0U};
    std::mutex chat_history_mutex;
    std::deque<std::string> chat_history;
    std::atomic_bool region_handshake_world_logged{false};
    std::atomic_bool movement_complete_world_logged{false};
    std::atomic_uint32_t terrain_packets{0U};
    std::atomic_bool terrain_live_logged{false};
    std::atomic_bool parcel_live_logged{false};
    bool world_ready_logged = false;
    bool self_avatar_proxy_gl_logged=false;
    bool self_avatar_bake_gl_logged=false;
    bool self_avatar_original_mesh_gl_logged=false;
    bool self_avatar_weighted_skin_gl_logged=false;
    std::atomic_uint32_t object_packets{0U};
    std::atomic_uint32_t avatar_packets{0U};
    std::atomic_uint32_t avatar_animation_packets{0U};
    std::atomic_bool coarse_avatar_live_logged{false};
    std::atomic_bool full_object_live_logged{false};
    std::atomic_bool compressed_object_live_logged{false};
    std::atomic_bool cached_object_live_logged{false};
    std::atomic_bool terse_object_live_logged{false};
    std::atomic_uint32_t other_packets{0U};

    packet_dispatcher.register_handler(
        protocol::msg_region_handshake,
        [&region_handshake_world_logged, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto handshake =
                protocol::decode_region_handshake(packet.payload);
            if (!handshake.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            const world::RegionState region = world_model.region();
            world_model.apply_region({
                region.handle,
                handshake->sim_name.empty() ? region.name : handshake->sim_name,
                region.agent_position,
                region.simulator_ip,
                region.simulator_port,
                region.seed_capability,
                handshake->region_id,
                handshake->region_flags,
                handshake->region_flags_extended,
                handshake->region_protocols,
                handshake->sim_access,
                handshake->water_height,
                region.channel_version,
                region.size_x,
                region.size_y,
                region.connected});

            world::TerrainMaterials materials;
            materials.base_ids = handshake->terrain_base;
            materials.detail_ids = handshake->terrain_detail;
            materials.start_heights = handshake->terrain_start_height;
            materials.height_ranges = handshake->terrain_height_range;
            world_model.set_terrain_materials(std::move(materials));

            if (!region_handshake_world_logged.exchange(true))
            {
                core::info("WorldModel RegionHandshake metadata validated");
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_agent_movement_complete,
        [&movement_complete_world_logged, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto movement =
                protocol::decode_agent_movement_complete(packet.payload);
            if (!movement.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            const world::AgentState agent = world_model.agent();
            const world::RegionState region = world_model.region();

            if (movement->agent_id != agent.agent_id ||
                movement->session_id != agent.session_id ||
                movement->region_handle != region.handle)
            {
                return;
            }

            world_model.apply_agent({
                agent.agent_id,
                agent.session_id,
                movement->position,
                agent.rotation,
                agent.present});

            world_model.apply_region({
                region.handle,
                region.name,
                movement->position,
                region.simulator_ip,
                region.simulator_port,
                region.seed_capability,
                region.region_id,
                region.region_flags,
                region.region_flags_extended,
                region.region_protocols,
                region.sim_access,
                region.water_height,
                movement->channel_version,
                region.size_x,
                region.size_y,
                region.connected});

            if (!movement_complete_world_logged.exchange(true))
            {
                core::info("WorldModel AgentMovementComplete validated");
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_chat_from_simulator,
        [&local_chat_received,&chat_history_mutex,&chat_history](
            const protocol::DecodedPacket& packet)
        {
            const auto incoming=protocol::decode_chat_from_simulator(
                packet.payload);
            if(!incoming)return;
            local_chat_received.fetch_add(1U,std::memory_order_relaxed);
            // Real simulator chat is displayed locally, never written
            // verbatim to logs; bounded to prevent memory exhaustion.
            std::string sender=incoming->from_name.substr(0U,60U);
            std::string message=incoming->message.substr(0U,360U);
            for(auto* raw:{&sender,&message})
                for(char& ch:*raw)
                    if(ch=='\n'||ch=='\r'||ch=='\t')ch=' ';
            std::scoped_lock lock{chat_history_mutex};
            chat_history.push_back((sender.empty()?"System":sender)+
                                   ": "+message);
            while(chat_history.size()>48U)chat_history.pop_front();
        });

    packet_dispatcher.register_handler(
        protocol::msg_layer_data,
        [&terrain_packets,
         &terrain_live_logged,
         &world_model,
         &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto layer = protocol::decode_layer_data(packet.payload);
            if (!layer.has_value() || !layer->is_terrain())
            {
                return;
            }

            const auto terrain = protocol::decode_terrain_layer(
                layer->data,
                layer->type);
            if (!terrain.success)
            {
                core::warning(
                    "Terrain LayerData decode failed: " +
                    terrain.message);
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected)
            {
                return;
            }

            terrain_packets.fetch_add(1U, std::memory_order_relaxed);

            if (!terrain_live_logged.exchange(true))
            {
                core::info("WorldModel terrain stream validated");
            }

            for (const auto& patch : terrain.patches)
            {
                world_model.apply_terrain_patch(patch);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_parcel_properties,
        [&parcel_live_logged, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto parcel =
                protocol::decode_parcel_properties(packet.payload);
            if (!parcel.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected)
            {
                return;
            }

            if (!parcel_live_logged.exchange(true))
            {
                core::info("WorldModel parcel stream validated");
            }

            world_model.set_parcel({
                parcel->local_id,
                parcel->name,
                parcel->description,
                parcel->owner_id,
                parcel->area,
                parcel->parcel_flags,
                parcel->sale_price,
                parcel->music_url,
                parcel->media_url,
                true});
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update,
        [&object_packets,
         &avatar_packets,
         &full_object_live_logged,
         &self_avatar_local_id,
         &self_avatar_region_handle,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto update = protocol::decode_object_update(packet.payload);
            if (!update.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected ||
                update->region_handle != world_model.region().handle)
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!full_object_live_logged.exchange(true))
            {
                core::info("WorldModel full object stream validated");
            }

            const std::string self_id = world_model.agent().agent_id;

            for (const auto& entry : update->objects)
            {
                if (entry.is_avatar())
                {
                    avatar_packets.fetch_add(1U, std::memory_order_relaxed);

                    if (entry.id == self_id)
                    {
                        if (self_avatar_local_id != entry.local_id ||
                            self_avatar_region_handle != update->region_handle)
                        {
                            self_avatar_local_id = entry.local_id;
                            self_avatar_region_handle = update->region_handle;
                            core::info("Avatar self local-id bound from ObjectUpdate");
                        }
                        const world::AgentState current = world_model.agent();
                        world_model.apply_agent({
                            current.agent_id,
                            current.session_id,
                            entry.position,
                            entry.rotation,
                            current.present});
                        continue;
                    }

                    world_model.update_avatar_transform(
                        entry.id,entry.position,entry.rotation,entry.local_id);
                    continue;
                }

                world::ObjectState object;
                object.local_id = entry.local_id;
                object.id = entry.id;
                object.pcode = entry.pcode;
                object.parent_id = entry.parent_id;
                object.position = entry.position;
                object.rotation = entry.rotation;
                object.scale = entry.scale;
                object.shape = entry.shape;
                object.sculpt_asset_id = entry.sculpt_asset_id;
                object.sculpt_type = entry.sculpt_type;
                object.point_light = entry.point_light;
                object.particle_system = entry.particle_system;
                object.texture_entry = entry.texture_entry;
                object.parsed_texture_entry =
                    entry.parsed_texture_entry;
                object.default_texture = entry.default_texture;
                world_model.upsert_object(std::move(object));
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update_compressed,
        [&object_packets,
         &avatar_packets,
         &compressed_object_live_logged,
         &self_avatar_local_id,
         &self_avatar_region_handle,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto update =
                protocol::decode_object_update_compressed(packet.payload);
            if (!update.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected ||
                update->region_handle != world_model.region().handle)
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!compressed_object_live_logged.exchange(true))
            {
                core::info("WorldModel compressed object stream validated");
            }

            const std::string self_id = world_model.agent().agent_id;

            for (const auto& entry : update->objects)
            {
                if (entry.pcode == protocol::pcode_avatar)
                {
                    avatar_packets.fetch_add(1U, std::memory_order_relaxed);

                    if (entry.id == self_id)
                    {
                        if (self_avatar_local_id != entry.local_id ||
                            self_avatar_region_handle != update->region_handle)
                        {
                            self_avatar_local_id = entry.local_id;
                            self_avatar_region_handle = update->region_handle;
                            core::info("Avatar self local-id bound from ObjectUpdateCompressed");
                        }
                        const world::AgentState current = world_model.agent();
                        world_model.apply_agent({
                            current.agent_id,
                            current.session_id,
                            entry.position,
                            entry.rotation,
                            current.present});
                        continue;
                    }

                    world_model.update_avatar_transform(
                        entry.id,entry.position,entry.rotation,entry.local_id);
                    continue;
                }

                world::ObjectState object;
                object.local_id = entry.local_id;
                object.id = entry.id;
                object.pcode = entry.pcode;
                object.position = entry.position;
                object.rotation = entry.rotation;

                if (entry.has_complete_prim_data)
                {
                    object.parent_id = entry.parent_id;
                    object.scale = entry.scale;
                    object.shape = entry.shape;
                    object.sculpt_asset_id = entry.sculpt_asset_id;
                    object.sculpt_type = entry.sculpt_type;
                    object.point_light = entry.point_light;
                    object.particle_system = entry.particle_system;
                    object.texture_entry = entry.texture_entry;
                    object.parsed_texture_entry =
                        entry.parsed_texture_entry;
                    object.default_texture = entry.default_texture;
                }
                else if (const auto existing =
                             world_model.objects().find(entry.local_id);
                         existing != world_model.objects().end())
                {
                    object.parent_id = existing->second.parent_id;
                    object.scale = existing->second.scale;
                    object.shape = existing->second.shape;
                    object.sculpt_asset_id = existing->second.sculpt_asset_id;
                    object.sculpt_type = existing->second.sculpt_type;
                    object.point_light = existing->second.point_light;
                    object.particle_system = existing->second.particle_system;
                    object.texture_entry = existing->second.texture_entry;
                    object.parsed_texture_entry =
                        existing->second.parsed_texture_entry;
                    object.default_texture = existing->second.default_texture;
                }

                world_model.upsert_object(std::move(object));
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_object_update_cached,
        [&object_packets,
         &cached_object_live_logged,
         &world_model,
         &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto cached =
                protocol::decode_object_update_cached(packet.payload);
            if (!cached.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected ||
                cached->region_handle != world_model.region().handle)
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!cached_object_live_logged.exchange(true))
            {
                core::info("WorldModel cached object stream validated");
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_improved_terse_object_update,
        [&object_packets,
         &terse_object_live_logged,
         &self_avatar_local_id,
         &self_avatar_region_handle,
         &last_self_movement_log,
         &movement_feedback,
         &world_model,
         &world_mutex](const protocol::DecodedPacket& packet)
        {
            const auto terse =
                protocol::decode_improved_terse_object_update(packet.payload);
            if (!terse.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected ||
                terse->region_handle != world_model.region().handle)
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!terse_object_live_logged.exchange(true))
            {
                core::info("WorldModel terse object stream validated");
            }

            for (const auto& update : terse->updates)
            {
                if (update.avatar)
                {
                    if (self_avatar_local_id != 0U &&
                        self_avatar_region_handle == terse->region_handle &&
                        update.local_id == self_avatar_local_id)
                    {
                        const auto current = world_model.agent();
                        world_model.apply_agent({
                            current.agent_id, current.session_id,
                            update.position, update.rotation, current.present});
                        const auto now = std::chrono::steady_clock::now();
                        const auto evidence=movement_feedback.server_position(
                            update.position,now);
                        if(evidence==protocol::MovementEvidence::server_position_changed)
                            core::info("Avatar motion VERIFIED: simulator self-position changed "
                                       "after WASD input (>=0.25m); appearance not verified");
                        else if(evidence==protocol::MovementEvidence::server_position_stalled)
                            core::warning("Avatar motion NOT VERIFIED: directional AgentUpdate "
                                          "sent, but 3+ server position samples remained "
                                          "stationary for at least 4 seconds");
                        if (now - last_self_movement_log >
                                std::chrono::seconds{2})
                        {
                            last_self_movement_log = now;
                            core::info("Avatar self position synchronized: x=" +
                                std::to_string(update.position.x) +
                                " y=" + std::to_string(update.position.y) +
                                " z=" + std::to_string(update.position.z));
                        }
                        continue;
                    }
                    for (const auto& [id, existing] : world_model.avatars())
                    {
                        if (existing.local_id != update.local_id)
                        {
                            continue;
                        }

                        world_model.update_avatar_transform(
                            id,update.position,update.rotation,update.local_id);
                        break;
                    }
                    continue;
                }

                world::ObjectState object;
                object.local_id = update.local_id;
                object.position = update.position;
                object.rotation = update.rotation;

                if (const auto existing = world_model.objects().find(update.local_id);
                    existing != world_model.objects().end())
                {
                    object.id = existing->second.id;
                    object.pcode = existing->second.pcode;
                    object.parent_id = existing->second.parent_id;
                    object.scale = existing->second.scale;
                    object.shape = existing->second.shape;
                    object.sculpt_asset_id = existing->second.sculpt_asset_id;
                    object.sculpt_type = existing->second.sculpt_type;
                    object.point_light = existing->second.point_light;
                    object.particle_system = existing->second.particle_system;
                    object.texture_entry =
                        existing->second.texture_entry;
                    object.default_texture =
                        existing->second.default_texture;
                }

                world_model.upsert_object(std::move(object));
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_kill_object,
        [&object_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto kill = protocol::decode_kill_object(packet.payload);
            if (!kill.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected)
            {
                return;
            }

            object_packets.fetch_add(1U, std::memory_order_relaxed);

            for (const std::uint32_t local_id : kill->local_ids)
            {
                world_model.remove_object(local_id);
            }
        });

    packet_dispatcher.register_handler(
        protocol::msg_coarse_location_update,
        [&avatar_packets,
         &coarse_avatar_live_logged,
         &world_model,
         &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto coarse =
                protocol::decode_coarse_location_update(packet.payload);
            if (!coarse.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected)
            {
                return;
            }

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);
            if (!coarse_avatar_live_logged.exchange(true))
            {
                core::info("WorldModel coarse avatar stream validated");
            }

            const std::string self_id = world_model.agent().agent_id;
            std::vector<std::string> seen;
            seen.reserve(coarse->avatars.size());

            for (const auto& coarse_avatar : coarse->avatars)
            {
                if (coarse_avatar.agent_id == self_id)
                {
                    continue;
                }

                seen.push_back(coarse_avatar.agent_id);

                protocol::Quaternion rotation{};
                if (const auto existing =
                        world_model.avatars().find(coarse_avatar.agent_id);
                    existing != world_model.avatars().end())
                    rotation=existing->second.rotation;
                world_model.update_avatar_transform(
                    coarse_avatar.agent_id,coarse_avatar.position,rotation,0U);
            }

            std::vector<std::string> stale;
            for (const auto& [id, avatar] : world_model.avatars())
            {
                (void)avatar;
                if (std::find(seen.begin(), seen.end(), id) == seen.end())
                {
                    stale.push_back(id);
                }
            }

            for (const auto& id : stale)
            {
                world_model.remove_avatar(id);
            }
        });

    // Simulator AvatarAnimation High20 is a complete animation set.
    // It carries sender UUID, but NO session ID or region handle. Thus
    // bind it to the active authenticated simulator circuit and current
    // connected region. Unknown remote senders remain bounded, invisible
    // pending state until their ObjectUpdate arrives.
    packet_dispatcher.register_handler(
        protocol::msg_avatar_animation,
        [&avatar_animation_packets,&world_model,&world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto animation=protocol::decode_avatar_animation(
                packet.payload);
            if(!animation)return;
            std::scoped_lock lock{world_mutex};
            const bool first_self=animation->sender==
                world_model.agent().agent_id &&
                !world_model.self_animations().has_value();
            if(!world_model.apply_avatar_animation(*animation))return;
            avatar_animation_packets.fetch_add(1U,std::memory_order_relaxed);
            if(first_self)
                core::info(
                    "Firestorm AvatarAnimation SELF High20 readback: "
                    "active="+std::to_string(animation->animations.size())+
                    "; sources="+std::to_string(animation->sources.size())+
                    "; event_blocks="+
                    std::to_string(animation->physical_events.size())+
                    "; animation ASSET playback still pending");
        });

    packet_dispatcher.register_handler(
        protocol::msg_avatar_appearance,
        [&avatar_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto appearance =
                protocol::decode_avatar_appearance(packet.payload);
            if (!appearance.has_value())
            {
                return;
            }

            std::scoped_lock lock{world_mutex};
            if (!world_model.region().connected)
                return;

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);

            if (appearance->avatar_id == world_model.agent().agent_id)
            {
                const bool first=!world_model.self_appearance().has_value();
                if (world_model.apply_self_appearance(*appearance) && first)
                    core::info("AvatarAppearance SELF server readback accepted; "
                               "texture_entry_bytes="+
                               std::to_string(appearance->texture_entry.size())+
                               "; visual_params="+
                               std::to_string(appearance->visual_params.size())+
                               "; attachment_blocks="+
                               std::to_string(appearance->attachments.size())+
                               "; original weighted body rig used when installed; "
                               "morphs/COF not verified");
                return;
            }

            world::AvatarState avatar;
            avatar.id = appearance->avatar_id;
            if (const auto existing =
                    world_model.avatars().find(appearance->avatar_id);
                existing != world_model.avatars().end())
                avatar = existing->second;
            avatar.trial_appearance = appearance->trial;
            avatar.texture_entry = appearance->texture_entry;
            avatar.visual_params = appearance->visual_params;
            avatar.appearance_data = appearance->appearance_data;
            avatar.hover_heights = appearance->hover_heights;
            avatar.attachments = appearance->attachments;
            // Appearance-only updates can precede a position. Keep the
            // metadata, but do not create a visible ghost at (0,0,0).
            world_model.upsert_avatar(std::move(avatar));
        });

    packet_dispatcher.register_handler(
        protocol::msg_agent_data_update,
        [&avatar_packets, &world_model, &world_mutex](
            const protocol::DecodedPacket& packet)
        {
            const auto data = protocol::decode_agent_data_update(packet.payload);
            if (!data.has_value())
            {
                return;
            }

            avatar_packets.fetch_add(1U, std::memory_order_relaxed);

            std::scoped_lock lock{world_mutex};
            if (data->agent_id != world_model.agent().agent_id)
            {
                return;
            }

            world_model.set_agent_profile({
                data->first_name,
                data->last_name,
                data->group_title,
                data->active_group_id,
                data->group_powers,
                data->group_name});
        });

    packet_dispatcher.set_fallback_handler(
        [&other_packets](const protocol::DecodedPacket&)
        {
            other_packets.fetch_add(1U, std::memory_order_relaxed);
        });

    auto last_agent_update = std::chrono::steady_clock::now();
    protocol::MovementController movement_controller;
    std::uint32_t last_reported_control_flags = 0U;
    bool movement_heading_initialized = false;

    std::atomic_bool circuit_stop{false};
    std::future<protocol::CircuitStatus> circuit_future;
    std::mutex circuit_mutex;
    protocol::CircuitStatus circuit_status;
    bool circuit_started = false;

    std::atomic_bool caps_stop{false};
    std::future<void> caps_future;
    std::uint64_t login_caps_epoch=0U;
    std::vector<std::future<protocol::RegionEventQueueStats>>
        region_event_queue_futures;
    std::uint64_t region_event_started_epoch=0U;
    std::mutex caps_mutex;
    CapsRuntimeState caps_state;
    protocol::FirestormRegionLease firestorm_region_lease;
    std::string experience_permission_cap;
    std::future<protocol::ExperiencePermissionResponse>
        experience_permission_future;
    input_context.region_lease=&firestorm_region_lease;
    // OpenSim may notify item/folder deletion through legacy LLUDP rather
    // than EventQueueGet. Its message carries no parent-folder mapping.
    // Verify both session UUIDs against WorldModel and use the SAME inbox
    // as BulkUpdateInventory: the main thread will invalidate F9/F10 and
    // refetch content over owner-bound CAPS instead of deleting by hint.
    const auto inventory_remove_handler=
        [&world_model,&world_mutex,&inventory_events,
         &firestorm_region_lease](const protocol::DecodedPacket& packet)
        {
            std::string agent_id;
            std::string session_id;
            {
                std::scoped_lock lock{world_mutex};
                if(!world_model.region().connected)return;
                agent_id=world_model.agent().agent_id;
                session_id=world_model.agent().session_id;
            }
            if(!protocol::parse_inventory_remove_hint(
                   packet,agent_id,session_id))
                return;
            const auto epoch=firestorm_region_lease.epoch();
            (void)inventory_events.enqueue_full_refresh(
                epoch,firestorm_region_lease);
        };
    packet_dispatcher.register_handler(
        protocol::msg_remove_inventory_item,inventory_remove_handler);
    packet_dispatcher.register_handler(
        protocol::msg_remove_inventory_folder,inventory_remove_handler);
    packet_dispatcher.register_handler(
        protocol::msg_remove_inventory_objects,inventory_remove_handler);

    input_context.caps=&caps_state;
    input_context.caps_mutex=&caps_mutex;
    input_context.permission_cap=&experience_permission_cap;
    std::vector<std::future<protocol::FirestormRegionRead>>
        firestorm_region_read_futures;
    std::uint64_t firestorm_region_read_started_epoch=0U;
    std::string gallery_texture_cap;
    std::string gallery_inventory_cap;
    std::string gallery_inventory_items_cap;
    std::future<protocol::InventoryItemFetch> gallery_item_fetch_future;
    std::uint64_t gallery_item_fetch_epoch=0U;
    std::string gallery_item_fetch_folder;
    std::string gallery_item_fetch_completed_folder;
    protocol::FirestormInventorySync gallery_inventory_sync;
    std::future<protocol::OutfitInventoryRead> gallery_sync_future;
    std::uint64_t gallery_sync_issued_epoch=0U;
    std::uint64_t gallery_sync_dirty_revision=0U;
    bool gallery_force_refresh=false;
    auto last_gallery_sync=std::chrono::steady_clock::now();
    std::vector<std::future<GalleryTextureReply>> gallery_image_futures;
    std::map<std::string,std::pair<std::string,unsigned int>,std::less<>>
        gallery_gpu_textures;
    std::map<std::string,std::string,std::less<>> gallery_image_requested;
    protocol::RegionMaterialScope material_scope;
    std::future<protocol::CapabilityDiscoveryResult> region_material_discovery_future;
    std::uint64_t region_material_discovery_epoch = 0U;
    bool region_material_discovery_started = false;
    std::string scoped_render_materials_uri;
    std::string scoped_get_texture_uri;
    std::string scoped_get_mesh_uri;
    std::string scoped_get_mesh_alt_uri;
    auto mesh_memory_cache = std::make_shared<assets::MeshMemoryCache>();
    std::future<assets::CachedMeshResult> mesh_asset_future;
    std::string mesh_asset_loading_id;
    assets::MeshLod mesh_asset_loading_lod = assets::MeshLod::high;
    std::unordered_map<std::string, assets::MeshLod> mesh_desired_lods;
    std::unordered_map<std::string, assets::MeshLod> mesh_gpu_lods;
    assets::AssetLodDwell<assets::MeshLod> mesh_lod_dwell;
    assets::AssetGpuUploadGate asset_gpu_upload_gate;
    assets::AssetUploadTelemetry asset_gpu_upload_telemetry;
    assets::AssetScenePriorityFeedback mesh_priority_feedback;
    assets::AssetScenePriorityFeedback texture_priority_feedback;
    std::size_t texture_slot_replacements = 0U;
    std::unordered_map<std::string, std::size_t> mesh_gpu_asset_bytes;
    std::size_t mesh_gpu_estimated_bytes = 0U;
    constexpr std::size_t shared_gpu_budget_bytes = 384U*1024U*1024U;
    constexpr std::size_t mesh_gpu_budget_bytes = 256U*1024U*1024U;
    std::unordered_set<std::string> mesh_gpu_budget_deferred_ids;
    assets::GpuStagingRecovery mesh_gpu_budget_recovery;
    assets::AssetReuseHistory mesh_cpu_reuse_history;
    assets::AssetCachePressureWindow mesh_ram_pressure;
    std::size_t mesh_gpu_budget_deferrals = 0U;
    std::size_t mesh_worker_completions = 0U;
    std::uint64_t mesh_worker_elapsed_ms = 0U;
    std::chrono::steady_clock::time_point mesh_request_started{};
    std::size_t mesh_lod_swaps = 0U;
    std::uint64_t mesh_asset_request_epoch = 0U;
    std::vector<assets::MeshDistanceSample> scene_mesh_samples;
    assets::MeshResidencyPlan mesh_residency;
    auto last_mesh_residency_plan = std::chrono::steady_clock::time_point{};
    constexpr std::size_t mesh_gpu_asset_budget = 256U;
    std::unordered_set<std::string> mesh_asset_loaded_ids;
    assets::AssetRetrySchedule mesh_asset_retries;
    std::size_t mesh_retry_dispatches = 0U;
    std::size_t mesh_failure_events = 0U;
    assets::AssetFailureTelemetry mesh_failure_categories;
    assets::AssetMissingIdCounter mesh_missing_unique;
    std::size_t mesh_stale_results = 0U;
    std::size_t mesh_ram_hits = 0U;
    std::size_t mesh_disk_hits = 0U;
    std::size_t mesh_network_loads = 0U;
    std::size_t mesh_gpu_evictions = 0U;
    assets::AssetRequestPacer mesh_request_pacer{
        std::chrono::milliseconds{180}};
    assets::AssetRequestFairness mesh_request_fairness;
    std::size_t mesh_request_dispatches = 0U;
    std::chrono::steady_clock::time_point last_mesh_telemetry{};
    std::future<protocol::RenderMaterialsResult> render_materials_future;
    std::uint64_t render_materials_get_epoch = 0U;
    std::uint64_t render_materials_post_epoch = 0U;
    std::future<protocol::RenderMaterialsResult> render_materials_post_future;
    std::vector<std::string> render_materials_in_flight;
    bool render_materials_started = false;
    bool render_materials_get_complete = false;
    std::map<std::string, protocol::LegacyMaterial, std::less<>> legacy_materials;
    std::set<std::string, std::less<>> material_ids_pending;
    std::map<std::string, std::chrono::steady_clock::time_point, std::less<>>
        material_request_times;
    std::map<std::string, unsigned, std::less<>> material_request_attempts;
    std::optional<protocol::LlsdValue> ext_environment_payload;
    bool caps_started = false;
    // Only one LLUDP transport worker exists at a time. On a new
    // destination, its old reliable stream is stopped before another
    // UseCircuitCode/CompleteAgentMovement handshake is attempted.
    std::string circuit_target_ip;
    std::uint16_t circuit_target_port=0U;
    auto launch_circuit=[&](protocol::LoginSession circuit_session)
    {
        circuit_future = std::async(
            std::launch::async,
            [circuit_session = std::move(circuit_session),
                                 &circuit_stop,
                                 &circuit_mutex,
                                 &circuit_status,
                                 &packet_dispatcher,
                                 &outbound_queue,
                                 &world_model,&world_mutex]() mutable
                                {
                                    protocol::CircuitStatus final_status;
                                    protocol::CircuitState last_reported_state =
                                        protocol::CircuitState::disconnected;
                                    constexpr int max_attempts = 3;

                                    for (int attempt = 1;
                                         attempt <= max_attempts && !circuit_stop.load();
                                         ++attempt)
                                    {
                                        protocol::CircuitConnection connection{circuit_session};
                                        final_status = connection.run(
                                            circuit_stop,
                                            [&circuit_mutex,
                                             &circuit_status,
                                             &last_reported_state](
                                                const protocol::CircuitStatus& update)
                                            {
                                                {
                                                    std::scoped_lock lock{circuit_mutex};
                                                    circuit_status = update;
                                                }

                                                if (update.state != last_reported_state)
                                                {
                                                    last_reported_state = update.state;
                                                    core::info(
                                                        std::string{"LLUDP state: "} +
                                                        std::string{circuit_state_label(update.state)} +
                                                        " - " + update.message);
                                                }
                                            },
                                            [&packet_dispatcher,
                                             &world_model,&world_mutex,
                                             &circuit_stop,
                                             &circuit_session](
                                                const protocol::DecodedPacket& packet)
                                            {
                                                if(circuit_stop.load())return;
                                                {
                                                    std::scoped_lock lock{world_mutex};
                                                    if(!protocol::region_packet_matches(
                                                           circuit_session.simulator_ip,
                                                           circuit_session.simulator_port,
                                                           world_model.region().simulator_ip,
                                                           world_model.region().simulator_port,
                                                           circuit_stop.load()))
                                                        return;
                                                }
                                                (void)packet_dispatcher.dispatch(packet);
                                            },
                                            &outbound_queue);

                                        if (circuit_stop.load() ||
                                            final_status.state != protocol::CircuitState::failed)
                                        {
                                            break;
                                        }

                                        if (attempt < max_attempts)
                                        {
                                            {
                                                std::scoped_lock lock{circuit_mutex};
                                                circuit_status.message =
                                                    "LLUDP reconnect scheduled";
                                            }
                                            std::this_thread::sleep_for(std::chrono::seconds{1});
                                        }
                                    }

                                    return final_status;
                                });
    };

    using TerrainTextureResults =
        std::array<assets::TextureAssetResult, 4>;
    std::future<TerrainTextureResults> terrain_texture_future;
    bool terrain_texture_loading = false;
    std::uint64_t terrain_texture_request_epoch = 0U;
    std::uint32_t terrain_texture_request_generation =
        std::numeric_limits<std::uint32_t>::max();
    std::uint32_t terrain_texture_request_material_revision =
        std::numeric_limits<std::uint32_t>::max();

    auto texture_memory_cache = std::make_shared<assets::TextureMemoryCache>();
    assets::TexturePrefetchScope texture_prefetch_scope;
    auto texture_prefetch_live_epoch =
        std::make_shared<std::atomic<std::uint64_t>>(0U);
    std::future<bool> texture_prefetch_future;
    std::uint64_t texture_prefetch_request_epoch = 0U;
    std::size_t texture_prefetch_attempts = 0U;
    std::size_t texture_prefetch_warmed = 0U;
    std::size_t texture_prefetch_misses = 0U;
    std::size_t texture_prefetch_stale = 0U;
    assets::AssetRequestPacer texture_prefetch_pacer{
        std::chrono::milliseconds{650}};
    assets::AssetRequestPacer texture_request_pacer{
        std::chrono::milliseconds{125}};
    assets::AssetRequestFairness texture_request_fairness;
    std::size_t texture_request_dispatches = 0U;
    std::future<assets::TextureAssetResult> object_texture_future;
    bool object_texture_loading = false;
    std::uint64_t object_texture_request_epoch = 0U;
    std::string object_texture_loading_id;
    std::uint8_t object_texture_loading_mip = 0U;
    std::unordered_map<std::string, std::uint8_t> texture_desired_mips;
    std::unordered_map<std::string, std::uint8_t> texture_gpu_mips;
    assets::AssetLodDwell<std::uint8_t> texture_lod_dwell;
    std::unordered_map<std::string, std::size_t> texture_gpu_bytes;
    std::size_t texture_gpu_used_bytes = 0U;
    constexpr std::size_t texture_gpu_budget_bytes =
        256U * 1024U * 1024U;
    std::unordered_set<std::string> texture_budget_deferred_ids;
    assets::GpuStagingRecovery texture_gpu_staging_recovery;
    assets::AssetReuseHistory texture_cpu_reuse_history;
    assets::AssetCachePressureWindow texture_ram_pressure;
    std::size_t texture_mip_swaps = 0U;
    std::size_t texture_budget_rejections = 0U;
    std::unordered_set<std::string> object_texture_loaded_ids;
    assets::AssetRetrySchedule object_texture_retries;
    std::size_t texture_retry_dispatches = 0U;
    std::size_t texture_failure_events = 0U;
    assets::AssetFailureTelemetry texture_failure_categories;
    assets::AssetMissingIdCounter texture_missing_unique;
    std::size_t texture_stale_results = 0U;
    std::vector<std::string> scene_texture_ids;
    std::vector<assets::TextureDistanceSample> scene_texture_samples;
    assets::TextureResidencyPlan texture_residency;
    std::chrono::steady_clock::time_point last_texture_residency_plan{};
    std::size_t texture_ram_hits = 0U;
    std::size_t texture_worker_completions = 0U;
    std::uint64_t texture_worker_elapsed_ms = 0U;
    std::chrono::steady_clock::time_point texture_request_started{};
    std::size_t texture_cache_hits = 0U;
    std::size_t texture_http_loads = 0U;
    std::size_t texture_disk_misses = 0U;
    std::size_t texture_disk_corrupt = 0U;
    std::size_t texture_disk_io_errors = 0U;
    std::size_t texture_aborted_before_http = 0U;
    std::size_t texture_discarded_after_http = 0U;
    std::size_t mesh_aborted_before_http = 0U;
    std::size_t mesh_discarded_after_http = 0U;
    std::size_t texture_network_attempts = 0U;
    std::size_t texture_gpu_evictions = 0U;
    std::size_t texture_gpu_upload_failures = 0U;
    std::size_t mesh_gpu_upload_failures = 0U;
    std::size_t texture_gpu_staging_deferrals = 0U;
    std::chrono::steady_clock::time_point last_world_asset_health_log{};
    const auto inworld_session_started_ms=static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    std::uint64_t inworld_snapshot_sequence=0U;
    std::chrono::steady_clock::time_point last_texture_telemetry{};

    {
        auto world_renderer = render::create_legacy_opengl_renderer();
        if (!world_renderer ||
            !world_renderer->initialize(window_width, window_height))
        {
            core::error("OGLRender legacy backend initialization failed");
            glfwDestroyWindow(window);
            glfwTerminate();
            return 13;
        }

        // Bind the original Firestorm 133-joint rest hierarchy and 55
        // attachment slot positions. This is skeletal attachment-point
        // positioning, not yet driven by in-world animations.
        if(const auto dir=assets::find_firestorm_avatar_character_directory();
            dir)
        {
            if(const auto rig=assets::load_firestorm_avatar_rig(*dir);rig)
            {
                render::RenderAvatarBindRig bound;
                for(const auto& joint:rig->joints)
                    bound.joints.push_back({
                        joint.name,joint.parent,
                        {joint.local[0],joint.local[1],joint.local[2]},
                        {joint.bind_world[0],joint.bind_world[1],
                         joint.bind_world[2]}});
                for(std::size_t i=0U;i<bound.available.size();++i)
                {
                    const auto& offset=rig->attachment_bind_offsets[i];
                    bound.attachment_offsets[i]={
                        offset[0],offset[1],offset[2]};
                    bound.available[i]=rig->attachment_available[i];
                    bound.is_hud[i]=rig->attachment_is_hud[i];
                }
                world_renderer->set_avatar_bind_rig(bound);
                core::info("Firestorm original skeleton bind joints="+
                    std::to_string(rig->joints.size())+
                    "; attachment slots="+
                    std::to_string(rig->attachment_count)+
                    " (HUD points excluded from 3D scene)");
            }
            else core::warning("Firestorm avatar skeleton definition "
                "could not be loaded; neutral avatar attachment roots used");
        }
        // The first-party Firestorm asset format is used directly: read
        // original resident .llm meshes and send their real triangles,
        // normals and UVs to the OpenGL renderer. No additional file is
        // downloaded from untrusted grid servers, no assets are committed.
        if(const auto dir=
               assets::find_firestorm_avatar_character_directory();dir)
        {
            std::size_t uploaded=0U;
            std::size_t triangles=0U;
            const std::array<std::string_view,3U> body_files{
                "avatar_head.llm","avatar_upper_body.llm",
                "avatar_lower_body.llm"};
            for(std::size_t slot=0U;slot<body_files.size();++slot)
            {
                const auto asset=assets::load_firestorm_avatar_llm(
                    *dir/std::string(body_files[slot]));
                if(!asset || !asset->valid())break;
                render::RenderMeshGeometry geometry;
                geometry.vertices=asset->vertices;
                geometry.indices=asset->indices;
                if(const auto rig=assets::load_firestorm_avatar_rig(*dir);
                   rig)
                {
                    const auto palette=assets::firestorm_joint_render_palette(
                        *rig,asset->joint_names);
                    if(palette)
                    {
                        geometry.skin_weights=asset->skin_weights;
                        geometry.skin_palette=*palette;
                    }
                    else core::warning("Firestorm avatar .llm joint palette "
                                       "unresolved; bind-pose mesh only");
                }
                if(!world_renderer->set_avatar_body_mesh(slot,geometry))
                    break;
                ++uploaded;
                triangles+=geometry.indices.size()/3U;
            }
            if(uploaded==3U)
                core::info("OGLRenderer Firestorm ORIGINAL three-part "
                    "avatar bind-pose meshes GPU-resident; triangles="+
                    std::to_string(triangles)+
                    "; original joint-weight GPU path available, server animation pending");
            else core::warning(
                "Firestorm avatar .llm loading incomplete; "
                "seven-part fallback proxy remains active");
        }
        else core::warning(
            "Firestorm character assets unavailable; using provisional avatar "
            "geometry (set NEXVERSE_AVATAR_ASSETS to Firestorm's root)");
        ui::UiRenderer renderer;
        if (!renderer.initialize(window_width, window_height))
        {
            world_renderer->shutdown();
            glfwDestroyWindow(window);
            glfwTerminate();
            return 14;
        }

        ui::WelcomePage welcome_page;
        unsigned int welcome_texture = 0U;
        int welcome_texture_width = 0;
        int welcome_texture_height = 0;
        bool welcome_failure_logged = false;

        if (!welcome_page.initialize(window_width, window_height))
        {
            core::warning("Embedded NexVerse welcome page initialization failed");
        }

        const auto* gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        const auto* gl_renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        if (gl_version != nullptr)
        {
            core::info(std::string{"OpenGL: "} + gl_version);
        }
        if (gl_renderer != nullptr)
        {
            core::info(std::string{"GPU: "} + gl_renderer);
        }

        FrameMetrics frame_metrics;
        std::uint64_t diagnostic_frame_index = 0U;
        std::uint32_t rendered_terrain_generation =
            std::numeric_limits<std::uint32_t>::max();
        std::uint32_t rendered_terrain_revision =
            std::numeric_limits<std::uint32_t>::max();
        std::uint32_t rendered_material_revision =
            std::numeric_limits<std::uint32_t>::max();
        std::uint32_t rendered_environment_revision =
            std::numeric_limits<std::uint32_t>::max();
        std::uint32_t rendered_object_generation =
            std::numeric_limits<std::uint32_t>::max();
        std::uint32_t rendered_object_revision =
            std::numeric_limits<std::uint32_t>::max();
        constexpr std::size_t max_object_texture_assets = 512U;
        auto previous_frame = std::chrono::steady_clock::now();
        auto next_environment_resample = previous_frame;

        auto apply_environment_snapshot =
            [&world_model, &world_mutex](
                const protocol::WindLightEnvironment& parsed)
            {
                std::scoped_lock lock{world_mutex};
                world::EnvironmentState state =
                    world_model.environment();
                ++state.revision;
                state.available = true;
                state.ambient = parsed.ambient;
                state.blue_density = parsed.blue_density;
                state.blue_horizon = parsed.blue_horizon;
                state.sunlight_color = parsed.sunlight_color;
                state.cloud_color = parsed.cloud_color;
                state.water_fog_color = parsed.water_fog_color;
                state.haze_density = parsed.haze_density;
                state.haze_horizon = parsed.haze_horizon;
                state.density_multiplier = parsed.density_multiplier;
                state.distance_multiplier = parsed.distance_multiplier;
                state.gamma = parsed.gamma;
                state.star_brightness = parsed.star_brightness;
                state.sun_angle = parsed.sun_angle;
                state.east_angle = parsed.east_angle;
                state.cloud_shadow = parsed.cloud_shadow;
                state.water_fog_density = parsed.water_fog_density;
                state.sky_name = parsed.sky_name;
                state.water_normal_map = parsed.water_normal_map;
                world_model.set_environment(std::move(state));
            };

        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            glfwPollEvents();

            // Real UI commands are queued on the render/main thread,
            // never from a network callback or a login-only mockup.
            if(viewer_shell.chat_submit_requested)
            {
                viewer_shell.chat_submit_requested=false;
                auto msg=viewer_shell.chat_draft;
                const auto last=msg.find_last_not_of(" \r\n\t");
                if(last==std::string::npos)
                {
                    viewer_shell.status_text="Bitte eine Nachricht eingeben.";
                }
                else if(!simulation_ready||!login_ui.authenticated)
                {
                    viewer_shell.status_text="Nur in verbundener Welt moeglich.";
                }
                else
                {
                    msg.erase(last+1U);
                    const auto first=msg.find_first_not_of(" \r\n\t");
                    msg.erase(0U,first);
                    if(outbound_queue.push(protocol::LocalChatMessage{
                        std::move(msg),protocol::LocalChatType::normal,0}))
                    {
                        viewer_shell.chat_draft.clear();
                        viewer_shell.status_text="Chat-Nachricht in Sendewarteschlange.";
                    }
                    else viewer_shell.status_text=
                        "Netzwerk-Warteschlange voll: erneut versuchen.";
                }
            }
            if(viewer_shell.teleport_submit_requested)
            {
                viewer_shell.teleport_submit_requested=false;
                world::RegionState dest_region;
                {
                    std::scoped_lock lock{world_mutex};
                    dest_region=world_model.region();
                }
                const auto coords=ui::valid_teleport_target(
                    viewer_shell,dest_region.size_x,dest_region.size_y);
                if(!simulation_ready||!login_ui.authenticated||
                   !dest_region.connected||dest_region.handle==0U)
                    viewer_shell.status_text=
                        "Keine aktive Simulatorverbindung.";
                else if(!coords)
                    viewer_shell.status_text=
                        "Ungueltiges Ziel: X/Y im Regionbereich, Z -100..4096.";
                else if(outbound_queue.push(protocol::TeleportLocation{
                    dest_region.handle,
                    {(*coords)[0],(*coords)[1],(*coords)[2]},
                    {1.0f,0.0f,0.0f}}))
                    viewer_shell.status_text=
                        "Teleport angefordert (Simulatorbestaetigung ausstehend).";
                else viewer_shell.status_text=
                    "Netzwerk-Warteschlange voll: erneut versuchen.";
            }

            const auto frame_now = std::chrono::steady_clock::now();
            const double raw_frame_ms =
                std::chrono::duration<double, std::milli>(
                    frame_now - previous_frame).count();
            previous_frame = frame_now;

            if (frame_now >= next_environment_resample)
            {
                std::optional<protocol::LlsdValue> eep_payload;
                {
                    std::scoped_lock lock{caps_mutex};
                    eep_payload = ext_environment_payload;
                }

                if (eep_payload.has_value())
                {
                    const auto parsed =
                        protocol::parse_ext_environment_settings(
                            *eep_payload);
                    if (parsed.has_value())
                    {
                        apply_environment_snapshot(*parsed);
                    }
                }

                next_environment_resample =
                    frame_now + std::chrono::milliseconds{2500};
            }

            if (raw_frame_ms > 0.0 && raw_frame_ms < 1000.0)
            {
                if (frame_metrics.frame_time_ms <= 0.0)
                {
                    frame_metrics.frame_time_ms = raw_frame_ms;
                }
                else
                {
                    frame_metrics.frame_time_ms =
                        frame_metrics.frame_time_ms * 0.90 +
                        raw_frame_ms * 0.10;
                }
                frame_metrics.fps =
                    1000.0 / frame_metrics.frame_time_ms;
            }

            if (!discovery_applied &&
                discovery_future.valid() &&
                discovery_future.wait_for(std::chrono::milliseconds{0}) == std::future_status::ready)
            {
                discovery_applied = true;
                try
                {
                    auto result = discovery_future.get();
                    if (result.success)
                    {
                        if (!result.info.grid_name().empty())
                        {
                            grid_name = std::string{result.info.grid_name()};
                        }
                        else if (!result.info.grid_nick().empty())
                        {
                            grid_name = std::string{result.info.grid_nick()};
                        }

                        if (result.source == protocol::GridInfoSource::live)
                        {
                            grid_status = "LIVE";
                            grid_status_color = theme::success;
                        }
                        else
                        {
                            grid_status = "CACHE";
                            grid_status_color = theme::warning;
                        }

                        active_login_uri = std::string{result.info.login_uri()};
                        core::info(
                            std::string{"Active OpenSim login URI: "} +
                            active_login_uri);
                    }
                    else
                    {
                        grid_status = "ERROR";
                        grid_status_color = theme::magenta;
                        core::warning(result.message);
                    }
                }
                catch (const std::exception& error)
                {
                    grid_status = "ERROR";
                    grid_status_color = theme::magenta;
                    core::error(std::string{"Grid discovery worker failed: "} + error.what());
                }
            }

            // Escape callback closes the Experience floater before quitting.
            if (login_ui.busy &&
                login_future.valid() &&
                login_future.wait_for(std::chrono::milliseconds{0}) == std::future_status::ready)
            {
                login_ui.busy = false;

                try
                {
                    protocol::LoginResult result = login_future.get();
                    if (result.success && result.session.has_value())
                    {
                        active_session = std::move(result.session);
                        login_ui.authenticated = true;
                        movement_controller.reset();
                        movement_feedback.reset();
                        movement_heading_initialized = false;
                        last_reported_control_flags = 0U;
                        {
                            std::scoped_lock lock{world_mutex};
                            self_avatar_local_id = 0U;
                            self_avatar_region_handle = 0U;
                        }
                        login_ui.status = "CONNECTED";
                        login_ui.notice = result.message.empty()
                            ? "OpenSim-Anmeldung erfolgreich."
                            : result.message;
                        login_ui.password.clear();
                        core::info("OpenSim login succeeded; simulator session established");

                        const std::uint64_t region_handle =
                            (static_cast<std::uint64_t>(active_session->region_x) << 32U) |
                            static_cast<std::uint64_t>(active_session->region_y);

                        std::string region_name = active_session->start_location;
                        if (const auto found = active_session->scalar_fields.find("sim_name");
                            found != active_session->scalar_fields.end() && !found->second.empty())
                        {
                            region_name = found->second;
                        }

                        {
                            std::scoped_lock lock{world_mutex};
                            world_model.reset();
                            world_model.set_grid({grid_name, active_login_uri});
                            world_model.apply_region({
                                region_handle,
                                std::move(region_name),
                                {},
                                active_session->simulator_ip,
                                active_session->simulator_port,
                                active_session->seed_capability,
                                {},
                                0U,
                                0U,
                                0U,
                                0U,
                                0.0f,
                                {},
                                256U,
                                256U,
                                false});
                            world_model.apply_agent({
                                active_session->agent_id,
                                active_session->session_id,
                                {},
                                {},
                                false});
                            simulation_ready = world_model.ready();
                        }

                        if (!caps_started &&
                            active_session.has_value() &&
                            !active_session->seed_capability.empty())
                        {
                            caps_started = true;
                            const std::string seed_capability =
                                active_session->seed_capability;
                            std::uint64_t initial_handle=0U;
                            std::uint32_t initial_generation=0U;
                            {
                                std::scoped_lock lock{world_mutex};
                                initial_handle=world_model.region().handle;
                                initial_generation=world_model.region().generation;
                            }
                            const auto firestorm_issued_epoch=
                                firestorm_region_lease.enter(
                                    initial_handle,initial_generation,
                                    seed_capability);
                            login_caps_epoch=firestorm_issued_epoch;

                            caps_future = std::async(
                                std::launch::async,
                                [transport,
                                 seed_capability,
                                 firestorm_issued_epoch,
                                 &firestorm_region_lease,
                                 &caps_stop,
                                 &caps_mutex,
                                 &caps_state,
                                 &ext_environment_payload,
                                 &caps_event_dispatcher,
                                 &inventory_events,
                                 inventory_owner=active_session->agent_id,
                                 initial_handle,
                                 &world_model,
                                 &world_mutex]()
                                {
                                    protocol::CapabilityService service{transport};

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.status = "DISCOVERY";
                                    }

                                    protocol::CapabilityDiscoveryResult discovery =
                                        service.discover(seed_capability);

                                    if (!discovery.success)
                                    {
                                        {
                                            std::scoped_lock lock{caps_mutex};
                                            caps_state.status = "ERROR";
                                        }
                                        core::error(
                                            "CAPS seed discovery failed; no capability endpoints accepted");
                                        return;
                                    }

                                    core::info(
                                        "CAPS seed discovery succeeded; endpoints=" +
                                        std::to_string(discovery.capabilities.size()));

                                    std::string event_queue_uri;
                                    std::string simulator_features_uri;
                                    std::string environment_settings_uri;
                                    std::string ext_environment_uri;
                                    std::string simulator_features_message;

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.capability_count =
                                            static_cast<std::uint32_t>(
                                                discovery.capabilities.size());
                                        if(firestorm_region_lease.matches(
                                               firestorm_issued_epoch))
                                            caps_state.firestorm_caps =
                                                protocol::firestorm_caps_availability(
                                                    discovery.capabilities);
                                        caps_state.status = "CAPS READY";
                                    }

                                    // A separate read-only request must NOT
                                    // block EventQueue startup. The nested future
                                    // is joined when this CAPS worker exits.
                                    std::future<void> experiences_read_future;
                                    const auto owned=discovery.capabilities.find(
                                        "AgentExperiences");
                                    const auto lists=discovery.capabilities.find(
                                        "GetExperiences");
                                    const std::string owned_url=
                                        owned!=discovery.capabilities.end()
                                            ? owned->second : "";
                                    const std::string lists_url=
                                        lists!=discovery.capabilities.end()
                                            ? lists->second : "";
                                    if((!owned_url.empty()||!lists_url.empty()) &&
                                        !caps_stop.load())
                                    {
                                        experiences_read_future=std::async(
                                            std::launch::async,
                                            [transport,owned_url,lists_url,
                                             firestorm_issued_epoch,
                                             &firestorm_region_lease,
                                             &caps_stop,&caps_state,&caps_mutex]()
                                            {
                                                protocol::CapabilityService read_service{
                                                    transport};
                                                if(!owned_url.empty())
                                                {
                                                    const auto response=
                                                        read_service.fetch_llsd(
                                                            owned_url);
                                                    if(caps_stop.load())return;
                                                    if(response.success)
                                                    {
                                                        const auto parsed=
                                                            protocol::parse_agent_experiences(
                                                                response.value);
                                                        if(parsed && firestorm_region_lease.matches(
                                                               firestorm_issued_epoch))
                                                        {
                                                            std::scoped_lock lock{
                                                                caps_mutex};
                                                            if(!firestorm_region_lease.matches(
                                                                   firestorm_issued_epoch))
                                                                return;
                                                            caps_state.owned_ids=parsed->ids;
                                                            caps_state.owned_experiences_read=true;
                                                            caps_state.owned_experiences=
                                                                parsed->ids.size();
                                                            caps_state.experience_acquire_offered=
                                                                parsed->purchase_price.has_value();
                                                        }
                                                        else
                                                            core::warning(
                                                                "Firestorm AgentExperiences LLSD invalid");
                                                    }
                                                    else
                                                        core::warning(
                                                            "Firestorm AgentExperiences GET unavailable");
                                                }
                                                if(caps_stop.load() ||
                                                   lists_url.empty())return;
                                                const auto lists_response=
                                                    read_service.fetch_llsd(lists_url);
                                                if(caps_stop.load())return;
                                                if(lists_response.success)
                                                {
                                                    const auto parsed=
                                                        protocol::parse_resident_experiences(
                                                            lists_response.value);
                                                    if(parsed && firestorm_region_lease.matches(
                                                           firestorm_issued_epoch))
                                                    {
                                                        std::scoped_lock lock{
                                                            caps_mutex};
                                                        if(!firestorm_region_lease.matches(
                                                               firestorm_issued_epoch))
                                                            return;
                                                        caps_state.allowed_ids=parsed->allowed;
                                                        caps_state.blocked_ids=parsed->blocked;
                                                        caps_state.resident_experiences_read=true;
                                                        caps_state.allowed_experiences=
                                                            parsed->allowed.size();
                                                        caps_state.blocked_experiences=
                                                            parsed->blocked.size();
                                                    }
                                                    else
                                                        core::warning(
                                                            "Firestorm GetExperiences LLSD invalid");
                                                }
                                                else
                                                    core::warning(
                                                        "Firestorm GetExperiences GET unavailable");
                                            });
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("SimulatorFeatures");
                                        found != discovery.capabilities.end())
                                    {
                                        simulator_features_uri = found->second;
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("EventQueueGet");
                                        found != discovery.capabilities.end())
                                    {
                                        event_queue_uri = found->second;
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("EnvironmentSettings");
                                        found != discovery.capabilities.end())
                                    {
                                        environment_settings_uri = found->second;
                                    }

                                    if (const auto found =
                                            discovery.capabilities.find("ExtEnvironment");
                                        found != discovery.capabilities.end())
                                    {
                                        ext_environment_uri = found->second;
                                    }

                                    if (!simulator_features_uri.empty())
                                    {
                                        const auto features =
                                            service.fetch_simulator_features(
                                                simulator_features_uri);

                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.simulator_features_loaded =
                                            features.success;
                                        simulator_features_message = features.message;
                                        if (!features.success)
                                        {
                                            caps_state.status = "CAPS PARTIAL";
                                        }
                                    }

                                    if (!simulator_features_uri.empty())
                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        if (caps_state.simulator_features_loaded)
                                        {
                                            core::info("CAPS SimulatorFeatures loaded successfully");
                                        }
                                        else
                                        {
                                            core::warning(
                                                "CAPS SimulatorFeatures request failed: " +
                                                simulator_features_message);
                                        }
                                    }
                                    else
                                    {
                                        core::warning("CAPS SimulatorFeatures endpoint not advertised");
                                    }

                                    auto apply_environment =
                                        [&world_model,&world_mutex,
                                         &firestorm_region_lease,
                                         firestorm_issued_epoch](
                                            const protocol::WindLightEnvironment& parsed)
                                        {
                                            if(!firestorm_region_lease.matches(
                                                   firestorm_issued_epoch))return;
                                            std::scoped_lock lock{world_mutex};
                                            if(!firestorm_region_lease.matches(
                                                   firestorm_issued_epoch))return;
                                            world::EnvironmentState state =
                                                world_model.environment();
                                            ++state.revision;
                                            state.available = true;
                                            state.ambient = parsed.ambient;
                                            state.blue_density =
                                                parsed.blue_density;
                                            state.blue_horizon =
                                                parsed.blue_horizon;
                                            state.sunlight_color =
                                                parsed.sunlight_color;
                                            state.cloud_color =
                                                parsed.cloud_color;
                                            state.water_fog_color =
                                                parsed.water_fog_color;
                                            state.haze_density =
                                                parsed.haze_density;
                                            state.haze_horizon =
                                                parsed.haze_horizon;
                                            state.density_multiplier =
                                                parsed.density_multiplier;
                                            state.distance_multiplier =
                                                parsed.distance_multiplier;
                                            state.gamma = parsed.gamma;
                                            state.star_brightness =
                                                parsed.star_brightness;
                                            state.sun_angle =
                                                parsed.sun_angle;
                                            state.east_angle =
                                                parsed.east_angle;
                                            state.cloud_shadow =
                                                parsed.cloud_shadow;
                                            state.water_fog_density =
                                                parsed.water_fog_density;
                                            state.sky_name =
                                                parsed.sky_name;
                                            state.water_normal_map =
                                                parsed.water_normal_map;
                                            world_model.set_environment(
                                                std::move(state));
                                        };

                                    bool environment_loaded = false;

                                    if (!ext_environment_uri.empty())
                                    {
                                        const auto environment =
                                            service.fetch_llsd(
                                                ext_environment_uri);
                                        if (environment.success)
                                        {
                                            const auto parsed =
                                                protocol::parse_ext_environment_settings(
                                                    environment.value);
                                            if (parsed.has_value())
                                            {
                                                {
                                                    std::scoped_lock lock{caps_mutex};
                                                    ext_environment_payload =
                                                        environment.value;
                                                }
                                                apply_environment(*parsed);
                                                environment_loaded = true;
                                                core::info(
                                                    "CAPS ExtEnvironment EEP parsed successfully");
                                            }
                                            else
                                            {
                                                core::warning(
                                                    "CAPS ExtEnvironment format unsupported; trying legacy EnvironmentSettings");
                                            }
                                        }
                                        else
                                        {
                                            core::warning(
                                                "CAPS ExtEnvironment request failed; trying legacy EnvironmentSettings");
                                        }
                                    }

                                    if (!environment_loaded &&
                                        !environment_settings_uri.empty())
                                    {
                                        const auto environment =
                                            service.fetch_llsd(
                                                environment_settings_uri);
                                        if (environment.success)
                                        {
                                            const auto parsed =
                                                protocol::parse_environment_settings(
                                                    environment.value);
                                            if (parsed.has_value())
                                            {
                                                apply_environment(*parsed);
                                                environment_loaded = true;
                                                core::info(
                                                    "CAPS EnvironmentSettings parsed successfully");
                                            }
                                            else
                                            {
                                                core::warning(
                                                    "CAPS EnvironmentSettings format unsupported; renderer fallback remains active");
                                            }
                                        }
                                        else
                                        {
                                            core::warning(
                                                "CAPS EnvironmentSettings request failed: " +
                                                environment.message);
                                        }
                                    }

                                    if (!environment_loaded &&
                                        ext_environment_uri.empty() &&
                                        environment_settings_uri.empty())
                                    {
                                        core::warning(
                                            "CAPS environment endpoints not advertised; renderer fallback remains active");
                                    }

                                    if (event_queue_uri.empty())
                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.event_queue_available = false;
                                        if (caps_state.status != "ERROR")
                                        {
                                            caps_state.status = "CAPS READY";
                                        }
                                        core::warning("CAPS EventQueueGet endpoint not advertised");
                                        return;
                                    }

                                    core::info("CAPS EventQueueGet endpoint available; long-poll starting");

                                    {
                                        std::scoped_lock lock{caps_mutex};
                                        caps_state.event_queue_available = true;
                                        caps_state.status = "EVENTQUEUE";
                                    }

                                    std::optional<std::int32_t> acknowledgement;
                                    protocol::EventQueueRecovery recovery;
                                    bool event_queue_live_logged = false;
                                    bool idle_logged = false;

                                    auto pause_until_retry =
                                        [&caps_stop](std::chrono::milliseconds duration)
                                        {
                                            constexpr auto step =
                                                std::chrono::milliseconds{100};
                                            while (!caps_stop.load() &&
                                                   duration.count() > 0)
                                            {
                                                const auto next = std::min(duration, step);
                                                std::this_thread::sleep_for(next);
                                                duration -= next;
                                            }
                                        };

                                    // Never keep polling the old region's
                                    // EventQueue after a crossing/teleport.
                                    const auto still_origin=[&]()
                                    {
                                        if(!firestorm_region_lease.matches(
                                               firestorm_issued_epoch))
                                            return false;
                                        std::scoped_lock lock{world_mutex};
                                        return world_model.region().handle==
                                                   initial_handle &&
                                            world_model.region().seed_capability==
                                                   seed_capability;
                                    };
                                    while (!caps_stop.load())
                                    {
                                        if(!still_origin())break;
                                        const auto batch = service.poll_event_queue(
                                            event_queue_uri,
                                            acknowledgement,
                                            false);

                                        if (caps_stop.load()||!still_origin())
                                        {
                                            break;
                                        }

                                        // OpenSimulator uses HTTP 502 for an empty
                                        // long-poll after its ~58-second keepalive.
                                        // This is healthy activity, not a failure.
                                        if (batch.idle)
                                        {
                                            const auto recovered = recovery.failures();
                                            recovery.on_contact();
                                            {
                                                std::scoped_lock lock{caps_mutex};
                                                caps_state.status = event_queue_live_logged
                                                    ? "CAPS LIVE" : "CAPS IDLE";
                                            }
                                            if (!idle_logged)
                                            {
                                                idle_logged = true;
                                                core::info(
                                                    "CAPS EventQueue idle (HTTP 502 normal "
                                                    "server keepalive); polling continues");
                                            }
                                            if (recovered > 0U)
                                            {
                                                core::info(
                                                    "CAPS EventQueue recovered after " +
                                                    std::to_string(recovered) +
                                                    " failure(s); acknowledgement preserved");
                                            }
                                            // Guard against an endpoint returning 502
                                            // instantly rather than long-polling.
                                            pause_until_retry(std::chrono::milliseconds{150});
                                            continue;
                                        }

                                        if (batch.success)
                                        {
                                            const auto recovered = recovery.failures();
                                            acknowledgement = batch.id;
                                            recovery.on_contact();

                                            {
                                                std::scoped_lock lock{caps_mutex};
                                                caps_state.event_count +=
                                                    static_cast<std::uint32_t>(
                                                        batch.events.size());
                                                caps_state.status = "CAPS LIVE";
                                            }

                                            if (!event_queue_live_logged)
                                            {
                                                event_queue_live_logged = true;
                                                core::info(
                                                    "CAPS EventQueue live; first successful batch received");
                                            }
                                            if (recovered > 0U)
                                            {
                                                core::info(
                                                    "CAPS EventQueue recovered after " +
                                                    std::to_string(recovered) +
                                                    " failure(s); acknowledgement updated");
                                            }

                                            for (const auto& event : batch.events)
                                            {
                                                if(!still_origin())break;
                                                if(event.message=="BulkUpdateInventory")
                                                    (void)inventory_events.enqueue(
                                                        event,inventory_owner,
                                                        firestorm_issued_epoch,
                                                        firestorm_region_lease);
                                                (void)caps_event_dispatcher.dispatch(event);
                                            }
                                            continue;
                                        }

                                        const auto decision =
                                            recovery.on_failure(batch.expired);
                                        {
                                            std::scoped_lock lock{caps_mutex};
                                            caps_state.status =
                                                decision.failures >= 5U
                                                    ? "CAPS DEGRADED"
                                                    : "CAPS RETRY";
                                        }

                                        core::warning(
                                            "CAPS EventQueue poll failed; retry=" +
                                            std::to_string(decision.failures) +
                                            "; HTTP=" + std::to_string(batch.http_status) +
                                            "; cause=" + batch.message +
                                            "; acknowledgement " +
                                            (acknowledgement.has_value()
                                                ? "preserved" : "not established"));

                                        if (decision.refresh_seed && !caps_stop.load())
                                        {
                                            core::info(
                                                "CAPS EventQueue rediscovering endpoint from "
                                                "active login seed");
                                            const auto refreshed = service.discover(
                                                seed_capability, {"EventQueueGet"});
                                            if (caps_stop.load())
                                            {
                                                break;
                                            }
                                            if (refreshed.success)
                                            {
                                                const auto found = refreshed.capabilities.find(
                                                    "EventQueueGet");
                                                if (found != refreshed.capabilities.end() &&
                                                    !found->second.empty())
                                                {
                                                    if (event_queue_uri != found->second)
                                                    {
                                                        event_queue_uri = found->second;
                                                        acknowledgement.reset();
                                                        event_queue_live_logged = false;
                                                        idle_logged = false;
                                                        core::info(
                                                            "CAPS EventQueue endpoint refreshed; "
                                                            "acknowledgement reset for new queue");
                                                    }
                                                    else
                                                    {
                                                        core::info(
                                                            "CAPS EventQueue seed refreshed; "
                                                            "endpoint unchanged");
                                                    }
                                                }
                                                else
                                                {
                                                    core::warning(
                                                        "CAPS EventQueue seed response did "
                                                        "not advertise EventQueueGet");
                                                }
                                            }
                                            else
                                            {
                                                core::warning(
                                                    "CAPS EventQueue seed rediscovery failed: " +
                                                    refreshed.message);
                                            }
                                        }

                                        pause_until_retry(decision.pause);
                                    }
                                });

                            core::info(
                                "CAPS discovery started from login seed capability");
                        }

                        if (!circuit_started && active_session.has_value())
                        {
                            circuit_started = true;
                            circuit_target_ip=active_session->simulator_ip;
                            circuit_target_port=active_session->simulator_port;
                            launch_circuit(*active_session);
                        }
                    }
                    else
                    {
                        login_ui.status = "ERROR";
                        login_ui.notice = result.message.empty()
                            ? "OpenSim-Anmeldung fehlgeschlagen."
                            : result.message;
                        core::warning(
                            std::string{"OpenSim login failed: "} +
                            (result.reason.empty() ? "unknown" : result.reason));
                    }
                }
                catch (const std::exception& error)
                {
                    login_ui.status = "ERROR";
                    login_ui.notice = "Anmeldevorgang konnte nicht abgeschlossen werden.";
                    core::error(std::string{"OpenSim login worker failed: "} + error.what());
                }
            }

            if (login_ui.submit_requested)
            {
                login_ui.submit_requested = false;

                if (login_ui.busy || login_ui.authenticated)
                {
                    // Ignore duplicate submit while a login is active or already complete.
                }
                else if (active_login_uri.empty() ||
                         (grid_status != "LIVE" && grid_status != "CACHE"))
                {
                    login_ui.status = "ERROR";
                    login_ui.notice = "Keine gueltige Grid-Konfiguration verfuegbar.";
                }
                else if (login_ui.username.empty() || login_ui.password.empty())
                {
                    login_ui.status = "READY";
                    login_ui.notice = "Avatarname und Passwort eingeben.";
                }
                else
                {
                    const auto avatar_name = split_avatar_name(login_ui.username);
                    if (!avatar_name.has_value())
                    {
                        login_ui.status = "READY";
                        login_ui.notice = "Avatarname als Vorname Nachname eingeben.";
                    }
                    else
                    {
                        protocol::LoginCredentials credentials;
                        credentials.first_name = avatar_name->first;
                        credentials.last_name = avatar_name->second;
                        credentials.password = std::move(login_ui.password);
                        login_ui.password.clear();
                        credentials.start = "last";

                        const std::string login_uri = active_login_uri;
                        const std::string viewer_version{core::version()};

                        login_ui.busy = true;
                        login_ui.status = "CONNECTING";
                        login_ui.notice = "Anmeldung bei NexVerse wird ausgefuehrt...";

                        login_future = std::async(
                            std::launch::async,
                            [transport, login_uri, credentials = std::move(credentials), viewer_version]() mutable
                            {
                                protocol::LoginService service{transport};
                                protocol::LoginResult result = service.login(
                                    login_uri,
                                    credentials,
                                    "OpenGenesisLINK Viewer",
                                    viewer_version);

                                std::fill(
                                    credentials.password.begin(),
                                    credentials.password.end(),
                                    '\0');
                                credentials.password.clear();

                                return result;
                            });
                    }
                }
            }

            // TeleportFinish/CrossedRegion updates the WorldModel with
            // the destination simulator. Reconnect LLUDP to that simulator
            // without ever allowing simultaneous circuit workers to use
            // the shared packet dispatcher or command queue.
            if(circuit_started && active_session)
            {
                std::string destination_ip;
                std::uint16_t destination_port=0U;
                {
                    std::scoped_lock lock{world_mutex};
                    destination_ip=world_model.region().simulator_ip;
                    destination_port=world_model.region().simulator_port;
                }
                const bool old_finished=!circuit_future.valid() ||
                    circuit_future.wait_for(std::chrono::seconds{0})==
                        std::future_status::ready;
                const auto handoff=protocol::region_handoff_action(
                    circuit_target_ip,circuit_target_port,
                    destination_ip,destination_port,old_finished);
                if(handoff==protocol::RegionHandoffAction::wait_for_old_worker ||
                   handoff==protocol::RegionHandoffAction::launch_new_worker)
                {
                    // Drain an old UDP transport and queued movement/chat
                    // commands before opening the destination socket.
                    circuit_stop.store(true);
                    if(handoff==protocol::RegionHandoffAction::launch_new_worker)
                    {
                        if(circuit_future.valid())
                            (void)circuit_future.get();
                        outbound_queue.clear();
                        movement_controller.reset();
                        movement_feedback.reset();
                        movement_heading_initialized=false;
                        last_reported_control_flags=0U;
                        {
                            std::scoped_lock lock{world_mutex};
                            self_avatar_local_id=0U;
                            self_avatar_region_handle=0U;
                        }
                        circuit_target_ip=destination_ip;
                        circuit_target_port=destination_port;
                        protocol::LoginSession next=*active_session;
                        next.simulator_ip=destination_ip;
                        next.simulator_port=destination_port;
                        circuit_stop.store(false);
                        launch_circuit(std::move(next));
                        core::info(
                            "LLUDP region destination changed; a new "
                            "UseCircuitCode/CompleteAgentMovement handshake started");
                    }
                }
            }

            protocol::CircuitState current_circuit_state;
            {
                std::scoped_lock lock{circuit_mutex};
                current_circuit_state = circuit_status.state;
            }

            if (login_ui.authenticated && active_session.has_value())
            {
                const bool circuit_active =
                    current_circuit_state == protocol::CircuitState::active;

                std::scoped_lock lock{world_mutex};
                const world::RegionState region_snapshot = world_model.region();
                const bool region_circuit_active =
                    protocol::region_circuit_ready(
                        circuit_target_ip,circuit_target_port,
                        region_snapshot.simulator_ip,
                        region_snapshot.simulator_port,
                        circuit_active,circuit_stop.load());

                world_model.apply_region({
                    region_snapshot.handle,
                    region_snapshot.name,
                    region_snapshot.agent_position,
                    region_snapshot.simulator_ip,
                    region_snapshot.simulator_port,
                    region_snapshot.seed_capability,
                    region_snapshot.region_id,
                    region_snapshot.region_flags,
                    region_snapshot.region_flags_extended,
                    region_snapshot.region_protocols,
                    region_snapshot.sim_access,
                    region_snapshot.water_height,
                    region_snapshot.channel_version,
                    region_snapshot.size_x,
                    region_snapshot.size_y,
                    region_circuit_active});

                const world::AgentState agent_snapshot = world_model.agent();
                world_model.apply_agent({
                    agent_snapshot.agent_id,
                    agent_snapshot.session_id,
                    agent_snapshot.position,
                    agent_snapshot.rotation,
                    region_circuit_active});

                simulation_ready = world_model.ready();
            }
            else
            {
                simulation_ready = false;
            }

            if (!simulation_ready)
            {
                viewer_shell.close();
                world_ready_logged = false;
                experience_ui.close();
                outfit_gallery_open=false;
            }
            else if (!world_ready_logged)
            {
                world_ready_logged = true;

                world::WorldSnapshot snapshot;
                {
                    std::scoped_lock lock{world_mutex};
                    snapshot = world_model.snapshot();
                }

                core::info(
                    "WorldModel READY; simulation UI may unlock; terrain_patches=" +
                    std::to_string(snapshot.terrain.valid_patch_count) +
                    "; objects=" + std::to_string(snapshot.objects.size()) +
                    "; avatars=" + std::to_string(snapshot.avatars.size()) +
                    "; parcel=" +
                    std::string{snapshot.parcel.available ? "OK" : "-"} +
                    "; environment=" +
                    std::string{snapshot.environment.available ? "OK" : "-"});
            }

            const auto now = std::chrono::steady_clock::now();
            if (simulation_ready &&
                login_ui.authenticated &&
                current_circuit_state == protocol::CircuitState::active &&
                now - last_agent_update >= std::chrono::milliseconds{100})
            {
                const bool focused =
                    glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
                const auto pressed = [&](int key)
                {
                    return focused && !viewer_shell.input_captured() &&
                           !experience_ui.open &&
                           !outfit_gallery_open&&!inventory_tree_open &&
                           glfwGetKey(window, key) == GLFW_PRESS;
                };
                protocol::MovementKeys keys;
                keys.forward = pressed(GLFW_KEY_W) || pressed(GLFW_KEY_UP);
                keys.backward = pressed(GLFW_KEY_S) ||
                                pressed(GLFW_KEY_DOWN);
                keys.left = pressed(GLFW_KEY_A);
                keys.right = pressed(GLFW_KEY_D);
                keys.turn_left = pressed(GLFW_KEY_LEFT);
                keys.turn_right = pressed(GLFW_KEY_RIGHT);
                keys.jump = pressed(GLFW_KEY_SPACE);
                keys.descend = pressed(GLFW_KEY_C);
                keys.sprint = pressed(GLFW_KEY_LEFT_SHIFT) ||
                              pressed(GLFW_KEY_RIGHT_SHIFT);
                keys.toggle_fly = pressed(GLFW_KEY_F);

                world::AgentState agent_for_camera;
                {
                    std::scoped_lock lock{world_mutex};
                    agent_for_camera = world_model.agent();
                }
                if (!movement_heading_initialized)
                {
                    movement_controller.reset(agent_for_camera.rotation);
                    movement_heading_initialized = true;
                    core::info("Avatar controls active: WASD, arrow-turn, "
                               "Shift fast, Space jump, F flight toggle");
                }
                auto update = movement_controller.step(
                    keys, std::chrono::duration<float>(
                        now - last_agent_update).count());
                movement_feedback.outbound_controls(update.control_flags,now);
                agent_for_camera.rotation = update.body_rotation;
                {
                    std::scoped_lock lock{world_mutex};
                    const auto authoritative = world_model.agent();
                    world_model.apply_agent({
                        authoritative.agent_id,
                        authoritative.session_id,
                        authoritative.position,
                        update.body_rotation,
                        authoritative.present});
                }
                const CameraState camera =
                    camera_from_agent(agent_for_camera);
                update.camera_center = camera.center;
                update.camera_at_axis = camera.at;
                update.camera_left_axis = camera.left;
                update.camera_up_axis = camera.up;
                update.far_clip = camera.far_clip;

                if (update.control_flags != last_reported_control_flags)
                {
                    last_reported_control_flags = update.control_flags;
                    core::info("AgentUpdate controls changed: " +
                        std::to_string(update.control_flags) +
                        (movement_controller.flying()
                            ? " (flying)" : ""));
                }
                (void)outbound_queue.push(std::move(update));
                last_agent_update = now;
            }

            int framebuffer_width = 0;
            int framebuffer_height = 0;
            glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
            glfwGetWindowSize(window, &window_width, &window_height);

            if (framebuffer_width <= 0 || framebuffer_height <= 0 ||
                window_width <= 0 || window_height <= 0)
            {
                continue;
            }

            welcome_page.resize(window_width, window_height);
            welcome_page.pump();
            glfwMakeContextCurrent(window);

            const ui::WelcomeFrame& welcome_frame = welcome_page.frame();
            if (welcome_frame.ready &&
                !welcome_frame.rgba.empty() &&
                welcome_frame.width > 0 &&
                welcome_frame.height > 0)
            {
                if (welcome_texture == 0U)
                {
                    welcome_texture = renderer.create_rgba_texture(
                        welcome_frame.width,
                        welcome_frame.height,
                        welcome_frame.rgba.data());
                    welcome_texture_width = welcome_frame.width;
                    welcome_texture_height = welcome_frame.height;
                    if (welcome_texture != 0U)
                    {
                        core::info(
                            std::string{"NexVerse welcome background loaded: "} +
                            std::string{ui::nexverse_welcome_url});
                    }
                }
                else if (welcome_texture_width != welcome_frame.width ||
                         welcome_texture_height != welcome_frame.height)
                {
                    if (renderer.update_rgba_texture(
                            welcome_texture,
                            welcome_frame.width,
                            welcome_frame.height,
                            welcome_frame.rgba.data()))
                    {
                        welcome_texture_width = welcome_frame.width;
                        welcome_texture_height = welcome_frame.height;
                    }
                }
            }
            else if (welcome_frame.failed && !welcome_failure_logged)
            {
                welcome_failure_logged = true;
                core::warning(
                    std::string{"NexVerse welcome background unavailable: "} +
                    welcome_frame.status);
            }

            world_renderer->resize(
                framebuffer_width,
                framebuffer_height);

            world::AgentState render_agent;
            render::RenderTerrain terrain_update;
            render::RenderEnvironment environment_update;
            std::vector<render::RenderSceneObject> scene_object_update;
            std::vector<render::RenderAvatarProxy> avatar_proxy_update;
            std::vector<render::RenderPointLight> scene_point_lights;
            std::vector<render::RenderParticleEmitter> scene_particle_emitters;
            std::vector<std::string> scene_texture_update;
            bool terrain_changed = false;
            bool environment_changed = false;
            bool scene_objects_changed = false;
            bool terrain_materials_available = false;
            std::uint32_t texture_generation_current = 0U;
            std::uint32_t texture_material_revision_current = 0U;
            std::array<std::string, 4> terrain_detail_ids;

            // A new WorldModel region generation, handle or seed always
            // invalidates both the material cache and pending HTTP epochs.
            // Never destroy an unfinished std::async future on the UI thread:
            // old replies are consumed later and discarded by epoch.
            std::uint64_t current_region_handle = 0U;
            std::uint32_t current_region_generation = 0U;
            std::string current_region_seed;
            std::string current_region_simulator_ip;
            std::uint16_t current_region_simulator_port=0U;
            {
                std::scoped_lock lock{world_mutex};
                const auto& active_region = world_model.region();
                current_region_handle = active_region.handle;
                current_region_generation = active_region.generation;
                current_region_seed = active_region.seed_capability;
                current_region_simulator_ip=active_region.simulator_ip;
                current_region_simulator_port=active_region.simulator_port;
            }
            const auto previous_firestorm_epoch=firestorm_region_lease.epoch();
            const auto current_firestorm_epoch=firestorm_region_lease.enter(
                current_region_handle,current_region_generation,
                current_region_seed);
            gallery_inventory_sync.enter(current_firestorm_epoch);
            if(active_session&&inventory_tree.count()==0U)
            {
                inventory_tree=protocol::inventory_tree_from_login(
                    active_session->inventory_skeleton);
                inventory_parent=active_session->inventory_root_id;
            }
            if(current_firestorm_epoch!=previous_firestorm_epoch &&
               previous_firestorm_epoch!=0U)
            {
                std::scoped_lock lock{caps_mutex};
                caps_state.firestorm_caps={};
                caps_state.owned_experiences_read=false;
                caps_state.owned_experiences=0U;
                caps_state.experience_acquire_offered=false;
                caps_state.resident_experiences_read=false;
                caps_state.allowed_experiences=0U;
                caps_state.blocked_experiences=0U;
                caps_state.owned_ids.clear();
                caps_state.allowed_ids.clear();
                caps_state.blocked_ids.clear();
                caps_state.experience_names.clear();
                caps_state.experience_profiles.clear();
                caps_state.outfit_inventory_read=false;
                caps_state.outfit_folders_read=0U;
                caps_state.outfit_thumbnail_assets.clear();
                caps_state.outfit_folder_items.clear();
                caps_state.current_outfit_folder.clear();
                caps_state.inventory_dynamic_folders.clear();
                caps_state.resolved_outfit_items.clear();
                caps_state.resolved_outfit_folder.clear();
                gallery_item_fetch_folder.clear();
                gallery_item_fetch_completed_folder.clear();
                gallery_inventory_items_cap.clear();
                caps_state.experience_permission_status.clear();
                experience_permission_cap.clear();
                experience_consent.cancel();
                approved_experience_permission.reset();
                gallery_texture_cap.clear();
                gallery_inventory_cap.clear();
                gallery_image_requested.clear();
                for(const auto& [folder,image]:gallery_gpu_textures)
                {
                    (void)folder;
                    renderer.destroy_texture(image.second);
                }
                gallery_gpu_textures.clear();
                outfit_gallery_open=false;
                self_avatar_proxy_gl_logged=false;
                self_avatar_bake_gl_logged=false;
                self_avatar_original_mesh_gl_logged=false;
                self_avatar_weighted_skin_gl_logged=false;
                inventory_tree_open=false;
                inventory_fetched_folders.clear();
                inventory_folder_contents.clear();
                inventory_last_requested.clear();
                inventory_refresh_requested=false;
                inventory_dirty_revision=0U;
                inventory_events.reset();
                gallery_force_refresh=false;
                inventory_item_selection=0U;
                inventory_item_detail_requested=false;
                inventory_item_detail.reset();
                inventory_item_detail_folder.clear();
                inventory_item_detail_id.clear();
                inventory_item_detail_status.clear();
                if(active_session)
                {
                    inventory_tree=protocol::inventory_tree_from_login(
                        active_session->inventory_skeleton);
                    inventory_parent=active_session->inventory_root_id;
                }
                experience_ui.close();
            }
            // BulkUpdateInventory over the current EventQueue is a
            // server-origin change *hint*, never direct trusted content.
            // Main-thread only: invalidate precisely affected read caches.
            const auto dirty=inventory_events.drain(
                current_firestorm_epoch,firestorm_region_lease);
            if(!dirty.empty())
            {
                ++inventory_dirty_revision;
                if(dirty.full_refresh)
                {
                    inventory_folder_contents.clear();
                    inventory_last_requested.clear();
                    inventory_fetched_folders.clear();
                }
                else
                {
                    for(const auto& folder:dirty.folders)
                    {
                        inventory_folder_contents.erase(folder);
                        inventory_last_requested.erase(folder);
                    }
                }
                inventory_item_detail.reset();
                inventory_item_detail_id.clear();
                inventory_item_detail_status="Inventaraenderung: neu laden";
                gallery_force_refresh=true;
                core::info("Firestorm inventory change hint accepted; folders="+
                    std::to_string(dirty.folders.size())+
                    (dirty.full_refresh?"; full resync":"; targeted refresh"));
            }
            // After a simulator crossing, the original login seed is
            // revoked. Only the destination's new seed may provide a
            // replacement CAPS EventQueueGet endpoint. The replacement
            // starts when its LLUDP circuit has become active.
            for(auto it=region_event_queue_futures.begin();
                it!=region_event_queue_futures.end();)
            {
                if(it->wait_for(std::chrono::seconds{0})!=
                   std::future_status::ready){++it;continue;}
                try
                {
                    const auto result=it->get();
                    core::info("Region EventQueue worker stopped; events="+
                        std::to_string(result.accepted_events)+
                        "; endpoint="+
                        (result.advertised?"advertised":"unavailable"));
                }
                catch(const std::exception&)
                {
                    core::warning("Region EventQueue task failed");
                }
                it=region_event_queue_futures.erase(it);
            }
            protocol::CircuitState live_circuit_state;
            {
                std::scoped_lock lock{circuit_mutex};
                live_circuit_state=circuit_status.state;
            }
            const bool destination_circuit_active=
                live_circuit_state==protocol::CircuitState::active &&
                !circuit_stop.load() &&
                !current_region_seed.empty() &&
                current_region_simulator_ip==circuit_target_ip &&
                current_region_simulator_port==circuit_target_port &&
                current_region_handle!=0U &&active_session.has_value();
            if(destination_circuit_active&&
               current_firestorm_epoch!=0U &&
               current_firestorm_epoch!=login_caps_epoch &&
               current_firestorm_epoch!=region_event_started_epoch &&
               region_event_queue_futures.size()<4U)
            {
                region_event_started_epoch=current_firestorm_epoch;
                region_event_queue_futures.emplace_back(
                    std::async(std::launch::async,
                        [transport,seed=current_region_seed,
                         owner=active_session->agent_id,
                         epoch=current_firestorm_epoch,
                         &firestorm_region_lease,&caps_stop,
                         &caps_event_dispatcher,&inventory_events,
                         &caps_mutex,&caps_state]()
                        {
                            auto receive=[&](const protocol::EventQueueEvent& event)
                            {
                                if(caps_stop.load()||
                                   !firestorm_region_lease.matches(epoch))
                                    return;
                                if(event.message=="BulkUpdateInventory")
                                    (void)inventory_events.enqueue(
                                        event,owner,epoch,
                                        firestorm_region_lease);
                                (void)caps_event_dispatcher.dispatch(event);
                                if(firestorm_region_lease.matches(epoch))
                                {
                                    std::scoped_lock lock{caps_mutex};
                                    if(firestorm_region_lease.matches(epoch))
                                    {
                                        ++caps_state.event_count;
                                        caps_state.status="CAPS LIVE";
                                    }
                                }
                            };
                            return protocol::poll_region_event_queue(
                                transport,seed,epoch,firestorm_region_lease,
                                caps_stop,receive);
                        }));
                core::info("New region CAPS EventQueue discovery started");
            }
            // Region-specific Firestorm read CAPS. The existing login
            // EventQueue remains independent and is never joined here.
            // No std::async future is destroyed while still running on the
            // render thread: only completed tasks are erased.
            for(auto it=firestorm_region_read_futures.begin();
                it!=firestorm_region_read_futures.end();)
            {
                if(it->wait_for(std::chrono::milliseconds{0})!=
                   std::future_status::ready)
                {
                    ++it;
                    continue;
                }
                try
                {
                    auto reply=it->get();
                    if(reply.discovered &&
                       firestorm_region_lease.matches(reply.issued_epoch))
                    {
                        std::scoped_lock lock{caps_mutex};
                        if(firestorm_region_lease.matches(reply.issued_epoch))
                        {
                            caps_state.firestorm_caps=reply.available;
                            if(reply.owned)
                            {
                                caps_state.owned_ids=reply.owned->ids;
                                caps_state.owned_experiences=reply.owned->ids.size();
                                caps_state.owned_experiences_read=true;
                                caps_state.experience_acquire_offered=
                                    reply.owned->purchase_price.has_value();
                            }
                            if(reply.resident)
                            {
                                caps_state.allowed_ids=reply.resident->allowed;
                                caps_state.blocked_ids=reply.resident->blocked;
                                caps_state.allowed_experiences=
                                    reply.resident->allowed.size();
                                caps_state.blocked_experiences=
                                    reply.resident->blocked.size();
                                caps_state.resident_experiences_read=true;
                            }
                            caps_state.experience_names=std::move(reply.display_names);
                            caps_state.experience_profiles=std::move(reply.profiles);
                            (void)gallery_inventory_sync.merge(
                                reply.issued_epoch,reply.outfit_inventory);
                            caps_state.outfit_inventory_read=
                                gallery_inventory_sync.read();
                            caps_state.outfit_folders_read=
                                reply.outfit_inventory.returned_folders;
                            caps_state.outfit_folder_items=
                                gallery_inventory_sync.items();
                            caps_state.inventory_dynamic_folders=
                                reply.outfit_inventory.categories;
                            if(inventory_tree.count()==0 &&active_session)
                            {
                                inventory_tree=protocol::inventory_tree_from_login(
                                    active_session->inventory_skeleton);
                                inventory_parent=active_session->inventory_root_id;
                            }
                            (void)protocol::merge_inventory_categories(
                                inventory_tree,reply.outfit_inventory);
                            caps_state.outfit_thumbnail_assets.clear();
                            for(const auto& [folder,asset]:
                                gallery_inventory_sync.thumbnails())
                                if(!asset.empty())
                                    caps_state.outfit_thumbnail_assets.emplace(
                                        folder,asset);
                            caps_state.current_outfit_folder=active_session
                                ?protocol::current_outfit_folder_id(
                                    active_session->inventory_skeleton).value_or("")
                                :"";
                            gallery_texture_cap=std::move(reply.get_texture_uri);
                            gallery_inventory_cap=std::move(
                                reply.inventory_folder_uri);
                            gallery_inventory_items_cap=std::move(
                                reply.inventory_items_uri);
                            experience_permission_cap=std::move(
                                reply.experience_permissions_uri);
                            last_gallery_sync=std::chrono::steady_clock::now();
                        }
                    }
                }
                catch(const std::exception&)
                {
                    core::warning("Firestorm region read-only discovery failed");
                }
                it=firestorm_region_read_futures.erase(it);
            }
            // A region may change while older tasks remain in flight.
            // At most four tasks are outstanding; if saturated, retry the
            // newest region on a subsequent frame without blocking UI.
            if(current_firestorm_epoch!=0U &&
               current_firestorm_epoch!=firestorm_region_read_started_epoch &&
               !current_region_seed.empty() &&current_region_handle!=0U &&
               firestorm_region_read_futures.size()<4U)
            {
                firestorm_region_read_started_epoch=current_firestorm_epoch;
                firestorm_region_read_futures.emplace_back(
                    std::async(std::launch::async,
                        [transport,seed=current_region_seed,
                         agent=active_session
                             ? active_session->agent_id : std::string{},
                         outfits=active_session
                             ? protocol::outfits_from_login_skeleton(
                                   active_session->inventory_skeleton)
                             : std::vector<protocol::OutfitGalleryFolder>{},
                         cof=active_session
                             ?protocol::current_outfit_folder_id(
                                 active_session->inventory_skeleton).value_or("")
                             :std::string{},
                         issued=current_firestorm_epoch,
                         &firestorm_region_lease]() mutable
                        {
                            if(outfits.size()>16U)outfits.resize(16U);
                            return protocol::fetch_firestorm_region(
                                transport,seed,issued,firestorm_region_lease,
                                agent,outfits,cof);
                        }));
            }
            // An Experience permission is written ONLY after two explicit
            // user actions: A/B to stage and ENTER to confirm. The
            // server's updated lists, not HTTP 200 alone, prove success.
            if(experience_permission_future.valid() &&
               experience_permission_future.wait_for(
                   std::chrono::milliseconds{0})==
                    std::future_status::ready)
            {
                experience_permission_busy=false;
                try
                {
                    const auto response=experience_permission_future.get();
                    if(firestorm_region_lease.matches(response.epoch))
                    {
                        std::scoped_lock lock{caps_mutex};
                        if(response.confirmed&&response.resident)
                        {
                            caps_state.allowed_ids=response.resident->allowed;
                            caps_state.blocked_ids=response.resident->blocked;
                            caps_state.allowed_experiences=
                                response.resident->allowed.size();
                            caps_state.blocked_experiences=
                                response.resident->blocked.size();
                            caps_state.resident_experiences_read=true;
                            caps_state.experience_permission_status=
                                "Berechtigung vom Server bestaetigt";
                        }
                        else
                            caps_state.experience_permission_status=
                                "Server hat die Aenderung nicht bestaetigt";
                    }
                }
                catch(const std::exception&)
                {
                    std::scoped_lock lock{caps_mutex};
                    caps_state.experience_permission_status=
                        "Berechtigungs-Anfrage fehlgeschlagen";
                }
            }
            if(approved_experience_permission)
            {
                auto approval=std::move(*approved_experience_permission);
                approved_experience_permission.reset();
                if(!experience_permission_busy &&
                   !experience_permission_future.valid() &&
                   !experience_permission_cap.empty() &&
                   firestorm_region_lease.matches(approval.epoch))
                {
                    experience_permission_busy=true;
                    {
                        std::scoped_lock lock{caps_mutex};
                        caps_state.experience_permission_status=
                            "Berechtigung wird serverseitig geprueft";
                    }
                    experience_permission_future=std::async(
                        std::launch::async,
                        [transport,cap=experience_permission_cap,
                         request=std::move(approval),
                         &firestorm_region_lease]()
                        {
                            return protocol::write_experience_permission(
                                transport,cap,request,
                                firestorm_region_lease);
                        });
                }
                else
                {
                    std::scoped_lock lock{caps_mutex};
                    caps_state.experience_permission_status=
                        "Keine gueltige aktuelle Berechtigungs-CAP";
                }
            }
            // Refresh InventoryDescendents2 while the gallery is visible.
            // Never block the renderer on HTTP or auto-mutate inventory.
            if(gallery_sync_future.valid() &&
               gallery_sync_future.wait_for(std::chrono::milliseconds{0})==
                   std::future_status::ready)
            {
                try
                {
                    auto update=gallery_sync_future.get();
                    if(firestorm_region_lease.matches(
                           gallery_sync_issued_epoch) &&update.valid &&
                       gallery_sync_dirty_revision==inventory_dirty_revision)
                    {
                        std::scoped_lock lock{caps_mutex};
                        if(gallery_inventory_sync.merge(
                            gallery_sync_issued_epoch,update))
                        {
                            caps_state.outfit_folder_items=
                                gallery_inventory_sync.items();
                            for(const auto& [id,folder]:update.categories)
                                caps_state.inventory_dynamic_folders.insert_or_assign(
                                    id,folder);
                            (void)protocol::merge_inventory_categories(
                                inventory_tree,update);
                            gallery_item_fetch_completed_folder.clear();
                            gallery_item_fetch_folder.clear();
                            caps_state.resolved_outfit_items.clear();
                            caps_state.resolved_outfit_folder.clear();
                            caps_state.outfit_thumbnail_assets.clear();
                            for(const auto& [folder,asset]:
                                gallery_inventory_sync.thumbnails())
                                if(!asset.empty())
                                    caps_state.outfit_thumbnail_assets.emplace(
                                        folder,asset);
                            for(auto it=gallery_image_requested.begin();
                                it!=gallery_image_requested.end();)
                            {
                                const auto latest=caps_state.outfit_thumbnail_assets.find(
                                    it->first);
                                if(latest==caps_state.outfit_thumbnail_assets.end()||
                                   latest->second!=it->second)
                                    it=gallery_image_requested.erase(it);
                                else ++it;
                            }
                            for(auto it=gallery_gpu_textures.begin();
                                it!=gallery_gpu_textures.end();)
                            {
                                const auto latest=caps_state.outfit_thumbnail_assets.find(
                                    it->first);
                                if(latest==caps_state.outfit_thumbnail_assets.end()||
                                   latest->second!=it->second.first)
                                {
                                    renderer.destroy_texture(it->second.second);
                                    it=gallery_gpu_textures.erase(it);
                                }
                                else ++it;
                            }
                        }
                    }
                }
                catch(const std::exception&)
                {
                    core::warning("Firestorm inventory read-only refresh failed");
                }
            }
            if(outfit_gallery_open && !gallery_sync_future.valid() &&
               !gallery_inventory_cap.empty()&&active_session &&
               firestorm_region_lease.matches(current_firestorm_epoch)&&
               (std::chrono::steady_clock::now()-last_gallery_sync >=
                    std::chrono::seconds{30} ||
                (gallery_force_refresh &&
                 std::chrono::steady_clock::now()-last_gallery_sync >=
                    std::chrono::seconds{2})))
            {
                last_gallery_sync=std::chrono::steady_clock::now();
                gallery_sync_issued_epoch=current_firestorm_epoch;
                gallery_sync_dirty_revision=inventory_dirty_revision;
                gallery_force_refresh=false;
                auto outfits=protocol::outfits_from_login_skeleton(
                    active_session->inventory_skeleton);
                if(outfits.size()>16U)outfits.resize(16U);
                gallery_sync_future=std::async(std::launch::async,
                    [transport,cap=gallery_inventory_cap,
                     agent=active_session->agent_id,cof=
                     protocol::current_outfit_folder_id(
                         active_session->inventory_skeleton).value_or(""),
                     outfits=std::move(outfits),
                     epoch=current_firestorm_epoch,
                     &firestorm_region_lease]()
                    {
                        return protocol::fetch_outfit_descendents(
                            transport,cap,agent,outfits,epoch,
                            firestorm_region_lease,cof);
                    });
            }
            // FetchInventory2 resolves selected outfit's actual item
            // records on demand. All results remain current-epoch and
            // avatar-owner bound. Never mutate COF here.
            if(gallery_item_fetch_future.valid() &&
               gallery_item_fetch_future.wait_for(
                   std::chrono::milliseconds{0})==std::future_status::ready)
            {
                try
                {
                    auto fetched=gallery_item_fetch_future.get();
                    if(firestorm_region_lease.matches(
                        gallery_item_fetch_epoch)&&fetched.valid)
                    {
                        std::scoped_lock lock{caps_mutex};
                        caps_state.resolved_outfit_items=std::move(fetched.items);
                        caps_state.resolved_outfit_folder=
                            gallery_item_fetch_folder;
                        gallery_item_fetch_completed_folder=
                            gallery_item_fetch_folder;
                    }
                }
                catch(const std::exception&)
                {
                    core::warning("Firestorm FetchInventory2 on-demand failed");
                }
            }
            if(outfit_gallery_open&&active_session&&
               !gallery_inventory_items_cap.empty()&&
               !gallery_item_fetch_future.valid()&&
               firestorm_region_lease.matches(current_firestorm_epoch))
            {
                const auto outfits=protocol::outfits_from_login_skeleton(
                    active_session->inventory_skeleton);
                if(outfit_selection<outfits.size())
                {
                    const auto& folder=outfits[outfit_selection].id;
                    if(folder!=gallery_item_fetch_completed_folder &&
                       folder!=gallery_item_fetch_folder)
                    {
                        const auto entries=caps_state.outfit_folder_items.find(folder);
                        if(entries!=caps_state.outfit_folder_items.end() &&
                           !entries->second.empty())
                        {
                            std::vector<std::string> ids;
                            for(const auto& item:entries->second)
                            {
                                if(ids.size()>=32U)break;
                                ids.push_back(item.item_id);
                            }
                            gallery_item_fetch_folder=folder;
                            gallery_item_fetch_epoch=current_firestorm_epoch;
                            gallery_item_fetch_future=std::async(
                                std::launch::async,
                                [transport,cap=gallery_inventory_items_cap,
                                 owner=active_session->agent_id,
                                 ids=std::move(ids),
                                 epoch=current_firestorm_epoch,
                                 &firestorm_region_lease]()
                                {
                                    return protocol::fetch_inventory2(
                                        transport,cap,owner,ids,epoch,
                                        firestorm_region_lease);
                                });
                        }
                    }
                }
            }
            // F10 expands one arbitrary inventoried folder on demand.
            // It is read-only and only the current simulator CAPS owner
            // can contribute categories. No recursive unbounded fetches.
            if(inventory_folder_future.valid() &&
               inventory_folder_future.wait_for(
                   std::chrono::milliseconds{0})==std::future_status::ready)
            {
                try
                {
                    auto reply=inventory_folder_future.get();
                    if(firestorm_region_lease.matches(reply.epoch) &&
                       reply.invalidation_revision==inventory_dirty_revision)
                    {
                        // Cache failed/unsupported capability attempts for
                        // this epoch too: avoid tight HTTP retry loops.
                        inventory_fetched_folders.insert(reply.folder_id);
                    }
                    if(firestorm_region_lease.matches(reply.epoch)&&reply.valid &&
                       reply.invalidation_revision==inventory_dirty_revision)
                    {
                        (void)protocol::merge_inventory_folder_read(
                            inventory_tree,inventory_folder_contents,reply);
                        inventory_fetched_folders.insert(reply.folder_id);
                    }
                }
                catch(const std::exception&)
                {
                    core::warning("Firestorm Inventory folder read failed");
                }
            }
            if(inventory_tree_open &&active_session &&
               !gallery_inventory_cap.empty() &&
               !inventory_parent.empty() &&
               (inventory_fetched_folders.size()<256U ||
                inventory_fetched_folders.contains(inventory_parent)) &&
               !inventory_folder_future.valid() &&
               firestorm_region_lease.matches(current_firestorm_epoch) &&
               (inventory_refresh_requested ||
                !inventory_last_requested.contains(inventory_parent) ||
                std::chrono::steady_clock::now()-
                  inventory_last_requested.at(inventory_parent)>=
                    std::chrono::seconds{30}))
            {
                inventory_last_requested[inventory_parent]=
                    std::chrono::steady_clock::now();
                inventory_refresh_requested=false;
                inventory_folder_future=std::async(std::launch::async,
                    [transport,cap=gallery_inventory_cap,
                     owner=active_session->agent_id,
                     parent=inventory_parent,
                     issued=current_firestorm_epoch,
                     revision=inventory_dirty_revision,
                     &firestorm_region_lease]()
                    {
                        auto reply=protocol::fetch_inventory_folder_children(
                            transport,cap,owner,parent,issued,
                            firestorm_region_lease);
                        reply.invalidation_revision=revision;
                        return reply;
                    });
            }
            // F10 individual item details use the same authenticated
            // FetchInventory2 capability as F9. Never trust a reply from
            // an earlier region, folder, or selection.
            if(inventory_item_detail_future.valid() &&
               inventory_item_detail_future.wait_for(
                   std::chrono::milliseconds{0})==std::future_status::ready)
            {
                try
                {
                    auto reply=inventory_item_detail_future.get();
                    const auto folder=inventory_folder_contents.find(
                        inventory_parent);
                    const bool selected_is_current=
                        folder!=inventory_folder_contents.end() &&
                        inventory_item_selection<folder->second.items.size() &&
                        folder->second.items[inventory_item_selection].item_id==
                            inventory_item_detail_id;
                    if(firestorm_region_lease.matches(
                           inventory_item_detail_epoch) &&
                       inventory_item_detail_folder==inventory_parent &&
                       selected_is_current)
                    {
                        inventory_item_detail.reset();
                        for(const auto& item:reply.items)
                        {
                            if(reply.valid&&
                               item.item_id==inventory_item_detail_id)
                            {
                                inventory_item_detail=item;
                                break;
                            }
                        }
                        inventory_item_detail_status=inventory_item_detail
                            ?"FetchInventory2 vom Simulator bestaetigt"
                            :"Details vom Simulator nicht verfuegbar";
                    }
                }
                catch(const std::exception&)
                {
                    inventory_item_detail_status="Detailabruf fehlgeschlagen";
                }
            }
            if(inventory_item_detail_requested &&inventory_tree_open &&
               active_session&&!inventory_item_detail_future.valid())
            {
                inventory_item_detail_requested=false;
                auto found=inventory_folder_contents.find(inventory_parent);
                if(found!=inventory_folder_contents.end()&&
                   inventory_item_selection<found->second.items.size() &&
                   !gallery_inventory_items_cap.empty()&&
                   firestorm_region_lease.matches(current_firestorm_epoch))
                {
                    const auto id=found->second.items[
                        inventory_item_selection].item_id;
                    inventory_item_detail_id=id;
                    inventory_item_detail_folder=inventory_parent;
                    inventory_item_detail_epoch=current_firestorm_epoch;
                    inventory_item_detail.reset();
                    inventory_item_detail_status="Details werden geladen";
                    inventory_item_detail_future=std::async(
                        std::launch::async,
                        [transport,cap=gallery_inventory_items_cap,
                         owner=active_session->agent_id,id,
                         folder=inventory_parent,
                         epoch=current_firestorm_epoch,
                         &firestorm_region_lease]()
                        {
                            return protocol::fetch_inventory2(
                                transport,cap,owner,{id},epoch,
                                firestorm_region_lease,folder);
                        });
                }
            }
            // Completed optional outfit thumbnail decodes become UI GPU
            // textures exclusively on this OpenGL thread. Never upload a
            // stale region response or mismatch a replaced folder asset.
            for(auto it=gallery_image_futures.begin();
                it!=gallery_image_futures.end();)
            {
                if(it->wait_for(std::chrono::milliseconds{0})!=
                   std::future_status::ready){++it;continue;}
                try
                {
                    auto image=it->get();
                    if(firestorm_region_lease.matches(image.epoch) &&
                       image.image.valid()&&image.image.width<=1024U &&
                       image.image.height<=1024U &&
                       caps_state.outfit_thumbnail_assets.contains(image.folder_id) &&
                       caps_state.outfit_thumbnail_assets.at(image.folder_id)==image.asset_id)
                    {
                        const auto handle=renderer.create_rgba_texture(
                            static_cast<int>(image.image.width),
                            static_cast<int>(image.image.height),
                            image.image.pixels.data());
                        if(handle!=0U)
                        {
                            if(const auto old=gallery_gpu_textures.find(
                                image.folder_id);old!=gallery_gpu_textures.end())
                                renderer.destroy_texture(old->second.second);
                            gallery_gpu_textures.insert_or_assign(image.folder_id,
                                std::make_pair(std::move(image.asset_id),handle));
                        }
                    }
                }
                catch(const std::exception&)
                {
                    core::warning("Outfit thumbnail optional decode failed");
                }
                it=gallery_image_futures.erase(it);
            }
            if(!gallery_texture_cap.empty() &&
               firestorm_region_lease.matches(current_firestorm_epoch) &&
               gallery_image_requested.size()<8U &&
               gallery_image_futures.size()<3U)
            {
                for(const auto& [folder,asset]:
                    caps_state.outfit_thumbnail_assets)
                {
                    if(gallery_image_futures.size()>=3U||
                       gallery_image_requested.size()>=8U)break;
                    if(gallery_image_requested.contains(folder))continue;
                    gallery_image_requested.emplace(folder,asset);
                    gallery_image_futures.emplace_back(std::async(
                        std::launch::async,
                        [transport,cap=gallery_texture_cap,folder,asset,
                         issued=current_firestorm_epoch,
                         &firestorm_region_lease]()
                        {
                            GalleryTextureReply result;
                            result.epoch=issued;
                            result.folder_id=folder;
                            result.asset_id=asset;
                            if(firestorm_region_lease.matches(issued))
                            {
                                assets::TextureAssetLoader loader{transport};
                                auto loaded=loader.fetch_rgba(cap,asset);
                                if(firestorm_region_lease.matches(issued) &&
                                   loaded.success)
                                    result.image=std::move(loaded.image);
                            }
                            return result;
                        }));
                }
            }
            if (material_scope.update(
                    current_region_handle,
                    current_region_generation, current_region_seed))
            {
                scoped_render_materials_uri.clear();
                scoped_get_texture_uri.clear();
                scoped_get_mesh_uri.clear();
                scoped_get_mesh_alt_uri.clear();
                scene_mesh_samples.clear();
                mesh_residency = {};
                last_mesh_residency_plan = {};
                world_renderer->clear_mesh_assets();
                mesh_asset_loaded_ids.clear();
                mesh_asset_retries.enter(material_scope.epoch());
                mesh_desired_lods.clear();
                mesh_gpu_lods.clear();
                mesh_lod_dwell.reset();
                asset_gpu_upload_gate.reset();
                asset_gpu_upload_telemetry.reset();
                mesh_priority_feedback.reset();
                texture_priority_feedback.reset();
                mesh_gpu_asset_bytes.clear();
                mesh_gpu_estimated_bytes = 0U;
                mesh_gpu_budget_deferred_ids.clear();
                mesh_gpu_budget_recovery.reset();
                mesh_ram_pressure.reset();
                mesh_missing_unique.reset();
                mesh_request_pacer.reset();
                mesh_request_fairness.reset();
                region_material_discovery_started = false;
                render_materials_started = false;
                render_materials_get_complete = false;
                legacy_materials.clear();
                material_ids_pending.clear();
                material_request_times.clear();
                material_request_attempts.clear();
                render_materials_in_flight.clear();
                scene_texture_ids.clear();
                scene_texture_samples.clear();
                texture_residency = {};
                last_texture_residency_plan = {};
                last_world_asset_health_log = {};
                world_renderer->clear_object_textures();
                object_texture_loaded_ids.clear();
                object_texture_retries.enter(material_scope.epoch());
                texture_desired_mips.clear();
                texture_gpu_mips.clear();
                texture_lod_dwell.reset();
                texture_gpu_bytes.clear();
                texture_gpu_used_bytes = 0U;
                texture_budget_deferred_ids.clear();
                texture_gpu_staging_recovery.reset();
                texture_ram_pressure.reset();
                texture_missing_unique.reset();
                texture_request_pacer.reset();
                texture_request_fairness.reset();
                texture_prefetch_pacer.reset();
                texture_prefetch_scope.enter(material_scope.epoch());
                texture_prefetch_live_epoch->store(
                    material_scope.epoch(),std::memory_order_release);
                terrain_texture_request_generation =
                    std::numeric_limits<std::uint32_t>::max();
                terrain_texture_request_material_revision =
                    std::numeric_limits<std::uint32_t>::max();
                rendered_object_generation =
                    std::numeric_limits<std::uint32_t>::max();
                rendered_object_revision =
                    std::numeric_limits<std::uint32_t>::max();
                core::info("RenderMaterials region cache invalidated; epoch=" +
                    std::to_string(material_scope.epoch()));
            }

            if (region_material_discovery_future.valid() &&
                region_material_discovery_future.wait_for(std::chrono::seconds{0}) ==
                    std::future_status::ready)
            {
                try
                {
                    auto discovery = region_material_discovery_future.get();
                    if (material_scope.matches(region_material_discovery_epoch))
                    {
                        if (discovery.success)
                        {
                            if (const auto it =
                                    discovery.capabilities.find("RenderMaterials");
                                it != discovery.capabilities.end())
                                scoped_render_materials_uri = it->second;
                            if (const auto it =
                                    discovery.capabilities.find("GetTexture");
                                it != discovery.capabilities.end())
                                scoped_get_texture_uri = it->second;
                            if (const auto it =
                                    discovery.capabilities.find("GetMesh2");
                                it != discovery.capabilities.end())
                                scoped_get_mesh_uri = it->second;
                            if (const auto it =
                                    discovery.capabilities.find("GetMesh");
                                it != discovery.capabilities.end())
                            {
                                if (scoped_get_mesh_uri.empty())
                                    scoped_get_mesh_uri = it->second;
                                else if (it->second != scoped_get_mesh_uri)
                                    scoped_get_mesh_alt_uri = it->second;
                            }
                            if (scoped_get_mesh_uri.empty())
                                core::warning(
                                    "Region does not advertise GetMesh; mesh objects will use placeholders");
                            core::info("Region-scoped material capabilities ready");
                        }
                        else
                        {
                            core::warning("Region-scoped CAPS discovery failed: " +
                                discovery.message);
                        }
                    }
                }
                catch (const std::exception& ex)
                {
                    if (material_scope.matches(region_material_discovery_epoch))
                        core::warning(std::string{"Region-scoped CAPS error: "} +
                            ex.what());
                }
            }
            if (!region_material_discovery_started &&
                !region_material_discovery_future.valid() &&
                !current_region_seed.empty())
            {
                region_material_discovery_started = true;
                region_material_discovery_epoch = material_scope.epoch();
                region_material_discovery_future = std::async(
                    std::launch::async,
                    [transport, seed = current_region_seed]()
                    {
                        return protocol::CapabilityService{transport}.discover(
                            seed, {"RenderMaterials", "GetTexture", "GetMesh", "GetMesh2"});
                    });
            }

            // Material CAPS GET runs outside the render thread. Keep the
            // LLSD snapshot per region; scene rebuild applies resolved alpha.
            if (!render_materials_started &&
                !render_materials_future.valid() &&
                !scoped_render_materials_uri.empty())
            {
                const std::string uri = scoped_render_materials_uri;
                {
                    render_materials_started = true;
                    render_materials_get_epoch = material_scope.epoch();
                    render_materials_future = std::async(
                        std::launch::async,
                        [transport, uri]()
                        {
                            return protocol::RenderMaterialsService{transport}
                                .fetch_all(uri);
                        });
                }
            }
            if (render_materials_future.valid() &&
                render_materials_future.wait_for(std::chrono::seconds{0}) ==
                    std::future_status::ready)
            {
                const bool current_reply =
                    material_scope.matches(render_materials_get_epoch);
                if (current_reply)
                    render_materials_get_complete = true;
                try
                {
                    auto response = render_materials_future.get();
                    if (!current_reply)
                    {
                        core::info("Discarded stale RenderMaterials GET reply");
                    }
                    else if (response.success)
                    {
                        legacy_materials = std::move(response.materials);
                        const auto snapshot_time =
                            std::chrono::steady_clock::now();
                        for (const auto& [id, material] : legacy_materials)
                        {
                            (void)material;
                            material_request_times[id] = snapshot_time;
                        }
                        rendered_object_generation =
                            std::numeric_limits<std::uint32_t>::max();
                        rendered_object_revision =
                            std::numeric_limits<std::uint32_t>::max();
                        core::info("OpenSim RenderMaterials resolved: " +
                            std::to_string(legacy_materials.size()));
                    }
                    else
                    {
                        core::warning("OpenSim RenderMaterials fallback: " +
                            response.message);
                    }
                }
                catch (const std::exception& ex)
                {
                    if (current_reply)
                        core::warning(std::string{"RenderMaterials worker failed: "} +
                            ex.what());
                }
            }

            // Completed POST batches merge only validated material definitions.
            // Missing IDs are retried at most three times with a cooldown.
            if (render_materials_post_future.valid() &&
                render_materials_post_future.wait_for(std::chrono::seconds{0}) ==
                    std::future_status::ready)
            {
                bool updated = false;
                const bool current_reply =
                    material_scope.matches(render_materials_post_epoch);
                try
                {
                    auto result = render_materials_post_future.get();
                    if (!current_reply)
                    {
                        core::info("Discarded stale RenderMaterials POST reply");
                    }
                    else if (result.success)
                    {
                        for (auto& [id, material] : result.materials)
                        {
                            if (const auto found = legacy_materials.find(id);
                                found == legacy_materials.end() ||
                                found->second != material)
                            {
                                legacy_materials[id] = std::move(material);
                                updated = true;
                            }
                            material_request_attempts.erase(id);
                        }
                    }
                    else
                    {
                        core::warning("OpenSim RenderMaterials POST fallback: " +
                            result.message);
                    }
                }
                catch (const std::exception& ex)
                {
                    if (current_reply)
                        core::warning(
                            std::string{"RenderMaterials POST worker failed: "} +
                            ex.what());
                }
                if (current_reply)
                    for (const auto& id : render_materials_in_flight)
                    {
                        if (!legacy_materials.contains(id) &&
                            material_request_attempts[id] < 3U)
                            material_ids_pending.insert(id);
                    }
                render_materials_in_flight.clear();
                if (updated)
                {
                    rendered_object_generation =
                        std::numeric_limits<std::uint32_t>::max();
                    rendered_object_revision =
                        std::numeric_limits<std::uint32_t>::max();
                    core::info("OpenSim RenderMaterials updated from POST");
                }
            }

            {
                std::scoped_lock lock{world_mutex};
                render_agent = world_model.agent();
                // Legacy point lights are regular prim ExtraParams.
                // Choose a small bounded set of nearby lights for the
                // legacy OpenGL shader; region lights never leave CPU
                // WorldModel and do not require asset requests.
                struct ScoredPointLight
                {
                    float distance_squared = 0.0f;
                    render::RenderPointLight value;
                };
                std::vector<ScoredPointLight> light_candidates;
                for (const auto& [local_id, object] : world_model.objects())
                {
                    (void)local_id;
                    if (!object.point_light.has_value() ||
                        object.point_light->intensity <= 0.0f) continue;
                    protocol::Vec3 world_position = object.position;
                    if (object.parent_id != 0U)
                    {
                        if (const auto it =
                                world_model.objects().find(object.parent_id);
                            it != world_model.objects().end())
                        {
                            world_position.x += it->second.position.x;
                            world_position.y += it->second.position.y;
                            world_position.z += it->second.position.z;
                        }
                    }
                    const float dx = world_position.x -
                        render_agent.position.x;
                    const float dy = world_position.y -
                        render_agent.position.y;
                    const float dz = world_position.z -
                        render_agent.position.z;
                    const float dist = dx * dx + dy * dy + dz * dz;
                    if (!std::isfinite(dist)) continue;
                    const auto& light = *object.point_light;
                    light_candidates.push_back({
                        dist,
                        {{world_position.x, world_position.y, world_position.z},
                         {light.red, light.green, light.blue},
                         light.intensity, light.radius, light.falloff,
                         light.cutoff}});
                }
                constexpr std::size_t max_frame_point_lights = 8U;
                if (light_candidates.size() > max_frame_point_lights)
                {
                    std::partial_sort(
                        light_candidates.begin(),
                        light_candidates.begin() +
                            static_cast<std::ptrdiff_t>(max_frame_point_lights),
                        light_candidates.end(),
                        [](const auto& a, const auto& b)
                        {
                            return a.distance_squared < b.distance_squared;
                        });
                }
                for (std::size_t i = 0U; i < std::min(
                         light_candidates.size(), max_frame_point_lights); ++i)
                    scene_point_lights.push_back(light_candidates[i].value);

                struct ScoredParticleEmitter
                {
                    float distance_squared = 0.0f;
                    render::RenderParticleEmitter value;
                };
                std::vector<ScoredParticleEmitter> emitter_candidates;
                for (const auto& [local_id, object] : world_model.objects())
                {
                    if (!object.particle_system.has_value()) continue;
                    protocol::Vec3 world_position = object.position;
                    if (object.parent_id != 0U)
                    {
                        if (const auto parent =
                                world_model.objects().find(object.parent_id);
                            parent != world_model.objects().end())
                        {
                            world_position.x += parent->second.position.x;
                            world_position.y += parent->second.position.y;
                            world_position.z += parent->second.position.z;
                        }
                    }
                    const float dx = world_position.x -
                        render_agent.position.x;
                    const float dy = world_position.y -
                        render_agent.position.y;
                    const float dz = world_position.z -
                        render_agent.position.z;
                    const float distance_squared =
                        dx * dx + dy * dy + dz * dz;
                    if (!std::isfinite(distance_squared) ||
                        distance_squared > 192.0f * 192.0f) continue;
                    const auto& ps = *object.particle_system;
                    protocol::Vec3 target_position{};
                    bool target_available = false;
                    if (!ps.target_id.empty() &&
                        ps.target_id !=
                            "00000000-0000-0000-0000-000000000000")
                    {
                        for (const auto& [other_id, other] :
                             world_model.objects())
                        {
                            (void)other_id;
                            if (other.id == ps.target_id)
                            {
                                target_position = other.position;
                                target_available = true;
                                if (other.parent_id != 0U)
                                {
                                    if (const auto parent =
                                            world_model.objects().find(
                                                other.parent_id);
                                        parent != world_model.objects().end())
                                    {
                                        target_position.x +=
                                            parent->second.position.x;
                                        target_position.y +=
                                            parent->second.position.y;
                                        target_position.z +=
                                            parent->second.position.z;
                                    }
                                }
                                break;
                            }
                        }
                        if (!target_available)
                        {
                            if (const auto avatar =
                                    world_model.avatars().find(ps.target_id);
                                avatar != world_model.avatars().end())
                            {
                                target_position = avatar->second.position;
                                target_available = true;
                            }
                            else if (ps.target_id == render_agent.agent_id)
                            {
                                target_position = render_agent.position;
                                target_available = true;
                            }
                        }
                    }
                    emitter_candidates.push_back({
                        distance_squared,
                        {local_id, ps.crc,
                         {world_position.x,world_position.y,world_position.z},
                         {object.rotation.x, object.rotation.y,
                          object.rotation.z, object.rotation.w},
                         {ps.acceleration.x,ps.acceleration.y,ps.acceleration.z},
                         ps.texture_id,
                         ps.start_color,ps.end_color,
                         ps.burst_rate,ps.burst_radius,
                         ps.speed_min,ps.speed_max,
                         ps.particle_max_age,ps.source_max_age,
                         ps.start_glow,ps.end_glow,
                         ps.start_scale,ps.end_scale,
                         ps.burst_count,ps.pattern,ps.particle_flags,
                         ps.flags, target_available,
                         {target_position.x, target_position.y,
                          target_position.z},
                         ps.blend_source,ps.blend_destination}});
                }
                constexpr std::size_t emitter_budget = 32U;
                if (emitter_candidates.size() > emitter_budget)
                    std::partial_sort(emitter_candidates.begin(),
                        emitter_candidates.begin() +
                            static_cast<std::ptrdiff_t>(emitter_budget),
                        emitter_candidates.end(),
                        [](const auto& a, const auto& b)
                        {
                            return a.distance_squared < b.distance_squared;
                        });
                for (std::size_t i = 0U; i < std::min(
                         emitter_candidates.size(), emitter_budget); ++i)
                    scene_particle_emitters.push_back(
                        emitter_candidates[i].value);

                if (render_agent.present)
                {
                    // Server-reported appearance hover shifts the visual
                    // proxy only; do not alter simulator-authoritative agent
                    // coordinates or the collision/physics position.
                    float self_hover=0.0f;
                    if (const auto& appearance=world_model.self_appearance();
                        appearance && !appearance->hover_heights.empty())
                        self_hover=std::clamp(
                            appearance->hover_heights.front().z,-2.0f,2.0f);
                    std::array<std::string,3U> baked_ids{};
                    if(const auto& self=world_model.self_appearance();self)
                        baked_ids=protocol::extract_avatar_bakes(
                            self->texture_entry).visible_body();
                    avatar_proxy_update.push_back({
                        {render_agent.position.x,
                         render_agent.position.y,
                         render_agent.position.z+self_hover},
                        {render_agent.rotation.x,
                         render_agent.rotation.y,
                         render_agent.rotation.z,
                         render_agent.rotation.w},
                        true,baked_ids,self_avatar_local_id});
                    avatar_proxy_update.back().moving=
                        (last_reported_control_flags &
                         (protocol::control_at_pos | protocol::control_at_neg |
                          protocol::control_left_pos |
                          protocol::control_left_neg))!=0U;
                    const auto local_fallback=
                        avatar_proxy_update.back().moving
                            ? protocol::AvatarMotion::walk
                            : protocol::AvatarMotion::idle;
                    if(const auto& active=world_model.self_animations();active)
                    {
                        const auto known=protocol::detect_avatar_motion(
                            active->animations);
                        avatar_proxy_update.back().animation_from_server=
                            known.has_value();
                        avatar_proxy_update.back().motion=
                            known.value_or(local_fallback);
                    }
                    else avatar_proxy_update.back().motion=local_fallback;
                    if(const auto& appearance=world_model.self_appearance();
                       appearance)
                        for(const auto& attachment:appearance->attachments)
                            avatar_proxy_update.back().attachments.emplace(
                                attachment.object_id,attachment.attachment_point);
                }
                for (const auto& [id, avatar] : world_model.avatars())
                {
                    if (id == render_agent.agent_id ||
                        !avatar.position_known ||
                        avatar_proxy_update.size() >= 64U) continue;
                    const float hover=avatar.hover_heights.empty()?0.0f:
                        std::clamp(avatar.hover_heights.front().z,-2.0f,2.0f);
                    avatar_proxy_update.push_back({
                        {avatar.position.x,
                         avatar.position.y,
                         avatar.position.z+hover},
                        {avatar.rotation.x,
                         avatar.rotation.y,
                         avatar.rotation.z,
                         avatar.rotation.w},
                        false,
                        protocol::extract_avatar_bakes(
                            avatar.texture_entry).visible_body(),
                        avatar.local_id});
                    avatar_proxy_update.back().moving=
                        avatar.last_translation_at!=
                            std::chrono::steady_clock::time_point{} &&
                        std::chrono::steady_clock::now()-
                            avatar.last_translation_at<
                            std::chrono::milliseconds{650};
                    const auto remote_fallback=
                        avatar_proxy_update.back().moving
                            ? protocol::AvatarMotion::walk
                            : protocol::AvatarMotion::idle;
                    if(avatar.animations)
                    {
                        const auto known=protocol::detect_avatar_motion(
                            avatar.animations->animations);
                        avatar_proxy_update.back().animation_from_server=
                            known.has_value();
                        avatar_proxy_update.back().motion=
                            known.value_or(remote_fallback);
                    }
                    else avatar_proxy_update.back().motion=remote_fallback;
                    for(const auto& attachment:avatar.attachments)
                        avatar_proxy_update.back().attachments.emplace(
                            attachment.object_id,attachment.attachment_point);
                }

                const world::RegionState& region =
                    world_model.region();
                const world::TerrainState& terrain =
                    world_model.terrain();
                const world::EnvironmentState& environment =
                    world_model.environment();

                terrain_materials_available =
                    terrain.materials.available;
                texture_generation_current =
                    region.generation;
                texture_material_revision_current =
                    terrain.materials.revision;
                terrain_detail_ids =
                    terrain.materials.detail_ids;

                if (region.generation != rendered_terrain_generation ||
                    terrain.revision != rendered_terrain_revision ||
                    terrain.materials.revision != rendered_material_revision)
                {
                    rendered_terrain_generation = region.generation;
                    rendered_terrain_revision = terrain.revision;
                    rendered_material_revision = terrain.materials.revision;

                    terrain_update.generation = region.generation;
                    terrain_update.revision = terrain.revision;
                    terrain_update.width = 256U;
                    terrain_update.height = 256U;
                    terrain_update.patch_size = 16U;
                    terrain_update.water_height = region.water_height;
                    terrain_update.material_revision =
                        terrain.materials.revision;
                    terrain_update.terrain_start_height =
                        terrain.materials.start_heights;
                    terrain_update.terrain_height_range =
                        terrain.materials.height_ranges;
                    terrain_update.heights.assign(
                        terrain.heights.begin(),
                        terrain.heights.end());
                    terrain_update.valid_patches.reserve(
                        terrain.valid_patches.size());

                    for (const bool valid : terrain.valid_patches)
                    {
                        terrain_update.valid_patches.push_back(
                            valid ? 1U : 0U);
                    }

                    terrain_changed = true;
                }

                if (environment.revision != rendered_environment_revision)
                {
                    rendered_environment_revision = environment.revision;
                    environment_update.revision = environment.revision;
                    environment_update.available = environment.available;
                    environment_update.ambient = {
                        environment.ambient.x,
                        environment.ambient.y,
                        environment.ambient.z};
                    environment_update.blue_density = {
                        environment.blue_density.x,
                        environment.blue_density.y,
                        environment.blue_density.z};
                    environment_update.blue_horizon = {
                        environment.blue_horizon.x,
                        environment.blue_horizon.y,
                        environment.blue_horizon.z};
                    environment_update.sunlight_color = {
                        environment.sunlight_color.x,
                        environment.sunlight_color.y,
                        environment.sunlight_color.z};
                    environment_update.water_fog_color = {
                        environment.water_fog_color.x,
                        environment.water_fog_color.y,
                        environment.water_fog_color.z};
                    environment_update.haze_density = environment.haze_density;
                    environment_update.haze_horizon = environment.haze_horizon;
                    environment_update.gamma = environment.gamma;
                    environment_update.sun_angle = environment.sun_angle;
                    environment_update.east_angle = environment.east_angle;
                    environment_changed = true;
                }

                const std::uint32_t object_revision =
                    world_model.object_revision();

                if (region.generation != rendered_object_generation ||
                    object_revision != rendered_object_revision)
                {
                    rendered_object_generation = region.generation;
                    rendered_object_revision = object_revision;

                    scene_object_update.reserve(
                        world_model.objects().size());
                    scene_mesh_samples.clear();

                    for (const auto& [local_id, object] :
                         world_model.objects())
                    {
                        (void)local_id;

                        if (object.pcode != protocol::pcode_prim)
                        {
                            continue;
                        }

                        render::RenderSceneObject render_object;
                        render_object.local_id = object.local_id;
                        render_object.object_id = object.id;
                        render_object.parent_id = object.parent_id;
                        render_object.kind =
                            render_primitive_kind(object.shape);
                        if (object.is_mesh())
                        {
                            render_object.mesh_id = object.sculpt_asset_id;
                            // Child prim positions are commonly relative to
                            // their linkset root; approximate their region
                            // position with their parent's transform.
                            auto position = object.position;
                            if (object.parent_id != 0U)
                            {
                                if (const auto parent =
                                        world_model.objects().find(object.parent_id);
                                    parent != world_model.objects().end())
                                {
                                    position.x += parent->second.position.x;
                                    position.y += parent->second.position.y;
                                    position.z += parent->second.position.z;
                                }
                            }
                            scene_mesh_samples.push_back({
                                object.sculpt_asset_id,
                                position.x, position.y, position.z,
                                std::max({object.scale.x, object.scale.y,
                                          object.scale.z, 0.5f})});
                        }
                        render_object.position = {
                            object.position.x,
                            object.position.y,
                            object.position.z};
                        render_object.rotation = {
                            object.rotation.x,
                            object.rotation.y,
                            object.rotation.z,
                            object.rotation.w};
                        render_object.scale = {
                            object.scale.x,
                            object.scale.y,
                            object.scale.z};

                        constexpr std::uint8_t path_straight = 0x10U;
                        constexpr std::uint8_t path_curve_1 = 0x20U;
                        constexpr std::uint8_t path_curve_2 = 0x30U;
                        constexpr std::uint8_t path_flexible = 0x80U;
                        constexpr float pi = 3.14159265358979323846f;

                        render_object.linear_path_deform =
                            object.shape.path_curve == path_straight ||
                            object.shape.path_curve == path_flexible;
                        render_object.curved_path =
                            object.shape.path_curve == path_curve_1 ||
                            object.shape.path_curve == path_curve_2;

                        render_object.path_begin =
                            std::clamp(
                                static_cast<float>(object.shape.path_begin) *
                                    0.00002f,
                                0.0f,
                                1.0f);
                        render_object.path_end =
                            std::clamp(
                                1.0f -
                                    static_cast<float>(object.shape.path_end) *
                                        0.00002f,
                                render_object.path_begin + 0.0001f,
                                1.0f);

                        render_object.profile_begin =
                            std::clamp(
                                static_cast<float>(object.shape.profile_begin) *
                                    0.00002f,
                                0.0f,
                                1.0f);
                        render_object.profile_end =
                            std::clamp(
                                1.0f -
                                    static_cast<float>(object.shape.profile_end) *
                                        0.00002f,
                                render_object.profile_begin + 0.0001f,
                                1.0f);
                        render_object.profile_hollow =
                            std::clamp(
                                static_cast<float>(object.shape.profile_hollow) *
                                    0.00002f,
                                0.0f,
                                0.95f);

                        render_object.path_hole_x =
                            std::clamp(
                                static_cast<float>(
                                    200 - static_cast<int>(
                                        object.shape.path_scale_x)) *
                                    0.01f,
                                0.01f,
                                2.0f);
                        render_object.path_hole_y =
                            std::clamp(
                                static_cast<float>(
                                    200 - static_cast<int>(
                                        object.shape.path_scale_y)) *
                                    0.01f,
                                0.01f,
                                2.0f);
                        render_object.radius_offset =
                            static_cast<float>(
                                object.shape.path_radius_offset) *
                            0.01f;
                        render_object.revolutions =
                            1.0f +
                            static_cast<float>(
                                object.shape.path_revolutions) *
                                0.015f;
                        render_object.skew =
                            static_cast<float>(
                                object.shape.path_skew) *
                            0.01f;

                        switch (object.shape.hollow_shape())
                        {
                            case 0x10U:
                                render_object.hollow_profile =
                                    render::RenderProfileKind::circle;
                                break;
                            case 0x20U:
                                render_object.hollow_profile =
                                    render::RenderProfileKind::square;
                                break;
                            case 0x30U:
                                render_object.hollow_profile =
                                    render::RenderProfileKind::triangle;
                                break;
                            default:
                                render_object.hollow_profile =
                                    render::RenderProfileKind::same;
                                break;
                        }

                        render_object.taper_x =
                            static_cast<float>(object.shape.path_taper_x) * 0.01f;
                        render_object.taper_y =
                            static_cast<float>(object.shape.path_taper_y) * 0.01f;

                        const int shear_x =
                            object.shape.path_shear_x < 128U
                                ? static_cast<int>(object.shape.path_shear_x)
                                : static_cast<int>(object.shape.path_shear_x) - 256;
                        const int shear_y =
                            object.shape.path_shear_y < 128U
                                ? static_cast<int>(object.shape.path_shear_y)
                                : static_cast<int>(object.shape.path_shear_y) - 256;

                        render_object.shear_x =
                            static_cast<float>(shear_x) * 0.01f;
                        render_object.shear_y =
                            static_cast<float>(shear_y) * 0.01f;

                        render_object.twist_begin_radians =
                            static_cast<float>(object.shape.path_twist_begin) *
                            pi * 0.01f;
                        render_object.twist_end_radians =
                            static_cast<float>(object.shape.path_twist) *
                            pi * 0.01f;

                        if (object.default_texture.has_value())
                        {
                            const auto& face =
                                *object.default_texture;
                            render_object.texture_id =
                                face.texture_id;
                            render_object.color =
                                face.color;
                            render_object.repeat_u =
                                face.repeat_u;
                            render_object.repeat_v =
                                face.repeat_v;
                            render_object.offset_u =
                                face.offset_u;
                            render_object.offset_v =
                                face.offset_v;
                            render_object.texture_rotation =
                                face.rotation;
                            render_object.fullbright =
                                face.fullbright();
                            render_object.shiny =
                                face.shiny();
                            render_object.bumpiness =
                                face.bumpiness();
                            render_object.media_present =
                                face.media_present();
                            render_object.texgen_planar =
                                face.texgen_planar();
                            render_object.glow =
                                face.glow;
                            render_object.material_id =
                                face.material_id;
                            render_object.render_material_id =
                                face.render_material_id;
                            if (const auto found =
                                    legacy_materials.find(face.material_id);
                                found != legacy_materials.end())
                            {
                                render_object.diffuse_alpha_mode =
                                    found->second.diffuse_alpha_mode;
                                render_object.alpha_mask_cutoff =
                                    found->second.alpha_mask_cutoff;
                                render_object.legacy =
                                    render_legacy_appearance(found->second);
                            }

                            if (!face.texture_id.empty() &&
                                face.texture_id !=
                                    "00000000-0000-0000-0000-000000000000")
                            {
                                scene_texture_update.push_back(
                                    face.texture_id);
                            }
                        }

                        if (object.parsed_texture_entry.has_value())
                        {
                            const auto& texture_entry =
                                *object.parsed_texture_entry;

                            for (std::size_t face_index = 0U;
                                 face_index <
                                     texture_entry.face_overrides.size();
                                 ++face_index)
                            {
                                if (!texture_entry
                                         .face_overrides[face_index]
                                         .has_value())
                                {
                                    continue;
                                }

                                const auto& face =
                                    *texture_entry
                                         .face_overrides[face_index];

                                render::RenderFaceMaterial material;
                                material.texture_id =
                                    face.texture_id;
                                material.color =
                                    face.color;
                                material.repeat_u =
                                    face.repeat_u;
                                material.repeat_v =
                                    face.repeat_v;
                                material.offset_u =
                                    face.offset_u;
                                material.offset_v =
                                    face.offset_v;
                                material.rotation =
                                    face.rotation;
                                material.fullbright =
                                    face.fullbright();
                                material.shiny =
                                    face.shiny();
                                material.bumpiness =
                                    face.bumpiness();
                                material.media_present =
                                    face.media_present();
                                material.texgen_planar =
                                    face.texgen_planar();
                                material.glow =
                                    face.glow;
                                material.material_id =
                                    face.material_id;
                                material.render_material_id =
                                    face.render_material_id;
                                if (const auto found =
                                        legacy_materials.find(face.material_id);
                                    found != legacy_materials.end())
                                {
                                    material.diffuse_alpha_mode =
                                        found->second.diffuse_alpha_mode;
                                    material.alpha_mask_cutoff =
                                        found->second.alpha_mask_cutoff;
                                    material.legacy =
                                        render_legacy_appearance(found->second);
                                }

                                render_object
                                    .face_materials[face_index] =
                                    std::move(material);

                                if (!face.texture_id.empty() &&
                                    face.texture_id !=
                                        "00000000-0000-0000-0000-000000000000")
                                {
                                    scene_texture_update.push_back(
                                        face.texture_id);
                                }
                            }
                        }

                        scene_object_update.push_back(
                            std::move(render_object));
                    }

                    scene_objects_changed = true;
                }
            }

            if (scene_objects_changed)
            {
                // Only the legacy MaterialID is relevant for this capability.
                // Modern RenderMaterialID must use a separate PBR resolver.
                for (const auto& object : scene_object_update)
                {
                    const auto enqueue_legacy_maps =
                        [&scene_texture_update](
                            const render::RenderLegacyAppearance& legacy)
                        {
                            if (!legacy.normal.texture_id.empty())
                                scene_texture_update.push_back(
                                    legacy.normal.texture_id);
                            if (!legacy.specular.texture_id.empty())
                                scene_texture_update.push_back(
                                    legacy.specular.texture_id);
                        };
                    enqueue_legacy_maps(object.legacy);
                    for (const auto& face : object.face_materials)
                        if (face.has_value())
                            enqueue_legacy_maps(face->legacy);
                    if (protocol::is_valid_legacy_material_id(object.material_id))
                        material_ids_pending.insert(object.material_id);
                    for (const auto& face : object.face_materials)
                    {
                        if (face.has_value() &&
                            protocol::is_valid_legacy_material_id(face->material_id))
                            material_ids_pending.insert(face->material_id);
                    }
                }
                scene_texture_samples.clear();
                for (const auto& object:scene_object_update)
                {
                    const auto add=[&](const std::string& id)
                    {
                        if(id.empty())return;
                        scene_texture_samples.push_back({
                            id,object.position.x,object.position.y,
                            object.position.z});
                    };
                    add(object.texture_id);
                    add(object.legacy.normal.texture_id);
                    add(object.legacy.specular.texture_id);
                    for (const auto& face:object.face_materials)
                        if(face.has_value())
                        {
                            add(face->texture_id);
                            add(face->legacy.normal.texture_id);
                            add(face->legacy.specular.texture_id);
                        }
                }
                std::sort(
                    scene_texture_update.begin(),
                    scene_texture_update.end());
                scene_texture_update.erase(
                    std::unique(
                        scene_texture_update.begin(),
                        scene_texture_update.end()),
                    scene_texture_update.end());
                scene_texture_ids =
                    std::move(scene_texture_update);

                world_renderer->set_scene_objects(
                    scene_object_update);
            }

            // Do not POST until the initial snapshot is available (or failed).
            // A cooldown prevents repeated network requests on moving prims.
            if (render_materials_get_complete &&
                !render_materials_post_future.valid() &&
                !material_ids_pending.empty())
            {
                const std::string uri = scoped_render_materials_uri;
                if (!uri.empty())
                {
                    std::vector<std::string> batch;
                    const auto request_time =
                        std::chrono::steady_clock::now();
                    for (auto it = material_ids_pending.begin();
                         it != material_ids_pending.end() &&
                         batch.size() < protocol::max_render_materials_batch_ids;)
                    {
                        const auto& id = *it;
                        const auto last = material_request_times.find(id);
                        const bool known = legacy_materials.contains(id);
                        const bool recently_requested =
                            last != material_request_times.end() &&
                            request_time - last->second < std::chrono::minutes{2};

                        if (known && recently_requested)
                        {
                            it = material_ids_pending.erase(it);
                            continue;
                        }
                        if (material_request_attempts[id] >= 3U)
                        {
                            it = material_ids_pending.erase(it);
                            continue;
                        }
                        if (recently_requested)
                        {
                            ++it;
                            continue;
                        }
                        batch.push_back(id);
                        material_request_times[id] = request_time;
                        ++material_request_attempts[id];
                        it = material_ids_pending.erase(it);
                    }
                    if (!batch.empty())
                    {
                        render_materials_in_flight = batch;
                        render_materials_post_epoch = material_scope.epoch();
                        render_materials_post_future = std::async(
                            std::launch::async,
                            [transport, uri, batch]()
                            {
                                return protocol::RenderMaterialsService{transport}
                                    .fetch_ids(uri, batch);
                            });
                    }
                }
            }

            if (environment_changed)
            {
                world_renderer->set_environment(environment_update);
            }

            if (terrain_changed)
            {
                world_renderer->set_terrain(terrain_update);
            }

            const std::string texture_capability =
                scoped_get_texture_uri;

            if (!terrain_texture_loading &&
                terrain_materials_available &&
                !texture_capability.empty() &&
                (terrain_texture_request_generation !=
                     texture_generation_current ||
                 terrain_texture_request_material_revision !=
                     texture_material_revision_current))
            {
                terrain_texture_request_generation =
                    texture_generation_current;
                terrain_texture_request_material_revision =
                    texture_material_revision_current;
                terrain_texture_loading = true;
                terrain_texture_request_epoch = material_scope.epoch();

                const auto ids = terrain_detail_ids;
                const std::string capability = texture_capability;

                terrain_texture_future = std::async(
                    std::launch::async,
                    [transport, capability, ids]()
                    {
                        TerrainTextureResults results;
                        assets::TextureAssetLoader loader{transport};

                        for (std::size_t index = 0U;
                             index < ids.size();
                             ++index)
                        {
                            if (ids[index].empty() ||
                                ids[index] ==
                                    "00000000-0000-0000-0000-000000000000")
                            {
                                continue;
                            }

                            results[index] =
                                loader.fetch_rgba(
                                    capability,
                                    ids[index]);
                        }

                        return results;
                    });
            }

            if (terrain_texture_loading &&
                terrain_texture_future.valid() &&
                terrain_texture_future.wait_for(
                    std::chrono::seconds{0}) ==
                    std::future_status::ready)
            {
                TerrainTextureResults results =
                    terrain_texture_future.get();
                terrain_texture_loading = false;

                if (material_scope.matches(terrain_texture_request_epoch) &&
                    texture_generation_current ==
                        terrain_texture_request_generation &&
                    texture_material_revision_current ==
                        terrain_texture_request_material_revision)
                {
                    render::RenderTerrainTextures textures;
                    textures.generation =
                        terrain_texture_request_generation;
                    textures.material_revision =
                        terrain_texture_request_material_revision;

                    std::size_t loaded_count = 0U;
                    for (std::size_t index = 0U;
                         index < results.size();
                         ++index)
                    {
                        if (!results[index].success ||
                            !results[index].image.valid())
                        {
                            continue;
                        }

                        textures.detail[index].width =
                            results[index].image.width;
                        textures.detail[index].height =
                            results[index].image.height;
                        textures.detail[index].pixels =
                            std::move(
                                results[index].image.pixels);
                        ++loaded_count;
                    }

                    if (loaded_count > 0U)
                    {
                        world_renderer->set_terrain_textures(
                            textures);
                        core::info(
                            "Terrain texture assets loaded; count=" +
                            std::to_string(loaded_count));
                    }
                    else
                    {
                        std::string reason;
                        for (const auto& failed : results)
                        {
                            if (!failed.success && !failed.message.empty())
                            {
                                reason = failed.message;
                                break;
                            }
                        }
                        core::warning(
                            "Terrain texture assets unavailable: " + reason +
                            "; color fallback remains active");
                    }
                }
            }

            if (object_texture_loading &&
                object_texture_future.valid() &&
                object_texture_future.wait_for(
                    std::chrono::seconds{0}) ==
                    std::future_status::ready &&
                asset_gpu_upload_gate.ready(std::chrono::steady_clock::now()))
            {
                assets::TextureAssetResult result =
                    object_texture_future.get();
                object_texture_loading = false;
                switch(result.disk_lookup)
                {
                case assets::AssetCacheLookup::miss:
                    ++texture_disk_misses;break;
                case assets::AssetCacheLookup::corrupt:
                    ++texture_disk_corrupt;break;
                case assets::AssetCacheLookup::io_error:
                    ++texture_disk_io_errors;break;
                default:break;
                }
                if (result.network_requested) ++texture_network_attempts;
                if (result.cancelled_before_download)
                    ++texture_aborted_before_http;
                if (texture_request_started.time_since_epoch().count()!=0)
                {
                    ++texture_worker_completions;
                    texture_worker_elapsed_ms += static_cast<std::uint64_t>(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now()-
                            texture_request_started).count());
                }

                if (!material_scope.matches(object_texture_request_epoch))
                {
                    if (result.network_requested)
                        ++texture_discarded_after_http;
                    ++texture_stale_results;
                    core::info("Discarded stale GetTexture object reply");
                }
                else if (result.success &&
                    result.image.valid() &&
                    !object_texture_loading_id.empty() &&
                    !texture_residency.desired_ids.contains(
                        object_texture_loading_id))
                {
                    core::info("Discarded nonresident GetTexture reply");
                }
                else if (result.success &&
                    result.image.valid() &&
                    !object_texture_loading_id.empty() &&
                    texture_desired_mips.contains(object_texture_loading_id) &&
                    texture_desired_mips.at(object_texture_loading_id) !=
                        object_texture_loading_mip)
                {
                    core::info("Discarded out-of-date GetTexture mip result");
                }
                else if (result.success &&
                    result.image.valid() &&
                    !object_texture_loading_id.empty())
                {
                    const std::size_t next_bytes =
                        assets::gpu_rgba8_mip_bytes(result.image.width,
                                                    result.image.height);
                    const auto byte_plan = assets::plan_texture_byte_budget(
                        texture_residency.preferred_ids, texture_gpu_bytes,
                        object_texture_loading_id, next_bytes,
                        assets::effective_gpu_family_budget(
                            texture_gpu_budget_bytes,shared_gpu_budget_bytes,
                            mesh_gpu_estimated_bytes));
                    const auto slot_plan=assets::plan_texture_slot_admission(
                        texture_residency.preferred_ids,
                        object_texture_loaded_ids,byte_plan.eviction_ids,
                        object_texture_loading_id,max_object_texture_assets);
                    const auto staging=assets::gpu_staging_budget(
                        texture_gpu_used_bytes,mesh_gpu_estimated_bytes,
                        next_bytes,shared_gpu_budget_bytes);
                    if(!staging.fits)
                    {
                        ++texture_gpu_staging_deferrals;
                        texture_budget_deferred_ids.insert(
                            object_texture_loading_id);
                        texture_gpu_staging_recovery.defer(
                            object_texture_loading_id,
                            std::chrono::steady_clock::now(),
                            texture_gpu_used_bytes,mesh_gpu_estimated_bytes);
                        core::warning(
                            "Deferred GetTexture transactional GPU staging: "
                            "peak estimate exceeds reserve-adjusted budget;"
                            " candidate_bytes="+std::to_string(next_bytes)+
                            "; resident_texture_bytes="+
                            std::to_string(texture_gpu_used_bytes)+
                            "; resident_mesh_bytes="+
                            std::to_string(mesh_gpu_estimated_bytes)+
                            "; safety_reserve_bytes="+
                            std::to_string(staging.reserve_bytes));
                    }
                    else if (!byte_plan.fits || !slot_plan.fits)
                    {
                        texture_budget_deferred_ids.insert(
                            object_texture_loading_id);
                        texture_gpu_staging_recovery.defer(
                            object_texture_loading_id,
                            std::chrono::steady_clock::now(),
                            texture_gpu_used_bytes,mesh_gpu_estimated_bytes);
                        ++texture_budget_rejections;
                        core::warning("Deferred GetTexture GPU upload " +
                            object_texture_loading_id +
                            ": texture byte/slot budget exhausted");
                    }
                    else
                    {
                        std::vector<std::string> victims=byte_plan.eviction_ids;
                        victims.insert(victims.end(),
                            slot_plan.additional_evictions.begin(),
                            slot_plan.additional_evictions.end());
                        // Do not touch the old GPU slots until the new
                        // texture has been successfully uploaded.
                        const bool was_loaded =
                            object_texture_loaded_ids.contains(
                                object_texture_loading_id);
                        render::RenderImageRgba8 image;
                        image.width = result.image.width;
                        image.height = result.image.height;
                        image.pixels = std::move(result.image.pixels);
                        const auto gpu_submission_started =
                            std::chrono::steady_clock::now();
                        const auto upload_result =
                            world_renderer->replace_object_texture(
                                object_texture_loading_id,image,victims);
                        const bool texture_uploaded=upload_result.success;
                        const auto gpu_submission_end =
                            std::chrono::steady_clock::now();
                        const auto submission_cpu_time =
                            std::chrono::duration_cast<
                                std::chrono::microseconds>(
                                gpu_submission_end-gpu_submission_started);
                        asset_gpu_upload_telemetry.observe(submission_cpu_time);
                        if (!texture_uploaded)
                        {
                            ++texture_gpu_upload_failures;
                            ++texture_failure_events;
                            const auto type=upload_result.failure;
                            const auto kind=
                                type==render::GpuUploadFailure::invalid_input ||
                                type==render::GpuUploadFailure::unsupported_dimensions
                                ? assets::AssetFailure::invalid
                                : assets::AssetFailure::transient;
                            texture_failure_categories.observe(kind);
                            const auto delay=object_texture_retries.failure(
                                object_texture_loading_id,kind,
                                gpu_submission_end);
                            core::warning(
                                "GetTexture GPU upload rejected for " +
                                object_texture_loading_id +
                                "; gpu_failure=" +
                                std::string(render::gpu_upload_failure_name(type)) +
                                "; gl_error=" +
                                std::to_string(upload_result.gl_error) +
                                "; retry_delay_ms=" +
                                std::to_string(delay.count()) +
                                "; existing GPU slots preserved");
                        }
                        else
                        {
                        // The GL transaction has committed. Mirror only now
                        // its victim deletions in the CPU residency/accounting.
                        texture_slot_replacements +=
                            slot_plan.additional_evictions.size();
                        for(const auto& victim:victims)
                        {
                            object_texture_loaded_ids.erase(victim);
                            texture_gpu_mips.erase(victim);
                            if(const auto it=texture_gpu_bytes.find(victim);
                               it!=texture_gpu_bytes.end())
                            {
                                texture_gpu_used_bytes-=it->second;
                                texture_gpu_bytes.erase(it);
                            }
                            ++texture_gpu_evictions;
                        }
                        if(!victims.empty())
                            mesh_gpu_budget_deferred_ids.clear();
                        asset_gpu_upload_gate.uploaded(
                            gpu_submission_end,next_bytes,
                            submission_cpu_time);
                        if (const auto old = texture_gpu_bytes.find(
                                object_texture_loading_id);
                            old != texture_gpu_bytes.end())
                        {
                            if (next_bytes < old->second)
                                mesh_gpu_budget_deferred_ids.clear();
                            texture_gpu_used_bytes -= old->second;
                        }
                        texture_gpu_bytes[object_texture_loading_id] =
                            next_bytes;
                        texture_gpu_used_bytes += next_bytes;
                        // A texture upload can narrow remaining GPU headroom.
                        // Reconsider mesh deferrals after later evictions.
                        texture_gpu_mips[object_texture_loading_id] =
                            object_texture_loading_mip;
                        object_texture_loaded_ids.insert(
                            object_texture_loading_id);
                        object_texture_retries.success(object_texture_loading_id);
                        (void)texture_cpu_reuse_history.uploaded(
                            object_texture_loading_id,material_scope.epoch(),
                            object_texture_loading_mip,
                            result.message=="RAM texture cache hit"
                                ? assets::AssetReuseHistory::Tier::memory
                                : result.message=="Disk texture cache hit"
                                    ? assets::AssetReuseHistory::Tier::disk
                                    : assets::AssetReuseHistory::Tier::network);
                        texture_gpu_staging_recovery.resolved(
                            object_texture_loading_id);
                        if (was_loaded) ++texture_mip_swaps;
                        if (result.message == "RAM texture cache hit")
                            ++texture_ram_hits;
                        else if (result.message == "Disk texture cache hit")
                            ++texture_cache_hits;
                        else ++texture_http_loads;
                        core::info(
                            "Object texture asset loaded (" + result.message +
                            "); mip=" +
                            std::to_string(object_texture_loading_mip) +
                            "; gpu_texture_bytes=" +
                            std::to_string(texture_gpu_used_bytes) +
                            "; cached=" +
                            std::to_string(object_texture_loaded_ids.size()));
                        }
                    }
                }
                else if (!object_texture_loading_id.empty())
                {
                    const auto kind = result.failure == assets::AssetFailure::none
                        ? assets::AssetFailure::invalid : result.failure;
                    const auto delay = object_texture_retries.failure(
                        object_texture_loading_id,kind,
                        std::chrono::steady_clock::now());
                    ++texture_failure_events;
                    texture_failure_categories.observe(kind);
                    texture_missing_unique.observe(
                        object_texture_loading_id,kind);
                    core::warning(
                        "GetTexture failed for " + object_texture_loading_id +
                        ": " + result.message +
                        "; failure=" +
                        std::string(assets::asset_failure_name(kind)) +
                        "; retry_delay_ms=" + std::to_string(delay.count()) +
                        "; material fallback remains active");
                }

                object_texture_loading_id.clear();
            }

            // Capped texture streaming: prioritize nearby particle
            // images and current scene material assets, evict unneeded
            // GPU textures to keep the 512-slot cache reusable.
            std::vector<std::string> emitter_texture_ids;
            emitter_texture_ids.reserve(scene_particle_emitters.size());
            for (const auto& emitter : scene_particle_emitters)
                emitter_texture_ids.push_back(emitter.texture_id);
            const auto texture_plan_now = std::chrono::steady_clock::now();
            // Baked Head/Upper/Lower textures travel through the same
            // budgeted GetTexture, residency, cache and region lifecycle as
            // existing scene material textures. Recompute per-frame because
            // appearance can arrive without a world-object revision.
            for(const auto& ready_id:texture_gpu_staging_recovery.ready(
                    texture_plan_now,texture_gpu_used_bytes,
                    mesh_gpu_estimated_bytes))
            {
                texture_budget_deferred_ids.erase(ready_id);
                core::info("GetTexture staging retry eligible; "
                    "current residency permits re-evaluation");
            }
            if (scene_objects_changed ||
                texture_plan_now-last_texture_residency_plan >=
                    std::chrono::milliseconds{750})
            {
                last_texture_residency_plan = texture_plan_now;
                auto resident_texture_samples=scene_texture_samples;
                for(const auto& avatar:avatar_proxy_update)
                    for(const auto& baked:avatar.baked_texture_ids)
                        if(!baked.empty())
                            resident_texture_samples.push_back({
                                baked,avatar.position.x,
                                avatar.position.y,avatar.position.z});
                const std::unordered_set<std::string> active_particles{
                    emitter_texture_ids.begin(),emitter_texture_ids.end()};
                auto planned_mips = assets::plan_texture_mip_levels(
                    resident_texture_samples,render_agent.position.x,
                    render_agent.position.y,render_agent.position.z,
                    texture_desired_mips,active_particles);
                texture_lod_dwell.prune(planned_mips);
                for (auto& [id,level]:planned_mips)
                    if (!active_particles.contains(id))
                        level=texture_lod_dwell.resolve(
                            id,level,texture_gpu_mips,texture_plan_now);
                if (planned_mips != texture_desired_mips)
                {
                    for(const auto& [id,quality]:planned_mips)
                    {
                        const auto old=texture_desired_mips.find(id);
                        if(old!=texture_desired_mips.end() &&
                           old->second!=quality)
                        {
                            texture_budget_deferred_ids.erase(id);
                            texture_gpu_staging_recovery.resolved(id);
                        }
                    }
                    texture_desired_mips = std::move(planned_mips);
                }
                const auto view = render_camera_from_agent(render_agent);
                const assets::AssetCameraHint camera_hint{
                    view.center.x,view.center.y,view.center.z,
                    view.at.x,view.at.y,view.at.z};
                const auto nearby_textures = assets::rank_texture_distances(
                    resident_texture_samples,
                    render_agent.position.x,render_agent.position.y,
                    render_agent.position.z,object_texture_loaded_ids,
                    texture_priority_feedback.loaded_bonus(8.0f),
                    &camera_hint);
                auto updated_residency = assets::plan_texture_residency(
                    emitter_texture_ids, nearby_textures,
                    object_texture_loaded_ids, max_object_texture_assets);
                texture_residency = std::move(updated_residency);
                texture_gpu_staging_recovery.synchronize(
                    texture_budget_deferred_ids,texture_residency.desired_ids);
                texture_priority_feedback.observe(texture_residency.desired_ids);
                std::size_t evicted = 0U;
                for (const auto& id:texture_residency.eviction_ids)
                {
                    if(evicted>=8U)break;
                    world_renderer->remove_object_texture(id);
                    object_texture_loaded_ids.erase(id);
                    texture_gpu_mips.erase(id);
                    if (const auto it=texture_gpu_bytes.find(id);
                        it != texture_gpu_bytes.end())
                    {
                        texture_gpu_used_bytes -= it->second;
                        texture_gpu_bytes.erase(it);
                    }
                    ++evicted;
                }
                texture_gpu_evictions += evicted;
                if(evicted>0U) mesh_gpu_budget_deferred_ids.clear();
                if(evicted>0U)
                    core::info("GetTexture GPU residency evicted=" +
                        std::to_string(evicted));
            }
            if (!object_texture_loading && !texture_capability.empty() &&
                texture_request_pacer.ready(texture_plan_now))
            {
                std::unordered_set<std::string> satisfied;
                for (const auto& [id,level] : texture_gpu_mips)
                {
                    const auto target = texture_desired_mips.find(id);
                    if (target == texture_desired_mips.end()
                        ? level == 0U : level == target->second)
                        satisfied.insert(id);
                }
                const auto request_choice=texture_request_fairness.choose(
                    texture_residency.preferred_ids,satisfied,
                    texture_budget_deferred_ids,object_texture_retries,
                    texture_plan_now);
                if (request_choice)
                {
                    const std::string& texture_id=request_choice->id;
                    // A full GPU table is not by itself a reason to
                    // reject a high-priority request: the upload planner
                    // can atomically replace lower-priority resident slots.
                    // This is only preflight; decoded byte size still gets
                    // checked against the staging and byte budgets later.
                    const auto preflight=assets::plan_texture_slot_admission(
                        texture_residency.preferred_ids,
                        object_texture_loaded_ids,
                        texture_residency.eviction_ids,
                        texture_id,max_object_texture_assets);
                    if (!preflight.fits)
                    {
                        texture_budget_deferred_ids.insert(texture_id);
                        texture_gpu_staging_recovery.defer(texture_id,
                            texture_plan_now,texture_gpu_used_bytes,
                            mesh_gpu_estimated_bytes);
                        ++texture_budget_rejections;
                    }
                    else
                    {
                    if (request_choice->is_retry)
                        ++texture_retry_dispatches;
                    object_texture_loading_id = texture_id;
                    const auto desired_mip = texture_desired_mips.find(texture_id);
                    object_texture_loading_mip =
                        desired_mip == texture_desired_mips.end()
                            ? 0U : desired_mip->second;
                    object_texture_loading = true;
                    texture_request_started = std::chrono::steady_clock::now();
                    texture_request_pacer.dispatched(texture_request_started);
                    texture_request_fairness.dispatched();
                    ++texture_request_dispatches;
                    object_texture_request_epoch = material_scope.epoch();
                    const std::string capability = texture_capability;
                    const std::string requested_id = texture_id;
                    object_texture_future = std::async(
                        std::launch::async,
                        [transport, capability, requested_id,
                         mip=object_texture_loading_mip, texture_memory_cache,
                         live_epoch=texture_prefetch_live_epoch,
                         expected_epoch=object_texture_request_epoch]()
                        {
                            const auto active = [&]()
                            {
                                return live_epoch->load(
                                    std::memory_order_acquire)==expected_epoch;
                            };
                            assets::TextureDiskCache disk{
                                assets::TextureDiskCache::default_directory()};
                            return assets::load_texture_cached_scoped(
                                requested_id,mip,*texture_memory_cache,disk,
                                [&]()
                                {
                                    return assets::TextureAssetLoader{transport}
                                        .fetch_rgba(capability,requested_id);
                                },active);
                        });
                    }
                }
            }

            // Warm at most one decoded texture from the existing disk cache
            // in a worker. No prefetch HTTP request, GPU upload or CAPS URL
            // is retained. Region epoch protects stale worker admission.
            if (texture_prefetch_future.valid() &&
                texture_prefetch_future.wait_for(std::chrono::seconds{0}) ==
                    std::future_status::ready)
            {
                const bool warmed = texture_prefetch_future.get();
                if (texture_prefetch_scope.current(
                        texture_prefetch_request_epoch))
                {
                    if (warmed) ++texture_prefetch_warmed;
                    else ++texture_prefetch_misses;
                }
                else ++texture_prefetch_stale;
            }
            if (!texture_prefetch_future.valid() &&
                texture_prefetch_scope.current(material_scope.epoch()) &&
                !texture_ram_pressure.under_pressure() &&
                texture_prefetch_scope.attempted() < 48U &&
                texture_prefetch_pacer.ready(texture_plan_now))
            {
                std::unordered_set<std::string> temporarily_unavailable;
                for (const auto& id : texture_residency.preferred_ids)
                    if (!object_texture_retries.ready(id,texture_plan_now))
                        temporarily_unavailable.insert(id);
                const auto pending = texture_prefetch_scope.candidates(
                    texture_residency.preferred_ids,
                    object_texture_loaded_ids,temporarily_unavailable,4U);
                for (const auto& id : pending)
                {
                    if (texture_memory_cache->contains(id))
                    {
                        texture_prefetch_scope.dispatched(id);
                        continue;
                    }
                    texture_prefetch_scope.dispatched(id);
                    ++texture_prefetch_attempts;
                    texture_prefetch_request_epoch = material_scope.epoch();
                    texture_prefetch_pacer.dispatched(texture_plan_now);
                    const auto request_epoch=texture_prefetch_request_epoch;
                    texture_prefetch_future = std::async(std::launch::async,
                        [id,texture_memory_cache,texture_prefetch_live_epoch,
                         request_epoch]()
                        {
                            assets::TextureDiskCache disk{
                                assets::TextureDiskCache::default_directory()};
                            return assets::prefetch_texture_disk_to_memory(
                                id,disk,*texture_memory_cache,
                                *texture_prefetch_live_epoch,request_epoch);
                        });
                    break;
                }
            }

            if (last_texture_telemetry.time_since_epoch().count()==0 ||
                texture_plan_now-last_texture_telemetry >=
                    std::chrono::seconds{60})
            {
                last_texture_telemetry = texture_plan_now;
                core::info("GetTexture streaming metrics: ram_hits=" +
                    std::to_string(texture_ram_hits) +
                    " ram_assets=" +
                    std::to_string(texture_memory_cache->count()) +
                    " ram_bytes=" +
                    std::to_string(texture_memory_cache->bytes()) +
                    " demand_dispatches=" +
                    std::to_string(texture_request_dispatches) +
                    " retry_dispatches=" +
                    std::to_string(texture_retry_dispatches) +
                    " failure_events=" +
                    std::to_string(texture_failure_events) +
                    " backoff_assets=" +
                    std::to_string(object_texture_retries.count()) +
                    " currently_blocked=" +
                    std::to_string(object_texture_retries.blocked_count(
                        texture_plan_now)) +
                    " ready_retries=" +
                    std::to_string(object_texture_retries.ready_retry_count(
                        texture_plan_now)) +
                    " transient_failures=" +
                    std::to_string(object_texture_retries.count_kind(
                        assets::AssetFailure::transient)) +
                    " stale_results=" +
                    std::to_string(texture_stale_results) +
                    " prefetch_probes=" +
                    std::to_string(texture_prefetch_attempts) +
                    " prefetch_warmed=" +
                    std::to_string(texture_prefetch_warmed) +
                    " prefetch_missed=" +
                    std::to_string(texture_prefetch_misses) +
                    " prefetch_stale=" +
                    std::to_string(texture_prefetch_stale) +
                    " worker_completions=" +
                    std::to_string(texture_worker_completions) +
                    " avg_worker_elapsed_ms=" +
                    std::to_string(texture_worker_completions
                        ? texture_worker_elapsed_ms/texture_worker_completions
                        : 0U) +
                    " disk_hits=" + std::to_string(texture_cache_hits) +
                    " disk_misses=" + std::to_string(texture_disk_misses) +
                    " disk_corrupt=" + std::to_string(texture_disk_corrupt) +
                    " disk_io_errors=" + std::to_string(texture_disk_io_errors) +
                    " http_attempts=" + std::to_string(texture_network_attempts) +
                    " skipped_before_http=" +
                    std::to_string(texture_aborted_before_http) +
                    " stale_after_http=" +
                    std::to_string(texture_discarded_after_http) +
                    " network_loads=" + std::to_string(texture_http_loads) +
                    " gpu_evictions=" + std::to_string(texture_gpu_evictions) +
                    " mip_swaps=" +
                    std::to_string(texture_mip_swaps) +
                    " mip_dwell_pending=" +
                    std::to_string(texture_lod_dwell.waiting()) +
                    " slot_priority_evictions=" +
                    std::to_string(texture_slot_replacements) +
                    " gpu_upload_failures=" +
                    std::to_string(texture_gpu_upload_failures) +
                    " gpu_staging_deferrals=" +
                    std::to_string(texture_gpu_staging_deferrals) +
                    " gpu_staging_waiting=" +
                    std::to_string(texture_gpu_staging_recovery.pending()) +
                    " gpu_staging_released=" +
                    std::to_string(texture_gpu_staging_recovery.released()) +
                    " cpu_cached_region_reuse=" +
                    std::to_string(texture_cpu_reuse_history.region_cache_hits()) +
                    " cpu_cached_lod_reuse=" +
                    std::to_string(texture_cpu_reuse_history.lod_cache_hits()) +
                    " gpu_uploads=" +
                    std::to_string(asset_gpu_upload_gate.count()) +
                    " gpu_uploaded_bytes=" +
                    std::to_string(asset_gpu_upload_gate.bytes()) +
                    " gpu_submit_cpu_p95_us=" +
                    std::to_string(asset_gpu_upload_telemetry.recent_p95_us()) +
                    " gpu_submit_cpu_max_us=" +
                    std::to_string(asset_gpu_upload_telemetry.recent_max_us()) +
                    " gpu_upload_pacing_ms=" +
                    std::to_string(
                        asset_gpu_upload_gate.effective_interval().count()) +
                    " texture_priority_churn=" +
                    std::to_string(texture_priority_feedback.churn_events()) +
                    " budget_deferrals=" +
                    std::to_string(texture_budget_rejections) +
                    " gpu_texture_bytes=" +
                    std::to_string(texture_gpu_used_bytes) +
                    " gpu_texture_byte_budget=" +
                    std::to_string(texture_gpu_budget_bytes) +
                    " gpu_resident=" +
                    std::to_string(object_texture_loaded_ids.size()) +
                    " gpu_budget=" +
                    std::to_string(max_object_texture_assets));
            }

            // Select the closest distinct mesh assets to the avatar.
            // Already-resident assets receive hysteresis so tiny movement
            // does not cause repeated GPU uploads/evictions.
            const auto mesh_plan_now = std::chrono::steady_clock::now();
            for(const auto& key:mesh_gpu_budget_recovery.ready(
                    mesh_plan_now,texture_gpu_used_bytes,
                    mesh_gpu_estimated_bytes))
                mesh_gpu_budget_deferred_ids.erase(key);
            if (scene_objects_changed ||
                mesh_plan_now - last_mesh_residency_plan >=
                    std::chrono::milliseconds{750})
            {
                last_mesh_residency_plan = mesh_plan_now;
                auto planned_lods = assets::plan_mesh_lods(
                    scene_mesh_samples,
                    render_agent.position.x, render_agent.position.y,
                    render_agent.position.z, mesh_desired_lods);
                mesh_lod_dwell.prune(planned_lods);
                for(auto& [id,lod]:planned_lods)
                    lod=mesh_lod_dwell.resolve(
                        id,lod,mesh_gpu_lods,mesh_plan_now);
                mesh_desired_lods=std::move(planned_lods);
                const auto view = render_camera_from_agent(render_agent);
                const assets::AssetCameraHint camera_hint{
                    view.center.x,view.center.y,view.center.z,
                    view.at.x,view.at.y,view.at.z};
                mesh_residency = assets::plan_mesh_residency(
                    scene_mesh_samples,
                    render_agent.position.x,
                    render_agent.position.y,
                    render_agent.position.z,
                    mesh_asset_loaded_ids,
                    mesh_gpu_asset_budget,
                    mesh_priority_feedback.loaded_bonus(24.0f),
                    &camera_hint);
                mesh_priority_feedback.observe(mesh_residency.desired_ids);
                std::unordered_set<std::string> wanted_keys;
                for(const auto& id:mesh_residency.desired_ids)
                {
                    const auto it=mesh_desired_lods.find(id);
                    if(it!=mesh_desired_lods.end())
                        wanted_keys.insert(id+"."+
                            std::string(assets::mesh_lod_name(it->second)));
                }
                mesh_gpu_budget_recovery.synchronize(
                    mesh_gpu_budget_deferred_ids,wanted_keys);

                // Evict in small batches to avoid long OpenGL stalls. The
                // next frame can immediately start loading newly relevant
                // meshes, even when the cache previously hit its limit.
                std::size_t evicted = 0U;
                for (const auto& id : mesh_residency.eviction_ids)
                {
                    if (evicted >= 8U) break;
                    world_renderer->remove_mesh_asset(id);
                    mesh_asset_loaded_ids.erase(id);
                    mesh_gpu_lods.erase(id);
                    if (const auto it = mesh_gpu_asset_bytes.find(id);
                        it != mesh_gpu_asset_bytes.end())
                    {
                        mesh_gpu_estimated_bytes -= it->second;
                        mesh_gpu_asset_bytes.erase(it);
                    }
                    ++mesh_gpu_evictions;
                    mesh_gpu_budget_deferred_ids.clear();
                    texture_budget_deferred_ids.clear();
                    ++evicted;
                }
                if (evicted > 0U)
                    core::info("GetMesh cache evicted distant assets=" +
                        std::to_string(evicted) +
                        "; cached=" +
                        std::to_string(mesh_asset_loaded_ids.size()));
            }

            // Asset decoding runs off-thread, GPU uploads only on the
            // renderer thread. Never upload results from a former region
            // or for meshes that have left the current residency set.
            if (mesh_asset_future.valid() &&
                mesh_asset_future.wait_for(std::chrono::seconds{0}) ==
                    std::future_status::ready &&
                asset_gpu_upload_gate.ready(std::chrono::steady_clock::now()))
            {
                auto completed_mesh = mesh_asset_future.get();
                if (mesh_request_started.time_since_epoch().count()!=0)
                {
                    ++mesh_worker_completions;
                    mesh_worker_elapsed_ms += static_cast<std::uint64_t>(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now()-
                            mesh_request_started).count());
                }
                assets::MeshAssetResult result = std::move(completed_mesh.asset);
                if (!material_scope.matches(mesh_asset_request_epoch))
                {
                    ++mesh_stale_results;
                    if (result.message.find("before ")!=std::string::npos)
                        ++mesh_aborted_before_http;
                    else if (result.message.find("after HTTP")!=std::string::npos)
                        ++mesh_discarded_after_http;
                    core::info("Discarded stale GetMesh response");
                }
                else if (!mesh_residency.desired_ids.contains(
                             mesh_asset_loading_id))
                {
                    core::info("Discarded no-longer-prioritized GetMesh response");
                }
                else if (const auto it = mesh_desired_lods.find(
                             mesh_asset_loading_id);
                         it == mesh_desired_lods.end() ||
                         it->second != mesh_asset_loading_lod)
                {
                    core::info("Discarded out-of-date GetMesh LOD response");
                }
                else if (result.success && result.geometry.valid())
                {
                    const std::size_t gpu_bytes =
                        result.geometry.vertices.size()*sizeof(float) +
                        result.geometry.indices.size()*sizeof(std::uint32_t);
                    const bool replacement =
                        mesh_gpu_lods.contains(mesh_asset_loading_id);
                    const auto current_bytes = mesh_gpu_asset_bytes.find(
                        mesh_asset_loading_id);
                    const std::size_t old_bytes =
                        current_bytes==mesh_gpu_asset_bytes.end()
                            ? 0U : current_bytes->second;
                    const std::size_t budget =
                        assets::effective_gpu_family_budget(
                            mesh_gpu_budget_bytes,shared_gpu_budget_bytes,
                            texture_gpu_used_bytes);
                    if (!assets::can_replace_gpu_bytes(
                            mesh_gpu_estimated_bytes,old_bytes,
                            gpu_bytes,budget))
                    {
                        const auto retry_key=mesh_asset_loading_id+"."+
                            std::string(assets::mesh_lod_name(mesh_asset_loading_lod));
                        mesh_gpu_budget_deferred_ids.insert(retry_key);
                        mesh_gpu_budget_recovery.defer(retry_key,
                            std::chrono::steady_clock::now(),
                            texture_gpu_used_bytes,mesh_gpu_estimated_bytes);
                        ++mesh_gpu_budget_deferrals;
                        core::warning("Deferred GetMesh GPU upload due to "
                            "combined mesh/texture byte budget");
                    }
                    else
                    {
                    render::RenderMeshGeometry geometry;
                    geometry.vertices = std::move(result.geometry.vertices);
                    geometry.indices = std::move(result.geometry.indices);
                    for (const auto& face : result.geometry.faces)
                        geometry.faces.push_back({
                            face.face, face.first_index, face.index_count});
                    const auto gpu_submission_started =
                        std::chrono::steady_clock::now();
                    const auto mesh_upload=world_renderer->upload_mesh_asset(
                        mesh_asset_loading_id,geometry);
                    const bool mesh_uploaded=mesh_upload.success;
                    const auto gpu_submission_end =
                        std::chrono::steady_clock::now();
                    const auto submission_cpu_time =
                        std::chrono::duration_cast<
                            std::chrono::microseconds>(
                            gpu_submission_end-gpu_submission_started);
                    asset_gpu_upload_telemetry.observe(submission_cpu_time);
                    if (mesh_uploaded)
                    {
                        asset_gpu_upload_gate.uploaded(
                            gpu_submission_end,gpu_bytes,
                            submission_cpu_time);
                        mesh_asset_loaded_ids.insert(mesh_asset_loading_id);
                        const auto successful_key=mesh_asset_loading_id+"."+
                            std::string(assets::mesh_lod_name(mesh_asset_loading_lod));
                        mesh_asset_retries.success(successful_key);
                        mesh_gpu_budget_recovery.resolved(successful_key);
                        (void)mesh_cpu_reuse_history.uploaded(
                            mesh_asset_loading_id,material_scope.epoch(),
                            static_cast<std::uint32_t>(mesh_asset_loading_lod),
                            completed_mesh.tier==assets::MeshCacheTier::memory
                                ? assets::AssetReuseHistory::Tier::memory
                                : completed_mesh.tier==assets::MeshCacheTier::disk
                                    ? assets::AssetReuseHistory::Tier::disk
                                    : assets::AssetReuseHistory::Tier::network);
                        if (replacement) ++mesh_lod_swaps;
                        if (const auto old = mesh_gpu_asset_bytes.find(
                                mesh_asset_loading_id);
                            old != mesh_gpu_asset_bytes.end())
                            mesh_gpu_estimated_bytes -= old->second;
                        mesh_gpu_asset_bytes[mesh_asset_loading_id] = gpu_bytes;
                        mesh_gpu_estimated_bytes += gpu_bytes;
                        if (gpu_bytes < old_bytes)
                            texture_budget_deferred_ids.clear();
                        mesh_gpu_lods[mesh_asset_loading_id] =
                            mesh_asset_loading_lod;
                        switch (completed_mesh.tier)
                        {
                        case assets::MeshCacheTier::memory: ++mesh_ram_hits; break;
                        case assets::MeshCacheTier::disk: ++mesh_disk_hits; break;
                        case assets::MeshCacheTier::network: ++mesh_network_loads; break;
                        }
                        core::info(
                            "GetMesh loaded " + mesh_asset_loading_id +
                            "; requested_lod=" +
                            std::string(assets::mesh_lod_name(mesh_asset_loading_lod)) +
                            "; " + result.message +
                            "; cached=" +
                            std::to_string(mesh_asset_loaded_ids.size()));
                    }
                    else
                    {
                        const auto gpu_kind=
                            mesh_upload.failure==
                                render::GpuUploadFailure::invalid_input ||
                            mesh_upload.failure==
                                render::GpuUploadFailure::unsupported_dimensions
                                ? assets::AssetFailure::invalid
                                : assets::AssetFailure::transient;
                        mesh_failure_categories.observe(gpu_kind);
                        const auto delay = mesh_asset_retries.failure(
                            mesh_asset_loading_id + "." +
                                std::string(assets::mesh_lod_name(
                                    mesh_asset_loading_lod)),
                            gpu_kind,std::chrono::steady_clock::now());
                        ++mesh_failure_events;
                        ++mesh_gpu_upload_failures;
                        core::warning("GetMesh GPU upload failed for " +
                            mesh_asset_loading_id +
                            "; gpu_failure=" + std::string(
                                render::gpu_upload_failure_name(
                                    mesh_upload.failure)) +
                            "; gl_error=" +
                            std::to_string(mesh_upload.gl_error) +
                            "; retry_delay_ms=" + std::to_string(delay.count()));
                    }
                    }
                }
                else
                {
                    const auto kind = result.failure == assets::AssetFailure::none
                        ? assets::AssetFailure::invalid : result.failure;
                    const auto delay = mesh_asset_retries.failure(
                        mesh_asset_loading_id + "." +
                            std::string(assets::mesh_lod_name(
                                mesh_asset_loading_lod)),
                        kind,std::chrono::steady_clock::now());
                    ++mesh_failure_events;
                    mesh_failure_categories.observe(kind);
                    mesh_missing_unique.observe(mesh_asset_loading_id,kind);
                    core::warning(
                        "GetMesh failed for " + mesh_asset_loading_id +
                        ": " + result.message + "; failure=" +
                        std::string(assets::asset_failure_name(kind)) +
                        "; retry_delay_ms=" + std::to_string(delay.count()) +
                        "; placeholder remains visible");
                }
                mesh_asset_loading_id.clear();
            }
            if (!mesh_asset_future.valid() &&
                !scoped_get_mesh_uri.empty() &&
                mesh_request_pacer.ready(mesh_plan_now))
            {
                // Keep original view/distance ranking inside each class,
                // but permit ready retries to run after a burst of new assets.
                const auto request_choice=mesh_request_fairness.choose(
                    mesh_residency.prioritized_ids,{},
                    mesh_gpu_budget_deferred_ids,mesh_asset_retries,
                    mesh_plan_now,
                    [&](const std::string& id)
                    {
                        const auto desired=mesh_desired_lods.find(id);
                        if(desired==mesh_desired_lods.end())
                            return std::string{};
                        return id+"."+
                            std::string(assets::mesh_lod_name(desired->second));
                    },
                    [&](const std::string& id)
                    {
                        const auto desired=mesh_desired_lods.find(id);
                        if(desired==mesh_desired_lods.end())return false;
                        const auto existing=mesh_gpu_lods.find(id);
                        if(existing!=mesh_gpu_lods.end() &&
                           existing->second==desired->second)return false;
                        return mesh_asset_loaded_ids.contains(id) ||
                            mesh_asset_loaded_ids.size()<mesh_gpu_asset_budget;
                    });
                if (request_choice)
                {
                    const auto& id=request_choice->id;
                    const auto lod=mesh_desired_lods.at(id);
                    if (request_choice->is_retry)
                        ++mesh_retry_dispatches;
                    mesh_asset_loading_id = id;
                    mesh_request_started = std::chrono::steady_clock::now();
                    mesh_request_pacer.dispatched(mesh_request_started);
                    mesh_request_fairness.dispatched();
                    ++mesh_request_dispatches;
                    mesh_asset_loading_lod = lod;
                    mesh_asset_request_epoch = material_scope.epoch();
                    const std::string cap = scoped_get_mesh_uri;
                    const std::string alt_cap = scoped_get_mesh_alt_uri;
                    mesh_asset_future = std::async(
                        std::launch::async,
                        [transport, cap, alt_cap, id, lod, mesh_memory_cache,
                         live_epoch=texture_prefetch_live_epoch,
                         expected_epoch=mesh_asset_request_epoch]()
                        {
                            const auto active=[&]()
                            {
                                return live_epoch->load(
                                    std::memory_order_acquire)==expected_epoch;
                            };
                            assets::MeshDiskCache disk{
                                assets::MeshDiskCache::default_directory()};
                            return assets::load_mesh_cached(id,
                                *mesh_memory_cache,disk,
                                [&transport,&cap,&alt_cap,&id,lod]()
                                {
                                    return assets::MeshAssetLoader{transport}
                                        .fetch(cap,id,alt_cap,lod);
                                },lod,active);
                        });
                    core::info("GetMesh request started for " + id +
                        "; requested_lod=" + std::string(assets::mesh_lod_name(lod)));
                }
            }

            if (last_mesh_telemetry.time_since_epoch().count() == 0 ||
                mesh_plan_now - last_mesh_telemetry >=
                    std::chrono::seconds{60})
            {
                last_mesh_telemetry = mesh_plan_now;
                core::info("GetMesh streaming metrics: ram_hits=" +
                    std::to_string(mesh_ram_hits) +
                    " disk_hits=" + std::to_string(mesh_disk_hits) +
                    " network_loads=" + std::to_string(mesh_network_loads) +
                    " gpu_evictions=" + std::to_string(mesh_gpu_evictions) +
                    " ram_bytes=" + std::to_string(mesh_memory_cache->bytes()) +
                    " ram_assets=" + std::to_string(mesh_memory_cache->count()) +
                    " lod_swaps=" + std::to_string(mesh_lod_swaps) +
                    " lod_dwell_pending=" +
                    std::to_string(mesh_lod_dwell.waiting()) +
                    " mesh_priority_churn=" +
                    std::to_string(mesh_priority_feedback.churn_events()) +
                    " gpu_upload_failures=" +
                    std::to_string(mesh_gpu_upload_failures) +
                    " gpu_estimated_bytes=" + std::to_string(mesh_gpu_estimated_bytes) +
                    " shared_gpu_estimated_bytes=" +
                    std::to_string(mesh_gpu_estimated_bytes +
                        texture_gpu_used_bytes) +
                    " shared_gpu_byte_budget=" +
                    std::to_string(shared_gpu_budget_bytes) +
                    " mesh_budget_deferrals=" +
                    std::to_string(mesh_gpu_budget_deferrals) +
                    " mesh_budget_waiting=" +
                    std::to_string(mesh_gpu_budget_recovery.pending()) +
                    " mesh_budget_released=" +
                    std::to_string(mesh_gpu_budget_recovery.released()) +
                    " cpu_cached_region_reuse=" +
                    std::to_string(mesh_cpu_reuse_history.region_cache_hits()) +
                    " cpu_cached_lod_reuse=" +
                    std::to_string(mesh_cpu_reuse_history.lod_cache_hits()) +
                    " demand_dispatches=" +
                    std::to_string(mesh_request_dispatches) +
                    " retry_dispatches=" +
                    std::to_string(mesh_retry_dispatches) +
                    " failure_events=" +
                    std::to_string(mesh_failure_events) +
                    " backoff_assets=" +
                    std::to_string(mesh_asset_retries.count()) +
                    " currently_blocked=" +
                    std::to_string(mesh_asset_retries.blocked_count(
                        mesh_plan_now)) +
                    " ready_retries=" +
                    std::to_string(mesh_asset_retries.ready_retry_count(
                        mesh_plan_now)) +
                    " transient_failures=" +
                    std::to_string(mesh_asset_retries.count_kind(
                        assets::AssetFailure::transient)) +
                    " stale_results=" +
                    std::to_string(mesh_stale_results) +
                    " skipped_before_http=" +
                    std::to_string(mesh_aborted_before_http) +
                    " stale_after_http=" +
                    std::to_string(mesh_discarded_after_http) +
                    " worker_completions=" +
                    std::to_string(mesh_worker_completions) +
                    " avg_worker_elapsed_ms=" +
                    std::to_string(mesh_worker_completions
                        ? mesh_worker_elapsed_ms/mesh_worker_completions : 0U) +
                    " gpu_resident=" +
                    std::to_string(mesh_asset_loaded_ids.size()) +
                    " gpu_budget=" + std::to_string(mesh_gpu_asset_budget));
            }

            if (simulation_ready &&
                (last_world_asset_health_log.time_since_epoch().count()==0 ||
                 mesh_plan_now-last_world_asset_health_log >=
                    std::chrono::seconds{30}))
            {
                last_world_asset_health_log=mesh_plan_now;
                const auto texture_coverage=assets::asset_scene_coverage(
                    texture_residency.preferred_ids,object_texture_loaded_ids);
                const auto mesh_coverage=assets::asset_scene_coverage(
                    mesh_residency.prioritized_ids,mesh_asset_loaded_ids);
                assets::AssetStreamingSnapshot snapshot;
                snapshot.region_epoch=material_scope.epoch();
                snapshot.session_started_unix_ms=inworld_session_started_ms;
                snapshot.emitted_unix_ms=static_cast<std::uint64_t>(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count());
                snapshot.sequence=++inworld_snapshot_sequence;
                snapshot.texture_scene_samples=scene_texture_samples.size();
                snapshot.mesh_scene_samples=scene_mesh_samples.size();
                snapshot.textures=texture_coverage;
                snapshot.meshes=mesh_coverage;
                snapshot.texture_in_flight=object_texture_loading?1U:0U;
                snapshot.mesh_in_flight=mesh_asset_future.valid()?1U:0U;
                snapshot.texture_retry_blocked=
                    object_texture_retries.blocked_count(mesh_plan_now);
                snapshot.mesh_retry_blocked=
                    mesh_asset_retries.blocked_count(mesh_plan_now);
                snapshot.texture_ready_retries=
                    object_texture_retries.ready_retry_count(mesh_plan_now);
                snapshot.mesh_ready_retries=
                    mesh_asset_retries.ready_retry_count(mesh_plan_now);
                const auto texture_ram_stats=texture_memory_cache->stats();
                const auto mesh_ram_stats=mesh_memory_cache->stats();
                texture_ram_pressure.observe(texture_ram_stats);
                mesh_ram_pressure.observe(mesh_ram_stats);
                snapshot.texture_ram_hit_rate_pct=texture_ram_pressure.hit_pct();
                snapshot.mesh_ram_hit_rate_pct=mesh_ram_pressure.hit_pct();
                snapshot.texture_ram_interval_evictions=
                    texture_ram_pressure.interval_evictions();
                snapshot.mesh_ram_interval_evictions=
                    mesh_ram_pressure.interval_evictions();
                snapshot.texture_ram_pressure=
                    texture_ram_pressure.under_pressure()?1U:0U;
                snapshot.mesh_ram_pressure=
                    mesh_ram_pressure.under_pressure()?1U:0U;
                snapshot.texture_ram_bytes=texture_ram_stats.bytes;
                snapshot.mesh_ram_bytes=mesh_ram_stats.bytes;
                snapshot.texture_ram_hits=texture_ram_stats.hits;
                snapshot.texture_ram_misses=texture_ram_stats.misses;
                snapshot.texture_ram_evictions=texture_ram_stats.evictions;
                snapshot.mesh_ram_hits=mesh_ram_stats.hits;
                snapshot.mesh_ram_misses=mesh_ram_stats.misses;
                snapshot.mesh_ram_evictions=mesh_ram_stats.evictions;
                snapshot.gpu_texture_bytes=texture_gpu_used_bytes;
                snapshot.gpu_mesh_bytes=mesh_gpu_estimated_bytes;
                snapshot.gpu_shared_budget_bytes=shared_gpu_budget_bytes;
                snapshot.gpu_upload_cpu_samples=
                    asset_gpu_upload_telemetry.recent_count();
                snapshot.gpu_upload_cpu_avg_us=
                    asset_gpu_upload_telemetry.recent_avg_us();
                snapshot.gpu_upload_cpu_p95_us=
                    asset_gpu_upload_telemetry.recent_p95_us();
                snapshot.gpu_upload_cpu_max_us=
                    asset_gpu_upload_telemetry.recent_max_us();
                snapshot.gpu_upload_pacing_ms=static_cast<std::size_t>(
                    asset_gpu_upload_gate.effective_interval().count());
                snapshot.texture_gpu_upload_failures=
                    texture_gpu_upload_failures;
                snapshot.texture_gpu_staging_deferrals=
                    texture_gpu_staging_deferrals;
                snapshot.texture_gpu_staging_waiting=
                    texture_gpu_staging_recovery.pending();
                snapshot.texture_gpu_staging_released=
                    texture_gpu_staging_recovery.released();
                snapshot.mesh_gpu_budget_waiting=
                    mesh_gpu_budget_recovery.pending();
                snapshot.mesh_gpu_budget_released=
                    mesh_gpu_budget_recovery.released();
                snapshot.texture_cpu_cached_region_reuse=
                    texture_cpu_reuse_history.region_cache_hits();
                snapshot.texture_cpu_cached_lod_reuse=
                    texture_cpu_reuse_history.lod_cache_hits();
                snapshot.mesh_cpu_cached_region_reuse=
                    mesh_cpu_reuse_history.region_cache_hits();
                snapshot.mesh_cpu_cached_lod_reuse=
                    mesh_cpu_reuse_history.lod_cache_hits();
                snapshot.texture_cpu_cached_region_ram=
                    texture_cpu_reuse_history.region_ram_hits();
                snapshot.texture_cpu_cached_region_disk=
                    texture_cpu_reuse_history.region_disk_hits();
                snapshot.texture_cpu_cached_lod_ram=
                    texture_cpu_reuse_history.lod_ram_hits();
                snapshot.texture_cpu_cached_lod_disk=
                    texture_cpu_reuse_history.lod_disk_hits();
                snapshot.mesh_cpu_cached_region_ram=
                    mesh_cpu_reuse_history.region_ram_hits();
                snapshot.mesh_cpu_cached_region_disk=
                    mesh_cpu_reuse_history.region_disk_hits();
                snapshot.mesh_cpu_cached_lod_ram=
                    mesh_cpu_reuse_history.lod_ram_hits();
                snapshot.mesh_cpu_cached_lod_disk=
                    mesh_cpu_reuse_history.lod_disk_hits();
                snapshot.texture_cpu_reuse_history_evictions=
                    texture_cpu_reuse_history.evictions();
                snapshot.mesh_cpu_reuse_history_evictions=
                    mesh_cpu_reuse_history.evictions();
                snapshot.mesh_gpu_upload_failures=
                    mesh_gpu_upload_failures;
                snapshot.mesh_fail_missing=mesh_failure_categories.missing;
                snapshot.mesh_fail_transient=mesh_failure_categories.transient;
                snapshot.mesh_fail_invalid=mesh_failure_categories.invalid;
                snapshot.mesh_fail_configuration=
                    mesh_failure_categories.configuration;
                snapshot.mesh_missing_unique=mesh_missing_unique.unique();
                snapshot.mesh_missing_unique_overflow=
                    mesh_missing_unique.overflow();
                snapshot.texture_fail_missing=
                    texture_failure_categories.missing;
                snapshot.texture_fail_transient=
                    texture_failure_categories.transient;
                snapshot.texture_fail_invalid=
                    texture_failure_categories.invalid;
                snapshot.texture_fail_configuration=
                    texture_failure_categories.configuration;
                snapshot.texture_missing_unique=
                    texture_missing_unique.unique();
                snapshot.texture_missing_unique_overflow=
                    texture_missing_unique.overflow();
                snapshot.texture_priority_churn=
                    texture_priority_feedback.churn_events();
                snapshot.mesh_priority_churn=
                    mesh_priority_feedback.churn_events();
                const auto previous_render=world_renderer->diagnostics();
                snapshot.previous_frame_draw_calls=previous_render.draw_calls;
                snapshot.previous_frame_visible_objects=
                    previous_render.visible_objects;
                snapshot.previous_frame_scene_draw_calls=
                    previous_render.scene_draw_calls;
                snapshot.previous_frame_scene_mesh_draw_calls=
                    previous_render.scene_mesh_draw_calls;
                snapshot.previous_frame_scene_prim_draw_calls=
                    previous_render.scene_prim_draw_calls;
                snapshot.previous_frame_scene_opaque_draw_calls=
                    previous_render.scene_opaque_draw_calls;
                snapshot.previous_frame_scene_alpha_draw_calls=
                    previous_render.scene_alpha_draw_calls;
                snapshot.previous_frame_scene_alpha_mask_draw_calls=
                    previous_render.scene_alpha_mask_draw_calls;
                snapshot.previous_frame_scene_missing_mesh_draw_calls=
                    previous_render.scene_missing_mesh_draw_calls;
                snapshot.previous_frame_scene_missing_mesh_objects=
                    previous_render.scene_missing_mesh_objects;
                snapshot.previous_frame_scene_missing_mesh_triangles=
                    previous_render.scene_missing_mesh_triangles;
                core::info("InWorldAssetSnapshot " + snapshot.json());
                core::info("InWorld asset health: epoch=" +
                    std::to_string(material_scope.epoch()) +
                    " scene_texture_samples=" +
                    std::to_string(scene_texture_samples.size()) +
                    " scene_mesh_samples=" +
                    std::to_string(scene_mesh_samples.size()) +
                    " textures_loaded=" +
                    std::to_string(texture_coverage.resident) +
                    "/" + std::to_string(texture_coverage.requested) +
                    " texture_coverage_pct=" +
                    std::to_string(texture_coverage.percentage()) +
                    " textures_pending=" +
                    std::to_string(texture_coverage.pending) +
                    " meshes_loaded=" +
                    std::to_string(mesh_coverage.resident) +
                    "/" + std::to_string(mesh_coverage.requested) +
                    " mesh_coverage_pct=" +
                    std::to_string(mesh_coverage.percentage()) +
                    " meshes_pending=" +
                    std::to_string(mesh_coverage.pending) +
                    " texture_in_flight=" +
                    std::to_string(object_texture_loading?1U:0U) +
                    " mesh_in_flight=" +
                    std::to_string(mesh_asset_future.valid()?1U:0U) +
                    " texture_failures_tracked=" +
                    std::to_string(object_texture_retries.count()) +
                    " mesh_failures_tracked=" +
                    std::to_string(mesh_asset_retries.count()) +
                    " gpu_asset_estimated_bytes=" +
                    std::to_string(texture_gpu_used_bytes+
                        mesh_gpu_estimated_bytes));
            }
            world_renderer->set_point_lights(scene_point_lights);
            world_renderer->set_particle_emitters(scene_particle_emitters);
            world_renderer->set_avatar_proxies(avatar_proxy_update);
            if (simulation_ready)
            {
                const CameraState camera =
                    render_camera_from_agent(render_agent);
                world_renderer->set_camera({
                    {camera.center.x, camera.center.y, camera.center.z},
                    {camera.at.x, camera.at.y, camera.at.z},
                    {camera.left.x, camera.left.y, camera.left.z},
                    {camera.up.x, camera.up.y, camera.up.z},
                    60.0f,
                    0.1f,
                    camera.far_clip});
            }

            world_renderer->begin_frame();
            if (simulation_ready)
            {
                world_renderer->render_world(glfwGetTime());
            }
            world_renderer->end_frame();

            const render::RenderDiagnostics render_diagnostics =
                world_renderer->diagnostics();
            if(simulation_ready&&!self_avatar_proxy_gl_logged &&
               render_diagnostics.self_avatar_proxy_draw_calls>0U)
            {
                self_avatar_proxy_gl_logged=true;
                core::info(
                    "Avatar SELF placeholder geometry actually submitted to OpenGL; "
                    "this is NOT a baked Firestorm avatar appearance; draws="+
                    std::to_string(render_diagnostics.self_avatar_proxy_draw_calls));
            }
            if(simulation_ready&&!self_avatar_weighted_skin_gl_logged &&
               render_diagnostics.self_avatar_weighted_skin_draw_calls>=3U)
            {
                self_avatar_weighted_skin_gl_logged=true;
                core::info("Avatar SELF original Firestorm .llm joint-weights "
                    "submitted to GPU skin shader: 3 body meshes; "
                    "procedural local locomotion, server animations pending");
            }
            if(simulation_ready&&!self_avatar_original_mesh_gl_logged &&
               render_diagnostics.self_avatar_anatomical_mesh_draw_calls>=3U)
            {
                self_avatar_original_mesh_gl_logged=true;
                core::info(
                    "Avatar SELF Firestorm original .llm bind-pose MESH actually "
                    "drawn in GPU: 3 body meshes; server animation playback pending");
            }
            if(simulation_ready&&!self_avatar_bake_gl_logged &&
               render_diagnostics.self_avatar_baked_texture_draw_calls>0U)
            {
                self_avatar_bake_gl_logged=true;
                core::info(
                    "Avatar SELF Head/Upper/Lower baked TEXTURE submitted to OpenGL; "
                    "original UV/body when .llm available, server animation pending; draws="+
                    std::to_string(
                        render_diagnostics.self_avatar_baked_texture_draw_calls));
            }
            const core::SystemMemoryMetrics memory_metrics =
                core::system_memory_metrics();

            ogl_diagnostics_snapshot_v1 module_diagnostics{};
            module_diagnostics.struct_size = sizeof(module_diagnostics);
            module_diagnostics.version = OGL_DIAGNOSTICS_VERSION_1;
            module_diagnostics.frame_index = ++diagnostic_frame_index;
            module_diagnostics.fps = frame_metrics.fps;
            module_diagnostics.frame_time_ms = frame_metrics.frame_time_ms;
            module_diagnostics.draw_calls = render_diagnostics.draw_calls;
            module_diagnostics.triangles = render_diagnostics.triangles;
            module_diagnostics.visible_objects = render_diagnostics.visible_objects;
            module_diagnostics.ram_total_bytes = memory_metrics.total_bytes;
            module_diagnostics.ram_available_bytes = memory_metrics.available_bytes;
            module_diagnostics.vram_total_bytes = render_diagnostics.vram_total_bytes;
            module_diagnostics.vram_available_bytes = render_diagnostics.vram_available_bytes;
            module_diagnostics.simulation_ready = simulation_ready ? 1U : 0U;
            module_manager.update_diagnostics(module_diagnostics);
            (void)module_manager.tick();

            double mouse_x = 0.0;
            double mouse_y = 0.0;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);

            protocol::CircuitStatus circuit_snapshot;
            {
                std::scoped_lock lock{circuit_mutex};
                circuit_snapshot = circuit_status;
            }

            const SimulationLoadStats load_stats{
                terrain_packets.load(std::memory_order_relaxed),
                object_packets.load(std::memory_order_relaxed),
                avatar_packets.load(std::memory_order_relaxed),
                other_packets.load(std::memory_order_relaxed)};

            CapsRuntimeState caps_snapshot;
            {
                std::scoped_lock lock{caps_mutex};
                caps_snapshot = caps_state;
            }

            render_shell(
                renderer, window_width, window_height, mouse_x, mouse_y,
                grid_name, grid_status, grid_status_color, login_ui,
                active_session.has_value() ? &*active_session : nullptr,
                circuit_snapshot,
                load_stats,
                caps_snapshot,
                frame_metrics,
                render_diagnostics,
                memory_metrics,
                simulation_ready,
                welcome_texture,viewer_shell,
                inventory_tree_open,outfit_gallery_open,experience_ui.open);
            if(simulation_ready)
            {
                ui::ViewerShellView shell_view;
                shell_view.avatar_name=login_ui.username;
                {
                    std::scoped_lock lock{world_mutex};
                    const auto& region=world_model.region();
                    const auto& agent=world_model.agent();
                    shell_view.region_name=region.name;
                    shell_view.region_connected=region.connected;
                    shell_view.region_width=region.size_x;
                    shell_view.region_height=region.size_y;
                    shell_view.agent_position={
                        agent.position.x,agent.position.y,agent.position.z};
                    shell_view.avatar_present=agent.present;
                    shell_view.objects=world_model.objects().size();
                    shell_view.remote_avatars=world_model.avatars().size();
                    shell_view.terrain_patches=
                        world_model.terrain().valid_patch_count;
                    shell_view.terrain_ready=world_model.terrain().available;
                    shell_view.parcel_ready=world_model.parcel().available;
                    shell_view.appearance_ready=
                        world_model.self_appearance().has_value();
                    shell_view.animations=world_model.self_animations()
                        ?world_model.self_animations()->animations.size():0U;
                    shell_view.attachments=world_model.self_appearance()
                        ?world_model.self_appearance()->attachments.size():0U;
                    if(viewer_shell.window==ui::ViewerWindow::map)
                        for(const auto& [id,avatar]:world_model.avatars())
                        {
                            (void)id;
                            if(avatar.position_known)
                                shell_view.avatar_positions.push_back({
                                    avatar.position.x,avatar.position.y});
                        }
                }
                if(viewer_shell.window==ui::ViewerWindow::chat)
                {
                    std::scoped_lock lock{chat_history_mutex};
                    shell_view.chat_lines.assign(
                        chat_history.begin(),chat_history.end());
                }
                ui::render_viewer_shell(
                    renderer,window_width,window_height,
                    viewer_shell,shell_view);
                if(outfit_gallery_open &&active_session)
                    render_outfit_gallery(renderer,window_width,window_height,
                        protocol::outfits_from_login_skeleton(
                            active_session->inventory_skeleton),
                        caps_snapshot,gallery_gpu_textures,outfit_selection);
                if(inventory_tree_open)
                    render_inventory_tree(renderer,window_width,window_height,
                                          inventory_tree,
                                          inventory_folder_contents,
                                          inventory_parent,inventory_selection,
                                          inventory_item_selection,inventory_item_detail,
                                          inventory_item_detail_folder,
                                          inventory_item_detail_id,
                                          inventory_item_detail_status);
                render_experience_floater(renderer,window_width,window_height,
                    experience_ui,caps_snapshot,experience_consent);
            }

            glfwSwapBuffers(window);
        }

        renderer.destroy_texture(welcome_texture);
        for(const auto& [folder,image]:gallery_gpu_textures)
        {
            (void)folder;
            renderer.destroy_texture(image.second);
        }
        gallery_gpu_textures.clear();
        world_renderer->shutdown();
    }

    circuit_stop.store(true);
    caps_stop.store(true);

    if (circuit_future.valid())
    {
        circuit_future.wait();
    }
    if (caps_future.valid())
    {
        caps_future.wait();
    }
    for(auto& pending:region_event_queue_futures)
        pending.wait();

    glfwSetWindowUserPointer(window, nullptr);
    core::info("OpenGenesisLINK Viewer shutdown");
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
}
