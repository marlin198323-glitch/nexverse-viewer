#!/usr/bin/env python3
"""Fail-closed final release readiness (never turns estimates into proof)."""
import json
import sys
from pathlib import Path

root=Path(__file__).resolve().parents[1]
data=json.loads((root/"docs/PROJECT_PROGRESS.json").read_text())
areas=data.get("areas",[])
required={"connection","world","avatar","assets","ui","functions","release"}
actual={area.get("id") for area in areas}
if actual!=required or len(areas)!=len(required):
    print("RELEASE BLOCKED: missing/duplicate project scope in areas")
    sys.exit(2)
total=0.0
weighted=0.0
for area in areas:
    pct=area["completion_pct"]
    weight=area["weight_pct"]
    if isinstance(pct,bool) or isinstance(weight,bool) or not all(
        isinstance(x,(float,int)) and 0 <= x <= 100 for x in (pct,weight)):
        print("RELEASE BLOCKED: invalid area metrics")
        sys.exit(2)
    total+=weight
    weighted+=weight*pct/100
    print(f"{area['id']:>12}: {pct:.0f}% — {area['name']}")
if abs(total-100)>1e-9:
    print("RELEASE BLOCKED: invalid weights")
    sys.exit(2)
print(f"Overall weighted estimate: {weighted:.2f}%")
# Source-specific final acceptance is mandatory, not replaced by tests
# against synthetic local GPU textures or old Pi CI commits.
required_checks=(
    "authenticated_live_grid_3_body_bakes_gpu_rgb",
    "authenticated_live_grid_actual_outfit_and_cof",
    "authenticated_live_grid_server_confirmed_wasd_yaw",
    "firestorm_ui_end_to_end_actions",
    "firestorm_mesh_material_render_parity",
    "windows_macos_linux_release_builds",
    "exact_release_sha_ci_security"
)
proof=data.get("final_release_evidence",{})
missing=[name for name in required_checks if
         not isinstance(proof.get(name),dict) or
         proof[name].get("verified") is not True or
         not proof[name].get("evidence_id")]
for name in missing:
    print(f"BLOCKED (real E2E evidence missing): {name}")
if missing or weighted < 100:
    print("FINAL RELEASE: BLOCKED — never claim 100% or deploy")
    sys.exit(2)
print("FINAL RELEASE: READY — all scope and real E2E evidence confirmed")
