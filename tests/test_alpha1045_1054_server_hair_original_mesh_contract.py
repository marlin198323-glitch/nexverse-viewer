"""Alpha1054: original Firestorm hair .llm and Hair TE baked slot 5 are routed to real GL."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()
class OriginalHair(unittest.TestCase):
 def test_exact_authenticated_server_hair_te_never_head_alias(self):
  app=read("src/app/application.cpp")
  a=app.index("System-hair is a separate baked TE face")
  b=app.index("avatar_proxy_update.back().moving=",a)
  self.assertIn("world_model.agent().session_id==active_session->session_id",app[a:b])
  self.assertIn("appearance->avatar_id==active_session->agent_id",app[a:b])
  self.assertIn("confirmed.ids[5U]",app[a:b])
  self.assertIn("remote_bakes.ids[5U]",app)
  self.assertIn("avatar_hair.llm",app)
 def test_real_current_gpu_admission_shared_by_hair_body_requests(self):
  app=read("src/app/application.cpp")
  self.assertEqual(app.count("assets::admit_avatar_bake_gpu_candidates("),1)
  self.assertIn("avatar.eyes_bake_id,avatar.hair_bake_id",app)
  self.assertIn("accessory,avatar.x,avatar.y,avatar.z",app)
  self.assertIn("avatar_body_priority=",app)
  self.assertIn("Firestorm Hair-TE GPU-GL:",app)
  admission=read("include/nexverse/assets/avatar_bake_gpu_admission.hpp")
  self.assertIn("proxy.hair_bake_texture_id",admission)
  ranked=read("include/nexverse/assets/avatar_bake_stream_priority.hpp")
  self.assertLess(ranked.index("for(const auto& id:avatar.body)add(id);"), ranked.index("add(avatar.hair_bake_id);"))
  self.assertIn("baked_placeholders",ranked)
 def test_firestorm_gl_hair_draw_never_counts_as_fourth_body_bake(self):
  gl=read("src/render/opengl_renderer.cpp")
  hdr=read("include/nexverse/render/renderer.hpp")
  self.assertIn("std::array<GpuMesh,7U> avatar_body_meshes_",gl)
  self.assertIn("avatar_body_meshes_.begin()+3",gl)
  self.assertIn("avatar_body_meshes_[3U]",gl)
  self.assertIn("object_textures_.find(\n                            avatar.hair_bake_texture_id)",gl)
  self.assertIn("++diagnostics_.self_avatar_hair_bake_gl_draws;",gl)
  self.assertIn("std::string hair_bake_texture_id;",hdr)
  self.assertIn("self_avatar_hair_bake_gl_draws",hdr)
 def test_actual_framebuffer_asset_withdrawal_and_gpu_proof(self):
  t=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for expected in ("avatar_hair.llm", "set_avatar_body_mesh(3U,",
                   "ORIGINAL_FIRESTORM_HAIR_BAKE_PIXELS=",
                   "actual.self_avatar_hair_bake_gl_draws!=1U",
                   "actual.self_avatar_anatomical_mesh_draw_calls!=3U",
                   "WITHDRAWN_HAIR_TE_GHOST_PIXELS=",
                   "renderer->remove_object_texture(\"hair-bake\")",
                   "renderer->diagnostics().self_avatar_hair_bake_gl_draws!=0U"):
    self.assertIn(expected,t)
if __name__=="__main__":unittest.main()
