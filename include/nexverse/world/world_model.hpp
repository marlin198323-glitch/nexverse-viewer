/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once

#include "nexverse/protocol/viewer_messages.hpp"

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

struct AvatarState
{
    std::string id;
    std::string name;
    Vec3 position;
    Quaternion rotation;
};

struct ObjectState
{
    std::uint32_t local_id = 0U;
    std::string id;
    Vec3 position;
    Quaternion rotation;
};

struct TerrainState
{
    std::uint32_t revision = 0U;
    bool available = false;
};

struct ParcelState
{
    std::uint32_t local_id = 0U;
    std::string name;
};

struct EnvironmentState
{
    bool available = false;
};

struct RegionUpdate
{
    std::uint64_t handle = 0U;
    std::string name;
    Vec3 agent_position;
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

class WorldModel
{
public:
    void reset();

    void set_grid(GridState grid);
    void apply_region(const RegionUpdate& update);
    void apply_agent(const AgentUpdate& update);
    void upsert_avatar(AvatarState avatar);
    void remove_avatar(const std::string& id);
    void upsert_object(ObjectState object);
    void remove_object(std::uint32_t local_id);
    void set_terrain(TerrainState terrain);
    void set_parcel(ParcelState parcel);
    void set_environment(EnvironmentState environment);

    [[nodiscard]] const GridState& grid() const noexcept;
    [[nodiscard]] const RegionState& region() const noexcept;
    [[nodiscard]] const AgentState& agent() const noexcept;
    [[nodiscard]] const TerrainState& terrain() const noexcept;
    [[nodiscard]] const ParcelState& parcel() const noexcept;
    [[nodiscard]] const EnvironmentState& environment() const noexcept;
    [[nodiscard]] const std::map<std::string, AvatarState, std::less<>>& avatars() const noexcept;
    [[nodiscard]] const std::map<std::uint32_t, ObjectState>& objects() const noexcept;

    [[nodiscard]] bool ready() const noexcept;

private:
    GridState grid_;
    RegionState region_;
    AgentState agent_;
    std::map<std::string, AvatarState, std::less<>> avatars_;
    std::map<std::uint32_t, ObjectState> objects_;
    TerrainState terrain_;
    ParcelState parcel_;
    EnvironmentState environment_;
};
}
