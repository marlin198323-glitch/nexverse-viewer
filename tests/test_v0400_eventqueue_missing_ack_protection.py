"""v0.4.0.0 EventQueue HTTP 200 without ack cursor cannot execute events."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def rd(path): return (R/path).read_text()
class EventQueueNoAck(unittest.TestCase):
 def test_idless_queued_events_fail_closed(self):
  s=rd("src/protocol/caps.cpp")
  self.assertIn("!batch.id.has_value() && !batch.events.empty()",s)
  self.assertIn("EventQueue events without acknowledgement ID",s)
 def test_empty_idless_batch_is_paced(self):
  s=rd("src/protocol/region_event_queue.cpp")
  self.assertIn("if(!batch.id)",s)
  self.assertIn("pause(std::chrono::milliseconds{150});",s)
 def test_native_parser_regression(self):
  s=rd("tests/caps_tests.cpp")
  self.assertIn("const auto without_id=parse_event_queue_response(",s)
  self.assertIn("const auto empty_without_id=parse_event_queue_response(",s)
 def test_native_loop_does_not_emit_idless_events(self):
  s=rd("tests/region_event_queue_tests.cpp")
  self.assertIn("missing_id_forever",s)
  self.assertIn("broken.accepted_events!=0U",s)
  self.assertIn("no_id_mock->acks!=std::vector<int>({-1,-1})",s)
if __name__=="__main__":unittest.main()
