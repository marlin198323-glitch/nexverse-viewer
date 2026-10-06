/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/terrain_codec.hpp"
#include "nexverse/protocol/viewer_messages.hpp"

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>

namespace nexverse::world
{
using nexverse::protocol::Quaternion;
using nexverse::protocol::Vec3;

struct GridState
{
    std::string name;
    std::string login_uri;
};

struct RegionState
{
    std::uint64_t handle = 0U;
    std::string name;
    Vec3 agent_position;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0U;
    std::string seed_capability;
    std::string region_id;
    std::uint32_t region_flags = 0U;
    std::uint64_t region_flags_extended = 0U;
    std::uint64_t region_protocols = 0U;
    std::uint8_t sim_access = 0U;
    float water_height = 0.0f;
    std::string channel_version;
    std::uint32_t size_x = 256U;
    std::uint32_t size_y = 256U;
    std::uint32_t generation = 0U;
    bool connected = false;
};

struct AgentState
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Quaternion rotation;
    bool present = false;
};

struct AgentProfileState
{
    std::string first_name;
    std::string last_name;
    std::string group_title;
    std::string active_group_id;
    std::uint64_t group_powers = 0U;
    std::string group_name;
};

struct AvatarState
{
    std::string id;
    std::string name;
    Vec3 position;
    Quaternion rotation;
    std::uint32_t local_id = 0U;
    bool trial_appearance = false;
    std::vector<std::uint8_t> texture_entry;
    std::vector<std::uint8_t> visual_params;
    std::vector<protocol::AvatarAppearanceData> appearance_data;
};

struct ObjectState
{
    std::uint32_t local_id = 0U;
    std::string id;
    Vec3 position;
    Quaternion rotation;
};

struct TerrainMaterials
{
    std::uint32_t revision = 0U;
    bool available = false;
    std::array<std::string, 4> base_ids;
    std::array<std::string, 4> detail_ids;
    std::array<float, 4> start_heights{};
    std::array<float, 4> height_ranges{};
};

struct TerrainState
{
    std::uint32_t revision = 0U;
    bool available = false;
    std::array<float, 256U * 256U> heights{};
    std::array<bool, 16U * 16U> valid_patches{};
    std::uint16_t valid_patch_count = 0U;
    TerrainMaterials materials;
};

struct ParcelState
{
    std::uint32_t local_id = 0U;
    std::string name;
    std::string description;
    std::string owner_id;
    std::int32_t area = 0;
    std::uint32_t parcel_flags = 0U;
    std::int32_t sale_price = 0;
    std::string music_url;
    std::string media_url;
    bool available = false;
};

struct EnvironmentState
{
    std::uint32_t revision = 0U;
    bool available = false;
};

struct RegionUpdate
{
    std::uint64_t handle = 0U;
    std::string name;
    Vec3 agent_position;
    std::string simulator_ip;
    std::uint16_t simulator_port = 0U;
    std::string seed_capability;
    std::string region_id;
    std::uint32_t region_flags = 0U;
    std::uint64_t region_flags_extended = 0U;
    std::uint64_t region_protocols = 0U;
    std::uint8_t sim_access = 0U;
    float water_height = 0.0f;
    std::string channel_version;
    std::uint32_t size_x = 256U;
    std::uint32_t size_y = 256U;
    bool connected = false;
};

struct AgentUpdate
{
    std::string agent_id;
    std::string session_id;
    Vec3 position;
    Quaternion rotation;
    bool present = false;
};

struct WorldSnapshot
{
    GridState grid;
    RegionState region;
    AgentState agent;
    AgentProfileState agent_profile;
    std::map<std::string, AvatarState, std::less<>> avatars;
    std::map<std::uint32_t, ObjectState> objects;
    TerrainState terrain;
    ParcelState parcel;
    EnvironmentState environment;
    bool ready = false;
};

class WorldModel
{
public:
    void reset();

    void set_grid(GridState grid);
    void apply_region(const RegionUpdate& update);
    void begin_region_transition(const RegionUpdate& update);
    void apply_agent(const AgentUpdate& update);
    void set_agent_profile(AgentProfileState profile);
    void upsert_avatar(AvatarState avatar);
    void remove_avatar(const std::string& id);
    void upsert_object(ObjectState object);
    void remove_object(std::uint32_t local_id);
    void set_terrain(TerrainState terrain);
    void set_terrain_materials(TerrainMaterials materials);
    void apply_terrain_patch(const protocol::TerrainPatch& patch);
    void set_parcel(ParcelState parcel);
    void set_environment(EnvironmentState environment);

    [[nodiscard]] const GridState& grid() const noexcept;
    [[nodiscard]] const RegionState& region() const noexcept;
    [[nodiscard]] const AgentState& agent() const noexcept;
    [[nodiscard]] const AgentProfileState& agent_profile() const noexcept;
    [[nodiscard]] const TerrainState& terrain() const noexcept;
    [[nodiscard]] const ParcelState& parcel() const noexcept;
    [[nodiscard]] const EnvironmentState& environment() const noexcept;
    [[nodiscard]] const std::map<std::string, AvatarState, std::less<>>& avatars() const noexcept;
    [[nodiscard]] const std::map<std::uint32_t, ObjectState>& objects() const noexcept;
    [[nodiscard]] WorldSnapshot snapshot() const;

    [[nodiscard]] bool ready() const noexcept;

private:
    GridState grid_;
    RegionState region_;
    AgentState agent_;
    AgentProfileState agent_profile_;
    std::map<std::string, AvatarState, std::less<>> avatars_;
    std::map<std::uint32_t, ObjectState> objects_;
    TerrainState terrain_;
    ParcelState parcel_;
    EnvironmentState environment_;
};
}
