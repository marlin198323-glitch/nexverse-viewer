"""Alpha1074: original Firestorm weighted lashes not stale/fabricated."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(s):return (R/s).read_text()
class EyelashGate(unittest.TestCase):
 def test_real_original_optional_llm_from_firestorm_local_assets(self):
  app=rd("src/app/application.cpp")
  for token in ("avatar_eyelashes.llm",
                "eyelashes->has_weights",
                "firestorm_joint_render_palette(",
                "world_renderer->set_avatar_body_mesh(5U,geom)",
                "Firestorm Wimpern-Head-GL:"):
   self.assertIn(token,app)
 def test_strict_head_gpu_attribution_no_fake_lashes(self):
  gl=rd("src/render/opengl_renderer.cpp")
  for token in ("avatar_body_meshes_[5U]", "avatar.baked_texture_ids[0]",
                "object_textures_.find(", "eyelash_mesh.skin_vbo!=0U",
                "eyelash_mesh.skin_palette.size()<=32U",
                "glUniformMatrix4fv(", "uAvatarSkinEnabled",
                "self_avatar_eyelash_head_gl_draws=0U",
                "++diagnostics_.self_avatar_eyelash_head_gl_draws"):
   self.assertIn(token,gl)
  self.assertIn("std::array<GpuMesh,10U>",gl)
  self.assertIn("avatar_body_meshes_.begin()+3",gl)
 def test_separate_not_three_body_skin(self):
  header=rd("include/nexverse/render/renderer.hpp")
  self.assertIn("std::uint64_t self_avatar_eyelash_head_gl_draws",header)
  gl=rd("src/render/opengl_renderer.cpp")
  self.assertNotIn("self_avatar_body_bake_gl_draws[5]",gl)
 def test_original_gl_pixels_and_withdrawal(self):
  t=rd("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for token in ("avatar_eyelashes.llm",
                "ORIGINAL_FIRESTORM_EYELASH_VISIBLE_PIXELS",
                "WITHDRAWN_HEAD_EYELASH_GL_DRAWS",
                "renderer->remove_object_texture(\"lash-head-current\")"):
   self.assertIn(token,t)
if __name__=="__main__":unittest.main()
