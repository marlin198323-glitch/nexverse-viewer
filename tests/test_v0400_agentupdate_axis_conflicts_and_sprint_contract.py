"""v0.4.0.0: High4 AgentUpdate cannot carry opposing vertical controls
or fast modifiers of axes not actually requested by current keys."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(x):return (R/x).read_text()
class AgentUpdateDirectionalSafetyContract(unittest.TestCase):
 def test_vertical_opposites_cancel_like_horizontal_opposites(self):
  code=read("include/nexverse/protocol/movement_controls.hpp")
  self.assertIn("const bool move_up=keys.jump&&!keys.descend;",code)
  self.assertIn("const bool move_down=keys.descend&&!keys.jump;",code)
  self.assertIn("(move_up ? control_up_pos : 0U)",code)
  self.assertIn("(move_down ? control_up_neg : 0U)",code)
  self.assertNotIn("(keys.jump ? control_up_pos : 0U)",code)
 def test_fast_bits_are_axis_specific(self):
  code=read("include/nexverse/protocol/movement_controls.hpp")
  self.assertIn("if(move_forward||move_backward)",code)
  self.assertIn("if(strafe_left||strafe_right)",code)
  self.assertIn("if(move_up||move_down)",code)
  self.assertIn("out.control_flags|=control_fast_up;",code)
  self.assertNotIn("out.control_flags |= control_fast_at |",code)
 def test_true_high4_wire_bits_checked_not_just_controller(self):
  test=read("tests/movement_controls_tests.cpp")
  self.assertIn("auto server_control_bits=",test)
  self.assertIn("encode_agent_update(52U,session,action)",test)
  self.assertIn("server_control_bits(cancel_vertical)",test)
  self.assertIn("server_control_bits(jump)!=control_up_pos",test)
  self.assertIn("server_control_bits(descend)!=control_up_neg",test)
 def test_shift_idle_turn_forward_strafe_and_up_signed(self):
  test=read("tests/movement_controls_tests.cpp")
  for x in ("control_at_pos|control_fast_at",
            "control_left_pos|control_fast_left",
            "control_up_pos|control_fast_up",
            "control_up_neg|control_fast_up",
            "control_turn_left|control_yaw_pos"):
   self.assertIn(x,test)
 def test_cancelled_axis_does_not_receive_fast_flag(self):
  test=read("tests/movement_controls_tests.cpp")
  self.assertIn("k.sprint=true;k.forward=true;k.backward=true",test)
  self.assertIn("k.sprint=true;k.left=true;k.right=true",test)
  self.assertIn("k.sprint=true;k.jump=true;k.descend=true",test)
  self.assertIn("control_fast_at|control_fast_left|control_fast_up",test)
 def test_releasing_either_vertical_key_recovers_without_repeat_edge(self):
  test=read("tests/movement_controls_tests.cpp")
  self.assertIn("k.descend=false;",test)
  self.assertIn("k.descend=true;k.jump=false;",test)
  self.assertIn("return 57;",test)
  self.assertIn("return 58;",test)
 def test_flight_toggle_not_lost_and_no_stale_vertical(self):
  test=read("tests/movement_controls_tests.cpp")
  self.assertIn("server_control_bits(c.step(k,0.1f))!=control_fly",test)
  self.assertIn("(control_fly|control_up_pos|control_fast_up)",test)
  self.assertIn("(control_fly|control_up_neg|control_fast_up)",test)
 def test_vertical_feedback_never_claims_contradictory_command(self):
  vert=read("include/nexverse/protocol/vertical_movement_feedback.hpp")
  native=read("tests/vertical_movement_feedback_tests.cpp")
  self.assertIn("return up==down?0:up?1:-1;",vert)
  self.assertIn("monitor.queued_controls(control_up_pos|control_up_neg)",native)
  self.assertIn("monitor.outbound_controls(control_up_pos|control_up_neg",native)
  self.assertIn("return 20;",native)
  self.assertIn("return 21;",native)
 def test_real_viewer_keys_and_socket_send_path_unchanged(self):
  app=read("src/app/application.cpp")
  circuit=read("src/protocol/circuit.cpp")
  self.assertIn("keys.jump = pressed(GLFW_KEY_SPACE);",app)
  self.assertIn("keys.descend = pressed(GLFW_KEY_C);",app)
  self.assertIn("movement_controller.step(",app)
  self.assertIn("if(outbound_queue.push(std::move(update)))",app)
  self.assertIn("on_agent_update_sent(value.control_flags,value.body_rotation)",circuit)
if __name__=="__main__":unittest.main()
