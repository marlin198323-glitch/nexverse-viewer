#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    auto* window=glfwCreateWindow(400,230,"Region Transition",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}
    using namespace nexverse::render;
    int error=0;
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(400,230))error=2;
        if(!error)
        {
            const std::string same_id="00000000-0000-0000-0000-000000000a75";
            // Three independently mapped faces, opaque, tint-blended,
            // and alpha-textured. All faces use the same GPU mesh UUID.
            RenderMeshGeometry mesh{
                {3.0f,-1.0f,-0.8f,1.0f,0.0f,0.0f,0.0f,0.0f,
                 3.0f, 1.0f,-0.8f,1.0f,0.0f,0.0f,1.0f,0.0f,
                 3.0f, 1.0f, 0.8f,1.0f,0.0f,0.0f,1.0f,1.0f,
                 3.0f,-1.0f, 0.8f,1.0f,0.0f,0.0f,0.0f,1.0f},
                {0U,1U,2U, 0U,2U,3U, 0U,1U,3U},
                {{0U,0U,3U},{1U,3U,3U},{2U,6U,3U}}};
            RenderImageRgba8 opaque{2U,2U,
                std::vector<std::uint8_t>(16U,255U)};
            RenderImageRgba8 alpha=opaque;
            alpha.pixels[3U]=110U;
            if(!renderer->set_object_texture("opaque",opaque) ||
               !renderer->set_object_texture("alpha",alpha) ||
               !renderer->set_mesh_asset(same_id,mesh))error=3;
            RenderSceneObject obj;
            obj.local_id=200U;
            obj.mesh_id=same_id;
            obj.position={0.0f,0.0f,0.0f};
            obj.texture_id="opaque";
            for(unsigned face=0U;face<3U;++face)
            {
                RenderFaceMaterial material;
                material.texture_id=face==2U?"alpha":"opaque";
                material.color={1.0f,0.8f,0.5f,face==1U?0.45f:1.0f};
                material.fullbright=face==0U;
                material.repeat_u=1.0f+static_cast<float>(face);
                material.shiny=static_cast<std::uint8_t>(face);
                obj.face_materials[face]=material;
            }
            renderer->set_camera({{0.0f,0.0f,0.0f},
                {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                {0.0f,0.0f,1.0f},60.0f,0.1f,40.0f});
            const auto render=[&]()
            {
                renderer->set_scene_objects({obj});
                renderer->begin_frame();
                renderer->render_world(0.0);
                renderer->end_frame();
                return renderer->diagnostics();
            };
            if(!error)
            {
                const auto first=render();
                if(first.scene_mesh_draw_calls!=3U ||
                   first.scene_missing_mesh_objects!=0U ||
                   glGetError()!=GL_NO_ERROR)error=4;
            }
            // Region transition: old GPU resources must not leak into the
            // next region even if the immutable asset UUID is repeated.
            renderer->clear_mesh_assets();
            renderer->clear_object_textures();
            if(!error)
            {
                const auto missing=render();
                if(missing.scene_mesh_draw_calls!=0U ||
                   missing.scene_missing_mesh_objects!=1U ||
                   missing.scene_prim_draw_calls==0U ||
                   glGetError()!=GL_NO_ERROR)error=5;
            }
            opaque.pixels[0]=50U;
            if(!renderer->set_object_texture("opaque",opaque) ||
               !renderer->set_object_texture("alpha",alpha) ||
               !renderer->set_mesh_asset(same_id,mesh))error=6;
            if(!error)
            {
                const auto restored=render();
                if(restored.scene_mesh_draw_calls!=3U ||
                   restored.scene_missing_mesh_objects!=0U ||
                   glGetError()!=GL_NO_ERROR)error=7;
            }
            renderer->set_scene_objects({});
            renderer->clear_mesh_assets();
            renderer->clear_object_textures();
            renderer->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    if(error)
    {
        std::cerr<<"Three-face GPU region transition test failed: "<<error<<'\n';
        return error;
    }
    std::cout<<"GL3.3 multi-face materials, transparent pass, region GPU clear and reupload OK\n";
    return 0;
}
