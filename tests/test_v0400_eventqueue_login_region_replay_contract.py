"""v0.4.0.0 CAPS: login/region EventQueue replay and ACK contracts."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def content(path):return (ROOT/path).read_text()
class LoginRegionAckGuard(unittest.TestCase):
 def test_login_rejects_idless_200_and_nonadjacent_replay(self):
  app=content("src/app/application.cpp")
  zone=app[app.index("protocol::EventQueueRecentBatches login_recent_batches;"):
           app.index('CAPS discovery started from login seed capability')]
  self.assertIn("if(!batch.id)",zone)
  self.assertIn("!login_recent_batches.first_delivery(",zone)
  self.assertIn("acknowledgement = batch.id;",zone)
  self.assertLess(zone.index("if(!batch.id)"),
                  zone.index("acknowledgement = batch.id;"))
  self.assertLess(zone.index("!login_recent_batches.first_delivery("),
                  zone.index("acknowledgement = batch.id;"))
  self.assertIn("pause_until_retry(",zone)
 def test_both_login_and_region_reset_on_expiry_even_with_same_uri(self):
  app=content("src/app/application.cpp")
  region=content("src/protocol/region_event_queue.cpp")
  self.assertIn("event_queue_uri != found->second||",app)
  self.assertIn("batch.expired)",app)
  self.assertIn("login_recent_batches.reset();",app)
  self.assertIn("found->second!=uri||batch.expired",region)
  self.assertIn("recent_batches.reset();",region)
 def test_duplicate_does_not_rewind_ack(self):
  hdr=content("include/nexverse/protocol/region_event_queue.hpp")
  source=content("src/protocol/region_event_queue.cpp")
  self.assertIn("capacity=64U;",hdr)
  self.assertIn("if(!recent_batches.first_delivery(*batch.id))",source)
  self.assertIn("++stats.replayed_batches;",source)
  self.assertIn("ack=batch.id;",source)
  self.assertLess(source.index("first_delivery(*batch.id)"),
                  source.index("ack=batch.id;"))
 def test_mock_covers_nonadjacent_replay_and_queue_incarnation(self):
  test=content("tests/region_event_queue_tests.cpp")
  self.assertIn("scripted_ids={17,18,17,19}",test)
  self.assertIn("std::vector<int>({-1,17,18,18})",test)
  self.assertIn("replay_after_404->scripted_ids={17,17,17}",test)
  self.assertIn("after_rebind.accepted_events!=2U",test)
 def test_diagnostics_never_log_uri_or_payload(self):
  app=content("src/app/application.cpp")
  source=content("src/protocol/region_event_queue.cpp")
  self.assertIn('"; duplicate_batches="+',app)
  self.assertIn('"; idless_batches="+',app)
  self.assertIn('"; idle_polls="+',app)
  self.assertIn('"; failed_polls="+',app)
  self.assertIn('"; rediscoveries="+',app)
  self.assertNotIn("core::info(uri)",source)
if __name__=="__main__":unittest.main()
