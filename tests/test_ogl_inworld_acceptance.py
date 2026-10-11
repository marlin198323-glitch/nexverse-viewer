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

    def test_consecutive_live_like_fixture(self):
        report=report_from([snapshot(i) for i in (1,2,3)])
        self.assertEqual(report["latest_epoch_snapshots"], 3)
        self.assertNotIn("secret", str(report))
        result=assess(report, now_unix_ms=NOW)
        self.assertEqual(result["status"], "evidence_pass")
        self.assertTrue(all(result["checks"].values()))

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

    def test_stale_and_missing_mesh(self):
        report=report_from([snapshot(i) for i in (1,2,3)])
        self.assertEqual(
            assess(report, now_unix_ms=NOW+180_001)["status"],
            "not_verified")
        missing=report_from([snapshot(i,mesh_draws=0) for i in (1,2,3)])
        result=assess(missing,now_unix_ms=NOW)
        self.assertFalse(result["checks"]["actual_uploaded_mesh_draw_calls"])

if __name__=="__main__":
    unittest.main()
