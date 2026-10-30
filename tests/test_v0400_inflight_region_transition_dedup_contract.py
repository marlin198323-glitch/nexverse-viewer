"""v0.4.0.0: repeated CAPS handoff events must not erase pending
avatar/scene or increment the simulator lease generation twice."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(name):return (ROOT/name).read_text()
class CapsTransitionPendingDuplicateContract(unittest.TestCase):
 def test_world_stores_unfinished_transition_fingerprint(self):
  h=read("include/nexverse/world/world_model.hpp")
  self.assertIn("std::optional<RegionUpdate> pending_region_transition_",h)
  s=read("src/world/world_model.cpp")
  self.assertIn("pending_region_transition_=update;",s)
 def test_pending_exact_replay_returns_before_scene_teardown(self):
  s=read("src/world/world_model.cpp")
  part=s[s.index("void WorldModel::begin_region_transition("):
         s.index("void WorldModel::apply_agent(")]
  self.assertIn("same_target(*pending_region_transition_,update)",part)
  self.assertLess(part.index("same_target(*pending_region_transition_,update)"),
                  part.index("avatars_.clear();"))
  self.assertLess(part.index("same_target(*pending_region_transition_,update)"),
                  part.index("region_.generation = generation;"))
 def test_comparison_includes_target_position_circuit_and_seed(self):
  s=read("src/world/world_model.cpp")
  part=s[s.index("const auto same_target="):
         s.index("if(pending_region_transition_")]
  for x in ("a.handle==b.handle", "a.simulator_ip==b.simulator_ip",
            "a.simulator_port==b.simulator_port",
            "a.seed_capability==b.seed_capability",
            "a.agent_position.x==b.agent_position.x",
            "a.agent_position.y==b.agent_position.y",
            "a.agent_position.z==b.agent_position.z"):
   self.assertIn(x,part)
 def test_real_circuit_connect_and_reset_release_guard(self):
  s=read("src/world/world_model.cpp")
  self.assertIn("if(leave_region||update.connected)",s)
  self.assertIn("pending_region_transition_.reset();",s)
  self.assertIn("if(replacing_identity)",s)
  reset=s[:s.index("void WorldModel::set_grid(")]
  self.assertIn("pending_region_transition_.reset();",reset)
 def test_recovery_through_different_new_target_still_clears_and_restarts(self):
  native=read("tests/world_model_tests.cpp")
  for x in ("different_position.agent_position.x=128.0f",
            "new_seed.seed_capability=",
            "new_socket.simulator_port=9101U",
            "other_handle.handle+=0x100ULL",
            "generation_after_handshake",
            "live.connected=true",
            "world.begin_region_transition(new_socket);"):
   self.assertIn(x,native)
 def test_duplicate_current_target_does_not_hide_new_session_login(self):
  s=read("src/world/world_model.cpp")
  start=s.index("void WorldModel::apply_agent(")
  end=s.index("bool WorldModel::accept_simulator_self_position",start)
  self.assertIn("pending_region_transition_.reset();",s[start:end])
  native=read("tests/world_model_tests.cpp")
  self.assertIn("world.reset();",native)
  self.assertIn("return 114;",native)
 def test_login_and_region_queues_both_use_same_world_transition(self):
  s=read("src/app/application.cpp")
  self.assertIn('"TeleportFinish"',s)
  self.assertIn('"CrossedRegion"',s)
  self.assertGreaterEqual(s.count("world_model.begin_region_transition({"),2)
  self.assertIn("world_model.agent().session_id",s)
 def test_does_not_affect_existing_ack_and_region_queue_replay_guards(self):
  eq=read("src/protocol/region_event_queue.cpp")
  login=read("src/app/application.cpp")
  self.assertIn("recent_batches.first_delivery(",eq)
  self.assertIn("login_recent_batches.first_delivery(",login)
  self.assertIn("if(!still_origin())break;",login)
if __name__=="__main__":unittest.main()
