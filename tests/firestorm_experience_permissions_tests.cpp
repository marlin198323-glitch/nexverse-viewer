#include "nexverse/protocol/firestorm_experience_permissions.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <chrono>
#include <iostream>
#include <memory>
using namespace nexverse::protocol;
namespace
{
constexpr auto ID="22222222-2222-2222-2222-222222222222";
class Fixture : public nexverse::net::IHttpTransport
{
public:
    int writes=0;
    bool contradictory=false;
    nexverse::net::HttpResponse get(std::string_view,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {return {404,{},"not available"};}
    nexverse::net::HttpResponse post(std::string_view,std::string_view,
        std::string_view,std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {return {405,{},"not available"};}
    nexverse::net::HttpResponse put(std::string_view uri,std::string_view,
        std::string_view body,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {
        if(uri!="https://caps.test/permissions")return {404,{},"wrong cap"};
        auto req=parse_llsd_xml(body);
        if(!req.ok()||!req.value->is_map())return {400,{},"invalid"};
        auto entry=req.value->find(ID);
        if(!entry||!entry->is_map()||
           !entry->find("permission"))return {400,{},"wrong id"};
        ++writes;
        const auto allow=entry->find("permission")->as_string()=="Allow";
        const auto accepted=contradictory?!allow:allow;
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"experiences",LlsdValue::array(
                accepted?std::vector<LlsdValue>{LlsdValue::uuid(ID)}
                        :std::vector<LlsdValue>{})},
            {"blocked",LlsdValue::array(
                !accepted?std::vector<LlsdValue>{LlsdValue::uuid(ID)}
                         :std::vector<LlsdValue>{})}
        })),{}};
    }
};
}
int main()
{
    FirestormRegionLease lease;
    auto epoch=lease.enter(1U,1U,"https://caps.test/seed");
    ExperiencePermissionConsent prompt;
    if(prompt.propose(ID,ExperiencePermissionDecision::allow,epoch,false) ||
       prompt.confirm(epoch))return 1;
    if(!prompt.propose(ID,ExperiencePermissionDecision::allow,epoch,true))
        return 2;
    prompt.cancel();
    if(prompt.confirm(epoch))return 3;
    if(!prompt.propose(ID,ExperiencePermissionDecision::allow,epoch,true))
        return 4;
    auto approved=prompt.confirm(epoch);
    if(!approved||prompt.pending())return 5;
    const auto wire=build_experience_permission_request(*approved);
    if(!wire)return 6;
    auto decoded=parse_llsd_xml(*wire);
    if(!decoded.ok()||decoded.value->map_value.size()!=1U||
       decoded.value->find(ID)->find("permission")->as_string()!="Allow")
        return 7;
    auto transport=std::make_shared<Fixture>();
    if(transport->writes!=0)return 8;
    auto response=write_experience_permission(transport,
        "https://caps.test/permissions",*approved,lease);
    if(!response.confirmed||!response.resident||
       transport->writes!=1)return 9;
    transport->contradictory=true;
    if(write_experience_permission(transport,
        "https://caps.test/permissions",*approved,lease).confirmed)
        return 10;
    auto later=lease.enter(2U,1U,"https://caps.test/seed");
    (void)later;
    if(write_experience_permission(transport,
        "https://caps.test/permissions",*approved,lease).confirmed||
       transport->writes!=2)return 11;
    if(prompt.propose("invalid",ExperiencePermissionDecision::block,
                      lease.epoch(),true))
        return 12;
    if(!prompt.propose(ID,ExperiencePermissionDecision::block,
                       lease.epoch(),true))return 13;
    if(prompt.confirm(epoch))return 14;
    if(!prompt.propose(ID,ExperiencePermissionDecision::block,
                       lease.epoch(),true))return 15;
    auto block=prompt.confirm(lease.epoch());
    transport->contradictory=false;
    if(!block||!write_experience_permission(transport,
        "https://caps.test/permissions",*block,lease).confirmed)
        return 16;
    std::cout<<"Explicit 2-step Experience consent: verified PUT LLSD,"
                " server readback, stale epoch and cancellation OK\n";
    return 0;
}
