#!/usr/bin/env python3
"""Consistent vMAJOR.MINOR.PATCH.REVISION and fail-closed /goal release target checks."""
from __future__ import annotations
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
PATTERN=re.compile(r"^v(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)$")
def valid(s: str) -> bool:
    return bool(PATTERN.fullmatch(s))

def policy():
    data=json.loads((ROOT/"docs/RELEASE_ROADMAP.json").read_text(encoding="utf-8"))
    v=(ROOT/"VERSION").read_text(encoding="utf-8").strip()
    progress=json.loads((ROOT/"docs/PROJECT_PROGRESS.json").read_text(encoding="utf-8"))
    if not valid(v) or data.get("current_version")!=v or progress.get("viewer_version")!=v:
        raise ValueError("VERSION, project progress and roadmap must contain identical vX.X.X.X")
    stages=data.get("milestones",[])
    names=[x.get("version") for x in stages]
    if len(names)!=len(set(names)) or len(names)<3 or any(not valid(x) for x in names):
        raise ValueError("invalid/duplicate 4-part roadmap milestones")
    nums=[tuple(map(int,n[1:].split("."))) for n in names]
    if nums!=sorted(nums):
        raise ValueError("roadmap versions must be strictly ascending")
    if v not in names or data.get("first_stable_release")!="v1.0.0.0" or data.get("after_first_release")!="v1.0.0.1":
        raise ValueError("missing expected current/1.0.0.0/1.0.0.1 milestones")
    if data.get("merge_policy",{}).get("release_target_branch")!="main":
        raise ValueError("release target must be main")
    for x in stages:
        if not x.get("tasks") or not x.get("acceptance"):
            raise ValueError(f"missing tasks/criteria in {x['version']}")
        if any(item.get("status") not in ("OFFEN","IN_ARBEIT","IMPLEMENTIERT","GETESTET","FERTIG","BLOCKIERT") for item in x["tasks"]+x["acceptance"]):
            raise ValueError(f"invalid task/criteria status {x['version']}")
        if any(y not in names for y in x.get("depends_on",[])):
            raise ValueError(f"unknown dependency {x['version']}")
    cmake=(ROOT/"CMakeLists.txt").read_text(encoding="utf-8")
    if "NEXVERSE_VERSION_NUMERIC" not in cmake or 'project(NexVerseViewer VERSION' not in cmake:
        raise ValueError("CMake project version not derived from VERSION")
    app=(ROOT/"src/app/application.cpp").read_text(encoding="utf-8")
    if 'std::string{"v"} + std::string{nexverse::core::version()}' in app:
        raise ValueError("GUI would display a doubled v prefix")
    if v not in (ROOT/"README.md").read_text(encoding="utf-8")[:1400]:
        raise ValueError("README current version inconsistent")
    return v,data,stages

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    action=parser.add_mutually_exclusive_group()
    action.add_argument("--check",action="store_true",help="validate current version and roadmap")
    action.add_argument("--target",metavar="vX.X.X.X",help="show versioned /goal scope; makes no changes")
    action.add_argument("--release-check",metavar="vX.X.X.X",help="fail-closed readiness gate for main release")
    args=parser.parse_args()
    try:v,data,stages=policy()
    except (ValueError,OSError,KeyError,TypeError,json.JSONDecodeError) as exc:
        print(f"VERSION/ROADMAP BLOCKED: {exc}",file=sys.stderr)
        return 2
    print(f"OGL VERSION VALID: {v} (development, not automatically published)")
    if args.target or args.release_check:
        target=args.target or args.release_check
        match=next((stage for stage in stages if stage["version"]==target),None)
        if match is None:
            print(f"UNKNOWN TARGET VERSION: {target}; choose one from ROADMAP.md",file=sys.stderr)
            return 2
        print(f"GOAL {target}: {match['title']} / {match['status']}")
        print(f"Tasks: {len(match['tasks'])}; acceptance: {len(match['acceptance'])}")
        for item in match["tasks"]:
            print("  "+item["id"]+" "+item["status"]+": "+item["description"])
        if not args.release_check:return 0
        if not match.get("release_to_main"):
            print("RELEASE BLOCKED: pre-v1 development milestone cannot merge to main")
            return 2
        index=stages.index(match)
        checks=stages[:index+1]
        if v!=target or any(stage["status"]!="FERTIG" for stage in checks) or any(
                t["status"]!="FERTIG" for stage in checks for t in stage["tasks"]) or any(
                a["status"]!="GETESTET" for stage in checks for a in stage["acceptance"]):
            print("RELEASE BLOCKED: target must be active and ALL dependencies/tasks/acceptance complete")
            return 2
        result=subprocess.run([sys.executable,str(ROOT/"scripts/ogl-release-readiness.py")],
                              check=False)
        if result.returncode:
            print("RELEASE BLOCKED: seven real E2E or full project coverage not proven")
            return 2
        print("VERSION MILESTONES + E2E GATE PASS; release PR/branch protection still required")
    return 0

if __name__=="__main__":
    sys.exit(main())
