"""Alpha.255-264: real F7 terrain manual recovery, epoch safety and visible UI."""
import json
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def src(path): return (ROOT/path).read_text(encoding="utf-8")
class TerrainManualRecoveryContract(unittest.TestCase):
    def test_safe_policy_is_called_for_actual_requests(self):
        policy=src("include/nexverse/assets/terrain_detail_retry.hpp")
        app=src("src/app/application.cpp")
        for needle in ("max_manual_restarts=2U","attempts<max_attempts",
                       "now<retry_after","!capability_available",
                       "if(!manual_allowed(ids,now,in_flight,capability_available))",
                       "++manual_restarts","attempts=0U","retry_after=now"):
            self.assertIn(needle,policy)
        self.assertIn("terrain_detail_retry.restart_manually(",app)
        self.assertIn("terrain_materials_available &&",app)
        self.assertIn("terrain_texture_loading,!texture_capability.empty()",app)
        self.assertIn("terrain_detail_retry.ready(",app)
        self.assertIn("terrain_detail_retry.missing(ids)",app)
        self.assertIn("material_scope.matches(terrain_texture_request_epoch)",app)
    def test_new_button_cannot_send_unavailable_request(self):
        shell=src("include/nexverse/ui/viewer_shell.hpp")
        renderer=src("src/ui/viewer_shell_render.cpp")
        app=src("src/app/application.cpp")
        self.assertIn("return state.terrain_retry_available",shell)
        self.assertIn("?ViewerAction::retry_terrain:ViewerAction::disabled",shell)
        self.assertIn("shell.window==nexverse::ui::ViewerWindow::region &&",app)
        self.assertIn("shell.terrain_retry_available",app)
        self.assertIn("GLFW_KEY_T",app)
        self.assertIn('enabled?"Fehlende Layer nachladen [T]"',renderer)
        self.assertIn("state.status_text",renderer)
    def test_status_is_grounded_without_server_identifiers(self):
        status=src("include/nexverse/ui/terrain_recovery_status.hpp")
        renderer=src("src/ui/viewer_shell_render.cpp")
        app=src("src/app/application.cpp")
        for needle in ("CAP fehlt","Download aktiv",
                       "retry_wait_seconds(now)","manuell ausgeschoepft",
                       "terrain_recovery_layers","L1","geladen"):
            if needle=="L1":
                self.assertIn('std::to_string(i+1U)',status)
            else:
                self.assertIn(needle,status)
        self.assertIn("shell_view.terrain_recovery_layers=",app)
        self.assertIn("v.terrain_recovery_layers",renderer)
        self.assertNotIn("terrain_detail_ids[index]",renderer)
    def test_server_addon_remains_opt_in(self):
        report=src("docs/ALPHA255_264_TERRAIN_MANUAL_RECOVERY.md")
        self.assertIn("bf55a17524a9",report)
        self.assertIn("disabled-by-default",report)
        self.assertIn("KEIN",report)
    def test_release_and_progress_accounting(self):
        data=json.loads(src("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(src("VERSION").strip(),"0.2.0-alpha.394")
        self.assertEqual(data["viewer_version"],src("VERSION").strip())
        self.assertEqual(data["next_batch"],"Alpha.395–404")
        self.assertEqual(data["default_blocks_per_run"],10)
        for f in ("AGENTS.md","projectplan.md","info.md","CHATUEBERGABE.md",
                  "docs/PROJEKTFORTSCHRITT.md"):
            with self.subTest(file=f):
                head=src(f)[:2400]
                self.assertIn("39 %",head)
                self.assertIn("Alpha.395–404",head)
if __name__=="__main__": unittest.main()
