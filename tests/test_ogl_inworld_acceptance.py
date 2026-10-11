import tempfile
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
from ogl_inworld_acceptance import assess
from ogl_asset_log_report import read_report


class InWorldAcceptanceTests(unittest.TestCase):
    def test_no_false_success_without_live_snapshots(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary) / "viewer.log"
            source.write_text("2026-10-11 11:00:00 Viewer started\n")
            result = assess(read_report(source))
            self.assertEqual(result["status"], "not_verified")
            self.assertFalse(result["checks"]["multiple_live_snapshots"])

    def test_different_epochs_cannot_be_combined(self):
        with tempfile.TemporaryDirectory() as temporary:
            source=Path(temporary)/"viewer.log"
            data=('{"schema":"ogl.asset.streaming.v1",'
                  '"region_epoch":19,"textures_wanted":10,'
                  '"meshes_wanted":3,"texture_coverage_pct":100,'
                  '"mesh_coverage_pct":100,'
                  '"previous_frame_scene_draw_calls":2,'
                  '"previous_frame_scene_mesh_draw_calls":2}')
            source.write_text("".join(
                "2026-10-11 11:00:00 [INFO] InWorldAssetSnapshot " +
                data.replace('"region_epoch":19',
                    '"region_epoch":18' if i < 2 else '"region_epoch":19') +
                "\n" for i in range(3)))
            result=assess(read_report(source))
            self.assertEqual(result["status"], "not_verified")
            self.assertEqual(
                result["observations"]["consecutive_snapshots_in_latest_region"],1)

    def test_synthetic_fixtures_verify_gate_logic_only(self):
        # Synthetic snapshots are deliberately NOT an actual in-world test.
        good = ('{"schema":"ogl.asset.streaming.v1",'
                '"region_epoch":17,"textures_wanted":10,"meshes_wanted":5,'
                '"texture_coverage_pct":90,"mesh_coverage_pct":80,'
                '"previous_frame_scene_draw_calls":15,'
                '"previous_frame_scene_mesh_draw_calls":10}')
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary) / "viewer.log"
            source.write_text("".join(
                "2026-10-11 11:00:00 [INFO] InWorldAssetSnapshot " +
                good + "\n" for _ in range(3)))
            report = read_report(source)
            result = assess(report)
            self.assertEqual(result["status"], "evidence_pass")
            self.assertTrue(all(result["checks"].values()))
            source.write_text("2026-10-11 11:00:00 [INFO] "
                "InWorldAssetSnapshot " + good.replace(
                    '"previous_frame_scene_mesh_draw_calls":10',
                    '"previous_frame_scene_mesh_draw_calls":0') + "\n")
            result = assess(read_report(source), min_snapshots=1)
            self.assertEqual(result["status"], "not_verified")
            self.assertFalse(
                result["checks"]["actual_uploaded_mesh_draw_calls"])


if __name__ == "__main__":
    unittest.main()
