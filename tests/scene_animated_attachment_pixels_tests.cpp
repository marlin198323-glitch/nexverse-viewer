/* MPL-2.0: GPU framebuffer test for Firestorm joint-following attachments. */
#include "nexverse/render/renderer.hpp"
#include "nexverse/world/attachment_state.hpp"
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
        // The real OpenGenesisLINK legacy Low158 omits AttachmentBlock:
        // derive the same render-bone point from current simulator prim
        // State/ParentID instead of manually creating avatar.attachments.
        nexverse::world::ObjectState root{};
        root.local_id=200U;root.id="sleeve";
        root.pcode=nexverse::protocol::pcode_prim;
        root.parent_id=51U;root.state=0x10U;
        nexverse::world::WorldModel region_objects;
        region_objects.apply_region({
            0x0000010000000200ULL,"NexVerse",{128.0f,128.0f,25.0f},
            "127.0.0.1",9000U,"https://caps.example/seed",
            "aaaaaaaa-1111-2222-3333-bbbbbbbbbbbb",0x10U,0U,0U,
            13U,20.0f,"OpenGenesisLINK Test",256U,256U,true});
        root.rotation={0.0f,0.0f,0.0f,1.0f};
        region_objects.upsert_object(root);
        const std::map<std::string,std::uint8_t,std::less<>> old_low158{
            {"sleeve",31U} // stale HUD slot, actual prim says shoulder
        };
        const auto inferred=nexverse::world::reconcile_avatar_attachments(
            region_objects.objects(),avatar.local_id,old_low158);
        avatar.attachments.insert(
            inferred.points.begin(),inferred.points.end());
        if(avatar.attachments.size()!=1U||
           avatar.attachments.at("sleeve")!=1U||
           inferred.current_object_points!=1U||
           inferred.pending_appearance_points!=0U)failure=11;
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
        // ImprovedTerse now modifies ONLY the same live root prim pose.
        // The Firestorm animated shoulder must remain attached, and a
        // changed server prim offset must actually move GPU pixels.
        const auto old_pose=normalized_object(0.22);
        if(!region_objects.apply_object_terse(
               200U,{0.0f,0.8f,0.0f},
               {0.0f,0.0f,0.0f,1.0f})||
           nexverse::world::infer_avatar_attachment_points(
               region_objects.objects(),avatar.local_id)
               .at("sleeve")!=1U)
            failure=12;
        const auto& new_pos=region_objects.objects().at(200U).position;
        attached.position={new_pos.x,new_pos.y,new_pos.z};
        const auto moved_pose=normalized_object(0.22);
        const auto actual_terse_gpu_pixels=differences(old_pose,moved_pose);
        std::cout<<"TERSE_ATTACHMENT_WORLD_MODEL_GPU_PIXELS="
                 <<actual_terse_gpu_pixels<<"\n";
        if(actual_terse_gpu_pixels<40U)failure=13;
        // The 64-byte ObjectUpdateCompressed header is ALSO pose/State-
        // only. A current UUID-matched header must keep its avatar
        // attachment and change genuine framebuffer pixels.
        nexverse::protocol::ObjectUpdateCompressedEntry partial{};
        partial.id="sleeve";
        partial.local_id=200U;
        partial.pcode=nexverse::protocol::pcode_prim;
        partial.state=0x10U;
        partial.position={0.0f,-0.8f,0.0f};
        partial.rotation={0.0f,0.0f,0.0f,1.0f};
        if(!region_objects.apply_object_compressed_partial(partial)||
           nexverse::world::infer_avatar_attachment_points(
               region_objects.objects(),avatar.local_id).at("sleeve")!=1U)
            failure=14;
        attached.position={partial.position.x,partial.position.y,partial.position.z};
        const auto compressed_pixels=normalized_object(0.22);
        const auto compressed_delta=differences(moved_pose,compressed_pixels);
        std::cout<<"PARTIAL_COMPRESSED_ATTACHMENT_GL_PIXELS="
                 <<compressed_delta<<"\n";
        if(compressed_delta<40U)failure=15;
        attached.position={0.0f,0.0f,0.0f};
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
        // A real KillObject for the REMOTE avatar LocalID must immediately
        // remove the entire linkset and its Firestorm avatar GPU proxy.
        // First establish an actual remote avatar ObjectUpdate.
        nexverse::world::AvatarState actual_remote{};
        actual_remote.id="received-avatar";
        actual_remote.local_id=51U;
        actual_remote.position_known=true;
        actual_remote.simulator_object_update_seen=true;
        region_objects.upsert_avatar(actual_remote);
        rig.attachment_joint_plus_one[1U]=2U;
        renderer->set_avatar_bind_rig(rig);
        avatar.attachments["sleeve"]=1U;
        renderer->set_avatar_proxies({avatar});
        renderer->set_scene_objects({attached,sentinel});
        const auto before_kill=capture(0.22);
        renderer->set_avatar_proxies({});
        renderer->set_scene_objects({sentinel});
        const auto empty_scene=capture(0.22); // no avatar OR mesh
        const auto before_kill_pixels=differences(before_kill,empty_scene);
        if(before_kill_pixels<25U)failure=16;
        // Restore the real pre-kill render scene; the next frame is
        // derived from current WorldModel after the simulator KillObject.
        renderer->set_avatar_proxies({avatar});
        renderer->set_scene_objects({attached,sentinel});
        if(!region_objects.kill_simulator_local_id(51U)||
           !region_objects.avatars().empty()||
           region_objects.objects().contains(200U))failure=17;
        renderer->set_avatar_proxies({});
        renderer->set_scene_objects({sentinel});
        const auto after_kill=capture(0.22);
        const auto stale_gpu_pixels=differences(empty_scene,after_kill);
        std::cout<<"REMOTE_KILLOBJECT_STALE_ATTACHMENT_GPU_PIXELS="
                 <<stale_gpu_pixels
                 <<" BEFORE="<<before_kill_pixels<<"\n";
        if(stale_gpu_pixels!=0U)failure=18;
        // UDP may omit the previous KillObject. A new Full/Compressed
        // avatar ObjectUpdate reuses LocalID 51 for a *different* UUID.
        // WorldModel must retire the old rig and its worn mesh before
        // the renderer uploads its next frame's avatar/scene proxies.
        region_objects.update_avatar_transform(
            "old-remote",{4.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,1.0f},
            51U,true);
        region_objects.upsert_object(root);
        renderer->set_avatar_proxies({avatar});
        renderer->set_scene_objects({attached,sentinel});
        const auto reused_before=capture(0.22);
        if(differences(reused_before,empty_scene)<25U)failure=19;
        region_objects.update_avatar_transform(
            "new-remote",{6.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,1.0f},
            51U,true);
        if(region_objects.avatars().contains("old-remote")||
           !region_objects.avatars().contains("new-remote")||
           region_objects.objects().contains(200U)||
           region_objects.remote_avatar_local_id_rebinds()!=1U)
            failure=20;
        renderer->set_avatar_proxies({});
        renderer->set_scene_objects({sentinel});
        const auto reused_after=capture(0.22);
        const auto orphan_rgb=differences(empty_scene,reused_after);
        std::cout<<"REUSED_AVATAR_LOCALID_STALE_GPU_PIXELS="
                 <<orphan_rgb<<"\n";
        if(orphan_rgb!=0U)failure=21;
        if(glGetError()!=GL_NO_ERROR)failure=6;
    }
    if(renderer)renderer->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    if(!failure)std::cout<<"OGL_JOINT_ATTACHMENT_GPU_PASS\n";
    return failure;
}
