#!/usr/bin/env python3
"""Offline OGL acceptance dashboard and untrusted manual-review report validator.

A review is NEVER a release-authority or a substitute for the real Grid tests.
"""
from __future__ import annotations
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
TEMPLATE=ROOT/"tools/acceptance/dashboard.template.html"
OUTPUT=ROOT/"build/acceptance/ABNAHME_DASHBOARD.html"
GUIDED_SPEC=ROOT/"docs/ACCEPTANCE_GUIDED_V0400.json"
GUIDED_STATUSES={"FUNKTIONIERT","FEHLER","NICHT_MOEGLICH","NICHT_GETESTET"}
RELEASE_CHECKS=(
    ("authenticated_live_grid_3_body_bakes_gpu_rgb","Live-Grid Skin: echte Head/Upper/Lower RGB-Bakes 3/3"),
    ("authenticated_live_grid_actual_outfit_and_cof","Live-Grid Outfit/CurrentOutfitFolder"),
    ("authenticated_live_grid_server_confirmed_wasd_yaw","WASD/Yaw mit Simulatorpositions-Readback"),
    ("firestorm_ui_end_to_end_actions","Firestorm-Menues/Fenster: echte E2E-Backendaktionen"),
    ("firestorm_mesh_material_render_parity","Mesh/Material/LOD/Render-Paritaet"),
    ("windows_macos_linux_release_builds","Linux/Windows/macOS Installations- und Laufzeittests"),
    ("exact_release_sha_ci_security","Exakte Release-SHA: CI/Security/Signaturen"),
)
ALLOWED_STATUSES={"OFFEN","IN_ARBEIT","BLOCKIERT","BESTANDEN"}

def snapshot() -> dict:
    roadmap=json.loads((ROOT/"docs/RELEASE_ROADMAP.json").read_text(encoding="utf-8"))
    progress=json.loads((ROOT/"docs/PROJECT_PROGRESS.json").read_text(encoding="utf-8"))
    version=(ROOT/"VERSION").read_text(encoding="utf-8").strip()
    sha=subprocess.check_output(
        ["git","-C",str(ROOT),"rev-parse","HEAD"],text=True).strip()
    if len(sha)!=40 or not re.fullmatch(r"[0-9a-f]{40}",sha):
        raise ValueError("invalid project git SHA")
    if roadmap.get("current_version")!=version or progress.get("viewer_version")!=version:
        raise ValueError("roadmap/progress/VERSION mismatch")
    guided=json.loads(GUIDED_SPEC.read_text(encoding="utf-8"))
    if guided.get("version")!=version:
        raise ValueError("guided acceptance version does not match VERSION")
    expected_ids={entry["id"] for stage in roadmap["milestones"]
        for kind in ("tasks","acceptance") for entry in stage[kind]}
    seen=set()
    for step in guided["steps"]:
        if (step["id"] in seen
            or step.get("related_roadmap_id") not in expected_ids
            or not isinstance(step.get("instructions"),list)
            or not step["instructions"]
            or not isinstance(step.get("expected"),str)):
            raise ValueError("invalid guided acceptance mapping")
        seen.add(step["id"])
    return {
        "schema_version":1,"project":"OpenGenesisLINK Viewer",
        "guided":guided,
        "version":version,"snapshot_sha":sha,
        "weighted_pct":round(sum(
            a["weight_pct"]*a["completion_pct"]/100 for a in progress["areas"]),2),
        "milestones":roadmap["milestones"],
        "release_evidence":[
            {"id":key,"title":title,"verified":(
                isinstance(progress.get("final_release_evidence",{}).get(key),dict)
                and progress["final_release_evidence"][key].get("verified") is True
                and bool(progress["final_release_evidence"][key].get("evidence_id"))
            )}
            for key,title in RELEASE_CHECKS],
        "published_release":False,
        "release_eligible":False,
        "release_note":"Entwurf – niemals automatische Freigabe. Das offizielle Release-Gate ist allein massgeblich.",
    }

