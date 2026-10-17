#!/usr/bin/env python3
"""Tests the exact Cinnamon taskbar applet settings contract."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
PINNER = ROOT / "scripts/pin-mint-cinnamon-launcher.py"
NAME = "opengenesislink-viewer.desktop"


class CinnamonPinTests(unittest.TestCase):
    def test_dry_run_and_existing_apps_preserved(self):
        with tempfile.TemporaryDirectory() as d:
            directory = Path(d)
            config = directory/"2.json"
            previous = ["nemo.desktop", "firestorm-viewer.desktop", "firefox.desktop"]
            config.write_text(json.dumps({"pinned-apps": {"type": "generic", "value": previous}}))
            before = config.read_bytes()
            def run(*args):
                return subprocess.run([sys.executable, str(PINNER), "--config-root",
                                       str(directory), *args], text=True, capture_output=True,
                                      check=True).stdout
            self.assertIn("WOULD_PIN", run())
            self.assertEqual(before, config.read_bytes())
            self.assertIn("PINNED", run("--apply"))
            new = json.loads(config.read_text())["pinned-apps"]["value"]
            self.assertEqual(new, previous + [NAME])
            self.assertIn("ALREADY_PINNED", run("--apply"))
            self.assertEqual(new, json.loads(config.read_text())["pinned-apps"]["value"])

    def test_missing_and_wrong_config_is_safe(self):
        with tempfile.TemporaryDirectory() as d:
            directory = Path(d)
            result = subprocess.run([sys.executable, str(PINNER), "--config-root",
                                     d, "--apply"], capture_output=True, text=True, check=True)
            self.assertIn("NOT_FOUND", result.stdout)
            p = directory/"1.json"
            p.write_text('{"other":true}')
            original = p.read_bytes()
            result = subprocess.run([sys.executable, str(PINNER), "--config-root",
                                     d, "--apply"], capture_output=True, text=True, check=True)
            self.assertIn("SKIP_NO_PINNED_APPS", result.stdout)
            self.assertEqual(p.read_bytes(), original)

    def test_desktop_entry_matches_glfw_window_class(self):
        entry = (ROOT/"packaging/linux/opengenesislink-viewer.desktop").read_text()
        source = (ROOT/"src/app/application.cpp").read_text()
        self.assertIn("StartupWMClass=opengenesislink-viewer", entry)
        self.assertIn("Icon=opengenesislink-viewer", entry)
        self.assertIn('glfwWindowHintString(GLFW_X11_CLASS_NAME,"opengenesislink-viewer")', source)
        self.assertIn('glfwSetWindowIcon(window,1,&icon)', source)


if __name__ == "__main__":
    unittest.main()
