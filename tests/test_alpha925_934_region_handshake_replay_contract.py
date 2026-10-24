"""LLUDP retransmitted RegionHandshake must not regress an active circuit or proven SSA ACK."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(path):return (R/path).read_text()
class RegionHandshakeReplayContract(unittest.TestCase):
 def test_bounded_out_of_order_replay_and_session_clear(self):
  h=rd("include/nexverse/protocol/region_handshake_replay.hpp")
  self.assertIn("static constexpr std::size_t capacity=16U",h)
  self.assertIn("already_replied(",h)
  self.assertIn("void note_replied(",h)
  self.assertIn("void reset() noexcept",h)
  self.assertIn("std::array<std::uint32_t,capacity>",h)
 def test_real_lludp_still_acks_before_replay_check(self):
  s=rd("src/protocol/circuit.cpp")
  a=s.index("if ((packet.header.flags & packet_reliable) != 0U)")
  b=s.index("if (packet.header.message == msg_region_handshake)",a)
  c=s.index("if (packet.header.message == msg_agent_movement_complete)",b)
  head=s[a:b]
  handler=s[b:c]
  self.assertIn("encode_packet_ack(next_sequence++, ack_ids)",head)
  self.assertIn("region_handshake_replay_.already_replied(packet.header.sequence)",handler)
  self.assertIn("region_handshake_replay_.note_replied(packet.header.sequence)",handler)
  self.assertIn("const bool was_active=status.state==CircuitState::active",handler)
  self.assertIn("const bool prior_ack=status.self_appearance_support_acknowledged",handler)
  self.assertIn("if(!was_active)",handler)
  self.assertIn("self_appearance_receipt_.note_refresh(reply_sequence)",handler)
  self.assertIn("if(prior_ack&&status.region_handshake_pre_active_ack_preserved<9999U)",handler)
  self.assertIn("status.state=CircuitState::awaiting_region_ready",handler)
  self.assertLess(handler.index("if(!was_active)"),handler.index("status.state=CircuitState::awaiting_region_ready"))
  self.assertNotIn("status.state=CircuitState::active",handler)
 def test_existing_ssa_ack_and_real_f3_evidence_remain(self):
  s=rd("src/protocol/circuit.cpp")
  h=rd("include/nexverse/protocol/circuit.hpp")
  app=rd("src/app/application.cpp")
  self.assertIn("self_appearance_receipt_.note_ack(ack,tracker_matched)",s)
  self.assertIn("region_handshake_replay_.reset();",s)
  self.assertIn("region_handshake_duplicates",h+app)
  self.assertIn("region_handshake_refreshes",h+app)
  self.assertIn("RegionHandshake Duplikate:",app)
  self.assertIn("RegionHandshake Aktualisierung:",app)
  self.assertIn("self_appearance_support_acknowledged",app)
 def test_native_and_pi5_integration(self):
  s=rd("tests/region_handshake_replay_tests.cpp")
  for k in ("SelfAppearanceHandshakeReceipt receipt", "receipt.note_ack(77U,true)",
            "replay.already_replied(9U)", "replay.note_replied(116U)",
            "replay.reset();", "0xFFFFFFFFU"):
   self.assertIn(k,s)
  self.assertIn("nexverse-region-handshake-replay-tests",rd("CMakeLists.txt"))
  self.assertIn("region_handshake_replay_tests wearables",rd("scripts/pi5-headless-tests.sh"))
 def test_version_and_next_batch(self):
  self.assertEqual(rd("VERSION").strip(),"v0.3.0.0")
  self.assertIn("v0.4.0.0",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
