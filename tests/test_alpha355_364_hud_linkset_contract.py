from pathlib import Path
import json,unittest
R=Path(__file__).resolve().parents[1]
def src(p):return (R/p).read_text()
class HudTests(unittest.TestCase):
 def test_renderer_and_hierarchy(self):
  self.assertIn("classify_attachment_scene(",src("src/render/opengl_renderer.cpp"))
  self.assertIn("AttachmentSceneKind::invalid_parent",src("include/nexverse/render/attachment_scene_classification.hpp"))
 def test_f3_counters(self):
  self.assertIn("avatar_hud_world_excluded",src("src/app/application.cpp"))
  self.assertIn("HUD 3D aus:",src("src/ui/viewer_shell_render.cpp"))
 def test_persistent_release(self):
  self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.374")
  p=json.loads(src("docs/PROJECT_PROGRESS.json"))
  self.assertEqual(p["next_batch"],"Alpha.375–384")
if __name__=="__main__":unittest.main()
