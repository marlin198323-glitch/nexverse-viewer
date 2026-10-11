#include "nexverse/protocol/firestorm_inventory_items.hpp"
#include "nexverse/protocol/llsd.hpp"
#include <chrono>
#include <iostream>
#include <memory>
using namespace nexverse::protocol;
constexpr auto AGENT="11111111-1111-1111-1111-111111111111";
constexpr auto A="22222222-2222-2222-2222-222222222222";
constexpr auto ASSET="33333333-3333-3333-3333-333333333333";
struct Http : nexverse::net::IHttpTransport
{
    int requests=0;
    nexverse::net::HttpResponse get(std::string_view,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {return {405,{},"unavailable"};}
    nexverse::net::HttpResponse post(std::string_view url,std::string_view,
        std::string_view body,std::size_t,std::chrono::milliseconds,
        std::chrono::milliseconds) override
    {
        if(url!="https://sim.test/FetchInventory2" ||
           !parse_llsd_xml(body).ok())return {400,{},"bad"};
        ++requests;
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"agent_id",LlsdValue::uuid(AGENT)},
            {"items",LlsdValue::array({LlsdValue::map({
                {"item_id",LlsdValue::uuid(A)},
                {"name",LlsdValue::string("Shirt")},
                {"asset_id",LlsdValue::uuid(ASSET)},
                {"type",LlsdValue::integer(5)},
                {"inv_type",LlsdValue::integer(18)},
                {"permissions",LlsdValue::map({
                    {"owner_id",LlsdValue::uuid(AGENT)}
                })}
            })})}
        })),{}};
    }
};
int main()
{
    const auto body=build_fetch_inventory2_request(AGENT,{A});
    if(!body||!parse_llsd_xml(*body).ok()||
       build_fetch_inventory2_request("no",{A})||
       build_fetch_inventory2_request(AGENT,{A,A}))
        return 1;
    FirestormRegionLease lease;
    const auto epoch=lease.enter(3U,4U,"https://sim.test/seed");
    auto http=std::make_shared<Http>();
    const auto fetched=fetch_inventory2(http,"https://sim.test/FetchInventory2",
        AGENT,{A},epoch,lease);
    if(!fetched.valid||fetched.items.size()!=1U ||
       fetched.items[0].name!="Shirt"||
       fetched.items[0].asset_id!=ASSET||
       http->requests!=1)
        return 2;
    const auto evil=parse_fetch_inventory2(LlsdValue::map({
        {"agent_id",LlsdValue::uuid(AGENT)},
        {"items",LlsdValue::array({LlsdValue::map({
            {"item_id",LlsdValue::uuid(A)},
            {"name",LlsdValue::string("Forged")},
            {"type",LlsdValue::integer(5)},
            {"inv_type",LlsdValue::integer(18)},
            {"permissions",LlsdValue::map({
                {"owner_id",LlsdValue::uuid(ASSET)}
            })}
        })})}
    }),AGENT,{A});
    if(!evil.valid||!evil.items.empty())return 3;
    const auto changed=lease.enter(8U,4U,"https://sim.test/new");
    (void)changed;
    if(fetch_inventory2(http,"https://sim.test/FetchInventory2",
        AGENT,{A},epoch,lease).valid ||http->requests!=1)
        return 4;
    std::cout<<"FetchInventory2 owner scoped, bounded item fetch, invalid owner"
                " and stale region rejected\n";
    return 0;
}
