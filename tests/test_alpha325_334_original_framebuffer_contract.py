import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
class PixelTest(unittest.TestCase):
 def test_pixel(self):
  s=(R/'tests/avatar_body_framebuffer_pixels_tests.cpp').read_text()
  self.assertIn('glReadPixels',s)
  self.assertIn('BODY_PART_BAKE_PIXELS_',s)
  self.assertIn('EVICTED_BAKE_STALE_RGB_PIXELS=',s)
