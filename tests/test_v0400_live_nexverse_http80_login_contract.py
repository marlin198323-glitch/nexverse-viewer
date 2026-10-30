"""Regression: public Stadt NexVerse GridInfo on 2026-10-30
advertises http://hg.stadt-nexverse.de/ (port 80), not :8002.
Never regress exact-host pinning or permit arbitrary login ports."""
from pathlib import Path
import unittest

R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class LiveNexverseHttpPort80Contract(unittest.TestCase):
 def test_actual_public_login_destination_supported(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertIn(':(port.empty()||port=="80"||port=="8002")',src)
  self.assertIn("The LIVE Stadt NexVerse /get_grid_info advertises",src)
 def test_https_and_direct_xmlrpc_still_supported(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertIn('port.empty()||port=="443"||port=="8002"',src)
  self.assertIn('port.empty()||port=="80"||port=="8002"',src)
 def test_real_public_xml_fixture_success_and_cache_recovery(self):
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn('<login>http://hg.stadt-nexverse.de/</login>',native)
  self.assertIn('live_port80.info.login_uri()!="http://hg.stadt-nexverse.de/"',native)
  self.assertIn('cached_port80.source!=GridInfoSource::cache',native)
  self.assertIn('cached_port80.info.login_uri()!="http://hg.stadt-nexverse.de/"',native)
 def test_explicit_80_and_casefolded_8002_support(self):
  native=read("tests/grid_discovery_tests.cpp")
  self.assertIn('http://hg.stadt-nexverse.de:80/',native)
  self.assertIn('http://HG.STADT-NEXVERSE.DE:8002/',native)
 def test_foreign_hosts_and_unadvertised_ports_still_blocked(self):
  native=read("tests/grid_discovery_tests.cpp")
  for invalid in ("http://evil.invalid:8002/login",
                  "http://hg.stadt-nexverse.de.evil.invalid:8002/",
                  "http://hg.stadt-nexverse.de:8080/",
                  "http://hg.stadt-nexverse.de:8003/",
                  "https://hg.stadt-nexverse.de:8443/"):
   self.assertIn(invalid,native)
  self.assertIn("poisoned_disk.success",native)
 def test_public_gridinfo_live_and_cache_both_pinned(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertGreaterEqual(src.count("trusted_nexverse_login_uri("),3)
  self.assertIn("host.size()!=trusted.size()",src)
  self.assertIn("is_valid_http_uri(uri)",src)
 def test_plain_http_is_not_misrepresented_as_encrypted(self):
  src=read("src/protocol/grid_discovery.cpp")
  self.assertIn("Host pinning alone does NOT make an HTTP login confidential",src)
  self.assertIn('"GridInfo rejected: login authority is not the NexVerse grid"',src)
 def test_login_credentials_cannot_follow_cross_host_redirect(self):
  src=read("src/net/curl_http_transport.cpp")
  self.assertIn("CURLOPT_FOLLOWLOCATION, 0L",src)
  self.assertIn("CURLOPT_MAXREDIRS, 0L",src)

if __name__=="__main__":unittest.main()
