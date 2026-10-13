/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace nexverse::assets
{
// Firestorm 7.2.4 LLKeyframeMotion serialized binary (MPL independent
// parser); C-string joint names and 1.0 U16-packed rotations/positions.
struct MotionQuat{float x=0,y=0,z=0,w=1;};
struct MotionVec3{float x=0,y=0,z=0;};
struct MotionRotationKey{float time=0;MotionQuat rotation{};};
struct MotionPositionKey{float time=0;MotionVec3 position{};};
struct MotionJoint
{
    std::string name;
    std::int32_t priority=0;
    std::vector<MotionRotationKey> rotations;
    std::vector<MotionPositionKey> positions;
};
struct FirestormKeyframeMotion
{
    std::int32_t priority=0;
    float duration=0,loop_in=0,loop_out=0,ease_in=0,ease_out=0;
    bool loop=false;
    std::uint32_t hand_pose=0;
    std::string emote;
    std::vector<MotionJoint> joints;
    // Constraints are conservatively unsupported. Malformed/constraint
    // populated clips are rejected, NOT silently used as full parity.
};
namespace detail_motion
{
struct Reader
{
    std::span<const std::uint8_t> bytes;
    std::size_t at=0;
    bool take(std::size_t n)const noexcept
    {return n<=bytes.size()-at;}
    std::optional<std::uint8_t> u8()
    {
        if(!take(1U))return std::nullopt;
        return bytes[at++];
    }
    std::optional<std::uint16_t> u16()
    {
        if(!take(2U))return std::nullopt;
        const auto v=static_cast<std::uint16_t>(
            bytes[at]|(static_cast<unsigned>(bytes[at+1U])<<8U));
        at+=2U;return v;
    }
    std::optional<std::uint32_t> u32()
    {
        if(!take(4U))return std::nullopt;
        std::uint32_t v=0U;
        for(unsigned i=0;i<4U;++i)
            v|=static_cast<std::uint32_t>(bytes[at+i])<<(i*8U);
        at+=4U;return v;
    }
    std::optional<std::int32_t> i32()
    {
        const auto n=u32();
        if(!n)return std::nullopt;
        return std::bit_cast<std::int32_t>(*n);
    }
    std::optional<float> f32()
    {
        const auto n=u32();
        if(!n)return std::nullopt;
        const float f=std::bit_cast<float>(*n);
        if(!std::isfinite(f))return std::nullopt;
        return f;
    }
    std::optional<std::string> cstring(std::size_t bound)
    {
        const auto start=at;
        while(at<bytes.size()&&at-start<=bound&&bytes[at]!=0U)
        {
            if(bytes[at]<32U||bytes[at]>126U)return std::nullopt;
            ++at;
        }
        if(!take(1U)||at-start>bound)return std::nullopt;
        std::string out{
            reinterpret_cast<const char*>(bytes.data()+start),at-start};
        ++at;return out;
    }
};
inline float unquant(std::uint16_t n,float lo,float hi)
{return lo+(hi-lo)*(static_cast<float>(n)/65535.0f);}
inline MotionQuat normalize(MotionQuat q)
{
    const float mag=std::sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w);
    if(!std::isfinite(mag)||mag<1e-6f)return {};
    return {q.x/mag,q.y/mag,q.z/mag,q.w/mag};
}
inline MotionQuat unpack_quat(float x,float y,float z)
{
    const float w=std::sqrt(std::max(0.0f,1.0f-x*x-y*y-z*z));
    return normalize({x,y,z,w});
}
inline MotionQuat slerp(MotionQuat a,MotionQuat b,float f)
{
    f=std::clamp(f,0.0f,1.0f);
    float dot=a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
    if(dot<0.0f)
    {dot=-dot;b={-b.x,-b.y,-b.z,-b.w};}
    if(dot>0.9995f)
        return normalize({a.x+(b.x-a.x)*f,a.y+(b.y-a.y)*f,
                          a.z+(b.z-a.z)*f,a.w+(b.w-a.w)*f});
    const float theta=std::acos(std::clamp(dot,-1.0f,1.0f));
    const float d=std::sin(theta);
    const float p=std::sin((1.0f-f)*theta)/d;
    const float q=std::sin(f*theta)/d;
    return normalize({p*a.x+q*b.x,p*a.y+q*b.y,
                      p*a.z+q*b.z,p*a.w+q*b.w});
}
}
[[nodiscard]] inline std::optional<FirestormKeyframeMotion>
decode_firestorm_keyframe(std::span<const std::uint8_t> bytes)
{
    using namespace detail_motion;
    if(bytes.size()<40U||bytes.size()>2U*1024U*1024U)
        return std::nullopt;
    Reader r{bytes};
    const auto version=r.u16(),sub=r.u16();
    if(!version||!sub||!((*version==1U&&*sub==0U)||
                          (*version==0U&&*sub==1U)))
        return std::nullopt;
    const bool old=*version==0U;
    FirestormKeyframeMotion clip;
    const auto priority=r.i32();
    const auto duration=r.f32();
    const auto emote=r.cstring(63U);
    const auto loop_in=r.f32();
    const auto loop_out=r.f32();
    const auto loop=r.i32();
    const auto ease_in=r.f32();
    const auto ease_out=r.f32();
    const auto hand=r.u32();
    const auto joints=r.u32();
    if(!priority||!duration||!emote||!loop_in||!loop_out||!loop||
       !ease_in||!ease_out||!hand||!joints ||
       *priority<0||*priority>7||*duration<=0.0f||*duration>600.0f||
       *loop_in<0.0f||*loop_out<*loop_in||*loop_out>*duration||
       *ease_in<0.0f||*ease_out<0.0f||
       *ease_in>120.0f||*ease_out>120.0f||
       *hand>64U||*joints==0U||*joints>256U)
        return std::nullopt;
    clip.priority=*priority;clip.duration=*duration;clip.emote=*emote;
    clip.loop_in=*loop_in;clip.loop_out=*loop_out;clip.loop=*loop!=0;
    clip.ease_in=*ease_in;clip.ease_out=*ease_out;clip.hand_pose=*hand;
    clip.joints.reserve(*joints);
    std::size_t total_keys=0U;
    for(std::uint32_t i=0U;i<*joints;++i)
    {
        const auto name=r.cstring(80U);
        const auto prio=r.i32();
        const auto nr=r.i32();
        if(!name||name->empty()||*name=="mRoot"||*name=="mScreen"||
           !prio||*prio<0||*prio>7||!nr||*nr<0||*nr>8192||
           total_keys+static_cast<unsigned>(*nr)>32768U)
            return std::nullopt;
        MotionJoint joint;
        joint.name=*name;joint.priority=*prio;
        for(const auto& existing:clip.joints)
            if(existing.name==joint.name)return std::nullopt;
        joint.rotations.reserve(static_cast<std::size_t>(*nr));
        total_keys+=static_cast<unsigned>(*nr);
        for(std::int32_t k=0;k<*nr;++k)
        {
            float time;
            MotionQuat q;
            if(old)
            {
                const auto t=r.f32(),x=r.f32(),y=r.f32(),z=r.f32();
                if(!t||!x||!y||!z)return std::nullopt;
                time=*t;
                // Legacy 0.1 stores Euler Maya ZYX radians.
                const float cx=std::cos(*x*0.5f),sx=std::sin(*x*0.5f);
                const float cy=std::cos(*y*0.5f),sy=std::sin(*y*0.5f);
                const float cz=std::cos(*z*0.5f),sz=std::sin(*z*0.5f);
                q=normalize({sx*cy*cz-cx*sy*sz,
                             cx*sy*cz+sx*cy*sz,
                             cx*cy*sz-sx*sy*cz,
                             cx*cy*cz+sx*sy*sz});
            }
            else
            {
                const auto t=r.u16(),x=r.u16(),y=r.u16(),z=r.u16();
                if(!t||!x||!y||!z)return std::nullopt;
                time=unquant(*t,0.0f,clip.duration);
                q=unpack_quat(unquant(*x,-1.0f,1.0f),
                              unquant(*y,-1.0f,1.0f),
                              unquant(*z,-1.0f,1.0f));
            }
            if(time<0||time>clip.duration||
               (!joint.rotations.empty()&&
                 time<=joint.rotations.back().time))
                return std::nullopt;
            joint.rotations.push_back({time,q});
        }
        const auto np=r.i32();
        if(!np||*np<0||*np>8192||
           total_keys+static_cast<unsigned>(*np)>32768U)
            return std::nullopt;
        total_keys+=static_cast<unsigned>(*np);
        joint.positions.reserve(static_cast<std::size_t>(*np));
        for(std::int32_t k=0;k<*np;++k)
        {
            float time;MotionVec3 v;
            if(old)
            {
                const auto t=r.f32(),x=r.f32(),y=r.f32(),z=r.f32();
                if(!t||!x||!y||!z)return std::nullopt;
                time=*t;v={*x,*y,*z};
            }
            else
            {
                const auto t=r.u16(),x=r.u16(),y=r.u16(),z=r.u16();
                if(!t||!x||!y||!z)return std::nullopt;
                time=unquant(*t,0.0f,clip.duration);
                v={unquant(*x,-5.0f,5.0f),
                   unquant(*y,-5.0f,5.0f),
                   unquant(*z,-5.0f,5.0f)};
            }
            if(time<0||time>clip.duration||
               (!joint.positions.empty()&&
                 time<=joint.positions.back().time)||
               std::abs(v.x)>5.01f||std::abs(v.y)>5.01f||
               std::abs(v.z)>5.01f)
                return std::nullopt;
            joint.positions.push_back({time,v});
        }
        clip.joints.push_back(std::move(joint));
    }
    // Parse constraint count; the extended collision-chain constraints
    // require a full LLCharacter contact system. Refuse them for now.
    const auto constraints=r.i32();
    if(!constraints||*constraints!=0||r.at!=bytes.size())
        return std::nullopt;
    return clip;
}
[[nodiscard]] inline MotionQuat sample_firestorm_rotation(
    const MotionJoint& joint,float seconds)
{
    const auto& keys=joint.rotations;
    if(keys.empty()||!std::isfinite(seconds))return {};
    if(seconds<=keys.front().time)return keys.front().rotation;
    if(seconds>=keys.back().time)return keys.back().rotation;
    const auto upper=std::upper_bound(
        keys.begin(),keys.end(),seconds,
        [](float t,const MotionRotationKey& k){return t<k.time;});
    const auto& a=*(upper-1);
    const auto& b=*upper;
    return detail_motion::slerp(a.rotation,b.rotation,
                               (seconds-a.time)/(b.time-a.time));
}
[[nodiscard]] inline MotionVec3 sample_firestorm_position(
    const MotionJoint& joint,float seconds)
{
    const auto& keys=joint.positions;
    if(keys.empty()||!std::isfinite(seconds))return {};
    if(seconds<=keys.front().time)return keys.front().position;
    if(seconds>=keys.back().time)return keys.back().position;
    const auto upper=std::upper_bound(
        keys.begin(),keys.end(),seconds,
        [](float t,const MotionPositionKey& k){return t<k.time;});
    const auto& a=*(upper-1);const auto& b=*upper;
    const float f=(seconds-a.time)/(b.time-a.time);
    return {a.position.x+(b.position.x-a.position.x)*f,
            a.position.y+(b.position.y-a.position.y)*f,
            a.position.z+(b.position.z-a.position.z)*f};
}
[[nodiscard]] inline float firestorm_clip_time(
    const FirestormKeyframeMotion& clip,float seconds)
{
    if(!std::isfinite(seconds)||seconds<0.0f)return 0.0f;
    if(!clip.loop)return std::min(seconds,clip.duration);
    if(seconds<=clip.loop_out)return std::min(seconds,clip.duration);
    const float span=clip.loop_out-clip.loop_in;
    return span>0.0f?clip.loop_in+
        std::fmod(seconds-clip.loop_out,span):clip.loop_out;
}
}
