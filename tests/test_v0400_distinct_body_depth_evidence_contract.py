"""v0.4.0.0: three ORIGINAL Firestorm body parts must finish GPU depth queries."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def src(name):return (ROOT/name).read_text()
class Low158ThreePartDepthTests(unittest.TestCase):
 def test_aggregate_one_part_gl_result_is_not_three_part_proof(self):
  policy=src("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("!r.avatar_depth_known[0]||!r.avatar_depth_known[1]",policy)
  self.assertIn("!r.avatar_depth_known[2]",policy)
  self.assertIn('"depth_visibility_unverified"',policy)
  self.assertNotIn("if(!r.gpu_fragment_result_known)",policy.split("std::string avatar_skin_blocker_hint")[0])
 def test_no_fragments_only_when_all_three_queries_have_finished(self):
  policy=src("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("!(r.avatar_depth_passed[0]||",policy)
  self.assertIn("r.avatar_depth_passed[1]||",policy)
  self.assertIn("r.avatar_depth_passed[2]))",policy)
  self.assertIn('"avatar_depth_fragments_zero"',policy)
 def test_renderer_passes_per_part_gpu_results_to_diagnostics(self):
  app=src("src/app/application.cpp")
  renderer=src("src/render/opengl_renderer.cpp")
  self.assertIn("avatar_diagnostic.avatar_depth_known=",app)
  self.assertIn("avatar_proof.part_visibility.known;",app)
  self.assertIn("avatar_diagnostic.avatar_depth_passed=",app)
  self.assertIn("avatar_proof.part_visibility.passed;",app)
  self.assertIn("diagnostics_.self_avatar_part_samples_known=",renderer)
  self.assertIn("diagnostics_.self_avatar_part_samples_passed=",renderer)
 def test_live_tracer_logs_three_known_and_passed_flags(self):
  tracer=src("include/nexverse/ui/avatar_bake_progress_trace.hpp")
  self.assertIn("next.body_depth_known=r.avatar_depth_known",tracer)
  self.assertIn("next.body_depth_passed=r.avatar_depth_passed",tracer)
  self.assertIn('"; depth_part_known="+',tracer)
  self.assertIn('"; depth_part_passed="+',tracer)
 def test_native_negative_scenario_head_known_only(self):
  native=src("tests/avatar_diagnostic_report_tests.cpp")
  self.assertIn("stage.avatar_depth_known={true,false,false};",native)
  self.assertIn("stage.avatar_depth_known={true,true,false};",native)
  self.assertIn("stage.avatar_depth_known={true,true,true};",native)
  self.assertIn("stage.avatar_depth_passed={false,false,false};",native)
  trace=src("tests/avatar_bake_progress_trace_tests.cpp")
  self.assertIn("depth_part_known=1,0,0",trace)
 def test_no_premature_full_skin_pixel_readback_claim(self):
  source=src("src/ui/avatar_diagnostic_report.cpp")
  tracer=src("include/nexverse/ui/avatar_bake_progress_trace.hpp")
  self.assertIn('"depth_visible_final_skin_pixels_unverified"',source)
  self.assertIn('"; final_skin_rgb_unverified"',tracer)
if __name__=="__main__":unittest.main()
