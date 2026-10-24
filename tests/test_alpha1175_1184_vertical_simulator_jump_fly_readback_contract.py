"""Alpha1184 correct native vertical jump/fly confirmations require real socket + self pose."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def get(rel):return (R/rel).read_text()
class VerticalSelfMovement(unittest.TestCase):
 def test_new_native_monitors_in_real_CTest(self):
  c=get("CMakeLists.txt")
  for t in ("nexverse-vertical-movement-feedback-tests",
    "tests/vertical_movement_feedback_tests.cpp",
    "add_test(NAME nexverse-vertical-movement-feedback-tests"):
   self.assertIn(t,c)
 def test_real_send_not_queued_is_only_vertical_trigger(self):
  s=get("src/app/application.cpp")
  self.assertIn("vertical_feedback.queued_controls(queued_controls)",s)
  self.assertIn("vertical_feedback.outbound_controls(",s)
  self.assertIn("flags,sent_at);",s)
  h=get("include/nexverse/protocol/vertical_movement_feedback.hpp")
  for t in ("queued_seen_&&!queued_","void outbound_controls(",
   "sent_=true;","control_up_pos","control_up_neg","control_fly"):
   self.assertIn(t,h)
 def test_auth_self_position_from_all_three_wire_families(self):
  s=get("src/app/application.cpp")
  self.assertEqual(s.count("vertical_feedback.server_position("),3)
  for t in ("protocol::decode_object_update(packet.payload)",
    "protocol::decode_object_update_compressed(packet.payload)",
    "protocol::decode_improved_terse_object_update(packet.payload)",
    "world_model.accept_simulator_self_pose(",
    "world_model.reject_retired_self_object_pose(",
    "protocol::region_packet_matches("):
   self.assertIn(t,s)
 def test_region_kill_logout_reconnect_invalid_pose_resets_all_monitor_states(self):
  s=get("src/app/application.cpp")
  self.assertEqual(s.count("vertical_feedback.reset();"),
                   s.count("movement_feedback.reset();"))
  self.assertGreaterEqual(s.count("vertical_feedback.reset();"),8)
 def test_signed_z_projection_requires_positive_025m_server_delta(self):
  h=get("include/nexverse/protocol/vertical_movement_feedback.hpp")
  for t in ("static_cast<double>(position.z)-baseline_->z",
     "const double projected=dz*sign_;", "projected>=0.25",
     "projected<=-0.25", "confirmed_=true"):
   self.assertIn(t,h)
 def test_no_teleport_invalid_stale_or_pre_send_evidence(self):
  h=get("include/nexverse/protocol/vertical_movement_feedback.hpp")
  for t in ("!std::isfinite(position.z)", "now<*last_sample_at_",
    "now<sent_at_", "dist_sq>=1024.0",
    "baseline_=position;", "samples_=0U;"):
   self.assertIn(t,h)
 def test_snapshots_separate_vertical_from_horizontal_proof(self):
  h=get("include/nexverse/protocol/vertical_movement_feedback.hpp")
  self.assertIn("struct VerticalMovementSnapshot",h)
  self.assertIn("contradictory_samples",h)
  s=get("src/app/application.cpp")
  self.assertIn("shell_view.avatar_movement_status+=",s)
  self.assertIn("vertical_feedback.status()",s)
 def test_native_regression_covers_queue_send_wait_confirm_and_reverse(self):
  t=get("tests/vertical_movement_feedback_tests.cpp")
  for x in ("monitor.queued_controls(control_up_pos)",
    "monitor.outbound_controls(control_up_pos",
    "monitor.queued_controls(control_up_neg)",
    "monitor.server_position({12,21,29.6f}",
    "monitor.snapshot().contradictory_samples",
    "monitor.snapshot().confirmed",
    "OGL_REAL_SIMULATOR_VERTICAL_POSITION_READBACK_SIGN_AND_RESET_PASS"):
   self.assertIn(x,t)
 def test_emits_no_fake_avatar_position_or_wire_message(self):
  s=get("include/nexverse/protocol/vertical_movement_feedback.hpp")
  for absent in ("send_datagram(","AgentSetAppearance(", "world_model.",
      "set_avatar_position(", "apply_agent("):
   self.assertNotIn(absent,s)
 def test_real_authenticated_skin_outfit_wasd_e2e_still_open(self):
  p=get("docs/PROJECT_PROGRESS.json")
  self.assertRegex(p,r"Alpha[0-9]{3,4}")
  self.assertIn("still open",p)
  self.assertIn("BLOCKED",p)
if __name__=="__main__":unittest.main()
