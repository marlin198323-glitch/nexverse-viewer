#include "nexverse/protocol/firestorm_inventory_tree.hpp"
#include "nexverse/protocol/firestorm_outfit_wear_preflight.hpp"
#include <iostream>
#include <chrono>
#include <memory>
using namespace nexverse::protocol;
constexpr auto OWNER="11111111-1111-1111-1111-111111111111";
constexpr auto ROOT="22222222-2222-2222-2222-222222222222";
constexpr auto FOLDER="33333333-3333-3333-3333-333333333333";
constexpr auto LINK="44444444-4444-4444-4444-444444444444";
constexpr auto ITEM="55555555-5555-5555-5555-555555555555";
struct FolderHttp : nexverse::net::IHttpTransport
{
    int calls=0;
    bool wrong_owner=false;
    nexverse::net::HttpResponse get(std::string_view,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {return {405,{},"not allowed"};}
    nexverse::net::HttpResponse post(std::string_view uri,std::string_view,
        std::string_view body,std::size_t,
        std::chrono::milliseconds,std::chrono::milliseconds) override
    {
        if(uri!="https://sim.test/inventory")return {404,{},"unavailable"};
        auto request=parse_llsd_xml(body);
        if(!request.ok())return {400,{},"malformed"};
        ++calls;
        return {200,serialize_llsd_xml(LlsdValue::map({
            {"folders",LlsdValue::array({LlsdValue::map({
                {"folder_id",LlsdValue::uuid(FOLDER)},
                {"owner_id",LlsdValue::uuid(wrong_owner?LINK:OWNER)},
                {"categories",LlsdValue::array({LlsdValue::map({
                    {"category_id",LlsdValue::uuid(LINK)},
                    {"parent_id",LlsdValue::uuid(FOLDER)},
                    {"name",LlsdValue::string("Nested folder")},
                    {"type_default",LlsdValue::integer(0)},
                    {"version",LlsdValue::integer(10)}
                })})}
            })})}
        })),{}};
    }
};
int main()
{
    auto tree=inventory_tree_from_login({
        {ROOT,"","Inventory",8,4},{FOLDER,ROOT,"Outfit",47,8},
        {"invalid",ROOT,"Malformed",47,8}});
    if(tree.count()!=2U)return 1;
    OutfitInventoryRead update;
    update.valid=true;
    update.categories.emplace(LINK,InventoryFolder{
        LINK,FOLDER,"Subfolder",0,7});
    if(merge_inventory_categories(tree,update)!=1U ||
       tree.folders.at(LINK).parent_id!=FOLDER)return 2;
    update.categories.at(LINK).version=5;
    update.categories.at(LINK).name="Older";
    if(merge_inventory_categories(tree,update)!=0U ||
       tree.folders.at(LINK).name!="Subfolder")return 3;
    auto parsed=apply_inventory_categories(tree,LlsdValue::map({
       {"folders",LlsdValue::array({LlsdValue::map({
          {"folder_id",LlsdValue::uuid(FOLDER)},
          {"owner_id",LlsdValue::uuid(OWNER)},
          {"categories",LlsdValue::array({LlsdValue::map({
             {"category_id",LlsdValue::uuid(LINK)},
             {"parent_id",LlsdValue::uuid(FOLDER)},
             {"name",LlsdValue::string("Fresh")},
             {"type_default",LlsdValue::integer(0)},
             {"version",LlsdValue::integer(9)}
          })})}
       })})}
    }),OWNER,{FOLDER});
    if(parsed!=1U ||tree.folders.at(LINK).name!="Fresh")return 4;
    const auto rejected=apply_inventory_categories(tree,LlsdValue::map({
       {"folders",LlsdValue::array({LlsdValue::map({
          {"folder_id",LlsdValue::uuid(FOLDER)},
          {"owner_id",LlsdValue::uuid(LINK)},
          {"categories",LlsdValue::array({LlsdValue::map({
             {"category_id",LlsdValue::uuid(LINK)},
             {"parent_id",LlsdValue::uuid(FOLDER)},
             {"name",LlsdValue::string("Forged")},
             {"type_default",LlsdValue::integer(0)},
             {"version",LlsdValue::integer(11)}
          })})}
       })})}
    }),OWNER,{FOLDER});
    if(rejected!=0U||tree.folders.at(LINK).name!="Fresh")return 5;
    const std::vector<OutfitInventoryItem> worn{{ITEM,ITEM,"Worn",5,18}};
    const std::vector<OutfitInventoryItem> links{{LINK,ITEM,"Link",24,18}};
    auto ready=check_firestorm_outfit_preflight(FOLDER,ROOT,
        &links,&worn,worn);
    if(ready.readiness!=OutfitWearReadiness::incomplete_appearance||
       ready.can_wear()||ready.link_entries!=1U)return 6;
    auto missing=check_firestorm_outfit_preflight(FOLDER,ROOT,
        &links,&worn,{});
    if(missing.readiness!=OutfitWearReadiness::unresolved_links||
       missing.unresolved_links!=1U)return 7;
    if(check_firestorm_outfit_preflight(FOLDER,ROOT,&links,nullptr,worn)
        .readiness!=OutfitWearReadiness::missing_cof)return 8;
    if(check_firestorm_outfit_preflight(FOLDER,ROOT,nullptr,&worn,worn)
        .readiness!=OutfitWearReadiness::no_inventory)return 9;
    FirestormRegionLease lease;
    const auto epoch=lease.enter(7U,1U,"https://sim.test/seed");
    auto http=std::make_shared<FolderHttp>();
    const auto loaded=fetch_inventory_folder_children(http,
        "https://sim.test/inventory",OWNER,FOLDER,epoch,lease);
    if(!loaded.valid||loaded.tree.count()!=1U ||
       loaded.tree.folders.at(LINK).name!="Nested folder"||
       http->calls!=1)return 10;
    http->wrong_owner=true;
    if(fetch_inventory_folder_children(http,
        "https://sim.test/inventory",OWNER,FOLDER,epoch,lease).valid)
        return 11;
    const auto changed=lease.enter(8U,1U,"https://sim.test/new");
    (void)changed;
    if(fetch_inventory_folder_children(http,
        "https://sim.test/inventory",OWNER,FOLDER,epoch,lease).valid||
       http->calls!=2)return 12;
    std::cout<<"Inventory tree owner/version checks; Firestorm COF links and"
                " wear preflight fail-closed OK\n";
    return 0;
}
