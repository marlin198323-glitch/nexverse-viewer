#include "nexverse/render/renderer.hpp"
#include "nexverse/render/avatar_attachment_anchor.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
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
                if(first.scene_mesh_draw_calls!=3U ||
                   first.scene_missing_mesh_objects!=0U ||
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
                for(std::size_t slot=0U;slot<3U;++slot)
                    if(!renderer->set_avatar_body_mesh(slot,body))
                        error=25;
                if(!error)
                {
                    const auto anatomical=render();
                    if(anatomical.avatar_anatomical_mesh_draw_calls!=3U ||
                       anatomical.self_avatar_anatomical_mesh_draw_calls!=3U ||
                       anatomical.avatar_proxy_draw_calls!=0U ||
                       anatomical.avatar_baked_texture_draw_calls!=3U ||
                       glGetError()!=GL_NO_ERROR)error=26;
                    // With installed Firestorm resource files, the GPU
                    // test also uploads the real 7,186-triangle anatomical
                    // bind-pose meshes (never a synthetic primitive proxy).
                    if(const auto dir=
                       nexverse::assets::find_firestorm_avatar_character_directory();
                       dir)
                    {
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
                            if(!renderer->set_avatar_body_mesh(slot,{
                                asset->vertices,asset->indices,{}}))
                            {
                                error=29;
                                break;
                            }
                        }
                        if(!error)
                        {
                            const auto original=render();
                            if(original.self_avatar_anatomical_mesh_draw_calls!=3U ||
                               original.self_avatar_baked_texture_draw_calls!=3U ||
                               glGetError()!=GL_NO_ERROR)error=30;
                        }
                    }
                    renderer->remove_object_texture("avatar-bake");
                    const auto untextured=render();
                    if(untextured.avatar_anatomical_mesh_draw_calls!=3U ||
                       untextured.avatar_baked_texture_draw_calls!=0U ||
                       glGetError()!=GL_NO_ERROR)error=27;
                }
                renderer->set_avatar_proxies({});
                const auto cleared_avatar=render();
                if(cleared_avatar.avatar_proxy_draw_calls!=0U ||
                   cleared_avatar.self_avatar_proxy_draw_calls!=0U||
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
