/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/self_appearance_admission.hpp"
#include "nexverse/protocol/avatar_cof_revision.hpp"
#include "nexverse/protocol/wearables_serial_order.hpp"
#include "nexverse/world/simulator_scene_validity.hpp"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
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
    early_self_appearance_.reset();
    early_self_wearables_.reset();
    self_pose_confirmed_=false;
    stale_self_cof_appearances_=0U;
    self_cof_version_highwater_=0U;
    self_cof_format_highwater_=0U;
    self_cof_by_format_highwater_.fill(0U);
    self_wearables_.reset();
    self_object_bakes_.reset();
    self_object_bake_withdrawals_=0U;
    self_object_bake_outfit_invalidations_=0U;
    self_object_kills_=0U;
    self_animations_.reset();
    pending_avatar_animations_.clear();
    avatars_.clear();
    objects_.clear();
    prim_uuid_to_local_id_.clear();
    object_revision_ = 0U;
    remote_avatar_kills_=0U;
    removed_prim_kills_=0U;
    remote_avatar_local_id_rebinds_=0U;
    prim_local_id_rebinds_=0U;
    prim_uuid_relocations_=0U;
    invalid_scene_updates_=0U;
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
    // apply_region() also runs on RegionHandshake, AgentMovementComplete
    // and circuit connect/disconnect. A *same connected region* refresh
    // MUST preserve the scene. An actual handle change or loss of circuit
    // readiness, however, invalidates ALL regional prims and avatars;
    // clearing only our self appearance left ghost meshes, attachments,
    // old terrain and cached particles visible after the disconnect.
    // A simulator restart or reconnect can reuse the same RegionHandle
    // while moving the UDP circuit to a different IP/port. RegionHandle
    // equality alone cannot authorize retaining old local IDs, body pose,
    // baked outfit or prim geometry. Treat an actual known socket change
    // as a region-epoch transition, but not incomplete handshakes where
    // the socket is still unknown.
    // Some intermediate RegionHandshake/AgentMovementComplete updates
    // do not include both IP and port. While refreshing the SAME region,
    // preserve the last concrete circuit socket: otherwise a blank
    // handshake could erase the identity and let a later different
    // real socket inherit the previous authenticated avatar/scene.
    const bool same_handle=region_.handle!=0U&&
        region_.handle==update.handle;
    const std::string effective_ip=
        same_handle&&update.simulator_ip.empty()
        ?region_.simulator_ip:update.simulator_ip;
    const std::uint16_t effective_port=
        same_handle&&update.simulator_port==0U
        ?region_.simulator_port:update.simulator_port;
    const bool simulator_socket_changed=
        same_handle &&
        !region_.simulator_ip.empty() && !effective_ip.empty() &&
        region_.simulator_port!=0U && effective_port!=0U &&
        (region_.simulator_ip!=effective_ip ||
         region_.simulator_port!=effective_port);
    const bool leave_region =
        (region_.handle!=0U && region_.handle!=update.handle) ||
        (region_.connected && !update.connected) ||
        simulator_socket_changed;
    if(leave_region)
    {
        avatars_.clear();
        objects_.clear();
        prim_uuid_to_local_id_.clear();
        ++object_revision_; // force current scene to become empty
        remote_avatar_kills_=0U;
        removed_prim_kills_=0U;
        remote_avatar_local_id_rebinds_=0U;
        prim_local_id_rebinds_=0U;
        prim_uuid_relocations_=0U;
        invalid_scene_updates_=0U;
        terrain_={};
        parcel_={};
        environment_={};
        agent_.present=false;
        if(region_.handle!=0U)
            ++region_.generation; // direct transitions invalidate lease
        self_appearance_.reset();
    early_self_appearance_.reset();
    early_self_wearables_.reset();
        self_pose_confirmed_=false;
    stale_self_cof_appearances_=0U;
    self_cof_version_highwater_=0U;
    self_cof_format_highwater_=0U;
    self_cof_by_format_highwater_.fill(0U);
        self_wearables_.reset();
        self_object_bakes_.reset();
        self_object_bake_withdrawals_=0U;
    self_object_bake_outfit_invalidations_=0U;
        self_object_kills_=0U;
        self_animations_.reset();
        pending_avatar_animations_.clear();
    }
    region_.handle = update.handle;
    region_.name = update.name;
    region_.agent_position = update.agent_position;
    region_.simulator_ip = effective_ip;
    region_.simulator_port = effective_port;
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
    prim_uuid_to_local_id_.clear();
    object_revision_ = 0U;
    remote_avatar_kills_=0U;
    removed_prim_kills_=0U;
    remote_avatar_local_id_rebinds_=0U;
    prim_local_id_rebinds_=0U;
    prim_uuid_relocations_=0U;
    invalid_scene_updates_=0U;
    terrain_ = {};
    parcel_ = {};
    environment_ = {};
    self_appearance_.reset();
    early_self_appearance_.reset();
    early_self_wearables_.reset();
    self_pose_confirmed_=false;
    stale_self_cof_appearances_=0U;
    self_cof_version_highwater_=0U;
    self_cof_format_highwater_=0U;
    self_cof_by_format_highwater_.fill(0U);
    self_wearables_.reset();
    self_object_bakes_.reset();
    self_object_bake_withdrawals_=0U;
    self_object_bake_outfit_invalidations_=0U;
    self_object_kills_=0U;
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
        // This object can be reused after re-authentication without
        // WorldModel::reset(). Region LocalIDs, appearance/COF, prim mesh
        // UUIDs and remote avatars are scoped to the *prior* login.
        // The first assignment from a fresh model is harmless.
        const bool replacing_identity=
            !agent_.agent_id.empty() || !agent_.session_id.empty();
        if(replacing_identity)
        {
            avatars_.clear();
            objects_.clear();
            prim_uuid_to_local_id_.clear();
            ++object_revision_;
            remote_avatar_kills_=0U;
            removed_prim_kills_=0U;
            remote_avatar_local_id_rebinds_=0U;
            prim_local_id_rebinds_=0U;
            prim_uuid_relocations_=0U;
            invalid_scene_updates_=0U;
            terrain_={};
            parcel_={};
            environment_={};
            ++region_.generation;
        }
        self_object_bakes_.reset();
        self_object_bake_withdrawals_=0U;
    self_object_bake_outfit_invalidations_=0U;
        self_object_kills_=0U;
        self_appearance_.reset();
    early_self_appearance_.reset();
    early_self_wearables_.reset();
        self_pose_confirmed_=false;
    stale_self_cof_appearances_=0U;
    self_cof_version_highwater_=0U;
    self_cof_format_highwater_=0U;
    self_cof_by_format_highwater_.fill(0U);
        self_wearables_.reset();
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

