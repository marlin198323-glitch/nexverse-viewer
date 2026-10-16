"""Linux Mint primary build/handoff regression; not a GUI E2E test."""
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parent.parent

class MintBuildContractTests(unittest.TestCase):
    def test_version_triggers_cmake_reconfigure(self):
        cmake=(ROOT/"CMakeLists.txt").read_text()
        self.assertIn("CMAKE_CONFIGURE_DEPENDS",cmake)
        self.assertIn('file(READ "'+chr(36)+'{CMAKE_CURRENT_SOURCE_DIR}/VERSION"',cmake)
        self.assertIn('NEXVERSE_VERSION="'+chr(36)+'{NEXVERSE_VERSION_LABEL}"',cmake)
    def test_login_ui_truthfully_reports_original_gpu_asset_state(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("original_body_gpu_ready=true;",app)
        self.assertIn("Avatar-Mesh: Firestorm 3/3 auf GPU bereit",app)
        self.assertIn("Avatar-Mesh: Originaldateien fehlen",app)
        self.assertIn("welcome_texture,original_body_gpu_ready,viewer_shell",app)

    def test_avatar_packet_gpu_draw_stages_on_inworld_hud(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("ui::AvatarPipelineProof avatar_proof{}",app)
        self.assertIn("protocol::audit_avatar_bake_evidence(",app)
        self.assertIn("describe_avatar_pipeline(avatar_proof)",app)
        self.assertIn("describe_avatar_body_pipeline(avatar_proof)",app)
        self.assertIn("self_bake_owner_is_current(",app)
        # System status must no longer overlap with the taller CAPS card.
        self.assertIn('rail_y + 237.0f * scale,\n        11.0f * scale,\n        "Systemstatus"',app)
        self.assertIn("rail_y+370.0f*scale",app)
        self.assertIn("world_hud_panel_visibility(rail_h,scale)",app)
        self.assertIn("if(card_visibility.connection)",app)
        self.assertIn("if(card_visibility.system)",app)
        self.assertIn("if(card_visibility.avatar)",app)
        self.assertIn("avatar_proof.body_bake_draws=",app)

    def test_world_hud_f5_and_click_keep_3d_active(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("key==GLFW_KEY_F5",app)
        self.assertIn("renderer,width,height,false,camera_distance)",app)
        self.assertIn("if(world_hud_visible)",app)
        self.assertIn("input_context.world_hud_visible=&world_hud_visible",app)
        self.assertIn("HUD EIN  [F5]",app)

    def test_camera_zoom_scroll_raycast_streaming_and_f9_shortcut(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("glfwSetScrollCallback(window,scroll_callback)",app)
        self.assertIn("hit_camera_zoom(",app)
        self.assertIn("render_camera_from_agent(render_agent,camera_zoom_distance)",app)
        self.assertIn("render_camera_from_agent(agent,camera_zoom_distance)",app)
        self.assertIn("camera_zoom_label(camera_distance)",app)
        self.assertIn("input_context.camera_zoom_distance=&camera_zoom_distance",app)
        self.assertIn("key==GLFW_KEY_B&&context->outfit_bake_preview_open",app)

    def test_third_person_camera_is_sanitized_and_visible_in_hud(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("render::third_person_camera(",app)
        self.assertIn("camera_fallback_this_frame=",app)
        self.assertIn("scene_proof.camera_fallback=camera_fallback_this_frame",app)
        self.assertIn("render_camera_from_agent(render_agent,camera_zoom_distance)",app)

    def test_scene_can_unlock_without_terrain_and_status_is_honest(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        model=(ROOT/"src/world/world_model.cpp").read_text()
        self.assertEqual(app.count("simulation_ready = world_model.scene_ready();"),2)
        self.assertIn("return scene_ready() && terrain_.available;",model)
        self.assertIn("describe_terrain_progress(scene_proof)",app)
        self.assertIn("describe_world_draw_progress(scene_proof)",app)
        self.assertIn("caps.status=="+chr(34)+"CAPS LIVE"+chr(34),app)
        self.assertIn("terrain independent",app)

    def test_world_hud_discloses_real_avatar_gpu_provenance(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("describe_avatar_gpu_submission(",app)
        self.assertIn("self_avatar_anatomical_mesh_draw_calls",app)
        self.assertIn("self_avatar_body_bake_gl_draws",app)
        self.assertIn("Avatar GL: kein Body-Draw",
                      (ROOT/"include/nexverse/render/avatar_gpu_submission_status.hpp").read_text())

    def test_host_instructions_keep_pi_runner_separate(self):
        agents=(ROOT/"AGENTS.md").read_text()
        self.assertIn("jam-MS-7D28",agents)
        self.assertIn("nur als Self-hosted-CI",agents)
        self.assertIn("develop-current",agents)

if __name__=="__main__":
    unittest.main()
