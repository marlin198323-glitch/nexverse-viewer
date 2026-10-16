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
    def test_host_instructions_keep_pi_runner_separate(self):
        agents=(ROOT/"AGENTS.md").read_text()
        self.assertIn("jam-MS-7D28",agents)
        self.assertIn("nur als Self-hosted-CI",agents)
        self.assertIn("develop-current",agents)

if __name__=="__main__":
    unittest.main()
