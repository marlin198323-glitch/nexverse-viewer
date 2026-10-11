/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#include "nexverse/protocol/firestorm_experience_permissions.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include "nexverse/protocol/viewer_messages.hpp"
#include <algorithm>
#include <chrono>

namespace nexverse::protocol
{
bool ExperiencePermissionConsent::propose(
    std::string_view id,ExperiencePermissionDecision decision,
    std::uint64_t epoch,bool capability_advertised)
{
    cancel();
    const auto bytes=uuid_to_bytes(id);
    if(!capability_advertised||epoch==0U||!bytes||
       std::all_of(bytes->begin(),bytes->end(),
           [](std::uint8_t value){return value==0U;}))
        return false;
    pending_=ExperiencePermissionRequest{epoch,std::string{id},decision};
    return true;
}
std::optional<ExperiencePermissionRequest>
ExperiencePermissionConsent::confirm(std::uint64_t epoch)
{
    if(!pending_||epoch==0U||pending_->epoch!=epoch)
    {
        cancel();
        return std::nullopt;
    }
    auto approved=std::move(pending_);
    cancel();
    return approved;
}
std::optional<std::string> build_experience_permission_request(
    const ExperiencePermissionRequest& request)
{
    const auto bytes=uuid_to_bytes(request.experience_id);
    if(request.epoch==0U ||!bytes ||
       std::all_of(bytes->begin(),bytes->end(),
          [](std::uint8_t byte){return byte==0U;}))
        return std::nullopt;
    return serialize_llsd_xml(LlsdValue::map({
        {request.experience_id,LlsdValue::map({
            {"permission",LlsdValue::string(
                request.decision==ExperiencePermissionDecision::allow
                    ?"Allow":"Block")}
        })}
    }));
}
ExperiencePermissionResponse write_experience_permission(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view uri,const ExperiencePermissionRequest& request,
    const FirestormRegionLease& lease)
{
    ExperiencePermissionResponse result;
    result.epoch=request.epoch;
    if(!lease.matches(request.epoch)||!transport||
       uri.empty()||uri.size()>4096U||
       uri.find('#')!=std::string_view::npos||!is_valid_http_uri(uri))
        return result;
    const auto body=build_experience_permission_request(request);
    if(!body)return result;
    const auto response=transport->put(uri,"application/llsd+xml",*body,
        64U*1024U,std::chrono::seconds{4},std::chrono::seconds{12});
    if(!lease.matches(request.epoch)||!response.ok())return result;
    const auto parsed=parse_llsd_xml(response.body);
    if(!parsed.ok())return result;
    result.resident=parse_resident_experiences(*parsed.value);
    if(!result.resident)return result;
    const auto& list=request.decision==ExperiencePermissionDecision::allow
        ?result.resident->allowed:result.resident->blocked;
    const auto& opposite=request.decision==ExperiencePermissionDecision::allow
        ?result.resident->blocked:result.resident->allowed;
    result.confirmed=std::find(list.begin(),list.end(),
         request.experience_id)!=list.end() &&
        std::find(opposite.begin(),opposite.end(),
         request.experience_id)==opposite.end();
    return result;
}
}
