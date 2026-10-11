#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
int main()
{
    if(!glfwInit())
    {
        std::cout<<"SKIP: GLFW display/context unavailable\n";
        return 77;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    GLFWwindow* window=glfwCreateWindow(320,200,"OGL Render Test",nullptr,nullptr);
    if(!window)
    {
        glfwTerminate();
        std::cout<<"SKIP: no OpenGL 3.3 context\n";
        return 77;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    using namespace nexverse::render;
    int failed=0;
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(320,200)) failed=2;
        if(!failed)
        {
            RenderImageRgba8 image{2U,2U,{
                255,0,0,255, 0,255,0,255,
                0,0,255,255, 255,255,0,255}};
            renderer->set_object_texture("synthetic-texture",image);
            image.pixels[0]=100U;
            renderer->set_object_texture("synthetic-texture",image);
            renderer->remove_object_texture("synthetic-texture");
            renderer->set_object_texture("synthetic-texture",image);
            RenderMeshGeometry geometry{
                {3.0f,-1.0f,-1.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,
                 3.0f,1.0f,-1.0f,  1.0f,0.0f,0.0f, 1.0f,0.0f,
                 3.0f,0.0f,1.0f,   1.0f,0.0f,0.0f, 0.5f,1.0f},
                {0U,1U,2U},{{0U,0U,3U}}};
            if(!renderer->set_mesh_asset("synthetic-mesh",geometry))failed=3;
            if(!failed)
            {
                geometry.vertices[0]+=0.2f;
                if(!renderer->set_mesh_asset("synthetic-mesh",geometry))
                    failed=4;
                geometry.indices.clear();
                if(renderer->set_mesh_asset("synthetic-mesh",geometry))
                    failed=5;
            }
            RenderSceneObject scene;
            scene.local_id=100U;
            scene.mesh_id="synthetic-mesh";
            scene.position={0.0f,0.0f,0.0f};
            scene.texture_id="synthetic-texture";
            renderer->set_scene_objects({scene});
            renderer->set_camera({{0.0f,0.0f,0.0f},
                {1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f},
                {0.0f,0.0f,1.0f},60.0f,0.1f,50.0f});
            renderer->begin_frame();
            renderer->render_world(0.0);
            renderer->end_frame();
            const auto info=renderer->diagnostics();
            if(!failed && info.backend.empty())failed=6;
            if(!failed && info.visible_objects==0U)
                failed=8; // Synthetic mesh object must reach the renderer.
            const auto gl_error=glGetError();
            if(!failed && gl_error!=GL_NO_ERROR)
            {
                std::cerr<<"Renderer OpenGL error: "<<gl_error<<'\n';
                failed=7;
            }
            renderer->remove_mesh_asset("synthetic-mesh");
            renderer->clear_mesh_assets();
            renderer->clear_object_textures();
            renderer->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    if(failed)
    {
        std::cerr<<"Synthetic GPU renderer test failed: "<<failed<<'\n';
        return failed;
    }
    std::cout<<"OpenGL 3.3 hidden-context real mesh/texture upload, replacement and draw OK\n";
    return 0;
}
