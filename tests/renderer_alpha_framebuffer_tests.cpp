#include "nexverse/render/renderer.hpp"
#include "nexverse/render/third_person_camera.hpp"
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
Pixel capture_at(nexverse::render::Renderer& renderer,int x,int y)
{
    renderer.begin_frame();
    renderer.render_world(0.0);
    std::array<unsigned char,4> result{};
    glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,result.data());
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
            // Real OpenGL center-pixel proof for the app third-person
            // camera, with NO terrain streamed: receiving a mesh is not
            // enough; the object must actually change the framebuffer.
            if(!error)
            {
                const auto view=third_person_camera(
                    {0.0f,0.0f,0.0f},{});
                renderer->set_camera({
                    {view.position.x,view.position.y,view.position.z},
                    {view.forward.x,view.forward.y,view.forward.z},
                    {view.left.x,view.left.y,view.left.z},
                    {view.up.x,view.up.y,view.up.z},
                    60.0f,0.1f,40.0f});
                for(std::size_t i=3U;i<texture.pixels.size();i+=4U)
                    texture.pixels[i]=255U;
                if(!renderer->set_object_texture(scene.texture_id,texture))
                    error=12;
                scene.face_materials[0]->diffuse_alpha_mode=0U;
                renderer->set_scene_objects({});
                const auto clear_frame=capture(*renderer);
                renderer->set_scene_objects({scene});
                const auto camera_frame=capture(*renderer);
                const auto camera_stats=renderer->diagnostics();
                if(!error && (distance(clear_frame,camera_frame)<35 ||
                   camera_stats.scene_mesh_draw_calls==0U))
                    error=13;
                // No NaN quaternion may poison the active GPU view.
                const auto fallback=third_person_camera(
                    {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f});
                renderer->set_camera({
                    {fallback.position.x,fallback.position.y,
                        fallback.position.z},
                    {fallback.forward.x,fallback.forward.y,
                        fallback.forward.z},
                    {fallback.left.x,fallback.left.y,fallback.left.z},
                    {fallback.up.x,fallback.up.y,fallback.up.z},
                    60.0f,0.1f,40.0f});
                const auto fallback_frame=capture(*renderer);
                if(!error && (!fallback.position_valid||
                   fallback.orientation_valid||
                   distance(camera_frame,fallback_frame)>6 ||
                   glGetError()!=GL_NO_ERROR))
                    error=14;
            }
            // Actual camera zoom must change world mesh pixel extent
            // in GPU framebuffer, not merely the GUI zoom label.
            if(!error)
            {
                const auto make_camera=[&](float distance)
                {
                    const auto v=third_person_camera(
                        {0.0f,0.0f,0.0f},{},distance);
                    renderer->set_camera({
                        {v.position.x,v.position.y,v.position.z},
                        {v.forward.x,v.forward.y,v.forward.z},
                        {v.left.x,v.left.y,v.left.z},
                        {v.up.x,v.up.y,v.up.z},
                        60.0f,0.1f,60.0f});
                };
                renderer->set_scene_objects({scene});
                make_camera(3.0f);
                const auto near_sample=capture_at(*renderer,202,100);
                const auto near_stats=renderer->diagnostics();
                make_camera(24.0f);
                const auto far_sample=capture_at(*renderer,202,100);
                const auto far_stats=renderer->diagnostics();
                if(near_stats.scene_mesh_draw_calls==0U||
                   far_stats.scene_mesh_draw_calls==0U||
                   distance(near_sample,far_sample)<30||
                   glGetError()!=GL_NO_ERROR)
                {
                    std::cerr<<"Zoom pixels: near="<<int(near_sample[0])
                        <<","<<int(near_sample[1])<<","<<int(near_sample[2])
                        <<" far="<<int(far_sample[0])
                        <<","<<int(far_sample[1])<<","<<int(far_sample[2])
                        <<" draws="<<near_stats.scene_mesh_draw_calls
                        <<","<<far_stats.scene_mesh_draw_calls<<'\n';
                    error=16;
                }
            }
            // ACTUAL GPU pixel evidence for back- and front-facing
            // self-avatar camera orbit. The demo quad is double-sided
            // for the test: only the camera moves; the world/mesh does
            // not follow it and no TerrainLayerData is required.
            if(!error)
            {
                auto orbit_quad=quad;
                for(std::size_t v=0;v<orbit_quad.vertices.size()/8U;++v)
                    orbit_quad.vertices[v*8U+2U]*=3.0f;
                orbit_quad.indices={0U,1U,2U,0U,2U,3U,
                                     2U,1U,0U,3U,2U,0U};
                scene.position={0.0f,0.0f,0.0f};
                scene.mesh_id="orbit-double-sided-quad";
                if(!renderer->set_mesh_asset(scene.mesh_id,orbit_quad))
                    error=17;
                const auto test_orbit=[&](float degrees)->bool
                {
                    const auto pose=third_person_camera(
                        {0.0f,0.0f,0.0f},{},8.0f,degrees);
                    renderer->set_camera({
                        {pose.position.x,pose.position.y,pose.position.z},
                        {pose.forward.x,pose.forward.y,pose.forward.z},
                        {pose.left.x,pose.left.y,pose.left.z},
                        {pose.up.x,pose.up.y,pose.up.z},
                        60.0f,0.1f,40.0f});
                    renderer->set_scene_objects({});
                    const auto sky=capture(*renderer);
                    renderer->set_scene_objects({scene});
                    const auto mesh=capture(*renderer);
                    const auto stats=renderer->diagnostics();
                    const int pixels=distance(sky,mesh);
                    if(pixels<35||stats.scene_mesh_draw_calls==0U)
                        std::cerr<<"Orbit "<<degrees<<" GPU pixels="<<
                            pixels<<" draws="<<stats.scene_mesh_draw_calls<<'\n';
                    return pixels>=35&&stats.scene_mesh_draw_calls>0U&&
                        glGetError()==GL_NO_ERROR;
                };
                if(!error&&(!test_orbit(0.0f)||!test_orbit(180.0f)))
                    error=18;
            }
            // Objects entirely behind the camera must no longer count
            // as visible or successful GL-world submissions.
            if(!error)
            {
                // This fixture's mesh has local x=3 vertices but its
                // object origin is x=0. Move the actual object origin
                // behind the camera too; scene sphere-culling is based
                // on the object's transform, not arbitrary mesh offsets.
                scene.position={4.0f,0.0f,0.0f};
                renderer->set_camera({{0.0f,0.0f,0.0f},
                    {-1.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f},
                    {0.0f,0.0f,1.0f},60.0f,0.1f,40.0f});
                renderer->set_scene_objects({});
                const auto reverse_sky=capture(*renderer);
                renderer->set_scene_objects({scene});
                const auto reverse_world=capture(*renderer);
                const auto reverse_stats=renderer->diagnostics();
                const int reverse_pixel_diff=
                    distance(reverse_sky,reverse_world);
                const auto reverse_gl_error=glGetError();
                if(reverse_stats.scene_draw_calls!=0U ||
                   reverse_stats.visible_objects!=0U ||
                   reverse_pixel_diff>3 ||
                   reverse_gl_error!=GL_NO_ERROR)
                {
                    std::cerr<<"Reverse camera: draws="<<
                        reverse_stats.scene_draw_calls<<
                        " visible="<<reverse_stats.visible_objects<<
                        " pixel_difference="<<reverse_pixel_diff<<
                        " gl="<<reverse_gl_error<<'\n';
                    error=15;
                }
            }
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