bool WorldModel::accept_simulator_self_position(
    std::string_view avatar_id,std::string_view session_id,
    std::uint64_t region_handle,Vec3 position)
{
    if(avatar_id.empty()||session_id.empty()||region_handle==0U||
       avatar_id!=agent_.agent_id||session_id!=agent_.session_id||
       region_handle!=region_.handle||
       !finite_simulator_position(position))
        return false;
    // Preserve an already independently verified BodyRotation if Full
    // or Terse arrived first. But on initial login this position-only
    // packet MUST NOT set self_pose_confirmed_ or release High4 controls.
    apply_agent({agent_.agent_id,agent_.session_id,position,
                 agent_.rotation,agent_.present});
    return true;
}

bool WorldModel::accept_simulator_self_pose(
    std::string_view avatar_id,std::string_view session_id,
    std::uint64_t region_handle,Vec3 position,Quaternion rotation)
{
    // Only a decoded, authenticated AgentMovementComplete or real self
    // ObjectUpdate/Compressed/Terse may confirm inworld body geometry.
    // A connected circuit or an unsent client WASD update never can.
    if(avatar_id.empty()||session_id.empty()||region_handle==0U||
       avatar_id!=agent_.agent_id||session_id!=agent_.session_id||
       region_handle!=region_.handle||
       !valid_simulator_avatar_pose(position,rotation))
        return false;
    apply_agent({agent_.agent_id,agent_.session_id,position,rotation,
                 agent_.present});
    self_pose_confirmed_=true;
    return true;
}

