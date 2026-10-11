#include "nexverse/protocol/firestorm_compat.hpp"
#include <iostream>
int main()
{
    using namespace nexverse::protocol;
    constexpr auto folder="44444444-4444-4444-4444-444444444444";
    constexpr auto old="55555555-5555-5555-5555-555555555555";
    constexpr auto next="66666666-6666-6666-6666-666666666666";
    const auto missing=parse_inventory_thumbnail_metadata(LlsdValue::map({}));
    if(!missing||missing->advertised||missing->asset_id)return 1;
    const auto present=parse_inventory_thumbnail_metadata(LlsdValue::map({
        {"thumbnail",LlsdValue::map({
            {"asset_id",LlsdValue::uuid(old)}
        })}
    }));
    if(!present||!present->advertised||present->asset_id!=old)return 2;
    const auto replaced=parse_inventory_thumbnail_metadata(LlsdValue::map({
        {"thumbnail",LlsdValue::map({
            {"asset_id",LlsdValue::uuid(next)}
        })}
    }));
    if(!replaced||replaced->asset_id!=next)return 3;
    const auto empty=parse_inventory_thumbnail_metadata(LlsdValue::map({
        {"thumbnail",LlsdValue::map({
            {"asset_id",LlsdValue::uuid(
                "00000000-0000-0000-0000-000000000000")}
        })}
    }));
    if(!empty||!empty->advertised||empty->asset_id)return 4;
    if(parse_inventory_thumbnail_metadata(LlsdValue::map({
        {"thumbnail",LlsdValue::string("bad")}
    }))||parse_inventory_thumbnail_metadata(LlsdValue::map({
        {"thumbnail",LlsdValue::map({
            {"asset_id",LlsdValue::string("invalid")}
        })}
    })))return 5;
    InventoryFolderThumbnailCache cache;
    if(cache.apply(folder,22U,*present)!=ThumbnailCacheUpdate::applied)
        return 6;
    if(cache.apply(folder,22U,*present)!=ThumbnailCacheUpdate::unchanged)
        return 7;
    // Firestorm 7.2.4 gallery bug: an asset UUID replacement must
    // invalidate a cached association even with a stale folder version.
    if(cache.apply(folder,22U,*replaced)!=ThumbnailCacheUpdate::applied)
        return 8;
    if(cache.apply(folder,21U,*present)!=ThumbnailCacheUpdate::stale)
        return 9;
    if(cache.apply(folder,24U,*replaced)!=ThumbnailCacheUpdate::applied)
        return 10;
    // Old Robust or mixed versions may omit a thumbnail property entirely.
    if(cache.apply(folder,25U,*missing)!=ThumbnailCacheUpdate::unchanged)
        return 15;
    // Explicit zero UUID, unlike a missing field, removes the association.
    if(cache.apply(folder,25U,*empty)!=ThumbnailCacheUpdate::applied)
        return 16;
    if(cache.apply(folder,26U,*replaced)!=ThumbnailCacheUpdate::applied)
        return 17;
    if(cache.apply(folder,23U,*missing)!=ThumbnailCacheUpdate::stale)
        return 11;
    if(cache.count()!=1U)return 12;
    if(cache.apply("bad-id",25U,*empty)!=ThumbnailCacheUpdate::stale)
        return 13;
    cache.reset();
    if(cache.count()!=0U)return 14;
    std::cout<<"Thumbnail metadata missing vs empty, new asset version"
        " and Firestorm folder cache rollover OK\n";
    return 0;
}
