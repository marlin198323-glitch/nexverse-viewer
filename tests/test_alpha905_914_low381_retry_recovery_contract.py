"""A reliable Low381 ACK is not an authenticated Low382 readback."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def rd(file):return (R/file).read_text()
class Low381ReadbackRetry(unittest.TestCase):
 def test_bounded_request_cadence(self):
  h=rd("include/nexverse/protocol/wearables_readback_retry.hpp")
  for x in ("seconds{5}", "seconds{12}", "count_<2U",
            "void note_response()", "bool due(", "void note_initial_sent("):
   self.assertIn(x,h)
 def test_real_circuit_retries_not_mutations(self):
  c=rd("src/protocol/circuit.cpp")
  self.assertIn("WearablesReadbackRetry wearables_retry;",c)
  self.assertIn("wearables_retry.due(now)",c)
  self.assertIn("encode_agent_wearables_request(seq,session_)",c)
  self.assertIn("send_reliable(",c)
  self.assertIn("wearables_retry.note_retry_sent();",c)
  self.assertIn("decode_agent_wearables_update(",c)
  self.assertIn("reply->agent_id==session_.agent_id",c)
  self.assertIn("reply->session_id==session_.session_id",c)
  self.assertIn("wearables_retry.note_response();",c)
  self.assertIn("status.wearables_valid_wire_received=true;",c)
  self.assertNotIn("encode_agent_is_now_wearing(",c)
 def test_ui_observability_no_false_skin_claim(self):
  a=rd("src/app/application.cpp")
  h=rd("include/nexverse/protocol/circuit.hpp")
  self.assertIn("wearables_read_retries_sent",h+a)
  self.assertIn("wearables_valid_wire_received",h+a)
  self.assertIn("Low381 Wiederholungen",a)
  self.assertIn("Low382 empfangen (COF ungeprueft)",a)
 def test_real_native_registration(self):
  t=rd("tests/wearables_readback_retry_tests.cpp")
  for x in ("retry.note_initial_sent(base)", "valid.note_response();",
            "retry.due(base+5s)", "retry.due(base+12s)",
            "new_session.note_initial_sent(base+120s)"):
   self.assertIn(x,t)
  self.assertIn("nexverse-wearables-readback-retry-tests",rd("CMakeLists.txt"))
  self.assertIn("wearables_readback_retry_tests local_heading",rd("scripts/pi5-headless-tests.sh"))
 def test_release_handoff(self):
  self.assertEqual(rd("VERSION").strip(),"0.2.0-alpha.954")
  self.assertIn("Alpha.955–964",rd("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
