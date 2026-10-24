"""OpenGenesisLINK legacy low158 omits attachments; root prim State supplies them."""
import json,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(f):return (ROOT/f).read_text(encoding="utf-8")
class RealAttachmentSource(unittest.TestCase):
 def test_full_and_compressed_preserve_wire_state(self):
  h=read("include/nexverse/protocol/viewer_messages.hpp")
  self.assertGreaterEqual(h.count("std::uint8_t state=0U;"),2)
  s=read("src/protocol/viewer_messages.cpp")
  self.assertIn("object.state=payload[offset++];",s)
  self.assertIn("object.state = data[21U];",s)
 def test_world_preserves_parent_and_state(self):
  s=read("src/app/application.cpp")
  self.assertIn("object.state=entry.state;",s)
  self.assertIn("object.parent_id = entry.parent_id;",s)
  # Since Alpha.564 a 64-byte compressed header cannot inherit
  # another UUID's parent; only an exact current UUID can update it.
  self.assertIn("world_model.apply_object_compressed_partial(entry)",s)
  self.assertIn("object.parent_id = entry.parent_id;",s)
  self.assertIn("std::uint8_t state=0U;",read("include/nexverse/world/world_model.hpp"))
 def test_attachment_nibbles_and_current_sim_parent(self):
  s=read("include/nexverse/world/attachment_state.hpp")
  for t in ("(wire_state>>4U)|(wire_state<<4U)", "avatar_local_id==0U",
   "prim.parent_id!=avatar_local_id","prim.pcode!=protocol::pcode_prim",
   "slot==0U||slot>=56U","result.emplace(prim.id,slot);"):
   self.assertIn(t,s)
 def test_real_self_and_other_render_rig(self):
  s=read("src/app/application.cpp")
  self.assertGreaterEqual(s.count("world::reconcile_avatar_attachments("),4)
  self.assertIn("world_model.objects(),world::current_self_avatar_local_id(",s)
  self.assertIn("world_model.objects(),avatar.local_id",s)
  self.assertIn("appearance->attachments",s)
 def test_true_wire_native_and_gpu(self):
  wire=read("tests/viewer_messages_tests.cpp")
  self.assertIn("full_update->objects[0].state != 0x10U",wire)
  self.assertIn("compressed->objects[0].state != 0xF1U",wire)
  native=read("tests/self_attachment_state_tests.cpp")
  for t in ("decode_server_attachment_point(0xF1U)",
            'selected.contains("unrelated")','selected.contains("child")',
            "AttachmentSceneKind::hud","objects.erase(200U)"):
   self.assertIn(t,native)
  gl=read("tests/scene_animated_attachment_pixels_tests.cpp")
  self.assertIn("world::infer_avatar_attachment_points(",gl)
  self.assertIn("JOINT_ATTACHMENT_POSED_RGB_PIXELS",gl)
 def test_f3_sources_no_uuid_leak(self):
  self.assertIn('" | Obj-Anhaenge:"+std::to_string(',read("src/ui/viewer_shell_render.cpp"))
  d=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn('"self_object_attachment_roots="+',d)
  self.assertNotIn("self_object_attachment_uuid=",d)
 def test_handoff(self):
  self.assertEqual(read("VERSION").strip(),"v0.4.0.0")
  j=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(j["viewer_version"],"v0.4.0.0")
  self.assertEqual(j["next_batch"],"v0.4.0.0")
  self.assertEqual(j["default_blocks_per_run"],10)
  self.assertIn("still open",j["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("v0.4.0.0",read(f)[:1600])
if __name__=="__main__":unittest.main()
