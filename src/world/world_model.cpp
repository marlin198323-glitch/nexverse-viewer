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
    agent_profile_ = {};
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
    region_.simulator_ip = update.simulator_ip;
    region_.simulator_port = update.simulator_port;
    region_.seed_capability = update.seed_capability;
    region_.region_id = update.region_id;
    region_.region_flags = update.region_flags;
    region_.region_flags_extended = update.region_flags_extended;
    region_.region_protocols = update.region_protocols;
    region_.sim_access = update.sim_access;
    region_.water_height = update.water_height;
    region_.channel_version = update.channel_version;
    region_.size_x = update.size_x;
    region_.size_y = update.size_y;
    region_.connected = update.connected;
}

void WorldModel::begin_region_transition(const RegionUpdate& update)
{
    const std::uint32_t generation = region_.generation + 1U;

    avatars_.clear();
    objects_.clear();
    terrain_ = {};
    parcel_ = {};
    environment_ = {};

    apply_region(update);
    region_.generation = generation;

    agent_.position = update.agent_position;
    agent_.present = false;
}

void WorldModel::apply_agent(const AgentUpdate& update)
{
    agent_.agent_id = update.agent_id;
    agent_.session_id = update.session_id;
    agent_.position = update.position;
    agent_.rotation = update.rotation;
    agent_.present = update.present;
}

void WorldModel::set_agent_profile(AgentProfileState profile)
{
    agent_profile_ = std::move(profile);
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

void WorldModel::apply_terrain_patch(const protocol::TerrainPatch& patch)
{
    if (patch.x >= 16U || patch.y >= 16U)
    {
        return;
    }

    const std::size_t patch_slot =
        static_cast<std::size_t>(patch.y) * 16U +
        static_cast<std::size_t>(patch.x);

    if (!terrain_.valid_patches[patch_slot])
    {
        terrain_.valid_patches[patch_slot] = true;
        ++terrain_.valid_patch_count;
    }

    const std::size_t base_x = static_cast<std::size_t>(patch.x) * 16U;
    const std::size_t base_y = static_cast<std::size_t>(patch.y) * 16U;

    for (std::size_t local_y = 0U; local_y < 16U; ++local_y)
    {
        for (std::size_t local_x = 0U; local_x < 16U; ++local_x)
        {
            const std::size_t world_x = base_x + local_x;
            const std::size_t world_y = base_y + local_y;
            terrain_.heights[world_y * 256U + world_x] =
                patch.heights[local_y * 16U + local_x];
        }
    }

    ++terrain_.revision;
    terrain_.available = terrain_.valid_patch_count > 0U;
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
const AgentProfileState& WorldModel::agent_profile() const noexcept { return agent_profile_; }
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
