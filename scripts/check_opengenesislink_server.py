#!/usr/bin/env python3
"""Review OpenGenesisLINK main before/after each OGL development block."""
import argparse
import json
import os
from pathlib import Path
import sys
import urllib.request
from urllib.error import URLError

ROOT=Path(__file__).resolve().parent.parent
REPO="opengenesislink/opengenesislinkserver"
API="https://api.github.com/repos/"+REPO
BASE=ROOT/"docs/OPENGENESISLINK_SERVER_BASELINE.json"

def classify(path):
    x=path.lower()
    if "cross_region_chat_smoke" in x:
        return "SERVER_ONLY_LSL_CHAT_TEST"
    if "cross_region_lsl_chat" in x or "crossregionchat" in x:
        return "SERVER_ONLY_LSL_CHAT"
    if "touch_hold" in x or "touchhold" in x:
        return "VIEWER_CORE_GRAB_COMPAT"
    if "region_dimensions" in x or "regiondimensions" in x:
        return "VIEWER_CORE_LSL_EDITOR_FUTURE"
    if "thumbnail" in x or ("inventory" in x and "connector" in x):
        return "VIEWER_CORE_INVENTORY_THUMBNAIL"
    if "scene.packethandlers.cs" in x or "/capabilities/" in x:
        return "VIEWER_CORE_PROTOCOL_REVIEW"
    if "/archives/" in x or "iar" in x:
        return "SERVER_ONLY_ARCHIVE"
    if "/nodeagent/" in x or "nexbus" in x:
        return "SERVER_ONLY_NODE_BUS"
    if "/lsl_api" in x or "/lsl_stub" in x or "/ilsl_api" in x:
        return "SERVER_ONLY_LSL_RUNTIME"
    if "/chat/" in x or "worldcomm" in x:
        return "SERVER_ONLY_CHAT_DISPATCH"
    if x.startswith(".github/") or x.startswith("tools/ci/"):
        return "SERVER_ONLY_CI"
    if x.startswith("doc/") or x.startswith("tools/lsl/"):
        return "REFERENCE_ONLY"
    return "MANUAL_REVIEW"

def retrieve(url):
    headers={"Accept":"application/vnd.github+json",
             "User-Agent":"OGL-Viewer-Upstream-Compat",
             "X-GitHub-Api-Version":"2022-11-28"}
    token=os.environ.get("GH_TOKEN") or os.environ.get("GITHUB_TOKEN")
    if token: headers["Authorization"]="Bearer "+token
    with urllib.request.urlopen(urllib.request.Request(url,headers=headers),timeout=12) as resp:
        return json.load(resp)

def main(argv=None):
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--baseline",type=Path,default=BASE)
    ap.add_argument("--fixture",type=Path,help="Offline fixture; NEVER live evidence")
    ap.add_argument("--strict",action="store_true",
                    help="Exit 2 for unreviewed upstream commits")
    options=ap.parse_args(argv)
    try:
        baseline=json.loads(options.baseline.read_text(encoding="utf-8"))
        old=baseline["reviewed_sha"]
        if len(old)!=40: raise ValueError("Invalid reviewed SHA")
        if options.fixture:
            print("OFFLINE FIXTURE: no live GitHub request!")
            comp=json.loads(options.fixture.read_text(encoding="utf-8"))
            head=comp["head"]
        else:
            upstream=retrieve(API+"/branches/main")
            head=upstream["commit"]["sha"]
            if head==old:
                print("SERVER CURRENT HEAD "+head+" (reviewed, no new commits)")
                return 0
            comp=retrieve(API+"/compare/"+old+"..."+head)
        if len(head)!=40: raise ValueError("Invalid current SHA")
        print("UPSTREAM",REPO,"reviewed",old,"current",head)
        print("GitHub upstream commits since review:",comp.get("ahead_by","?"))
        for change in comp.get("files",[]):
            path=change.get("filename")
            if path: print("  ["+classify(path)+"]",path)
        if "files" not in comp:
            raise ValueError("Comparison lacks file list; cannot claim reviewed")
        if head!=old:
            print("REVIEW REQUIRED: do not advance baseline without manual integration")
            return 2 if options.strict else 0
        return 0
    except (OSError, URLError, ValueError, KeyError,
            TimeoutError, json.JSONDecodeError) as error:
        print("UPSTREAM CHECK FAILED:",error,file=sys.stderr)
        return 3

if __name__=="__main__":
    sys.exit(main())
