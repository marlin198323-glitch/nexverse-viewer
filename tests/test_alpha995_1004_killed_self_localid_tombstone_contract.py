"""Alpha.1004: killed own LocalID stays blocked at incoming full/compact self pose ingress."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(n): return (R/n).read_text()
class KilledLocalIDTombstone(unittest.TestCase):
 def test_both_actual_full_compressed_handlers_guard_before_binding_and_pose(self):
  s=rd("src/app/application.cpp")
  for msg in ("protocol::msg_object_update,","protocol::msg_object_update_compressed,"):
   part=s[s.index(msg):][:11000]
   a=part.index("world_model.reject_retired_self_object_pose(")
   self.assertLess(a,part.index("self_avatar_local_id = entry.local_id;"))
   self.assertLess(a,part.index("world_model.accept_simulator_self_pose("))
  self.assertIn("world_model.withdraw_self_object_on_kill(",s)
  self.assertIn("self_avatar_local_id=0U;",s)
 def test_kill_tombstones_are_bounded_and_session_scoped(self):
  m=rd("src/world/world_model.cpp")
  h=rd("include/nexverse/world/world_model.hpp")
  for token in ("killed_self_local_ids_.clear();",
                "killed_self_pose_packets_blocked_=0U;",
                "if(killed_self_local_ids_.size()>=16U)",
                "killed_self_local_ids_.pop_front();",
                "killed_self_local_ids_.push_back(",
                "killed_self_pose_packets_blocked_<UINT32_MAX",
                "old.visible_body==bakes",
                "const bool active_rebind=self_object_bakes_"):
   self.assertIn(token,m)
  self.assertEqual(m.count("killed_self_local_ids_.clear();"),4)
  self.assertIn("std::deque<SelfAvatarObjectBakes> killed_self_local_ids_;",h)
 def test_real_native_kill_regressions_and_gui_diagnostic(self):
  s=rd("tests/self_avatar_kill_binding_tests.cpp")
  for token in ("w.killed_self_local_id_guards()!=1U",
                "w.killed_self_pose_packets_blocked()!=3U",
                "w.reject_retired_self_object_pose(own,region,51U,rebind_te)",
                "w.killed_self_local_id_guards()!=2U",
                "many.killed_self_local_id_guards()!=0U",
                "w.reject_retired_self_object_pose(own,region,51U,te)"):
   self.assertIn(token,s)
  app=rd("src/app/application.cpp")
  self.assertIn("KillObject-IDs gesperrt:",app)
  self.assertIn("Späte KillObject-Posen verworfen:",app)
  self.assertNotIn("std::to_string(killed_self_local_ids_",app)
if __name__=="__main__":unittest.main()
