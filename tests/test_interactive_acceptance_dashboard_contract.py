"""Interactive OGL manual acceptance is useful but never a false release gate."""
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
SRC=ROOT/"scripts/ogl-acceptance.py"

class AcceptanceDashboardContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        spec=importlib.util.spec_from_file_location("ogl_acceptance",SRC)
        cls.m=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.m)
        cls.snap=cls.m.snapshot()

    def run_cli(self,*args):
        return subprocess.run([sys.executable,str(SRC),*args],
           cwd=ROOT,capture_output=True,text=True,check=False)

    def check_report(self,reviews,**overrides):
        data={
            "schema_version":1,"project":"OpenGenesisLINK Viewer",
            "version":self.snap["version"],
            "snapshot_sha":self.snap["snapshot_sha"],
            "release_authorized":False,"local_review_only":True,
            "reviews":reviews,**overrides}
        with tempfile.TemporaryDirectory() as tmp:
            f=Path(tmp)/"report.json"
            f.write_text(json.dumps(data),encoding="utf-8")
            return self.run_cli("--validate",str(f))

    def test_build_is_local_offline_html(self):
        result=self.run_cli("--build")
        self.assertEqual(result.returncode,0,result.stderr)
        page=self.m.OUTPUT.read_text(encoding="utf-8")
        self.assertIn('<html lang="de">',page)
        self.assertIn("Interaktive Projektabnahme",page)
        self.assertIn(self.snap["snapshot_sha"],page)
        self.assertIn("OGL_ACCEPTANCE_BROWSER_SELFTEST_PASS",page)
        self.assertNotIn("/*__OGL_ACCEPTANCE_SNAPSHOT__*/",page)
        self.assertNotIn("<script src=",page)

    def test_12_versions_34_checks_7_release_proofs(self):
        self.assertEqual(len(self.snap["milestones"]),12)
        self.assertEqual(sum(len(m["acceptance"]) for m in self.snap["milestones"]),34)
        self.assertEqual(len(self.snap["release_evidence"]),7)
        self.assertEqual(self.snap["version"],"v0.4.0.0")

    def test_local_review_cannot_flip_official_release(self):
        self.assertFalse(self.snap["release_eligible"])
        self.assertFalse(self.snap["published_release"])
        self.assertEqual(self.check_report({}).returncode,0)
        self.assertEqual(self.run_cli("--build").returncode,0)
        self.assertEqual(self.run_cli("--validate","/dev/null").returncode,2)

    def test_accepts_valid_manual_evidence_but_not_publishes(self):
        r={"v0.4.0.0-A01":{"status":"BESTANDEN",
            "evidence":"logs/real-grid-session-e2e.txt",
            "tested_at":"2026-10-24T09:45:00Z","reviewer":"QA","notes":"manual claim"}}
        out=self.check_report(r)
        self.assertEqual(out.returncode,0,out.stderr)
        self.assertIn("DO NOT CHANGE ROADMAP OR RELEASE STATUS",out.stdout)

    def test_rejects_false_pass_without_evidence(self):
        r={"v0.4.0.0-A01":{"status":"BESTANDEN",
           "evidence":"","tested_at":"2026-10-24T09:45:00Z"}}
        self.assertEqual(self.check_report(r).returncode,2)

    def test_rejects_unknown_id_and_invalid_status(self):
        self.assertEqual(self.check_report({"unknown":{"status":"BLOCKIERT"}}).returncode,2)
        self.assertEqual(self.check_report({"v0.4.0.0-A01":{"status":"AUTORELEASE"}}).returncode,2)

    def test_rejects_forged_release_permission(self):
        self.assertEqual(self.check_report({},release_authorized=True).returncode,2)
        self.assertEqual(self.check_report({},local_review_only=False).returncode,2)

    def test_rejects_stale_and_other_project_snapshots(self):
        self.assertEqual(self.check_report({},snapshot_sha="0"*40).returncode,2)
        self.assertEqual(self.check_report({},version="v1.0.0.0").returncode,2)
        self.assertEqual(self.check_report({},project="FakeViewer").returncode,2)

    def test_text_is_dom_safe_and_export_import_do_not_modify_code(self):
        src=self.m.TEMPLATE.read_text(encoding="utf-8")
        for name in ("textContent","Object.entries(reviews)","localStorage",
                     "URL.createObjectURL","file.text()","release_authorized:false",
                     'id="version"','id="upload"','id="evidenceList"'):
            self.assertIn(name,src)
        self.assertNotIn("eval(",src)
        self.assertNotIn("document.write(",src)
        self.assertNotIn("innerHTML=",src)

    def test_incomplete_manual_pass_remains_visible_until_evidence_added(self):
        src=self.m.TEMPLATE.read_text(encoding="utf-8")
        self.assertIn("function rawReview(id)",src)
        self.assertIn("const old=rawReview(id)",src)
        self.assertIn("const v=rawReview(item.id)",src)
        self.assertIn('const incomplete=rawReview(first).status==="BESTANDEN"&&!safeReview(first)',src)
        self.assertIn('reviewEdit(first,"evidence","smoke-tests/chromium-ui-evidence.log")',src)

    def test_beginner_guided_acceptance_has_eight_real_viewer_tasks(self):
        guided=self.snap["guided"]
        self.assertEqual(guided["version"],"v0.4.0.0")
        self.assertEqual(len(guided["steps"]),8)
        ids=[step["id"] for step in guided["steps"]]
        self.assertEqual(len(ids),len(set(ids)))
        for step in guided["steps"]:
            self.assertGreaterEqual(len(step["instructions"]),2)
            self.assertTrue(step["expected"].strip())
            self.assertIn(step["related_roadmap_id"],
                {item["id"] for stage in self.snap["milestones"]
                 for kind in ("tasks","acceptance") for item in stage[kind]})
        self.assertIn("walk",ids)
        self.assertIn("teleport",ids)
        self.assertIn("login",ids)

    def test_guided_results_accept_user_observation_without_release_approval(self):
        out=self.check_report({},guided_results={"login":{
            "status":"FUNKTIONIERT",
            "notes":"Angemeldet, 3D-Welt ist sichtbar",
            "tested_at":"2026-10-24T15:01:00Z"
        }})
        self.assertEqual(out.returncode,0,out.stderr)
        self.assertIn("1 guided results",out.stdout)
        self.assertFalse(self.snap["release_eligible"])

    def test_guided_results_block_unknown_status_id_and_false_attestation(self):
        cases=[
            {"no-real-step":{"status":"FEHLER","notes":"error","tested_at":"2026-10-24T15:01:00Z"}},
            {"login":{"status":"RELEASED","notes":"","tested_at":"2026-10-24T15:01:00Z"}},
            {"login":{"status":"FUNKTIONIERT","notes":"","tested_at":""}},
            {"login":{"status":"FEHLER","notes":123,"tested_at":"2026-10-24T15:01:00Z"}}
        ]
        for entries in cases:
            with self.subTest(entries=entries):
                self.assertEqual(self.check_report({},guided_results=entries).returncode,2)

    def test_guided_steps_ui_has_clear_buttons_next_back_and_expected_result(self):
        src=self.m.TEMPLATE.read_text(encoding="utf-8")
        for token in ('id="guidedInstructions"','id="guidedExpected"',
            'id="guidedNext"','id="guidedPrevious"',
            'data-result="FUNKTIONIERT"','data-result="FEHLER"',
            'data-result="NICHT_MOEGLICH"',
            'function renderGuided()','function setGuidedResult(status)',
            "guided_results:Object.fromEntries","const guidedIncoming=Object.create(null)",
            'const guidedPass=guidedResults[guidedSteps[0].id].status==="FUNKTIONIERT"'):
            self.assertIn(token,src)
        self.assertNotIn("fetch(",src)
        self.assertNotIn("innerHTML=",src)

    def test_no_release_from_dashboard_even_when_user_marks_all_passed(self):
        src=self.m.TEMPLATE.read_text(encoding="utf-8")
        self.assertIn("!SNAPSHOT.release_eligible",src)
        self.assertIn("Manuelle",src)
        self.assertFalse(any(x["verified"] for x in self.snap["release_evidence"]))
        result=subprocess.run([sys.executable,str(ROOT/"scripts/ogl-version.py"),
            "--release-check","v1.0.0.0"],cwd=ROOT,capture_output=True,text=True)
        self.assertEqual(result.returncode,2)

if __name__=="__main__":unittest.main()
