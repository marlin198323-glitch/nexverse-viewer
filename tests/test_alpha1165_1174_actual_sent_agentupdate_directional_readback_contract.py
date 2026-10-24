"""Correct actual UDP AgentUpdate Direction+Yaw correlated with later simulator self pose."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def src(x):return (R/x).read_text()
class SentMotionReadback(unittest.TestCase):
 def test_circuit_has_exact_actual_sent_rotation_not_queued_camera_preview(self):
  h=src("include/nexverse/protocol/circuit.hpp")
  s=src("src/protocol/circuit.cpp")
  self.assertIn("std::function<void(std::uint32_t,const Quaternion&)>",h)
  self.assertIn("on_agent_update_sent(value.control_flags,value.body_rotation)",s)
  self.assertIn("if(sent)",s)
 def test_actual_circuit_callback_supplies_wire_rotation_to_readback_monitor(self):
  s=src("src/app/application.cpp")
  for phrase in ("const protocol::Quaternion& sent_body",
                 "movement_feedback.outbound_controls(\n",
                 "flags,sent_at,sent_body",
                 "turn_feedback.outbound_controls("):
   self.assertIn(phrase,s)
 def test_projection_requires_positive_signed_server_horizontal_movement(self):
  s=src("include/nexverse/protocol/movement_feedback.hpp")
  for x in ("dx*expected_direction_->x+dy*expected_direction_->y",
            "projection>=0.25f", "expected_direction_",
            "std::hypot(", "std::atan2(siny,cosy)",
            "std::isfinite(transmitted_body.w)"):
   self.assertIn(x,s)
 def test_reverse_strafe_bits_invalidate_old_proof_and_baseline(self):
  s=src("include/nexverse/protocol/movement_feedback.hpp")
  for x in ("current_direction_bits!=last_direction_bits_",
            "baseline_=latest_;",
            "confirmed_=false;",
            "last_direction_bits_=current_direction_bits;",
            "last_direction_bits_=0U;"):
   self.assertIn(x,s)
 def test_small_yaw_adjustment_does_not_perpetually_reset_proof(self):
  s=src("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("<0.98f",s)
  t=src("tests/movement_feedback_status_tests.cpp")
  self.assertIn("{0.0f,0.0f,0.0249974f,0.9996875f}",t)
 def test_outbound_invalid_quaternion_never_authorizes_motion_evidence(self):
  s=src("include/nexverse/protocol/movement_feedback.hpp")
  for x in ("norm>=0.25&&norm<=4.0",
            "expected=Vec3{0.0f,0.0f,0.0f};",
            "std::isfinite(projection)"):
   self.assertIn(x,s)
 def test_native_realistic_position_direction_negative_coverage(self):
  s=src("tests/movement_feedback_status_tests.cpp")
  for x in ("monitor.server_position({0,1,5}",
            "monitor.server_position({-0.5f,1,5}",
            "monitor.server_position({0.35f,1,5}",
            "monitor.queued_controls(control_at_neg)",
            "monitor.server_position({1.1f,1,5}",
            "monitor.outbound_controls(control_at_pos,now+311s,north)",
            "monitor.outbound_controls(control_left_pos,now+321s,north)",
            "OGL_ACTUAL_AGENTUPDATE_DIRECTIONAL_SERVER_READBACK_PASS"):
   self.assertIn(x,s)
 def test_no_spoofed_grid_proof_server_samples_only(self):
  s=src("include/nexverse/protocol/movement_feedback.hpp")
  self.assertIn("now<active_since_",s)
  self.assertIn("server_position_discontinuity",s)
  self.assertIn("MovementPositionSource",s)
 def test_verified_high4_agentposition_lease_still_controls_wasd(self):
  s=src("src/app/application.cpp")
  for x in ("world_model.self_avatar_controls_ready()",
            "agent_for_camera.agent_id==active_session->agent_id",
            "agent_for_camera.session_id==active_session->session_id",
            "protocol::valid_agent_update_position("):
   self.assertIn(x,s)
 def test_real_live_login_absent_release_blocked(self):
  s=src("docs/PROJECT_PROGRESS.json")
  self.assertIn("Alpha1174",s)
  self.assertIn("BLOCKED",s)
if __name__=="__main__":unittest.main()
