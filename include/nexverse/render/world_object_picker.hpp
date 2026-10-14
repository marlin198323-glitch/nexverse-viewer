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
#include <memory>
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
    // Immutable, GPU-matching LOD triangles; when absent only the
    // conservative OBB can be used, and UI must say "approximate".
    std::shared_ptr<const RenderMeshGeometry> mesh;
};
struct WorldPickHit
{
    std::uint32_t local_id=0U;
    float distance=0.0f;
    RenderVec3 point{};
    bool exact_mesh=false;
    std::uint32_t face=0U;
    float u=0.0f,v=0.0f;
    RenderVec3 normal{};
    RenderVec3 binormal{};
    bool surface_ready=false;
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
inline bool finite(RenderVec3 v) noexcept
{return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
inline RenderVec3 div(RenderVec3 a,RenderVec3 b) noexcept
{return {a.x/b.x,a.y/b.y,a.z/b.z};}
inline RenderVec3 rotate(RenderQuaternion q,RenderVec3 v) noexcept
{
    const float l=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
    if(!(l>1e-8f)||!std::isfinite(l))return v;
    const float scale=1.0f/std::sqrt(l);
    const float x=q.x*scale,y=q.y*scale,z=q.z*scale,w=q.w*scale;
    const auto t=mul(cross({x,y,z},v),2.0f);
    return add(v,add(mul(t,w),cross({x,y,z},t)));
}
[[nodiscard]] inline std::optional<WorldPickHit> mesh_hit(
    const WorldPickObject& obj,RenderVec3 world_origin,RenderVec3 world_ray,
    float max_distance) noexcept
{
    const auto& mesh=*obj.mesh;
    // Interleaved xyz/nxyz/uv, standard GetMesh geometry, indexed faces.
    // Bounded to 65K triangles per single object-click; never iterate
    // untrusted million-triangle assets on the UI thread.
    if(mesh.vertices.size()%8U!=0U||
       mesh.indices.size()%3U!=0U||
       mesh.indices.size()>196608U||
       mesh.vertices.size()/8U>65536U)
        return std::nullopt;
    const auto origin=div(unrotate(obj.rotation,
                                  sub(world_origin,obj.center)),obj.size);
    const auto direction=div(unrotate(obj.rotation,world_ray),obj.size);
    float best=max_distance;
    std::optional<WorldPickHit> result;
    const auto n=mesh.vertices.size()/8U;
    for(std::size_t i=0U;i+2U<mesh.indices.size();i+=3U)
    {
        const auto ia=mesh.indices[i],ib=mesh.indices[i+1U],
                   ic=mesh.indices[i+2U];
        if(ia>=n||ib>=n||ic>=n)continue;
        const auto point=[&](std::uint32_t idx)
        {
            const auto at=8U*static_cast<std::size_t>(idx);
            return RenderVec3{mesh.vertices[at],mesh.vertices[at+1U],
                              mesh.vertices[at+2U]};
        };
        const auto a=point(ia),b=point(ib),c=point(ic);
        if(!finite(a)||!finite(b)||!finite(c))continue;
        const auto edge1=sub(b,a),edge2=sub(c,a);
        const auto p=cross(direction,edge2);
        const float det=dot(edge1,p);
        if(!std::isfinite(det)||std::abs(det)<1e-8f)continue;
        const float inv=1.0f/det;
        const auto tvec=sub(origin,a);
        const float bu=dot(tvec,p)*inv;
        if(!(bu>=0.0f&&bu<=1.0f))continue;
        const auto q=cross(tvec,edge1);
        const float bv=dot(direction,q)*inv;
        if(!(bv>=0.0f&&bu+bv<=1.0f))continue;
        const float dist=dot(edge2,q)*inv;
        if(!std::isfinite(dist)||dist<=1e-5f||dist>best)continue;
        const auto uv=[&](std::uint32_t index,int coord)
        {return mesh.vertices[8U*static_cast<std::size_t>(index)+
                              6U+static_cast<std::size_t>(coord)];};
        const float u=uv(ia,0)*(1.0f-bu-bv)+uv(ib,0)*bu+uv(ic,0)*bv;
        const float v=uv(ia,1)*(1.0f-bu-bv)+uv(ib,1)*bu+uv(ic,1)*bv;
        if(!std::isfinite(u)||!std::isfinite(v))continue;
        std::uint32_t face=0U;
        bool face_found=false;
        for(const auto& range:mesh.faces)
            if(i>=range.first_index &&
               i-range.first_index<range.index_count)
            {face=range.face;face_found=true;break;}
        const auto we1=rotate(obj.rotation,{
            edge1.x*obj.size.x,edge1.y*obj.size.y,edge1.z*obj.size.z});
        const auto we2=rotate(obj.rotation,{
            edge2.x*obj.size.x,edge2.y*obj.size.y,edge2.z*obj.size.z});
        auto normal=unit(cross(we1,we2));
        if(dot(normal,world_ray)>0.0f)normal=mul(normal,-1.0f);
        const float du1=uv(ib,0)-uv(ia,0);
        const float dv1=uv(ib,1)-uv(ia,1);
        const float du2=uv(ic,0)-uv(ia,0);
        const float dv2=uv(ic,1)-uv(ia,1);
        const float uvdet=du1*dv2-du2*dv1;
        RenderVec3 binormal{};
        if(std::isfinite(uvdet)&&std::abs(uvdet)>1e-8f)
        {
            const auto bitangent=mul(
                sub(mul(we2,du1),mul(we1,du2)),1.0f/uvdet);
            // Gram-Schmidt guarantees a unit vector perpendicular to
            // the geometric surface normal, even with scaled meshes.
            binormal=unit(sub(bitangent,mul(normal,dot(bitangent,normal))));
        }
        best=dist;
        const bool precise=face_found&&face<64U&&
            dot(normal,normal)>0.5f&&dot(binormal,binormal)>0.5f;
        result=WorldPickHit{obj.local_id,dist,
            add(world_origin,mul(world_ray,dist)),true,face,u,v,
            normal,binormal,precise};
    }
    return result;
}
}
[[nodiscard]] inline std::optional<WorldPickHit> pick_world_object(
    const RenderCamera& camera,int viewport_width,int viewport_height,
    float screen_x,float screen_y,std::span<const WorldPickObject> objects,
    float max_distance=40.0f) noexcept
{
    using namespace detail_pick;
    if(viewport_width<=0||viewport_height<=0||
       !std::isfinite(screen_x)||!std::isfinite(screen_y)||
       screen_x<0||screen_y<0||screen_x>=static_cast<float>(viewport_width)||
       screen_y>=static_cast<float>(viewport_height)||
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
        // If the exact resident mesh was provided, its triangles are
        // authoritative; do not fall back to OBB in empty mesh holes.
        const auto precise=object.mesh
            ?mesh_hit(object,camera.position,ray,max_distance)
            :std::optional<WorldPickHit>{};
        if(object.mesh&&!precise)continue;
        const WorldPickHit candidate=precise
            ?*precise:WorldPickHit{object.local_id,entry,
                add(camera.position,mul(ray,entry))};
        if(!nearest||candidate.distance<nearest->distance||
           (candidate.distance==nearest->distance&&
            object.local_id<nearest->local_id))
            nearest=candidate;
    }
    return nearest;
}
}
