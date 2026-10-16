"""Loading-screen diagnostics: no world-ready dependence, safe manual retry."""
import pathlib
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[1]
APP=(ROOT/"src/app/application.cpp").read_text()
UI=(ROOT/"include/nexverse/ui/loading_diagnostics_layout.hpp").read_text()
POLICY=(ROOT/"include/nexverse/ui/loading_circuit_retry_policy.hpp").read_text()
REPORT=(ROOT/"src/ui/avatar_diagnostic_report.cpp").read_text()
MODEL=(ROOT/"include/nexverse/ui/world_entry_stage.hpp").read_text()

class LoadingDiagnosticsWorkflowTests(unittest.TestCase):
    def test_f3_works_while_authenticated_but_scene_missing(self):
        self.assertIn('context->login->authenticated&&',APP)
        self.assertIn('context->simulation_ready&&!*context->simulation_ready&&',APP)
        self.assertIn('context->loading_diagnostics_open&&action==GLFW_PRESS',APP)
        self.assertIn('key==GLFW_KEY_F3',APP)
        self.assertIn('key==GLFW_KEY_ESCAPE',APP)
        self.assertIn('key==GLFW_KEY_S&&context->avatar_report_save_requested',APP)

    def test_loading_mouse_buttons_have_real_actions(self):
        for token in ('LoadingDiagAction::open','LoadingDiagAction::close',
                      'LoadingDiagAction::save','LoadingDiagAction::retry'):
            self.assertIn(token,APP)
            self.assertIn(token,UI)
        self.assertIn('render_loading_diagnostics_panel(renderer,',APP)
        self.assertIn('loading_circuit_retry_available,world_entry_wait_hint)',APP)
        self.assertIn('"DIAGNOSE [F3]"',APP)
        self.assertIn('"SIMULATOR NEU VERBINDEN"',APP)

    def test_retry_never_starts_a_second_worker_or_new_endpoint(self):
        for required in ('worker_finished','stop_requested','valid_target',
                         'cooldown_expired','final_failed'):
            self.assertIn(required,POLICY)
        self.assertIn('circuit_future.wait_for(std::chrono::seconds{0})',APP)
        self.assertIn('region.simulator_port==circuit_target_port',APP)
        self.assertIn('last_manual_circuit_retry',APP)
        self.assertIn('std::chrono::seconds{15}',APP)
        self.assertIn('(void)circuit_future.get()',APP)
        self.assertIn('launch_circuit(std::move(retry_session))',APP)

    def test_report_stage_is_whitelisted_and_region_is_independent(self):
        self.assertIn('WorldEntryStage world_entry_stage',(
            ROOT/"include/nexverse/ui/avatar_diagnostic_report.hpp").read_text())
        self.assertIn('world_entry_stage_key(r.world_entry_stage)',REPORT)
        self.assertIn('avatar_diagnostic.region_connected=region.connected',APP)
        self.assertNotIn('world_entry_status=="3D-Welt freigegeben"',APP)
        for field in ('terrain_packets','object_packets','avatar_packets',
                      'other_packets','identity_pair_present'):
            self.assertIn(field,REPORT)

    def test_loading_progress_ignores_optional_assets_and_welcome_stays(self):
        self.assertIn('world_entry_fraction(world_entry_stage)',APP)
        self.assertIn('std::clamp(required_progress,0.04f,0.96f)',APP)
        self.assertIn('world_entry_wait_monitor.seconds(',APP)
        self.assertIn('world_entry_wait_hint(',MODEL)
        self.assertIn('renderer.image(welcome_texture,',APP)
        self.assertNotIn('static_cast<float>(completed) / 5.0f',APP)

if __name__=='__main__':
    unittest.main()
