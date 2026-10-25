"""v0.4.0.0: self avatar pose, Shape mesh and viewport must retire stale
asynchronous GPU visibility results without per-frame invalidation."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(file):return (R/file).read_text()
class SelfAvatarVisibilityEpochContract(unittest.TestCase):
 def test_pose_changes_are_measured_from_last_invalidation(self):
  h=read("include/nexverse/render/self_avatar_visibility_pose_gate.hpp")
  self.assertIn("minimum_displacement_squared=0.05*0.05",h)
  self.assertIn("minimum_rotation_dot=0.99985",h)
  self.assertIn("std::abs(q_dot)",h)
  self.assertIn("(*previous_)[i]",h)
  self.assertIn("previous_=pose;",h)
 def test_live_renderer_checks_self_pose_after_normalization(self):
  s=read("src/render/opengl_renderer.cpp")
  a=s.index("void set_avatar_proxies(")
  b=s.index("void begin_frame()",a)
  part=s[a:b]
  self.assertIn("sanitize_avatar_proxy_pose(candidate)",part)
  self.assertIn("current_self_pose=SelfAvatarVisibilityPoseGate::Pose{",part)
  self.assertIn("self_avatar_visibility_pose_.observe(current_self_pose)",part)
  self.assertIn("if(source_changed||pose_changed)",part)
  self.assertNotIn("if(source_changed||",part[:part.index("self_avatar_visibility_pose_.observe")])
 def test_shape_morphed_real_head_upper_lower_mesh_retire_old_gl(self):
  s=read("src/render/opengl_renderer.cpp")
  a=s.index("bool set_avatar_body_mesh(")
  b=s.index("void reset_self_avatar_visibility()",a)
  block=s[a:b]
  self.assertIn("if(!upload_mesh(candidate,source))",block)
  self.assertIn("avatar_body_meshes_[slot]=std::move(candidate);",block)
  self.assertIn("if(slot<=2U||slot==6U||slot==8U||slot==9U)",block)
  self.assertIn("reset_self_avatar_visibility();",block)
 def test_resize_changes_projection_and_invalidates_but_same_size_doesnt(self):
  s=read("src/render/opengl_renderer.cpp")
  a=s.index("void resize(int width, int height) override")
  b=s.index("void set_camera(",a)
  block=s[a:b]
  self.assertIn("if(width_!=next_width||height_!=next_height)",block)
  self.assertIn("reset_self_avatar_visibility();",block)
 def test_real_xvfb_gl_regression_has_moving_and_rotating_self(self):
  test=read("tests/renderer_region_transition_tests.cpp")
  self.assertIn("changed_self.position={3.12f,0.0f,0.0f}",test)
  self.assertIn("changed_self.rotation={",test)
  self.assertIn("renderer->set_avatar_body_mesh(1U,body)",test)
  self.assertIn("renderer->resize(420,230)",test)
  self.assertIn("error=65;",test)
 def test_pure_gate_handles_network_jitter_and_q_sign_flip(self):
  test=read("tests/self_avatar_visibility_pose_gate_tests.cpp")
  self.assertIn("3.01f",test)
  self.assertIn("3.03f",test)
  self.assertIn("3.07f",test)
  self.assertIn("-0.9961947f",test)
  self.assertIn("std::nullopt",test)
  cmake=read("CMakeLists.txt")
  self.assertIn("nexverse-self-avatar-visibility-pose-gate-tests",cmake)
 def test_no_claim_of_full_rgb_skin_or_cof_e2e(self):
  s=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn('"depth_visible_final_skin_pixels_unverified"',s)
  self.assertIn("self_low158_current_body_gl_drawn",s)
if __name__=="__main__":unittest.main()
