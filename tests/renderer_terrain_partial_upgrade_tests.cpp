/* This Source Code Form is subject to the Mozilla Public License, v. 2.0. */
#include "nexverse/render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>
int main()
{
    if(!glfwInit())return 77;
    glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    auto* w=glfwCreateWindow(320,200,"Terrain partial upgrade",nullptr,nullptr);
    if(!w){glfwTerminate();return 77;}
    glfwMakeContextCurrent(w);
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {glfwDestroyWindow(w);glfwTerminate();return 1;}
    int error=0;
    {
        using namespace nexverse::render;
        auto r=create_legacy_opengl_renderer();
        if(!r||!r->initialize(320,200))error=2;
        if(!error)
        {
            RenderTerrain terrain;
            terrain.generation=11U;
            terrain.revision=1U;
            terrain.material_revision=3U;
            terrain.width=4U;terrain.height=4U;
            terrain.patch_size=4U;
            terrain.valid_patches={1U};
            terrain.heights=std::vector<float>(16U,10.0f);
            terrain.terrain_height_range={12.f,12.f,12.f,12.f};
            r->set_terrain(terrain);
            r->set_camera({{1.5f,1.5f,18.f},
                           {0.f,0.f,-1.f},
                           {0.f,1.f,0.f},
                           {1.f,0.f,0.f},60.f,0.1f,40.f});
            RenderTerrainTextures tex;
            tex.generation=11U;tex.material_revision=3U;
            tex.detail[0]={2U,2U,std::vector<std::uint8_t>(16U,240U)};
            const auto draw=[&]()
            {
                r->begin_frame();
                r->render_world(0.0);
                r->end_frame();
                return r->diagnostics();
            };
            r->set_terrain_textures(tex);
            auto before=draw();
            if(before.terrain_textured_draw_calls!=1U||
               before.terrain_real_detail_layers!=1U)error=3;
            // Retry supplied another REAL texture during SAME epoch.
            tex.detail[1]={2U,2U,std::vector<std::uint8_t>(16U,100U)};
            r->set_terrain_textures(tex);
            const auto upgraded=draw();
            if(upgraded.terrain_textured_draw_calls!=1U||
               upgraded.terrain_real_detail_layers!=2U)
            {
                std::cerr<<"GL upgrade draw="<<upgraded.terrain_textured_draw_calls
                    <<" real="<<upgraded.terrain_real_detail_layers<<"\n";
                error=4;
            }
            // A late downgraded packet may not destroy good GL data.
            tex.detail[1]={};
            r->set_terrain_textures(tex);
            if(draw().terrain_real_detail_layers!=2U)error=5;
            // A NEW material revision genuinely invalidates old layers.
            tex.material_revision=4U;
            terrain.material_revision=4U;
            terrain.revision=2U;
            r->set_terrain(terrain);
            r->set_terrain_textures(tex);
            if(draw().terrain_real_detail_layers!=1U)error=6;
            if(glGetError()!=GL_NO_ERROR)error=7;
            r->shutdown();
        }
    }
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(w);
    glfwTerminate();
    if(error){std::cerr<<"Terrain GPU partial upgrade failure "<<error<<"\n";return error;}
    std::cout<<"Actual hidden NVIDIA GL terrain SAME-revision upload upgrade / no downgrade PASS\n";
    return 0;
}
