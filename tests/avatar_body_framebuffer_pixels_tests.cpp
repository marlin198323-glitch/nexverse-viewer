/* MPL-2.0 */
#include "nexverse/render/renderer.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <iostream>
using namespace nexverse;
int main()
{
  auto dir=assets::find_firestorm_avatar_character_directory();
  if(!dir)return 77;
  auto rig=assets::load_firestorm_avatar_rig(*dir);
  if(!rig)return 1;
  if(!glfwInit())return 77;
  glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  auto* window=glfwCreateWindow(400,300,"Avatar pixels",nullptr,nullptr);
  if(!window){glfwTerminate();return 77;}
  glfwMakeContextCurrent(window);
  int error=0;
  if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))error=2;
  auto renderer=render::create_legacy_opengl_renderer();
  if(!error&&!renderer->initialize(400,300))error=3;
  if(!error)
  {
    render::RenderAvatarBindRig bound;
    for(const auto& joint:rig->joints)
      bound.joints.push_back({joint.name,joint.parent,
        {joint.local[0],joint.local[1],joint.local[2]},
        {joint.bind_world[0],joint.bind_world[1],joint.bind_world[2]}});
    renderer->set_avatar_bind_rig(bound);
    for(std::size_t slot=0;slot<3;++slot)
    {
      const std::array<std::string,3> names={
        "avatar_head.llm","avatar_upper_body.llm","avatar_lower_body.llm"};
      auto a=assets::load_firestorm_avatar_llm(*dir/names[slot]);
      if(!a||!a->valid()){error=4;break;}
      auto palette=assets::firestorm_joint_render_palette(*rig,a->joint_names);
      if(!palette||!renderer->set_avatar_body_mesh(slot,
          {a->vertices,a->indices,{},a->skin_weights,*palette}))
      {error=5;break;}
    }
  }
  if(!error)
  {
    renderer->set_camera({{0,0,1},{1,0,0},{0,1,0},{0,0,1},65,0.1f,40});
    auto draw=[&]{
      renderer->begin_frame();renderer->render_world(0);renderer->end_frame();
      glFinish();
      std::vector<unsigned char> rgb(400*300*4);
      glReadBuffer(GL_BACK);
      glReadPixels(0,0,400,300,GL_RGBA,GL_UNSIGNED_BYTE,rgb.data());
      return rgb;
    };
    auto empty=draw();
    render::RenderAvatarProxy self{};
    self.position={3,0,0};self.is_self=true;
    renderer->set_avatar_proxies({self});
    auto plain=draw();
    auto difference=[](const auto& a,const auto& b){
      std::size_t count=0;
      for(std::size_t i=0;i<a.size();i+=4)
        if(a[i]!=b[i]||a[i+1]!=b[i+1]||a[i+2]!=b[i+2])++count;
      return count;
    };
    auto initial=difference(empty,plain);
    auto d=renderer->diagnostics();
    std::cout<<"ORIGINAL_AVATAR_PIXELS="<<initial<<"\n";
    if(initial<100||d.self_avatar_anatomical_mesh_draw_calls!=3||
       d.self_avatar_weighted_skin_draw_calls!=3)error=6;
    render::RenderImageRgba8 pixels{4,4,std::vector<std::uint8_t>(64,255)};
    for(std::size_t i=0;i<pixels.pixels.size();i+=4)
    {pixels.pixels[i]=20;pixels.pixels[i+1]=220;pixels.pixels[i+2]=60;}
    if(!error&&!renderer->set_object_texture("fixture-bake",pixels))error=7;
    for(std::size_t slot=0;slot<3 && !error;++slot)
    {
      self.baked_texture_ids={};
      self.baked_texture_ids[slot]="fixture-bake";
      renderer->set_avatar_proxies({self});
      auto only=draw();
      auto count=difference(plain,only);
      std::cout<<"BODY_PART_BAKE_PIXELS_"<<slot<<"="<<count<<"\n";
      if(count<25)error=10+static_cast<int>(slot);
    }
    // Three DIFFERENT server-baked RGBA textures must reach the
    // original head/upper/lower Firestorm .llm UVs independently.
    // Prior tests used just one shared flat green texture.
    const std::array<std::string,3U> bake_ids{
        "head-bake-live","upper-bake-live","lower-bake-live"};
    for(std::size_t slot=0U;slot<3U && !error;++slot)
    {
        render::RenderImageRgba8 paint{
            4,4,std::vector<std::uint8_t>(64,255)};
        for(std::size_t i=0U;i<paint.pixels.size();i+=4)
        {
            paint.pixels[i+0]=(slot==0U?240U:20U);
            paint.pixels[i+1]=(slot==1U?240U:20U);
            paint.pixels[i+2]=(slot==2U?240U:20U);
        }
        if(!renderer->set_object_texture(bake_ids[slot],paint))
            error=14+static_cast<int>(slot);
    }
    if(!error)
    {
        self.baked_texture_ids=bake_ids;
        renderer->set_avatar_proxies({self});
        const auto three_independent=draw();
        const auto native_draw=renderer->diagnostics();
        const auto independent_pixels=difference(plain,three_independent);
        std::cout<<"THREE_INDEPENDENT_FIRESTORM_BAKE_PIXELS="
                 <<independent_pixels<<"\n";
        if(independent_pixels<100U ||
           native_draw.self_avatar_body_bake_gl_draws!=
              (std::array<std::uint64_t,3>{1U,1U,1U}))
            error=17;
        // Replacing only Upper must NOT accidentally rebind Head/Lower
        // to a stale slot-4 GL texture from the previous draw.
        render::RenderImageRgba8 upper_replacement{
            4,4,std::vector<std::uint8_t>(64,255)};
        for(std::size_t i=0U;i<upper_replacement.pixels.size();i+=4)
        {
            upper_replacement.pixels[i]=240U;
            upper_replacement.pixels[i+1]=240U;
            upper_replacement.pixels[i+2]=30U;
        }
        if(!renderer->set_object_texture("upper-replacement",
                                        upper_replacement))
            error=18;
        self.baked_texture_ids[1]="upper-replacement";
        renderer->set_avatar_proxies({self});
        const auto changed_upper=draw();
        if(difference(three_independent,changed_upper)<20U)
            error=19;
        renderer->remove_object_texture("head-bake-live");
        renderer->set_avatar_proxies({self});
        const auto head_evicted=draw();
        if(difference(head_evicted,changed_upper)<20U ||
           renderer->diagnostics().self_avatar_body_bake_gl_draws!=
              (std::array<std::uint64_t,3>{0U,1U,1U}))
            error=20;
        renderer->remove_object_texture("upper-bake-live");
        renderer->remove_object_texture("upper-replacement");
        renderer->remove_object_texture("lower-bake-live");
    }
    self.baked_texture_ids={"fixture-bake","fixture-bake","fixture-bake"};
    renderer->set_avatar_proxies({self});
    if(!error)
    {
      auto baked=draw();
      d=renderer->diagnostics();
      auto delta=difference(plain,baked);
      std::cout<<"ORIGINAL_BAKE_PIXELS="<<delta<<"\n";
      if(delta<100||d.self_avatar_body_bake_gl_draws!=
          (std::array<std::uint64_t,3>{1,1,1}))error=8;
    }
    if(!error)
    {
      // A real current self TE can withdraw all provisional bake refs
      // WITHOUT evicting already uploaded GPU texture objects. The
      // original Firestorm body must stop sampling the old skin on the
      // very next frame, not resurrect it from the residency cache.
      self.baked_texture_ids={};
      renderer->set_avatar_proxies({self});
      const auto withdrawn=draw();
      const auto draw_status=renderer->diagnostics();
      const auto ghost_pixels=difference(plain,withdrawn);
      std::cout<<"WITHDRAWN_SELF_BAKES_STALE_GPU_PIXELS="
               <<ghost_pixels<<"\n";
      if(ghost_pixels!=0U ||
         draw_status.self_avatar_baked_texture_draw_calls!=0U ||
         draw_status.self_avatar_body_bake_gl_draws!=
             (std::array<std::uint64_t,3>{0U,0U,0U}))
          error=21;
      self.baked_texture_ids={"fixture-bake","fixture-bake","fixture-bake"};
      renderer->set_avatar_proxies({self});
      if(difference(plain,draw())<100U)error=22;
    }
    if(!error)
    {
      renderer->remove_object_texture("fixture-bake");
      const auto after_evict=draw();
      const auto evicted=renderer->diagnostics();
      const auto stale_pixels=difference(plain,after_evict);
      std::cout<<"EVICTED_BAKE_STALE_RGB_PIXELS="<<stale_pixels<<"\n";
      if(stale_pixels!=0U || evicted.self_avatar_baked_texture_draw_calls!=0U)
        error=13;
    }
    if(glGetError()!=GL_NO_ERROR)error=9;
  }
  if(renderer)renderer->shutdown();
  glfwDestroyWindow(window);glfwTerminate();
  if(!error)std::cout<<"OGL_REAL_ORIGINAL_AVATAR_FRAMEBUFFER_PASS\n";
  return error;
}
