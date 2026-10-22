"""Alpha.305-314: preserve authenticated baked skin on partial and preview updates."""
import json
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
def read(path):return (ROOT/path).read_text(encoding="utf-8")
class Alpha305WorldModelContinuity(unittest.TestCase):
    def test_world_model_enforces_authoritative_admission(self):
        model=read("src/world/world_model.cpp")
        guard=read("include/nexverse/protocol/self_appearance_admission.hpp")
        self.assertIn("protocol::assess_self_appearance(",model)
        self.assertIn("agent_.agent_id,region_.connected",model)
        self.assertIn("protocol::appearance_is_authoritative(",model)
        for marker in ("region_not_ready","not_self","trial_preview",
                       "texture_entry_missing","visual_params_missing",
                       "accepted"):
            self.assertIn(marker,guard)
        self.assertIn("if(candidate.trial)",guard)
    def test_handler_does_not_clobber_valid_self(self):
        app=read("src/app/application.cpp")
        self.assertIn("appearance->avatar_id == world_model.agent().agent_id",app)
        self.assertIn("protocol::assess_self_appearance(",app)
        self.assertIn("avatar_appearance_self_trial_ignored.fetch_add(",app)
        self.assertIn("avatar_appearance_self_incomplete_ignored.fetch_add(",app)
        self.assertIn("const bool accepted=world_model.apply_self_appearance(*appearance);",app)
        self.assertIn("self_appearance_serial.fetch_add(1U",app)
        self.assertLess(app.index("const auto admission=protocol::assess_self_appearance("),
                        app.index("const bool accepted=world_model.apply_self_appearance(*appearance);"))
    def test_keep_existing_skin_native_regression(self):
        modeltests=read("tests/world_model_tests.cpp")
        for marker in ("trial.trial=true;","no_texture.texture_entry.clear();",
                       "no_params.visual_params.clear();",
                       "world.self_appearance()->texture_entry!=own.texture_entry"):
            self.assertIn(marker,modeltests)
        native=read("tests/self_appearance_admission_tests.cpp")
        self.assertIn("SelfAppearanceAdmission::accepted",native)
        self.assertIn("SelfAppearanceAdmission::trial_preview",native)
        self.assertIn("SelfAppearanceAdmission::texture_entry_missing",native)
        self.assertIn("SelfAppearanceAdmission::visual_params_missing",native)
        self.assertIn("self_appearance_admission_tests",read("scripts/pi5-headless-tests.sh"))
    def test_f3_actual_status_and_no_faked_bakes(self):
        view=read("include/nexverse/ui/viewer_shell_render.hpp")
        shell=read("src/ui/viewer_shell_render.cpp")
        app=read("src/app/application.cpp")
        self.assertIn("avatar_admission_status",view)
        self.assertIn("v.avatar_admission_status",shell)
        self.assertIn("shell_view.avatar_admission_status=",app)
        self.assertIn("ui::describe_self_appearance_admission(",app)
        self.assertIn("avatar_proof.appearance_received",app)
    def test_private_whitelist_and_limits(self):
        report=read("src/ui/avatar_diagnostic_report.cpp")
        app=read("src/app/application.cpp")
        for marker in ("appearance_self_trial_ignored=",
                       "appearance_self_incomplete_ignored="):
            self.assertIn(marker,report)
        self.assertIn("avatar_diagnostic.appearance_self_trial_ignored=",app)
        self.assertIn("avatar_diagnostic.appearance_self_incomplete_ignored=",app)
        for secret in ('"agent_id="','"session_id="','"capability_url="'):
            self.assertNotIn(secret,report)
    def test_next_batch_and_weighted_percentage(self):
        self.assertEqual(read("VERSION").strip(),"0.2.0-alpha.794")
        progress=json.loads(read("docs/PROJECT_PROGRESS.json"))
        self.assertEqual(progress["viewer_version"],"0.2.0-alpha.794")
        self.assertEqual(progress["next_batch"],"Alpha.795–804")
        self.assertEqual(progress["default_blocks_per_run"],10)
        for file in ("CHATUEBERGABE.md","AGENTS.md","info.md",
                     "projectplan.md","docs/PROJEKTFORTSCHRITT.md"):
            head=read(file)[:1900]
            self.assertIn("39 %",head)
            self.assertIn("Alpha.795–804",head)
if __name__=="__main__":unittest.main()
