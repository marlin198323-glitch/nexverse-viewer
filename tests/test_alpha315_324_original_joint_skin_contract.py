"""Alpha.315-324: original Firestorm GPU skinning palette last bone."""
import json
from pathlib import Path
import unittest
R=Path(__file__).resolve().parents[1]
def src(p):return (R/p).read_text(encoding="utf-8")
class OriginalAvatarSkinning(unittest.TestCase):
    def test_last_joint_is_real_not_identity(self):
        s=src("src/render/opengl_renderer.cpp")
        self.assertIn("uniform int uAvatarJointCount;",s)
        self.assertIn("int base=min(int(floor(weight)),uAvatarJointCount-2)",s)
        self.assertIn("float factor=clamp(weight-float(base),0.0,1.0)",s)
        self.assertNotIn("float factor=fract(weight);",s)
        self.assertNotIn("float weight=clamp(aAvatarJointWeight,0.0,30.999)",s)
    def test_per_body_mesh_gpu_palette_upload(self):
        s=src("src/render/opengl_renderer.cpp")
        self.assertIn("resolve_avatar_skin_weight(",s)
        self.assertIn("weight,geometry.skin_palette.size()).valid",s)
        self.assertIn('program_,"uAvatarJointCount"',s)
        self.assertIn("gpu.skin_palette.size()):0",s)
        self.assertIn("skin_valid?GL_TRUE:GL_FALSE",s)
    def test_joint_weight_boundary_native(self):
        h=src("include/nexverse/render/avatar_skin_weight_pair.hpp")
        t=src("tests/avatar_skin_weight_pair_tests.cpp")
        for name in ("palette_size-2U","palette_size>32U","palette_size<2U",
                     "weight>=static_cast<float>(palette_size)",
                     "weight-static_cast<float>(first)"):
            self.assertIn(name,h)
        for name in ("31.9f","1.5f","quiet_NaN()","w+=0.125f"):
            self.assertIn(name,t)
        self.assertIn("avatar_skin_weight_pair_tests",src("scripts/pi5-headless-tests.sh"))
        self.assertIn("nexverse-avatar-skin-weight-pair-tests",src("CMakeLists.txt"))
    def test_real_visible_f3_skinning_draw_count(self):
        app=src("src/app/application.cpp")
        shell=src("src/ui/viewer_shell_render.cpp")
        view=src("include/nexverse/ui/viewer_shell_render.hpp")
        self.assertIn("shell_view.avatar_skinning_status=",app)
        self.assertIn("render_diagnostics.self_avatar_weighted_skin_draw_calls",app)
        self.assertIn("render_diagnostics.self_avatar_keyframe_skin_draw_calls",app)
        self.assertIn("v.avatar_skinning_status",shell)
        self.assertIn("avatar_skinning_status",view)
    def test_handoff_and_progress(self):
        self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.434")
        data=json.loads(src("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(data["viewer_version"],"0.2.0-alpha.434")
        self.assertEqual(data["next_batch"],"Alpha.435–444")
        self.assertEqual(data["default_blocks_per_run"],10)
        for p in ("AGENTS.md","info.md","CHATUEBERGABE.md","projectplan.md",
                  "docs/PROJEKTFORTSCHRITT.md"):
            s=src(p)[:2000]
            self.assertIn("39 %",s)
            self.assertIn("Alpha.435–444",s)
if __name__=="__main__":unittest.main()
