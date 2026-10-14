/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 */
#include "nexverse/render/world_object_picker.hpp"
#include <array>
#include <cmath>
#include <iostream>
int main()
{
    using namespace nexverse::render;
    RenderCamera camera;
    camera.position={0.0f,0.0f,0.0f};
    camera.forward={1.0f,0.0f,0.0f};
    camera.up={0.0f,0.0f,1.0f};
    std::array<WorldPickObject,3> objects{{
        {10U,{8.0f,0.0f,0.0f},{0,0,0,1},{2,2,2}},
        {20U,{4.0f,0.0f,0.0f},{0,0,0,1},{2,2,2}},
        {30U,{10.0f,-5.0f,0.0f},{0,0,0,1},{1,1,1}}
    }};
    const auto hit=pick_world_object(camera,800,600,399.5f,299.5f,objects);
    if(!hit||hit->local_id!=20U||std::abs(hit->distance-3.0f)>1e-3f)
        return 1;
    const auto shifted=pick_world_object(camera,800,600,399.5f,299.5f,
        std::span<const WorldPickObject>{objects}.first(1));
    if(!shifted||shifted->local_id!=10U||
       std::abs(shifted->distance-7.0f)>1e-3f)return 2;
    for(const std::array<float,2> screen:std::array<std::array<float,2>,4>{
        std::array<float,2>{-1.0f,10.0f},
        {800.0f,30.0f},{10.0f,-1.0f},{10.0f,600.0f}})
        if(pick_world_object(camera,800,600,screen[0],screen[1],objects))
            return 3;
    if(pick_world_object(camera,0,600,100,100,objects))return 4;
    if(pick_world_object(camera,800,600,399.5f,299.5f,objects,1.0f))
        return 5;
    objects[1].local_id=0U;
    if(pick_world_object(camera,800,600,399.5f,299.5f,objects)->local_id!=10U)
        return 6;
    // Rotated box: 90 degrees about world Z, a thin 4m X-axis
    // local cuboid becomes a wide world-Y cuboid.
    objects[0]={11U,{8,0,0},{0,0,0.70710678f,0.70710678f},{4,0.4f,2}};
    const auto rotated=pick_world_object(camera,800,600,399.5f,299.5f,
                                      std::span{objects}.first(1));
    if(!rotated||rotated->local_id!=11U||
       std::abs(rotated->distance-7.8f)>0.02f)return 7;
    objects[0].size={-1,2,2};
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 8;
    objects[0].size={2,2,2};
    objects[0].center.x=std::numeric_limits<float>::quiet_NaN();
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 9;
    // Aspects and vertical FOV must not cause center drift.
    objects[0]={40U,{8,0,0},{0,0,0,1},{2,2,2}};
    for(const std::array<int,2> wh:std::array<std::array<int,2>,3>{
        std::array<int,2>{1280,720},{3440,1440},{5120,1440}})
    {
        const auto center=pick_world_object(camera,wh[0],wh[1],
               wh[0]*0.5f-0.5f,wh[1]*0.5f-0.5f,
               std::span{objects}.first(1));
        if(!center||center->local_id!=40U)return 10;
    }
    // Behind-camera bounding box must not receive selection.
    objects[0].center={-5,0,0};
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 11;
    // Real resident GetMesh interleaved XYZ/nXYZ/UV triangle on
    // the world-front face (x=-0.5), with barycentric UV/face readback.
    auto mesh=std::make_shared<RenderMeshGeometry>();
    mesh->vertices={
        -0.5f,-0.5f,-0.5f, 1,0,0, 0,0,
        -0.5f, 0.5f,-0.5f, 1,0,0, 1,0,
        -0.5f, 0.0f, 0.5f, 1,0,0, 0.5f,1
    };
    mesh->indices={0,1,2};
    mesh->faces={{7U,0U,3U}};
    objects[0]={90U,{8,0,0},{0,0,0,1},{2,2,2},mesh};
    const auto exact=pick_world_object(camera,800,600,
             399.5f,299.5f,std::span{objects}.first(1));
    if(!exact||!exact->exact_mesh||exact->face!=7U||
       exact->local_id!=90U||
       std::abs(exact->distance-7.0f)>1e-4f||
       std::abs(exact->u-0.5f)>1e-4f||
       std::abs(exact->v-0.5f)>1e-4f)return 12;
    // Behind the triangle inside the cube bounding box is NOT
    // acceptable: exact mesh wins and no phantom OBB pick.
    mesh->vertices[2]=1.0f;
    mesh->vertices[10]=1.0f;
    mesh->vertices[18]=1.0f;
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 13;
    mesh->vertices[2]=-0.5f;
    mesh->vertices[10]=-0.5f;
    mesh->vertices[18]=0.5f;
    mesh->indices[2]=888888U; // corrupt decoder/index
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 14;
    mesh->indices[2]=2U;
    mesh->vertices[0]=std::numeric_limits<float>::quiet_NaN();
    if(pick_world_object(camera,800,600,399.5f,299.5f,
                         std::span{objects}.first(1)))return 15;
    std::cout<<"World OBB + exact loaded-mesh face/UV picker OK\n";
    return 0;
}
