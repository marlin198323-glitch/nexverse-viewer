/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/render/renderer.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>

namespace nexverse::render
{
// Conservative pick over server-provided root-prim bounding boxes.
// This is NOT yet exact sculpt/mesh hit testing, UV picking, or
// a Firestorm SurfaceInfo face result.
struct WorldPickObject
{
    std::uint32_t local_id=0U;
    RenderVec3 center{};
    RenderQuaternion rotation{};
    RenderVec3 size{1.0f,1.0f,1.0f};
};
struct WorldPickHit
{
    std::uint32_t local_id=0U;
    float distance=0.0f;
    RenderVec3 point{};
};
namespace detail_pick
{
inline float dot(RenderVec3 a,RenderVec3 b) noexcept
{return a.x*b.x+a.y*b.y+a.z*b.z;}
inline RenderVec3 add(RenderVec3 a,RenderVec3 b) noexcept
{return {a.x+b.x,a.y+b.y,a.z+b.z};}
inline RenderVec3 sub(RenderVec3 a,RenderVec3 b) noexcept
{return {a.x-b.x,a.y-b.y,a.z-b.z};}
inline RenderVec3 mul(RenderVec3 a,float n) noexcept
{return {a.x*n,a.y*n,a.z*n};}
inline RenderVec3 cross(RenderVec3 a,RenderVec3 b) noexcept
{return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
inline RenderVec3 unit(RenderVec3 a) noexcept
{
    const float l=std::sqrt(dot(a,a));
    return l>1e-7f&&std::isfinite(l)?mul(a,1.0f/l):RenderVec3{};
}
inline RenderVec3 unrotate(RenderQuaternion q,RenderVec3 v) noexcept
{
    const float l=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
    if(!(l>1e-8f)||!std::isfinite(l))return v;
    const float x=-q.x/std::sqrt(l),y=-q.y/std::sqrt(l),
                z=-q.z/std::sqrt(l),w=q.w/std::sqrt(l);
    const RenderVec3 t=mul(cross({x,y,z},v),2.0f);
    return add(v,add(mul(t,w),cross({x,y,z},t)));
}
inline float component(RenderVec3 v,int i) noexcept
{return i==0?v.x:i==1?v.y:v.z;}
}
[[nodiscard]] inline std::optional<WorldPickHit> pick_world_object(
    const RenderCamera& camera,int viewport_width,int viewport_height,
    float screen_x,float screen_y,std::span<const WorldPickObject> objects,
    float max_distance=40.0f) noexcept
{
    using namespace detail_pick;
    if(viewport_width<=0||viewport_height<=0||
       !std::isfinite(screen_x)||!std::isfinite(screen_y)||
       screen_x<0||screen_y<0||screen_x>=viewport_width||
       screen_y>=viewport_height||
       !(max_distance>0.0f)||!std::isfinite(max_distance)||
       !(camera.vertical_fov_degrees>1.0f)||
       !(camera.vertical_fov_degrees<179.0f))
        return std::nullopt;
    const auto forward=unit(camera.forward);
    const auto right=unit(cross(forward,camera.up));
    const auto up=unit(cross(right,forward));
    if(dot(forward,forward)<0.5f||dot(right,right)<0.5f)
        return std::nullopt;
    const float aspect=static_cast<float>(viewport_width)/
                       static_cast<float>(viewport_height);
    const float fov=std::tan(camera.vertical_fov_degrees*
                            0.008726646259971648f);
    const float nx=(2.0f*(screen_x+0.5f)/
                    static_cast<float>(viewport_width)-1.0f);
    const float ny=(1.0f-2.0f*(screen_y+0.5f)/
                    static_cast<float>(viewport_height));
    const auto ray=unit(add(forward,add(mul(right,nx*aspect*fov),
                                       mul(up,ny*fov))));
    std::optional<WorldPickHit> nearest;
    for(const auto& object:objects)
    {
        if(object.local_id==0U)continue;
        if(!std::isfinite(object.center.x)||!std::isfinite(object.center.y)||
           !std::isfinite(object.center.z)||
           !std::isfinite(object.size.x)||!std::isfinite(object.size.y)||
           !std::isfinite(object.size.z)||
           !std::isfinite(object.rotation.x)||
           !std::isfinite(object.rotation.y)||
           !std::isfinite(object.rotation.z)||
           !std::isfinite(object.rotation.w)||
           object.size.x<=0.0f||object.size.y<=0.0f||
           object.size.z<=0.0f)continue;
        // Validity and distance prefilter before expensive slab intersection.
        const auto center_delta=sub(object.center,camera.position);
        const float c2=dot(center_delta,center_delta);
        const float extent=0.5f*std::sqrt(
            object.size.x*object.size.x+
            object.size.y*object.size.y+
            object.size.z*object.size.z);
        if(!std::isfinite(c2)||!std::isfinite(extent)||
           c2>(max_distance+extent)*(max_distance+extent))continue;
        const auto relative_origin=unrotate(
            object.rotation,sub(camera.position,object.center));
        const auto local_ray=unrotate(object.rotation,ray);
        float entry=0.0f,exit=max_distance;
        bool valid=true;
        for(int axis=0;axis<3;++axis)
        {
            const float o=component(relative_origin,axis);
            const float direction=component(local_ray,axis);
            const float half=component(object.size,axis)*0.5f;
            if(std::abs(direction)<1e-7f)
            {
                if(o < -half||o > half){valid=false;break;}
                continue;
            }
            float near=(-half-o)/direction;
            float far=(half-o)/direction;
            if(near>far)std::swap(near,far);
            entry=std::max(entry,near);
            exit=std::min(exit,far);
            if(exit<entry){valid=false;break;}
        }
        if(!valid||entry<0.0f||entry>max_distance)continue;
        if(!nearest||entry<nearest->distance||
           (entry==nearest->distance&&object.local_id<nearest->local_id))
            nearest=WorldPickHit{object.local_id,entry,
                add(camera.position,mul(ray,entry))};
    }
    return nearest;
}
}