void WorldModel::set_agent_profile(AgentProfileState profile)
{
    agent_profile_ = std::move(profile);
}

bool WorldModel::stage_early_self_appearance(
    protocol::AvatarAppearanceMessage appearance)
{
    if(region_.connected||region_.handle==0U||
       agent_.agent_id.empty()||agent_.session_id.empty()||
       !protocol::appearance_is_authoritative(
           protocol::assess_self_appearance(
               appearance,agent_.agent_id,true))||
       !protocol::valid_self_appearance_texture_entry(
           appearance.texture_entry))
        return false;
    // Ignore older retransmits with the same appearance format. A
    // fast, valid reply must never regress the staged outfit before the
    // main thread has confirmed circuit readiness.
    if(early_self_appearance_)
    {
        const auto old=protocol::latest_nonzero_avatar_cof(
            early_self_appearance_->packet);
        const auto incoming=protocol::latest_nonzero_avatar_cof(appearance);
        if(old&&(!incoming||
                 (incoming->appearance_version==old->appearance_version&&
                  incoming->cof_version<old->cof_version)))
            return false;
    }
    early_self_appearance_=EarlySelfPacket<protocol::AvatarAppearanceMessage>{
        std::move(appearance),
        {agent_.agent_id,agent_.session_id,region_.handle,region_.generation,
         region_.simulator_ip,region_.simulator_port}};
    return true;
}

bool WorldModel::stage_early_self_wearables(
    protocol::AgentWearablesUpdateMessage update)
{
    if(region_.connected||region_.handle==0U||
       agent_.agent_id.empty()||agent_.session_id.empty()||
       update.agent_id!=agent_.agent_id||
       update.session_id!=agent_.session_id)
        return false;
    if(early_self_wearables_&&
       !protocol::wearables_serial_newer(
           update.serial,early_self_wearables_->packet.serial))
        return false;
    early_self_wearables_=EarlySelfPacket<protocol::AgentWearablesUpdateMessage>{
        std::move(update),
        {agent_.agent_id,agent_.session_id,region_.handle,region_.generation,
         region_.simulator_ip,region_.simulator_port}};
    return true;
}

