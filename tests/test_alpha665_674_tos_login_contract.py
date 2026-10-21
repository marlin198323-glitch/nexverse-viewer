"""Verify actual viewer and OpenSim/Firestorm ToS consent wiring."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path):return (ROOT/path).read_text()
class TosContract(unittest.TestCase):
 def test_no_automatic_agree_in_real_xml_rpc(self):
  s=read("src/protocol/login.cpp")
  self.assertIn("credentials.agree_to_tos",s)
  self.assertNotIn('append_boolean(document, structure, "agree_to_tos", true)',s)
  self.assertIn("false;",read("include/nexverse/protocol/login.hpp"))
 def test_real_login_server_challenge_and_retry(self):
  a=read("src/app/application.cpp")
  for key in ('result.reason=="tos"', 'login_ui.tos.challenge(',
              'credentials.agree_to_tos=login_ui.tos.approved_for(',
              'active_login_uri', 'login_ui.tos.reset();'):
   self.assertIn(key,a)
  self.assertIn('state.tos.visible()',a)
 def test_user_gesture_and_modal(self):
  a=read("src/app/application.cpp")
  for key in ("render_login_tos_dialog(","open_official_tos_document(login.tos.document_url)",
              "TosAction::open_document","TosAction::accept",
              "TosAction::decline","login.tos.document_opened()",
              "login.tos.agree()","glfwSetClipboardString"):
   self.assertIn(key,a)
  self.assertIn("render_login_tos_dialog(renderer,window_width,window_height,",a)
  self.assertIn("#include <shellapi.h>",a)
  self.assertIn("posix_spawnp(&child,executable,nullptr,nullptr,args,environ)!=0",a)
  self.assertIn("waitpid(child,&status,0)",a)
  self.assertIn("target_link_libraries(nexverse-viewer PRIVATE shell32)",
                read("CMakeLists.txt"))
 def test_only_verified_official_url(self):
  s=read("include/nexverse/protocol/login_tos.hpp")
  self.assertIn("https://stadt-nexverse.de/buergerverordnung",s)
  self.assertIn("https://stadt-nexverse.de/service/tos",s)
  self.assertIn('reason!="tos"',s)
  self.assertIn("login_uri=grid",s)
  self.assertIn("avatar_name=avatar",s)
  self.assertIn("phase==TosPhase::agreed&&avatar==avatar_name&&grid==login_uri",s)
 def test_native_regression_and_cmake(self):
  self.assertIn("login_tos_consent_tests",read("CMakeLists.txt"))
  self.assertIn("login_tos_consent_tests",read("scripts/pi5-headless-tests.sh"))
  self.assertIn("challenge.reason",read("tests/login_tests.cpp"))
if __name__=="__main__":unittest.main()
