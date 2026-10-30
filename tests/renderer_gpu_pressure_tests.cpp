#include "nexverse/render/renderer.hpp"
#include "nexverse/assets/mesh_residency_target.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    if (!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window=glfwCreateWindow(360,220,"OGL GPU capacity test",nullptr,nullptr);
    if (!window) {glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(
            reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}

    using namespace nexverse::render;
    int error=0;
    const auto stress_started=std::chrono::steady_clock::now();
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(360,220))error=2;
        if(!error)
        {
            RenderImageRgba8 texture{2U,2U,std::vector<std::uint8_t>(16U,255U)};
            RenderMeshGeometry mesh{
                {4.0f,-1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
                 4.0f, 1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
                 4.0f, 0.0f, 1.0f,1.0f,0.0f,0.0f,0.5f,1.0f},
                {0U,1U,2U},{{0U,0U,3U}}};
            std::vector<RenderSceneObject> scene;
            scene.reserve(32U);
            for(std::size_t n=0;n<512U;++n)
            {
                const auto id="tex-"+std::to_string(n);
                if(!renderer->set_object_texture(id,texture))
                {error=3;break;}
                if(n<nexverse::assets::max_mesh_resident_assets)
                {
                    const auto mesh_id="mesh-"+std::to_string(n);
                    if(!renderer->set_mesh_asset(mesh_id,mesh))
                    {error=4;break;}
                    if(n<32U)
                    {
                        RenderSceneObject obj;
                        obj.local_id=static_cast<std::uint32_t>(n+1U);
                        obj.mesh_id=mesh_id;
                        obj.texture_id=id;
                        obj.position={0.0f,static_cast<float>(n%8U)*0.15f,
                            static_cast<float>(n/8U)*0.15f};
                        scene.push_back(obj);
                    }
                }
                if(n==15U || n==127U || n==255U || n==511U)
                {
                    renderer->set_scene_objects(scene);
                    renderer->set_camera({{0.0f,0.0f,0.0f},
                        {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                        {0.0f,0.0f,1.0f},60.0f,0.1f,80.0f});
                    renderer->begin_frame();
                    renderer->render_world(1.0);
                    renderer->end_frame();
                    const auto health=renderer->diagnostics();
                    if(health.visible_objects==0U || health.draw_calls==0U ||
                       health.scene_mesh_draw_calls==0U ||
                       health.scene_draw_calls!=
                           health.scene_mesh_draw_calls+
                               health.scene_prim_draw_calls||
                       health.scene_textured_draw_calls==0U||
                       health.scene_requested_texture_fallback_draw_calls!=0U)
                    {error=5;break;}
                    if(glGetError()!=GL_NO_ERROR){error=6;break;}
                }
            }
            if(!error)
            {
                // Distinguish a resident-but-not-bound or unavailable
                // texture from a genuinely textured OpenGL draw. The
                // present scene references tex-0, so removing just it
                // must produce a real requested-texture fallback draw.
                renderer->remove_object_texture("tex-0");
                renderer->begin_frame();
                renderer->render_world(2.0);
                renderer->end_frame();
                const auto missing_tex=renderer->diagnostics();
                if(missing_tex.scene_requested_texture_fallback_draw_calls==0U||
                   missing_tex.scene_textured_draw_calls==0U)
                    error=27;
                if(!renderer->set_object_texture("tex-0",texture))
                    error=28;
                renderer->begin_frame();
                renderer->render_world(3.0);
                renderer->end_frame();
                const auto recovered=renderer->diagnostics();
                if(recovered.scene_requested_texture_fallback_draw_calls!=0U||
                   recovered.scene_textured_draw_calls==0U)
                    error=29;
                if(renderer->set_object_texture("tex-over",texture))
                    error=7;
                const auto mesh_full=renderer->upload_mesh_asset("mesh-over",mesh);
                if(mesh_full.success ||
                   mesh_full.failure!=GpuUploadFailure::capacity)error=8;
                // A rejected replacement cannot corrupt a loaded resource.
                RenderImageRgba8 invalid;
                if(renderer->set_object_texture("tex-0",invalid))error=9;
                RenderMeshGeometry invalid_mesh=mesh;
                invalid_mesh.indices={0U,1U,900U};
                const auto bad_mesh=renderer->upload_mesh_asset(
                    "mesh-0",invalid_mesh);
                if(bad_mesh.success ||
                   bad_mesh.failure!=GpuUploadFailure::invalid_input)
                    error=10;
                if(!renderer->set_object_texture("tex-0",texture))
                    error=11;
                if(!renderer->set_mesh_asset("mesh-0",mesh))
                    error=12;
                // A failed transaction must not remove the victim in a
                // fully occupied table (512 slots).
                auto failed=renderer->replace_object_texture(
                    "tex-transaction",invalid,{"tex-511"});
                if(failed.success ||
                   failed.failure!=GpuUploadFailure::invalid_input)
                    error=16;
                if(renderer->set_object_texture("tex-transaction",texture))
                    error=17; // tex-511 must still occupy its slot.
                const auto full=renderer->replace_object_texture(
                    "tex-transaction",texture,{});
                if(full.success || full.failure!=GpuUploadFailure::capacity)
                    error=18;
                // An incoming ID cannot self-evict; no side effects.
                const auto self=renderer->replace_object_texture(
                    "tex-transaction",texture,{"tex-transaction"});
                if(self.success ||
                   self.failure!=GpuUploadFailure::invalid_input)
                    error=19;
                GLint max_texture_side=0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max_texture_side);
                if(max_texture_side>0 && max_texture_side<65536)
                {
                    const std::size_t beyond=
                        static_cast<std::size_t>(max_texture_side)+1U;
                    RenderImageRgba8 oversized{
                        static_cast<std::uint32_t>(beyond),1U,
                        std::vector<std::uint8_t>(beyond*4U,255U)};
                    const auto oversize=renderer->replace_object_texture(
                        "tex-transaction",oversized,{"tex-511"});
                    if(oversize.success ||
                       oversize.failure!=GpuUploadFailure::unsupported_dimensions)
                        error=20;
                    if(renderer->set_object_texture("tex-transaction",texture))
                        error=21; // rollback still holds tex-511
                }
                if(!error)
                {
                    const auto commit=renderer->replace_object_texture(
                        "tex-transaction",texture,{"tex-511","tex-511"});
                    if(!commit.success || commit.failure!=GpuUploadFailure::none)
                        error=22;
                    // The committed transaction evicted exactly one slot.
                    else if(renderer->set_object_texture("tex-511",texture))
                        error=23;
                    else if(!renderer->set_object_texture(
                        "tex-transaction",texture))
                        error=24; // new UUID remains resident
                }
                renderer->remove_object_texture("tex-transaction");
                // Reproduce the live-grid regression: planner requested
                // 384 distinct meshes while the previous renderer
                // rejected every mesh from 257 onward with capacity=0x0.
                // A small mesh must fit through 384 slots, but the 385th
                // is still rejected independent of GPU brand/VRAM size.
                const auto overflow=renderer->upload_mesh_asset(
                    "mesh-over-limit",mesh);
                if(overflow.success||
                   overflow.failure!=GpuUploadFailure::capacity)
                    error=25;
                renderer->remove_mesh_asset(
                    "mesh-"+std::to_string(
                        nexverse::assets::max_mesh_resident_assets-1U));
                if(!renderer->set_mesh_asset("mesh-reused",mesh))
                    error=26;
                if(!renderer->set_object_texture("tex-reused",texture))
                    error=13;

                if(glGetError()!=GL_NO_ERROR)error=15;
            }
            renderer->clear_object_textures();
            renderer->clear_mesh_assets();
            renderer->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    if(error)
    {
        std::cerr<<"GPU full-residency pressure test failed: "<<error<<'\n';
        return error;
    }
    const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now()-stress_started).count();
    std::cout<<"GPU 512 textures / 384 meshes, 4 draw stages, rejection,"
        " replacement and recovery OK; CPU wall_ms="<<elapsed<<'\n';
    return 0;
}
