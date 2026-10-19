"""WASD feedback must reject teleport/jumps, Z-only and stale server samples."""
import unittest,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text(encoding="utf-8")
class MotionEvidenceContract(unittest.TestCase):
 def test_real_udp_send_required(self):
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  app=read("src/app/application.cpp")
  self.assertIn("if(queued_signals_seen_&&!queued_direction_)return;",mon)
  self.assertIn("movement_feedback.queued_controls(queued_controls)",app)
  self.assertIn("movement_feedback.outbound_controls(",app)
  self.assertNotIn("movement_feedback.outbound_controls(queued_controls,now)",app)
 def test_only_horizontal_sample_can_confirm_wasd(self):
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("const float distance_squared=dx*dx+dy*dy;",mon)
  self.assertIn("distance_squared>=0.0625f",mon)
  self.assertIn("std::isfinite(position.z)",mon)
 def test_discontinuity_not_walk(self):
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  for s in ("teleport_like_sq=32.0f*32.0f",
            "return MovementEvidence::server_position_discontinuity",
            "awaiting_after_discontinuity_=true","baseline_=position",
            "active_since_=now","samples_=0U",
            "WASD: Positionssprung/Teleport"):
   self.assertIn(s,mon)
 def test_monotonic_server_sample_timing(self):
  mon=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("now<*last_sample_at_",mon)
  self.assertIn("active_&&now<active_since_",mon)
  self.assertIn("stale_samples_ignored_",mon)
  self.assertIn("if(samples_!=UINT32_MAX)++samples_",mon)
 def test_reset_on_region_lease(self):
  app=read("src/app/application.cpp")
  region=app.index("if(animation_asset_epoch!=current_firestorm_epoch)")
  reset=app.index("movement_feedback.reset();",region)
  appearance=app.index("avatar_appearance_received.store(0U",region)
  self.assertLess(region,reset)
  self.assertLess(reset,appearance)
  self.assertEqual(app.count("MovementEvidence::server_position_discontinuity"),3)
  for src in ("MovementPositionSource::full_object",
              "MovementPositionSource::compressed_object",
              "const auto evidence=movement_feedback.server_position("):
   self.assertIn(src,app)
 def test_ui_and_privacy_report(self):
  header=read("include/nexverse/ui/avatar_diagnostic_report.hpp")
  report=read("src/ui/avatar_diagnostic_report.cpp")
  app=read("src/app/application.cpp")
  for key in ("movement_discontinuities","movement_stale_samples_ignored",
              "movement_waiting_after_jump"):
   self.assertIn(key,header)
   self.assertIn(key+"=",report)
   self.assertIn("avatar_diagnostic."+key,app)
  self.assertNotIn("movement_agent_id=",report)
  self.assertNotIn("movement_server_coordinates=",report)
 def test_real_cxx_proof(self):
  native=read("tests/movement_feedback_status_tests.cpp")
  for claim in ("server_position_discontinuity","Positionssprung/Teleport",
                "stale_samples_ignored!=1U","{30,40,6.5f}",
                "server_position_stalled","awaiting_after_discontinuity"):
   self.assertIn(claim,native)
 def test_release_documentation(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.494")
  state=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(state["viewer_version"],"0.2.0-alpha.494")
  self.assertEqual(state["next_batch"],"Alpha.495–504")
  self.assertEqual(state["default_blocks_per_run"],10)
  self.assertIn("still open",state["assessment"])
  for filename in ("CHATUEBERGABE.md","AGENTS.md","projectplan.md",
                   "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.495–504",read(filename)[:1100])
if __name__=="__main__":unittest.main()
