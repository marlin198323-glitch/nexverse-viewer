"""Ten-block live renderer/UI contract; NOT a fake Firestorm E2E proof."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(relative):
    return (ROOT/relative).read_text(encoding="utf-8")

class VisualWorldTenBlocksContract(unittest.TestCase):
    def test_skin_server_ref_gpu_and_actual_draw_are_independent(self):
        code=source("include/nexverse/ui/avatar_visual_truth.hpp")
        for token in ("server_appearance","server_body_references",
                      "resident_body_bakes","baked_body_gl_draws"):
            self.assertIn(token,code)
        self.assertIn("avatar_proof.visual_truth.server_body_references",
                      source("src/app/application.cpp"))

    def test_self_animation_gl_counts_not_remote_avatar_counts(self):
        render=source("src/render/opengl_renderer.cpp")
        self.assertIn("self_avatar_keyframe_skin_draw_calls",render)
        self.assertIn("self_avatar_server_selected_skin_draw_calls",render)
        self.assertIn("if(avatar.is_self)",render)
        self.assertIn("avatar_proof.animation_stage()",source("src/app/application.cpp"))

    def test_genuine_four_terrain_layers_not_any_real(self):
        render=source("src/render/opengl_renderer.cpp")
        self.assertIn("++terrain_real_detail_layers_",render)
        self.assertIn("diagnostics_.terrain_real_detail_layers=",render)
        self.assertIn("terrain_real_detail_layers_",render)
        self.assertIn("terrain_material_status(",source("src/app/application.cpp"))
        self.assertIn("Boden: 4/4 echte Terrain-Texturen",
                      source("include/nexverse/render/terrain_layer_coverage.hpp"))

    def test_quality_is_requested_lod_equals_gpu_lod(self):
        audit=source("include/nexverse/ui/mesh_quality_evidence.hpp")
        self.assertIn("it->second==lod",audit)
        app=source("src/app/application.cpp")
        self.assertIn("inspect_mesh_quality(mesh_desired_lods,mesh_gpu_lods)",app)
        self.assertIn("shell_view.mesh_quality_status=",app)
        self.assertIn("v.mesh_quality_status",source("src/ui/viewer_shell_render.cpp"))

    def test_f3_avatar_status_and_private_report(self):
        app=source("src/app/application.cpp")
        self.assertIn("shell_view.avatar_skin_status=",app)
        self.assertIn("shell_view.avatar_animation_status=",app)
        for f in ("terrain_real_layers","mesh_lod_matched",
                  "avatar_body_bakes_gl","avatar_self_keyframe_draws"):
            self.assertIn("avatar_diagnostic."+f+"=",app)
            self.assertIn('"'+f+'="',source("src/ui/avatar_diagnostic_report.cpp"))
        report=source("src/ui/avatar_diagnostic_report.cpp")
        for forbidden in ("session_id=","agent_id=","simulator_ip=",
                          "password=","avatar_name="):
            self.assertNotIn('"'+forbidden,report)
        self.assertIn(",0600)",report)

    def test_original_welcome_and_system_icon_preserved(self):
        app=source("src/app/application.cpp")
        self.assertIn("welcome_texture",app)
        self.assertIn("brand_texture",app)
        self.assertIn("mint-isolated-gui-smoke.sh",
                      source("docs/ALPHA235_244_VISUAL_EVIDENCE.md"))

    def test_next_chat_has_ten_blocks_and_progress(self):
        handoff=source("CHATUEBERGABE.md")
        self.assertIn("Alpha.505–514",handoff[:2600])
        self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",handoff[:700])
        self.assertIn("39 %",handoff[:1000])
        self.assertIn("ZEHN-BLÖCKE-REGEL",source("projectplan.md")[:2500])

if __name__=="__main__":
    unittest.main()
