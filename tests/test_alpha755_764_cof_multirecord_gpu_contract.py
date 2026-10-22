"""Low158 Firestorm multi-row COF rollback must preserve real GPU skin."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(name):return (R/name).read_text()
class CofMultiRecordGuard(unittest.TestCase):
 def test_actual_model_not_just_diagnostics(self):
  s=src("src/world/world_model.cpp")
  self.assertIn("appearance.appearance_data.rbegin()",s)
  self.assertIn("entry.cof_version!=0U",s)
  self.assertIn("revision!=appearance.appearance_data.rend()",s)
  self.assertIn("incoming.cof_version<self_cof_version_highwater_",s)
  self.assertLess(s.index("incoming.cof_version<self_cof_version_highwater_"),
                  s.index("self_appearance_ = std::move(appearance);"))
 def test_world_model_reset_and_mixed_legacy(self):
  s=src("tests/self_appearance_texture_integrity_tests.cpp")
  for x in ("padded_stale.appearance_data.push_back({2U,0U,0U});",
            "padded_new.appearance_data.push_back({2U,0U,0U});",
            "unknown.appearance_data={{2U,0U,0U},{2U,0U,0U}};",
            "model.stale_self_cof_appearances()!=5U"):
   self.assertIn(x,s)
 def test_real_original_firestorm_gpu_body_pixels(self):
  s=src("tests/avatar_body_framebuffer_pixels_tests.cpp")
  for x in ("padded_old.appearance_data.push_back({2U,0U,0U});",
            "skin.stale_self_cof_appearances()!=2U",
            "difference(new_skin,after_padded_old)!=0U",
            "delayed.appearance_data.push_back({2U,0U,0U});",
            "difference(new_skin,actually_newer)<100U"):
   self.assertIn(x,s)
 def test_version_and_handoff(self):
  self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.804")
  self.assertIn("Alpha.805–814",src("CHATUEBERGABE.md"))
if __name__=="__main__":unittest.main()
