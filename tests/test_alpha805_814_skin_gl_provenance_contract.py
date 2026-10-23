from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text()
class BakeGlSource(unittest.TestCase):
 def test_real_gl_writes_per_frame(self):
  s=read('src/render/opengl_renderer.cpp')
  self.assertIn('diagnostics_.self_avatar_body_drawn_bake_ids.fill({});',s)
  self.assertIn('diagnostics_.self_avatar_body_drawn_bake_ids[slot]=id;',s)
 def test_low158_and_object_provisional_require_same_sources(self):
  s=read('src/app/application.cpp')
  self.assertEqual(s.count('avatar_proof.current_body_bake_sources_drawn[slot]='),2)
  self.assertEqual(s.count('render_diagnostics.self_avatar_body_drawn_bake_ids[slot]'),2)
  p=read('include/nexverse/ui/avatar_pipeline_proof.hpp')
  self.assertEqual(p.count('current_bake_sources_match() &&'),2)
  self.assertIn('Texturstand abweichend',read('include/nexverse/ui/avatar_bake_inspector.hpp'))
 def test_original_firestorm_gl_bound_uuid(self):
  s=read('tests/avatar_body_framebuffer_pixels_tests.cpp')
  self.assertIn('native_draw.self_avatar_body_drawn_bake_ids!=bake_ids',s)
 def test_handoff(self):
  self.assertEqual(read('VERSION').strip(),'0.2.0-alpha.1024')
  self.assertIn('Alpha.1025–1034',read('CHATUEBERGABE.md'))
