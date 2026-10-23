"""OpenGenesisLINK legacy Low158 hover-only packet is legitimate self appearance."""
from pathlib import Path
import json
import unittest
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text(encoding="utf-8")
class OpenGenesisLow158(unittest.TestCase):
 def test_real_server_omitted_attachment_block(self):
  cpp=read("src/protocol/viewer_messages.cpp")
  self.assertIn("LLClientView.SendAppearance emits one AppearanceHover vector",cpp)
  self.assertIn("if (offset == payload.size()) return message;",cpp)
  self.assertIn("message.attachment_block_present=true;",cpp)
 def test_strict_modern_distinction(self):
  hdr=read("include/nexverse/protocol/viewer_messages.hpp")
  self.assertIn("bool attachment_block_present=false;",hdr)
  app=read("src/app/application.cpp")
  for s in ("appearance->attachment_block_present;",
            "ap->attachment_block_present;",
            "legacy_attachment_block=omitted",
            "modern_attachment_block=present"):
   self.assertIn(s,app)
 def test_upstream_zerocoded_wire_to_world(self):
  code=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for s in ("upstream_legacy_hover_packet(",
    "protocol::packet_zerocoded",
    "const auto server_frame=protocol::decode_lludp_packet(upstream_wire);",
    "const auto legacy=decode(upstream_wire);",
    "legacy->hover_heights.front().z!=0.25f",
    "legacy->attachment_block_present",
    "!w.apply_self_appearance(*legacy)",
    "w.self_appearance()->attachment_block_present"):
   self.assertIn(s,code)
 def test_actual_send_appearance_fields(self):
  code=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for s in ("p.push_back(0U); // appearance-data count=0",
      "p.push_back(1U); // hover count=1",
      "le32(p,0U);le32(p,0U);le32(p,0x3e800000U)",
      "legacy->appearance_data.empty()"):
   self.assertIn(s,code)
 def test_truncated_hover_and_other_agent_rejected(self):
  code=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  self.assertIn("upstream_truncated.pop_back();",code)
  self.assertIn("if(decode(upstream_truncated))return 31;",code)
  self.assertIn("w.apply_self_appearance(*upstream_foreign)",code)
  msg=read("tests/viewer_messages_tests.cpp")
  self.assertIn("if(decode_avatar_appearance(truncated_hover))return 134;",msg)
 def test_user_interface_and_gl_test(self):
  view=read("src/ui/viewer_shell_render.cpp")
  self.assertIn("SSA-Anhaenge: Legacy-Block fehlt",view)
  self.assertIn("SSA-Anhaenge: Serverantwort ausstehend",view)
  test=read("tests/viewer_shell_gl_tests.cpp")
  self.assertIn("view.appearance_ready=true;",test)
  self.assertIn("view.avatar_self_attachment_packet=false;",test)
 def test_model_preserves_skin_gate(self):
  model=read("src/world/world_model.cpp")
  self.assertIn("valid_self_appearance_texture_entry(appearance.texture_entry)",model)
 def test_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.974")
  meta=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(meta["viewer_version"],"0.2.0-alpha.974")
  self.assertEqual(meta["next_batch"],"Alpha.975–984")
  self.assertEqual(meta["default_blocks_per_run"],10)
  self.assertIn("still open",meta["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.975–984",read(f)[:1500])
if __name__=="__main__":unittest.main()
