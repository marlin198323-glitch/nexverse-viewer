"""Read-only viewer login diagnostics must not expose raw XML-RPC messages."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "scripts/ogl-login-diagnostics.py"
spec = importlib.util.spec_from_file_location("ogl_login_triage", SCRIPT)
triage = importlib.util.module_from_spec(spec)
spec.loader.exec_module(triage)

class PresenceLoginTriageTests(unittest.TestCase):
    def test_last_session_filter(self):
        text = (
            "2026-10-30 21:00:00 [INFO] Starting OpenGenesisLINK Viewer v0.4\n"
            "2026-10-30 21:01:00 [WARN] OpenSim login failed, reason: presence\n"
            "2026-10-30 21:10:00 [INFO] Starting OpenGenesisLINK Viewer v0.4\n"
            "2026-10-30 21:12:00 [WARN] OpenSim login failed, category: presence_already_online\n"
        )
        entries, counter = triage.summarize(text)
        self.assertEqual(entries, [("2026-10-30 21:12:00", "presence_already_online")])
        self.assertEqual(counter["presence_already_online"], 1)
        entries, _ = triage.summarize(text, latest_session=False)
        self.assertEqual(len(entries), 2)

    def test_legacy_and_new_log_formats(self):
        sample = (
            "2026-10-30 21:33:05 [WARN] OpenSim login failed, reason: presence\n"
            "2026-10-30 21:40:05 [WARN] OpenSim login failed, category: presence_restricted\n"
        )
        entries, c = triage.summarize(sample)
        self.assertEqual([v for _, v in entries], ["presence", "presence_restricted"])
        self.assertEqual(c["presence"], 1)

    def test_hostile_reason_is_not_echoed(self):
        evil = (
            "2026-10-30 21:33:05 [WARN] OpenSim login failed, reason: "
            "https://evil.example/?password=secret\n"
        )
        entries, _ = triage.summarize(evil)
        self.assertEqual(entries, [("2026-10-30 21:33:05", "other")])

    def test_log_raw_body_does_not_show_up_in_summary(self):
        payload = (
            "2026-10-30 21:33:05 [WARN] OpenSim login failed, category: presence_unknown\n"
            "2026-10-30 21:33:06 [INFO] credential=supersecret\n"
        )
        entries, c = triage.summarize(payload)
        self.assertEqual(entries, [("2026-10-30 21:33:05", "presence_unknown")])
        self.assertNotIn("supersecret", str(entries) + str(c))

    def test_invalid_log_line_ignored(self):
        entries, _ = triage.summarize(
            "OpenSim login failed, category: presence_unknown\n"
            "2026-10-30 21:33:05 [WARN] random text\n"
        )
        self.assertEqual(entries, [])

    def test_cli_prints_only_safe_categories(self):
        with tempfile.TemporaryDirectory() as td:
            log = Path(td) / "viewer.log"
            log.write_text(
                "2026-10-30 21:33:00 [INFO] Starting OpenGenesisLINK Viewer\n"
                "2026-10-30 21:33:05 [WARN] OpenSim login failed, reason: presence\n"
                "2026-10-30 21:33:06 [INFO] password=DO_NOT_ECHO\n"
            )
            proc = subprocess.run(
                [sys.executable, str(SCRIPT), "--log", str(log)],
                text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=5, check=False
            )
            self.assertEqual(proc.returncode, 0, proc.stderr)
            self.assertIn("presence: 1", proc.stdout)
            self.assertIn("ROBUST-Loginlogs", proc.stdout)
            self.assertNotIn("DO_NOT_ECHO", proc.stdout)
            self.assertNotIn(str(log), proc.stdout)

    def test_cli_missing_file_no_path_leak(self):
        with tempfile.TemporaryDirectory() as td:
            path = Path(td) / "password=DO_NOT_LEAK.log"
            proc = subprocess.run(
                [sys.executable, str(SCRIPT), "--log", str(path)],
                text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=5, check=False
            )
            self.assertEqual(proc.returncode, 2)
            self.assertNotIn("DO_NOT_LEAK", proc.stdout + proc.stderr)


if __name__ == "__main__":
    unittest.main()
