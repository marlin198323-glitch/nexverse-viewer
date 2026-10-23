"""Alpha.365-374: 128-depth avatar linksets, renderer pixels and F3 evidence."""
import json
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[1]
def source(path):return (R/path).read_text(encoding="utf-8")
class LinksetDepth(unittest.TestCase):
 def test_shared_parent_chain(self):
  header=source('include/nexverse/render/scene_parent_chain.hpp')
  classifier=source('include/nexverse/render/attachment_scene_classification.hpp')
  shader=source('src/render/opengl_renderer.cpp')
  self.assertIn('max_depth=128U',header)
  self.assertIn('resolve_scene_parent_chain(',classifier)
  self.assertIn('resolve_scene_parent_chain(',shader)
  self.assertNotIn('depth >= 16U',shader)
  self.assertIn('chain.leaf_to_root[i-1U]',shader)
 def test_hud_and_cycles(self):
  t=source('tests/scene_parent_chain_tests.cpp')
  for marker in ('objects,lookup,avatars,20U','objects,lookup,avatars,127U',
                 'objects,lookup,avatars,128U','AttachmentSceneKind::invalid_parent',
                 'incomplete.missing_parent'):
   self.assertIn(marker,t)
 def test_actual_gl_pixel_acceptance(self):
  t=source('tests/scene_deep_linkset_pixels_tests.cpp')
  for marker in ('glReadPixels(','DEEP_LINKSET_WORN_RGB_PIXELS=',
                 'DEEP_HUD_LINKSET_STALE_RGB_PIXELS=',
                 'stats.scene_hud_linkset_excluded','hud_delta!=0U',
                 'scene_max_visible_parent_depth','OGL_DEEP_LINKSET_GL_PIXELS_PASS'):
   self.assertIn(marker,t)
  c=source('CMakeLists.txt')
  self.assertIn('nexverse-scene-deep-linkset-pixels-tests',c)
  self.assertIn('SKIP_RETURN_CODE 77',c)
 def test_real_f3_and_private_diagnostics(self):
  ui=source('src/ui/viewer_shell_render.cpp')
  app=source('src/app/application.cpp')
  self.assertIn('Linktiefe:',ui)
  self.assertIn('avatar_scene_max_parent_depth',app)
  self.assertIn('scene_linkset_max_depth',source('src/ui/avatar_diagnostic_report.cpp'))
  self.assertIn('scene_linkset_deep_visible',source('include/nexverse/ui/avatar_diagnostic_report.hpp'))
 def test_release_handoff(self):
  self.assertEqual(source('VERSION').strip(),'0.2.0-alpha.1124')
  j=json.loads(source('docs/PROJECT_PROGRESS.json'))
  self.assertEqual(j['viewer_version'],'0.2.0-alpha.1124')
  self.assertEqual(j['next_batch'],'Alpha.1125–1134')
  self.assertEqual(j['default_blocks_per_run'],10)
  for f in ('CHATUEBERGABE.md','AGENTS.md','info.md','projectplan.md','docs/PROJEKTFORTSCHRITT.md'):
   self.assertIn('Alpha.1125–1134',source(f)[:2000])
   self.assertIn('39 %',source(f)[:2000])
if __name__=="__main__":unittest.main()
