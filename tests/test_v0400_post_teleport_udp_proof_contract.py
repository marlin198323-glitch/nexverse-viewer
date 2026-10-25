"""v0.4.0.0: post-teleport movement proof requires a fresh UDP send."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name): return (R/name).read_text()
class PostTeleportAuthority(unittest.TestCase):
 def test_horizontal_discontinuity_disarms_old_udp(self):
  source=read("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("active_=false;",source)
  self.assertIn("awaiting_after_discontinuity_=true;",source)
  self.assertIn("expected_direction_.reset();",source)
  self.assertIn("last_direction_bits_=0U;",source)
  self.assertIn("neuer UDP-Versand ausstehend",source)
 def test_vertical_discontinuity_disarms_old_udp(self):
  source=read("include/nexverse/protocol/vertical_movement_feedback.hpp")
  self.assertIn("sent_=false;sign_=0;last_fly_=false;",source)
  self.assertIn("awaiting_fresh_udp_after_jump_=true;",source)
  self.assertIn("awaiting_fresh_udp_after_jump_=false;",source)
  self.assertIn("neuer UDP-Versand ausstehend",source)
 def test_queued_inputs_alone_do_not_restore_motion_proof(self):
  horizontal=read("tests/movement_feedback_status_tests.cpp")
  vertical=read("tests/vertical_movement_feedback_tests.cpp")
  self.assertIn("monitor.queued_controls(control_at_pos); // UI queue != real send",horizontal)
  self.assertIn("monitor.snapshot().socket_direction_sent)return 41;",horizontal)
  self.assertIn("monitor.outbound_controls(control_at_pos,now+104s);",horizontal)
  self.assertIn("monitor.snapshot().awaiting_fresh_udp_after_jump)",vertical)
  self.assertIn("monitor.outbound_controls(control_up_pos,t+25s);",vertical)
 def test_wire_send_is_only_entry_point_of_proof(self):
  app=read("src/app/application.cpp")
  self.assertIn("movement_feedback.outbound_controls(",app)
  self.assertIn("vertical_feedback.outbound_controls(",app)
  self.assertIn("movement_feedback.reset();",app)
  self.assertIn("vertical_feedback.reset();",app)
  self.assertIn("protocol::MovementEvidence::server_position_discontinuity",app)
if __name__=="__main__": unittest.main()
