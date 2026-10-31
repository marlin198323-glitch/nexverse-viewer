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
enum class WearableViewerAssetFailure
{
    none, request_unavailable, stale_region, http_not_found,
    http_forbidden, http_method_unsupported, http_server_error,
    http_client_error, http_transport_error, parser_rejected,
    category_mismatch
};
[[nodiscard]] inline constexpr std::string_view
wearable_viewer_asset_failure_label(WearableViewerAssetFailure failure) noexcept
{
    switch(failure)
    {
    case WearableViewerAssetFailure::none:return "none";
    case WearableViewerAssetFailure::request_unavailable:return "request_unavailable";
    case WearableViewerAssetFailure::stale_region:return "stale_region";
    case WearableViewerAssetFailure::http_not_found:return "http_not_found";
    case WearableViewerAssetFailure::http_forbidden:return "http_forbidden";
    case WearableViewerAssetFailure::http_method_unsupported:return "http_method_unsupported";
    case WearableViewerAssetFailure::http_server_error:return "http_server_error";
    case WearableViewerAssetFailure::http_client_error:return "http_client_error";
    case WearableViewerAssetFailure::http_transport_error:return "http_transport_error";
    case WearableViewerAssetFailure::parser_rejected:return "parser_rejected";
    case WearableViewerAssetFailure::category_mismatch:return "category_mismatch";
    }
    return "unknown";
}
[[nodiscard]] inline constexpr WearableViewerAssetFailure
classify_viewer_asset_http_failure(long status) noexcept
{
    if(status==404L)return WearableViewerAssetFailure::http_not_found;
    if(status==401L||status==403L)
        return WearableViewerAssetFailure::http_forbidden;
    if(status==405L||status==501L)
        return WearableViewerAssetFailure::http_method_unsupported;
    if(status>=500L)return WearableViewerAssetFailure::http_server_error;
    if(status>=400L)return WearableViewerAssetFailure::http_client_error;
    return WearableViewerAssetFailure::http_transport_error;
}
struct WearableViewerAssetResult
{
    std::uint64_t epoch=0U;
    std::string asset_id;
    std::shared_ptr<const assets::FirestormWearableAsset> asset;
    std::string error;
    bool capability_attempted=false;
    long http_status=0L;
    WearableViewerAssetFailure failure=WearableViewerAssetFailure::none;
    [[nodiscard]] bool method_unsupported() const noexcept
    {
        return capability_attempted &&
            failure==WearableViewerAssetFailure::http_method_unsupported;
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
    {
        result.error="ViewerAsset not safely available";
        result.failure=WearableViewerAssetFailure::request_unavailable;
        return result;
    }
    result.capability_attempted=true;
    const auto response=transport->get(*url,
        assets::max_firestorm_wearable_bytes,
        std::chrono::seconds{3},std::chrono::seconds{10});
    result.http_status=response.status_code;
    if(!lease.matches(epoch))
    {
        result.error="stale region";
        result.failure=WearableViewerAssetFailure::stale_region;
        return result;
    }
    if(!response.ok())
    {
        result.error="ViewerAsset HTTP failure";
        result.failure=classify_viewer_asset_http_failure(
            response.status_code);
        return result;
    }
    const auto decoded=assets::parse_firestorm_wearable(
        response.body,std::nullopt);
    if(!decoded.ok())
    {
        result.error="ViewerAsset wearable parser rejected data";
        result.failure=WearableViewerAssetFailure::parser_rejected;
        return result;
    }
    // Clothing (type 5) must have a non-bodypart wearable subtype;
    // no decoded subtype may cross the authenticated inventory category.
    if((asset_type==asset_type_bodypart)!=(decoded.asset->type<4))
    {
        result.error="ViewerAsset bodypart/clothing category mismatch";
        result.failure=WearableViewerAssetFailure::category_mismatch;
        return result;
    }
    result.asset=std::make_shared<const assets::FirestormWearableAsset>(
        std::move(*decoded.asset));
    return result;
}
}
