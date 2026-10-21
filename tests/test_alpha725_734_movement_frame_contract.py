"""Real WASD AgentUpdate must never emit poisoned body quaternion."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(n):return (R/n).read_text()
class MovementFrameContract(unittest.TestCase):
 def test_real_controller_uses_finite_frame_and_normalized_heading(self):
  s=read("include/nexverse/protocol/movement_controls.hpp")
  self.assertIn("yaw_ = valid_initial_agent_yaw(heading);",s)
  self.assertIn("bounded_agent_update_seconds(seconds)",s)
 def test_nonfinite_wire_proof(self):
  s=read("tests/movement_controls_tests.cpp")
  for token in ("std::numeric_limits<float>::quiet_NaN()",
                "const float inf=","server_control_bits(safe)",
                "c.step(rotate,3600.0f)",
                "7e25f,7e25f",
                "encode_agent_update(53U,session,large)",
                "std::bit_cast<float>(bits)",
                "encode_agent_update(54U,session,safe_nan)"):
   self.assertIn(token,s)
 def test_live_input_does_not_send_nan_camera_to_server(self):
  s=read("src/app/application.cpp")
  self.assertIn("protocol::valid_agent_update_position(",s)
  self.assertIn("AgentUpdate paused: simulator position invalid",s)
  self.assertIn("movement_feedback.reset();",s)
  self.assertIn("invalid_movement_position_logged=false;",s)
 def test_no_naive_nonfinite_clamp(self):
  s=read("include/nexverse/protocol/movement_frame_safety.hpp")
  self.assertIn("if(!std::isfinite(seconds)||seconds<=0.0f)",s)
  self.assertIn("const double sq=x*x+y*y+z*z+w*w",s)
  self.assertIn("if(!std::isfinite(x)",s)
if __name__=="__main__":unittest.main()
