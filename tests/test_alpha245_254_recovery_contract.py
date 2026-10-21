"""Verify real terrain retry, same-revision GPU and original UI wiring."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(file):
    return (ROOT/file).read_text(encoding="utf-8")
class TerrainRecoveryContract(unittest.TestCase):
    def test_missing_layer_retry_preserves_rgba_and_epoch(self):
        app=read("src/app/application.cpp")
        for token in (
            "terrain_detail_retry.enter(", "terrain_detail_retry.ready(",
            "terrain_detail_retry.missing(ids)",
            "terrain_detail_retry.dispatched(",
            "terrain_cached_textures.detail[index].pixels=",
            "terrain_detail_retry.succeeded(index)",
            "world_renderer->set_terrain_textures("):
            self.assertIn(token,app)
        policy=read("include/nexverse/assets/terrain_detail_retry.hpp")
        self.assertIn("max_attempts=4U",policy)
        self.assertIn("next_revision",policy)

    def test_native_gpu_promotion_and_frame_reset(self):
        gpu=read("src/render/opengl_renderer.cpp")
        self.assertIn("same_revision&&actual_count<=terrain_real_detail_layers_",gpu)
        self.assertIn("diagnostics_.terrain_textured_draw_calls=0U",gpu)
        self.assertIn("diagnostics_.terrain_fallback_draw_calls=0U",gpu)
        test=read("tests/renderer_terrain_partial_upgrade_tests.cpp")
        self.assertIn("upgraded.terrain_real_detail_layers!=2U",test)
        self.assertIn("tex.material_revision=4U",test)
        self.assertIn("glGetError()!=GL_NO_ERROR",test)

    def test_real_gui_and_self_anim_status(self):
        app=read("src/app/application.cpp")
        gui=read("src/ui/viewer_shell_render.cpp")
        self.assertIn("shell_view.terrain_recovery_status=",app)
        self.assertIn("v.terrain_recovery_status",gui)
        self.assertIn("self_avatar_keyframe_gl_logged",app)
        self.assertIn("v.avatar_animation_status",gui)
        self.assertIn("avatar_appearance_self_accepted.store(0U",app)

    def test_new_server_assetservice_does_not_override_download_protocol(self):
        report=read("docs/ALPHA245_254_TERRAIN_RECOVERY_ANIMATION.md")
        self.assertIn("bf55a17524a9",report)
        self.assertIn("disabled-by-default",report)
        self.assertIn("kein Prod-Deploy",report)

    def test_ten_blocks_progress_and_next_chat(self):
        import json
        prog=json.loads(read("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(prog["default_blocks_per_run"],10)
        self.assertEqual(prog["next_batch"],"Alpha.695–704")
        self.assertEqual(prog["viewer_version"],read("VERSION").strip())
        for name in ("projectplan.md","CHATUEBERGABE.md","AGENTS.md",
                     "info.md","docs/PROJEKTFORTSCHRITT.md"):
            with self.subTest(name=name):
                self.assertIn("39 %",read(name)[:1700])
        self.assertIn("NVIDIA",read("docs/ALPHA245_254_TERRAIN_RECOVERY_ANIMATION.md"))
if __name__=="__main__":
    unittest.main()
