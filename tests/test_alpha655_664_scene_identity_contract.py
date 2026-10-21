"""Production-path contracts: LocalID/UUID leases and safe F7 reporting."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(p): return (ROOT/p).read_text()
class SceneIdentityTests(unittest.TestCase):
 def test_collision_and_relocation_are_distinct(self):
  s=read("src/world/world_model.cpp")
  start=s.index("void WorldModel::upsert_object(")
  stop=s.index("bool WorldModel::apply_object_terse(",start)
  block=s[start:stop]
  for text in ("valid_simulator_prim_geometry(","prim_uuid_to_local_id_.find(object.id)",
               "kill_simulator_local_id(prior_uuid->second,false)",
               "kill_simulator_local_id(object.local_id,false)",
               "++prim_uuid_relocations_","++prim_local_id_rebinds_"):
   self.assertIn(text,block)
 def test_current_self_avatar_full_compressed_terse_guards(self):
  a=read("src/app/application.cpp")
  full=a[a.index("protocol::msg_object_update,"):a.index("protocol::msg_object_update_compressed,")]
  compressed=a[a.index("protocol::msg_object_update_compressed,"):a.index("protocol::msg_object_update_cached,")]
  terse=a[a.index("protocol::msg_improved_terse_object_update,"):a.index("protocol::msg_kill_object,")]
  for piece in (full,compressed,terse):
   self.assertIn("world::valid_simulator_avatar_pose(",piece)
  self.assertIn("world_model.apply_agent(",terse)
 def test_f7_has_real_source_and_distinct_visible_rows(self):
  a=read("src/app/application.cpp"); h=read("include/nexverse/ui/viewer_shell_render.hpp")
  ui=read("src/ui/viewer_shell_render.cpp")
  for field in ("prim_uuid_relocations","invalid_scene_updates","prim_local_id_rebinds"):
   self.assertIn("world_model."+field+"()",a)
   self.assertIn(field,h)
   self.assertIn("v."+field,ui)
  for visible in ("UUID umgezogen:","Daten verworfen:","KillObject Avatar/Prims:","ID neu:"):
   self.assertIn(visible,ui)
 def test_world_reset_counters_and_invalid_first(self):
  s=read("src/world/world_model.cpp")
  self.assertGreaterEqual(s.count("prim_uuid_relocations_=0U;"),4)
  self.assertGreaterEqual(s.count("invalid_scene_updates_=0U;"),4)
  i=s.index("void WorldModel::upsert_object(")
  b=s[i:s.index("bool WorldModel::apply_object_terse(",i)]
  self.assertLess(b.index("valid_simulator_prim_geometry("),b.index("kill_simulator_local_id("))
 def test_uuid_index_always_tracks_scene_mutation(self):
  src=read("src/world/world_model.cpp")
  self.assertEqual(src.count("prim_uuid_to_local_id_.clear();"),4)
  self.assertIn("prim_uuid_to_local_id_.erase(index);",src)
  self.assertIn("prim_uuid_to_local_id_.insert_or_assign(object.id,object.local_id);",src)
 def test_native_tests_are_real_not_only_text_fixture(self):
  src=read("tests/prim_uuid_relocation_tests.cpp")
  for token in ("old-child","grandchild","invalid_scene_updates()",
                "update_avatar_transform","prim_uuid_relocations()","apply_region"):
   self.assertIn(token,src)
  cmake=read("CMakeLists.txt"); runner=read("scripts/pi5-headless-tests.sh")
  self.assertIn("tests/prim_uuid_relocation_tests.cpp",cmake)
  self.assertIn("prim_uuid_relocation_tests",runner)
  self.assertIn("simulator_scene_validity_tests",runner)
if __name__=="__main__": unittest.main()
