#include "nexverse/protocol/firestorm_inventory_gallery.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    constexpr auto r="11111111-1111-1111-1111-111111111111";
    constexpr auto o="22222222-2222-2222-2222-222222222222";
    constexpr auto a="33333333-3333-3333-3333-333333333333";
    constexpr auto b="44444444-4444-4444-4444-444444444444";
    std::vector<InventoryFolder> skeleton{
        {r,"", "My Outfits",48,6},
        {o,r,"Formal Outfit",47,12},
        {a,r,"Casual\nTest",0,13},
        {b,"", "Normal Textures",0,2},
        {o,r,"Duplicate",47,90},
        {"bad",r,"Malformed",47,1},
        {"55555555-5555-5555-5555-555555555555","invalid","Bad Parent",47,1}
    };
    const auto gallery=outfits_from_login_skeleton(skeleton);
    if(gallery.size()!=2U ||gallery[0].id!=o ||
       gallery[0].name!="Formal Outfit"||gallery[0].version!=12 ||
       gallery[1].id!=a||gallery[1].name!="CasualTest")
        return 1;
    if(!outfits_from_login_skeleton({}).empty())return 2;
    skeleton.push_back({
        "66666666-6666-6666-6666-666666666666",r,
        "Current Outfit",46,4});
    if(current_outfit_folder_id(skeleton)!=
       "66666666-6666-6666-6666-666666666666")return 3;
    std::cout<<"Firestorm outfit 47 and My Outfits 48 from real login"
        " inventory skeleton; duplicate and invalid ID filtering OK\n";
    return 0;
}
