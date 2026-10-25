#include "nexverse/render/renderer.hpp"
#include "nexverse/render/avatar_attachment_anchor.hpp"
#include "nexverse/ui/world_scene_proof.hpp"
#include "nexverse/ui/avatar_pipeline_proof.hpp"
#include "nexverse/ui/asset_stream_proof.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <array>
#include <algorithm>
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
    // Attachment parent binding is based on simulator local IDs,
    // never on remote UUID heuristics or absolute object position.
    const std::array<RenderAvatarProxy,2U> anchors{{
        {{4.0f,2.0f,1.0f},{0.0f,0.0f,0.0f,1.0f},true,{},51U},
        {{8.0f,4.0f,1.0f},{0.0f,0.0f,0.0f,1.0f},false,{},52U}
    }};
    if(find_attachment_avatar_parent(anchors,0U) ||
       find_attachment_avatar_parent(anchors,999U) ||
       !find_attachment_avatar_parent(anchors,51U) ||
       find_attachment_avatar_parent(anchors,51U)->position.x!=4.0f ||
       !find_attachment_avatar_parent(anchors,52U) ||
       find_attachment_avatar_parent(anchors,52U)->position.x!=8.0f)
        return 23;
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
                // The scene must draw *before any terrain set_terrain*
                // call, as when OpenGenesisLINK sends LLUDP objects ahead
                // of delayed LayerData. Use real GL counters in the HUD
                // evidence formatter, never fake reception as visibility.
                const nexverse::ui::WorldSceneProof proof{
                    false,0U,1U,first.scene_draw_calls,
                    first.scene_missing_mesh_draw_calls};
                if(first.scene_mesh_draw_calls!=3U ||
                   first.scene_missing_mesh_objects!=0U ||
                   nexverse::ui::describe_terrain_progress(proof)!=
                       "Terrain 0/256 wartet" ||
                   nexverse::ui::describe_world_draw_progress(proof)!=
                       "GL 3 Draw | MeshFehlt 0" ||
                   glGetError()!=GL_NO_ERROR)error=4;
            }
            // The self avatar is a provisional humanoid proxy, not a
            // real baked/skinned appearance. Verify actual GL draw calls.
            if(!error)
            {
                renderer->set_avatar_proxies({{
                    {3.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,1.0f},true}});
                const auto with_avatar=render();
                if(with_avatar.avatar_proxy_draw_calls!=7U||
                   with_avatar.self_avatar_proxy_draw_calls!=7U ||
                   with_avatar.scene_mesh_draw_calls!=3U||
                   glGetError()!=GL_NO_ERROR)error=18;
                // A baked texture already resident in the shared
                // GetTexture GPU table must be sampled by all 7 body
                // proxy draw calls; absent assets revert to color tint.
                if(!renderer->set_object_texture("avatar-bake",opaque))
                    error=20;
                if(!error)
                {
                    RenderAvatarProxy textured;
                    textured.position={3.0f,0.0f,0.0f};
                    textured.is_self=true;
                    textured.local_id=53U;
                    textured.baked_texture_ids={
                        "avatar-bake","avatar-bake","avatar-bake"};
                    renderer->set_avatar_proxies({textured});
                    const auto baked_frame=render();
                    if(baked_frame.avatar_proxy_draw_calls!=7U ||
                       baked_frame.avatar_baked_texture_draw_calls!=7U ||
                       baked_frame.self_avatar_baked_texture_draw_calls!=7U ||
                       glGetError()!=GL_NO_ERROR)error=21;
                    renderer->remove_object_texture("avatar-bake");
                    const auto fallback=render();
                    if(fallback.avatar_proxy_draw_calls!=7U ||
                       fallback.avatar_baked_texture_draw_calls!=0U ||
                       fallback.self_avatar_baked_texture_draw_calls!=0U ||
                       glGetError()!=GL_NO_ERROR)error=22;
                }
                // Three complete Firestorm-layout bind-pose meshes replace
                // the 7 box/sphere proxy draw calls and sample the original
                // baked Head/Upper/Lower slots through the same shader.
                if(!renderer->set_object_texture("avatar-bake",opaque))
                    error=24;
                RenderMeshGeometry body;
                body.vertices={
                    0.0f,0.0f,1.7f, 0.0f,0.0f,1.0f, 0.0f,0.0f,
                    0.0f,0.1f,1.7f, 0.0f,0.0f,1.0f, 0.0f,1.0f,
                    0.1f,0.0f,1.7f, 0.0f,0.0f,1.0f, 1.0f,0.0f};
                body.indices={0U,1U,2U};
                // Broken Firestorm skin metadata must not overwrite an
                // existing GPU mesh and must not perform invalid GL work.
                RenderMeshGeometry invalid_body=body;
                invalid_body.skin_weights={1.0f,1.0f}; // too short
                invalid_body.skin_palette={0U,1U};
                if(renderer->set_avatar_body_mesh(0U,invalid_body))
                    error=33;
                invalid_body.skin_weights={3.0f,3.0f,3.0f};
                if(renderer->set_avatar_body_mesh(0U,invalid_body))
                    error=34;
                for(std::size_t slot=0U;slot<3U;++slot)
                    if(!renderer->set_avatar_body_mesh(slot,body))
                        error=25;
                if(!error)
                {
                    const auto anatomical=render();
                    // Real hidden GLFW/OpenGL 3.3 context, not a static
                    // source check: three bound original body-bake GL
                    // submissions and a valid live read-only preview name.
                    if(anatomical.avatar_anatomical_mesh_draw_calls!=3U ||
                       anatomical.self_avatar_anatomical_mesh_draw_calls!=3U ||
                       anatomical.avatar_proxy_draw_calls!=0U ||
                       anatomical.avatar_baked_texture_draw_calls!=3U ||
                       anatomical.self_avatar_body_bake_gl_draws!=
                           (std::array<std::uint64_t,3U>{1U,1U,1U}) ||
                       renderer->avatar_bake_preview_handle("avatar-bake")==0U ||
                       renderer->avatar_bake_preview_handle("missing")!=0U ||
                       glGetError()!=GL_NO_ERROR)error=26;
                    // This GPU fixture must actually resolve an asynchronous
                    // GL_ANY_SAMPLES_PASSED result (not just report GL draw
                    // calls). Query can finish 1-2 frames later; glFlush
                    // here is TEST ONLY, never on the live render path.
                    if(!error)
                    {
                        bool result_observed=false;
                        for(int sample_frame=0;sample_frame<6;++sample_frame)
                        {
                            // Deterministic fixture only: force the GPU
                            // to retire its pending GL queries. Production
                            // rendering never uses glFinish or blocking
                            // GL_QUERY_RESULT.
                            glFinish();
                            const auto queried=render();
                            if(queried.self_avatar_visible_samples_known)
                            {
                                result_observed=true;
                                break;
                            }
                        }
                        if(!result_observed)error=43;
                        if(!error)
                        {
                            // Region unchanged: replace just one current
                            // server bake while the last GPU query reports
                            // known. Old-skin queries must be discarded.
                            RenderAvatarProxy changed_self{};
                            changed_self.position={3.0f,0.0f,0.0f};
                            changed_self.is_self=true;
                            changed_self.baked_texture_ids={
                                "avatar-bake","new-upper","avatar-bake"};
                            renderer->set_avatar_proxies({changed_self});
                            const auto after_rebake=render();
                            if(after_rebake.self_avatar_visible_samples_known)
                                error=48;
                            // Identical source on the NEXT frame must NOT
                            // invalidate every ongoing query indefinitely.
                            renderer->set_avatar_proxies({changed_self});
                            glFinish(); // isolated test only
                            const auto same_bakes=render();
                            if(error==0&&
                               !same_bakes.self_avatar_visible_samples_known)
                                error=49;
                            // The old source is also a distinct incarnation.
                            changed_self.baked_texture_ids={
                                "avatar-bake","avatar-bake","avatar-bake"};
                            renderer->set_avatar_proxies({changed_self});
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=50;
                            // Re-uploading the SAME server texture UUID
                            // can change alpha-tested coverage even without
                            // a new bake ID. Old depth result must expire.
                            glFinish();
                            (void)render();
                            if(error==0&&
                               !renderer->set_object_texture(
                                   "avatar-bake",opaque))error=51;
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=52;
                            // Unrelated object updates must NOT retire the
                            // self visibility proof.
                            glFinish();
                            (void)render();
                            if(error==0&&
                               !renderer->set_object_texture(
                                   "unrelated-asset",opaque))error=53;
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=54;
                            // REAL GPU test: camera unchanged but the Self
                            // avatar moves. An old depth sample is no
                            // longer proof at the new world location.
                            changed_self.position={3.12f,0.0f,0.0f};
                            renderer->set_avatar_proxies({changed_self});
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=55;
                            renderer->set_avatar_proxies({changed_self});
                            glFinish(); // tests only
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=56;
                            // Rotation changes geometry even if no new
                            // server bake or camera movement happened.
                            changed_self.rotation={
                                0.0f,0.0f,0.08715574f,0.9961947f};
                            renderer->set_avatar_proxies({changed_self});
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=57;
                            renderer->set_avatar_proxies({changed_self});
                            glFinish();
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=58;
                            // A fresh original Firestorm Upper upload is
                            // also a different silhouette at the SAME
                            // server-bake ID. Reject previous GL depth.
                            if(error==0&&
                               !renderer->set_avatar_body_mesh(1U,body))
                                error=59;
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=60;
                            glFinish();
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=61;
                            renderer->resize(420,230);
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=62;
                            glFinish();
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=63;
                            // An identical resize does not invalidate
                            // the new current-viewport proof each frame.
                            renderer->resize(420,230);
                            if(error==0&&
                               !render().self_avatar_visible_samples_known)
                                error=64;
                            renderer->resize(400,230);
                            if(error==0&&
                               render().self_avatar_visible_samples_known)
                                error=65;
                        }
                        // The SAME camera GL sample must become UNKNOWN
                        // when the user pans/zooms/rotates. Old GPU samples
                        // are no longer evidence for a new camera pose.
                        renderer->set_camera({{0.25f,0.0f,0.0f},
                            {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                            {0.0f,0.0f,1.0f},60.0f,0.1f,40.0f});
                        const auto moved_camera=render();
                        if(moved_camera.self_avatar_visible_samples_known)
                            error=47;
                        renderer->set_camera({{0.0f,0.0f,0.0f},
                            {1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},
                            {0.0f,0.0f,1.0f},60.0f,0.1f,40.0f});
                        renderer->reset_self_avatar_visibility();
                        const auto after_reset=render();
                        if(after_reset.self_avatar_visible_samples_known)
                            error=44;
                    }
                    // After the GL render, combine actual GPU Draw counters
                    // with a server-like AvatarAppearance TextureEntry's
                    // currently-resident baked UUIDs. This test still uses
                    // fixtures, not an authenticated real grid packet.
                    nexverse::protocol::AvatarBakedTextures fake_server{};
                    fake_server.ids[0]="avatar-bake";
                    fake_server.ids[1]="avatar-bake";
                    fake_server.ids[2]="avatar-bake";
                    nexverse::ui::AvatarPipelineProof pipeline{};
                    pipeline.appearance_received=true;
                    pipeline.bakes=nexverse::protocol::audit_avatar_bake_evidence(
                        fake_server,{"avatar-bake"});
                    pipeline.original_body_draws=
                        anatomical.self_avatar_anatomical_mesh_draw_calls;
                    pipeline.body_bake_draws=
                        anatomical.self_avatar_body_bake_gl_draws;
                    if(nexverse::ui::describe_avatar_pipeline(pipeline)!=
                           "Server TE 3/11 GPU 3/11" ||
                       nexverse::ui::describe_avatar_body_pipeline(pipeline)!=
                           "H:G U:G L:G | LLM 3 BakeGL 3")
                        error=39;
                    // Read back the actual GL 3.3 texture stored under
                    // the server-like bake ID. Draw counters alone cannot
                    // prove GPU contents, and synthetic F9 textures would
                    // incorrectly pass a draw-only test.
                    const GLuint baked_handle=renderer->
                        avatar_bake_preview_handle("avatar-bake");
                    // A valid actual GL name backs our GPU read-only
                    // World stream label: duplicated scene references
                    // count only one unique texture, not two.
                    struct TextureSample {std::string id;};
                    const std::array<TextureSample,2U> samples{{
                        {"avatar-bake"},{"avatar-bake"}}};
                    const auto coverage=nexverse::ui::asset_gpu_coverage(
                        samples,baked_handle!=0U?
                          std::unordered_set<std::string>{"avatar-bake"}:
                          std::unordered_set<std::string>{});
                    if(coverage.requested!=1U||coverage.gpu_resident!=1U)
                        error=41;
                    if(!error)
                    {
                        GLint old_unit=0;
                        glGetIntegerv(GL_ACTIVE_TEXTURE,&old_unit);
                        glActiveTexture(GL_TEXTURE0);
                        GLint old_texture=0;
                        glGetIntegerv(GL_TEXTURE_BINDING_2D,&old_texture);
                        glBindTexture(GL_TEXTURE_2D,baked_handle);
                        GLint tex_width=0,tex_height=0;
                        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,
                            GL_TEXTURE_WIDTH,&tex_width);
                        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,
                            GL_TEXTURE_HEIGHT,&tex_height);
                        std::array<std::uint8_t,16U> gpu_pixels{};
                        glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,
                            GL_UNSIGNED_BYTE,gpu_pixels.data());
                        const GLenum readback_error=glGetError();
                        glBindTexture(GL_TEXTURE_2D,
                            static_cast<GLuint>(old_texture));
                        glActiveTexture(static_cast<GLenum>(old_unit));
                        if(tex_width!=2||tex_height!=2||
                           readback_error!=GL_NO_ERROR||
                           !std::equal(gpu_pixels.begin(),
                               gpu_pixels.end(),opaque.pixels.begin()))
                            error=37;
                    }
                    // With installed Firestorm resource files, the GPU
                    // test also uploads the real 7,186-triangle anatomical
                    // bind-pose meshes (never a synthetic primitive proxy).
                    if(const auto dir=
                       nexverse::assets::find_firestorm_avatar_character_directory();
                       dir)
                    {
                        const auto rig=nexverse::assets::load_firestorm_avatar_rig(
                            *dir);
                        if(!rig)error=31;
                        RenderAvatarBindRig gpu_rig;
                        if(rig)
                        {
                            for(const auto& j:rig->joints)
                                gpu_rig.joints.push_back({
                                    j.name,j.parent,
                                    {j.local[0],j.local[1],j.local[2]},
                                    {j.bind_world[0],j.bind_world[1],
                                     j.bind_world[2]}});
                            renderer->set_avatar_bind_rig(gpu_rig);
                        }
                        const std::array<std::string,3U> filenames{
                            "avatar_head.llm","avatar_upper_body.llm",
                            "avatar_lower_body.llm"};
                        for(std::size_t slot=0U;slot<filenames.size();++slot)
                        {
                            const auto asset=
                                nexverse::assets::load_firestorm_avatar_llm(
                                    *dir/filenames[slot]);
                            if(!asset || asset->indices.size()<3000U)
                            {
                                error=28;
                                break;
                            }
                            const auto palette=
                                nexverse::assets::firestorm_joint_render_palette(
                                    *rig,asset->joint_names);
                            if(!palette || !renderer->set_avatar_body_mesh(slot,{
                                asset->vertices,asset->indices,{},
                                asset->skin_weights,*palette}))
                            {
                                error=29;
                                break;
                            }
                        }
                        if(!error)
                        {
                            const auto original=render();
                            if(original.self_avatar_anatomical_mesh_draw_calls!=3U ||
                               original.avatar_locally_animated_skin_draw_calls!=0U ||
                               original.self_avatar_weighted_skin_draw_calls!=3U ||
                               original.self_avatar_baked_texture_draw_calls!=3U ||
                               glGetError()!=GL_NO_ERROR)error=30;
                            RenderAvatarProxy walking_avatar;
                            walking_avatar.position={3.0f,0.0f,0.0f};
                            walking_avatar.is_self=true;
                            walking_avatar.moving=true;
                            walking_avatar.baked_texture_ids={
                                "avatar-bake","avatar-bake","avatar-bake"};
                            renderer->set_avatar_proxies({walking_avatar});
                            const auto walking=render();
                            if(walking.self_avatar_weighted_skin_draw_calls!=3U ||
                               walking.avatar_locally_animated_skin_draw_calls!=3U ||
                               walking.self_avatar_anatomical_mesh_draw_calls!=3U ||
                               glGetError()!=GL_NO_ERROR)error=32;
                            // Server-selected run/sit/fly/jump/hover poses
                            // must remain weighted (no new proxy boxes),
                            // even with no local WASD/position movement.
                            walking_avatar.moving=false;
                            walking_avatar.animation_from_server=true;
                            for(const auto mode:{
                                nexverse::protocol::AvatarMotion::run,
                                nexverse::protocol::AvatarMotion::sit,
                                nexverse::protocol::AvatarMotion::jump,
                                nexverse::protocol::AvatarMotion::fly,
                                nexverse::protocol::AvatarMotion::hover})
                            {
                                walking_avatar.motion=mode;
                                renderer->set_avatar_proxies({walking_avatar});
                                const auto active=render();
                                if(active.avatar_server_selected_skin_draw_calls!=3U||
                                   active.avatar_locally_animated_skin_draw_calls!=3U||
                                   active.self_avatar_weighted_skin_draw_calls!=3U||
                                   active.avatar_proxy_draw_calls!=0U||
                                   glGetError()!=GL_NO_ERROR)
                                {
                                    error=35;break;
                                }
                            }
                            if(!error)
                            {
                                auto clip=std::make_shared<
                                    nexverse::assets::FirestormKeyframeMotion>();
                                clip->duration=2.0f;
                                clip->loop=true;
                                clip->loop_in=0.5f;
                                clip->loop_out=1.5f;
                                nexverse::assets::MotionJoint knee;
                                knee.name="mKneeLeft";
                                knee.rotations={{
                                    0.0f,{0.0f,0.0f,0.0f,1.0f}},
                                    {2.0f,{0.0f,0.38f,0.0f,0.925f}}};
                                clip->joints.push_back(std::move(knee));
                                walking_avatar.motion=
                                    nexverse::protocol::AvatarMotion::idle;
                                walking_avatar.animation_from_server=false;
                                walking_avatar.keyframe_clip=std::move(clip);
                                // Actual LLUDP clip playback is anchored
                                // to the current AnimationSequenceID, not
                                // the viewer process uptime.
                                walking_avatar.keyframe_elapsed_seconds=0.8f;
                                renderer->set_avatar_proxies({walking_avatar});
                                const auto keyframed=render();
                                if(keyframed.avatar_keyframe_skin_draw_calls!=3U||
                                   keyframed.self_avatar_weighted_skin_draw_calls!=3U||
                                   keyframed.avatar_server_selected_skin_draw_calls!=0U||
                                   glGetError()!=GL_NO_ERROR)error=36;
                            }
                        }
                    }
                    renderer->remove_object_texture("avatar-bake");
                    if(glIsTexture(baked_handle)!=GL_FALSE)
                        error=38;
                    const auto evicted_coverage=
                        nexverse::ui::asset_gpu_coverage(
                            samples,renderer->avatar_bake_preview_handle(
                                "avatar-bake")!=0U?
                                std::unordered_set<std::string>{"avatar-bake"}:
                                std::unordered_set<std::string>{});
                    if(evicted_coverage.requested!=1U||
                       evicted_coverage.gpu_resident!=0U)
                        error=42;
                    const auto untextured=render();
                    if(!error)
                    {
                        pipeline.bakes=
                            nexverse::protocol::audit_avatar_bake_evidence(
                                fake_server,{});
                        pipeline.original_body_draws=
                            untextured.self_avatar_anatomical_mesh_draw_calls;
                        pipeline.body_bake_draws=
                            untextured.self_avatar_body_bake_gl_draws;
                        if(nexverse::ui::describe_avatar_body_pipeline(
                            pipeline)!="H:S U:S L:S | LLM 3 BakeGL 0")
                            error=40;
                    }
                    if(untextured.avatar_anatomical_mesh_draw_calls!=3U ||
                       untextured.avatar_baked_texture_draw_calls!=0U ||
                       untextured.self_avatar_body_bake_gl_draws!=
                           (std::array<std::uint64_t,3U>{0U,0U,0U}) ||
                       renderer->avatar_bake_preview_handle("avatar-bake")!=0U ||
                       glGetError()!=GL_NO_ERROR)error=27;
                }
                // Truly behind-camera ORIGINAL LLM geometry still
                // submits GL draws but zero fragments should survive.
                // This proves the query is not just a draw counter.
                renderer->reset_self_avatar_visibility();
                RenderAvatarProxy behind_avatar;
                behind_avatar.position={-4.0f,0.0f,0.0f};
                behind_avatar.is_self=true;
                renderer->set_avatar_proxies({behind_avatar});
                bool hidden_known=false;
                for(int check=0;check<6 && !error;++check)
                {
                    glFinish(); // Test only; production remains async
                    const auto hidden=render();
                    if(hidden.self_avatar_visible_samples_known)
                    {
                        hidden_known=true;
                        if(hidden.self_avatar_visible_samples_passed||
                           hidden.self_avatar_anatomical_mesh_draw_calls!=3U)
                            error=45;
                        break;
                    }
                }
                if(!hidden_known)error=46;
                renderer->reset_self_avatar_visibility();
                renderer->set_avatar_proxies({});
                const auto cleared_avatar=render();
                if(cleared_avatar.avatar_proxy_draw_calls!=0U ||
                   cleared_avatar.self_avatar_proxy_draw_calls!=0U||
                   cleared_avatar.self_avatar_visible_samples_known||
                   glGetError()!=GL_NO_ERROR)error=19;
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
            // Repeated live-like material changes on two overlapping
            // multi-face objects. Test draw routing, not pixel-perfect PBR.
            RenderSceneObject overlay=obj;
            overlay.local_id=201U;
            overlay.position={0.0f,0.14f,0.0f};
            for(unsigned frame=0U;frame<48U&&!error;++frame)
            {
                for(unsigned face=0U;face<3U;++face)
                {
                    auto& first=obj.face_materials[face];
                    auto& second=overlay.face_materials[face];
                    if(!first||!second){error=8;break;}
                    first->color[3]=(frame+face)%2U?0.33f:1.0f;
                    first->glow=static_cast<float>((frame+face)%5U)*0.1f;
                    first->rotation=
                        static_cast<float>(frame%11U)*0.11f;
                    first->texgen_planar=frame%3U==0U;
                    first->repeat_v=1.0f+static_cast<float>(face);
                    if(face==2U)
                    {
                        first->diffuse_alpha_mode=2U;
                        first->alpha_mask_cutoff=128U;
                        first->color[3]=1.0f;
                    }
                    second->color[3]=(frame+face)%3U?0.85f:0.25f;
                    second->fullbright=frame%2U==0U;
                    second->texture_id=face==1U?"alpha":"opaque";
                    second->offset_u=static_cast<float>(frame%7U)*0.04f;
                }
                if(error)break;
                renderer->set_scene_objects({obj,overlay});
                renderer->begin_frame();
                renderer->render_world(static_cast<double>(frame)*0.03);
                renderer->end_frame();
                const auto diagnostics=renderer->diagnostics();
                if(diagnostics.scene_mesh_draw_calls!=6U ||
                   diagnostics.scene_opaque_draw_calls==0U ||
                   diagnostics.scene_alpha_draw_calls==0U ||
                   diagnostics.scene_alpha_mask_draw_calls==0U ||
                   diagnostics.scene_opaque_draw_calls+
                       diagnostics.scene_alpha_draw_calls!=6U ||
                   diagnostics.scene_missing_mesh_objects!=0U ||
                   diagnostics.scene_draw_calls!=
                       diagnostics.scene_mesh_draw_calls+
                           diagnostics.scene_prim_draw_calls ||
                   glGetError()!=GL_NO_ERROR)error=9;
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
