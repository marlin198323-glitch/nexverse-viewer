"""Same region handle does not authorize stale avatar data from a new simulator socket."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class SimulatorEndpointEpochContract(unittest.TestCase):
 def test_known_simulator_socket_change_invalidates_scene(self):
  s=read("src/world/world_model.cpp")
  for x in ("const bool same_handle=", "const std::string effective_ip=",
            "const std::uint16_t effective_port=",
            "const bool simulator_socket_changed=",
            "region_.simulator_ip!=effective_ip",
            "region_.simulator_port!=effective_port",
            "simulator_socket_changed;",
            "region_.simulator_ip = effective_ip;",
            "region_.simulator_port = effective_port;"):
   self.assertIn(x,s)
  self.assertIn("early_self_appearance_.reset();",s)
  self.assertIn("self_pose_confirmed_=false;",s)
 def test_incomplete_same_region_update_does_not_erase_endpoint(self):
  s=read("src/world/world_model.cpp")
  self.assertIn("same_handle&&update.simulator_ip.empty()",s)
  self.assertIn("same_handle&&update.simulator_port==0U",s)
  self.assertIn("region_.simulator_ip:update.simulator_ip",s)
  self.assertIn("region_.simulator_port:update.simulator_port",s)
 def test_native_real_worldmodel_reconnect_proof(self):
  s=read("tests/early_avatar_readback_tests.cpp")
  for marker in ("socket_region.apply_region(area(region,true));",
                 "socket_region.simulator_self_pose_confirmed()",
                 "socket_region.avatars().empty()",
                 "socket_region.region().generation!=stable_epoch",
                 "incomplete.simulator_ip.clear();",
                 "incomplete.simulator_port=0U;",
                 "switched.simulator_port=9001U;",
                 "switched.simulator_ip=\"127.0.0.2\";",
                 "socket_region.self_avatar_renderable()"):
   self.assertIn(marker,s)
  self.assertIn("early_avatar_readback_tests; do",read("scripts/pi5-headless-tests.sh"))
 def test_next_phase(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1084")
  self.assertIn("Alpha.1085–1094",read("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
