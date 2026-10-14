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
constexpr auto COF="55555555-5555-5555-5555-555555555555";
constexpr auto LINK="66666666-6666-6666-6666-666666666666";
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
                    {"owner_id",LlsdValue::uuid(O)},
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
                    {"owner_id",LlsdValue::uuid(O)},
                    {"version",LlsdValue::integer(8)},
                    {"items",LlsdValue::array({
                        LlsdValue::map({
                            {"item_id",LlsdValue::uuid(T)},
                            {"asset_id",LlsdValue::uuid(T)},
                            {"name",LlsdValue::string("Outfit Shirt")},
                            {"type",LlsdValue::integer(5)},
                            {"inv_type",LlsdValue::integer(18)}
                        })
                    })},
                    {"categories",LlsdValue::array({})}
                }),
                LlsdValue::map({
                    {"folder_id",LlsdValue::uuid(COF)},
                    {"owner_id",LlsdValue::uuid(O)},
                    {"version",LlsdValue::integer(42)},
                    {"items",LlsdValue::array({
                        LlsdValue::map({
                            {"item_id",LlsdValue::uuid(LINK)},
                            {"asset_id",LlsdValue::uuid(T)},
                            {"name",LlsdValue::string("COF Shirt Link")},
                            {"type",LlsdValue::integer(24)},
                            {"inv_type",LlsdValue::integer(18)}
                        })
                    })},
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
       fetched.thumbnails.at(F).asset_id!=T||transport->posts!=1||
       fetched.items.at(F).size()!=1U ||
       fetched.items.at(F)[0].name!="Outfit Shirt")
        return 6;
    if(!build_outfit_descendents_request(O,{},F))return 18;
    // COF-Pruefen MUST fetch the actual Current Outfit Folder (type 46),
    // not merely FetchInventory2 of the selected outfit folder.
    const auto with_cof=fetch_outfit_descendents(transport,
        "https://caps.test/folders",O,outfits,epoch,lease,COF);
    if(!with_cof.valid||with_cof.returned_folders!=3U||
       !with_cof.versions.contains(COF)||
       with_cof.versions.at(COF)!=42U||
       !with_cof.items.contains(COF)||
       with_cof.items.at(COF).size()!=1U||
       with_cof.items.at(COF)[0].item_id!=LINK||
       with_cof.items.at(COF)[0].asset_id!=T||
       with_cof.items.at(COF)[0].asset_type!=24||
       transport->posts!=2)return 21;
    const auto parsed_query=parse_llsd_xml(transport->request);
    if(!parsed_query.ok())return 22;
    const auto* requests=parsed_query.value->find("folders");
    if(!requests||!requests->is_array()||
       requests->array_value.size()!=3U)return 23;
    bool cof_requested=false;
    for(const auto& f:requests->array_value)
        if(f.find("folder_id")&&f.find("folder_id")->as_string()==COF)
            cof_requested=true;
    if(!cof_requested)return 24;
    if(parse_outfit_descendents(LlsdValue::map({
        {"folders",LlsdValue::array({
            LlsdValue::map({
                {"folder_id",LlsdValue::uuid(F)},
                {"owner_id",LlsdValue::uuid(R)},
                {"version",LlsdValue::integer(8)},
                {"categories",LlsdValue::array({})}
            })
        })}
    }),outfits,"",O).returned_folders!=0U)
        return 20;
    const auto next=lease.enter(101U,1U,"https://caps.test/next");
    (void)next;
    const auto stale=fetch_outfit_descendents(transport,
        "https://caps.test/folders",O,outfits,epoch,lease);
    if(stale.valid||transport->posts!=2)return 7;
    if(fetch_outfit_descendents(transport,"file:///etc/passwd",
       O,outfits,lease.epoch(),lease).valid)return 8;
    std::cout<<"Firestorm FetchInventoryDescendents2 parent+child POST, "
       "thumbnail metadata, owner and epoch safeguards OK\n";
    return 0;
}
