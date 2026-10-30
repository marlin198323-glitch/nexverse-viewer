/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#include "nexverse/protocol/wearable_asset_transfer.hpp"
#include "nexverse/protocol/firestorm_wearable_viewer_asset.hpp"
#include <cassert>
#include <iostream>
using namespace nexverse::protocol;
constexpr auto ASSET="11111111-1111-1111-1111-111111111111";
constexpr auto TX="22222222-2222-2222-2222-222222222222";
constexpr auto TID="33333333-3333-4333-8333-333333333333";
constexpr auto OTH="44444444-4444-4444-8444-444444444444";
constexpr auto NIL="00000000-0000-0000-0000-000000000000";
static std::vector<std::uint8_t> info(std::string_view transfer,
    std::string_view asset,int type,int status,int size)
{
    std::vector<std::uint8_t> out;
    const auto tid=uuid_to_bytes(transfer),aid=uuid_to_bytes(asset);
    out.insert(out.end(),tid->begin(),tid->end());
    transfer_detail::append32(out,2U);
    transfer_detail::append32(out,0U);
    transfer_detail::append32(out,static_cast<std::uint32_t>(status));
    transfer_detail::append32(out,static_cast<std::uint32_t>(size));
    out.push_back(20U);out.push_back(0U);
    out.insert(out.end(),aid->begin(),aid->end());
    transfer_detail::append32(out,static_cast<std::uint32_t>(type));
    return out;
}
static AnimationTransferPacket part(std::uint32_t index,bool last,
                                    std::string_view payload)
{
    return {TID,index,last,
            std::vector<std::uint8_t>(payload.begin(),payload.end())};
}
int main()
{
    WearableAssetRequest req{ASSET,TID,5};
    auto wire=encode_wearable_asset_transfer(93U,req);
    auto decoded=decode_lludp_packet(wire);
    if(!decoded.ok()||decoded.packet->header.message!=msg_transfer_request||
       (decoded.packet->header.flags&packet_reliable)==0||
       decoded.packet->payload.size()!=16U+4U+4U+4U+2U+16U+4U||
       transfer_detail::i32(decoded.packet->payload,46U)!=5)return 1;
    req.asset_type=13;
    wire=encode_wearable_asset_transfer(94U,req);
    decoded=decode_lludp_packet(wire);
    if(!decoded.ok()||transfer_detail::i32(decoded.packet->payload,46U)!=13)
        return 2;
    req.asset_type=20;
    if(!encode_wearable_asset_transfer(94U,req).empty())return 3;
    req.asset_type=5;req.transfer_id=NIL;
    if(!encode_wearable_asset_transfer(94U,req).empty())return 4;
    const auto bytes=info(TID,ASSET,5,0,150);
    auto parsed=decode_wearable_transfer_info(bytes);
    if(!parsed||parsed->asset_id!=ASSET||parsed->type!=5||
       parsed->size!=150)return 5;
    if(decode_wearable_transfer_info(info(TID,ASSET,20,0,150))||
       decode_wearable_transfer_info(info(TID,ASSET,5,0,0))||
       decode_wearable_transfer_info(info(TID,ASSET,5,0,3000000)))
        return 6;
    auto absent=decode_wearable_transfer_info(info(TID,ASSET,13,-2,0));
    if(!absent||absent->status!=-2)return 7;
    WearableTransferInbox inbox;inbox.reset(7U);
    if(inbox.begin(7U,WearableAssetRequest{ASSET,TID,20})||
       inbox.begin(7U,WearableAssetRequest{ASSET,NIL,5})||
       inbox.begin(7U,WearableAssetRequest{NIL,TID,5}))return 20;
    if(!inbox.begin(7U,WearableAssetRequest{ASSET,TID,5})||
       inbox.begin(8U,WearableAssetRequest{ASSET,OTH,5})||
       inbox.begin(7U,WearableAssetRequest{ASSET,OTH,5}))return 8;
    const std::string asset=
        "LLWearable version 1\n"
        "White Shirt\n"
        "A nice shirt\n"
        "permissions 0\n"
        "{\nmask 7fffffff\n}\n"
        "sale_info 0\n"
        "{\nprice 0\n}\n"
        "type 4\n"
        "parameters 1\n"
        "100 0.75\n"
        "textures 1\n"
        "1 22222222-2222-2222-2222-222222222222\n";
    const std::size_t split=asset.size()/2U;
    const auto first=part(0U,false,
        std::string_view{asset}.substr(0U,split));
    const auto last=part(1U,true,
        std::string_view{asset}.substr(split));
    inbox.on_packet(last);
    inbox.on_info(*decode_wearable_transfer_info(info(
        TID,ASSET,5,0,static_cast<int>(asset.size()))));
    if(inbox.pop())return 9;
    inbox.on_packet(first);
    auto finished=inbox.pop();
    if(!finished||!finished->asset||
       finished->asset->type!=4||
       finished->asset->name!="White Shirt"||
       finished->asset->textures.at(1)!=TX||
       finished->asset->visual_params.at(100)!=0.75f||
       inbox.pending_count()!=0U)return 10;
    inbox.reset(8U);
    if(inbox.begin(7U,WearableAssetRequest{ASSET,TID,5})||
       !inbox.begin(8U,WearableAssetRequest{ASSET,TID,5}))return 11;
    inbox.on_info(*decode_wearable_transfer_info(info(TID,ASSET,13,0,10)));
    finished=inbox.pop();
    if(!finished||finished->asset||
       finished->error.find("mismatch")==std::string::npos)return 12;
    inbox.reset(9U);
    if(!inbox.begin(9U,WearableAssetRequest{ASSET,TID,5}))return 13;
    inbox.on_packet(part(0U,true,"bad"));
    inbox.on_info(*decode_wearable_transfer_info(info(TID,ASSET,5,0,3)));
    finished=inbox.pop();
    if(!finished||finished->asset||finished->error.empty())return 14;
    inbox.reset(10U);
    if(!inbox.begin(10U,WearableAssetRequest{ASSET,TID,5}))return 15;
    inbox.on_packet(part(0U,false,"abc"));
    inbox.on_packet(part(0U,false,"abcd"));
    finished=inbox.pop();
    if(!finished||finished->asset)return 16;
    inbox.reset(11U);
    if(!inbox.begin(11U,WearableAssetRequest{ASSET,TID,5}))return 17;
    inbox.on_info(*decode_wearable_transfer_info(info(TID,ASSET,5,-2,0)));
    finished=inbox.pop();
    if(!finished||finished->asset||finished->error.empty())return 18;
    inbox.reset(12U);
    if(inbox.pop()||inbox.pending_count()!=0U)return 19;
    // OpenSim ViewerAsset authentic original wearable GET, not a bake.
    struct TestViewerAsset : nexverse::net::IHttpTransport
    {
        std::string seen;
        std::string fixture;
        long code=200;
        nexverse::net::HttpResponse get(std::string_view url,
            std::size_t max,std::chrono::milliseconds,
            std::chrono::milliseconds) override
        {
            seen=std::string{url};
            if(max!=nexverse::assets::max_firestorm_wearable_bytes)
                return {400,{},"unbounded"};
            return {code,fixture,{}};
        }
        nexverse::net::HttpResponse post(std::string_view,std::string_view,
            std::string_view,std::size_t,std::chrono::milliseconds,
            std::chrono::milliseconds) override
        {return {405,{},"read-only"};}
    };
    const auto url=wearable_viewer_asset_url("https://caps.test/asset",
        ASSET,asset_type_clothing);
    if(!url||*url!="https://caps.test/asset?clothing_id="+
        std::string{ASSET} ||
       wearable_viewer_asset_url("https://caps.test/asset?x=1",
           ASSET,asset_type_clothing)||
       wearable_viewer_asset_url("https://caps.test/asset#secret",
           ASSET,asset_type_clothing)||
       wearable_viewer_asset_url("file:///asset",
           ASSET,asset_type_clothing)||
       wearable_viewer_asset_url("https://caps.test/asset",
           NIL,asset_type_clothing)||
       wearable_viewer_asset_url("https://caps.test/asset",
           ASSET,20))
        return 21;
    const auto body_url=wearable_viewer_asset_url(
        "https://caps.test/asset",ASSET,asset_type_bodypart);
    if(!body_url||*body_url!="https://caps.test/asset?bodypart_id="+
        std::string{ASSET})return 22;
    auto http=std::make_shared<TestViewerAsset>();
    http->fixture=asset;
    FirestormRegionLease lease;
    const auto epoch=lease.enter(1U,1U,"https://caps.test/seed");
    const auto via_cap=fetch_wearable_viewer_asset(
        http,"https://caps.test/asset",ASSET,asset_type_clothing,
        epoch,lease);
    if(!via_cap.asset||via_cap.asset->type!=4 ||
       via_cap.asset->name!="White Shirt"||
       via_cap.epoch!=epoch||via_cap.asset_id!=ASSET||
       !via_cap.capability_attempted||http->seen!=*url)return 23;
    const auto mismatched_category=fetch_wearable_viewer_asset(
        http,"https://caps.test/asset",ASSET,asset_type_bodypart,
        epoch,lease);
    if(mismatched_category.asset||
       mismatched_category.error.find("category")==std::string::npos)
        return 24;
    http->code=404;
    const auto missing=fetch_wearable_viewer_asset(
        http,"https://caps.test/asset",ASSET,asset_type_clothing,
        epoch,lease);
    if(missing.asset||!missing.capability_attempted ||
       missing.error.find("HTTP failure")==std::string::npos)
        return 25;
    http->code=200;http->fixture="invalid wearable binary";
    if(fetch_wearable_viewer_asset(
        http,"https://caps.test/asset",ASSET,asset_type_clothing,
        epoch,lease).asset)return 26;
    const auto next_epoch=lease.enter(2U,2U,"https://caps.test/new");
    (void)next_epoch;
    if(fetch_wearable_viewer_asset(
        http,"https://caps.test/asset",ASSET,asset_type_clothing,
        epoch,lease).capability_attempted)return 27;
    std::cout<<"Wearable LLUDP Low153 real Clothing/Bodypart transfer multi-packet, owner type/transfer/epoch isolation and malformed/timeout failure OK\n";
    return 0;
}
