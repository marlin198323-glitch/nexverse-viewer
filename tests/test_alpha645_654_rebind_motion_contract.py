"""Regression contracts for actual prim UUID lease and self motion reset."""
import pathlib, unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
def read(p): return (ROOT/p).read_text()
class TestLease(unittest.TestCase):
 def test_real_prim_identity_clears_old_children(self):
  src=read("src/world/world_model.cpp")
  start=src.index("void WorldModel::upsert_object(")
  end=src.index("bool WorldModel::apply_object_terse(",start)
  scope=src[start:end]
  for token in ("prior->second.id!=object.id","kill_simulator_local_id(object.local_id,false)",
                "++prim_local_id_rebinds_","objects_[object.local_id] = std::move(object)",
                "!region_.connected","object.id.empty()"):
   self.assertIn(token,scope)
 def test_self_kill_and_rebind_reset_both_monitors(self):
  src=read("src/app/application.cpp")
  full=src[src.index("protocol::msg_object_update,"):src.index("protocol::msg_object_update_compressed,")]
  compressed=src[src.index("protocol::msg_object_update_compressed,"):src.index("protocol::msg_object_update_cached,")]
  kill=src[src.index("protocol::msg_kill_object,"):src.index("protocol::msg_coarse_location_update,")]
  for part in (full,compressed,kill):
   self.assertIn("movement_feedback.reset();",part)
   self.assertIn("turn_feedback.reset();",part)
  self.assertIn("self_avatar_local_id=0U;",kill)
 def test_f7_counts_are_separate(self):
  for p,t in [("include/nexverse/world/world_model.hpp","prim_local_id_rebinds"),
              ("include/nexverse/ui/viewer_shell_render.hpp","prim_local_id_rebinds"),
              ("src/ui/viewer_shell_render.cpp","Prim neu:"),
              ("src/app/application.cpp","world_model.prim_local_id_rebinds()")]:
   self.assertIn(t,read(p))
 def test_native_regression_exercises_children_and_disconnected(self):
  s=read("tests/prim_localid_reuse_tests.cpp")
  for token in ("old-grandchild","new-root","apply_object_terse",
                "removed_prim_kills()","region(0x10000000300ULL,false)"):
   self.assertIn(token,s)
if __name__=="__main__":unittest.main()
