"""Login EventQueue reconnect contract (October 2026)."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(p):return (ROOT/p).read_text()
class LoginEventQueueRecoveryContract(unittest.TestCase):
 def test_initial_seed_503_does_not_permanently_disable_all_caps(self):
  app=source("src/app/application.cpp")
  p=app[app.index("protocol::CapabilityDiscoveryResult discovery ="):
        app.index("CAPS seed discovery succeeded; endpoints=")]
  self.assertIn("while(!discovery.success&&!caps_stop.load()",p)
  self.assertIn("seed_recovery.on_failure(true)",p)
  self.assertIn("retry.failures>=5U",p)
  self.assertIn("discovery=service.discover(seed_capability);",p)
  self.assertIn("firestorm_region_lease.matches(",p)
  self.assertNotIn("CAPS seed discovery failed; no capability endpoints accepted",p)
 def test_initial_seed_without_eventqueue_uses_retry_loop(self):
  app=source("src/app/application.cpp")
  first=app[app.index("if (event_queue_uri.empty())"):
            app.index("std::optional<std::int32_t> acknowledgement;")]
  self.assertIn('caps_state.status="CAPS RETRY"',first)
  self.assertIn("seed rediscovery pending",first)
  self.assertNotIn("return;",first)
 def test_expired_login_queue_disables_old_address(self):
  app=source("src/app/application.cpp")
  worker=app[app.index("protocol::EventQueueRecentBatches login_recent_batches;"):
             app.index('CAPS discovery started from login seed capability')]
  self.assertIn("if(batch.expired&&!rediscovered_endpoint)",worker)
  self.assertIn("event_queue_uri.clear();",worker)
  self.assertIn("acknowledgement.reset();",worker)
  self.assertIn("login_recent_batches.reset();",worker)
  self.assertIn("caps_state.event_queue_available=false;",worker)
 def test_empty_endpoint_is_never_sent_to_queue_post(self):
  app=source("src/app/application.cpp")
  worker=app[app.index("protocol::EventQueueRecentBatches login_recent_batches;"):
             app.index('CAPS discovery started from login seed capability')]
  self.assertIn("if(event_queue_uri.empty())",worker)
  self.assertIn('service.discover(\n                                                seed_capability,{"EventQueueGet"}',worker)
  self.assertLess(worker.index("if(event_queue_uri.empty())"),
                  worker.index("service.poll_event_queue("))
  self.assertIn("if(recovered_endpoint)continue;",worker)
 def test_reconnect_remains_scoped_to_authenticated_region(self):
  app=source("src/app/application.cpp")
  worker=app[app.index("protocol::EventQueueRecentBatches login_recent_batches;"):
             app.index('CAPS discovery started from login seed capability')]
  self.assertIn("if(!still_origin())break;",worker)
  self.assertIn("caps_stop.load()||!still_origin()",worker)
  self.assertIn("pause_until_retry(retry.pause)",worker)
  self.assertIn("recovery.on_contact();",worker)
 def test_same_uri_new_queue_resets_ack_and_replay(self):
  app=source("src/app/application.cpp")
  self.assertIn("event_queue_uri != found->second||",app)
  self.assertIn("batch.expired)",app)
  self.assertIn("login_recent_batches.reset();",app)
 def test_http_transport_native_mock_covers_recovery(self):
  test=source("tests/caps_tests.cpp")
  for needle in ("unavailable_seed", "initial_no_queue",
                 "recovered_seed", "old_queue.expired",
                 "renewed_seed", "renewed_batch"):
   self.assertIn(needle,test)
 def test_log_does_not_disclose_seed_error_body_or_cap_url(self):
  app=source("src/app/application.cpp")
  self.assertIn('"CAPS EventQueue seed rediscovery failed"',app)
  self.assertNotIn('"CAPS EventQueue seed rediscovery failed: " +',app)
  self.assertIn('"Login CAPS EventQueue seed unavailable "',app)
 def test_healthy_502_and_replay_rules_unchanged(self):
  app=source("src/app/application.cpp")
  self.assertIn("if (batch.idle)",app)
  self.assertIn("!login_recent_batches.first_delivery(",app)
  self.assertIn("acknowledgement = batch.id;",app)
if __name__=="__main__":unittest.main()