WorldModel::EarlySelfReadbackResult
WorldModel::flush_early_self_readbacks()
{
    EarlySelfReadbackResult result{};
    if(!region_.connected||agent_.agent_id.empty()||
       agent_.session_id.empty())return result;
    const auto valid=[this](const EarlySelfSource& source)
    {
        return source.agent_id==agent_.agent_id&&
            source.session_id==agent_.session_id&&
            source.region_handle==region_.handle&&
            source.region_generation==region_.generation&&
            source.simulator_ip==region_.simulator_ip&&
            source.simulator_port==region_.simulator_port;
    };
    if(early_self_appearance_)
    {
        auto staged=std::move(*early_self_appearance_);
        early_self_appearance_.reset();
        // The live packet handler may have already accepted a NEWER
        // Low158 between circuit-ready and this UI-thread flush.
        // The staged packet cannot be ordered against a legacy unversioned
        // live reply; retain the live accepted skin regardless of format.
        if(valid(staged.source)&&!self_appearance_)
            result.appearance=apply_self_appearance(std::move(staged.packet));
    }
    if(early_self_wearables_)
    {
        auto staged=std::move(*early_self_wearables_);
        early_self_wearables_.reset();
        if(valid(staged.source))
            result.wearables=apply_self_wearables(std::move(staged.packet));
    }
    return result;
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
    // Genuine modern COF revision changes identify a different outfit.
    // The earlier self ObjectUpdate TE belongs to the old outfit and
    // cannot safely fill missing Head/Upper/Lower slots in NEW Low158.
    // Do not clear on the first Low158, unversioned legacy messages,
    // repeated same revision or packets rejected by the rollback gate.
    const auto previous_outfit=self_appearance_
        ?protocol::latest_nonzero_avatar_cof(*self_appearance_)
        :std::optional<protocol::AvatarAppearanceData>{};
    const auto incoming_outfit=
        protocol::latest_nonzero_avatar_cof(appearance);
    // A legacy count=0 Low158 may have arrived after an authoritative
    // modern outfit. Preserve the last accepted meaningful COF via the
    // existing watermark, otherwise a subsequent new modern outfit
    // could reuse object bakes from a much older wardrobe.
    const bool prior_outfit_known=previous_outfit.has_value()||
        self_cof_version_highwater_!=0U;
    const auto prior_outfit_version=previous_outfit
        ?previous_outfit->cof_version:self_cof_version_highwater_;
    const auto prior_outfit_format=previous_outfit
        ?previous_outfit->appearance_version:self_cof_format_highwater_;
    // Modern Firestorm Low158 can carry an ordered COF version.
    // Out-of-order UDP retransmission of a PREVIOUS outfit must not
    // restore obsolete body/skin. Real OpenGenesisLINK sends the legacy
    // AppearanceData count=0; accept such unknown-version packets.
    if(!appearance.appearance_data.empty())
    {
        // Firestorm Low158 permits multiple AppearanceData rows. A
        // trailing version=0 row is NOT evidence that an earlier
        // versioned entry should bypass our existing COF rollback gate.
        // Use the last *meaningful* row, preserving all legacy count=0
        // and version=0 compatibility.
        const auto revision=
            protocol::latest_nonzero_avatar_cof(appearance);
        if(revision)
        {
            const auto& incoming=*revision;
            if(incoming.cof_version!=0U)
            {
            if((self_cof_version_highwater_!=0U&&
                self_cof_format_highwater_==incoming.appearance_version&&
                incoming.cof_version<self_cof_version_highwater_)||
               (self_cof_by_format_highwater_[incoming.appearance_version]!=0U&&
                incoming.cof_version<
                    self_cof_by_format_highwater_[incoming.appearance_version]))
            {
                if(stale_self_cof_appearances_<UINT32_MAX)
                    ++stale_self_cof_appearances_;
                return false;
            }
            // A legacy count=0 snapshot must not reset this watermark.
            // COF integers are comparable only within the same
            // AppearanceVersion; different versions start a new series.
            self_cof_version_highwater_=incoming.cof_version;
            self_cof_format_highwater_=incoming.appearance_version;
            auto& per_format=
                self_cof_by_format_highwater_[incoming.appearance_version];
            per_format=std::max(per_format,incoming.cof_version);
            }
        }
    }
    if(prior_outfit_known&&incoming_outfit&&
       (prior_outfit_format!=incoming_outfit->appearance_version||
        prior_outfit_version!=incoming_outfit->cof_version))
    {
        if(self_object_bakes_)
        {
            self_object_bakes_.reset();
            if(self_object_bake_outfit_invalidations_<UINT32_MAX)
                ++self_object_bake_outfit_invalidations_;
        }
    }
    self_appearance_ = std::move(appearance);
    // Remove any erroneously cached self entity, e.g. from a
    // pre-login appearance update; the self render path uses agent_.
    avatars_.erase(agent_.agent_id);
    return true;
}

bool WorldModel::apply_self_wearables(
    protocol::AgentWearablesUpdateMessage update)
{
    if(!region_.connected||agent_.agent_id.empty()||
       agent_.session_id.empty()||
       update.agent_id!=agent_.agent_id||
       update.session_id!=agent_.session_id)return false;
    // Low382 SerialNum is a 32-bit cyclic sequence. A raw <= would
    // reject every legitimate readback after 0xffffffff wraps to zero.
    // Modular half-cycle admission also rejects delayed pre-wrap UDP
    // retransmits. Agent and session ownership were checked above.
    if(self_wearables_&&!protocol::wearables_serial_newer(
           update.serial,self_wearables_->serial))
        return false;
    self_wearables_=std::move(update);
    return true;
}

