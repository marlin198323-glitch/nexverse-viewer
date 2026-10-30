"""v0.4.0.0 malformed incoming CAPS Region/CrossedRegion should
never poison avatar/camera pose, UDP simulator port or world dimensions."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def get(x):return (R/x).read_text()
class CapsWorldFiniteAndBounds(unittest.TestCase):
 def test_reject_nan_and_infinite_before_float_cast(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("std::isfinite(number)",x)
  self.assertIn("std::numeric_limits<float>::max()",x)
  self.assertIn("std::isfinite(out)",x)
 def test_read_vec3_checks_both_position_and_lookat(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn('read_vec3(\n            info->find("Position"),',x)
  self.assertIn('read_vec3(\n            info->find("LookAt"),',x)
  self.assertIn("read_number(value->array_value[2U], out.z)",x)
 def test_native_nan_inf_extreme_position_and_lookat(self):
  x=get("tests/caps_world_events_tests.cpp")
  for y in ("quiet_NaN()", "infinity()", "-inf", "1.0e100",
            "position_of(bad_pose)", "look_at_of(bad_look)",
            "parse_crossed_region_caps_event(bad_look)"):
   self.assertIn(y,x)
 def test_integer_overflow_guard_all_u32_fields(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("std::numeric_limits<std::uint32_t>::max()",x)
  self.assertIn("value->integer_value <",x)
 def test_int64_regionhandle_preserved_not_truncated(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("out = static_cast<std::uint64_t>(value->integer_value)",x)
  self.assertNotIn("out = static_cast<std::uint32_t>(value->integer_value);\n        return true;\n    }\n\n    return false;\n}\n\nbool read_ip(",x)
  test=get("tests/caps_world_events_tests.cpp")
  self.assertIn("0x11223344556677ULL",test)
 def test_port_rejects_zero_and_overflow(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("parsed == 0U",x)
  self.assertIn("parsed > 65535U",x)
  test=get("tests/caps_world_events_tests.cpp")
  self.assertIn("4294967297LL",test)
 def test_region_dimension_limits_and_varregion(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("out.region_size_x>16384U",x)
  self.assertIn("out.region_size_y>16384U",x)
  t=get("tests/caps_world_events_tests.cpp")
  self.assertIn("16385LL",t)
  self.assertIn("LlsdValue::integer(8192)",t)
  self.assertIn("LlsdValue::integer(4096)",t)
 def test_seed_capability_must_use_existing_hardened_uri_parser(self):
  x=get("src/protocol/caps_world_events.cpp")
  self.assertIn("nexverse::protocol::is_valid_http_uri(out.seed_capability)",x)
  t=get("tests/caps_world_events_tests.cpp")
  self.assertIn("ftp://caps.example/seed",t)
  self.assertIn("https://user@caps.example/seed",t)
 def test_real_teleport_and_crossedregion_handler_dispatch_remains(self):
  x=get("src/app/application.cpp")
  self.assertIn('caps_event_dispatcher.register_handler(\n        "TeleportFinish",',x)
  self.assertIn('caps_event_dispatcher.register_handler(\n        "CrossedRegion",',x)
  self.assertIn("protocol::parse_crossed_region_caps_event(event)",x)
 def test_circuit_handoff_and_replay_protection_stays(self):
  app=get("src/app/application.cpp")
  world=get("src/world/world_model.cpp")
  self.assertIn("region_handoff_action(",app)
  self.assertIn("pending_region_transition_",world)
  self.assertIn("same_target(*pending_region_transition_,update)",world)
if __name__=="__main__":unittest.main()
