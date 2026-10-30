"""v0.4.0.0: CAPS EstablishAgentCommunication simulator socket and
seed validation must reject malformed input without claiming a circuit."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(name):return (ROOT/name).read_text()
class CapsEstablishSocketSourceContract(unittest.TestCase):
 def test_shared_simulator_host_parser_rejects_unsafe_delimiters(self):
  c=read("src/protocol/caps_world_events.cpp")
  self.assertIn("bool valid_simulator_host(std::string_view host)",c)
  self.assertIn('host.find_first_of("/?#@',c)
  self.assertIn('nexverse::protocol::is_valid_http_uri(',c)
 def test_numeric_ipv4_octets_rejected_without_truncation(self):
  c=read("src/protocol/caps_world_events.cpp")
  self.assertIn("std::from_chars(",c)
  self.assertIn("octet>255U",c)
  self.assertIn("group<4",c)
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("256.0.2.5:9000",t)
  self.assertIn("192.0.2.999:9000",t)
 def test_endpoint_port_required_and_bounded(self):
  c=read("src/protocol/caps_world_events.cpp")
  self.assertIn("bool valid_simulator_endpoint(std::string_view endpoint)",c)
  self.assertIn("number>=1U&&number<=65535U",c)
  self.assertIn("endpoint.find(':')!=split",c)
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("192.0.2.5:65536",t)
  self.assertIn("192.0.2.5:0",t)
 def test_legacy_ipv4_dns_and_bracketed_ipv6_accepted(self):
  t=read("tests/caps_world_events_tests.cpp")
  for s in ("127.0.0.1:9000","192.0.2.5:65535",
            "simulator.nexverse.example:9000","[2001:db8::1]:9000"):
   self.assertIn(s,t)
 def test_establish_seed_uri_uses_hardened_https_uri_validator(self):
  c=read("src/protocol/caps_world_events.cpp")
  start=c.index("parse_establish_agent_communication_event(")
  end=c.index("parse_teleport_finish_caps_event(",start)
  self.assertIn("valid_simulator_endpoint(parsed.simulator_endpoint)",c[start:end])
  self.assertIn("is_valid_http_uri(parsed.seed_capability)",c[start:end])
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("https://user@caps.example/seed",t)
  self.assertIn("https://caps.example:443/seed?identity=opaque",t)
 def test_teleport_and_enable_event_text_simip_uses_same_guard(self):
  c=read("src/protocol/caps_world_events.cpp")
  a=c.index("bool read_ip(")
  b=c.index("bool read_number(",a)
  self.assertIn("return valid_simulator_host(out);",c[a:b])
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("parse_teleport_finish_caps_event(bad)",t)
  self.assertIn("parse_enable_simulator_event(bad_enable)",t)
 def test_ipv6_binary_caps_payload_unmodified(self):
  c=read("src/protocol/caps_world_events.cpp")
  self.assertIn("value->binary_value.size() == 16U",c)
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("auto binary_ipv6=crossed;",t)
  self.assertIn('"2001:db8:0:0:0:0:0:1"',t)
 def test_ipv6_malformed_group_counts_and_mapped_octets_rejected(self):
  c=read("src/protocol/caps_world_events.cpp")
  t=read("tests/caps_world_events_tests.cpp")
  self.assertIn("bool valid_ipv6_literal(std::string_view address)",c)
  self.assertIn("groups==8U:groups<8U",c)
  self.assertIn("address.find(\"::\",compressed+2U)",c)
  for value in ("[1:2:3]:9000", "[1:::2]:9000",
                "[fffff::1]:9000", "[::ffff:999.1.2.3]:9000"):
   self.assertIn(value,t)
 def test_ipv6_compressed_full_mapped_accepted(self):
  t=read("tests/caps_world_events_tests.cpp")
  for value in ("[::1]:9000", "[1:2:3:4:5:6:7:8]:9000",
                "[::ffff:192.0.2.5]:9000"):
   self.assertIn(value,t)
 def test_bad_simulator_endpoint_cannot_be_claimed_valid(self):
  app=read("src/app/application.cpp")
  self.assertIn('"CAPS EstablishAgentCommunication rejected"',app)
  self.assertIn("no new circuit activated",app)
  self.assertNotIn('"CAPS EstablishAgentCommunication world event validated"',app)
 def test_no_rejected_socket_tokens_agent_details_in_log(self):
  app=read("src/app/application.cpp")
  self.assertIn('core::warning("CAPS EstablishAgentCommunication rejected");',app)
  c=read("src/protocol/caps_world_events.cpp")
  self.assertNotIn('core::warning(parsed.simulator_endpoint)',c)
 def test_world_handoff_session_and_region_gates_unchanged(self):
  app=read("src/app/application.cpp")
  self.assertIn("region_handoff_action(",app)
  self.assertIn("world_model.begin_region_transition({",app)
  self.assertIn("crossed->session_id != world_model.agent().session_id",app)
if __name__=="__main__":unittest.main()
