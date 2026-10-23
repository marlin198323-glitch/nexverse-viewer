"""Low AgentMovementComplete message ID must not activate an unauthenticated circuit."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class CircuitMovementAdmission(unittest.TestCase):
 def test_pure_full_decoded_current_login_gate(self):
  s=rd("include/nexverse/protocol/movement_complete_admission.hpp")
  for k in ("valid_circuit_movement_complete(", "decode_agent_movement_complete(payload)",
            "if(!movement_request_sent)", "reply->agent_id!=session.agent_id",
            "reply->session_id!=session.session_id",
            "reply->region_handle==0U", "std::abs(p.x)<=1000000.0f"):
   self.assertIn(k,s)
 def test_real_circuit_state_not_flipped_before_gate(self):
  c=rd("src/protocol/circuit.cpp")
  start=c.index("if (packet.header.message == msg_agent_movement_complete)")
  end=c.index("return true;\n}",start)
  body=c[start:end]
  self.assertIn("if(!valid_circuit_movement_complete(",body)
  self.assertIn("packet.payload,session_,movement_sent",body)
  self.assertIn("status.movement_complete_rejected",body)
  self.assertIn("status.movement_complete_validated=true;",body)
  self.assertLess(body.index("if(!valid_circuit_movement_complete("),
                  body.index("status.state = CircuitState::active;"))
  self.assertLess(body.index("status.state = CircuitState::active;"),
                  body.index("encode_agent_wearables_request("))
  self.assertIn("if(!wearables_initial_requested_)",body)
 def test_f3_does_not_fake_world_login(self):
  s=rd("src/app/application.cpp")
  self.assertIn("Low15 abgewiesen:",s)
  self.assertIn("circuit_snapshot.movement_complete_rejected",s)
  self.assertIn("world_model.self_avatar_controls_ready()",s)
  h=rd("include/nexverse/protocol/circuit.hpp")
  self.assertIn("bool movement_complete_validated=false;",h)
  self.assertIn("std::uint32_t movement_complete_rejected=0U;",h)
 def test_wire_regression_in_arm64_ci(self):
  native=rd("tests/movement_complete_admission_tests.cpp")
  for k in ("encode_packet(packet_reliable", "decode_lludp_packet(packet)",
            "valid_circuit_movement_complete(", "mutated[0]^=1U",
            "mutated[16]^=1U", "mutated.resize(50U)",
            "2000000.0f", "quiet_NaN()", "vertical LookAt"):
   self.assertIn(k,native)
  self.assertIn("nexverse-movement-complete-admission-tests",rd("CMakeLists.txt"))
  self.assertIn("movement_complete_admission_tests",rd("scripts/pi5-headless-tests.sh"))
 def test_version_and_next_batch(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.1144")
  self.assertIn("Alpha.1145–1154",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
