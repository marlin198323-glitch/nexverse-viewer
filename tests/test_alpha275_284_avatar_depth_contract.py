"""Alpha.275-284: true original mesh per-part GPU depth probes and UI export."""
import json
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path): return (ROOT/path).read_text()

class Alpha275DepthProof(unittest.TestCase):
    def test_no_silent_negative_overwrite_for_other_body(self):
        source=read("src/render/opengl_renderer.cpp")
        for needle in (
            "std::array<GLuint,6U> self_avatar_visibility_queries_",
            "std::array<bool,3U> self_avatar_part_samples_passed_",
            "std::array<std::uint64_t,3U> self_avatar_part_resolved_sequence_",
            "const std::size_t part=i/2U;",
            "self_avatar_visibility_query_sequence_[i]<",
            "self_avatar_part_resolved_sequence_[part]",
            "self_avatar_part_samples_passed_[part]=passed!=GL_FALSE",
            "std::any_of(\n            self_avatar_part_samples_passed_.begin()",
            "for(std::size_t slot=0U;slot<3U;++slot)",
            "self_avatar_visibility_query_epoch_[i]=",
            "glBeginQuery(GL_ANY_SAMPLES_PASSED,",
            "glEndQuery(GL_ANY_SAMPLES_PASSED);"):
            self.assertIn(needle,source)
        self.assertNotIn("self_avatar_samples_passed_=passed!=GL_FALSE;",source)

    def test_pipeline_uses_real_renderer_current_frame(self):
        app=read("src/app/application.cpp")
        proof=read("include/nexverse/ui/avatar_pipeline_proof.hpp")
        for fragment in (
            "render_diagnostics.self_avatar_part_samples_known",
            "render_diagnostics.self_avatar_part_samples_passed",
            "avatar_proof.part_visibility.compact()",
            "shell_view.avatar_depth_parts=",
            "avatar_diagnostic.avatar_depth_passed=",
        ):
            self.assertIn(fragment,app)
        self.assertIn("part_visibility.any_known()",proof)
        self.assertIn("part_visibility.any_passed()",proof)

    def test_real_gui_and_authentic_bake_distinction(self):
        gui=read("src/ui/viewer_shell_render.cpp")
        inspector=read("include/nexverse/ui/avatar_bake_inspector.hpp")
        self.assertIn("v.avatar_depth_parts",gui)
        self.assertIn("proof.part_visibility.stage(i)",inspector)
        self.assertIn("proof.appearance_received",inspector)
        self.assertIn("BakeGL",inspector)
        self.assertIn("state.avatar_bake_page==1U?0U:6U",gui)

    def test_private_export_and_epoch_reset(self):
        exporter=read("src/ui/avatar_diagnostic_report.cpp")
        source=read("src/render/opengl_renderer.cpp")
        for part in ("head","upper","lower"):
            for field in ("known","passed"):
                self.assertIn("avatar_depth_"+part+"_"+field,exporter)
        self.assertIn("self_avatar_part_samples_known_.fill(false)",source)
        self.assertIn("self_avatar_part_samples_passed_.fill(false)",source)
        self.assertIn("self_avatar_visibility_query_epoch_[i]!=",source)
        self.assertNotIn("std::to_string(packet.avatar_id)",exporter)

    def test_versions_and_hand_off(self):
        self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.684")
        progress=json.loads(read("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(progress["viewer_version"],"0.2.0-alpha.684")
        self.assertEqual(progress["default_blocks_per_run"],10)
        self.assertEqual(progress["next_batch"],"Alpha.685–694")
        for path in ("CHATUEBERGABE.md","AGENTS.md","info.md","projectplan.md","docs/PROJEKTFORTSCHRITT.md"):
            text=read(path)[:2100]
            self.assertIn("39 %",text)
            self.assertIn("Alpha.685–694",text)
if __name__=="__main__":unittest.main()
