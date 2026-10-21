"""Server #263: canonical HTTPS /service/tos and safe legacy whitelist."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(path):return (R/path).read_text()
class ServerTosCanonical(unittest.TestCase):
 def test_canonical_server_path_and_legacy_strict(self):
  s=read("include/nexverse/protocol/login_tos.hpp")
  self.assertIn('https://stadt-nexverse.de/service/tos',s)
  self.assertIn('https://stadt-nexverse.de/buergerverordnung',s)
  self.assertIn("canonical_nexverse_tos_url(",s)
  self.assertIn("document_url=official;",s)
  self.assertIn("document_url=nexverse_tos_page;",s)
 def test_real_ui_and_browser_open_only_chosen_whitelisted_url(self):
  s=read("src/app/application.cpp")
  self.assertIn("requested_url==nexverse::protocol::nexverse_tos_page",s)
  self.assertIn("requested_url==nexverse::protocol::nexverse_legacy_tos_page",s)
  self.assertIn("if(official.empty())return false;",s)
  self.assertIn("glfwSetClipboardString(window,login.tos.document_url.data());",s)
  self.assertIn("open_official_tos_document(login.tos.document_url)",s)
  self.assertIn("std::string{tos.document_url},theme::cyan",s)
  self.assertNotIn("https://stadt-nexverse.de/buergerverordnung\",\n        nullptr",s)
 def test_native_login_response_and_scope(self):
  s=read("tests/login_tests.cpp")
  self.assertIn("https://stadt-nexverse.de/service/tos",s)
  s=read("tests/login_tos_consent_tests.cpp")
  self.assertIn("protocol::nexverse_legacy_tos_page",s)
  self.assertIn("protocol::nexverse_tos_page",s)
  self.assertIn("http://stadt-nexverse.de/service/tos",s)
if __name__=="__main__":unittest.main()
