"""Compile the full viewer and LoginService on Linux x64 on develop-current."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
class LinuxGuiCi(unittest.TestCase):
 def test_beyond_arm64_headless(self):
  y=(R/".github/workflows/linux-viewer-gui-ci.yml").read_text()
  for text in ("develop-current","ubuntu-24.04","libwebkit2gtk-4.1-dev",
            "libgl1-mesa-dev","cmake --preset debug",
            "--target nexverse-viewer nexverse-login-tests nexverse-login-tos-consent-tests",
            "ctest --test-dir build/debug", "OGL_LINUX_X64_VIEWER_COMPILED"):
   self.assertIn(text,y)
 def test_login_xmlrpc_source(self):
  s=(R/"tests/login_tests.cpp").read_text()
  for text in ("credentials.agree_to_tos=true","credentials.agree_to_tos=false",
       "challenge.reason"):
   self.assertIn(text,s)
if __name__=="__main__":unittest.main()
