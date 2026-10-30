"""v0.4.0.0 simulator region identity is a handle+generation, not only
IP:port; prevent a same-socket cross-region transition from reusing the
prior circuit active state, packet source, or EventQueue readiness."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(x):return (R/x).read_text()
class SameSocketHandoffIdentityContract(unittest.TestCase):
 def test_handoff_key_captures_region_handle_and_generation(self):
  s=read("include/nexverse/protocol/region_circuit_handoff.hpp")
  self.assertIn("struct RegionCircuitKey",s)
  self.assertIn("std::uint64_t handle=0U",s)
  self.assertIn("std::uint32_t generation=0U",s)
  self.assertIn("active.handle==target.handle",s)
  self.assertIn("active.generation==target.generation",s)
 def test_same_socket_pending_transition_launches_new_circuit(self):
  s=read("include/nexverse/protocol/region_circuit_handoff.hpp")
  self.assertIn("bool old_worker_finished,bool transition_pending",s)
  self.assertIn("if(!transition_pending||region_circuit_key_matches",s)
  t=read("tests/region_circuit_handoff_tests.cpp")
  self.assertIn("region_handoff_action(region_a,region_b,false,true)",t)
  self.assertIn("region_handoff_action(region_a,region_b,true,true)",t)
 def test_same_region_same_socket_new_teleport_generation(self):
  t=read("tests/region_circuit_handoff_tests.cpp")
  self.assertIn("same_region_new_teleport",t)
  self.assertIn("region_handoff_action(region_a,same_region_new_teleport,true,true)",t)
 def test_nonteleport_disconnect_does_not_spin_auto_launches(self):
  t=read("tests/region_circuit_handoff_tests.cpp")
  self.assertIn("region_handoff_action(region_a,same_region_new_teleport,true,false)",t)
  self.assertIn("RegionHandoffAction::unchanged",t)
 def test_worker_captures_epoch_at_launch(self):
  a=read("src/app/application.cpp")
  self.assertIn("target_handle=circuit_target_handle",a)
  self.assertIn("target_generation=circuit_target_generation",a)
  self.assertIn("target_handle,target_generation](",a)
  self.assertIn("region_packet_matches(\n",a)
 def test_world_pending_epoch_only_during_unfinished_handoff(self):
  h=read("include/nexverse/world/world_model.hpp")
  t=read("tests/world_model_tests.cpp")
  self.assertIn("bool region_transition_pending() const noexcept",h)
  self.assertIn("return pending_region_transition_.has_value();",h)
  self.assertIn("if(!world.region_transition_pending())return 118",t)
  self.assertIn("if(world.region_transition_pending())return 119",t)
  self.assertIn("if(world.region_transition_pending())return 120",t)
 def test_handoff_checks_full_key_and_preserves_stop_then_join(self):
  a=read("src/app/application.cpp")
  self.assertIn("old_finished,destination_transition_pending",a)
  self.assertIn("circuit_stop.store(true);",a)
  self.assertIn("(void)circuit_future.get();",a)
  self.assertIn("outbound_queue.clear();",a)
 def test_old_active_state_cannot_pretend_new_region_ready(self):
  a=read("src/app/application.cpp")
  self.assertIn('"New region circuit handshake pending"',a)
  self.assertIn("protocol::CircuitState::opening_socket",a)
  self.assertIn("region_circuit_ready(\n                        {circuit_target_ip",a)
 def test_destination_eventqueue_waits_for_matching_generation(self):
  a=read("src/app/application.cpp")
  self.assertIn("current_region_handle==circuit_target_handle",a)
  self.assertIn("current_region_generation==circuit_target_generation",a)
 def test_manual_retry_rebinds_generation_and_origins(self):
  a=read("src/app/application.cpp")
  self.assertIn("circuit_target_handle=world_model.region().handle;",a)
  self.assertIn("circuit_target_generation=world_model.region().generation;",a)
  t=read("tests/region_circuit_handoff_tests.cpp")
  self.assertIn("region_packet_matches(region_a,region_b,false)",t)
  self.assertIn("region_circuit_ready(region_a,region_b,true,false)",t)
if __name__=="__main__": unittest.main()
