#include "nexverse/render/renderer.hpp"
#include "nexverse/assets/gpu_staging_budget.hpp"
#include "nexverse/assets/gpu_staging_recovery.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    auto* window=glfwCreateWindow(360,220,"OGL Soak",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}
    using namespace nexverse::render;
    using namespace nexverse::assets;
    int fail=0;
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(360,220))fail=2;
        if(!fail)
        {
            RenderMeshGeometry geometry{
                {3.0f,-1.0f,-1.0f, 1.0f,0.0f,0.0f,0.0f,0.0f,
                 3.0f, 1.0f,-1.0f, 1.0f,0.0f,0.0f,1.0f,0.0f,
                 3.0f, 0.0f, 1.0f, 1.0f,0.0f,0.0f,0.5f,1.0f},
                {0U,1U,2U},{{0U,0U,3U}}};
            RenderImageRgba8 image{16U,16U,
                std::vector<std::uint8_t>(16U*16U*4U,230U)};
            std::vector<RenderSceneObject> objects;
            for(unsigned i=0U;i<24U;++i)
            {
                RenderSceneObject obj;
                obj.local_id=i+1U;
                obj.position={0.0f,(static_cast<float>(i%8U)-3.5f)*0.18f,
                    (static_cast<float>(i/8U)-1.0f)*0.18f};
                obj.mesh_id=i%6U==0U ? "never-resident-mesh" :
                    "soak-mesh-"+std::to_string(i);
                obj.texture_id="soak-texture-"+std::to_string(i%8U);
                RenderFaceMaterial material;
                material.texture_id=obj.texture_id;
                material.color={0.8f,0.7f,0.95f,
                    i%3U==0U?0.43f:1.0f};
                material.fullbright=i%4U==0U;
                material.shiny=static_cast<std::uint8_t>(i%3U);
                material.repeat_u=1.0f+static_cast<float>(i%3U);
                material.offset_v=static_cast<float>(i%5U)*0.05f;
                obj.face_materials[0]=material;
                objects.push_back(obj);
                if(i%6U!=0U &&
                   !renderer->set_mesh_asset(obj.mesh_id,geometry))
                    {fail=3;break;}
            }
            for(unsigned i=0U;i<8U&&!fail;++i)
            {
                for(std::size_t px=3U;px<image.pixels.size();px+=4U)
                    image.pixels[px]=i%2U==0U?255U:140U;
                if(!renderer->set_object_texture(
                    "soak-texture-"+std::to_string(i),image))
                    fail=4;
            }
            renderer->set_scene_objects(objects);
            renderer->set_camera({{0.0f,0.0f,0.0f},
                {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                {0.0f,0.0f,1.0f},60.0f,0.1f,65.0f});
            GpuStagingRecovery recovery;
            const auto t=GpuStagingRecovery::Clock::time_point{
                std::chrono::seconds{100}};
            const auto start=std::chrono::steady_clock::now();
            unsigned staged=0U,committed=0U;
            for(unsigned frame=0U;frame<160U&&!fail;++frame)
            {
                image.pixels[0]=static_cast<std::uint8_t>(frame);
                for(std::size_t px=3U;px<image.pixels.size();px+=4U)
                    image.pixels[px]=frame%2U==0U?255U:110U;
                // Exercise changing face tint/alpha without replacing geometry.
                auto& material=objects[frame%objects.size()].face_materials[0];
                if(material)
                    material->color[3]=frame%2U==0U?0.35f:1.0f;
                const auto tx="soak-texture-"+std::to_string(frame%8U);
                if(frame%4U==0U)
                {
                    const auto sizing=gpu_staging_budget(
                        13U*1024U*1024U, 2U*1024U*1024U,
                        1024U*1024U, 16U*1024U*1024U);
                    if(sizing.fits)fail=5;
                    recovery.defer(tx,t+std::chrono::seconds{frame},
                        13U*1024U*1024U,2U*1024U*1024U);
                    ++staged;
                    auto ready=recovery.ready(
                        t+std::chrono::seconds{frame+1U},
                        10U*1024U*1024U,2U*1024U*1024U);
                    if(ready.size()!=1U)fail=6;
                }
                const auto okay=gpu_staging_budget(
                    8U*1024U*1024U,2U*1024U*1024U,
                    1024U*1024U,16U*1024U*1024U);
                if(!okay.fits)fail=7;
                const auto upload=renderer->replace_object_texture(tx,image,{});
                if(!upload.success)fail=8;
                else
                {
                    ++committed;
                    recovery.resolved(tx);
                }
                if(frame%16U==0U)
                {
                    auto slot=1U+(frame/16U)%20U;
                    if(slot%6U==0U)++slot; // keep deliberate missing meshes missing
                    auto& obj=objects[slot];
                    renderer->remove_mesh_asset(obj.mesh_id);
                    if(!renderer->set_mesh_asset(obj.mesh_id,geometry))
                        fail=9;
                }
                renderer->set_scene_objects(objects);
                renderer->begin_frame();
                renderer->render_world(static_cast<double>(frame)*0.016);
                renderer->end_frame();
                const auto stats=renderer->diagnostics();
                if(stats.scene_mesh_draw_calls==0U ||
                   stats.scene_prim_draw_calls==0U ||
                   stats.scene_missing_mesh_objects!=4U ||
                   stats.scene_missing_mesh_draw_calls<
                       stats.scene_missing_mesh_objects ||
                   stats.scene_draw_calls!=
                       stats.scene_mesh_draw_calls+stats.scene_prim_draw_calls ||
                   glGetError()!=GL_NO_ERROR)
                {
                    fail=10;
                }
            }
            const auto elapsed=std::chrono::duration_cast<
                std::chrono::milliseconds>(
                    std::chrono::steady_clock::now()-start).count();
            if(!fail && (staged!=40U ||committed!=160U ||
                recovery.pending()!=0U))fail=11;
            std::cout<<"GL_SOAK frames=160 staging_events="<<staged
                <<" uploaded="<<committed<<" cpu_total_ms="<<elapsed<<'\n';
            renderer->clear_mesh_assets();
            renderer->clear_object_textures();
            renderer->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    if(fail){std::cerr<<"GL soak failed: "<<fail<<'\n';return fail;}
    return 0;
}
