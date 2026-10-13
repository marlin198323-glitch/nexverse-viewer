/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/assets/firestorm_keyframe_motion.hpp"
#include <array>
#include <bit>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <random>
#include <span>
#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace nexverse::protocol
{
// Firestorm message_template.msg: TransferRequest Low153 (viewer->sim),
// TransferInfo Low154 and TransferPacket High17 (sim->viewer).
inline constexpr MessageId msg_transfer_request{MessageFrequency::low,153U};
inline constexpr MessageId msg_transfer_info{MessageFrequency::low,154U};
inline constexpr MessageId msg_transfer_packet{MessageFrequency::high,17U};
inline constexpr std::int32_t transfer_channel_asset=2;
inline constexpr std::int32_t transfer_source_asset=2;
inline constexpr std::int32_t asset_type_animation=20;
inline constexpr std::size_t max_animation_asset_bytes=2U*1024U*1024U;

struct AnimationAssetRequest
{
    std::string asset_id;
    std::string transfer_id;
};
// TransferID must not be the asset UUID: simultaneous client sessions
// and repeated transfers share asset IDs but require distinct reply IDs.
[[nodiscard]] inline std::string random_animation_transfer_id()
{
    std::random_device rd;
    std::array<std::uint8_t,16U> bytes{};
    for(auto& byte:bytes)
        byte=static_cast<std::uint8_t>(rd());
    bytes[6U]=static_cast<std::uint8_t>(
        (bytes[6U]&0x0fU)|0x40U);
    bytes[8U]=static_cast<std::uint8_t>(
        (bytes[8U]&0x3fU)|0x80U);
    static constexpr char hex[]="0123456789abcdef";
    std::string result;
    for(std::size_t i=0U;i<bytes.size();++i)
    {
        if(i==4U||i==6U||i==8U||i==10U)result.push_back('-');
        result.push_back(hex[bytes[i]>>4U]);
        result.push_back(hex[bytes[i]&0x0fU]);
    }
    return result;
}

namespace transfer_detail
{
inline std::uint32_t u32(std::span<const std::uint8_t> b,std::size_t o)
{
    return static_cast<std::uint32_t>(b[o])|
          (static_cast<std::uint32_t>(b[o+1U])<<8U)|
          (static_cast<std::uint32_t>(b[o+2U])<<16U)|
          (static_cast<std::uint32_t>(b[o+3U])<<24U);
}
inline std::int32_t i32(std::span<const std::uint8_t> b,std::size_t o)
{return std::bit_cast<std::int32_t>(u32(b,o));}
inline void append32(std::vector<std::uint8_t>& b,std::uint32_t v)
{
    for(unsigned i=0U;i<4U;++i)
        b.push_back(static_cast<std::uint8_t>(v>>(8U*i)));
}
inline std::string uuid(std::span<const std::uint8_t> b)
{
    static constexpr char hex[]="0123456789abcdef";
    std::string result;
    if(b.size()<16U)return result;
    for(std::size_t i=0U;i<16U;++i)
    {
        if(i==4U||i==6U||i==8U||i==10U)result.push_back('-');
        result.push_back(hex[b[i]>>4U]);
        result.push_back(hex[b[i]&0x0fU]);
    }
    return result;
}
}
[[nodiscard]] inline std::vector<std::uint8_t> encode_animation_asset_transfer(
    std::uint32_t sequence,const AnimationAssetRequest& request)
{
    const auto tid=uuid_to_bytes(request.transfer_id);
    const auto aid=uuid_to_bytes(request.asset_id);
    if(!tid||!aid||request.transfer_id==
       "00000000-0000-0000-0000-000000000000"||
       request.asset_id=="00000000-0000-0000-0000-000000000000")
        return {};
    std::vector<std::uint8_t> b;
    b.insert(b.end(),tid->begin(),tid->end());
    transfer_detail::append32(b,static_cast<std::uint32_t>(transfer_channel_asset));
    transfer_detail::append32(b,static_cast<std::uint32_t>(transfer_source_asset));
    transfer_detail::append32(b,std::bit_cast<std::uint32_t>(100.0f));
    b.push_back(20U);b.push_back(0U);
    b.insert(b.end(),aid->begin(),aid->end());
    transfer_detail::append32(b,static_cast<std::uint32_t>(asset_type_animation));
    return encode_packet(packet_reliable,sequence,msg_transfer_request,b);
}
struct AnimationTransferInfo
{
    std::string transfer_id;
    std::int32_t status=0;
    std::int32_t size=0;
    std::string asset_id;
    std::int32_t type=0;
};
[[nodiscard]] inline std::optional<AnimationTransferInfo>
decode_animation_transfer_info(std::span<const std::uint8_t> b)
{
    // TransferID(16) Channel(4) TargetType(4) Status(4)
    // Size(4) Params(2 + either 0 or assetUUID(16)+type(4))
    if(b.size()<34U||b.size()>256U)return std::nullopt;
    const auto len=static_cast<std::size_t>(b[32U])|
                   (static_cast<std::size_t>(b[33U])<<8U);
    if(len!=b.size()-34U||len!=20U)return std::nullopt;
    if(transfer_detail::i32(b,16U)!=transfer_channel_asset||
       transfer_detail::i32(b,20U)!=0)return std::nullopt;
    AnimationTransferInfo info;
    info.transfer_id=transfer_detail::uuid(b.first(16U));
    info.status=transfer_detail::i32(b,24U);
    info.size=transfer_detail::i32(b,28U);
    info.asset_id=transfer_detail::uuid(b.subspan(34U,16U));
    info.type=transfer_detail::i32(b,50U);
    // OpenGenesisLINK LLClientView.SendAssetNotFound replies with
    // Status=-2, Size=0 and echo of request Params (asset + type).
    if(info.type!=asset_type_animation)return std::nullopt;
    if(info.status==-2&&info.size==0)return info;
    if(info.status!=0||info.size<=0||
       static_cast<std::size_t>(info.size)>max_animation_asset_bytes)
        return std::nullopt;
    return info;
}
struct AnimationTransferPacket
{
    std::string transfer_id;
    std::uint32_t index=0U;
    bool last=false;
    std::vector<std::uint8_t> data;
};
[[nodiscard]] inline std::optional<AnimationTransferPacket>
decode_animation_transfer_packet(std::span<const std::uint8_t> b)
{
    if(b.size()<30U||b.size()>lludp_max_payload)return std::nullopt;
    const auto n=static_cast<std::size_t>(b[28U])|
                 (static_cast<std::size_t>(b[29U])<<8U);
    if(n==0U||n!=b.size()-30U)return std::nullopt;
    if(transfer_detail::i32(b,16U)!=transfer_channel_asset)return std::nullopt;
    const auto idx=transfer_detail::i32(b,20U);
    const auto status=transfer_detail::i32(b,24U);
    if(idx<0||idx>4096||!(status==0||status==1))return std::nullopt;
    AnimationTransferPacket packet;
    packet.transfer_id=transfer_detail::uuid(b.first(16U));
    packet.index=static_cast<std::uint32_t>(idx);
    packet.last=status==1;
    packet.data.assign(b.begin()+30,b.end());
    return packet;
}
// Bounded epoch-scoped assembler. Network events arrive from the
// circuit thread, snapshots are read on the main render thread.
class AnimationTransferInbox
{
public:
    struct Result
    {
        std::string asset_id;
        std::shared_ptr<const assets::FirestormKeyframeMotion> clip;
        std::string error;
    };
    void reset(std::uint64_t epoch)
    {
        std::scoped_lock lock{mutex_};
        epoch_=epoch; pending_.clear(); done_.clear();
    }
    [[nodiscard]] bool begin(std::uint64_t epoch,
                              const AnimationAssetRequest& request)
    {
        if(!uuid_to_bytes(request.asset_id)||
           !uuid_to_bytes(request.transfer_id))return false;
        std::scoped_lock lock{mutex_};
        if(epoch_!=epoch||epoch==0U||pending_.size()>=4U||
           pending_.contains(request.transfer_id))return false;
        for(const auto& [id,entry]:pending_)
            if(entry.asset_id==request.asset_id)return false;
        pending_.emplace(request.transfer_id,Pending{request.asset_id});
        return true;
    }
    [[nodiscard]] bool has_pending(std::string_view asset) const
    {
        std::scoped_lock lock{mutex_};
        for(const auto& [id,entry]:pending_)
            if(entry.asset_id==asset)return true;
        return false;
    }
    void on_info(const AnimationTransferInfo& info)
    {
        std::scoped_lock lock{mutex_};
        const auto found=pending_.find(info.transfer_id);
        if(found==pending_.end())return;
        if(info.asset_id!=found->second.asset_id ||
           info.type!=asset_type_animation)
        {
            reject(found,"TransferInfo asset/type mismatch");return;
        }
        if(info.status==-2)
        {
            reject(found,"Asset absent on simulator");return;
        }
        if(info.status!=0||info.size<=0||
           static_cast<std::size_t>(info.size)>max_animation_asset_bytes)
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
           pending.size+packet.data.size()>max_animation_asset_bytes ||
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
        explicit Pending(std::string id):asset_id(std::move(id)){}
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
        auto clip=assets::decode_firestorm_keyframe(data);
        if(done_.size()>=16U)done_.erase(done_.begin());
        done_.push_back({p.asset_id,
            clip?std::make_shared<const assets::FirestormKeyframeMotion>(
                    std::move(*clip)):nullptr,
            clip?std::string{}:std::string{"Invalid LLKeyframeMotion asset"}});
        pending_.erase(it);
    }
    mutable std::mutex mutex_;
    std::uint64_t epoch_=0U;
    std::map<std::string,Pending,std::less<>> pending_;
    std::vector<Result> done_;
};
}