const std::optional<protocol::AgentWearablesUpdateMessage>&
WorldModel::self_wearables() const noexcept
{
    return self_wearables_;
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
    // Malformed / absent TextureEntry must not clobber last valid skin:
    // the compressed avatar header often has no TE at all.
    if(!protocol::parse_texture_entry(texture_entry))
        return false;
    // A *valid* full TextureEntry with only absent or placeholder baked
    // faces is different: the current simulator has explicitly withdrawn
    // those provisional references (e.g. after an outfit rebake).
    // Do not carry a stale previous skin forward under the same LocalID.
    const auto bakes=protocol::extract_avatar_bakes(texture_entry).visible_body();
    if(bakes[0].empty()&&bakes[1].empty()&&bakes[2].empty())
    {
        if(self_object_bakes_&&self_object_bakes_->local_id==local_id)
        {
            self_object_bakes_.reset();
            ++self_object_bake_withdrawals_;
            return true; // real visual-source change, no invented new bake
        }
        return false;
    }
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
std::uint32_t WorldModel::self_object_bake_withdrawals() const noexcept
{
    return self_object_bake_withdrawals_;
}
std::uint32_t WorldModel::self_object_bake_outfit_invalidations()
    const noexcept
{
    return self_object_bake_outfit_invalidations_;
}
bool WorldModel::withdraw_self_object_on_kill(
    std::string_view avatar_id,std::uint64_t region_handle,
    std::uint32_t local_id)
{
    if(!region_.connected||region_.handle!=region_handle||
       agent_.agent_id.empty()||agent_.session_id.empty()||
       avatar_id!=agent_.agent_id||local_id==0U)
        return false;
    // The dispatcher additionally authenticates this LocalID against
    // its own current ObjectUpdate binding before calling. A duplicate
    // KillObject is not counted once that binding was cleared.
    if(self_object_bakes_&&self_object_bakes_->local_id==local_id)
        self_object_bakes_.reset();
    self_pose_confirmed_=false; // killed body cannot remain confirmed
    if(self_object_kills_<UINT32_MAX)++self_object_kills_;
    return true;
}

std::uint32_t WorldModel::self_object_kills() const noexcept
{
    return self_object_kills_;
}

SelfRenderBakeSelection WorldModel::self_bake_selection() const
{
    SelfRenderBakeSelection out;
    if(!region_.connected||agent_.agent_id.empty()||
       agent_.session_id.empty())return out;
    if(self_appearance_)
    {
        out.ids=protocol::extract_avatar_bakes(
            self_appearance_->texture_entry).visible_body();
        for(std::size_t i=0U;i<out.ids.size();++i)
            out.from_low158[i]=!out.ids[i].empty();
    }
    if(self_object_bakes_)
        for(std::size_t i=0U;i<out.ids.size();++i)
            if(out.ids[i].empty()&&!self_object_bakes_->visible_body[i].empty())
            {
                out.ids[i]=self_object_bakes_->visible_body[i];
                out.from_object_update[i]=true;
            }
    return out;
}

std::array<std::string,3U> WorldModel::self_render_bakes() const
{
    return self_bake_selection().ids;
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
    Quaternion rotation,std::uint32_t local_id,
    bool from_avatar_object_update)
{
    if(id.empty() ||
       (!agent_.agent_id.empty() && id == agent_.agent_id))
        return;
    // A decoded but identity-less LocalID=0 (or a stale packet from a
    // disconnected region) cannot authorize a visible 3D avatar.
    if(from_avatar_object_update&&(!region_.connected||local_id==0U))
        return;
    // No malformed network-sourced pose can evict the previous avatar
    // and its GPU-resident worn mesh tree.
    if(!finite_simulator_position(position)||
       !finite_simulator_rotation(rotation)||
       (from_avatar_object_update&&
        !valid_simulator_avatar_pose(position,rotation)))
    {
        if(invalid_scene_updates_<UINT32_MAX)++invalid_scene_updates_;
        return;
    }
    if(from_avatar_object_update&&region_.connected&&local_id!=0U)
    {
        // ObjectUpdate establishes this simulator LocalID's current UUID.
        // UDP KillObject is not guaranteed to arrive before LocalID reuse.
        // Retire a displaced avatar and its attached mesh descendants.
        std::vector<std::uint32_t> retired;
        for(const auto& [other_id,old]:avatars_)
            if(other_id!=id&&old.local_id==local_id&&
               old.simulator_object_update_seen)
                retired.push_back(old.local_id);
        // If a known avatar moves to a new LocalID, preserve its actual
        // Low158 appearance/animations but retire attachments under the
        // old parent LocalID; those prims cannot belong to the new ID.
        auto previous=avatars_.find(id);
        std::optional<AvatarState> saved;
        if(previous!=avatars_.end()&&
           previous->second.simulator_object_update_seen&&
           previous->second.local_id!=0U&&
           previous->second.local_id!=local_id)
        {
            retired.push_back(previous->second.local_id);
            saved=previous->second;
        }
        for(const auto retired_id:retired)
        {
            // This is NOT a received KillObject, so do not fabricate
            // network packet or removal counters for actual KillObject.
            if(kill_simulator_local_id(retired_id,false)&&
               remote_avatar_local_id_rebinds_<UINT32_MAX)
                ++remote_avatar_local_id_rebinds_;
        }
        if(saved)
        {
            saved->local_id=0U;
            avatars_.insert_or_assign(id,std::move(*saved));
        }
    }
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
    if(from_avatar_object_update)
    {
        avatar.simulator_object_update_seen=true;
        avatar.coarse_position_only=false;
    }
}

bool WorldModel::apply_coarse_avatar_locations(
    const protocol::CoarseLocationUpdate& update)
{
    if(!region_.connected||agent_.agent_id.empty())
        return false;
    // CoarseLocationUpdate contains byte-quantized positions intended
    // for the minimap. It is not a complete ObjectUpdate and is not
    // guaranteed to list every detailed avatar in the region.
    std::unordered_set<std::string> seen;
    seen.reserve(std::min(update.avatars.size(),std::size_t{128}));
    for(const auto& entry:update.avatars)
    {
        const auto& id=entry.agent_id;
        if(id.empty()||
           id=="00000000-0000-0000-0000-000000000000"||
           id==agent_.agent_id||
           !std::isfinite(entry.position.x)||
           !std::isfinite(entry.position.y)||
           !std::isfinite(entry.position.z))
            continue;
        if(seen.size()>=128U)break;
        if(!seen.insert(id).second)continue;
        const auto existing=avatars_.find(id);
        // Never replace a full avatar's accurate transform/rig by the
        // coarse integer position; only maintain coarse-only map dots.
        if(existing!=avatars_.end()&&
           existing->second.simulator_object_update_seen)
            continue;
        const bool is_new=existing==avatars_.end();
        const Quaternion rotation= is_new?Quaternion{}:
            existing->second.rotation;
        update_avatar_transform(id,entry.position,rotation,0U);
        if(auto it=avatars_.find(id);it!=avatars_.end())
            it->second.coarse_position_only=true;
    }
    // Drop only old purely coarse positions. Never discard a detailed
    // AvatarAppearance/High20 entry just because one coarse snapshot
    // omits that avatar.
    for(auto it=avatars_.begin();it!=avatars_.end();)
    {
        const auto& av=it->second;
        if(av.coarse_position_only&&!seen.contains(it->first)&&
           !av.simulator_object_update_seen&&av.local_id==0U&&
           av.texture_entry.empty()&&av.visual_params.empty()&&
           av.appearance_data.empty()&&av.attachments.empty())
        {
            pending_avatar_animations_.erase(it->first);
            it=avatars_.erase(it);
        }
        else ++it;
    }
    return true;
}

void WorldModel::remove_avatar(const std::string& id)
{
    avatars_.erase(id);
    pending_avatar_animations_.erase(id);
}

void WorldModel::upsert_object(ObjectState object)
{
    // An ObjectUpdate is current-region evidence only. Zero/unknown UUID
    // must never promote an identity-less prim into a GPU scene.
    if (!region_.connected || object.local_id == 0U || object.id.empty())
        return;
    // Malformed replacement must NOT evict a healthy mesh/linkset.
    if(!valid_simulator_prim_geometry(
           object.position,object.rotation,object.scale))
    {
        if(invalid_scene_updates_<UINT32_MAX)++invalid_scene_updates_;
        return;
    }
    const auto prior=objects_.find(object.local_id);
    const bool reused_local_id=
        prior!=objects_.end()&&prior->second.id!=object.id;
    // A dropped KillObject may leave the SAME UUID at another LocalID.
    // Use the indexed UUID rather than scanning all scene objects on
    // every Full/Compressed ObjectUpdate.
    const auto prior_uuid=prim_uuid_to_local_id_.find(object.id);
    if(prior_uuid!=prim_uuid_to_local_id_.end()&&
       prior_uuid->second!=object.local_id)
    {
        (void)kill_simulator_local_id(prior_uuid->second,false);
        if(prim_uuid_relocations_<UINT32_MAX)
            ++prim_uuid_relocations_;
    }
    // A different UUID at the incoming LocalID must also withdraw the
    // old tree before the new object is admitted.
    if(reused_local_id)
    {
        (void)kill_simulator_local_id(object.local_id,false);
        if(prim_local_id_rebinds_<UINT32_MAX)
            ++prim_local_id_rebinds_;
    }
    prim_uuid_to_local_id_.insert_or_assign(object.id,object.local_id);
    objects_[object.local_id] = std::move(object);
    ++object_revision_;
}

bool WorldModel::apply_object_terse(
    std::uint32_t local_id,Vec3 position,Quaternion rotation)
{
    if(!region_.connected || local_id==0U)return false;
    const auto it=objects_.find(local_id);
    // Terse packets never establish a prim's UUID, parent, material or
    // sculpt asset. Wait for a FULL/COMPRESSED ObjectUpdate.
    if(it==objects_.end()||it->second.pcode!=protocol::pcode_prim||
       it->second.id.empty())return false;
    const auto finite=[](float v){return std::isfinite(v);};
    if(!finite(position.x)||!finite(position.y)||!finite(position.z)||
       !finite(rotation.x)||!finite(rotation.y)||
       !finite(rotation.z)||!finite(rotation.w))
        return false;
    const double norm=double(rotation.x)*rotation.x+
        double(rotation.y)*rotation.y+
        double(rotation.z)*rotation.z+
        double(rotation.w)*rotation.w;
    if(!std::isfinite(norm)||norm<0.25||norm>4.0)
        return false;
    auto& current=it->second;
    if(current.position.x==position.x&&
       current.position.y==position.y&&
       current.position.z==position.z&&
       current.rotation.x==rotation.x&&
       current.rotation.y==rotation.y&&
       current.rotation.z==rotation.z&&
       current.rotation.w==rotation.w)
        return false; // duplicate Terse does not reupload scene
    current.position=position;
    current.rotation=rotation;
    ++object_revision_;
    return true;
}

bool WorldModel::apply_object_compressed_partial(
    const protocol::ObjectUpdateCompressedEntry& partial)
{
    if(!region_.connected || partial.has_complete_prim_data ||
       partial.local_id==0U || partial.id.empty() ||
       partial.pcode!=protocol::pcode_prim)
        return false;
    const auto it=objects_.find(partial.local_id);
    // LocalIDs may be recycled after KillObject. Never graft the previous
    // mesh/TE/attachment ParentID onto a *different* UUID.
    if(it==objects_.end()||it->second.id!=partial.id||
       it->second.pcode!=partial.pcode)return false;
    const auto finite=[](float v){return std::isfinite(v);};
    const auto& pos=partial.position;
    const auto& q=partial.rotation;
    if(!finite(pos.x)||!finite(pos.y)||!finite(pos.z)||
       !finite(q.x)||!finite(q.y)||!finite(q.z)||!finite(q.w))
        return false;
    const double norm=double(q.x)*q.x+double(q.y)*q.y+
        double(q.z)*q.z+double(q.w)*q.w;
    if(!std::isfinite(norm)||norm<0.25||norm>4.0)
        return false;
    auto& current=it->second;
    if(current.state==partial.state &&
       current.position.x==pos.x&&current.position.y==pos.y&&
       current.position.z==pos.z&&current.rotation.x==q.x&&
       current.rotation.y==q.y&&current.rotation.z==q.z&&
       current.rotation.w==q.w)
        return false;
    // A partial compressed packet DOES carry a fresh State byte, which
    // can signal an attachment-point change or detachment (state=0).
    current.state=partial.state;
    current.position=pos;
    current.rotation=q;
    ++object_revision_;
    return true;
}

void WorldModel::remove_object(std::uint32_t local_id)
{
    const auto it=objects_.find(local_id);
    if(it==objects_.end())return;
    const auto index=prim_uuid_to_local_id_.find(it->second.id);
    if(index!=prim_uuid_to_local_id_.end()&&index->second==local_id)
        prim_uuid_to_local_id_.erase(index);
    objects_.erase(it);
    ++object_revision_;
}

bool WorldModel::kill_simulator_local_id(
    std::uint32_t local_id,bool count_kill_object)
{
    if(!region_.connected||local_id==0U)return false;
    bool removed=false;
    // A KillObject for a remote avatar identifies it by simulator
    // LocalID, not the avatar UUID from AvatarAppearance/High20.
    for(auto it=avatars_.begin();it!=avatars_.end();)
    {
        if(it->second.local_id!=local_id)
        {
            ++it;
            continue;
        }
        pending_avatar_animations_.erase(it->first);
        it=avatars_.erase(it);
        if(count_kill_object&&remote_avatar_kills_<UINT32_MAX)
            ++remote_avatar_kills_;
        removed=true;
    }
    // The server may send a separate KillObject for the whole avatar
    // root; attached prims and linkset children are no longer valid even
    // if their own LocalIDs were not enumerated in that UDP packet.
    std::unordered_map<std::uint32_t,std::vector<std::uint32_t>> children;
    children.reserve(objects_.size());
    for(const auto& [id,prim]:objects_)
        if(prim.parent_id!=0U&&id!=prim.parent_id)
            children[prim.parent_id].push_back(id);
    std::vector<std::uint32_t> todo{local_id};
    std::unordered_set<std::uint32_t> dropped;
    dropped.reserve(objects_.size()+1U);
    for(std::size_t i=0U;i<todo.size();++i)
    {
        const auto id=todo[i];
        if(!dropped.insert(id).second)continue;
        if(const auto it=children.find(id);it!=children.end())
            for(const auto child:it->second)
                if(!dropped.contains(child))todo.push_back(child);
    }
    std::size_t removed_prims=0U;
    for(const auto id:dropped)
    {
        const auto it=objects_.find(id);
        if(it==objects_.end())continue;
        const auto index=prim_uuid_to_local_id_.find(it->second.id);
        if(index!=prim_uuid_to_local_id_.end()&&index->second==id)
            prim_uuid_to_local_id_.erase(index);
        objects_.erase(it);
        ++removed_prims;
    }
    if(removed_prims>0U)
    {
        ++object_revision_; // one scene invalidation for whole linkset
        if(count_kill_object)
        {
            const auto remaining=UINT32_MAX-removed_prim_kills_;
            removed_prim_kills_+=static_cast<std::uint32_t>(
                std::min<std::size_t>(removed_prims,remaining));
        }
        removed=true;
    }
    return removed;
}

std::uint32_t WorldModel::remote_avatar_local_id_rebinds() const noexcept
{
    return remote_avatar_local_id_rebinds_;
}

std::uint32_t WorldModel::prim_local_id_rebinds() const noexcept
{
    return prim_local_id_rebinds_;
}
std::uint32_t WorldModel::prim_uuid_relocations() const noexcept
{
    return prim_uuid_relocations_;
}

std::uint32_t WorldModel::invalid_scene_updates() const noexcept
{
    return invalid_scene_updates_;
}

std::uint32_t WorldModel::remote_avatar_kills() const noexcept
{
    return remote_avatar_kills_;
}

std::uint32_t WorldModel::removed_prim_kills() const noexcept
{
    return removed_prim_kills_;
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
std::uint32_t WorldModel::stale_self_cof_appearances() const noexcept
{ return stale_self_cof_appearances_; }
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
    value.self_wearables = self_wearables_;
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

bool WorldModel::self_avatar_controls_ready() const noexcept
{
    return self_avatar_renderable()&&
        valid_simulator_avatar_pose(agent_.position,agent_.rotation);
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
