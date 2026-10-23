"""FastRez early Low158/Low382 cannot overwrite newer direct readback."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class LateFlushAuthority(unittest.TestCase):
 def test_world_model_current_sim_socket_lease(self):
  h=rd("include/nexverse/world/world_model.hpp")
  s=rd("src/world/world_model.cpp")
  for token in ("std::string simulator_ip;","std::uint16_t simulator_port=0U;",
                "source.simulator_ip==region_.simulator_ip",
                "source.simulator_port==region_.simulator_port",
                "region_.simulator_ip,region_.simulator_port"):
   self.assertIn(token,h+s)
  self.assertEqual(s.count("region_.simulator_ip,region_.simulator_port"),2)
 def test_newer_live_current_low158_never_replaced(self):
  s=rd("src/world/world_model.cpp")
  self.assertIn("if(valid(staged.source)&&!self_appearance_)",s)
  self.assertIn("result.wearables=apply_self_wearables(std::move(staged.packet));",s)
  test=rd("tests/early_avatar_readback_tests.cpp")
  for marker in ("const auto newer=appearance(42);","const auto late_flush=live.flush_early_self_readbacks();",
                 "live_legacy.appearance_data.clear();","other_socket.simulator_port=9001U;",
                 "endpoint.early_self_readbacks_pending()[0]"):
   self.assertIn(marker,test)
 def test_ui_pending_true_until_authorized(self):
  s=rd("src/app/application.cpp")
  self.assertIn("world_model.early_self_readbacks_pending()",s)
  self.assertIn("Low158 gepuffert (warte Circuit)",s)
  self.assertIn("Low382 gepuffert (warte Circuit)",s)
  self.assertIn("shell_view.avatar_handshake_status+=",s)
  self.assertIn("world_model.flush_early_self_readbacks()",s)
  self.assertIn("world_model.stage_early_self_appearance(*appearance)",s)
  self.assertIn("world_model.stage_early_self_wearables(*update)",s)
 def test_original_native_arm_and_release(self):
  s=rd("scripts/pi5-headless-tests.sh")
  self.assertIn("early_avatar_readback_tests; do",s)
  self.assertIn("nexverse-early-avatar-readback-tests",rd("CMakeLists.txt"))
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.934")
  self.assertIn("Alpha.935–944",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
