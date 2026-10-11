#!/usr/bin/env python3
"""Summarize OpenGenesisLINK viewer logs without disclosing UUIDs or CAP URLs.

Input can be a real historical viewer.log or a synthetic integration fixture.
The report contains only counts, timestamps, versionless categories and the
numeric InWorldAssetSnapshot fields; it never echoes raw log lines.
"""
from __future__ import annotations

import argparse
import json
from collections import Counter
from pathlib import Path
from typing import Any


EVENTS = {
    "mesh_failures": "GetMesh failed for ",
    "texture_failures": "GetTexture failed for ",
    "mesh_metrics": "GetMesh streaming metrics:",
    "texture_metrics": "GetTexture streaming metrics:",
    "inworld_legacy": "InWorld asset health:",
    "stale_mesh": "Discarded stale GetMesh",
    "stale_texture": "Discarded stale GetTexture",
    "eventqueue_failures": "EventQueue error",
}
FAILURE_KINDS = ("transient", "missing", "invalid", "configuration")


def read_report(path: Path) -> dict[str, Any]:
    counts: Counter[str] = Counter()
    reasons: Counter[str] = Counter()
    first_date: str | None = None
    last_date: str | None = None
    snapshots: list[dict[str, Any]] = []
    malformed = 0
    with path.open("r", encoding="utf-8", errors="replace") as stream:
        for raw in stream:
            if len(raw) > 65_536:
                malformed += 1
                continue
            date = raw[:10]
            if len(date) == 10 and date[4] == "-" and date[7] == "-":
                if first_date is None:
                    first_date = date
                last_date = date
            for category, marker in EVENTS.items():
                if marker in raw:
                    counts[category] += 1
            if "GetMesh failed for " in raw or "GetTexture failed for " in raw:
                if "failure=" in raw:
                    suffix = raw.split("failure=", 1)[1].split(";", 1)[0].strip()
                    reason = suffix if suffix in FAILURE_KINDS else "unclassified"
                elif "404" in raw:
                    reason = "legacy_http_404"
                elif "5xx" in raw or "503" in raw:
                    reason = "legacy_http_5xx"
                else:
                    reason = "legacy_other"
                reasons[reason] += 1
            marker = "InWorldAssetSnapshot "
            if marker in raw:
                counts["snapshot_lines"] += 1
                payload = raw.split(marker, 1)[1].strip()
                try:
                    data = json.loads(payload)
                except (json.JSONDecodeError, ValueError):
                    malformed += 1
                    continue
                if not isinstance(data, dict) or data.get("schema") != "ogl.asset.streaming.v1":
                    malformed += 1
                    continue
                # Explicit numeric allowlist: never output UUIDs or URLs from
                # an accidentally extended producer schema.
                fields = (
                    "region_epoch", "session_started_unix_ms",
                    "emitted_unix_ms", "sequence",
                    "textures_wanted", "textures_resident",
                    "textures_pending", "texture_coverage_pct",
                    "meshes_wanted", "meshes_resident", "meshes_pending",
                    "mesh_coverage_pct", "texture_retry_blocked",
                    "mesh_retry_blocked", "gpu_texture_bytes",
                    "texture_ram_hits", "texture_ram_misses",
                    "texture_ram_evictions", "mesh_ram_hits",
                    "mesh_ram_misses", "mesh_ram_evictions",
                    "gpu_mesh_bytes", "gpu_shared_budget_bytes",
                    "gpu_upload_cpu_samples", "gpu_upload_cpu_avg_us",
                    "gpu_upload_cpu_p95_us", "gpu_upload_cpu_max_us",
                    "gpu_upload_pacing_ms", "texture_priority_churn",
                    "mesh_priority_churn",
                    "texture_gpu_upload_failures",
                    "mesh_gpu_upload_failures",
                    "mesh_fail_missing", "mesh_fail_transient",
                    "mesh_fail_invalid", "mesh_fail_configuration",
                    "texture_fail_missing", "texture_fail_transient",
                    "texture_fail_invalid", "texture_fail_configuration",
                    "texture_gpu_staging_deferrals",
                    "texture_gpu_staging_waiting",
                    "texture_gpu_staging_released",
                    "mesh_gpu_budget_waiting",
                    "mesh_gpu_budget_released",
                    "texture_cpu_cached_region_reuse",
                    "texture_cpu_cached_lod_reuse",
                    "mesh_cpu_cached_region_reuse",
                    "mesh_cpu_cached_lod_reuse",
                    "texture_cpu_cached_region_ram",
                    "texture_cpu_cached_region_disk",
                    "texture_cpu_cached_lod_ram",
                    "texture_cpu_cached_lod_disk",
                    "mesh_cpu_cached_region_ram",
                    "mesh_cpu_cached_region_disk",
                    "mesh_cpu_cached_lod_ram",
                    "mesh_cpu_cached_lod_disk",
                    "texture_cpu_reuse_history_evictions",
                    "mesh_cpu_reuse_history_evictions",
                    "previous_frame_draw_calls",
                    "previous_frame_visible_objects",
                    "previous_frame_scene_draw_calls",
                    "previous_frame_scene_mesh_draw_calls",
                    "previous_frame_scene_prim_draw_calls",
                    "previous_frame_scene_missing_mesh_draw_calls",
                    "previous_frame_scene_missing_mesh_objects",
                    "previous_frame_scene_missing_mesh_triangles",
                )
                sanitized = {
                    key: data[key] for key in fields
                    if isinstance(data.get(key), int)
                    and not isinstance(data[key], bool)
                    and data[key] >= 0
                }
                snapshots.append(sanitized)

    # Only *consecutive*, monotonic real sample intervals from the most
    # recent session AND region count towards acceptance.
    latest_epoch_snapshots = 0
    latest = snapshots[-1] if snapshots else {}
    current=latest
    if (current.get("session_started_unix_ms",0)>0 and
            current.get("emitted_unix_ms",0)>0 and
            current.get("sequence",0)>0 and
            current.get("region_epoch",0)>0):
        latest_epoch_snapshots=1
        for prior in reversed(snapshots[:-1]):
            expected=current.get("emitted_unix_ms",0)-prior.get(
                "emitted_unix_ms",0)
            if (prior.get("session_started_unix_ms") !=
                    current.get("session_started_unix_ms") or
                prior.get("region_epoch")!=current.get("region_epoch") or
                prior.get("sequence",0)+1!=current.get("sequence") or
                not 15_000<=expected<=90_000):
                break
            latest_epoch_snapshots+=1
            current=prior

    region_window = snapshots[-latest_epoch_snapshots:] if latest_epoch_snapshots else []
    def window_min(key):
        observed=[sample[key] for sample in region_window if key in sample]
        return min(observed) if len(observed)==len(region_window) and observed else None
    def window_max(key):
        observed=[sample[key] for sample in region_window if key in sample]
        return max(observed) if len(observed)==len(region_window) and observed else None

    return {
        "schema": "ogl.asset.log-report.v1",
        "latest_epoch_snapshots": latest_epoch_snapshots,
        "latest_region_min_texture_coverage_pct": window_min(
            "texture_coverage_pct"),
        "latest_region_min_mesh_coverage_pct": window_min(
            "mesh_coverage_pct"),
        "latest_region_max_missing_mesh_fallback_draws": window_max(
            "previous_frame_scene_missing_mesh_draw_calls"),
        "latest_region_min_mesh_draw_calls": window_min(
            "previous_frame_scene_mesh_draw_calls"),
        "latest_region_max_mesh_budget_waiting": window_max(
            "mesh_gpu_budget_waiting"),
        "last_snapshot_emitted_unix_ms": latest.get("emitted_unix_ms"),
        "last_snapshot_session_unix_ms": latest.get("session_started_unix_ms"),
        "date_first": first_date,
        "date_last": last_date,
        "event_counts": dict(sorted(counts.items())),
        "failure_categories": dict(sorted(reasons.items())),
        "snapshots_valid": len(snapshots),
        "snapshots_malformed": malformed,
        "latest_snapshot": snapshots[-1] if snapshots else None,
        "lowest_texture_coverage_pct": min(
            (s["texture_coverage_pct"] for s in snapshots
             if "texture_coverage_pct" in s), default=None),
        "lowest_mesh_coverage_pct": min(
            (s["mesh_coverage_pct"] for s in snapshots
             if "mesh_coverage_pct" in s), default=None),
        "peak_gpu_upload_cpu_p95_us": max(
            (s["gpu_upload_cpu_p95_us"] for s in snapshots
             if "gpu_upload_cpu_p95_us" in s), default=None),
        "peak_texture_priority_churn": max(
            (s["texture_priority_churn"] for s in snapshots
             if "texture_priority_churn" in s), default=None),
        "peak_mesh_priority_churn": max(
            (s["mesh_priority_churn"] for s in snapshots
             if "mesh_priority_churn" in s), default=None),
        "max_texture_gpu_upload_failures": max(
            (s["texture_gpu_upload_failures"] for s in snapshots
             if "texture_gpu_upload_failures" in s), default=None),
        "max_mesh_gpu_upload_failures": max(
            (s["mesh_gpu_upload_failures"] for s in snapshots
             if "mesh_gpu_upload_failures" in s), default=None),
        "max_texture_gpu_staging_waiting": max(
            (s["texture_gpu_staging_waiting"] for s in snapshots
             if "texture_gpu_staging_waiting" in s), default=None),
        "max_texture_gpu_staging_released": max(
            (s["texture_gpu_staging_released"] for s in snapshots
             if "texture_gpu_staging_released" in s), default=None),
        "max_mesh_gpu_budget_waiting": max(
            (s["mesh_gpu_budget_waiting"] for s in snapshots
             if "mesh_gpu_budget_waiting" in s), default=None),
        "peak_texture_ram_evictions": max(
            (s["texture_ram_evictions"] for s in snapshots
             if "texture_ram_evictions" in s), default=None),
        "max_mesh_missing_events": max(
            (s["mesh_fail_missing"] for s in snapshots
             if "mesh_fail_missing" in s), default=None),
        "max_mesh_transient_events": max(
            (s["mesh_fail_transient"] for s in snapshots
             if "mesh_fail_transient" in s), default=None),
        "peak_mesh_ram_evictions": max(
            (s["mesh_ram_evictions"] for s in snapshots
             if "mesh_ram_evictions" in s), default=None),
        "max_mesh_gpu_budget_released": max(
            (s["mesh_gpu_budget_released"] for s in snapshots
             if "mesh_gpu_budget_released" in s), default=None),
        "peak_missing_mesh_fallback_draws": max(
            (s["previous_frame_scene_missing_mesh_draw_calls"]
             for s in snapshots
             if "previous_frame_scene_missing_mesh_draw_calls" in s),
            default=None),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("logfile", type=Path, help="Path to viewer.log")
    parser.add_argument("--compact", action="store_true", help="Single-line JSON")
    args = parser.parse_args()
    if not args.logfile.is_file():
        parser.error("logfile does not exist")
    report = read_report(args.logfile)
    print(json.dumps(report, ensure_ascii=False, indent=None if args.compact else 2,
                     sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
