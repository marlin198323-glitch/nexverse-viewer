#include "nexverse/protocol/firestorm_inventory_sync.hpp"
#include <iostream>
using namespace nexverse::protocol;
int main()
{
    constexpr auto F="33333333-3333-3333-3333-333333333333";
    constexpr auto T="44444444-4444-4444-4444-444444444444";
    constexpr auto U="55555555-5555-5555-5555-555555555555";
    FirestormInventorySync sync;
    sync.enter(7);
    OutfitInventoryRead original;
    original.valid=true;
    original.versions[F]=8;
    original.items[F]={{F,T,"Old item",5,5}};
    original.thumbnails[F]={true,std::string{T}};
    if(!sync.merge(7,original)||sync.items().at(F).size()!=1U ||
       sync.thumbnails().at(F)!=T)return 1;
    auto stale=original;
    stale.versions[F]=7;
    stale.items[F]={{F,U,"Stale item",1,1}};
    stale.thumbnails[F]={true,std::string{U}};
    if(sync.merge(7,stale)||sync.items().at(F)[0].name!="Old item" ||
       sync.thumbnails().at(F)!=T)return 2;
    auto omitted=original;
    omitted.items.clear();
    omitted.thumbnails.clear();
    if(sync.merge(7,omitted)||sync.items().at(F).size()!=1U ||
       sync.thumbnails().at(F)!=T)return 3;
    auto replaced=original;
    replaced.thumbnails[F]={true,std::string{U}};
    if(!sync.merge(7,replaced)||sync.thumbnails().at(F)!=U)return 4;
    auto newer=original;
    newer.versions[F]=9;
    newer.items[F]={{F,T,"New item",5,5}};
    newer.thumbnails.clear();
    if(!sync.merge(7,newer)||sync.items().at(F)[0].name!="New item" ||
       sync.versions().at(F)!=9U||sync.thumbnails().at(F)!=U)return 5;
    sync.enter(8);
    if(sync.read()||!sync.items().empty()||!sync.thumbnails().empty()||
       sync.merge(7,original)||sync.merge(8,OutfitInventoryRead{}))
        return 6;
    if(!sync.merge(8,newer)||sync.items().at(F)[0].name!="New item")
        return 7;
    std::cout<<"Firestorm inventory sync: version monotonic, replaced thumbnails,"
                " legacy omissions, new region reset OK\n";
    return 0;
}
