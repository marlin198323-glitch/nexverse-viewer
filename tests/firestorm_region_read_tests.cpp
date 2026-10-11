#include "nexverse/protocol/firestorm_region_read.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <string>
namespace
{
constexpr auto A="11111111-1111-1111-1111-111111111111";
constexpr auto B="22222222-2222-2222-2222-222222222222";
class Fixture : public nexverse::net::IHttpTransport
{
public:
    bool pause=false;
    std::promise<void> entered,release;
    std::future<void> resume=release.get_future();
    std::atomic<int> reads{0};
    nexverse::net::HttpResponse post(
        std::string_view url,std::string_view,std::string_view,
        std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        if(url!="https://caps.test/seed")return {404,{},"wrong seed"};
        if(pause){entered.set_value();resume.wait();}
        using namespace nexverse::protocol;
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"AgentExperiences",LlsdValue::uri("https://caps.test/owned")},
            {"GetExperiences",LlsdValue::uri("https://caps.test/lists")},
            {"GetExperienceInfo",LlsdValue::uri("https://caps.test/info")}
        })),{}};
    }
    nexverse::net::HttpResponse get(
        std::string_view url,std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        ++reads;
        using namespace nexverse::protocol;
        LlsdValue body;
        if(url=="https://caps.test/owned")
            body=LlsdValue::map({
                {"experience_ids",LlsdValue::array({LlsdValue::uuid(A)})}});
        else if(url=="https://caps.test/lists")
            body=LlsdValue::map({
                {"experiences",LlsdValue::array({LlsdValue::uuid(B)})},
                {"blocked",LlsdValue::array({})}});
        else if(url.find("https://caps.test/info/id/?page_size=12")!=
                std::string_view::npos)
            body=LlsdValue::map({
                {"experience_keys",LlsdValue::array({
                    LlsdValue::map({
                        {"public_id",LlsdValue::uuid(A)},
                        {"name",LlsdValue::string("NexVerse Experience")}}),
                    LlsdValue::map({
                        {"public_id",LlsdValue::uuid(B)},
                        {"name",LlsdValue::string("Sandbox")}})
                })}
            });
        else return {404,{},"wrong URL"};
        return {200,serialize_llsd_xml(body),{}};
    }
};
}
int main()
{
    using namespace nexverse::protocol;
    const auto profile_uri=experience_profile_query(
        "https://caps.test/info",{A,B});
    if(!profile_uri ||
       profile_uri->find("&public_id=11111111")==std::string::npos ||
       profile_uri->find("&public_id=22222222")==std::string::npos ||
       experience_profile_query("file:///etc/passwd",{A}) ||
       experience_profile_query("https://caps.test/info",{"bad"}))
        return 1;
    const auto names=parse_experience_profiles(LlsdValue::map({
        {"experience_keys",LlsdValue::array({
            LlsdValue::map({
                {"public_id",LlsdValue::uuid(A)},
                {"name",LlsdValue::string("Live Name")}}),
            LlsdValue::map({
                {"public_id",LlsdValue::uuid(B)},
                {"name",LlsdValue::string("Bad\nNewline")}})
        })}
    }));
    if(names.size()!=1U||names.at(A)!="Live Name")return 2;
    auto http=std::make_shared<Fixture>();
    FirestormRegionLease region;
    const auto first=region.enter(1U,1U,"https://caps.test/seed");
    const auto found=fetch_firestorm_region(http,"https://caps.test/seed",
                                            first,region);
    if(!found.discovered||!found.owned||!found.resident ||
       found.owned->ids.size()!=1U||found.resident->allowed.size()!=1U||
       found.display_names.size()!=2U||http->reads!=3)
        return 3;
    auto paused=std::make_shared<Fixture>();
    paused->pause=true;
    auto stale=std::async(std::launch::async,[&]
    {
        return fetch_firestorm_region(paused,
            "https://caps.test/seed",first,region);
    });
    if(paused->entered.get_future().wait_for(std::chrono::seconds{3})!=
       std::future_status::ready)
    {
        paused->release.set_value();
        (void)stale.get();
        return 4;
    }
    const auto second=region.enter(2U,2U,"https://caps.test/seed");
    paused->release.set_value();
    const auto late=stale.get();
    if(late.discovered||late.owned||late.resident||paused->reads!=0)
        return 5;
    const auto fresh=fetch_firestorm_region(http,"https://caps.test/seed",
                                           second,region);
    if(!fresh.discovered ||fresh.issued_epoch!=second||
       !region.matches(fresh.issued_epoch))
        return 6;
    std::cout<<"Read-only real HTTP transport fixture: new region CAPS, "
        "owned/allowed and Experience profile names; stale seed discarded\n";
    return 0;
}
