"""F9 manual+live COF audit must agree with authenticated WorldModel."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text()
class F9CofConsistency(unittest.TestCase):
 def test_shared_last_nonzero_low158_cof_selection(self):
  header=read("include/nexverse/protocol/avatar_cof_revision.hpp")
  for x in ("packet.appearance_data.rbegin()",
            "entry.cof_version!=0U",
            "if(revision==packet.appearance_data.rend())return std::nullopt;",
            "return revision?revision->cof_version:0U;"):
   self.assertIn(x,header)
  self.assertIn("protocol::latest_nonzero_avatar_cof(appearance)",
                read("src/world/world_model.cpp"))
 def test_manual_and_live_f9_share_same_version(self):
  app=read("src/app/application.cpp")
  self.assertEqual(app.count("protocol::avatar_cof_version_for_display("),2)
  self.assertIn("protocol::avatar_cof_version_for_display(packet);",app)
  self.assertIn("protocol::avatar_cof_version_for_display(*packet),",app)
  self.assertNotIn("packet.appearance_data.back().cof_version",app)
  self.assertNotIn("packet->appearance_data.back().cof_version",app)
 def test_real_wire_and_native_status(self):
  wire=read("tests/avatar_appearance_large_attachment_wire_tests.cpp")
  self.assertIn("protocol::avatar_cof_version_for_display(*replay_v2)!=110U",wire)
  s=read("tests/avatar_bake_evidence_tests.cpp")
  for x in ("appearance.appearance_data={{2U,17U,0U},{2U,0U,0U}};",
            "avatar_cof_version_for_display(appearance)!=17U",
            "avatar_cof_version_for_display(appearance)!=42U",
            "avatar_cof_version_for_display(appearance)!=0U"):
   self.assertIn(x,s)
 def test_release(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.1014")
  self.assertIn("Alpha.1015–1024",read("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
