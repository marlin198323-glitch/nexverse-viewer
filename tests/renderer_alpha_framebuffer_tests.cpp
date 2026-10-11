#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
using Pixel=std::array<unsigned char,4>;
namespace
{
Pixel capture(nexverse::render::Renderer& renderer)
{
    renderer.begin_frame();
    renderer.render_world(0.0);
    std::array<unsigned char,4> result{};
    glReadPixels(160,100,1,1,GL_RGBA,GL_UNSIGNED_BYTE,result.data());
    renderer.end_frame();
    return result;
}
int distance(Pixel a,Pixel b)
{
    return std::abs(int(a[0])-int(b[0]))+
        std::abs(int(a[1])-int(b[1]))+
        std::abs(int(a[2])-int(b[2]));
}
}
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window=glfwCreateWindow(320,200,"Alpha Pixel",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(window);glfwTerminate();return 1;}
    using namespace nexverse::render;
    int error=0;
    {
        auto renderer=create_legacy_opengl_renderer();
        if(!renderer || !renderer->initialize(320,200))error=2;
        if(!error)
        {
            RenderMeshGeometry quad{
                {3.f,-2.f,-2.f, 1.f,0.f,0.f,0.f,0.f,
                 3.f, 2.f,-2.f, 1.f,0.f,0.f,1.f,0.f,
                 3.f, 2.f, 2.f, 1.f,0.f,0.f,1.f,1.f,
                 3.f,-2.f, 2.f, 1.f,0.f,0.f,0.f,1.f},
                {0U,1U,2U,0U,2U,3U},{{0U,0U,6U}}};
            RenderSceneObject scene;
            scene.local_id=700U;
            scene.mesh_id="alpha-pixel-quad";
            scene.texture_id="alpha-pixel-texture";
            auto& face=scene.face_materials[0];
            face=RenderFaceMaterial{};
            face->texture_id=scene.texture_id;
            face->fullbright=true;
            if(!renderer->set_mesh_asset(scene.mesh_id,quad))error=3;
            renderer->set_camera({{0.0f,0.0f,0.0f},
                {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                {0.0f,0.0f,1.0f},60.0f,0.1f,40.0f});
            renderer->set_scene_objects({});
            const auto background=capture(*renderer);
            RenderImageRgba8 texture{2U,2U,
                std::vector<std::uint8_t>(16U,255U)};
            for(std::size_t i=0U;i<texture.pixels.size();i+=4U)
            {
                texture.pixels[i]=245U;
                texture.pixels[i+1U]=10U;
                texture.pixels[i+2U]=12U;
            }
            if(!renderer->set_object_texture(scene.texture_id,texture))
                error=4;
            renderer->set_scene_objects({scene});
            const auto opaque=capture(*renderer);
            const auto opaque_stats=renderer->diagnostics();
            if(!error && (distance(background,opaque)<35 ||
               opaque_stats.scene_opaque_draw_calls!=1U))
                error=5;
            for(std::size_t i=3U;i<texture.pixels.size();i+=4U)
                texture.pixels[i]=0U;
            if(!renderer->set_object_texture(scene.texture_id,texture))
                error=6;
            scene.face_materials[0]->diffuse_alpha_mode=2U;
            scene.face_materials[0]->alpha_mask_cutoff=128U;
            renderer->set_scene_objects({scene});
            const auto masked=capture(*renderer);
            const auto mask_stats=renderer->diagnostics();
            // Compare masked-discard against a zero-alpha *blended* face
            // in the SAME scene. A no-scene frame takes a different
            // renderer branch, so it is not the pixel reference here.
            scene.face_materials[0]->diffuse_alpha_mode=1U;
            renderer->set_scene_objects({scene});
            const auto zero_alpha_blended=capture(*renderer);
            scene.face_materials[0]->diffuse_alpha_mode=2U;
            renderer->set_scene_objects({scene});
            if(!error && (distance(zero_alpha_blended,masked)>3 ||
               distance(opaque,masked)<35 ||
               mask_stats.scene_alpha_mask_draw_calls!=1U ||
               mask_stats.scene_alpha_draw_calls!=0U))
                error=7;
            for(std::size_t i=3U;i<texture.pixels.size();i+=4U)
                texture.pixels[i]=255U;
            if(!renderer->set_object_texture(scene.texture_id,texture))
                error=8;
            const auto unmasked=capture(*renderer);
            if(!error && distance(opaque,unmasked)>6)error=9;
            for(std::size_t i=3U;i<texture.pixels.size();i+=4U)
                texture.pixels[i]=128U;
            if(!renderer->set_object_texture(scene.texture_id,texture))
                error=10;
            scene.face_materials[0]->diffuse_alpha_mode=1U;
            renderer->set_scene_objects({scene});
            const auto blended=capture(*renderer);
            const auto blend_stats=renderer->diagnostics();
            if(!error && (distance(background,blended)<12 ||
                distance(opaque,blended)<12 ||
                blend_stats.scene_alpha_draw_calls!=1U ||
                blend_stats.scene_alpha_mask_draw_calls!=0U))
                error=11;
            std::cout<<"rgba center sky=["<<int(background[0])<<","
                <<int(background[1])<<","<<int(background[2])
                <<"] opaque=["<<int(opaque[0])<<","<<int(opaque[1])
                <<","<<int(opaque[2])<<"] masked=["
                <<int(masked[0])<<","<<int(masked[1])<<","
                <<int(masked[2])<<"] zeroBlend=["
                <<int(zero_alpha_blended[0])<<","<<int(zero_alpha_blended[1])
                <<","<<int(zero_alpha_blended[2])<<"] blended=["
                <<int(blended[0])<<","<<int(blended[1])<<","
                <<int(blended[2])<<"]\n";
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
        std::cerr<<"OpenGL framebuffer alpha/mask test failed "<<error<<'\n';
        return error;
    }
    return 0;
}
