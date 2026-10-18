/* MPL-2.0: real OpenGL deep avatar linkset pixel test. */
#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace nexverse::render;
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    auto* w=glfwCreateWindow(480,300,"OGL deep linkset",nullptr,nullptr);
    if(!w){glfwTerminate();return 77;}
    glfwMakeContextCurrent(w);
    int error=0;
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(
         glfwGetProcAddress)))error=1;
    auto renderer=create_legacy_opengl_renderer();
    if(!error&&!renderer->initialize(480,300))error=2;
    if(!error)
    {
        renderer->set_camera({{0.0f,0.0f,1.0f},
            {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
            {0.0f,0.0f,1.0f},65.0f,0.1f,40.0f});
        RenderAvatarProxy avatar{};
        avatar.local_id=51U;
        avatar.position={4.0f,0.0f,1.0f};
        avatar.attachments["worn"]=1U;
        RenderAvatarBindRig rig{};
        rig.available[1U]=true;
        rig.available[31U]=true;
        rig.is_hud[31U]=true;
        renderer->set_avatar_bind_rig(rig);
        renderer->set_avatar_proxies({avatar});
        std::vector<RenderSceneObject> linked(21U);
        for(std::size_t i=0;i<linked.size();++i)
        {
            linked[i].local_id=static_cast<std::uint32_t>(1000U+i);
            linked[i].parent_id=i==0U?51U:linked[i-1U].local_id;
            linked[i].scale={0.01f,0.01f,0.01f};
            linked[i].color={0.1f,0.1f,0.1f,1.0f};
        }
        linked[0].object_id="worn";
        linked.back().position={0.0f,0.8f,0.1f};
        linked.back().scale={1.0f,1.0f,1.0f};
        linked.back().color={1.0f,0.0f,0.1f,1.0f};
        const auto draw=[&]()
        {
            renderer->begin_frame();
            renderer->render_world(0.0);
            renderer->end_frame();
            glFinish(); // test only, not part of live rendering
            std::vector<std::uint8_t> rgba(480U*300U*4U);
            glReadBuffer(GL_BACK);
            glReadPixels(0,0,480,300,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());
            return rgba;
        };
        RenderSceneObject sentinel{};
        sentinel.local_id=9000U;
        sentinel.position={-30.0f,0.0f,0.0f};
        renderer->set_scene_objects({sentinel});
        const auto background=draw();
        linked.push_back(sentinel);
        renderer->set_scene_objects(linked);
        const auto worn=draw();
        auto stats=renderer->diagnostics();
        const auto changed=[&](const auto& a,const auto& b)
        {
            std::size_t n=0U;
            for(std::size_t i=0U;i<a.size();i+=4U)
                if(a[i]!=b[i]||a[i+1U]!=b[i+1U]||
                   a[i+2U]!=b[i+2U])++n;
            return n;
        };
        const auto wear_pixels=changed(background,worn);
        std::cout<<"DEEP_LINKSET_WORN_RGB_PIXELS="<<wear_pixels
            <<" MAX_DEPTH="<<stats.scene_max_visible_parent_depth
            <<" DEEP_COUNT="<<stats.scene_deep_visible_linksets<<"\n";
        if(wear_pixels<40U||stats.scene_max_visible_parent_depth<20U||
           stats.scene_deep_visible_linksets==0U)error=3;
        avatar.attachments["worn"]=31U;
        renderer->set_avatar_proxies({avatar});
        const auto hud=draw();
        stats=renderer->diagnostics();
        renderer->set_scene_objects({sentinel});
        const auto after=draw();
        const auto hud_delta=changed(after,hud);
        std::cout<<"DEEP_HUD_LINKSET_STALE_RGB_PIXELS="<<hud_delta
            <<" EXCLUDED="<<stats.scene_hud_linkset_excluded
            <<" SCENE_DRAWS="<<stats.scene_draw_calls
            <<" BASELINE_DRIFT="<<changed(background,after)<<"\n";
        if(hud_delta!=0U||stats.scene_hud_linkset_excluded!=linked.size()-1U)
            error=4;
        if(glGetError()!=GL_NO_ERROR)error=5;
    }
    if(renderer)renderer->shutdown();
    glfwDestroyWindow(w);
    glfwTerminate();
    if(!error)std::cout<<"OGL_DEEP_LINKSET_GL_PIXELS_PASS\n";
    return error;
}
