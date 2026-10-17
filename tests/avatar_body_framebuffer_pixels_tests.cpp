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
