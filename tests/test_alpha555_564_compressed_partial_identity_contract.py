"""64-byte ObjectUpdateCompressed cannot transplant mesh/TE onto another UUID."""
import json,unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(f):return (R/f).read_text(encoding="utf-8")
class PartialCompressed(unittest.TestCase):
 def test_world_exact_object_identity(self):
  a=read("include/nexverse/world/world_model.hpp")
  self.assertIn("[[nodiscard]] bool apply_object_compressed_partial(",a)
  b=read("src/world/world_model.cpp")
  for s in ("partial.has_complete_prim_data","it->second.id!=partial.id",
     "it->second.pcode!=partial.pcode","current.state=partial.state;",
     "current.position=pos;","current.rotation=q;"):
   self.assertIn(s,b)
 def test_real_dispatcher_routes_only_known_partial(self):
  s=read("src/app/application.cpp")
  a=s.index("protocol::msg_object_update_compressed,")
  b=s.index("protocol::msg_object_update_cached,",a)
  app=s[a:b]
  for t in ("if(!entry.has_complete_prim_data)",
            "world_model.apply_object_compressed_partial(entry)",
            "world_model.upsert_object(std::move(object));",
            "update->region_handle != world_model.region().handle"):
   self.assertIn(t,app)
 def test_no_phantom_or_cross_uuid(self):
  s=read("src/world/world_model.cpp")
  a=s.index("bool WorldModel::apply_object_compressed_partial(")
  b=s.index("void WorldModel::remove_object(",a)
  body=s[a:b]
  for t in ("it==objects_.end()||it->second.id!=partial.id",
            "!region_.connected","current.state==partial.state",
            "++object_revision_;"):
   self.assertIn(t,body)
  for t in ("upsert_object(","current.parent_id=",
            "current.sculpt_asset_id=","current.texture_entry="):
   self.assertNotIn(t,body)
 def test_native_mesh_preservation_and_detach(self):
  s=read("tests/object_compressed_partial_tests.cpp")
  for t in ("worn.state=0x10U","header.state=0x20U",
   "header.state=0U;", "worn.texture_entry={10U,11U,12U}",
   "worn.parsed_texture_entry=protocol::TextureEntry{}",
   "model.object_revision()!=2U","recycled.id=",
   "model.remove_object(42U)","model.begin_region_transition("):
   self.assertIn(t,s)
 def test_real_64_byte_wire_to_world(self):
  s=read("tests/object_compressed_partial_tests.cpp")
  for t in ("std::vector<std::uint8_t> raw(8U+2U+1U+4U+2U+64U,0U)",
    "protocol::uuid_to_bytes(worn.id)",
    "protocol::decode_object_update_compressed(raw)",
    "model.apply_object_compressed_partial(received->objects[0])",
    "recycled_wire->objects[0]"):
   self.assertIn(t,s)
 def test_actual_firestorm_gpu_pixels(self):
  s=read("tests/scene_animated_attachment_pixels_tests.cpp")
  for t in ("partial.state=0x10U",
    "region_objects.apply_object_compressed_partial(partial)",
    "PARTIAL_COMPRESSED_ATTACHMENT_GL_PIXELS=",
    "if(compressed_delta<40U)failure=15;"):
   self.assertIn(t,s)
 def test_previous_guards_preserved(self):
  s=read("src/app/application.cpp")
  self.assertIn("world_model.apply_object_terse(",s)
  self.assertIn("world::reconcile_avatar_attachments(",s)
 def test_version_and_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.934")
  d=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(d["viewer_version"],"0.2.0-alpha.934")
  self.assertEqual(d["next_batch"],"Alpha.935–944")
  self.assertEqual(d["default_blocks_per_run"],10)
  self.assertIn("still open",d["assessment"])
  for f in ("CHATUEBERGABE.md","projectplan.md","AGENTS.md",
            "info.md","docs/PROJEKTFORTSCHRITT.md"):
   self.assertIn("Alpha.935–944",read(f)[:1900])
if __name__=="__main__":unittest.main()
