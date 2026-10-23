"""Alpha.984: bounded multi-outfit stale TE history and F3 visibility contracts."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(name): return (R/name).read_text()
class MultiCofReplayHistory(unittest.TestCase):
 def test_real_worldmodel_bounded_history(self):
  cpp=rd("src/world/world_model.cpp")
  self.assertIn("retired_self_object_bakes_.size()>=8U",cpp)
  self.assertIn("retired_self_object_bakes_.pop_front();",cpp)
  self.assertIn("retired_self_object_bakes_.push_back(*self_object_bakes_);",cpp)
  self.assertIn("std::any_of(retired_self_object_bakes_.begin()",cpp)
  self.assertIn("std::none_of(retired_self_object_bakes_.begin()",cpp)
  self.assertIn("std::erase_if(retired_self_object_bakes_",cpp)
  self.assertIn("retired_self_object_bakes_.clear();",cpp)
  self.assertIn("std::deque<SelfAvatarObjectBakes> retired_self_object_bakes_;",
                rd("include/nexverse/world/world_model.hpp"))
 def test_native_multi_cof_and_bound_and_session_reauth(self):
  native=rd("tests/self_object_bake_fallback_tests.cpp")
  for k in ("chained.self_object_bake_replay_guards()!=2U",
            "chained.self_object_bake_stale_replays()!=2U",
            "bounded.self_object_bake_replay_guards()!=",
            "bounded.self_object_bake_replay_guards()!=7U",
            "bounded.self_object_bake_replay_guards()!=0U"):
   self.assertIn(k,native)
 def test_diagnostic_f3_is_count_only(self):
  app=rd("src/app/application.cpp")
  self.assertIn("world_model.self_object_bake_replay_guards()",app)
  self.assertIn("Alte Outfit-TE gesperrt:",app)
  self.assertNotIn("std::to_string(old.visible_body)",app)
if __name__=="__main__": unittest.main()
