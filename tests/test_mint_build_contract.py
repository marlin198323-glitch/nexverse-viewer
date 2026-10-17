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
        self.assertIn("welcome_texture,brand_texture,",app)
        self.assertIn("original_body_gpu_ready,viewer_shell",app)

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
        self.assertIn("renderer,width,height,false,camera_distance,",app)
        self.assertIn("if(world_hud_visible)",app)
        self.assertIn("input_context.world_hud_visible=&world_hud_visible",app)
        self.assertIn("HUD EIN  [F5]",app)

    def test_avatar_orbit_controls_are_real_hud_keyboard_and_gl(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("key==GLFW_KEY_V",app)
        self.assertIn("GLFW_KEY_LEFT_BRACKET",app)
        self.assertIn("GLFW_KEY_RIGHT_BRACKET",app)
        self.assertIn("hit_camera_orbit(",app)
        self.assertIn("orbit_label(camera_orbit_degrees)",app)
        self.assertIn("input_context.camera_orbit_degrees=&camera_orbit_degrees",app)
        self.assertIn("render_camera_from_agent(agent,camera_zoom_distance,camera_orbit_degrees)",app)
        self.assertIn("render_camera_from_agent(render_agent,camera_zoom_distance,camera_orbit_degrees)",app)

    def test_f3_avatar_report_is_real_and_secrets_excluded(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        ui=(ROOT/"src/ui/viewer_shell_render.cpp").read_text()
        query=(ROOT/"src/render/opengl_renderer.cpp").read_text()
        self.assertIn("key==GLFW_KEY_F3",app)
        self.assertIn("key==GLFW_KEY_S&&context->avatar_report_save_requested",app)
        self.assertIn("ViewerAction::save_avatar_report",app)
        self.assertIn("save_avatar_diagnostic(",app)
        self.assertIn("shell_view.avatar_fragment_status=avatar_proof.fragment_stage()",app)
        self.assertIn('"Bericht [S]"',ui)
        self.assertIn("camera_valid_",query)
        self.assertIn("reset_self_avatar_visibility();",query)
        report=(ROOT/"src/ui/avatar_diagnostic_report.cpp").read_text()
        for token in ('agent_id','session_id','password','avatar_name',
                      'simulator_ip','chat_message'):
            self.assertNotIn('"'+token+'=',report)

    def test_terrain_partial_retry_and_live_animation_are_integrated(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        ogl=(ROOT/"src/render/opengl_renderer.cpp").read_text()
        modal=(ROOT/"src/ui/viewer_shell_render.cpp").read_text()
        self.assertIn("terrain_detail_retry.ready(",app)
        self.assertIn("terrain_detail_retry.succeeded(index)",app)
        self.assertIn("terrain_cached_textures.detail[index]",app)
        self.assertIn("actual_count<=terrain_real_detail_layers_",ogl)
        self.assertIn("diagnostics_.terrain_textured_draw_calls=0U",ogl)
        self.assertIn("self_avatar_keyframe_gl_logged",app)
        self.assertIn("actually ",app)
        self.assertIn("v.avatar_animation_status",modal)
        self.assertIn("v.terrain_recovery_status",modal)

    def test_default_camera_closer_and_ultrawide_status_legible(self):
        camera=(ROOT/"include/nexverse/ui/camera_zoom_controls.hpp").read_text()
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("camera_zoom_default=5.5f",camera)
        self.assertIn("9.1f*scale,nexverse::ui::describe_avatar_pipeline",app)
        self.assertIn("8.7f*scale,avatar_proof.fragment_stage()",app)

    def test_terrain_same_revision_retry_updates_gpu_and_welcome_unchanged(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        ogl=(ROOT/"src/render/opengl_renderer.cpp").read_text()
        self.assertIn("terrain_detail_retry.ready(",app)
        self.assertIn("terrain_cached_textures.detail[index]",app)
        self.assertIn("terrain_detail_retry.succeeded(index)",app)
        self.assertIn("actual_count<=terrain_real_detail_layers_",ogl)
        self.assertIn("welcome_texture",app)

    def test_real_f2_detail_button_drives_mesh_lod_policy(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        policy=(ROOT/"include/nexverse/assets/mesh_lod_policy.hpp").read_text()
        self.assertIn("key==GLFW_KEY_F2",app)
        self.assertIn("hit_mesh_quality_button(",app)
        self.assertIn('"DETAIL [F2]"',app)
        self.assertIn("mesh_quality_detailed=true",app)
        self.assertIn("mesh_quality_detailed);",app)
        self.assertIn("bool detailed=false",policy)

    def test_self_avatar_occlusion_query_and_f4_are_live(self):
        src=(ROOT/"src/app/application.cpp").read_text()
        gpu=(ROOT/"src/render/opengl_renderer.cpp").read_text()
        contract=(ROOT/"include/nexverse/render/renderer.hpp").read_text()
        self.assertIn("key==GLFW_KEY_F4",src)
        self.assertIn("hit_avatar_focus(",src)
        self.assertIn('"AVATAR  [F4]"',src)
        self.assertIn("ui::focus_front_avatar()",src)
        self.assertIn("world_renderer->reset_self_avatar_visibility()",src)
        self.assertIn("avatar_proof.fragment_stage()",src)
        self.assertIn("GL_ANY_SAMPLES_PASSED",gpu)
        self.assertIn("GL_QUERY_RESULT_AVAILABLE",gpu)
        self.assertIn("self_avatar_visibility_query_epoch_",gpu)
        self.assertIn("self_avatar_visible_samples_passed",contract)
        self.assertNotIn("glFinish(",gpu)

    def test_camera_zoom_scroll_raycast_streaming_and_f9_shortcut(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("glfwSetScrollCallback(window,scroll_callback)",app)
        self.assertIn("hit_camera_zoom(",app)
        self.assertIn("render_camera_from_agent(render_agent,camera_zoom_distance,camera_orbit_degrees)",app)
        self.assertIn("render_camera_from_agent(agent,camera_zoom_distance,camera_orbit_degrees)",app)
        self.assertIn("camera_zoom_label(camera_distance)",app)
        self.assertIn("input_context.camera_zoom_distance=&camera_zoom_distance",app)
        self.assertIn("key==GLFW_KEY_B&&context->outfit_bake_preview_open",app)

    def test_third_person_camera_is_sanitized_and_visible_in_hud(self):
        app=(ROOT/"src/app/application.cpp").read_text()
        self.assertIn("render::third_person_camera(",app)
        self.assertIn("camera_fallback_this_frame=",app)
        self.assertIn("scene_proof.camera_fallback=camera_fallback_this_frame",app)
        self.assertIn("render_camera_from_agent(render_agent,camera_zoom_distance,camera_orbit_degrees)",app)

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
