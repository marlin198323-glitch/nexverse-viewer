"""Original legacy Low381/382 wearable query must remain authenticated and read-only."""
from pathlib import Path
import json
import unittest
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class ReadOnlyWearables(unittest.TestCase):
 def test_exact_low381_and_382_ids(self):
  h=read("include/nexverse/protocol/lludp.hpp")
  self.assertIn("msg_agent_wearables_request{MessageFrequency::low,381U}",h)
  self.assertIn("msg_agent_wearables_update{MessageFrequency::low,382U}",h)
  cpp=read("src/protocol/viewer_messages.cpp")
  for token in ("encode_agent_wearables_request(","append_uuid(payload,session.agent_id)",
                "append_uuid(payload,session.session_id)",
                "msg_agent_wearables_request","decode_agent_wearables_update(",
                "const std::size_t count=payload[36U]","header_size+count*item_size",
                "entry.wearable_type>16U"):
   self.assertIn(token,cpp)
 def test_initial_once_after_active_circuit(self):
  cpp=read("src/protocol/circuit.cpp")
  self.assertIn("packet.header.message == msg_agent_movement_complete",cpp)
  self.assertIn("if(!wearables_initial_requested_)",cpp)
  self.assertIn("wearables_initial_requested_=true",cpp)
  self.assertIn("status.wearables_read_request_sent=true",cpp)
  self.assertIn("send_reliable(socket,tracker,std::move(request)",cpp)
  self.assertIn("wearables_initial_requested_=false",cpp)
  self.assertNotIn("encode_agent_set_appearance(",cpp)
 def test_owner_session_serial_and_region(self):
  model=read("src/world/world_model.cpp")
  for s in ("bool WorldModel::apply_self_wearables(",
            "update.agent_id!=agent_.agent_id",
            "update.session_id!=agent_.session_id",
            "update.serial<=self_wearables_->serial",
            "self_wearables_.reset();","self_wearables = self_wearables_"):
   self.assertIn(s,model)
  test=read("tests/agent_wearables_readback_tests.cpp")
  for s in ("decode_lludp_packet","msg_agent_wearables_update",
            "w.apply_self_wearables(*foreign)",
            "w.apply_self_wearables(*wrong_session)",
            "w.apply_self_wearables(*older)",
            "w.begin_region_transition("):
   self.assertIn(s,test)
 def test_no_inferred_outfit_truth(self):
  app=read("src/app/application.cpp")
  self.assertIn("msg_agent_wearables_update",app)
  self.assertIn("world_model.apply_self_wearables",app)
  self.assertIn("not proof of current COF or body bakes",app)
  self.assertIn("Wearables: Legacy-Antwort ausstehend",app)
  self.assertIn("Assets (COF offen)",app)
 def test_ui_is_gl_tested(self):
  view=read("src/ui/viewer_shell_render.cpp")
  self.assertIn("v.avatar_wearables_status",view)
  renderer=read("tests/viewer_shell_gl_tests.cpp")
  self.assertIn("Wearables: Legacy 13/17 Assets (COF offen)",renderer)
  self.assertIn("Wearables: Legacy-Antwort ausstehend",renderer)
 def test_privacy_numeric_only(self):
  report=read("src/ui/avatar_diagnostic_report.cpp")
  app=read("src/app/application.cpp")
  for key in ("wearables_read_request_sent","wearables_legacy_update_received",
              "wearables_legacy_items","wearables_legacy_assets",
              "wearables_legacy_serial"):
   self.assertIn(key+"=",report)
   self.assertIn("avatar_diagnostic."+key,app)
  for bad in ("wearables_item_uuid=","wearables_asset_uuid=",
              "wearables_session_id="):
   self.assertNotIn(bad,report)
 def test_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.684")
  s=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(s["viewer_version"],"0.2.0-alpha.684")
  self.assertEqual(s["next_batch"],"Alpha.685–694")
  self.assertEqual(s["default_blocks_per_run"],10)
  self.assertIn("still open",s["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.685–694",read(f)[:1200])
if __name__=="__main__":unittest.main()
