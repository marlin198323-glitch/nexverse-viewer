/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include <cmath>

#include <utility>

namespace nexverse::world
{
void WorldModel::reset()
{
    grid_ = {};
    region_ = {};
    agent_ = {};
    agent_profile_ = {};
    self_appearance_.reset();
    self_object_bakes_.reset();
    self_animations_.reset();
    pending_avatar_animations_.clear();
    avatars_.clear();
    objects_.clear();
    object_revision_ = 0U;
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
    // A region can be replaced without begin_region_transition during
    // direct model integration. Never carry self skin/animation/object
    // candidates across handles, even if session credentials remain.
    if((region_.handle!=0U && region_.handle!=update.handle) ||
       (region_.connected && !update.connected))
    {
        self_appearance_.reset();
        self_object_bakes_.reset();
        self_animations_.reset();
        pending_avatar_animations_.clear();
    }
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
    object_revision_ = 0U;
    terrain_ = {};
    parcel_ = {};
    environment_ = {};
    self_appearance_.reset();
    self_object_bakes_.reset();
    self_animations_.reset();
    pending_avatar_animations_.clear();

    apply_region(update);
    region_.generation = generation;

    agent_.position = update.agent_position;
    agent_.present = false;
}

void WorldModel::apply_agent(const AgentUpdate& update)
{
    // Both provisional and authoritative self appearances are tied to
    // the authenticated AGENT + SESSION, not merely the region handle.
    // Full ObjectUpdate calls apply_agent repeatedly with the same keys.
    if(agent_.agent_id!=update.agent_id ||
       agent_.session_id!=update.session_id)
    {
        self_object_bakes_.reset();
        self_appearance_.reset();
        self_animations_.reset();
        pending_avatar_animations_.clear();
        agent_profile_ = {};
    }
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

bool WorldModel::apply_self_appearance(
    protocol::AvatarAppearanceMessage appearance)
{
    // A newer simulator preview or an incomplete shape/TE packet may
    // arrive after the real self body: preserve the authoritative skin.
    // The same admission rule is used by the packet handler and model.
    if (!protocol::appearance_is_authoritative(
            protocol::assess_self_appearance(
                appearance,agent_.agent_id,region_.connected)) ||
        !protocol::valid_self_appearance_texture_entry(appearance.texture_entry))
        return false;
    self_appearance_ = std::move(appearance);
    // Remove any erroneously cached self entity, e.g. from a
    // pre-login appearance update; the self render path uses agent_.
    avatars_.erase(agent_.agent_id);
    return true;
}

bool WorldModel::observe_self_object_bakes(
    std::string_view avatar_id,std::uint64_t region_handle,
    std::uint32_t local_id,std::span<const std::uint8_t> texture_entry)
{
    if(!region_.connected||region_.handle!=region_handle||
       agent_.agent_id.empty()||agent_.session_id.empty()||
       avatar_id!=agent_.agent_id||local_id==0U)
        return false;
    // Rebound self local IDs must never reuse a previously bound
    // avatar body's texture even when that new packet omits the TE.
    if(self_object_bakes_&&self_object_bakes_->local_id!=local_id)
        self_object_bakes_.reset();
    // Empty, corrupt, inherited-default and placeholder assets cannot
    // create a visible skin, regardless of the avatar's world position.
    const auto bakes=protocol::extract_avatar_bakes(texture_entry).visible_body();
    if(bakes[0].empty()&&bakes[1].empty()&&bakes[2].empty())
        return false;
    // Store asset IDs solely inside the session-scoped model; F3/report
    // receives only counts. Modern Low158 overrides candidate rendering.
    if(self_object_bakes_&&self_object_bakes_->local_id==local_id&&
       self_object_bakes_->visible_body==bakes)
        return false; // repeated simulator state, not a new visible change
    self_object_bakes_=SelfAvatarObjectBakes{bakes,local_id};
    return true;
}

const std::optional<SelfAvatarObjectBakes>&
WorldModel::self_object_bakes() const noexcept
{
    return self_object_bakes_;
}

std::array<std::string,3U> WorldModel::self_render_bakes() const
{
    if(self_appearance_)
        return protocol::extract_avatar_bakes(
            self_appearance_->texture_entry).visible_body();
    if(self_object_bakes_)return self_object_bakes_->visible_body;
    return {};
}

bool WorldModel::apply_avatar_animation(
    protocol::AvatarAnimationMessage animation)
{
    if(!region_.connected||animation.sender.empty()||
       agent_.agent_id.empty())return false;
    if(animation.sender==agent_.agent_id)
    {
        self_animations_=std::move(animation);
        return true;
    }
    const auto existing=avatars_.find(animation.sender);
    if(existing!=avatars_.end())
    {
        existing->second.animations=std::move(animation);
        return true;
    }
    // Firestorm servers can send High20 before an ObjectUpdate. Keep a
    // small bounded pending snapshot without inventing world geometry.
    if(!pending_avatar_animations_.contains(animation.sender)&&
       pending_avatar_animations_.size()>=64U)
        return false;
    pending_avatar_animations_.insert_or_assign(
        animation.sender,std::move(animation));
    return true;
}

const std::optional<protocol::AvatarAnimationMessage>&
WorldModel::self_animations() const noexcept
{
    return self_animations_;
}

void WorldModel::upsert_avatar(AvatarState avatar)
{
    if (avatar.id.empty() ||
        (!agent_.agent_id.empty() && avatar.id == agent_.agent_id))
    {
        return;
    }
    if(!avatar.animations)
    {
        const auto pending=pending_avatar_animations_.find(avatar.id);
        if(pending!=pending_avatar_animations_.end())
        {
            avatar.animations=std::move(pending->second);
            pending_avatar_animations_.erase(pending);
        }
    }
    avatars_[avatar.id] = std::move(avatar);
}

void WorldModel::update_avatar_transform(
    const std::string& id,Vec3 position,
    Quaternion rotation,std::uint32_t local_id)
{
    if(id.empty() ||
       (!agent_.agent_id.empty() && id == agent_.agent_id))
        return;
    auto& avatar=avatars_[id];
    if(!avatar.animations)
    {
        const auto pending=pending_avatar_animations_.find(id);
        if(pending!=pending_avatar_animations_.end())
        {
            avatar.animations=std::move(pending->second);
            pending_avatar_animations_.erase(pending);
        }
    }
    if(avatar.position_known)
    {
        const float dx=position.x-avatar.position.x;
        const float dy=position.y-avatar.position.y;
        const float dz=position.z-avatar.position.z;
        const float delta=dx*dx+dy*dy+dz*dz;
        // Ignore network quantization and teleport-sized discontinuities.
        if(std::isfinite(delta)&&delta>0.0025f&&delta<9.0f)
            avatar.last_translation_at=std::chrono::steady_clock::now();
    }
    avatar.id=id;
    avatar.position=position;
    avatar.rotation=rotation;
    if(local_id!=0U)avatar.local_id=local_id;
    avatar.position_known=true;
}

void WorldModel::remove_avatar(const std::string& id)
{
    avatars_.erase(id);
    pending_avatar_animations_.erase(id);
}

void WorldModel::upsert_object(ObjectState object)
{
    if (object.local_id == 0U)
    {
        return;
    }
    objects_[object.local_id] = std::move(object);
    ++object_revision_;
}

void WorldModel::remove_object(std::uint32_t local_id)
{
    if (objects_.erase(local_id) > 0U)
    {
        ++object_revision_;
    }
}

void WorldModel::set_terrain(TerrainState terrain)
{
    terrain_ = std::move(terrain);
}

void WorldModel::set_terrain_materials(TerrainMaterials materials)
{
    const std::uint32_t next_revision =
        terrain_.materials.revision + 1U;

    materials.revision = next_revision;

    bool has_material = false;
    for (const auto& id : materials.detail_ids)
    {
        if (!id.empty() &&
            id != "00000000-0000-0000-0000-000000000000")
        {
            has_material = true;
            break;
        }
    }

    materials.available = has_material;
    terrain_.materials = std::move(materials);
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
const std::optional<protocol::AvatarAppearanceMessage>&
WorldModel::self_appearance() const noexcept { return self_appearance_; }
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

std::uint32_t WorldModel::object_revision() const noexcept
{
    return object_revision_;
}

WorldSnapshot WorldModel::snapshot() const
{
    WorldSnapshot value;
    value.grid = grid_;
    value.region = region_;
    value.agent = agent_;
    value.agent_profile = agent_profile_;
    value.self_appearance = self_appearance_;
    value.self_object_bakes = self_object_bakes_;
    value.self_animations = self_animations_;
    value.avatars = avatars_;
    value.objects = objects_;
    value.terrain = terrain_;
    value.parcel = parcel_;
    value.environment = environment_;
    value.ready = ready();
    return value;
}

bool WorldModel::scene_ready() const noexcept
{
    return region_.connected &&
           region_.handle != 0U &&
           agent_.present &&
           !agent_.agent_id.empty() &&
           !agent_.session_id.empty();
}

bool WorldModel::ready() const noexcept
{
    return scene_ready() && terrain_.available;
}
}
