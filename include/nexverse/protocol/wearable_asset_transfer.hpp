/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. */
#pragma once
#include "nexverse/protocol/animation_asset_transfer.hpp"
#include "nexverse/assets/firestorm_wearable_asset.hpp"
#include <memory>
namespace nexverse::protocol
{
inline constexpr std::int32_t asset_type_clothing=5;
inline constexpr std::int32_t asset_type_bodypart=13;
struct WearableAssetRequest
{
    std::string asset_id,transfer_id;
    std::int32_t asset_type=asset_type_clothing;
};
[[nodiscard]] inline std::vector<std::uint8_t> encode_wearable_asset_transfer(
    std::uint32_t sequence,const WearableAssetRequest& request)
{
    const auto id=uuid_to_bytes(request.asset_id);
    const auto tid=uuid_to_bytes(request.transfer_id);
    const auto valid=[](const auto& x){
        return x&&std::any_of(x->begin(),x->end(),
                             [](std::uint8_t v){return v!=0U;});
    };
    if(!valid(id)||!valid(tid)||
       (request.asset_type!=asset_type_clothing&&
        request.asset_type!=asset_type_bodypart))return {};
    std::vector<std::uint8_t> body;
    body.insert(body.end(),tid->begin(),tid->end());
    transfer_detail::append32(body,
        static_cast<std::uint32_t>(transfer_channel_asset));
    transfer_detail::append32(body,
        static_cast<std::uint32_t>(transfer_source_asset));
    transfer_detail::append32(body,std::bit_cast<std::uint32_t>(100.0f));
    body.push_back(20U);body.push_back(0U);
    body.insert(body.end(),id->begin(),id->end());
    transfer_detail::append32(body,
        static_cast<std::uint32_t>(request.asset_type));
    return encode_packet(packet_reliable,sequence,msg_transfer_request,body);
}
struct WearableTransferInfo
{
    std::string transfer_id,asset_id;
    std::int32_t status=0,size=0,type=0;
};
[[nodiscard]] inline std::optional<WearableTransferInfo>
decode_wearable_transfer_info(std::span<const std::uint8_t> bytes)
{
    if(bytes.size()!=54U||bytes[32U]!=20U||bytes[33U]!=0U||
       transfer_detail::i32(bytes,16U)!=transfer_channel_asset||
       transfer_detail::i32(bytes,20U)!=0)return std::nullopt;
    const auto type=transfer_detail::i32(bytes,50U);
    if(type!=asset_type_clothing&&type!=asset_type_bodypart)
        return std::nullopt;
    const auto status=transfer_detail::i32(bytes,24U);
    const auto size=transfer_detail::i32(bytes,28U);
    if(!((status==0&&size>0&&
           static_cast<std::size_t>(size)<=assets::max_firestorm_wearable_bytes)||
         (status==-2&&size==0)))return std::nullopt;
    return WearableTransferInfo{
        transfer_detail::uuid(bytes.first(16U)),
        transfer_detail::uuid(bytes.subspan(34U,16U)),status,size,type};
}
class WearableTransferInbox
{
public:
    struct Result
    {
        std::string asset_id;
        std::shared_ptr<const assets::FirestormWearableAsset> asset;
        std::string error;
    };
    void reset(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        epoch_=epoch; pending_.clear(); done_.clear();
    }
    [[nodiscard]] bool begin(std::uint64_t epoch,
                              const WearableAssetRequest& request)
    {
        const auto aid=uuid_to_bytes(request.asset_id);
        const auto tid=uuid_to_bytes(request.transfer_id);
        const auto nonzero=[](const auto& b) {
            return b&&std::any_of(b->begin(),b->end(),
                [](std::uint8_t n){return n!=0U;});
        };
        if(!nonzero(aid)||!nonzero(tid)||
           (request.asset_type!=asset_type_clothing&&
            request.asset_type!=asset_type_bodypart))return false;
        std::scoped_lock lock{mutex_};
        if(epoch_!=epoch||epoch==0U||pending_.size()>=4U||
           pending_.contains(request.transfer_id))return false;
        for(const auto& [id,entry]:pending_)
            if(entry.asset_id==request.asset_id)return false;
        pending_.emplace(request.transfer_id,Pending{request.asset_id,request.asset_type});
        return true;
    }
    [[nodiscard]] bool has_pending(std::string_view asset) const
    {
        std::scoped_lock lock{mutex_};
        for(const auto& [id,entry]:pending_)
            if(entry.asset_id==asset)return true;
        return false;
    }
    void on_info(const WearableTransferInfo& info)
    {
        std::scoped_lock lock{mutex_};
        const auto found=pending_.find(info.transfer_id);
        if(found==pending_.end())return;
        if(info.asset_id!=found->second.asset_id ||
           info.type!=found->second.asset_type)
        {
            reject(found,"TransferInfo asset/type mismatch");return;
        }
        if(info.status==-2)
        {
            reject(found,"Asset absent on simulator");return;
        }
        if(info.status!=0||info.size<=0||
           static_cast<std::size_t>(info.size)>assets::max_firestorm_wearable_bytes)
        {
            reject(found,"TransferInfo invalid size/status");return;
        }
        found->second.expected=static_cast<std::size_t>(info.size);
        finish_if_ready(found);
    }
    void on_packet(const AnimationTransferPacket& packet)
    {
        std::scoped_lock lock{mutex_};
        const auto found=pending_.find(packet.transfer_id);
        if(found==pending_.end())return;
        auto& pending=found->second;
        if(packet.index>4096U||packet.data.empty()||
           pending.size+packet.data.size()>assets::max_firestorm_wearable_bytes ||
           pending.packets.size()>=4096U)
        {
            reject(found,"TransferPacket bounded budget exceeded");return;
        }
        if(const auto seen=pending.packets.find(packet.index);
           seen!=pending.packets.end())
        {
            if(seen->second!=packet.data||
               (pending.last&&*pending.last==packet.index&&!packet.last))
                reject(found,"TransferPacket conflicting duplicate");
            return;
        }
        if(pending.last&&packet.index>*pending.last)
        {
            reject(found,"TransferPacket arrived beyond final");return;
        }
        if(packet.last)
        {
            if(pending.last&&*pending.last!=packet.index)
            {reject(found,"TransferPacket multiple finals");return;}
            pending.last=packet.index;
        }
        pending.size+=packet.data.size();
        pending.packets.emplace(packet.index,packet.data);
        finish_if_ready(found);
    }
    [[nodiscard]] std::optional<Result> pop()
    {
        std::scoped_lock lock{mutex_};
        if(done_.empty())return std::nullopt;
        auto result=std::move(done_.front());
        done_.erase(done_.begin());
        return result;
    }
    [[nodiscard]] std::size_t pending_count() const
    {
        std::scoped_lock lock{mutex_};
        return pending_.size();
    }
private:
    struct Pending
    {
        Pending(std::string id,std::int32_t type):asset_id(std::move(id)),asset_type(type){}
        std::int32_t asset_type=0;
        std::string asset_id;
        std::optional<std::size_t> expected;
        std::optional<std::uint32_t> last;
        std::map<std::uint32_t,std::vector<std::uint8_t>> packets;
        std::size_t size=0U;
    };
    using Iterator=std::map<std::string,Pending,std::less<>>::iterator;
    void reject(Iterator it,std::string message)
    {
        if(done_.size()>=16U)done_.erase(done_.begin());
        done_.push_back({it->second.asset_id,{},std::move(message)});
        pending_.erase(it);
    }
    void finish_if_ready(Iterator it)
    {
        auto& p=it->second;
        if(!p.expected||!p.last||p.size!=*p.expected||
           p.packets.size()!=static_cast<std::size_t>(*p.last)+1U)
            return;
        std::vector<std::uint8_t> data;
        data.reserve(p.size);
        for(std::uint32_t i=0U;i<=*p.last;++i)
        {
            const auto block=p.packets.find(i);
            if(block==p.packets.end())return;
            data.insert(data.end(),block->second.begin(),block->second.end());
        }
        const auto parsed=assets::parse_firestorm_wearable(
            std::string_view{reinterpret_cast<const char*>(data.data()),
                             data.size()},
            std::nullopt);
        // Original subtype comparison is performed against owner-fetched
        // inventory flags on the rendering main thread.
        if(done_.size()>=16U)done_.erase(done_.begin());
        done_.push_back({p.asset_id,
            parsed.asset?std::make_shared<const assets::FirestormWearableAsset>(
                std::move(*parsed.asset)):nullptr,
            parsed.ok()?std::string{}:parsed.error});
        pending_.erase(it);
    }
    mutable std::mutex mutex_;
    std::uint64_t epoch_=0U;
    std::map<std::string,Pending,std::less<>> pending_;
    std::vector<Result> done_;
};
}
