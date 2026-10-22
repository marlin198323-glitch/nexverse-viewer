"""Live upstream #268 single-name account login must work in actual
GLFW UI, serializer and transport, not just a mocked parser."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(p): return (R/p).read_text()
class SingleNameLoginContract(unittest.TestCase):
 def test_gui_routes_single_name_to_username_only(self):
  app=read("src/app/application.cpp")
  self.assertIn("parse_avatar_login_identity(name)",app)
  self.assertIn("if(avatar_name->account_username)",app)
  self.assertIn("credentials.account_username=avatar_name->first;",app)
  self.assertIn("credentials.last_name=avatar_name->last;",app)
  self.assertIn("credentials.agree_to_tos=login_ui.tos.approved_for(",app)
 def test_rpc_and_transport(self):
  login=read("src/protocol/login.cpp")
  self.assertIn('append_member(document, structure, "username",',login)
  self.assertIn('append_member(document, structure, "first", credentials.first_name)',login)
  self.assertIn('append_member(document, structure, "last", credentials.last_name)',login)
  self.assertIn("const bool account_identity=",login)
  self.assertIn("!credentials.account_username.empty()",login)
  self.assertIn("(account_identity&&legacy_identity)",login)
  source=read("tests/login_tests.cpp")
  for k in ("parse_avatar_login_identity", "<name>username</name>",
            "account_tos", "accepted_tos", "account_challenge",
            "LoginService service{transport}"):
   self.assertIn(k,source)
 def test_strict_identity_parse(self):
  header=read("include/nexverse/protocol/avatar_login_identity.hpp")
  self.assertIn("if(separator==std::string_view::npos)",header)
  self.assertIn("if(start_last==std::string_view::npos)",header)
  self.assertIn("find_first_of",header)
 def test_version_and_handoff(self):
  self.assertIn("Alpha.845–854",read("CHATUEBERGABE.md"))
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.844")
if __name__=="__main__":unittest.main()
