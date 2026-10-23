"""Alpha.994: retired avatar ObjectUpdate must be rejected before motion lease."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(name): return (R/name).read_text()
class OldBodyPoseLeaseGuard(unittest.TestCase):
 def test_real_full_and_compressed_self_ingress_order(self):
  app=read("src/app/application.cpp")
  for msg in ("protocol::msg_object_update,",
              "protocol::msg_object_update_compressed,"):
   start=app.index(msg)
   part=app[start:start+11000]
   guard=part.index("world_model.reject_retired_self_object_pose(")
   binding=part.index("if (self_avatar_local_id != entry.local_id")
   pose=part.index("world_model.accept_simulator_self_pose(")
   self.assertLess(guard,binding)
   self.assertLess(guard,pose)
   self.assertIn("entry.local_id,entry.texture_entry))continue;",part)
 def test_model_is_authenticated_and_bounded(self):
  src=read("src/world/world_model.cpp")
  header=read("include/nexverse/world/world_model.hpp")
  self.assertIn("bool WorldModel::reject_retired_self_object_pose(",src)
  for k in ("region_.handle!=region_handle",
            "agent_.session_id.empty()",
            "avatar_id!=agent_.agent_id||local_id==0U",
            "retired_self_object_bakes_.begin()",
            "if(!exact_old&&(fresh_body||current_candidate))return false;",
            "if(retired_self_pose_packets_blocked_<UINT32_MAX)",
            "++retired_self_pose_packets_blocked_"):
   self.assertIn(k,src)
  self.assertIn("std::uint32_t retired_self_pose_packets_blocked_=0U;",header)
  self.assertEqual(src.count("retired_self_pose_packets_blocked_=0U;"),4)
 def test_native_retired_exact_missing_and_new_body(self):
  t=read("tests/self_object_bake_fallback_tests.cpp")
  for token in ("ingress.retired_self_pose_packets_blocked()!=4U",
                "ingress.self_object_bake_stale_replays()!=1U",
                "ingress.retired_self_pose_packets_blocked()!=5U",
                "ingress.self_object_bakes()->local_id!=81U",
                "ingress.retired_self_pose_packets_blocked()!=0U"):
   self.assertIn(token,t)
  self.assertIn("nexverse-self-object-bake-fallback-tests",read("CMakeLists.txt"))
 def test_gui_diagnostic_no_session_or_te_logging(self):
  app=read("src/app/application.cpp")
  self.assertIn("world_model.retired_self_pose_packets_blocked()",app)
  self.assertIn("Alte Avatar-Pose vor WASD verworfen:",app)
  self.assertNotIn("std::to_string(retired_self_object_bakes_",app)
if __name__=="__main__": unittest.main()
