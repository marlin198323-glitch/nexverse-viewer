"""Regressionscontracts for Firestorm joint-following wearable geometry."""
import json
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def src(file):return (ROOT/file).read_text(encoding="utf-8")
class AnimatedAttachmentContract(unittest.TestCase):
 def test_firestorm_official_joint_mapping(self):
  rig=src("src/assets/firestorm_avatar_rig.cpp")
  self.assertIn("rig.attachment_joint_plus_one[id]",rig)
  self.assertIn("rig.attachment_local_offsets[id]",rig)
  self.assertIn("const bool hud=",rig)
  self.assertIn("const bool root_joint=",rig)
 def test_render_frames_and_fallback(self):
  gl=src("src/render/opengl_renderer.cpp")
  self.assertIn("attachment_joint_poses_.clear()",gl)
  self.assertIn("avatar_joint_deformations(",gl)
  self.assertIn("frames->second[joint-1U]",gl)
  self.assertIn("avatar_bind_rig_.attachment_offsets[slot]",gl)
  self.assertIn("classify_attachment_scene(",gl)
 def test_propagated_into_actual_viewer(self):
  app=src("src/app/application.cpp")
  self.assertIn("bound.attachment_joint_plus_one[i]",app)
  self.assertIn("bound.attachment_local_offsets[i]",app)
  self.assertIn("shell_view.avatar_bone_pose_avatars=",app)
 def test_visible_f3_and_private_safe_export(self):
  self.assertIn("Bone-Pose:",src("src/ui/viewer_shell_render.cpp"))
  self.assertIn("scene_bone_pose_avatars=",src("src/ui/avatar_diagnostic_report.cpp"))
  private=src("include/nexverse/ui/avatar_diagnostic_report.hpp")
  self.assertIn("std::uint32_t scene_bone_pose_avatars",private)
  self.assertNotIn("std::string avatar_name",private)
 def test_gl_fixture_uses_real_pixels(self):
  gl=src("tests/scene_animated_attachment_pixels_tests.cpp")
  for mark in ("glReadPixels(","normalized_object","HUD_STALE_RGB_PIXELS=",
               "JOINT_FALLBACK_DRIFT=","OGL_JOINT_ATTACHMENT_GPU_PASS"):
   self.assertIn(mark,gl)
  self.assertIn("nexverse-scene-animated-attachment-pixels-tests",src("CMakeLists.txt"))
 def test_next_batch_and_remaining_live_e2e(self):
  self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.904")
  progress=json.loads(src("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(progress["next_batch"],"Alpha.905–914")
  self.assertEqual(progress["default_blocks_per_run"],10)
  self.assertIn("still open",progress["assessment"])
  for p in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md","info.md"):
   self.assertIn("Alpha.905–914",src(p)[:1000])
if __name__=="__main__":unittest.main()
