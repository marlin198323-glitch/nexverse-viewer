/* MPL-2.0 */
#include "nexverse/render/renderer.hpp"
#include "nexverse/world/world_model.hpp"
#include "nexverse/protocol/lludp.hpp"
#include "nexverse/assets/firestorm_avatar_mesh.hpp"
#include "nexverse/assets/firestorm_avatar_rig.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <iostream>
#include <limits>
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
    if(!error)
    {
      // Real framebuffer, not a mock: malformed own position must never
      // produce GL pixels or a false original-skin draw. Recovery on the
      // next valid frame must restore original Firestorm body pixels.
      const float nan=std::numeric_limits<float>::quiet_NaN();
      auto poisoned=self;
      poisoned.position.x=nan;
      renderer->set_avatar_proxies({poisoned});
      const auto rejected=draw();
      const auto invalid=renderer->diagnostics();
      if(difference(empty,rejected)!=0U||
         !invalid.self_avatar_pose_invalid_dropped||
         invalid.avatar_pose_invalid_dropped!=1U||
         invalid.self_avatar_anatomical_mesh_draw_calls!=0U)
          error=34;
      if(!error)
      {
        // Broken orientation must not poison the already valid position.
        auto damaged=self;
        damaged.rotation={nan,0.0f,0.0f,1.0f};
        renderer->set_avatar_proxies({damaged});
        const auto fixed=draw();
        const auto recovered=renderer->diagnostics();
        if(difference(plain,fixed)!=0U||
           recovered.avatar_rotation_repaired!=1U||
           recovered.self_avatar_pose_invalid_dropped||
           recovered.self_avatar_anatomical_mesh_draw_calls!=3U)
            error=35;
      }
      if(!error)
      {
        // Dropping a malformed REMOTE quaternion/position cannot delete
        // or modify an otherwise valid self body in the same GL frame.
        auto remote=self;
        remote.is_self=false;
        remote.position.z=std::numeric_limits<float>::infinity();
        renderer->set_avatar_proxies({self,remote});
        const auto current=draw();
        const auto stats=renderer->diagnostics();
        if(difference(plain,current)!=0U||
           stats.avatar_pose_invalid_dropped!=1U||
           stats.self_avatar_pose_invalid_dropped||
           stats.self_avatar_anatomical_mesh_draw_calls!=3U)
            error=36;
      }
      renderer->set_avatar_proxies({self});
    }
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
        if(native_draw.self_avatar_body_drawn_bake_ids!=bake_ids)error=43;
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
      // End-to-end: authenticated modern Low158 -> COF-version gate ->
      // WorldModel self bake IDs -> original Firestorm body shaders.
      // A late OLD Low158 must not switch three genuinely GPU-resident
      // baked textures back to a previous outfit.
      auto wire_te=[](std::string_view asset)
      {
        std::vector<std::uint8_t> te(16U,0x11U);
        const auto uuid=protocol::uuid_to_bytes(asset);
        if(!uuid)return std::vector<std::uint8_t>{};
        for(const auto mask:{0x82U,0x84U,0x88U})
        {
          te.push_back(mask);te.push_back(0U);
          te.insert(te.end(),uuid->begin(),uuid->end());
        }
        te.insert(te.end(),{
          0U,255U,255U,255U,255U,0U,
          0U,0U,128U,63U,0U,0U,128U,63U,0U,
          0U,0U,0U,0U,0U,0U,0U,0U,
          0U,0U,0U,0U,0U,0U
        });
        return te;
      };
      const std::string old_id=
          "bbbbbbbb-2222-3333-4444-111111111111";
      const std::string new_id=
          "aaaaaaaa-2222-3333-4444-999999999999";
      const auto make_paint=[](std::uint8_t red,std::uint8_t green)
      {
        render::RenderImageRgba8 result{
            4,4,std::vector<std::uint8_t>(64U,255U)};
        for(std::size_t i=0U;i<64U;i+=4U)
        {
          result.pixels[i]=red;result.pixels[i+1]=green;
          result.pixels[i+2]=35U;
        }
        return result;
      };
      world::WorldModel skin;
      skin.apply_region({
          0x0000010000000200ULL,"NexVerse",{128,128,25},
          "127.0.0.1",9000U,"","",
          0U,0U,0U,13U,20.0f,"",256U,256U,true});
      skin.apply_agent({"11111111-2222-3333-4444-555555555555",
          "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",{},{},true});
      protocol::AvatarAppearanceMessage appearance;
      appearance.avatar_id=skin.agent().agent_id;
      appearance.visual_params={120U,121U,122U};
      appearance.appearance_data={{2U,202U,0U}};
      appearance.texture_entry=wire_te(new_id);
      const bool paint_loaded=
          renderer->set_object_texture(new_id,make_paint(235U,20U))&&
          renderer->set_object_texture(old_id,make_paint(20U,235U));
      if(!paint_loaded||!skin.apply_self_appearance(appearance)||
         skin.self_render_bakes()!=
             (std::array<std::string,3U>{new_id,new_id,new_id}))
          error=23;
      if(!error)
      {
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto new_skin=draw();
        if(difference(plain,new_skin)<100U)error=24;
        auto delayed=appearance;
        delayed.appearance_data.back().cof_version=201U;
        delayed.texture_entry=wire_te(old_id);
        if(skin.apply_self_appearance(delayed)||
           skin.stale_self_cof_appearances()!=1U)
            error=25;
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto after_old=draw();
        const auto stale_gpu=difference(new_skin,after_old);
        std::cout<<"STALE_COF_LOW158_OLD_SKIN_GPU_PIXELS="
                 <<stale_gpu<<"\n";
        if(stale_gpu!=0U)error=26;
        // Wire-compatible multiple AppearanceData records: trailing
        // COF=0 cannot let the OLD green body repaint the new outfit.
        auto padded_old=delayed;
        padded_old.appearance_data.push_back({2U,0U,0U});
        if(skin.apply_self_appearance(padded_old)||
           skin.stale_self_cof_appearances()!=2U)error=37;
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto after_padded_old=draw();
        if(difference(new_skin,after_padded_old)!=0U)error=38;
        // Real newer outfit is still allowed despite its own trailing
        // unknown COF row, and the actual GPU body pixels must change.
        delayed.appearance_data.back().cof_version=203U;
        delayed.appearance_data.push_back({2U,0U,0U});
        if(!skin.apply_self_appearance(delayed))error=27;
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto actually_newer=draw();
        if(difference(new_skin,actually_newer)<100U)error=28;
        // Both Firestorm AppearanceVersion formats can be interleaved.
        // After version 3 becomes most recent, old version 2 COF must
        // NEVER roll back already published GPU Head/Upper/Lower pixels.
        auto format3=appearance;
        format3.appearance_data={{3U,52U,0U}};
        if(!skin.apply_self_appearance(format3))error=39;
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto skin_v3=draw();
        if(difference(actually_newer,skin_v3)<100U)error=40;
        auto v2_stale=delayed;
        v2_stale.appearance_data.front().cof_version=202U;
        if(skin.apply_self_appearance(v2_stale))error=41;
        self.baked_texture_ids=skin.self_render_bakes();
        renderer->set_avatar_proxies({self});
        const auto after_format_replay=draw();
        if(difference(skin_v3,after_format_replay)!=0U)error=42;
      }
      renderer->remove_object_texture(old_id);
      renderer->remove_object_texture(new_id);
      self.baked_texture_ids={"fixture-bake",
                              "fixture-bake","fixture-bake"};
      renderer->set_avatar_proxies({self});
    }
    if(!error)
    {
      // A real own-avatar KillObject invalidates an authenticated
      // PROVISIONAL TextureEntry even while its GL asset stays resident.
      const auto uuid=protocol::uuid_to_bytes(
          "44444444-4444-4444-4444-444444444444");
      const auto own="11111111-2222-3333-4444-555555555555";
      const auto session="aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
      constexpr std::uint64_t region=0x0000010000000200ULL;
      world::WorldModel self_world;
      self_world.apply_region({region,"Sim",{128,128,25},
          "127.0.0.1",9000U,"","",0U,0U,0U,13U,
          20.0f,"",256U,256U,true});
      self_world.apply_agent({own,session,{},{},true});
      std::vector<std::uint8_t> te(16U,0x11U);
      if(!uuid)error=29;
      if(!error)
      {
        te.insert(te.end(),{0x82U,0U});
        te.insert(te.end(),uuid->begin(),uuid->end());
        te.insert(te.end(),{
            0U,255U,255U,255U,255U,0U,
            0U,0U,128U,63U,0U,0U,128U,63U,0U,
            0U,0U,0U,0U,0U,0U,0U,0U,
            0U,0U,0U,0U,0U,0U});
        render::RenderImageRgba8 color{
            4,4,std::vector<std::uint8_t>(64U,255U)};
        for(std::size_t i=0U;i<color.pixels.size();i+=4U)
        {color.pixels[i]=245U;color.pixels[i+1]=80U;color.pixels[i+2]=25U;}
        if(!renderer->set_object_texture(
                 "44444444-4444-4444-4444-444444444444",color)||
           !self_world.observe_self_object_bakes(own,region,51U,te))
            error=30;
        if(!error)
        {
          self.baked_texture_ids=self_world.self_render_bakes();
          renderer->set_avatar_proxies({self});
          const auto before=draw();
          if(difference(plain,before)<20U)error=31;
          if(!self_world.withdraw_self_object_on_kill(own,region,51U)||
             self_world.self_render_bakes()!=
                 (std::array<std::string,3U>{}) ||
             self_world.self_object_kills()!=1U)error=32;
          self.baked_texture_ids=self_world.self_render_bakes();
          renderer->set_avatar_proxies({self});
          const auto after=draw();
          const auto remaining=difference(plain,after);
          std::cout<<"SELF_KILLOBJECT_STALE_GPU_SKIN_PIXELS="
                   <<remaining<<"\n";
          if(remaining!=0U||
             renderer->diagnostics().self_avatar_baked_texture_draw_calls!=0U)
              error=33;
        }
        renderer->remove_object_texture(
            "44444444-4444-4444-4444-444444444444");
      }
      self.baked_texture_ids={"fixture-bake",
                              "fixture-bake","fixture-bake"};
      renderer->set_avatar_proxies({self});
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
