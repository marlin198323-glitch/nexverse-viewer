"""Low158 modern multiattachment decode and truthful viewer F3 evidence."""
import unittest,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(f):return (ROOT/f).read_text(encoding="utf-8")
class Low158Attachments(unittest.TestCase):
 def test_full_firestorm_variable1_capacity(self):
  cpp=read("src/protocol/viewer_messages.cpp")
  self.assertIn("attachment_count != (payload.size() - offset) / 17U",cpp)
  self.assertNotIn("attachment_count > 64U",cpp)
  self.assertIn("(payload.size() - offset) % 17U != 0U",cpp)
  self.assertIn("Firestorm AttachmentBlock is Variable 1",cpp)
 def test_actual_low158_transport_and_mtu(self):
  cpp=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for x in ("protocol::encode_packet(","protocol::msg_avatar_appearance",
            "decode_lludp_packet(","datagram(self,65U)","datagram(self,255U)",
            "maximal_frame.size()<=protocol::lludp_mtu",
            "protocol::decode_avatar_appearance(","attachments.size()!=65U",
            "attachments.size()!=255U","w.apply_self_appearance(*medium)"):
   self.assertIn(x,cpp)
 def test_malformed_and_other_identity(self):
  cpp=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  for x in ("broken.pop_back()","reason!=protocol::AvatarAppearanceFailure::attachments",
            "inconsistent[offset]","w.apply_self_appearance(*foreign)",
            "w.apply_agent({other,session", "begin_region_transition("):
   self.assertIn(x,cpp)
 def test_actual_f3_draw_and_session_scope(self):
  app=read("src/app/application.cpp")
  view=read("src/ui/viewer_shell_render.cpp")
  hdr=read("include/nexverse/ui/viewer_shell_render.hpp")
  self.assertIn("shell_view.avatar_self_attachment_packet=",app)
  self.assertIn("appearance->attachment_block_present;",app)
  self.assertIn("appearance->avatar_id==active_session->agent_id",app)
  self.assertIn("shell_view.attachments=appearance->attachments.size();",app)
  self.assertIn("SSA-Anhaenge:",view)
  self.assertIn("nur Server, Render offen",view)
  self.assertIn("avatar_self_attachment_packet",hdr)
  self.assertNotIn("Objekte:"+'+"std::to_string(v.attachments)',view)
 def test_private_numeric_only_and_visual_gl_test(self):
  report=read("src/ui/avatar_diagnostic_report.cpp")
  for text in ("self_attachment_block_received=","self_attachment_block_count="):
   self.assertIn(text,report)
  self.assertNotIn("self_attachment_uuid=",report)
  cpp=read("tests/viewer_shell_gl_tests.cpp")
  self.assertIn("view.avatar_self_attachment_packet=true",cpp)
  self.assertIn("view.attachments=85U",cpp)
  self.assertIn("view.avatar_self_attachment_packet=false",cpp)
 def test_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.764")
  meta=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(meta["viewer_version"],"0.2.0-alpha.764")
  self.assertEqual(meta["next_batch"],"Alpha.765–774")
  self.assertEqual(meta["default_blocks_per_run"],10)
  self.assertIn("still open",meta["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.765–774",read(f)[:1200])
if __name__=="__main__":unittest.main()
