"""Alpha1064 original left/right Firestorm eyes from genuine baked Eyes face 11."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text()
class DualEyes(unittest.TestCase):
 def test_authenticated_low158_and_remote_original_eyes(self):
  app=read("src/app/application.cpp")
  self.assertIn("confirmed.ids[3U]",app)
  self.assertIn("remote_bakes.ids[3U]",app)
  self.assertIn("avatar_eye.llm",app)
  self.assertIn("eyes_bake_texture_id",app)
  self.assertIn("avatar.eyes_bake_id",app)
  self.assertEqual(app.count("assets::admit_avatar_bake_gpu_candidates("),1)
 def test_eyes_lower_priority_than_real_skin(self):
  priority=read("include/nexverse/assets/avatar_bake_stream_priority.hpp")
  self.assertLess(priority.index("for(const auto& id:avatar.body)add(id);"),
                  priority.index("add(avatar.eyes_bake_id);"))
  self.assertLess(priority.index("add(avatar.eyes_bake_id);"),
                  priority.index("add(avatar.hair_bake_id);"))
  self.assertIn("proxy.eyes_bake_texture_id",
      read("include/nexverse/assets/avatar_bake_gpu_admission.hpp"))
 def test_eye_joint_attachment_is_animated_not_untranslated_head(self):
  gl=read("src/render/opengl_renderer.cpp")
  for t in ("mEyeLeft","mEyeRight","avatar_eye_joint_indices_",
            "avatar_body_meshes_[4U]", "posed[static_cast<std::size_t>(",
            "avatar.eyes_bake_texture_id", "self_avatar_eyes_bake_gl_draws"):
   self.assertIn(t,gl)
  self.assertIn("std::array<GpuMesh,7U>",gl)
  self.assertIn("avatar_body_meshes_.begin()+3",gl)
  self.assertIn("std::string eyes_bake_texture_id",
      read("include/nexverse/render/renderer.hpp"))
 def test_real_gl_framebuffer_and_withdrawal(self):
  t=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for val in ("avatar_eye.llm","set_avatar_body_mesh(4U",
              "ORIGINAL_FIRESTORM_DUAL_EYE_PIXELS",
              "self_avatar_eyes_bake_gl_draws!=2U",
              "DUAL_EYE_SEPARATE_JOINT_PIXELS",
              "WITHDRAWN_EYES_TE_GHOST_PIXELS",
              "remove_object_texture(\"eyes-current\")"):
   self.assertIn(val,t)
if __name__=="__main__":unittest.main()
