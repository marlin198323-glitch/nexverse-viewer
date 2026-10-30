"""Regression contracts for OpenGenesisLINK presence login failure diagnostics.
The historical server reason alone is ambiguous, and arbitrary XML-RPC
messages must never be written to viewer diagnostics."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(path):return (R/path).read_text()
class PresenceFailureDiagnosticContract(unittest.TestCase):
 def test_ambiguity_is_explicit_not_already_online_assumed(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn('if(reason=="presence")',code)
  self.assertIn('return {"presence_unknown"',code)
  self.assertIn("Der genaue Grund ist nicht erkennbar",code)
 def test_upstream_already_logged_in_message_classified(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn('login_text_contains(message,"already logged in")',code)
  self.assertIn('"presence_already_online"',code)
  self.assertIn("Gridbetreiber",code)
 def test_upstream_restricted_login_classified(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn('login_text_contains(message,"logins are currently restricted")',code)
  self.assertIn('"presence_restricted"',code)
 def test_upstream_unverified_account_classified(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn('login_text_contains(message,"not yet been verified")',code)
  self.assertIn('"presence_unverified"',code)
 def test_login_failure_result_keeps_message_for_disambiguation(self):
  code=src("src/protocol/login.cpp")
  self.assertIn('const std::string message = field(fields, "message");',code)
  self.assertIn('reason.empty() ? "login_failed" : reason,',code)
 def test_raw_server_message_never_written_to_log(self):
  app=src("src/app/application.cpp")
  self.assertIn("OpenSim login failed, category: ",app)
  self.assertIn("protocol::diagnose_login_failure(result)",app)
  self.assertNotIn('std::string{"OpenSim login failed, reason: "} +',app)
 def test_presence_ui_message_is_fixed_localized_string(self):
  app=src("src/app/application.cpp")
  self.assertIn("login_ui.notice=diagnostic.user_notice.empty()",app)
  self.assertIn("std::string{diagnostic.user_notice}",app)
 def test_tos_scope_and_consent_unmodified(self):
  app=src("src/app/application.cpp")
  self.assertIn('if(result.reason=="tos")',app)
  self.assertIn("login_ui.tos.challenge(",app)
  self.assertIn("login_ui.tos.reset();",app)
 def test_never_change_authentication_or_auto_retry(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn("This does not change credentials, server state or login retry policy.",code)
  self.assertNotIn("LoggedOut(",code)
  self.assertNotIn("LoginAgent(",code)
 def test_hostile_server_token_response_native_test(self):
  t=src("tests/login_tests.cpp")
  self.assertIn("https://attacker.invalid/?token=secret",t)
  self.assertIn('std::string(3000U,\'A\')+"already logged in"',t)
  self.assertIn("presence_unknown",t)
 def test_tos_nonpresence_keeps_existing_flow(self):
  code=src("include/nexverse/protocol/login_failure_diagnostics.hpp")
  self.assertIn('if(reason=="tos")return {"tos",{}};',code)
  self.assertIn("const auto tos_class=",src("tests/login_tests.cpp"))
 def test_server_failure_login_does_not_initialize_circuit(self):
  app=src("src/app/application.cpp")
  self.assertIn('login_ui.status = "ERROR";',app)
  self.assertIn("if (!circuit_started && active_session.has_value())",app)
if __name__=="__main__":unittest.main()
