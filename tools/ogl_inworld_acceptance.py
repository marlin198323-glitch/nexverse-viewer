#!/usr/bin/env python3
"""Validate recent real-world rendering evidence from OpenGenesisLINK logs.

Exit 0 = sufficient recent session-specific asset/draw evidence.
Exit 2 = absent/stale/inconclusive evidence. This does NOT test movement,
avatar appearance, visual fidelity, teleport or EventQueue.
"""
from __future__ import annotations
import argparse
import json
import time
from pathlib import Path
from ogl_asset_log_report import read_report


def assess(report: dict, min_snapshots: int = 3,
           min_coverage_pct: int = 75,
           now_unix_ms: int | None = None,
           max_age_ms: int = 180_000,
           max_missing_mesh_draws: int = 0) -> dict:
    if now_unix_ms is None:
        now_unix_ms = time.time_ns() // 1_000_000
    latest = report.get("latest_snapshot") or {}
    count = report.get("latest_epoch_snapshots", 0)
    emitted = latest.get("emitted_unix_ms", 0)
    session = latest.get("session_started_unix_ms", 0)
    age = now_unix_ms - emitted if emitted else None
    # Require healthy evidence throughout the latest consecutive window,
    # not only in the most recent frame after earlier missing textures.
    window_texture=report.get("latest_region_min_texture_coverage_pct")
    window_mesh=report.get("latest_region_min_mesh_coverage_pct")
    window_draws=report.get("latest_region_min_mesh_draw_calls")
    window_missing=report.get("latest_region_max_missing_mesh_fallback_draws")
    checks = {
        "session_and_epoch": (latest.get("region_epoch", 0) > 0 and
                              session > 0 and emitted >= session and
                              latest.get("sequence", 0) > 0),
        "recent_snapshot": age is not None and
                           -10_000 <= age <= max_age_ms,
        "consecutive_timed_snapshots": count >= min_snapshots,
        "nonempty_texture_scene": latest.get("textures_wanted", 0) > 0,
        "nonempty_mesh_scene": latest.get("meshes_wanted", 0) > 0,
        "texture_coverage": (window_texture is not None and
                             window_texture>=min_coverage_pct),
        "mesh_coverage": (window_mesh is not None and
                          window_mesh>=min_coverage_pct),
        "actual_scene_draw_calls": latest.get(
            "previous_frame_scene_draw_calls", 0) > 0,
        "actual_uploaded_mesh_draw_calls": (
            window_draws is not None and window_draws>0),
        "missing_mesh_fallback_within_limit": (
            window_missing is not None and
            window_missing<=max_missing_mesh_draws),
    }
    # All codes are hardcoded and safe for support tickets. Never place
    # UUIDs, player data, CAPS URLs, HTTP bodies, or raw log text here.
    codes = {
        "session_and_epoch": "no_valid_region_session",
        "recent_snapshot": "missing_or_stale_snapshot",
        "consecutive_timed_snapshots": "insufficient_region_samples",
        "nonempty_texture_scene": "texture_demand_missing",
        "nonempty_mesh_scene": "mesh_demand_missing",
        "texture_coverage": "texture_residency_incomplete",
        "mesh_coverage": "mesh_residency_incomplete",
        "actual_scene_draw_calls": "scene_draws_missing",
        "actual_uploaded_mesh_draw_calls": "gpu_mesh_draws_missing",
        "missing_mesh_fallback_within_limit": "mesh_fallback_still_drawn",
    }
    failures = ([codes[name] for name,passed in checks.items()
                 if not passed] if latest else ["no_current_region_snapshots"])
    if not failures:
        next_check = "No failed asset/render evidence checks."
    elif not latest:
        next_check = ("Log into a simulator region and capture three "
                      "consecutive InWorldAssetSnapshot records.")
    elif not checks["recent_snapshot"]:
        next_check = ("Capture recent in-world snapshots from the current "
                      "viewer session; old logs cannot prove live rendering.")
    elif not checks["consecutive_timed_snapshots"]:
        next_check = ("Collect at least three uninterrupted 30-second "
                      "snapshots from one session and region epoch.")
    elif not checks["nonempty_mesh_scene"] or not checks["nonempty_texture_scene"]:
        next_check = ("Check whether the region has emitted object, mesh "
                      "and texture demand before evaluating GPU residency.")
    elif not checks["texture_coverage"] or not checks["mesh_coverage"]:
        next_check = ("Inspect sanitized GetMesh/GetTexture failure classes "
                      "and RAM/GPU budget pressure in the active region.")
    elif not checks["actual_uploaded_mesh_draw_calls"] or not checks["actual_scene_draw_calls"]:
        next_check = ("Inspect actual world-object mesh draw counters and "
                      "the renderer's missing GPU mesh fallback counters.")
    else:
        next_check = ("Inspect missing-mesh fallback counts; accepted mesh "
                      "downloads alone do not establish actual mesh draws.")
    return {
        "schema": "ogl.inworld.acceptance.v2",
        "status": "evidence_pass" if all(checks.values()) else "not_verified",
        "failure_codes": failures,
        "next_check": next_check,
        "checks": checks,
        "observations": {
            "snapshots": report.get("snapshots_valid", 0),
            "consecutive_snapshots_in_latest_region": count,
            "last_region_epoch": latest.get("region_epoch"),
            "last_session_unix_ms": latest.get("session_started_unix_ms"),
            "last_sample_unix_ms": latest.get("emitted_unix_ms"),
            "age_ms": age,
            "last_textures_wanted": latest.get("textures_wanted"),
            "last_meshes_wanted": latest.get("meshes_wanted"),
            "last_scene_draw_calls": latest.get(
                "previous_frame_scene_draw_calls"),
            "last_mesh_draw_calls": latest.get(
                "previous_frame_scene_mesh_draw_calls"),
            "region_min_texture_coverage_pct": window_texture,
            "region_min_mesh_coverage_pct": window_mesh,
            "region_min_mesh_draw_calls": window_draws,
            "region_max_missing_mesh_draw_calls": window_missing,
            "region_mesh_missing_events_delta": report.get(
                "latest_region_mesh_missing_events_delta"),
            "region_texture_missing_events_delta": report.get(
                "latest_region_texture_missing_events_delta"),
            "region_peak_missing_mesh_unique": report.get(
                "latest_region_peak_missing_mesh_unique"),
            "region_peak_texture_ram_pressure": report.get(
                "latest_region_peak_texture_ram_pressure"),
            "region_peak_mesh_ram_pressure": report.get(
                "latest_region_peak_mesh_ram_pressure"),
            "region_texture_ram_pressure_samples": report.get(
                "latest_region_texture_ram_pressure_samples"),
            "region_mesh_ram_pressure_samples": report.get(
                "latest_region_mesh_ram_pressure_samples"),
            "region_peak_alpha_draw_calls": report.get(
                "latest_region_peak_alpha_draw_calls"),
            "region_peak_alpha_mask_draw_calls": report.get(
                "latest_region_peak_alpha_mask_draw_calls"),
            "last_mesh_fallback_draw_calls": latest.get(
                "previous_frame_scene_missing_mesh_draw_calls"),
            "last_missing_mesh_objects": latest.get(
                "previous_frame_scene_missing_mesh_objects"),
        },
        "caveat": "Evidence of recent OpenGL scene draws/asset residency "
                  "cannot establish avatar appearance, movement, "
                  "visual correctness, teleport or EventQueue.",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("logfile", type=Path)
    parser.add_argument("--min-snapshots", type=int, default=3)
    parser.add_argument("--min-coverage", type=int, default=75)
    parser.add_argument("--max-age-seconds", type=int, default=180)
    parser.add_argument("--max-missing-mesh-draws", type=int, default=0)
    args = parser.parse_args()
    if (args.min_snapshots < 1 or not 0 <= args.min_coverage <= 100
            or not 1 <= args.max_age_seconds <= 3600
            or args.max_missing_mesh_draws < 0):
        parser.error("Invalid acceptance thresholds")
    if not args.logfile.is_file():
        parser.error("Viewer log not found")
    result = assess(read_report(args.logfile),
                    args.min_snapshots, args.min_coverage,
                    max_age_ms=args.max_age_seconds * 1000,
                    max_missing_mesh_draws=args.max_missing_mesh_draws)
    print(json.dumps(result, sort_keys=True, indent=2))
    return 0 if result["status"] == "evidence_pass" else 2


if __name__ == "__main__":
    raise SystemExit(main())
