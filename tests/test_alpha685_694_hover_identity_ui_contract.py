"""Real GL-proxy hover paths reject nonfinite simulator appearance data."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def source(path):return (R/path).read_text()
class HoverSafetyContract(unittest.TestCase):
 def test_self_remote_real_proxy(self):
  s=source("src/app/application.cpp")
  self.assertEqual(s.count("render::safe_avatar_hover("),2)
  self.assertNotIn("std::clamp(avatar.hover_heights.front().z",s)
  self.assertNotIn("appearance->hover_heights.front().z,-2.0f,2.0f",s)
  self.assertIn("render_agent.position.z+self_hover",s)
  self.assertIn("avatar.position.z+hover.meters",s)
 def test_real_pipeline_warning_not_bogus_success(self):
  s=source("src/app/application.cpp")
  p=source("include/nexverse/ui/avatar_pipeline_proof.hpp")
  self.assertIn("avatar_proof.self_hover_invalid=current_self_hover_invalid",s)
  self.assertIn("avatar_proof.remote_hover_invalid=current_remote_hover_invalid",s)
  self.assertIn("const auto hover_warning=avatar_proof.hover_stage()",s)
  self.assertIn("avatar_hover_status(",p)
 def test_reject_nan_infinite_and_constrain_large_finite(self):
  h=source("include/nexverse/render/avatar_hover_safety.hpp")
  self.assertIn("!std::isfinite(z)",h)
  self.assertIn("std::clamp(z,-2.0f,2.0f)",h)
 def test_native_cmake_and_pi_runner(self):
  self.assertIn("avatar_hover_safety_tests",
                source("scripts/pi5-headless-tests.sh"))
  self.assertIn("avatar_hover_projection_tests",
                source("scripts/pi5-headless-tests.sh"))
  self.assertIn("tests/avatar_hover_projection_tests.cpp",
                source("CMakeLists.txt"))
  self.assertIn("tests/avatar_hover_safety_tests.cpp",
                source("CMakeLists.txt"))
if __name__=="__main__":unittest.main()
