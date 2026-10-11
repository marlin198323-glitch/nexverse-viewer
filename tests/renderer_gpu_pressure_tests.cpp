#include "nexverse/render/renderer.hpp"
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
                if(n<256U)
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
                    if(health.visible_objects==0U || health.draw_calls==0U)
                    {error=5;break;}
                    if(glGetError()!=GL_NO_ERROR){error=6;break;}
                }
            }
            if(!error)
            {
                if(renderer->set_object_texture("tex-over",texture))
                    error=7;
                if(renderer->set_mesh_asset("mesh-over",mesh))error=8;
                // A rejected replacement cannot corrupt a loaded resource.
                RenderImageRgba8 invalid;
                if(renderer->set_object_texture("tex-0",invalid))error=9;
                RenderMeshGeometry invalid_mesh=mesh;
                invalid_mesh.indices={0U,1U,900U};
                if(renderer->set_mesh_asset("mesh-0",invalid_mesh))
                    error=10;
                if(!renderer->set_object_texture("tex-0",texture))
                    error=11;
                if(!renderer->set_mesh_asset("mesh-0",mesh))
                    error=12;
                renderer->remove_object_texture("tex-511");
                renderer->remove_mesh_asset("mesh-255");
                if(!renderer->set_object_texture("tex-reused",texture))
                    error=13;
                if(!renderer->set_mesh_asset("mesh-reused",mesh))
                    error=14;
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
    std::cout<<"GPU 512 textures / 256 meshes, 4 draw stages, rejection,"
        " replacement and recovery OK; CPU wall_ms="<<elapsed<<'\n';
    return 0;
}
