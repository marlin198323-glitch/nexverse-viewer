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

def stable_predecessor_released(
    target: str,
    progress: dict,
    repo: Path = ROOT,
) -> tuple[bool, str]:
    """Fail closed: a hotfix may only follow a published stable main release.

    Both an annotated tag on origin/main and a recorded PUBLIC GitHub
    release are required. A local/unsigned tag or the current branch's
    VERSION text does not establish that v1.0.0.0 shipped.
    """
    if target != "v1.0.0.1":
        return True, "No first-hotfix dependency"
    stable = "v1.0.0.0"
    def git(*args: str) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            ["git", "-C", str(repo), *args],
            capture_output=True, text=True, check=False, timeout=15,
        )
    # An annotated tag, not a lightweight or unpushed local label.
    tagged = git("rev-parse", "--verify", f"refs/tags/{stable}^{{tag}}")
    if tagged.returncode:
        return False, f"missing annotated predecessor tag {stable}"
    base = git("rev-parse", "--verify", f"refs/tags/{stable}^{{commit}}")
    if base.returncode:
        return False, f"missing predecessor release commit {stable}"
    sha = base.stdout.strip()
    if not re.fullmatch(r"[0-9a-f]{40}", sha):
        return False, "invalid predecessor release SHA"
    on_main = git("merge-base", "--is-ancestor", sha, "refs/remotes/origin/main")
    if on_main.returncode:
        return False, f"predecessor {stable} not merged into origin/main"
    # The v1.0 tag must actually describe the v1.0 program, not a
    # renamed v0.3 or a dangling release label.
    base_version = git("show", f"{sha}:VERSION")
    if base_version.returncode or base_version.stdout.strip() != stable:
        return False, f"predecessor {stable} does not contain VERSION={stable}"
    published = progress.get("published_releases", {}).get(stable)
    if not isinstance(published, dict) or published.get("published") is not True:
        return False, f"missing verified publication receipt for {stable}"
    if published.get("release_commit_sha") != sha:
        return False, f"published predecessor SHA mismatch for {stable}"
    # A local tag can be fabricated independently of GitHub. Require
    # the exact annotated tag object to exist on the origin remote too.
    remote_tag = git("ls-remote", "--tags", "origin", f"refs/tags/{stable}")
    remote_lines = remote_tag.stdout.strip().splitlines()
    if (remote_tag.returncode or len(remote_lines)!=1
            or remote_lines[0].split() != [tagged.stdout.strip(),
                                         f"refs/tags/{stable}"]):
        return False, f"published predecessor {stable} tag is absent/mismatched on origin"
    expected_url = (
        "https://github.com/marlin198323-glitch/nexverse-viewer/"
        f"releases/tag/{stable}"
    )
    if published.get("release_url") != expected_url:
        return False, f"missing official GitHub Release URL for {stable}"
    # The receipt is not an external GitHub API attestation; perform
    # manual/web verification of the URL before actual publishing.
    return True, f"local annotated {stable} tag, main ancestry and publication receipt OK"


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
        # For hotfixes the predecessor must be an actual shipped
        # release, even if someone marks the roadmap JSON as FERTIG.
        progress=json.loads((ROOT/"docs/PROJECT_PROGRESS.json").read_text(
            encoding="utf-8"))
        prior_ok, prior_detail=stable_predecessor_released(target, progress)
        if not prior_ok:
            print(f"RELEASE BLOCKED: {prior_detail}")
            return 2
        if target=="v1.0.0.1":
            print("PREDECESSOR PREREQUISITE:",prior_detail)
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
