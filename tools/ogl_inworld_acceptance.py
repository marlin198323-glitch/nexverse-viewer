#!/usr/bin/env python3
"""Check a REAL viewer log against the in-world acceptance gate.

Exit 0: sufficient observed in-world coverage/draw evidence.
Exit 2: insufficient or missing evidence.
Never interprets synthetic GL test output as a real OpenSim login.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
from ogl_asset_log_report import read_report


def assess(report: dict, min_snapshots: int = 3,
           min_coverage_pct: int = 75) -> dict:
    counts = report.get("snapshots_valid", 0)
    consecutive = report.get("latest_epoch_snapshots", 0)
    latest = report.get("latest_snapshot") or {}
    observations = {
        "snapshots": counts,
        "consecutive_snapshots_in_latest_region": consecutive,
        "last_region_epoch": latest.get("region_epoch"),
        "last_textures_wanted": latest.get("textures_wanted"),
        "last_meshes_wanted": latest.get("meshes_wanted"),
        "last_scene_draw_calls": latest.get("previous_frame_scene_draw_calls"),
        "last_mesh_draw_calls": latest.get(
            "previous_frame_scene_mesh_draw_calls"),
    }
    checks = {
        "multiple_live_snapshots": consecutive >= min_snapshots,
        "active_region_epoch": latest.get("region_epoch", 0) > 0,
        "nonempty_texture_scene": latest.get("textures_wanted", 0) > 0,
        "nonempty_mesh_scene": latest.get("meshes_wanted", 0) > 0,
        "texture_coverage": latest.get("texture_coverage_pct", 0)
            >= min_coverage_pct,
        "mesh_coverage": latest.get("mesh_coverage_pct", 0)
            >= min_coverage_pct,
        "actual_scene_draw_calls": latest.get(
            "previous_frame_scene_draw_calls", 0) > 0,
        "actual_uploaded_mesh_draw_calls": latest.get(
            "previous_frame_scene_mesh_draw_calls", 0) > 0,
    }
    passing = all(checks.values())
    return {
        "schema": "ogl.inworld.acceptance.v1",
        "status": "evidence_pass" if passing else "not_verified",
        "checks": checks,
        "observations": observations,
        "caveat": "Asset residency and OpenGL draw evidence do not establish "
                  "avatar movement, EventQueue, teleport or visual correctness.",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("logfile", type=Path)
    parser.add_argument("--min-snapshots", type=int, default=3)
    parser.add_argument("--min-coverage", type=int, default=75)
    args = parser.parse_args()
    if args.min_snapshots < 1 or not 0 <= args.min_coverage <= 100:
        parser.error("Invalid acceptance thresholds")
    if not args.logfile.is_file():
        parser.error("Viewer log not found")
    result = assess(read_report(args.logfile),
                    args.min_snapshots, args.min_coverage)
    print(json.dumps(result, sort_keys=True, indent=2))
    return 0 if result["status"] == "evidence_pass" else 2


if __name__ == "__main__":
    raise SystemExit(main())
