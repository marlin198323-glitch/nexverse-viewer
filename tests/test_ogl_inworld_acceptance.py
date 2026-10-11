import json
import tempfile
import unittest
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
from ogl_inworld_acceptance import assess
from ogl_asset_log_report import read_report

NOW = 1_800_000_000_000
SESSION = NOW - 300_000

def snapshot(seq: int, epoch: int = 17, session: int = SESSION,
             issued: int | None = None, mesh_draws: int = 8) -> str:
    if issued is None:
        issued = NOW - (4-seq)*30_000
    return json.dumps({
        "schema": "ogl.asset.streaming.v1",
        "session_started_unix_ms": session,
        "emitted_unix_ms": issued,
        "sequence": seq,
        "region_epoch": epoch,
        "textures_wanted": 10,
        "meshes_wanted": 5,
        "texture_coverage_pct": 90,
        "mesh_coverage_pct": 80,
        "previous_frame_scene_draw_calls": 12,
        "previous_frame_scene_mesh_draw_calls": mesh_draws,
        "previous_frame_scene_missing_mesh_draw_calls": 0,
        "previous_frame_scene_missing_mesh_objects": 0,
        "secret": "https://private.caps.example",
    })

def report_from(samples: list[str]) -> dict:
    with tempfile.TemporaryDirectory() as temporary:
        path = Path(temporary)/"viewer.log"
        path.write_text("".join(
            "2026-10-11 12:00:00 [INFO] InWorldAssetSnapshot " + s + "\n"
            for s in samples))
        return read_report(path)

class InWorldAcceptanceTests(unittest.TestCase):
    def test_no_false_success_without_live_snapshots(self):
        result = assess(report_from([]), now_unix_ms=NOW)
        self.assertEqual(result["status"], "not_verified")
        self.assertFalse(result["checks"]["recent_snapshot"])
        self.assertEqual(result["failure_codes"],
                         ["no_current_region_snapshots"])
        self.assertIn("Log into a simulator region",result["next_check"])

    def test_consecutive_live_like_fixture(self):
        report=report_from([snapshot(i) for i in (1,2,3)])
        self.assertEqual(report["latest_epoch_snapshots"], 3)
        self.assertNotIn("secret", str(report))
        result=assess(report, now_unix_ms=NOW)
        self.assertEqual(result["status"], "evidence_pass")
        self.assertTrue(all(result["checks"].values()))
        self.assertEqual(result["failure_codes"],[])
        self.assertIn("No failed",result["next_check"])

    def test_warmup_does_not_permanently_fail_new_steady_state(self):
        import json
        samples=[]
        for index in range(1,6):
            record=json.loads(snapshot(index,issued=NOW-(6-index)*30_000))
            if index<=2:
                record["texture_coverage_pct"]=2
                record["mesh_coverage_pct"]=0
                record["previous_frame_scene_mesh_draw_calls"]=0
                record["previous_frame_scene_missing_mesh_draw_calls"]=9
            samples.append(json.dumps(record))
        report=report_from(samples)
        self.assertEqual(report["latest_epoch_snapshots"],5)
        self.assertEqual(report["latest_region_min_texture_coverage_pct"],2)
        self.assertEqual(report["latest_region_min_mesh_coverage_pct"],0)
        recent=assess(report,now_unix_ms=NOW)
        self.assertEqual(recent["status"],"evidence_pass")
        strict=assess(report,now_unix_ms=NOW,min_snapshots=5)
        self.assertEqual(strict["status"],"not_verified")
        self.assertIn("texture_residency_incomplete",strict["failure_codes"])
        self.assertNotIn("secret",str(report["latest_region_recent_samples"]))

    def test_session_epoch_and_sequence_boundaries(self):
        for samples in (
            [snapshot(1), snapshot(2), snapshot(3, epoch=18)],
            [snapshot(1), snapshot(2), snapshot(3,session=SESSION+1000)],
            [snapshot(1), snapshot(2), snapshot(6)],
            [snapshot(1), snapshot(2), snapshot(3,issued=NOW+40_000)],
            [snapshot(1), snapshot(2), snapshot(3,issued=NOW-51_000)],
        ):
            with self.subTest(samples=samples[-1][:120]):
                report=report_from(samples)
                self.assertLess(report["latest_epoch_snapshots"], 3)
                self.assertEqual(
                    assess(report, now_unix_ms=NOW)["status"],
                    "not_verified")

    def test_missing_mesh_fallback_requires_explicit_clean_frame(self):
        entries=[snapshot(i) for i in (1,2,3)]
        entries[-1]=entries[-1].replace(
            '"previous_frame_scene_missing_mesh_draw_calls": 0',
            '"previous_frame_scene_missing_mesh_draw_calls": 3')
        result=assess(report_from(entries),now_unix_ms=NOW)
        self.assertEqual(result["status"],"not_verified")
        self.assertFalse(result["checks"]["missing_mesh_fallback_within_limit"])
        self.assertIn("mesh_fallback_still_drawn",result["failure_codes"])
        relaxed=assess(report_from(entries),now_unix_ms=NOW,
                      max_missing_mesh_draws=3)
        self.assertEqual(relaxed["status"],"evidence_pass")

    def test_stale_and_missing_mesh(self):
        report=report_from([snapshot(i) for i in (1,2,3)])
        self.assertEqual(
            assess(report, now_unix_ms=NOW+180_001)["status"],
            "not_verified")
        missing=report_from([snapshot(i,mesh_draws=0) for i in (1,2,3)])
        result=assess(missing,now_unix_ms=NOW)
        self.assertFalse(result["checks"]["actual_uploaded_mesh_draw_calls"])
        self.assertIn("gpu_mesh_draws_missing",result["failure_codes"])
        self.assertNotIn("private.caps.example",str(result))

if __name__=="__main__":
    unittest.main()
