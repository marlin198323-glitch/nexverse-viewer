"""Alpha.265-274: source-to-UI avatar server-bake progress and safe recheck."""
import json
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def source(name): return (ROOT/name).read_text(encoding="utf-8")

class Alpha265AvatarBakeContracts(unittest.TestCase):
    def test_current_region_server_only_and_gpu_truth(self):
        app=source("src/app/application.cpp")
        inspect=source("include/nexverse/ui/avatar_bake_inspector.hpp")
        for needle in (
            "protocol::self_bake_owner_is_current(",
            "world_model.self_appearance()",
            "firestorm_region_lease.matches(current_firestorm_epoch)",
            "avatar_proof.bakes=protocol::audit_avatar_bake_evidence(",
            "avatar_proof.body_bake_draws=",
            "ui::inspect_avatar_bakes(avatar_proof)"):
            self.assertIn(needle,app)
        self.assertIn("proof.body_gpu_drawn()",inspect)
        self.assertIn("proof.fragment_samples_passed",inspect)
        self.assertIn("Server: ID / GPU fehlt",inspect)
        self.assertIn("BakeGL noch unvollstaendig",inspect)

    def test_recheck_only_replans_with_original_backoff(self):
        app=source("src/app/application.cpp")
        policy=source("include/nexverse/assets/avatar_bake_recheck.hpp")
        self.assertIn("avatar_bake_recheck.enter(material_scope.epoch())",app)
        self.assertIn("assets::avatar_bake_ready_missing(",app)
        self.assertIn("avatar_bake_recheck.request(",app)
        self.assertIn("last_texture_residency_plan={};",app)
        self.assertIn("texture_request_fairness.choose(",app)
        self.assertIn("object_texture_retries.ready(",app)
        self.assertIn("object_texture_loading",app)
        self.assertIn("max_requests=3U",policy)
        self.assertIn("std::chrono::seconds{12}",policy)
        self.assertIn("schedule.ready(id,now)",policy)
        self.assertIn("gpu.contains(id)",policy)
        self.assertNotIn("schedule.success(",policy)
        self.assertNotIn("fetch_rgba(",policy)

    def test_visible_f3_buttons_keyboard_and_both_pages(self):
        gui=source("src/ui/viewer_shell_render.cpp")
        shell=source("include/nexverse/ui/viewer_shell.hpp")
        app=source("src/app/application.cpp")
        self.assertIn("state.avatar_bake_page==0U",gui)
        self.assertIn("state.avatar_bake_page==1U?0U:6U",gui)
        self.assertIn("Bakes 1/2 [B]",gui)
        self.assertIn("Bakes pruefen [R]",gui)
        self.assertIn("Pruefung aus",gui)
        self.assertIn("state.avatar_recheck_available",shell)
        self.assertIn("?ViewerAction::recheck_avatar_bakes",shell)
        self.assertIn("GLFW_KEY_B",app)
        self.assertIn("GLFW_KEY_R",app)
        self.assertIn("shell.avatar_recheck_requested",app)
        self.assertIn("shell_view.avatar_bake_rows=bake_inspector.rows",app)

    def test_privacy_upload_separation(self):
        inspector=source("include/nexverse/ui/avatar_bake_inspector.hpp")
        for secret in ("avatar_id","session_id","password","authorization","access_token"):
            self.assertNotIn(secret,inspector)
        self.assertNotIn("upload",inspector.lower())
        self.assertIn("KEIN",source("docs/ALPHA265_274_AVATAR_BAKE_INSPECTOR.md"))

    def test_progress_and_next_chat_consistency(self):
        progress=json.loads(source("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(source("VERSION").strip(),"0.2.0-alpha.874")
        self.assertEqual(progress["viewer_version"],"0.2.0-alpha.874")
        self.assertEqual(progress["default_blocks_per_run"],10)
        self.assertEqual(progress["next_batch"],"Alpha.875–884")
        for name in ("CHATUEBERGABE.md","projectplan.md","info.md","AGENTS.md",
                     "docs/PROJEKTFORTSCHRITT.md"):
            with self.subTest(file=name):
                self.assertIn("39 %",source(name)[:2400])
                self.assertIn("Alpha.875–884",source(name)[:2400])
if __name__=="__main__":unittest.main()
