/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/world/world_model.hpp"

#include <utility>

namespace nexverse::world
{
void WorldModel::reset()
{
    grid_ = {};
    region_ = {};
    agent_ = {};
    avatars_.clear();
    objects_.clear();
    terrain_ = {};
    parcel_ = {};
    environment_ = {};
}

void WorldModel::set_grid(GridState grid)
{
    grid_ = std::move(grid);
}

void WorldModel::apply_region(const RegionUpdate& update)
{
    region_.handle = update.handle;
    region_.name = update.name;
    region_.agent_position = update.agent_position;
    region_.connected = update.connected;
}

void WorldModel::apply_agent(const AgentUpdate& update)
{
    agent_.agent_id = update.agent_id;
    agent_.session_id = update.session_id;
    agent_.position = update.position;
    agent_.rotation = update.rotation;
    agent_.present = update.present;
}

void WorldModel::upsert_avatar(AvatarState avatar)
{
    if (avatar.id.empty())
    {
        return;
    }
    avatars_[avatar.id] = std::move(avatar);
}

void WorldModel::remove_avatar(const std::string& id)
{
    avatars_.erase(id);
}

void WorldModel::upsert_object(ObjectState object)
{
    if (object.local_id == 0U)
    {
        return;
    }
    objects_[object.local_id] = std::move(object);
}

void WorldModel::remove_object(std::uint32_t local_id)
{
    objects_.erase(local_id);
}

void WorldModel::set_terrain(TerrainState terrain)
{
    terrain_ = std::move(terrain);
}

void WorldModel::set_parcel(ParcelState parcel)
{
    parcel_ = std::move(parcel);
}

void WorldModel::set_environment(EnvironmentState environment)
{
    environment_ = std::move(environment);
}

const GridState& WorldModel::grid() const noexcept { return grid_; }
const RegionState& WorldModel::region() const noexcept { return region_; }
const AgentState& WorldModel::agent() const noexcept { return agent_; }
const TerrainState& WorldModel::terrain() const noexcept { return terrain_; }
const ParcelState& WorldModel::parcel() const noexcept { return parcel_; }
const EnvironmentState& WorldModel::environment() const noexcept { return environment_; }

const std::map<std::string, AvatarState, std::less<>>& WorldModel::avatars() const noexcept
{
    return avatars_;
}

const std::map<std::uint32_t, ObjectState>& WorldModel::objects() const noexcept
{
    return objects_;
}

bool WorldModel::ready() const noexcept
{
    return region_.connected &&
           region_.handle != 0U &&
           agent_.present &&
           !agent_.agent_id.empty() &&
           !agent_.session_id.empty() &&
           terrain_.available;
}
}
