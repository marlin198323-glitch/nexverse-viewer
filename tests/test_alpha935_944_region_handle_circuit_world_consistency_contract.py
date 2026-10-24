"""Circuit Low15 must agree with the actual region that the WorldModel expects."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(f):return (R/f).read_text()
class Low15RegionIdentity(unittest.TestCase):
 def test_circuit_admission_matches_login_region(self):
  s=rd("include/nexverse/protocol/movement_complete_admission.hpp")
  self.assertIn("if(session.region_x!=0U||session.region_y!=0U)",s)
  self.assertIn("static_cast<std::uint64_t>(session.region_x)<<32U",s)
  self.assertIn("static_cast<std::uint64_t>(session.region_y)",s)
  self.assertIn("if(reply->region_handle!=expected)return false;",s)
  self.assertIn("decode_agent_movement_complete(payload)",s)
  self.assertIn("reply->agent_id!=session.agent_id",s)
 def test_real_worldmodel_same_session_handle_math(self):
  app=rd("src/app/application.cpp")
  self.assertIn("static_cast<std::uint64_t>(active_session->region_x) << 32U",app)
  self.assertIn("static_cast<std::uint64_t>(active_session->region_y)",app)
  self.assertIn("movement->region_handle != region.handle",app)
  self.assertIn("world_model.accept_simulator_self_pose(",app)
 def test_f3_distinguishes_wrong_region_from_other_low15_rejects(self):
  h=rd("include/nexverse/protocol/circuit.hpp")
  c=rd("src/protocol/circuit.cpp")
  app=rd("src/app/application.cpp")
  s=rd("include/nexverse/protocol/movement_complete_admission.hpp")
  self.assertIn("movement_complete_wrong_login_region(",s)
  self.assertIn("movement_complete_wrong_region=0U;",h)
  self.assertIn("movement_complete_wrong_login_region(packet.payload,session_)",c)
  self.assertIn("status.movement_complete_wrong_region<9999U",c)
  self.assertIn("Low15 falsche Login-Region:",app)
  self.assertIn("Low15 abgewiesen:",app)
  self.assertLess(c.index("if(!valid_circuit_movement_complete("),
                  c.index("status.state = CircuitState::active;"))
 def test_wire_native_proof_wrong_handle_and_legacy(self):
  t=rd("tests/movement_complete_admission_tests.cpp")
  for x in ("login.region_x=256U;", "login.region_y=512U;",
            "mutated[56U]^=1U;", "movement_complete_wrong_login_region(mutated,login)",
            "wrong_login_region.region_x=0U;", "wrong_login_region.region_y=0U;",
            "!valid_circuit_movement_complete(p,wrong_login_region,true)"):
   self.assertIn(x,t)
  self.assertIn("movement_complete_admission_tests",rd("scripts/pi5-headless-tests.sh"))
  self.assertIn("nexverse-movement-complete-admission-tests",rd("CMakeLists.txt"))
 def test_next_version(self):
  self.assertEqual(rd("VERSION").strip(),"v0.4.0.0")
  self.assertIn("v0.4.0.0",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
