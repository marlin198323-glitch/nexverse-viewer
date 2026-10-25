"""v0.4.0.0: real per-part Low158 bake GL proof and login progress trace."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()
class SelfBakeGLAndTraceContract(unittest.TestCase):
 def test_three_gl_draws_cannot_be_credited_to_three_body_parts(self):
  s=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("r.self_low158_current_body_gl_drawn[0]",s)
  self.assertIn("r.self_low158_current_body_gl_drawn[1]",s)
  self.assertIn("r.self_low158_current_body_gl_drawn[2]",s)
  prefix=s[:s.index("if(!r.avatar_depth_known[0]")]
  self.assertNotIn("if(r.avatar_body_bakes_gl<3U)",prefix)
 def test_provenance_must_match_current_server_texture_and_gl_draw(self):
  s=read("src/app/application.cpp")
  self.assertIn("avatar_diagnostic.self_low158_current_body_gl_drawn[slot]=",s)
  self.assertIn("avatar_proof.appearance_received&&",s)
  self.assertIn("avatar_proof.current_body_bake_sources_drawn[slot]&&",s)
  self.assertIn("avatar_proof.body_bake_draws[slot]>0U",s)
 def test_diagnostic_exports_three_independent_slots(self):
  head=read("include/nexverse/ui/avatar_diagnostic_report.hpp")
  cpp=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("self_low158_current_body_gl_drawn{};",head)
  for part in ("head","upper","lower"):
   self.assertIn(f"self_low158_{part}_current_gl_drawn=",cpp)
  tests=read("tests/avatar_diagnostic_report_tests.cpp")
  self.assertIn("stage.avatar_body_bakes_gl=8U;",tests)
  self.assertIn("stage.self_low158_current_body_gl_drawn={true,false,false};",tests)
 def test_trace_requires_authenticated_current_region_and_no_secrets(self):
  s=read("include/nexverse/ui/avatar_bake_progress_trace.hpp")
  for field in ("!r.authenticated","!r.simulator_circuit_active",
                "!r.region_connected","!r.agent_identity_present",
                "region_epoch_!=region_epoch","last_.reset()",
                "last_emit_<std::chrono::seconds{30}",
                "now-last_emit_<std::chrono::milliseconds{250}"):
   self.assertIn(field,s)
  for field in ("low158_refs=","low158_gettexture=",
                "selected_source=","selected_gettexture=",
                "current_low158_gl=","gpu_body=","depth_passed=",
                "final_skin_rgb_unverified"):
   self.assertIn(field,s)
 def test_live_log_only_no_private_report_files(self):
  app=read("src/app/application.cpp")
  self.assertIn("avatar_bake_progress_trace.observe(",app)
  self.assertIn("core::info(std::move(*stage));",app)
  self.assertIn("material_scope.epoch(),avatar_diagnostic,trace_now",app)
  self.assertIn("avatar_report_save_requested",app)
  self.assertNotIn("save_avatar_diagnostic(",read("include/nexverse/ui/avatar_bake_progress_trace.hpp"))
 def test_native_replay_and_full_mint_ctest_registered(self):
  cm=read("CMakeLists.txt")
  native=read("tests/avatar_bake_progress_trace_tests.cpp")
  self.assertIn("nexverse-avatar-bake-progress-trace-tests",cm)
  self.assertIn("logger.observe(7U,r,now+30s)",native)
  self.assertIn("logger.observe(8U,r,now+201s)",native)
  self.assertIn("Eight Head draws",read("tests/avatar_diagnostic_report_tests.cpp"))
if __name__=="__main__":unittest.main()
