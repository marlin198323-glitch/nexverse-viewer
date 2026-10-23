"""Alpha1094: real bounded Firestorm morphs, current COF shape, SELF-only GPU, resets."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class FirestormMorph1094(unittest.TestCase):
 def test_original_binary_morphs_are_actually_decoded_not_skipped(self):
  src=read("src/assets/firestorm_avatar_mesh.cpp")
  for key in ("End Morphs", "max_morph_targets=160U",
              "max_morph_vertices=60000U", "entry.index>=n",
              "total_morphed_vertices+count", "seen[entry.index]",
              "remaps>n", "r.cursor!=data.size()"):
   self.assertIn(key,src)
  hdr=read("include/nexverse/assets/firestorm_avatar_mesh.hpp")
  self.assertIn("struct FirestormMorphTarget",hdr)
  self.assertIn("morph_firestorm_avatar_mesh(",hdr)
 def test_true_original_sparse_morph_applies_only_to_copy(self):
  src=read("src/assets/firestorm_avatar_mesh.cpp")
  for key in ("FirestormAvatarMesh result=base;",
              "std::abs(value)>3.0f",
              "entry.position_delta[k]*value",
              "entry.normal_delta[k]*value",
              "entry.uv_delta[k]*value",
              "it==base.morph_targets.end()"):
   self.assertIn(key,src)
  test=read("tests/firestorm_avatar_mesh_tests.cpp")
  for key in ("head->morph_targets.size()!=95U",
              "upper->morph_targets.size()!=30U",
              "lower->morph_targets.size()!=24U",
              "lash->morph_targets.size()!=34U",
              "altered<100U", "decode_firestorm_avatar_llm(raw)"):
   self.assertIn(key,test)
 def test_inworld_is_exact_authenticated_owner_scope_and_shape_asset(self):
  app=read("src/app/application.cpp")
  start=app.index("An entirely resolved LLWearable snapshot supersedes")
  end=app.index("if(!gallery_asset_active_id.empty()&&",start)
  sub=app[start:end]
  for key in ("gallery_asset_errors==0U", "login_ui.authenticated",
              "gallery_wearables_fetch_owner==",
              "active_session->agent_id",
              "gallery_asset_epoch==current_firestorm_epoch",
              "firestorm_region_lease.matches(",
              "caps_state.current_outfit_folder==gallery_asset_cof",
              "gallery_decoded_wearable_assets.size()==",
              "wearable.type!=0", "multiple_shape",
              "firestorm_cof_shape_morph_from_original_lad(",
              "selection&&selection->active()",
              'morph_firestorm_avatar_mesh(',
               'upload("avatar_head.llm",6U,',
              'shape_head_preview_active=true'):
   self.assertIn(key,sub)
  self.assertIn("clear_self_avatar_head_shape_preview()",app)
 def test_actual_gpu_draw_is_self_only_and_reset_before_render(self):
  gl=read("src/render/opengl_renderer.cpp")
  for key in ("std::array<GpuMesh,10U>",
              "avatar.is_self&&",
              "slot==0U?6U:(slot==1U?8U:9U)",
              "clear_self_avatar_head_shape_preview() override",
              "self_avatar_shape_head_gl_draws"):
   self.assertIn(key,gl)
  app=read("src/app/application.cpp")
  a=app.index("if(shape_head_preview_active)")
  b=app.index("const auto admitted_bakes=",a)
  for key in ("current_firestorm_epoch!=shape_head_preview_epoch",
              "active_session->agent_id!=shape_head_preview_owner",
              "caps_state.current_outfit_folder!=",
              "ver->second!=shape_head_preview_cof_version",
              "world_renderer->clear_self_avatar_head_shape_preview()"):
   self.assertIn(key,app[a:b])
  self.assertIn("Firestorm COF-Shape-Head-GL:",app)
 def test_original_gl_framebuffer_proves_remote_not_morphed_and_reset(self):
  t=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for key in ("ORIGINAL_FIRESTORM_SHAPE_MORPH_PIXELS",
              "REMOTE_UNAFFECTED_BY_SELF_SHAPE_PIXELS",
              'morph_firestorm_avatar_mesh(',
              "self_avatar_shape_head_gl_draws!=1U",
              "remote_stale!=0U",
              "renderer->clear_self_avatar_head_shape_preview()"):
   self.assertIn(key,t)
if __name__=="__main__":unittest.main()
