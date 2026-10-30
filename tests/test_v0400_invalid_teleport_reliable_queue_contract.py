"""v0.4.0.0: malformed teleport never enters the reliable LLUDP
queue or reaches simulator serialization where it would fail the circuit."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(p):return (R/p).read_text()
class TeleportInvalidCommandSafetyContract(unittest.TestCase):
 def test_shared_validator_has_zero_region_and_finite_vectors(self):
  s=src("include/nexverse/protocol/viewer_messages.hpp")
  self.assertIn("valid_teleport_location(",s)
  self.assertIn("location.region_handle!=0U",s)
  self.assertIn("finite(location.position)&&finite(location.look_at)",s)
 def test_outbound_queue_rejects_before_capacity_or_mutation(self):
  s=src("src/protocol/outbound_queue.cpp")
  self.assertIn("std::get_if<TeleportLocation>(&command)",s)
  self.assertIn("!valid_teleport_location(*teleport)",s)
  self.assertLess(s.index("!valid_teleport_location(*teleport)"),
                  s.index("std::scoped_lock lock{mutex_};"))
 def test_direct_teleport_encoder_fails_closed(self):
  s=src("src/protocol/viewer_messages.cpp")
  start=s.index("encode_teleport_location_request(")
  end=s.index("decode_teleport_progress(",start)
  self.assertIn("if(!valid_teleport_location(target))return {};",s[start:end])
  self.assertLess(s[start:end].index("if(!valid_teleport_location"),
                  s[start:end].index("append_le_u64(payload, target.region_handle)"))
 def test_real_code_path_queues_validated_ui_teleport(self):
  s=src("src/app/application.cpp")
  self.assertIn("ui::valid_teleport_target(",s)
  self.assertIn("outbound_queue.push(protocol::TeleportLocation{",s)
 def test_native_invalid_nan_inf_queue_guard(self):
  s=src("tests/outbound_queue_tests.cpp")
  self.assertIn("std::numeric_limits<float>::quiet_NaN()",s)
  self.assertIn("std::numeric_limits<float>::infinity()",s)
  self.assertIn("if(queue.push(invalid)||queue.size()!=1U)",s)
 def test_native_preexisting_reliable_chat_preserved(self):
  s=src("tests/outbound_queue_tests.cpp")
  self.assertIn("if(!queue.push(chat)||queue.size()!=1U)",s)
  self.assertIn("std::holds_alternative<LocalChatMessage>(*preserved)",s)
 def test_native_zero_region_handle_rejected(self):
  s=src("tests/outbound_queue_tests.cpp")
  self.assertIn("no_region.region_handle=0U;",s)
  self.assertIn("if(queue.push(no_region)||queue.size()!=1U)",s)
  t=src("tests/viewer_messages_tests.cpp")
  self.assertIn("region_zero.region_handle=0U;",t)
 def test_legacy_zero_look_at_accepted(self):
  s=src("tests/viewer_messages_tests.cpp")
  self.assertIn("zero_look.look_at={};",s)
  self.assertIn("encode_teleport_location_request(90U,session,zero_look).empty()",s)
 def test_native_encoded_packet_size_unchanged(self):
  s=src("tests/viewer_messages_tests.cpp")
  self.assertIn("decoded_teleport.packet->payload.size() != 64U",s)
  self.assertIn("msg_teleport_location_request",s)
 def test_queue_rejection_does_not_block_normal_commands(self):
  s=src("src/protocol/outbound_queue.cpp")
  self.assertIn("queue_.push_back(std::move(command));",s)
  t=src("tests/outbound_queue_tests.cpp")
  self.assertIn("queue.push(valid_zero_look)",t)
if __name__=="__main__":unittest.main()
