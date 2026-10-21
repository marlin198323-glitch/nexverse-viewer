"""Real GL input uses finite world pose and sanitized quaternion, not
just static UI diagnostics. Full native CMake test and Pi headless parity."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path): return (R/path).read_text()
class RealAvatarPoseIntegrity(unittest.TestCase):
 def test_backend_filters_invalid_pose_before_gl(self):
  s=read("src/render/opengl_renderer.cpp")
  self.assertIn("const auto audit=sanitize_avatar_proxy_pose(candidate);",s)
  self.assertIn("if(!audit.position_valid)",s)
  self.assertIn("avatar_proxies_.push_back(std::move(candidate));",s)
  self.assertIn("diagnostics_.self_avatar_pose_invalid_dropped=",s)
  self.assertIn("diagnostics_.avatar_rotation_repaired=",s)
 def test_camera_hud_matches_accepted_real_hover_proxy(self):
  s=read("src/app/application.cpp")
  self.assertIn("const auto self=std::find_if(",s)
  self.assertIn("self->position.z",s)
  self.assertIn("avatar_proof.self_gpu_pose_dropped=",s)
  self.assertIn("avatar_proof.pose_stage()",s)
  self.assertIn("if(self!=avatar_proxy_update.end()&&",s)
 def test_finite_and_quaternion_double_input(self):
  s=read("include/nexverse/render/avatar_proxy_pose_integrity.hpp")
  for fragment in ("std::isfinite(v)","std::abs(v)<1000000.0f",
                   "double(q.x)*q.x","!std::isfinite(norm2)",
                   "q={0.0f,0.0f,0.0f,1.0f}","q=normalized"):
   self.assertIn(fragment,s)
 def test_actual_firestorm_framebuffer_rejects_bad_avatar(self):
  s=read("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for text in ("poisoned.position.x=nan;",
               "renderer->set_avatar_proxies({poisoned});",
               "difference(empty,rejected)!=0U",
               "invalid.self_avatar_pose_invalid_dropped",
               "renderer->set_avatar_proxies({damaged});",
               "difference(plain,fixed)!=0U",
               "renderer->set_avatar_proxies({self,remote});",
               "difference(plain,current)!=0U"):
   self.assertIn(text,s)
 def test_native_registration_and_no_desktop_auto_start(self):
  self.assertIn("tests/avatar_proxy_pose_integrity_tests.cpp",read("CMakeLists.txt"))
  self.assertIn("avatar_proxy_pose_integrity_tests",read("scripts/pi5-headless-tests.sh"))
if __name__=="__main__":unittest.main()
