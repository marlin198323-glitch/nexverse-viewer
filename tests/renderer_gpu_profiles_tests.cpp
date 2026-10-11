#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace
{
using Clock=std::chrono::steady_clock;
nexverse::render::RenderMeshGeometry grid(unsigned side)
{
    nexverse::render::RenderMeshGeometry mesh;
    // OpenSim-like mix of moderate and dense triangle meshes.
    mesh.vertices.reserve(static_cast<std::size_t>(side+1U)*(side+1U)*8U);
    mesh.indices.reserve(static_cast<std::size_t>(side)*side*6U);
    for(unsigned y=0U;y<=side;++y)
        for(unsigned x=0U;x<=side;++x)
        {
            const float fx=static_cast<float>(x)/static_cast<float>(side);
            const float fy=static_cast<float>(y)/static_cast<float>(side);
            const float h=(x+y)%2U==0U?0.05f:-0.05f;
            mesh.vertices.insert(mesh.vertices.end(),
                {3.5f+h,fx-0.5f,fy-0.5f,
                    1.0f,0.7f,0.6f,fx,fy});
        }
    for(unsigned y=0U;y<side;++y)
        for(unsigned x=0U;x<side;++x)
        {
            const auto a=y*(side+1U)+x;
            const auto b=a+1U;
            const auto c=a+side+1U;
            const auto e=c+1U;
            mesh.indices.insert(mesh.indices.end(),
                {a,b,c,b,e,c});
        }
    mesh.faces.push_back({0U,0U,
        static_cast<std::uint32_t>(mesh.indices.size())});
    return mesh;
}
}
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    auto* window=glfwCreateWindow(400,240,"OGL Upload Profile",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}
    using namespace nexverse::render;
    int failed=0;
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(400,240))failed=2;
        if(!failed)
        {
            struct Profile{unsigned tex_side;unsigned mesh_side;unsigned tex_count;unsigned mesh_count;};
            constexpr Profile profiles[]{
                {64U,16U,40U,8U},
                {256U,32U,20U,8U},
                {1024U,64U,6U,4U}};
            unsigned local_id=1U;
            for(const auto& profile:profiles)
            {
                const std::size_t bytes=static_cast<std::size_t>(
                    profile.tex_side)*profile.tex_side*4U;
                std::vector<std::uint8_t> pixels(bytes);
                for(std::size_t i=0;i<bytes;i+=4U)
                {
                    pixels[i]=static_cast<std::uint8_t>((i/4U)%251U);
                    pixels[i+1U]=static_cast<std::uint8_t>((i/8U)%255U);
                    pixels[i+2U]=122U;
                    pixels[i+3U]=255U;
                }
                RenderImageRgba8 texture{
                    profile.tex_side,profile.tex_side,std::move(pixels)};
                auto mesh=grid(profile.mesh_side);
                std::uint64_t texture_cpu_us=0U;
                std::uint64_t mesh_cpu_us=0U;
                std::vector<RenderSceneObject> scene;
                for(unsigned i=0U;i<profile.tex_count;++i)
                {
                    const auto id="profile-texture-"+std::to_string(
                        profile.tex_side)+"-"+std::to_string(i);
                    const auto started=Clock::now();
                    const auto upload=renderer->replace_object_texture(id,texture,{});
                    texture_cpu_us+=static_cast<std::uint64_t>(
                        std::chrono::duration_cast<std::chrono::microseconds>(
                            Clock::now()-started).count());
                    if(!upload.success){failed=3;break;}
                }
                if(failed)break;
                for(unsigned i=0U;i<profile.mesh_count;++i)
                {
                    const auto id="profile-mesh-"+std::to_string(
                        profile.mesh_side)+"-"+std::to_string(i);
                    const auto started=Clock::now();
                    const auto upload=renderer->upload_mesh_asset(id,mesh);
                    mesh_cpu_us+=static_cast<std::uint64_t>(
                        std::chrono::duration_cast<std::chrono::microseconds>(
                            Clock::now()-started).count());
                    if(!upload.success){failed=4;break;}
                    RenderSceneObject object;
                    object.local_id=local_id++;
                    object.mesh_id=id;
                    object.texture_id="profile-texture-"+std::to_string(
                        profile.tex_side)+"-0";
                    object.position={0.0f,static_cast<float>(i)*0.08f,0.0f};
                    scene.push_back(object);
                }
                if(failed)break;
                renderer->set_scene_objects(scene);
                renderer->set_camera({{0.0f,0.0f,0.0f},
                    {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                    {0.0f,0.0f,1.0f},60.0f,0.1f,80.0f});
                renderer->begin_frame();
                renderer->render_world(1.0);
                renderer->end_frame();
                const auto stats=renderer->diagnostics();
                if(stats.visible_objects==0U || stats.draw_calls==0U ||
                   glGetError()!=GL_NO_ERROR){failed=5;break;}
                std::cout<<"PROFILE texture_side="<<profile.tex_side
                    <<" textures="<<profile.tex_count
                    <<" avg_texture_submit_cpu_us="<<
                    texture_cpu_us/profile.tex_count
                    <<" mesh_grid_side="<<profile.mesh_side
                    <<" meshes="<<profile.mesh_count
                    <<" avg_mesh_submit_cpu_us="<<
                    mesh_cpu_us/profile.mesh_count<<'\n';
            }
            renderer->clear_object_textures();
            renderer->clear_mesh_assets();
            renderer->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    if(failed){std::cerr<<"Upload profiles test failed: "<<failed<<'\n';return failed;}
    std::cout<<"Mixed 64/256/1024 RGBA and grid16/32/64 GPU profiles OK\n";
    return 0;
}
