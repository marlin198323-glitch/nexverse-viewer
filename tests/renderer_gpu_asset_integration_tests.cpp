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
            if(!renderer->set_object_texture("synthetic-texture",image))
                failed=9;
            image.pixels[0]=100U;
            if(!renderer->set_object_texture("synthetic-texture",image))
                failed=9;
            renderer->remove_object_texture("synthetic-texture");
            if(!renderer->set_object_texture("synthetic-texture",image))
                failed=9;
            RenderMeshGeometry geometry{
                {3.0f,-1.0f,-1.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,
                 3.0f,1.0f,-1.0f,  1.0f,0.0f,0.0f, 1.0f,0.0f,
                 3.0f,0.0f,1.0f,   1.0f,0.0f,0.0f, 0.5f,1.0f,
                 3.0f,0.0f,0.0f,   1.0f,0.0f,0.0f, 0.5f,0.5f},
                {0U,1U,3U, 0U,3U,2U},
                {{0U,0U,3U},{1U,3U,3U}}};
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
            RenderSceneObject prim=scene;
            prim.local_id=101U;
            prim.mesh_id.clear();
            prim.position={0.0f,0.4f,0.0f};
            RenderSceneObject unresolved=scene;
            unresolved.local_id=102U;
            unresolved.mesh_id="not-uploaded-mesh";
            unresolved.position={0.0f,-0.4f,0.0f};
            renderer->set_scene_objects({scene,prim,unresolved});
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
            if(!failed && (info.scene_mesh_draw_calls!=2U ||
                info.scene_prim_draw_calls==0U ||
                info.scene_missing_mesh_draw_calls==0U ||
                info.scene_draw_calls!=
                    info.scene_mesh_draw_calls+info.scene_prim_draw_calls))
                failed=10;
            const auto gl_error=glGetError();
            if(!failed && gl_error!=GL_NO_ERROR)
            {
                std::cerr<<"Renderer OpenGL error: "<<gl_error<<'\n';
                failed=7;
            }
            renderer->remove_mesh_asset("synthetic-mesh");
            renderer->begin_frame();
            renderer->render_world(0.3);
            renderer->end_frame();
            const auto no_mesh=renderer->diagnostics();
            if(!failed && (no_mesh.scene_mesh_draw_calls!=0U ||
               no_mesh.scene_missing_mesh_draw_calls<
                   info.scene_missing_mesh_draw_calls))
                failed=11; // Frame counters reset and missing mesh falls back.
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
