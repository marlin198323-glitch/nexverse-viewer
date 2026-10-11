/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_region_read.hpp"
#include "nexverse/protocol/caps.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include <algorithm>
#include <set>

namespace nexverse::protocol
{
std::optional<std::string> experience_profile_query(
    std::string_view cap_uri,const std::vector<std::string>& ids)
{
    if(!is_valid_http_uri(cap_uri)||ids.empty()||ids.size()>12U||
       cap_uri.find('?')!=std::string_view::npos ||
       cap_uri.find('#')!=std::string_view::npos)
        return std::nullopt;
    std::string uri{cap_uri};
    if(uri.back()!='/')uri+='/';
    uri+="id/?page_size=12";
    for(const auto& id:ids)
    {
        const auto bytes=uuid_to_bytes(id);
        if(!bytes ||std::all_of(bytes->begin(),bytes->end(),
            [](std::uint8_t x){return x==0U;}))
            return std::nullopt;
        uri+="&public_id="+id;
    }
    return uri;
}
std::map<std::string,std::string,std::less<>>
parse_experience_profiles(const LlsdValue& response)
{
    std::map<std::string,std::string,std::less<>> result;
    if(!response.is_map())return result;
    const auto* rows=response.find("experience_keys");
    if(!rows||!rows->is_array()||rows->array_value.size()>128U)
        return result;
    for(const auto& row:rows->array_value)
    {
        if(!row.is_map())continue;
        const auto* id=row.find("public_id");
        const auto* name=row.find("name");
        if(!id||!name ||
           (id->type!=LlsdType::string&&id->type!=LlsdType::uuid) ||
           name->type!=LlsdType::string||
           !uuid_to_bytes(id->as_string())||
           name->string_value.empty()||name->string_value.size()>128U)
            continue;
        if(std::any_of(name->string_value.begin(),name->string_value.end(),
            [](unsigned char c){return c<32U ||c==127U;}))
            continue;
        result.emplace(std::string{id->as_string()},name->string_value);
    }
    return result;
}
std::map<std::string,ExperienceProfile,std::less<>>
parse_experience_profile_details(const LlsdValue& response)
{
    std::map<std::string,ExperienceProfile,std::less<>> details;
    if(!response.is_map())return details;
    const auto* rows=response.find("experience_keys");
    if(!rows||!rows->is_array()||rows->array_value.size()>128U)
        return details;
    for(const auto& row:rows->array_value)
    {
        if(!row.is_map())continue;
        const auto* id=row.find("public_id");
        const auto* name=row.find("name");
        if(!id||!name||!uuid_to_bytes(id->as_string())||
           name->type!=LlsdType::string ||
           name->string_value.empty()||name->string_value.size()>128U)
            continue;
        const auto clean=[](std::string_view value,std::size_t max)
        {
            if(value.size()>max)return false;
            return std::none_of(value.begin(),value.end(),
                [](unsigned char c){return c<32U||c==127U;});
        };
        if(!clean(name->string_value,128U))continue;
        ExperienceProfile profile;
        profile.name=name->string_value;
        if(const auto* desc=row.find("description");
           desc&&desc->type==LlsdType::string &&
           clean(desc->string_value,4096U))
            profile.description=desc->string_value.substr(0U,160U);
        if(const auto* maturity=row.find("maturity");
           maturity&&maturity->type==LlsdType::integer &&
           (maturity->integer_value==13 ||maturity->integer_value==21 ||
            maturity->integer_value==42))
            profile.maturity=static_cast<std::int32_t>(maturity->integer_value);
        if(const auto* properties=row.find("properties");
           properties&&properties->type==LlsdType::integer)
            profile.enabled=(properties->integer_value&64)==0;
        details.emplace(std::string{id->as_string()},std::move(profile));
    }
    return details;
}
FirestormRegionRead fetch_firestorm_region(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view seed,std::uint64_t issued_epoch,
    const FirestormRegionLease& lease,
    std::string_view agent_id,
    const std::vector<OutfitGalleryFolder>& outfits)
{
    FirestormRegionRead result;
    result.issued_epoch=issued_epoch;
    if(!transport ||seed.empty()||!lease.matches(issued_epoch))return result;
    CapabilityService client{transport};
    const auto caps=client.discover(seed,{
        "AgentExperiences","GetExperiences","GetExperienceInfo",
        "FetchInventory2","FetchInventoryDescendents2",
        "InventoryThumbnailUpload","GetTexture"});
    if(!caps.success ||!lease.matches(issued_epoch))return result;
    result.discovered=true;
    result.available=firestorm_caps_availability(caps.capabilities);
    if(const auto it=caps.capabilities.find("GetTexture");
       it!=caps.capabilities.end())
        result.get_texture_uri=it->second;
    if(const auto it=caps.capabilities.find("AgentExperiences");
       it!=caps.capabilities.end() &&lease.matches(issued_epoch))
    {
        const auto reply=client.fetch_llsd(it->second);
        if(!lease.matches(issued_epoch))return {};
        if(reply.success)
            result.owned=parse_agent_experiences(reply.value);
    }
    if(const auto it=caps.capabilities.find("GetExperiences");
       it!=caps.capabilities.end() &&lease.matches(issued_epoch))
    {
        const auto reply=client.fetch_llsd(it->second);
        if(!lease.matches(issued_epoch))return {};
        if(reply.success)
            result.resident=parse_resident_experiences(reply.value);
    }
    if(const auto it=caps.capabilities.find("GetExperienceInfo");
       it!=caps.capabilities.end()&&lease.matches(issued_epoch))
    {
        std::vector<std::string> ids;
        std::set<std::string> seen;
        const auto append=[&](const std::vector<std::string>& source)
        {
            for(const auto& id:source)
                if(ids.size()<12U &&seen.insert(id).second)
                    ids.push_back(id);
        };
        if(result.owned)append(result.owned->ids);
        if(result.resident)
        {
            append(result.resident->allowed);
            append(result.resident->blocked);
        }
        if(const auto url=experience_profile_query(it->second,ids))
        {
            const auto reply=client.fetch_llsd(*url);
            if(!lease.matches(issued_epoch))return {};
            if(reply.success)
            {
                result.display_names=parse_experience_profiles(reply.value);
                result.profiles=parse_experience_profile_details(reply.value);
            }
        }
    }
    if(const auto it=caps.capabilities.find("FetchInventoryDescendents2");
       it!=caps.capabilities.end() &&!agent_id.empty()&&
       !outfits.empty()&&lease.matches(issued_epoch))
    {
        result.outfit_inventory=fetch_outfit_descendents(
            transport,it->second,agent_id,outfits,issued_epoch,lease);
        if(!lease.matches(issued_epoch))return {};
    }
    return result;
}
}
