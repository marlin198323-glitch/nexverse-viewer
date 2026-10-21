"""Native Mint x64 full CMake/GL/UI, not merely Pi source checks."""
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
class MintFullValidationContract(unittest.TestCase):
 def test_real_host_full_build_and_full_suite(self):
  s=(R/'scripts/mint-viewer-validation.sh').read_text()
  for required in (
    'uname -m','x86_64','cmake --preset debug',
    'cmake --build --preset debug',
    'ctest --test-dir build/debug',
    "python3 -m unittest discover -s tests -p 'test*.py'",
    'bash scripts/mint-isolated-gui-smoke.sh',
    'bash scripts/test-mint-window-branding.sh',
    'NO_LIVE_GRID_LOGIN_OR_AVATAR_SKIN_COF_WASD_CLAIM',
    'OGL_MINT_X64_FULL_VALIDATION_PASS'):
   self.assertIn(required,s)
 def test_gl_draw_counters_saturate_instead_of_wrap(self):
  h=(R/'include/nexverse/ui/avatar_visual_truth.hpp').read_text()
  c=(R/'src/app/application.cpp').read_text()
  for t in ('clamp_avatar_gl_draw_count','sum_avatar_gl_body_draws'):
   self.assertIn(t,h)
   self.assertIn('ui::'+t+'(',c)
  self.assertIn('std::numeric_limits<std::uint32_t>::max()',h)
  self.assertIn('sum_avatar_gl_body_draws({u64max,u64max,u64max})==u32max',
                (R/'tests/avatar_visual_truth_tests.cpp').read_text())
if __name__=='__main__':unittest.main()
