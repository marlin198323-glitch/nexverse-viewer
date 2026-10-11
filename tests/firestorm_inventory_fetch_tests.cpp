#include "nexverse/protocol/firestorm_inventory_fetch.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
using namespace nexverse::protocol;
namespace
{
constexpr auto O="11111111-1111-1111-1111-111111111111";
constexpr auto R="22222222-2222-2222-2222-222222222222";
constexpr auto F="33333333-3333-3333-3333-333333333333";
constexpr auto T="44444444-4444-4444-4444-444444444444";
class Fixture : public nexverse::net::IHttpTransport
{
public:
    std::string request;
    int posts=0;
    nexverse::net::HttpResponse get(std::string_view,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {return {404,{},"read disabled"};}
    nexverse::net::HttpResponse post(std::string_view cap,
        std::string_view,std::string_view body,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {
        if(cap!="https://caps.test/folders")return {404,{},"cap not offered"};
        ++posts;request=std::string{body};
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"folders",LlsdValue::array({
                LlsdValue::map({
                    {"folder_id",LlsdValue::uuid(R)},
                    {"version",LlsdValue::integer(7)},
                    {"categories",LlsdValue::array({
                        LlsdValue::map({
                            {"category_id",LlsdValue::uuid(F)},
                            {"parent_id",LlsdValue::uuid(R)},
                            {"thumbnail",LlsdValue::map({
                                {"asset_id",LlsdValue::uuid(T)}
                            })}
                        })
                    })}
                }),
                LlsdValue::map({
                    {"folder_id",LlsdValue::uuid(F)},
                    {"version",LlsdValue::integer(8)},
                    {"categories",LlsdValue::array({})}
                })
            })}
        })),{}};
    }
};
}
int main()
{
    const std::vector<OutfitGalleryFolder> outfits{
        {F,"Classic Outfit",5,R}};
    auto request=build_outfit_descendents_request(O,outfits);
    if(!request)return 1;
    auto parsed=parse_llsd_xml(*request);
    if(!parsed.ok())return 2;
    const auto* folders=parsed.value->find("folders");
    if(!folders ||!folders->is_array()||
       folders->array_value.size()!=2U)return 3;
    for(const auto& f:folders->array_value)
       if(f.find("owner_id")->as_string()!=O ||
          !f.find("fetch_folders")->boolean_value ||
          !f.find("fetch_items")->boolean_value)return 4;
    if(build_outfit_descendents_request("invalid",outfits) ||
       build_outfit_descendents_request(O,{{"invalid","bad",0,R}}))
       return 5;
    FirestormRegionLease lease;
    const auto epoch=lease.enter(99U,1U,"https://caps.test/seed");
    auto transport=std::make_shared<Fixture>();
    const auto fetched=fetch_outfit_descendents(transport,
        "https://caps.test/folders",O,outfits,epoch,lease);
    if(!fetched.valid||fetched.returned_folders!=2U||
       fetched.versions.at(F)!=8U ||
       fetched.thumbnails.at(F).asset_id!=T||transport->posts!=1)
        return 6;
    const auto next=lease.enter(101U,1U,"https://caps.test/next");
    (void)next;
    const auto stale=fetch_outfit_descendents(transport,
        "https://caps.test/folders",O,outfits,epoch,lease);
    if(stale.valid||transport->posts!=1)return 7;
    if(fetch_outfit_descendents(transport,"file:///etc/passwd",
       O,outfits,lease.epoch(),lease).valid)return 8;
    std::cout<<"Firestorm FetchInventoryDescendents2 parent+child POST, "
       "thumbnail metadata, owner and epoch safeguards OK\n";
    return 0;
}
