/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#pragma once
#include "nexverse/render/renderer.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <span>
#include <vector>

namespace nexverse::render
{
// Camera-facing continuous triangle ribbon. One vertex is 9 floats:
// world XYZ, RGBA and normalized UV. The caller bounds the sample count.
[[nodiscard]] inline std::vector<float> build_ribbon_triangles(
    std::span<const RenderVec3> points,
    RenderVec3 camera,
    float width,
    const std::array<float,4>& color)
{
    std::vector<float> triangles;
    if (points.size() < 2U || points.size() > 64U ||
        !std::isfinite(width) || width <= 0.0f ||
        width > 8.0f) return triangles;
    triangles.reserve((points.size() - 1U)*54U);
    const auto sub=[](RenderVec3 a,RenderVec3 b)
    {return RenderVec3{a.x-b.x,a.y-b.y,a.z-b.z};};
    const auto cross=[](RenderVec3 a,RenderVec3 b)
    {return RenderVec3{a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};};
    const auto norm=[](RenderVec3 a)
    {
        const float len=std::sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
        return len > 0.00001f ?
            RenderVec3{a.x/len,a.y/len,a.z/len}:RenderVec3{};
    };
    const auto append=[&](RenderVec3 p,float u,float v)
    {
        triangles.insert(triangles.end(),
            {p.x,p.y,p.z,color[0],color[1],color[2],color[3],u,v});
    };
    for(std::size_t i=0;i+1U<points.size();++i)
    {
        const auto a=points[i],b=points[i+1U];
        const auto forward=norm(sub(b,a));
        if (forward.x==0.0f && forward.y==0.0f && forward.z==0.0f)
            continue;
        RenderVec3 right=norm(cross(forward,norm(sub(camera,a))));
        if (right.x==0.0f && right.y==0.0f && right.z==0.0f)
            right=norm(cross(forward,{0.0f,0.0f,1.0f}));
        if (right.x==0.0f && right.y==0.0f && right.z==0.0f)
            right={1.0f,0.0f,0.0f};
        const float w=width*0.5f;
        const auto offset=RenderVec3{right.x*w,right.y*w,right.z*w};
        const auto a0=sub(a,offset);
        const auto a1=RenderVec3{a.x+offset.x,a.y+offset.y,a.z+offset.z};
        const auto b0=sub(b,offset);
        const auto b1=RenderVec3{b.x+offset.x,b.y+offset.y,b.z+offset.z};
        const float v0=static_cast<float>(i)/
            static_cast<float>(points.size()-1U);
        const float v1=static_cast<float>(i+1U)/
            static_cast<float>(points.size()-1U);
        append(a0,0.0f,v0);append(a1,1.0f,v0);append(b0,0.0f,v1);
        append(b0,0.0f,v1);append(a1,1.0f,v0);append(b1,1.0f,v1);
    }
    return triangles;
}
}
