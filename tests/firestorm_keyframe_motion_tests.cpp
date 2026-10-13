/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/assets/firestorm_keyframe_motion.hpp"
#include <bit>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
using Bytes=std::vector<std::uint8_t>;
static void u16(Bytes& b,std::uint16_t v)
{b.push_back(static_cast<std::uint8_t>(v));b.push_back(static_cast<std::uint8_t>(v>>8U));}
static void u32(Bytes& b,std::uint32_t v)
{
    for(unsigned i=0U;i<4U;++i)
        b.push_back(static_cast<std::uint8_t>(v>>(8U*i)));
}
static void f32(Bytes& b,float v)
{u32(b,std::bit_cast<std::uint32_t>(v));}
static void name(Bytes& b,const char* str)
{while(*str)b.push_back(static_cast<std::uint8_t>(*str++));b.push_back(0U);}
static Bytes fixture(bool old=false)
{
    Bytes b;
    u16(b,old?0U:1U);u16(b,old?1U:0U);
    u32(b,3U);f32(b,2.0f);name(b,"");
    f32(b,0.5f);f32(b,1.5f);u32(b,1U);
    f32(b,0.2f);f32(b,0.3f);u32(b,2U);u32(b,1U);
    name(b,"mKneeLeft");u32(b,4U);u32(b,2U);
    if(old)
    {
        f32(b,0.0f);f32(b,0.0f);f32(b,0.0f);f32(b,0.0f);
        f32(b,2.0f);f32(b,0.0f);f32(b,0.0f);f32(b,1.0f);
    }
    else
    {
        // At rest quaternion (0,0,0,1): x,y,z quantize at 32768.
        for(const std::uint16_t time:{std::uint16_t{0},std::uint16_t{65535}})
        {
            u16(b,time);u16(b,32768U);u16(b,32768U);
            u16(b,time==0U?32768U:49152U);
        }
    }
    u32(b,1U); // one position key
    if(old)
    {
        f32(b,0.0f);f32(b,0.0f);f32(b,0.0f);f32(b,0.0f);
    }
    else
    {
        u16(b,0U);u16(b,32768U);u16(b,32768U);u16(b,32768U);
    }
    u32(b,0U); // zero constraints: supported; IK not yet implemented.
    return b;
}
int main()
{
    using namespace nexverse::assets;
    const auto one=decode_firestorm_keyframe(fixture());
    if(!one||one->priority!=3||one->duration!=2.0f||!one->loop||
       one->loop_in!=0.5f||one->loop_out!=1.5f||
       one->joints.size()!=1U||one->joints[0].name!="mKneeLeft"||
       one->joints[0].rotations.size()!=2U||
       one->joints[0].positions.size()!=1U)return 1;
    const auto a=sample_firestorm_rotation(one->joints[0],0.0f);
    const auto middle=sample_firestorm_rotation(one->joints[0],1.0f);
    const auto end=sample_firestorm_rotation(one->joints[0],2.0f);
    if(!std::isfinite(middle.w)||std::abs(a.w-1.0f)>0.01f||
       !(middle.z>a.z&&middle.z<end.z)||
       std::abs(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z-1.0f)>0.02f)
        return 2;
    if(std::abs(firestorm_clip_time(*one,2.0f)-1.0f)>0.01f||
       std::abs(firestorm_clip_time(*one,1.0f)-1.0f)>0.01f)
        return 3;
    const auto old=decode_firestorm_keyframe(fixture(true));
    if(!old||std::abs(sample_firestorm_rotation(
        old->joints[0],2.0f).z)<0.1f)return 4;
    auto truncated=fixture();
    for(std::size_t i=0U;i<truncated.size();++i)
    {
        if(decode_firestorm_keyframe(std::span<const std::uint8_t>{
              truncated.data(),i}))return 5;
    }
    auto extra=fixture();extra.push_back(0xffU);
    if(decode_firestorm_keyframe(extra))return 6;
    auto bad=fixture();
    bad[0]=8U;
    if(decode_firestorm_keyframe(bad))return 7;
    bad=fixture();
    bad[8]=0xffU;bad[9]=0xffU;bad[10]=0x7fU;bad[11]=0x7fU;
    if(decode_firestorm_keyframe(bad))return 8;
    bad=fixture();
    bad[4]=0xffU;bad[5]=0xffU;bad[6]=0xffU;bad[7]=0xffU;
    if(decode_firestorm_keyframe(bad))return 9;
    bad=fixture();
    bad[bad.size()-4U]=1U; // one unrecognized constraint
    if(decode_firestorm_keyframe(bad))return 10;
    // Non-loop sampling clamped to duration and empty joint
    auto plain=*one;
    plain.loop=false;
    if(firestorm_clip_time(plain,50.0f)!=2.0f||
       sample_firestorm_rotation(MotionJoint{},2.0f).w!=1.0f)
        return 11;
    if(std::abs(sample_firestorm_position(one->joints[0],1.0f).x)>
       0.0005f)return 12;
    std::cout<<"Firestorm LLKeyframeMotion v1.0/0.1 binary decoder and interpolated curves OK\n";
    return 0;
}
