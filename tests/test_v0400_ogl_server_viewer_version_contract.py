"""OGL v0.4.0.0 login must satisfy OpenGenesisLINK
OglApprovedViewerPolicy.IsNumericVersion by emitting 0.4.0.0 on wire.
The visible release remains v0.4.0.0; no other viewer branding."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(x):return (R/x).read_text()
class ServerApprovedViewerVersionContract(unittest.TestCase):
 def test_display_version_has_v_prefix(self):
  self.assertEqual(read("VERSION").strip(),"v0.4.0.0")
  self.assertIn("NEXVERSE_VERSION_LABEL",read("CMakeLists.txt"))
 def test_login_wire_version_strips_only_cosmetic_v(self):
  code=read("include/nexverse/protocol/login.hpp")
  self.assertIn("ogl_login_wire_version(",code)
  self.assertIn("display_version.substr(1U)",code)
  self.assertIn("display_version[1]>='0'",code)
  self.assertIn("display_version[1]<='9'",code)
 def test_real_serialize_path_uses_normalizer(self):
  code=read("src/protocol/login.cpp")
  self.assertIn("ogl_login_wire_version(version)",code)
  self.assertIn('append_member(document, structure, "version",',code)
  self.assertNotIn('append_member(document, structure, "version", version);',code)
 def test_no_change_to_identity_or_password_shape(self):
  code=read("src/protocol/login.cpp")
  self.assertIn('append_member(document, structure, "channel", channel)',code)
  self.assertIn('"passwd", hash_login_password(credentials.password)',code)
  self.assertIn("login_to_simulator",code)
 def test_client_calls_login_with_true_channel_and_display_version(self):
  code=read("src/app/application.cpp")
  self.assertIn('const std::string viewer_version{core::version()};',code)
  self.assertIn('"OpenGenesisLINK Viewer",\n                                    viewer_version',code)
 def test_native_wire_payload_v0400_to_numeric(self):
  code=read("tests/login_tests.cpp")
  self.assertIn('ogl_login_wire_version("v0.4.0.0")!="0.4.0.0"',code)
  self.assertIn('contains_rpc_member(wire,"version","0.4.0.0")',code)
  self.assertIn('contains_rpc_member(wire,"version","v0.4.0.0")',code)
 def test_real_login_service_transport_serializes_numeric_version(self):
  code=read("tests/login_tests.cpp")
  self.assertIn('const auto actual_service=service.login(',code)
  self.assertIn('"OpenGenesisLINK Viewer","v0.4.0.0"',code)
  self.assertIn('transport->posted_body.find(',code)
 def test_wires_old_version_and_alpha_unchanged(self):
  code=read("tests/login_tests.cpp")
  self.assertIn('ogl_login_wire_version("0.4.0.0")!="0.4.0.0"',code)
  self.assertIn('"0.1.0-alpha.1"',code)
 def test_non_numeric_viewer_names_not_masqueraded(self):
  code=read("tests/login_tests.cpp")
  self.assertIn('ogl_login_wire_version("Firestorm")!="Firestorm"',code)
 def test_public_admission_identity_trace_without_private_data(self):
  code=read("src/app/application.cpp")
  self.assertIn("OpenSim login client identity: channel=OpenGenesisLINK Viewer;",code)
  self.assertIn("wire_version=",code)
  self.assertIn("protocol::ogl_login_wire_version(viewer_version)",code)
 def test_previous_login_presence_categories_still_used(self):
  code=read("src/app/application.cpp")
  self.assertIn("protocol::diagnose_login_failure(result)",code)
  self.assertIn('"OpenSim login failed, category: "',code)
 def test_no_credential_post_added_by_normalizer(self):
  code=read("include/nexverse/protocol/login.hpp")
  segment=code[code.index("ogl_login_wire_version("):code.index("struct LoginCredentials")]
  self.assertNotIn("login_uri",segment)
  self.assertNotIn("curl",segment)
if __name__=="__main__":unittest.main()
