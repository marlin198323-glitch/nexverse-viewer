"""FastRez Low158/Low382 may arrive before CircuitState::active is published."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def src(path):return (ROOT/path).read_text()
class FastRezEarlyReadback(unittest.TestCase):
 def test_worldmodel_bounded_session_region_inbox(self):
  model=src("src/world/world_model.cpp")
  h=src("include/nexverse/world/world_model.hpp")
  for k in ("stage_early_self_appearance(", "stage_early_self_wearables(",
            "flush_early_self_readbacks()", "latest_nonzero_avatar_cof(",
            "valid_self_appearance_texture_entry(", "wearables_serial_newer(",
            "region_handle==region_.handle", "source.session_id==agent_.session_id"):
   self.assertIn(k,model)
  self.assertIn("std::optional<EarlySelfPacket<protocol::AvatarAppearanceMessage>>",h)
  self.assertIn("std::optional<EarlySelfPacket<protocol::AgentWearablesUpdateMessage>>",h)
  self.assertGreaterEqual(model.count("early_self_appearance_.reset();"),5)
  self.assertGreaterEqual(model.count("early_self_wearables_.reset();"),5)
 def test_real_udp_handlers_and_circuit_release(self):
  app=src("src/app/application.cpp")
  self.assertIn("world_model.stage_early_self_appearance(*appearance)",app)
  self.assertIn("world_model.stage_early_self_wearables(*update)",app)
  self.assertIn("world_model.flush_early_self_readbacks();",app)
  self.assertIn("if(recovered.appearance)",app)
  self.assertIn("self_appearance_serial.fetch_add(",app)
  self.assertIn("if(recovered.wearables)",app)
  self.assertIn("region_circuit_active});",app)
  self.assertLess(app.index("world_model.stage_early_self_appearance(*appearance)"),
                  app.index("world_model.stage_early_self_wearables(*update)"))
 def test_native_pi5_includes_real_worldmodel_test(self):
  native=src("tests/early_avatar_readback_tests.cpp")
  for k in ("stage_early_self_appearance(appearance())",
            "stage_early_self_wearables(worn(7))",
            "model.flush_early_self_readbacks()",
            "model.begin_region_transition(", "changed-session",
            "protocol::avatar_cof_version_for_display("):
   self.assertIn(k,native)
  self.assertIn("nexverse-early-avatar-readback-tests",src("CMakeLists.txt"))
  self.assertIn("early_avatar_readback_tests; do",src("scripts/pi5-headless-tests.sh"))
 def test_version_handoff(self):
  self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.934")
  self.assertIn("Alpha.935–944",src("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
