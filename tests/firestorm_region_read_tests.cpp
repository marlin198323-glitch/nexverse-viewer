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
constexpr auto ROOT="33333333-3333-3333-3333-333333333333";
constexpr auto THUMB="44444444-4444-4444-4444-444444444444";
class Fixture : public nexverse::net::IHttpTransport
{
public:
    bool pause=false;
    std::promise<void> entered,release;
    std::future<void> resume=release.get_future();
    std::atomic<int> reads{0};
    nexverse::net::HttpResponse post(
        std::string_view url,std::string_view,std::string_view body,
        std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        if(url=="https://caps.test/inventory")
        {
            const auto parsed=nexverse::protocol::parse_llsd_xml(body);
            if(!parsed.ok()||!parsed.value->find("folders"))
                return {400,{},"bad folder request"};
            using namespace nexverse::protocol;
            return {200,serialize_llsd_xml(LlsdValue::map({
                {"folders",LlsdValue::array({
                    LlsdValue::map({
                        {"folder_id",LlsdValue::uuid(ROOT)},
                        {"owner_id",LlsdValue::uuid(B)},
                        {"version",LlsdValue::integer(17)},
                        {"categories",LlsdValue::array({
                            LlsdValue::map({
                                {"category_id",LlsdValue::uuid(A)},
                                {"parent_id",LlsdValue::uuid(ROOT)},
                                {"version",LlsdValue::integer(18)},
                                {"thumbnail",LlsdValue::map({
                                    {"asset_id",LlsdValue::uuid(THUMB)}
                                })}
                            })
                        })}
                    }),
                    LlsdValue::map({
                        {"folder_id",LlsdValue::uuid(A)},
                        {"owner_id",LlsdValue::uuid(B)},
                        {"version",LlsdValue::integer(18)},
                        {"categories",LlsdValue::array({})}
                    })
                })}
            })),{}};
        }
        if(url!="https://caps.test/seed")return {404,{},"wrong seed"};
        if(pause){entered.set_value();resume.wait();}
        using namespace nexverse::protocol;
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"AgentExperiences",LlsdValue::uri("https://caps.test/owned")},
            {"GetExperiences",LlsdValue::uri("https://caps.test/lists")},
            {"GetExperienceInfo",LlsdValue::uri("https://caps.test/info")},
            {"FetchInventoryDescendents2",LlsdValue::uri(
                "https://caps.test/inventory")},
            {"GetTexture",LlsdValue::uri("https://caps.test/texture")}
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
                        {"name",LlsdValue::string("NexVerse Experience")},
                        {"description",LlsdValue::string("A real description")},
                        {"maturity",LlsdValue::integer(21)},
                        {"properties",LlsdValue::integer(0)}}),
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
    const auto profile_data=parse_experience_profile_details(LlsdValue::map({
        {"experience_keys",LlsdValue::array({
            LlsdValue::map({
                {"public_id",LlsdValue::uuid(A)},
                {"name",LlsdValue::string("My Experience")},
                {"description",LlsdValue::string("Description")},
                {"maturity",LlsdValue::integer(42)},
                {"properties",LlsdValue::integer(64)}})
        })}
    }));
    if(profile_data.size()!=1U||
       profile_data.at(A).name!="My Experience" ||
       profile_data.at(A).description!="Description" ||
       profile_data.at(A).maturity!=42 ||
       profile_data.at(A).enabled)return 7;
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
                                           second,region,
                                           B,{{A,"Outfit",18,ROOT}});
    if(!fresh.discovered ||fresh.issued_epoch!=second||
       !region.matches(fresh.issued_epoch)||
       !fresh.outfit_inventory.valid ||
       fresh.outfit_inventory.thumbnails.at(A).asset_id!=THUMB ||
       fresh.outfit_inventory.versions.at(A)!=18U||
       fresh.get_texture_uri!="https://caps.test/texture" ||
       fresh.profiles.at(A).description!="A real description" ||
       fresh.profiles.at(A).maturity!=21)
        return 6;
    std::cout<<"Read-only real HTTP transport fixture: new region CAPS, "
        "owned/allowed and Experience profile names; stale seed discarded\n";
    return 0;
}
