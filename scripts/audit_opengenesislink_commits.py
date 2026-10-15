#!/usr/bin/env python3
"""Bounded, read-only per-commit OpenGenesisLINK file inventory.

The GitHub compare endpoint truncates its cumulative files array after 300
entries. Fetch each commit's files using paged /commits/{sha} responses.
A complete inventory is NOT automatically an approved viewer compatibility pin.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import sys

from check_opengenesislink_server import API, BASE, classify, retrieve

PER_PAGE=100
MAX_COMMITS=100
MAX_PAGES=30
MAX_FILES=3000

def commit_files(sha, fetch):
    files=[]
    names=set()
    for page in range(1,MAX_PAGES+1):
        data=fetch(f"{API}/commits/{sha}?per_page={PER_PAGE}&page={page}")
        if data.get("sha") != sha or not isinstance(data.get("files"),list):
            raise ValueError("Missing or mismatched paged commit file list")
        batch=data["files"]
        if len(batch)>PER_PAGE:raise ValueError("Oversized GitHub commit page")
        for entry in batch:
            name=entry.get("filename")
            if not isinstance(name,str) or not name or name in names:
                raise ValueError("Duplicate/invalid commit filename")
            names.add(name)
            files.append(name)
        if len(files)>MAX_FILES:raise ValueError("Overlarge commit; manual review")
        if len(batch)<PER_PAGE:return files
    raise ValueError("Commit pagination limit reached: incomplete review")

def inventory(old,head,compare,fetch):
    if not (isinstance(old,str) and isinstance(head,str)
            and len(old)==40 and len(head)==40):
        raise ValueError("Invalid pinned or head SHA")
    commits=compare.get("commits")
    count=compare.get("ahead_by")
    if not isinstance(commits,list) or not isinstance(count,int) or count<1:
        raise ValueError("Missing commit comparison")
    if count>MAX_COMMITS or len(commits)!=count:
        raise ValueError("Compare commits truncated: separate smaller ranges")
    if commits[-1].get("sha")!=head:
        raise ValueError("Compare head not equal to actual upstream")
    rows=[]
    categories=Counter()
    seen=set()
    for entry in commits:
        sha=entry.get("sha")
        if not isinstance(sha,str) or len(sha)!=40 or sha in seen:
            raise ValueError("Missing/duplicate commit SHA")
        seen.add(sha)
        files=commit_files(sha,fetch)
        category_counts=Counter(classify(f) for f in files)
        categories.update(category_counts)
        rows.append({"sha":sha,"files":len(files),
                     "categories":dict(sorted(category_counts.items()))})
    return {"repository":"opengenesislink/opengenesislinkserver",
            "old_reviewed_sha":old,"current_sha":head,
            "commits_inventoried":len(rows),
            "changed_file_entries":sum(row["files"] for row in rows),
            "classifications":dict(sorted(categories.items())),
            "commits":rows,
            "compatibility_approved":False,
            "notes":"Complete file enumeration is not human patch review; "
                    "do not advance pinned reviewed_sha automatically."}

def run(argv=None):
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--baseline",type=Path,default=BASE)
    ap.add_argument("--output",type=Path)
    ap.add_argument("--fixture",type=Path,
                    help="offline fixture containing compare and paged commits")
    args=ap.parse_args(argv)
    try:
        old=json.loads(args.baseline.read_text())["reviewed_sha"]
        if args.fixture:
            fixture=json.loads(args.fixture.read_text())
            head=fixture["head"]
            compare=fixture["compare"]
            by_url=fixture["pages"]
            fetch=lambda url:by_url[url]
            print("OFFLINE FIXTURE; not current server evidence")
        else:
            head=retrieve(API+"/branches/main")["commit"]["sha"]
            if head==old:
                print("Already pinned and reviewed: no new commits")
                return 0
            compare=retrieve(API+"/compare/"+old+"..."+head)
            fetch=retrieve
        result=inventory(old,head,compare,fetch)
        text=json.dumps(result,ensure_ascii=False,indent=2)+"\n"
        if args.output:
            if not args.output.parent.is_dir():
                raise ValueError("Output directory does not exist")
            args.output.write_text(text)
        print("READ_ONLY_INVENTORY",result["commits_inventoried"],"commits",
              result["changed_file_entries"],"file entries",
              "unreviewed_manual_categories",
              result["classifications"].get("MANUAL_REVIEW",0),
              "COMPATIBILITY_NOT_APPROVED")
        print("No pin update, no server modification")
        return 0
    except (OSError,KeyError,ValueError,TypeError,json.JSONDecodeError) as error:
        print("INCOMPLETE UPSTREAM INVENTORY:",error,file=sys.stderr)
        return 2

if __name__=="__main__":
    sys.exit(run())
