"""ImprovedTerse is pose-only and must preserve actual server attachment data."""
import json,unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class TerseAttachmentPreservation(unittest.TestCase):
 def test_world_api_pose_only(self):
  h=read("include/nexverse/world/world_model.hpp")
  self.assertIn("[[nodiscard]] bool apply_object_terse(",h)
  s=read("src/world/world_model.cpp")
  for t in ("if(!region_.connected || local_id==0U)return false;",
            "if(it==objects_.end()||it->second.pcode!=protocol::pcode_prim||",
            "it->second.id.empty())return false;",
            "current.position=position;","current.rotation=rotation;",
            "++object_revision_;"):
   self.assertIn(t,s)
 def test_no_phantom_and_material_loss(self):
  s=read("src/world/world_model.cpp")
  a=s.index("bool WorldModel::apply_object_terse(")
  b=s.index("bool WorldModel::apply_object_compressed_partial(",a)
  c=s[a:b]
  self.assertNotIn("upsert_object(",c)
  self.assertNotIn("ObjectState object",c)
  self.assertNotIn("current.state=",c)
  self.assertNotIn("current.parent_id=",c)
 def test_invalid_or_duplicate_updates_rejected(self):
  s=read("src/world/world_model.cpp")
  for t in ("!finite(position.x)||!finite(position.y)||!finite(position.z)",
            "norm<0.25||norm>4.0","current.position.x==position.x",
            "return false; // duplicate Terse does not reupload scene"):
   self.assertIn(t,s)
 def test_actual_packet_dispatcher_uses_update_not_insert(self):
  s=read("src/app/application.cpp")
  a=s.index("protocol::msg_improved_terse_object_update,")
  b=s.index("protocol::msg_kill_object,",a)
  high9=s[a:b]
  self.assertIn("world_model.apply_object_terse(",high9)
  self.assertNotIn("world_model.upsert_object(",high9)
  self.assertIn("world_model.update_avatar_transform(",high9)
  self.assertIn("terse->region_handle != world_model.region().handle",high9)
 def test_native_attachment_and_mesh_preservation(self):
  s=read("tests/object_terse_attachment_tests.cpp")
  for t in ("sleeve.state=0x10U;","sleeve.parent_id=51U;",
   "sleeve.texture_entry={10U,11U,12U};",
   "sleeve.parsed_texture_entry=protocol::TextureEntry{};",
   "sleeve.default_texture=protocol::TextureEntryDefaultFace{};",
   "sleeve.sculpt_asset_id=",
   "updated.state!=0x10U","updated.parent_id!=51U",
   "updated.texture_entry!=sleeve.texture_entry",
   "!updated.parsed_texture_entry||!updated.default_texture",
   "model.objects().size()!=1U",
   "std::numeric_limits<float>::quiet_NaN()", "model.remove_object(42U);",
   "model.begin_region_transition("):
   self.assertIn(t,s)
 def test_real_gpu_pixels_respond_to_terse_pose(self):
  s=read("tests/scene_animated_attachment_pixels_tests.cpp")
  for t in ("region_objects.apply_object_terse(",
    "infer_avatar_attachment_points(",
    "attached.position={new_pos.x,new_pos.y,new_pos.z};",
    "TERSE_ATTACHMENT_WORLD_MODEL_GPU_PIXELS=",
    "if(actual_terse_gpu_pixels<40U)failure=13;"):
   self.assertIn(t,s)
 def test_original_upstream_decoder_still_present(self):
  h=read("include/nexverse/world/attachment_state.hpp")
  self.assertIn("(wire_state>>4U)|(wire_state<<4U)",h)
  s=read("src/app/application.cpp")
  self.assertIn("object.state=entry.state;",s)
  self.assertIn("object.parsed_texture_entry =",s)
 def test_next_batch_docs(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.924")
  s=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(s["viewer_version"],"0.2.0-alpha.924")
  self.assertEqual(s["next_batch"],"Alpha.925–934")
  self.assertEqual(s["default_blocks_per_run"],10)
  self.assertIn("still open",s["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.925–934",read(f)[:1700])
if __name__=="__main__":unittest.main()
