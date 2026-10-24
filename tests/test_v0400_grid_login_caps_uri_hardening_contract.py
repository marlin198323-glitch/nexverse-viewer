"""v0.4.0.0 initial Grid/Login/CAPS trust boundary regression contracts."""
import json, unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def src(path):return (ROOT/path).read_text()
class GridLoginTransportHardening(unittest.TestCase):
 def test_current_milestone_active_only_not_claimed_complete(self):
  data=json.loads(src("docs/RELEASE_ROADMAP.json"))
  m=next(x for x in data["milestones"] if x["version"]=="v0.4.0.0")
  self.assertEqual(data["current_version"],"v0.4.0.0")
  self.assertEqual(m["status"],"IN_ARBEIT")
  self.assertEqual(m["tasks"][0]["status"],"IN_ARBEIT")
  self.assertEqual([a["status"] for a in m["acceptance"]],["OFFEN"]*3)
 def test_existing_legacy_grid_info_endpoint_still_supported(self):
  info=src("include/nexverse/protocol/grid_discovery.hpp")
  self.assertIn("http://hg.stadt-nexverse.de/get_grid_info",info)
  t=src("tests/grid_info_tests.cpp")
  self.assertIn("http://hg.stadt-nexverse.de:8002/",t)
  self.assertIn("https://[2001:db8::1]/caps",t)
 def test_grid_uri_rejects_userinfo_host_spoof(self):
  s=src("src/protocol/grid_info.cpp")
  self.assertIn("authority.find('@')!=std::string_view::npos",s)
  t=src("tests/grid_info_tests.cpp")
  self.assertIn("hg.stadt-nexverse.de@evil.example",t)
  self.assertIn("user:pass@hg.stadt-nexverse.de",t)
 def test_bad_explicit_tcp_ports_rejected_with_bounds(self):
  s=src("src/protocol/grid_info.cpp")
  self.assertIn("value==0U||value>65535U",s)
  self.assertIn("authority.back()==':'",s)
  self.assertIn("port.size()>5U",s)
  t=src("tests/grid_info_tests.cpp")
  self.assertIn("example.org:65536",t)
  self.assertIn("example.org:443x",t)
 def test_bad_url_control_and_backslash_host_delimiter_guarded(self):
  s=src("src/protocol/grid_info.cpp")
  self.assertIn("ch<=0x20U||ch==0x7fU",s)
  self.assertIn("ch=='#'",s)
  self.assertIn("uri.size()>8192U",s)
 def test_login_service_checks_before_any_posted_secret(self):
  s=src("src/protocol/login.cpp")
  a=s.index("if (!is_valid_http_uri(login_uri))")
  b=s.index("const net::HttpResponse response = transport_->post(",a)
  self.assertLess(a,b)
  self.assertIn('"invalid_login_uri"',s[a:b])
  self.assertIn("transport->posted_body!=accepted_body",src("tests/login_tests.cpp"))
 def test_poisoned_grid_info_never_overwrites_last_good_cached_login(self):
  s=src("src/protocol/grid_discovery.cpp")
  self.assertIn("if (parsed.ok())",s)
  self.assertIn("GridInfoSource::cache",s)
  t=src("tests/grid_discovery_tests.cpp")
  self.assertIn("poisoned.source!=GridInfoSource::cache",t)
  self.assertIn('poisoned.info.grid_name()!="NexVerse"',t)
 def test_seed_caps_spoofed_urls_not_dispatched(self):
  s=src("src/protocol/caps.cpp")
  self.assertIn("if (!is_valid_http_uri(seed_capability))",s)
  self.assertIn("if (!is_valid_http_uri(uri))",s)
  t=src("tests/caps_tests.cpp")
  self.assertIn("malicious.success||transport->post_calls!=posts_before",t)
  self.assertIn("safe_seed.capabilities.size()!=1U",t)
 def test_libcurl_refuses_password_and_caps_bearer_redirects(self):
  s=src("src/net/curl_http_transport.cpp")
  self.assertIn("CURLOPT_FOLLOWLOCATION, 0L",s)
  self.assertIn("CURLOPT_MAXREDIRS, 0L",s)
  self.assertNotIn("CURLOPT_FOLLOWLOCATION,\n                     (method==",s)
 def test_real_loopback_http_307_native_for_get_and_post(self):
  s=src("tests/http_credential_redirect_tests.cpp")
  for x in ("307 Temporary Redirect","/stolen",
      "transport.post(", "transport.get(",
      "server.requests!=1", "server.reached_redirect",
      "OGL_LIBCURL_AUTH_POST_AND_TOKEN_GET_REDIRECT_NOT_FOLLOWED_PASS"):
   self.assertIn(x,s)
  cmake=src("CMakeLists.txt")
  self.assertIn("nexverse-http-credential-redirect-tests",cmake)
if __name__=="__main__":unittest.main()
