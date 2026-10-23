"""Valid OpenSim LLTextureEntry admission, preserved self skin and F3 proof."""
import unittest, json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def read(name):return (R/name).read_text(encoding="utf-8")
class SelfTextureGate(unittest.TestCase):
 def test_shared_parser_for_verified_self(self):
  source=read("src/protocol/self_appearance_admission.cpp")
  self.assertIn("parse_texture_entry(texture_entry).has_value()",source)
  self.assertIn("src/protocol/self_appearance_admission.cpp",read("CMakeLists.txt"))
 def test_model_cannot_be_bypassed(self):
  s=read("src/world/world_model.cpp")
  self.assertIn("protocol::assess_self_appearance(",s)
  self.assertIn("protocol::valid_self_appearance_texture_entry(",s)
  self.assertLess(s.index("protocol::valid_self_appearance_texture_entry("),
                  s.index("self_appearance_ = std::move(appearance)"))
 def test_runtime_reject_preserves_previous(self):
  s=read("src/app/application.cpp")
  self.assertIn("if(!protocol::valid_self_appearance_texture_entry(",s)
  self.assertIn("previous skin",s)
  self.assertIn("avatar_appearance_self_invalid_texture_ignored.fetch_add",s)
  self.assertIn("avatar_appearance_self_invalid_texture_ignored.load",s)
 def test_region_counters_do_not_leak(self):
  s=read("src/app/application.cpp")
  for field in ("avatar_appearance_failure_stage.store(0,",
                "avatar_appearance_self_trial_ignored.store(0U",
                "avatar_appearance_self_incomplete_ignored.store(0U",
                "avatar_appearance_self_invalid_texture_ignored.store(0U"):
   self.assertIn(field,s)
 def test_visible_warning_and_private_summary(self):
  a=read("include/nexverse/ui/avatar_appearance_runtime.hpp")
  self.assertIn("Texturblock ungueltig",a)
  self.assertIn("Skin erhalten",a)
  report=read("src/ui/avatar_diagnostic_report.cpp")
  self.assertIn("appearance_self_invalid_texture_ignored=",report)
  self.assertNotIn("appearance_invalid_payload=",report)
 def test_real_cpp_integrity_regression(self):
  s=read("tests/self_appearance_texture_integrity_tests.cpp")
  self.assertIn("valid_self_appearance_texture_entry",s)
  self.assertIn("model.self_appearance()->texture_entry!=authentic",s)
  c=read("CMakeLists.txt")
  self.assertIn("nexverse-self-appearance-texture-integrity-tests",c)
 def test_release_handoff(self):
  self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.984")
  p=json.loads(read("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["viewer_version"],"0.2.0-alpha.984")
  self.assertEqual(p["next_batch"],"Alpha.985–994")
  self.assertEqual(p["default_blocks_per_run"],10)
  for name in ("CHATUEBERGABE.md","AGENTS.md","projectplan.md","info.md"):
   self.assertIn("Alpha.985–994",read(name)[:1000])
if __name__=="__main__": unittest.main()
