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
