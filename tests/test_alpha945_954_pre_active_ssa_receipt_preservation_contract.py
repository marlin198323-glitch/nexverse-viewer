"""Pre-active fresh RegionHandshake must not erase a verified SSA-bit2 ACK."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(f):return (R/f).read_text()
class PreActiveSsaAck(unittest.TestCase):
 def test_correct_ssa_receipt_refresh_semantics(self):
  s=rd("include/nexverse/protocol/region_handshake_appearance.hpp")
  self.assertIn("void note_refresh(std::uint32_t outgoing_sequence)",s)
  self.assertIn("if(!acknowledged)\n            note_sent(outgoing_sequence);",s)
  self.assertIn("if(!sent||acknowledged||!pending_tracker_matched||",s)
 def test_real_packet_handler_preserves_ack_even_before_active(self):
  cpp=rd("src/protocol/circuit.cpp")
  a=cpp.index("if (packet.header.message == msg_region_handshake)")
  b=cpp.index("if (packet.header.message == msg_agent_movement_complete)",a)
  body=cpp[a:b]
  for x in ("const bool prior_ack=status.self_appearance_support_acknowledged;",
            "self_appearance_receipt_.note_refresh(reply_sequence);",
            "if(!was_active)",
            "status.state=CircuitState::awaiting_region_ready;",
            "RegionHandshakeReply refreshed before movement;",
            "existing SSA ACK preserved",
            "if(prior_ack&&status.region_handshake_pre_active_ack_preserved<9999U)",
            "region_handshake_replay_.already_replied(",
            "send_reliable("):
   self.assertIn(x,body)
  self.assertNotIn("self_appearance_receipt_.reset();",body)
  self.assertNotIn("status.self_appearance_support_acknowledged=false;",body)
  self.assertNotIn("status.self_appearance_ack_at={};",body)
  self.assertIn("self_appearance_receipt_.reset();",cpp)
 def test_native_pre_active_verified_and_pending_paths(self):
  native=rd("tests/self_appearance_ack_receipt_tests.cpp")
  for x in ("receipt.note_ack(120U,true)",
            "receipt.note_refresh(121U);",
            "receipt.sequence!=120U",
            "receipt.note_refresh(131U);",
            "receipt.note_ack(130U,true)",
            "receipt.note_ack(131U,true)",
            "receipt.reset();"):
   self.assertIn(x,native)
  replay=rd("tests/region_handshake_replay_tests.cpp")
  self.assertIn("receipt.note_refresh(79U)",replay)
  self.assertIn("nexverse-self-appearance-ack-receipt-tests",rd("CMakeLists.txt"))
  self.assertIn("self_appearance_ack_receipt_tests",rd("scripts/pi5-headless-tests.sh"))
 def test_f3_counts_truthfully_without_cof_success(self):
  h=rd("include/nexverse/protocol/circuit.hpp")
  a=rd("src/app/application.cpp")
  self.assertIn("region_handshake_pre_active_ack_preserved=0U;",h)
  self.assertIn("SSA ACK vor Inworld erhalten:",a)
  self.assertIn("circuit_snapshot.region_handshake_pre_active_ack_preserved",a)
  self.assertIn("circuit_snapshot.self_appearance_ack_at",a)
 def test_release(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.1114")
  self.assertIn("Alpha.1115–1124",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
