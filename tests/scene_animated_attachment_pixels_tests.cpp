/* MPL-2.0: GPU framebuffer test for Firestorm joint-following attachments. */
#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
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
    auto* window=glfwCreateWindow(480,300,"OGL joint attachment",nullptr,nullptr);
    if(!window){glfwTerminate();return 77;}
    glfwMakeContextCurrent(window);
    int failure=0;
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(
         glfwGetProcAddress)))failure=1;
    auto renderer=create_legacy_opengl_renderer();
    if(!failure&&!renderer->initialize(480,300))failure=2;
    if(!failure)
    {
        renderer->set_camera({{0.0f,0.0f,1.0f},
            {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
            {0.0f,0.0f,1.0f},65.0f,0.1f,40.0f});
        RenderAvatarBindRig rig{};
        rig.joints.push_back({"mPelvis",-1,{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}});
        rig.joints.push_back({"mShoulderLeft",0,{0.0f,0.0f,1.0f},
                              {0.0f,0.0f,1.0f}});
        rig.available[1U]=true;
        rig.attachment_offsets[1U]={0.85f,1.1f,1.0f};
        rig.attachment_local_offsets[1U]={0.85f,1.1f,0.0f};
        rig.attachment_joint_plus_one[1U]=2U;
        rig.available[31U]=true;
        rig.is_hud[31U]=true;
        renderer->set_avatar_bind_rig(rig);
        RenderAvatarProxy avatar{};
        avatar.local_id=51U;
        avatar.position={4.0f,0.0f,0.0f};
        avatar.motion=nexverse::protocol::AvatarMotion::walk;
        avatar.attachments["sleeve"]=1U;
        renderer->set_avatar_proxies({avatar});
        RenderSceneObject attached{};
        attached.object_id="sleeve";
        attached.local_id=200U;
        attached.parent_id=51U;
        attached.scale={0.85f,0.85f,0.85f};
        attached.color={1.0f,0.05f,0.12f,1.0f};
        RenderSceneObject sentinel{};
        sentinel.local_id=900U;
        sentinel.position={-30.0f,0.0f,0.0f};
        const auto capture=[&](double seconds)
        {
            renderer->begin_frame();
            renderer->render_world(seconds);
            renderer->end_frame();
            glFinish();
            std::vector<std::uint8_t> pixels(480U*300U*4U);
            glReadBuffer(GL_BACK);
            glReadPixels(0,0,480,300,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
            return pixels;
        };
        const auto differences=[](const auto& first,const auto& second)
        {
            std::size_t n=0U;
            for(std::size_t k=0U;k<first.size();k+=4U)
                if(first[k]!=second[k]||first[k+1U]!=second[k+1U]||
                   first[k+2U]!=second[k+2U])++n;
            return n;
        };
        const auto normalized_object=[&](double seconds)
        {
            // Removing each timepoint's own animated sky and unchanged
            // avatar isolates attachment geometry in the GL framebuffer.
            renderer->set_scene_objects({sentinel});
            const auto backdrop=capture(seconds);
            renderer->set_scene_objects({attached,sentinel});
            auto color=capture(seconds);
            for(std::size_t k=0U;k<color.size();k+=4U)
            {
                if(color[k]==backdrop[k] &&
                   color[k+1U]==backdrop[k+1U] &&
                   color[k+2U]==backdrop[k+2U])
                {
                    color[k]=0U;color[k+1U]=0U;color[k+2U]=0U;
                }
            }
            return color;
        };
        const auto before=normalized_object(0.0);
        const auto count=renderer->diagnostics().scene_bone_pose_avatars;
        const auto after=normalized_object(0.22);
        const auto changed=differences(before,after);
        const auto visible_pixels=[](const auto& data){std::size_t n=0;for(std::size_t k=0;k<data.size();k+=4U)if(data[k]||data[k+1U]||data[k+2U])++n;return n;};
        std::cout<<"ATTACHED_PIXELS_A="<<visible_pixels(before)<<" B="<<visible_pixels(after)<<" SCENE_DRAWS="<<renderer->diagnostics().scene_draw_calls<<"\n";
        std::cout<<"JOINT_ATTACHMENT_POSED_RGB_PIXELS="<<changed
                 <<" BONE_POSE_AVATARS="<<count<<"\n";
        if(changed<40U||count!=1U)failure=3;
        // Without a verified skeleton binding the same worn object stays
        // at its legacy bind offset, even as the provisional avatar walks.
        rig.attachment_joint_plus_one[1U]=0U;
        renderer->set_avatar_bind_rig(rig);
        const auto no_bone_before=normalized_object(0.0);
        const auto no_bone_after=normalized_object(0.22);
        const auto fallback_delta=differences(no_bone_before,no_bone_after);
        if(fallback_delta!=0U||
           renderer->diagnostics().scene_bone_pose_avatars!=0U)
            failure=4;
        // A HUD joint cannot become world geometry or consume a posed
        // attachment slot, even while the avatar changes animation.
        avatar.attachments["sleeve"]=31U;
        renderer->set_avatar_proxies({avatar});
        renderer->set_avatar_bind_rig(rig);
        const auto hud=capture(0.22);
        renderer->set_scene_objects({sentinel});
        const auto hud_baseline=capture(0.22);
        const auto hud_delta=differences(hud,hud_baseline);
        std::cout<<"JOINT_FALLBACK_DRIFT="<<fallback_delta
                 <<" HUD_STALE_RGB_PIXELS="<<hud_delta<<"\n";
        if(hud_delta!=0U)failure=5;
        if(glGetError()!=GL_NO_ERROR)failure=6;
    }
    if(renderer)renderer->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    if(!failure)std::cout<<"OGL_JOINT_ATTACHMENT_GPU_PASS\n";
    return failure;
}
