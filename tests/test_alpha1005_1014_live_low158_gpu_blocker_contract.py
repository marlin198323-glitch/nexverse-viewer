"""Alpha.1014: only the earliest missing authenticated skin proof is diagnosed."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def src(name):return (R/name).read_text()
class LiveLow158SkinBlocker(unittest.TestCase):
 def test_current_self_packet_logs_bake_count_without_uuids(self):
  s=src("src/app/application.cpp")
  self.assertIn("AvatarAppearance SELF body-bake evidence: ",s)
  self.assertIn("no server body bake UUIDs; GetTexture cannot",s)
  self.assertIn("core::info(\"AvatarAppearance SELF server readback accepted;",s)
  self.assertIn("UUIDs omitted for privacy",s)
 def test_current_snapshot_passed_to_actionable_f3_text(self):
  s=src("src/app/application.cpp")
  self.assertIn("avatar_diagnostic.self_low158_body_bake_refs",s)
  self.assertIn("avatar_diagnostic.avatar_body_bakes_gpu=",s)
  self.assertIn("avatar_diagnostic.avatar_body_bakes_gl=",s)
  self.assertIn("ui::avatar_skin_blocker_hint(avatar_diagnostic)",s)
  self.assertIn("shell_view.avatar_skin_status=avatar_proof.skin_stage()",s)
 def test_report_classifies_source_download_draw_depth_separately(self):
  s=src("src/ui/avatar_diagnostic_report.cpp")
  h=src("include/nexverse/ui/avatar_diagnostic_report.hpp")
  for code in ("self_low158_missing","server_body_bakes_missing",
               "server_body_bakes_partial","body_bake_assets_not_gpu_resident",
               "original_body_bake_gl_draw_missing","depth_visibility_unverified",
               "depth_visible_final_skin_pixels_unverified"):
   self.assertIn(code,s)
  self.assertIn("self_skin_blocker_stage=",s)
  self.assertIn("avatar_skin_blocker_key(",h)
 def test_native_scenarios_not_just_mock_source_check(self):
  s=src("tests/avatar_diagnostic_report_tests.cpp")
  for code in ('"server_body_bakes_partial"','"server_body_bakes_missing"',
               '"body_bake_assets_not_gpu_resident"',
               '"original_body_bake_gl_draw_missing"',
               '"depth_visible_final_skin_pixels_unverified"'):
   self.assertIn(code,s)
if __name__=="__main__":unittest.main()
