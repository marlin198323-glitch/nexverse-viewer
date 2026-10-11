import tempfile
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
from ogl_asset_log_report import read_report


class AssetLogReportTest(unittest.TestCase):
    def test_legacy_and_snapshot(self):
        with tempfile.TemporaryDirectory() as temp:
            logfile = Path(temp) / "viewer.log"
            logfile.write_text(
                '2026-10-10 12:00:00 [WARNING] GetMesh failed for '
                'aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa: HTTP 404\n'
                '2026-10-10 12:00:01 [WARNING] GetTexture failed for '
                'bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb: '
                'failure=transient; retry_delay_ms=2000\n'
                '2026-10-11 11:00:00 [INFO] InWorldAssetSnapshot '
                '{"schema":"ogl.asset.streaming.v1",'
                '"texture_coverage_pct":50,"mesh_coverage_pct":25,'
                '"region_epoch":12,"secret":"caps://sensitive",'
                '"textures_pending":3,"gpu_upload_cpu_p95_us":4900,'
                '"texture_priority_churn":4}\n'
                '2026-10-11 11:00:02 [INFO] InWorldAssetSnapshot broken\n'
                '2026-10-11 11:00:03 [INFO] GetMesh streaming metrics: ram_hits=1\n'
            )
            summary = read_report(logfile)
            self.assertEqual(summary["event_counts"]["mesh_failures"], 1)
            self.assertEqual(summary["event_counts"]["texture_failures"], 1)
            self.assertEqual(summary["failure_categories"]["legacy_http_404"], 1)
            self.assertEqual(summary["failure_categories"]["transient"], 1)
            self.assertEqual(summary["snapshots_valid"], 1)
            self.assertEqual(summary["snapshots_malformed"], 1)
            self.assertEqual(summary["lowest_mesh_coverage_pct"], 25)
            self.assertEqual(summary["latest_snapshot"]["gpu_upload_cpu_p95_us"], 4900)
            self.assertEqual(summary["latest_snapshot"]["texture_priority_churn"], 4)
            self.assertEqual(summary["peak_gpu_upload_cpu_p95_us"], 4900)
            self.assertEqual(summary["peak_texture_priority_churn"], 4)
            self.assertIsNone(summary["peak_mesh_priority_churn"])
            self.assertEqual(summary["date_last"], "2026-10-11")
            self.assertNotIn("secret", str(summary))
            self.assertNotIn("aaaaaaaa-", str(summary))

    def test_empty_log(self):
        with tempfile.TemporaryDirectory() as temp:
            log = Path(temp) / "empty.log"
            log.write_text("")
            report = read_report(log)
            self.assertIsNone(report["latest_snapshot"])
            self.assertIsNone(report["date_first"])
            self.assertEqual(report["failure_categories"], {})


if __name__ == "__main__":
    unittest.main()
