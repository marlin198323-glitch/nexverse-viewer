"""A decoded AgentMovementComplete has server LookAt, never BodyRotation."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def s(path):return (R/path).read_text()
class MovementLookAtContract(unittest.TestCase):
 def test_decoder_wire_contains_real_look_at_not_quaternion(self):
  m=s("include/nexverse/protocol/viewer_messages.hpp")
  self.assertIn("struct AgentMovementCompleteMessage",m)
  self.assertIn("Vec3 look_at;",m)
  self.assertIn("message.look_at.x = read_f32(",s("src/protocol/viewer_messages.cpp"))
  h=s("include/nexverse/protocol/agent_movement_look_at.hpp")
  self.assertIn("simulator_look_at_heading(Vec3 look_at)",h)
  self.assertIn("std::atan2(y,x)",h)
  self.assertIn("return std::nullopt;",h)
 def test_authentic_packet_handler_never_claims_provisional_yaw(self):
  a=s("src/app/application.cpp")
  begin=a.index("packet_dispatcher.register_handler(\n        protocol::msg_agent_movement_complete,")
  end=a.index("packet_dispatcher.register_handler(",begin+10)
  handler=a[begin:end]
  self.assertIn("protocol::simulator_look_at_heading(movement->look_at)",handler)
  self.assertIn("world_model.accept_simulator_self_position(",handler)
  self.assertIn("world_model.accept_simulator_self_pose(",handler)
  self.assertNotIn("movement->position,\n                   agent.rotation)",handler)
 def test_control_gate_and_actual_worldmodel_position_only(self):
  a=s("src/world/world_model.cpp")
  self.assertIn("bool WorldModel::accept_simulator_self_position(",a)
  self.assertIn("!finite_simulator_position(position)",a)
  self.assertIn("agent_.rotation,agent_.present",a)
  header=s("include/nexverse/world/world_model.hpp")
  self.assertIn("accept_simulator_self_position(",header)
  self.assertIn("self_avatar_controls_ready()",header)
 def test_native_and_pi5_and_version(self):
  n=s("tests/self_agent_movement_look_at_tests.cpp")
  for token in ("simulator_look_at_heading({0.0f,3.0f,0.0f})",
                "world.accept_simulator_self_position(",
                "world.accept_simulator_self_pose(",
                "world.self_avatar_controls_ready()", "world.apply_region(area(false));",
                "control_at_pos"):
   self.assertIn(token,n)
  self.assertIn("nexverse-self-agent-movement-look-at-tests",s("CMakeLists.txt"))
  self.assertIn("self_agent_movement_look_at_tests",s("scripts/pi5-headless-tests.sh"))
  self.assertEqual(s("VERSION").strip(),"0.2.0-alpha.1044")
  self.assertIn("Alpha.1045–1054",s("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
