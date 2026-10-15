"""Static wiring regression for anatomical avatar GL proof (not GPU E2E)."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parent.parent
RENDER=(ROOT/"src/render/opengl_renderer.cpp").read_text()
APP=(ROOT/"src/app/application.cpp").read_text()
RENDERER=(ROOT/"include/nexverse/render/renderer.hpp").read_text()


class AvatarBodyGpuWiring(unittest.TestCase):
    def test_per_frame_reset_before_anatomical_render(self):
        self.assertIn("self_avatar_body_bake_gl_draws.fill(0U);",RENDER)
        begin=RENDER.index("void begin_frame() override")
        render=RENDER.index("void render_world(double time_seconds) override")
        reset=RENDER.index("self_avatar_body_bake_gl_draws.fill(0U);")
        self.assertLess(begin,reset)
        self.assertLess(reset,render)
        self.assertIn("std::array<std::uint64_t,3U> self_avatar_body_bake_gl_draws",RENDERER)

    def test_no_procedural_proxy_can_count_as_original_mesh(self):
        self.assertEqual(RENDER.count(
            "++diagnostics_.self_avatar_body_bake_gl_draws[slot];"),1)
        original=RENDER.index("if(anatomical_ready)")
        marker=RENDER.index(
            "++diagnostics_.self_avatar_body_bake_gl_draws[slot];")
        proxy=RENDER.index("for (const auto& part : parts)",original)
        self.assertLess(original,marker)
        self.assertLess(marker,proxy)
        self.assertNotIn("self_avatar_body_bake_gl_draws",
                         RENDER[proxy:RENDER.index("draw_soft_particles(",proxy)])
        self.assertIn("if(baked)",RENDER[original:marker])
        self.assertIn("glDrawElements(GL_TRIANGLES,gpu.index_count",
                      RENDER[original:marker])

    def test_uv_rejection_before_gpu_upload(self):
        start=RENDER.index("bool set_avatar_body_mesh(")
        upload=RENDER.index("upload_mesh(candidate,source)",start)
        guard=RENDER.index("if(!audit_avatar_body_uv(",start)
        self.assertLess(guard,upload)
        self.assertIn("return false;",RENDER[guard:upload])

    def test_f9_real_bake_previews_do_not_fetch_assets(self):
        self.assertIn("select_avatar_bake_previews(",APP)
        self.assertIn("avatar_bake_preview_handle(",APP)
        self.assertIn("gallery_appearance_live_enabled&&simulation_ready",APP)
        self.assertIn("Server-Bake GPU (nur Vorschau)",APP)
        self.assertIn("live_body_previews",APP)
        self.assertIn("pin_self_avatar_body_full_mip(",APP)

    def test_avatar_shader_resets_scene_planar_mapping(self):
        marker=RENDER.index("// Avatar proxy rendering is deliberately separate")
        avatar_segment=RENDER[marker:RENDER.index("struct Part",marker)]
        self.assertIn('"uObjectTexGenPlanar"',avatar_segment)
        self.assertIn('"uObjectScale"',avatar_segment)
        self.assertIn('"uUsePrimDeform"',avatar_segment)

    def test_f9_draw_proof_is_connected_and_responsive(self):
        self.assertIn("render_diagnostics.self_avatar_body_bake_gl_draws,",APP)
        self.assertIn("bool wide_gallery=pw>=760.0f*scale;",APP)
        self.assertIn("describe_avatar_gl_body_draws(",APP)
        self.assertIn("gl_probe_enabled?theme::cyan:theme::text_muted",APP)
        self.assertIn("gallery_appearance_live_enabled=false;",APP)


if __name__=="__main__":
    unittest.main()
