"""OpenGenesisLINK v0.4: expired Region EventQueue must not strand worker."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()

class RegionCaps404ReconnectContract(unittest.TestCase):
 def test_expired_without_valid_rediscovery_exits_worker(self):
  src=read("src/protocol/region_event_queue.cpp")
  self.assertIn("if(batch.expired&&!renewed)",src)
  self.assertIn("++stats.expired_rediscovery_failures;",src)
  self.assertIn("stats.advertised=false;",src)
  self.assertIn("break;",src[src.index("if(batch.expired&&!renewed)"):][:600])
 def test_valid_new_queue_recovers_even_at_same_uri(self):
  src=read("src/protocol/region_event_queue.cpp")
  self.assertIn("if(found->second!=uri||batch.expired)",src)
  self.assertIn("ack.reset();",src)
  self.assertIn("recent_batches.reset();",src)
  self.assertIn("recovery.on_contact();",src)
 def test_native_http_503_after_expiry_covered(self):
  test=read("tests/region_event_queue_tests.cpp")
  for s in ("fail_second_seed=true", "lost_seed->expire_second=true",
            "failed_refresh.advertised", "failed_refresh.polls!=2U",
            "failed_refresh.expired_rediscovery_failures!=1U",
            "lost_seed->seed_calls!=3"):
   self.assertIn(s,test)
 def test_native_missing_eventqueue_after_404_covered(self):
  test=read("tests/region_event_queue_tests.cpp")
  self.assertIn("missing_second_seed=true",test)
  self.assertIn("missing_refresh.advertised",test)
  self.assertIn("missing_refresh.expired_rediscovery_failures!=1U",test)
 def test_bounded_retry_gate_and_region_epoch_enforced(self):
  header=read("include/nexverse/protocol/region_event_queue.hpp")
  src=read("src/app/application.cpp")
  self.assertIn("retry_after_=now+std::chrono::seconds{seconds}",header)
  self.assertIn("epoch!=login_epoch",header)
  self.assertIn("result.issued_epoch!=started_epoch_",header)
  self.assertIn("region_event_restart.can_start(",src)
  self.assertIn("region_event_restart.started(current_firestorm_epoch)",src)
  self.assertIn("region_event_restart.completed(",src)
 def test_reports_failure_count_not_caps_address(self):
  header=read("include/nexverse/protocol/region_event_queue.hpp")
  src=read("src/app/application.cpp")
  self.assertIn("expired_rediscovery_failures=0U",header)
  self.assertIn('"; expired_rediscovery_failures="+',src)
  self.assertNotIn("core::info(uri)",read("src/protocol/region_event_queue.cpp"))
 def test_login_caps_unchanged_and_healthy_502_still_allowed(self):
  src=read("src/protocol/region_event_queue.cpp")
  app=read("src/app/application.cpp")
  self.assertIn("if(batch.idle)",src)
  self.assertIn("recovery.on_contact();",src)
  self.assertIn("protocol::EventQueueRecentBatches login_recent_batches;",app)
  self.assertIn("if(!login_recent_batches.first_delivery(",app)

if __name__=="__main__":unittest.main()
