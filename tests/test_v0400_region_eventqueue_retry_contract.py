"""v0.4.0.0: Region CAPS EventQueue source-scoped failure recovery."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def text(path):return (ROOT/path).read_text()
class EventQueueRetryGuards(unittest.TestCase):
 def test_worker_results_are_bound_to_original_epoch(self):
  source=text("src/protocol/region_event_queue.cpp")
  hdr=text("include/nexverse/protocol/region_event_queue.hpp")
  self.assertIn("stats.issued_epoch=epoch;",source)
  self.assertIn("result.issued_epoch!=started_epoch_",hdr)
  self.assertIn("RegionEventRestartGate",hdr)
 def test_initial_failure_retries_with_backoff_not_each_frame(self):
  hdr=text("include/nexverse/protocol/region_event_queue.hpp")
  app=text("src/app/application.cpp")
  self.assertIn("5U*(1U<<(failures_-1U))",hdr)
  self.assertIn("std::min(60U,",hdr)
  self.assertIn("region_event_restart.completed(",app)
  self.assertIn("region_event_restart.can_start(",app)
  self.assertIn("region_event_restart.started(",app)
  self.assertNotIn("region_event_started_epoch",app)
 def test_http_404_same_uri_invalidates_ack(self):
  source=text("src/protocol/region_event_queue.cpp")
  self.assertIn("found->second!=uri||batch.expired",source)
  self.assertIn("ack.reset();",source)
  native=text("tests/region_event_queue_tests.cpp")
  self.assertIn("same_uri_after_refresh",native)
  self.assertIn("std::vector<int>({-1,17,-1})",native)
 def test_region_failure_exception_is_retryable_without_stale_worker(self):
  app=text("src/app/application.cpp")
  self.assertIn("failure.issued_epoch=epoch;",app)
  self.assertIn("source-scoped retry scheduled",app)
  native=text("tests/region_event_queue_tests.cpp")
  self.assertIn("gate.completed(unavailable",native)
  self.assertIn("gate.can_start(1002U",native)
 def test_unavailable_seed_once_resumes_with_same_source(self):
  native=text("tests/region_event_queue_tests.cpp")
  self.assertIn("fail_first_seed=true",native)
  self.assertIn("recovered.issued_epoch!=missing_epoch",native)
if __name__=="__main__": unittest.main()