def export_html() -> Path:
    data=snapshot()
    template=TEMPLATE.read_text(encoding="utf-8")
    if template.count("/*__OGL_ACCEPTANCE_SNAPSHOT__*/")!=1:
        raise ValueError("dashboard snapshot placeholder missing/duplicated")
    # Prevent HTML/script breakout from any untrusted milestone wording.
    js=json.dumps(data,ensure_ascii=False,separators=(",",":")).replace("<",r"\u003c").replace(">",r"\u003e")
    html=template.replace("/*__OGL_ACCEPTANCE_SNAPSHOT__*/","const SNAPSHOT = "+js+";")
    OUTPUT.parent.mkdir(parents=True,exist_ok=True)
    OUTPUT.write_text(html,encoding="utf-8")
    print("OGL_ACCEPTANCE_DASHBOARD_GENERATED",OUTPUT)
    print("Acceptance snapshot",data["version"],data["snapshot_sha"][:12],
          "stages",len(data["milestones"]),"checks",len(data["release_evidence"]))
    return OUTPUT

def validate_manual(path: Path) -> int:
    try:
        report=json.loads(path.read_text(encoding="utf-8"))
        snap=snapshot()
        if not isinstance(report,dict) or report.get("schema_version")!=1:
            raise ValueError("invalid schema")
        if report.get("project")!=snap["project"] or report.get("version")!=snap["version"]:
            raise ValueError("foreign project/version")
        if report.get("snapshot_sha")!=snap["snapshot_sha"]:
            raise ValueError("stale snapshot SHA: re-open newly built dashboard")
        if report.get("release_authorized") is not False or report.get("local_review_only") is not True:
            raise ValueError("manual review cannot authorize production release")
        reviews=report.get("reviews")
        if not isinstance(reviews,dict):
            raise ValueError("reviews is not an object")
        ids={x["id"] for stage in snap["milestones"]
             for group in ("tasks","acceptance") for x in stage[group]}
        ids.update(x["id"] for x in snap["release_evidence"])
        if len(reviews)>len(ids):raise ValueError("too many reviews")
        passed=blocked=0
        for ident,item in reviews.items():
            if ident not in ids or not isinstance(item,dict):
                raise ValueError("unknown review ID")
            state=item.get("status")
            if state not in ALLOWED_STATUSES:
                raise ValueError("illegal review status")
            for field in ("evidence","notes","reviewer","tested_at"):
                if not isinstance(item.get(field,""),str) or len(item.get(field,""))>4096:
                    raise ValueError("invalid review text")
            if state=="BESTANDEN":
                if len(item.get("evidence","").strip())<10 or not item.get("tested_at"):
                    raise ValueError("BESTANDEN requires concrete evidence and test timestamp")
                passed+=1
            if state=="BLOCKIERT":blocked+=1
        results=report.get("guided_results",{})
        if not isinstance(results,dict):
            raise ValueError("guided_results is not an object")
        guided_ids={step["id"] for step in snap["guided"]["steps"]}
        if len(results)>len(guided_ids):
            raise ValueError("too many guided test results")
        for ident,result in results.items():
            if ident not in guided_ids or not isinstance(result,dict):
                raise ValueError("unknown guided step")
            if result.get("status") not in GUIDED_STATUSES:
                raise ValueError("illegal guided status")
            if any(not isinstance(result.get(field,""),str)
                or len(result.get(field,""))>4096
                for field in ("notes","tested_at")):
                raise ValueError("invalid guided result text")
            if result["status"]!="NICHT_GETESTET" and not result.get("tested_at"):
                raise ValueError("guided result needs date/time")
        print(f"OGL_MANUAL_ACCEPTANCE_VALID: {len(reviews)} reviews; "
              f"{passed} user-reported passed; {blocked} blocked; "
              f"{len(results)} guided results")
        print("MANUAL REVIEWS DO NOT CHANGE ROADMAP OR RELEASE STATUS; "
              "real E2E and the official release-readiness gate remain mandatory")
        return 0
    except (OSError,ValueError,KeyError,TypeError,json.JSONDecodeError) as exc:
        print(f"OGL_MANUAL_ACCEPTANCE_REJECTED: {exc}",file=sys.stderr)
        return 2

def main() -> int:
    ap=argparse.ArgumentParser(description=__doc__)
    group=ap.add_mutually_exclusive_group(required=True)
    group.add_argument("--build",action="store_true")
    group.add_argument("--validate",type=Path,metavar="EXPORTED_JSON")
    options=ap.parse_args()
    if options.validate:return validate_manual(options.validate)
    try:export_html();return 0
    except (ValueError,OSError,subprocess.CalledProcessError) as exc:
        print("OGL_ACCEPTANCE_BUILD_FAILED:",exc,file=sys.stderr);return 2

if __name__=="__main__":sys.exit(main())
