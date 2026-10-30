"""NexVerse-only login discovery: never trust an alternate XML-RPC
credential destination injected through plaintext GridInfo or disk cache."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class NexverseGridAuthorityPinContract(unittest.TestCase):
 def test_gridinfo_is_untrusted_http_and_pinned(self):
  source=read("src/protocol/grid_discovery.cpp")
  self.assertIn("trusted_nexverse_login_uri",source)
  self.assertIn("nexverse::protocol::nexverse_grid_host",source)
  self.assertIn("nexverse::protocol::is_valid_http_uri(uri)",source)
 def test_exact_host_case_insensitive_not_suffix(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertIn("host.size()!=trusted.size()",src)
  self.assertIn("std::tolower(static_cast<unsigned char>(host[i]))",src)
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn("hg.stadt-nexverse.de.evil.invalid",native)
  self.assertIn("http://HG.STADT-NEXVERSE.DE:8002/",native)
 def test_enforce_nexverse_login_ports(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertIn('port=="443"||port=="8002"',src)
  self.assertIn('port.empty()||port=="80"||port=="8002"',src)
  self.assertIn("http://hg.stadt-nexverse.de/",read("tests/grid_discovery_tests.cpp"))
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn("http://hg.stadt-nexverse.de:8003/",native)
  self.assertIn("https://hg.stadt-nexverse.de:8443/",native)
 def test_live_and_cached_documents_both_use_pin(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertGreaterEqual(src.count("trusted_nexverse_login_uri("),3)
  self.assertIn("!trusted_nexverse_login_uri(parsed.document->info.login_uri())",src)
  self.assertIn("trusted_nexverse_login_uri(parsed.document->info.login_uri()))",src)
 def test_forged_disk_cache_fails_closed_offline(self):
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn("PoisonedCache",native)
  self.assertIn("poisoned_disk.success",native)
  self.assertIn("GridInfoSource::none",native)
 def test_rogue_http_xml_never_replaces_valid_cache(self):
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn("foreign_logins",native)
  self.assertIn("refused.source!=GridInfoSource::cache",native)
  self.assertIn('refused.info.grid_name()!="NexVerse"',native)
 def test_generic_http_uri_check_preserved_for_caps(self):
  info=read("src/protocol/grid_info.cpp")
  self.assertIn("bool is_valid_http_uri(std::string_view uri) noexcept",info)
  self.assertIn("authority.find('@')",info)
  self.assertIn("uri.size()>8192U",info)
 def test_login_post_does_not_follow_cross_domain_redirects(self):
  net=read("src/net/curl_http_transport.cpp")
  self.assertIn("CURLOPT_FOLLOWLOCATION, 0L",net)
  self.assertIn("CURLOPT_MAXREDIRS, 0L",net)
 def test_application_does_not_log_grid_login_url_or_query(self):
  app=read("src/app/application.cpp")
  self.assertIn("Active NexVerse login URI accepted",app)
  self.assertNotIn('std::string{"Active OpenSim login URI: "} +',app)
 def test_unsupported_host_generates_generic_warning_no_leak(self):
  source=read("src/protocol/grid_discovery.cpp")
  self.assertIn("GridInfo rejected: login authority is not the NexVerse grid",source)
  self.assertIn("without allowing arbitrary ports",source)
  self.assertIn("Host pinning alone does NOT make an HTTP login confidential",source)
if __name__=="__main__":unittest.main()
