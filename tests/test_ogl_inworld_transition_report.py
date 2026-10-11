import json
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent.parent/"tools"))
from ogl_asset_log_report import read_report
from ogl_inworld_acceptance import assess

NOW=1_820_000_000_000
START=NOW-240_000

def sample(sequence,epoch=2,session=START,coverage=95,
           missing=0,mesh_draws=4,include_missing=True):
    content={
        "schema":"ogl.asset.streaming.v1",
        "region_epoch":epoch,
        "session_started_unix_ms":session,
        "emitted_unix_ms":NOW-90_000+sequence*30_000,
        "sequence":sequence,
        "textures_wanted":10,
        "meshes_wanted":10,
        "texture_coverage_pct":coverage,
        "mesh_coverage_pct":coverage,
        "previous_frame_scene_draw_calls":7,
        "previous_frame_scene_mesh_draw_calls":mesh_draws,
        "previous_frame_scene_missing_mesh_draw_calls":missing,
        "secret":"https://private.example/caps-token"}
    if not include_missing:
        content.pop("previous_frame_scene_missing_mesh_draw_calls")
    return json.dumps(content)

def report(samples):
    with tempfile.TemporaryDirectory() as t:
        path=Path(t)/"viewer.log"
        path.write_text("".join(
            "2026-10-11 17:40:00 [INFO] InWorldAssetSnapshot "+s+"\n"
            for s in samples))
        return read_report(path)

class RegionReportTests(unittest.TestCase):
    def test_healthy_series(self):
        data=report([sample(i) for i in (1,2,3)])
        self.assertEqual(data["latest_epoch_snapshots"],3)
        self.assertEqual(data["latest_region_min_mesh_coverage_pct"],95)
        self.assertEqual(assess(data,now_unix_ms=NOW)["status"],
                         "evidence_pass")
        self.assertNotIn("caps-token",str(data))

    def test_weak_middle_frame_fails_even_if_latest_recovers(self):
        snapshots=[sample(1),sample(2,coverage=20,mesh_draws=0),
                   sample(3)]
        data=report(snapshots)
        self.assertEqual(data["latest_region_min_mesh_coverage_pct"],20)
        self.assertEqual(data["latest_region_min_mesh_draw_calls"],0)
        outcome=assess(data,now_unix_ms=NOW)
        self.assertEqual(outcome["status"],"not_verified")
        self.assertFalse(outcome["checks"]["mesh_coverage"])
        self.assertFalse(outcome["checks"]["actual_uploaded_mesh_draw_calls"])

    def test_earlier_missing_mesh_then_healthy_is_insufficient(self):
        snapshots=[sample(1,missing=1),sample(2),sample(3)]
        data=report(snapshots)
        self.assertEqual(data["latest_region_max_missing_mesh_fallback_draws"],1)
        self.assertEqual(assess(data,now_unix_ms=NOW)["status"],
                         "not_verified")

    def test_missing_telemetry_field_never_counts_as_zero(self):
        data=report([sample(1),sample(2,include_missing=False),
                     sample(3)])
        self.assertIsNone(data["latest_region_max_missing_mesh_fallback_draws"])
        self.assertEqual(assess(data,now_unix_ms=NOW)["status"],
                         "not_verified")

    def test_new_session_requires_own_three_samples(self):
        data=report([sample(1,session=START-90_000,epoch=1),
                     sample(2,session=START-90_000,epoch=1),
                     sample(3,session=START,epoch=2)])
        self.assertEqual(data["latest_epoch_snapshots"],1)
        self.assertEqual(assess(data,now_unix_ms=NOW)["status"],
                         "not_verified")

if __name__=="__main__":
    unittest.main()
