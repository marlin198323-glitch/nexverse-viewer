/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. */
#pragma once
#include "nexverse/protocol/wearable_asset_transfer.hpp"
#include "nexverse/protocol/firestorm_region_lease.hpp"
#include "nexverse/protocol/grid_info.hpp"
#include "nexverse/net/http_transport.hpp"
#include <memory>
#include <optional>
#include <string>
#include <string_view>
namespace nexverse::protocol
{
// Current OpenSim ViewerAsset CAP can supply the real ORIGINAL wearable
// bytes over authenticated HTTP, where legacy LLUDP asset transfers may
// time out. Only GET. Never treat this as an avatar bake/COF mutation.
[[nodiscard]] inline std::optional<std::string>
wearable_viewer_asset_url(std::string_view authenticated_cap,
                          std::string_view asset_id,std::int32_t asset_type)
{
    if(!is_valid_http_uri(authenticated_cap)||
       authenticated_cap.size()>4096U||authenticated_cap.find('?')!=
           std::string_view::npos||
       authenticated_cap.find('#')!=std::string_view::npos)
        return std::nullopt;
    const auto uuid=uuid_to_bytes(asset_id);
    if(!uuid||std::all_of(uuid->begin(),uuid->end(),
        [](std::uint8_t x){return x==0U;}))
        return std::nullopt;
    std::string url{authenticated_cap};
    url+=(asset_type==asset_type_bodypart?"?bodypart_id=":
          asset_type==asset_type_clothing?"?clothing_id=":"");
    if(asset_type!=asset_type_bodypart&&asset_type!=asset_type_clothing)
        return std::nullopt;
    url+=asset_id;
    return url;
}
struct WearableViewerAssetResult
{
    std::uint64_t epoch=0U;
    std::string asset_id;
    std::shared_ptr<const assets::FirestormWearableAsset> asset;
    std::string error;
    bool capability_attempted=false;
    long http_status=0L;
    [[nodiscard]] bool method_unsupported() const noexcept
    {
        return capability_attempted &&
            (http_status==405L||http_status==501L);
    }
};
[[nodiscard]] inline WearableViewerAssetResult
fetch_wearable_viewer_asset(
    const std::shared_ptr<net::IHttpTransport>& transport,
    std::string_view cap,std::string_view asset_id,
    std::int32_t asset_type,std::uint64_t epoch,
    const FirestormRegionLease& lease)
{
    WearableViewerAssetResult result;
    result.epoch=epoch;
    result.asset_id=std::string{asset_id};
    const auto url=wearable_viewer_asset_url(cap,asset_id,asset_type);
    if(!transport||!lease.matches(epoch)||!url)
    {result.error="ViewerAsset not safely available";return result;}
    result.capability_attempted=true;
    const auto response=transport->get(*url,
        assets::max_firestorm_wearable_bytes,
        std::chrono::seconds{3},std::chrono::seconds{10});
    result.http_status=response.status_code;
    if(!lease.matches(epoch))
    {result.error="stale region";return result;}
    if(!response.ok())
    {
        result.error="ViewerAsset HTTP failure";
        return result;
    }
    const auto decoded=assets::parse_firestorm_wearable(
        response.body,std::nullopt);
    if(!decoded.ok())
    {
        result.error="ViewerAsset wearable parser rejected data";
        return result;
    }
    // Clothing (type 5) must have a non-bodypart wearable subtype;
    // no decoded subtype may cross the authenticated inventory category.
    if((asset_type==asset_type_bodypart)!=(decoded.asset->type<4))
    {
        result.error="ViewerAsset bodypart/clothing category mismatch";
        return result;
    }
    result.asset=std::make_shared<const assets::FirestormWearableAsset>(
        std::move(*decoded.asset));
    return result;
}
}
