/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_compat.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include <algorithm>
#include <string_view>

namespace
{
using nexverse::protocol::LlsdValue;
using nexverse::protocol::LlsdType;
bool has(const nexverse::protocol::CapabilityMap& map,
         std::string_view name) noexcept
{
    return map.contains(name);
}
std::optional<std::vector<std::string>> uuids(const LlsdValue* field)
{
    if(!field || !field->is_array() || field->array_value.size()>1024U)
        return std::nullopt;
    std::vector<std::string> ids;
    ids.reserve(field->array_value.size());
    for(const auto& item:field->array_value)
    {
        if(item.type!=LlsdType::uuid && item.type!=LlsdType::string)
            return std::nullopt;
        const auto uuid=item.as_string();
        if(!nexverse::protocol::uuid_to_bytes(uuid))
            return std::nullopt;
        ids.emplace_back(uuid);
    }
    return ids;
}
}
namespace nexverse::protocol
{
std::size_t FirestormCapsAvailability::advertised() const noexcept
{
    return static_cast<std::size_t>(inventory_fetch)+
           static_cast<std::size_t>(inventory_folder_fetch)+
           static_cast<std::size_t>(inventory_thumbnail_upload)+
           static_cast<std::size_t>(experience_owned)+
           static_cast<std::size_t>(experience_permissions)+
           static_cast<std::size_t>(experience_lists)+
           static_cast<std::size_t>(experience_admin)+
           static_cast<std::size_t>(experience_update);
}
FirestormCapsAvailability firestorm_caps_availability(
    const CapabilityMap& caps)
{
    return {has(caps,"FetchInventory2"),
            has(caps,"FetchInventoryDescendents2"),
            has(caps,"InventoryThumbnailUpload"),
            has(caps,"AgentExperiences"),
            has(caps,"ExperiencePreferences"),
            has(caps,"GetExperiences"),
            has(caps,"GetAdminExperiences"),
            has(caps,"UpdateExperience")};
}
std::optional<OwnedExperiences> parse_agent_experiences(
    const LlsdValue& response)
{
    if(!response.is_map())return std::nullopt;
    const auto ids=uuids(response.find("experience_ids"));
    if(!ids)return std::nullopt;
    OwnedExperiences result{*ids,{}};
    if(const auto* price=response.find("purchase"))
    {
        if(price->type!=LlsdType::integer ||
           price->integer_value<0)return std::nullopt;
        result.purchase_price=price->integer_value;
    }
    return result;
}
std::optional<ResidentExperiences> parse_resident_experiences(
    const LlsdValue& response)
{
    if(!response.is_map())return std::nullopt;
    const auto allowed=uuids(response.find("experiences"));
    const auto blocked=uuids(response.find("blocked"));
    if(!allowed||!blocked)return std::nullopt;
    return ResidentExperiences{*allowed,*blocked};
}
std::optional<std::string> build_thumbnail_begin_request(
    ThumbnailTarget target,std::string_view inventory_uuid)
{
    const auto id=uuid_to_bytes(inventory_uuid);
    if(!id||std::all_of(id->begin(),id->end(),
                        [](std::uint8_t byte){return byte==0U;}))
        return std::nullopt;
    return serialize_llsd_xml(LlsdValue::map({
        {target==ThumbnailTarget::item?"item_id":"category_id",
         LlsdValue::uuid(std::string{inventory_uuid})}
    }));
}
std::optional<ThumbnailUploadStage> parse_thumbnail_stage(
    const LlsdValue& response)
{
    if(!response.is_map())return std::nullopt;
    const auto* state=response.find("state");
    if(!state || state->type!=LlsdType::string)
        return std::nullopt;
    if(state->as_string()=="upload")
    {
        const auto* uploader=response.find("uploader");
        if(!uploader||(uploader->type!=LlsdType::string &&
                       uploader->type!=LlsdType::uri)||
           !is_valid_http_uri(uploader->as_string()))
            return std::nullopt;
        return ThumbnailUploadStage{
            ThumbnailUploadStage::State::upload,
            std::string{uploader->as_string()},{}};
    }
    if(state->as_string()=="complete")
    {
        const auto* uuid=response.find("new_asset");
        if(!uuid||(uuid->type!=LlsdType::uuid &&
                   uuid->type!=LlsdType::string))
            return std::nullopt;
        const auto bytes=uuid_to_bytes(uuid->as_string());
        if(!bytes||std::all_of(bytes->begin(),bytes->end(),
                               [](std::uint8_t byte){return byte==0U;}))
            return std::nullopt;
        return ThumbnailUploadStage{
            ThumbnailUploadStage::State::complete,{},
            std::string{uuid->as_string()}};
    }
    return std::nullopt;
}
std::optional<InventoryThumbnailMetadata>
parse_inventory_thumbnail_metadata(const LlsdValue& node)
{
    if(!node.is_map())return std::nullopt;
    const auto* thumbnail=node.find("thumbnail");
    if(!thumbnail)return InventoryThumbnailMetadata{};
    if(!thumbnail->is_map())return std::nullopt;
    const auto* asset=thumbnail->find("asset_id");
    if(!asset)return std::nullopt;
    if(asset->type!=LlsdType::uuid && asset->type!=LlsdType::string)
        return std::nullopt;
    const auto bytes=uuid_to_bytes(asset->as_string());
    if(!bytes)return std::nullopt;
    InventoryThumbnailMetadata result;
    result.advertised=true;
    if(!std::all_of(bytes->begin(),bytes->end(),
                    [](std::uint8_t byte){return byte==0U;}))
        result.asset_id=std::string{asset->as_string()};
    return result;
}
ThumbnailCacheUpdate InventoryFolderThumbnailCache::apply(
    std::string_view folder_id,std::uint64_t version,
    const InventoryThumbnailMetadata& thumbnail)
{
    const auto id=uuid_to_bytes(folder_id);
    if(!id || std::all_of(id->begin(),id->end(),
                          [](std::uint8_t byte){return byte==0U;}))
        return ThumbnailCacheUpdate::stale;
    const auto it=entries_.find(folder_id);
    if(it!=entries_.end())
    {
        if(version<it->second.version)
            return ThumbnailCacheUpdate::stale;
        // Legacy inventory fetches omit the thumbnail map altogether;
        // omission is NOT an instruction to erase a known association.
        if(!thumbnail.advertised)
            return ThumbnailCacheUpdate::unchanged;
        if(version==it->second.version &&
           it->second.thumbnail.advertised==thumbnail.advertised &&
           it->second.thumbnail.asset_id==thumbnail.asset_id)
            return ThumbnailCacheUpdate::unchanged;
    }
    entries_.insert_or_assign(std::string{folder_id},
                              Entry{version,thumbnail});
    return ThumbnailCacheUpdate::applied;
}
}
