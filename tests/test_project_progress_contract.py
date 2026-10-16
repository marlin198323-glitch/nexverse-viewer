"""Prevent a new chat from receiving inconsistent OGL % status values."""
import json
from pathlib import Path
import runpy
import unittest

ROOT=Path(__file__).resolve().parents[1]
PROGRESS=ROOT/"docs/PROJECT_PROGRESS.json"

class ProjectProgressContractTests(unittest.TestCase):
    def test_weighted_percentage_and_version_are_consistent(self):
        data,raw,pct=runpy.run_path(str(ROOT/"scripts/project-progress.py"))["load_progress"]()
        self.assertEqual(sum(area["weight_pct"] for area in data["areas"]),100)
        self.assertTrue(all(0<=a["completion_pct"]<=100 for a in data["areas"]))
        self.assertEqual(data["viewer_version"],(ROOT/"VERSION").read_text().strip())
        self.assertEqual(int(raw+__import__("decimal").Decimal("0.5")),pct)

    def test_every_new_chat_sees_same_status(self):
        data=json.loads(PROGRESS.read_text())
        _,_,pct=runpy.run_path(str(ROOT/"scripts/project-progress.py"))["load_progress"]()
        for path in ("CHATUEBERGABE.md","AGENTS.md","projectplan.md","info.md",
                     "docs/PROJEKTFORTSCHRITT.md"):
            text=(ROOT/path).read_text()
            with self.subTest(document=path):
                self.assertIn(f"{pct} %",text[:1900])
        self.assertTrue(data["next_five_blocks"] in
                        (ROOT/"CHATUEBERGABE.md").read_text()[:3800])

    def test_ten_actual_blocks_per_new_chat_is_persistent(self):
        data=json.loads(PROGRESS.read_text())
        self.assertEqual(data["default_blocks_per_run"],10)
        self.assertEqual(data["next_batch"],data["next_five_blocks"])
        self.assertIn("ZEHN-BLÖCKE-REGEL",
                      (ROOT/"AGENTS.md").read_text()[:1000])
        self.assertIn("10 ECHTE ENTWICKLUNGSBLÖCKE",
                      (ROOT/"CHATUEBERGABE.md").read_text()[:700])

    def test_progress_is_not_derived_from_ci_or_version_numbers(self):
        script=(ROOT/"scripts/project-progress.py").read_text()
        self.assertIn('part["weight_pct"]',script)
        self.assertIn('part["completion_pct"]',script)
        self.assertNotIn("subprocess",script)
        self.assertNotIn("git log",script)
        self.assertIn("Firestorm", (ROOT/"docs/PROJEKTFORTSCHRITT.md").read_text())

if __name__=="__main__":
    unittest.main()
